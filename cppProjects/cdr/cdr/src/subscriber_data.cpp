#include "subscriber_data.hpp"
#include "exceptions_calsses.hpp"
#include "cdr_converter.hpp"
#include "logger.hpp"

#include <exception>
#include <assert.h>
#include <sstream> //ostringstream

namespace advcpp
{
    
SubscriberData::SubscriberData(size_t a_maxNOfElemets) 
: m_map()
, m_fullCondVar()
, m_emptyCondVar()
, m_actionLock()
, m_capacity(a_maxNOfElemets)
, m_db("localhost", "sivan", "123456")
{
    
}

SubscriberData::~SubscriberData()
{
    
}

void SubscriberData::InsertData(const Subscriber& a_newVal)
{
    bool actionResult = false; // false update true new subscriber
    mutexLockAndCheck();
    while (isFullNoGuard()){
        m_fullCondVar.Wait(m_actionLock);
    }   
    try{
        std::pair<std::map<std::string, Subscriber>::iterator,bool> ret;
        ret = m_map.insert( std::pair<std::string, Subscriber>(a_newVal.msisdn, a_newVal));
        actionResult = ret.second;
        if (ret.second == false) {
            (ret.first->second).AccumulateData(a_newVal);
        }
    }
    catch (const std::bad_alloc & a_ex){
        m_actionLock.Unlock();
        StartLogger().Exception(a_ex);//assert(!"failed allocated");
        StartLogger().Message("insert subscriber failed");//assert(!"failed allocated");
    } 
    catch (const WrongSubscriberException& a_ex){
        m_actionLock.Unlock();
        StartLogger().Exception(a_ex);//assert(!"failed allocated");
        StartLogger().Message("Insert subscriber");//assert(!"failed allocated");
    }
    mutexUnLockAndCheck();
    m_emptyCondVar.Signal();

    //insert data to sql
    if (actionResult == false){
        prepareUpdateQuery(a_newVal);
    }else if (!existKeyInDb(a_newVal.msisdn)){
        insertQueryToDb(a_newVal);
    }
}


Subscriber SubscriberData::GetValue(const std::string& a_subscriber) 
{
    mutexLockAndCheck();
    while (isEmptyNoGuard()) {
       m_emptyCondVar.Wait(m_actionLock);
    }   
    Subscriber subscriber;
    try{
        subscriber = m_map.at(a_subscriber);
    } catch(const std::out_of_range& e_ax){
        mutexUnLockAndCheck();
        subscriber.msisdn = "Not Found";
        return subscriber;
    }
    
    mutexUnLockAndCheck();
    m_fullCondVar.Signal();
    return subscriber;
}


bool SubscriberData::IsEmpty() const
{
    m_actionLock.Lock();
    bool result = isEmptyNoGuard();
    m_actionLock.Unlock();
    return result;    
}

bool SubscriberData::isEmptyNoGuard() const
{
    return m_map.empty();     
}

bool SubscriberData::isFullNoGuard() const
{
    return (m_capacity && m_map.size() == m_capacity );    
}

void SubscriberData::mutexLockAndCheck() 
{
    try{
        m_actionLock.Lock();
    }
    catch (const MutexAlreadyLocked& a_ex){
        assert(!a_ex.what());
    }    
}

void SubscriberData::mutexUnLockAndCheck() 
{
    try{
        m_actionLock.Unlock();
    }
    catch (const MtxDontBelong & a_mtx){
        assert(!a_mtx.what());
    }    
}

void SubscriberData::insertQueryToDb(const Subscriber& a_subscriber) 
{
    std::ostringstream osQuery;
    osQuery << "INSERT INTO cdrData.subscribers (msisdn, imsi, voice_in, voice_out, data_in, data_out, sms_in, sms_out) ";
    osQuery << "Values ('" << a_subscriber.msisdn.c_str() << "', '" << a_subscriber.imsi.c_str() << "', "<< a_subscriber.voiceIn << ", ";
    osQuery << a_subscriber.voiceOut << ", " << a_subscriber.dataIn << ", " << a_subscriber.dataOut << ", ";
    osQuery << a_subscriber.smsIn << ", " << a_subscriber.smsOut << ");";
    try{
         m_db.QueryExecute(osQuery.str());
    }
    catch (const std::exception& a_ex){
        StartLogger().Exception(a_ex);
    }

}

void SubscriberData::prepareUpdateQuery(const Subscriber& a_subscriber) 
{

    std::ostringstream osQuery; 
    osQuery << "UPDATE cdrData.subscribers SET voice_in = voice_in + " << a_subscriber.voiceIn;
    osQuery << ", voice_out = voice_out + " << a_subscriber.voiceOut <<  ", data_in = data_in + " << a_subscriber.dataIn;
    osQuery << ", data_out = data_out + " << a_subscriber.dataOut <<  ", sms_in = sms_in +" << a_subscriber.smsIn << ", sms_out = sms_out +" << a_subscriber.smsOut;
    osQuery << " WHERE msisdn = '" <<  a_subscriber.msisdn.c_str() << "';"  ;
    
    try{
         m_db.QueryExecute(osQuery.str());
    }
    catch (const std::exception& a_ex){
        StartLogger().Exception(a_ex);
    }
}

bool SubscriberData::existKeyInDb(const std::string& a_key) 
{
    std::ostringstream osQuery;
    osQuery << "SELECT msisdn FROM cdrData.subscribers WHERE msisdn= '" << a_key.c_str() << "';";
    bool res;
    try{
        res =  m_db.IsExist(osQuery.str());
    }
    catch (const std::exception& a_ex){
        StartLogger().Exception(a_ex);
    }    
    return res;
}


} // namespace advcpp


