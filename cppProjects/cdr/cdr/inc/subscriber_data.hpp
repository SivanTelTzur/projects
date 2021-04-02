#ifndef P301E107_84F2_4656_85E1_D476928F1A8C_H__
#define P301E107_84F2_4656_85E1_D476928F1A8C_H__

#include "mutex.hpp"
#include "condvar.hpp"
#include "noncopyable.hpp"
#include "cdr_subscriber.hpp"
#include "cpp_sql.hpp"

#include <map>
#include <string>


namespace advcpp
{

class SubscriberData : private NonCopyable{
public:
    //static const size_t MIN_SIZE_QUEUE = 10;
    explicit SubscriberData(size_t a_maxNOfElemets = 0);
    ~SubscriberData();

    void InsertData(const Subscriber& a_newVal);
    Subscriber GetValue(const std::string& a_subscriber) ;
    //void Update(const Key a_key, const Value& a_updateFromVal);

    bool IsEmpty() const;

private:
    bool isEmptyNoGuard() const;
    bool isFullNoGuard() const;
    void mutexLockAndCheck() ;
    void mutexUnLockAndCheck();
    void insertQueryToDb(const Subscriber& a_subscriber);
    void prepareUpdateQuery(const Subscriber& a_subscriber);
    bool existKeyInDb(const std::string& a_key);


private:
    std::map<std::string, Subscriber> m_map;
    ConditionVar m_fullCondVar;
    ConditionVar m_emptyCondVar;
    mutable Mutex m_actionLock;
    size_t m_capacity;
    CdrsDataBase m_db; //change to reference when added carrier
};

} // namespace advcpp

#endif // P301E107_84F2_4656_85E1_D476928F1A8C_H__
