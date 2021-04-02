#include "cdr_functors.hpp"
#include "files_notifier.hpp"
#include "logger.hpp"
#include "reader.hpp"
#include "cdr_sender.hpp"

namespace advcpp
{

NotifierThreadFunc::NotifierThreadFunc(MtWaitingQueue<std::string>& a_filesQueue, const std::string& a_dirName)
: m_filesQueue(a_filesQueue)
, m_dirName(a_dirName)
{
}

NotifierThreadFunc::~NotifierThreadFunc()
{
}

void NotifierThreadFunc::operator()()
{
    try{
        FilesNotifier fileNotifier(m_filesQueue, m_dirName);
        fileNotifier.Run();
    }catch(const std::exception& a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}

ObjThreadFunc* NotifierThreadFunc::Clone() const
{
    try{
        ObjThreadFunc* copy = new NotifierThreadFunc(m_filesQueue, m_dirName);
        return copy;
    }
    catch (std::bad_alloc & a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}


ReaderThreadFunc::ReaderThreadFunc(MtWaitingQueue<Cdr>& a_cdrQueue, const std::string& a_source)
: m_cdrs(a_cdrQueue)
, m_fileName(a_source)
{

}

ReaderThreadFunc::~ReaderThreadFunc()
{
}

void ReaderThreadFunc::operator()()
{
    try{
        Reader reader(m_cdrs, m_fileName);
        reader.Start();
    }catch(const std::exception& a_ex){
        StartLogger().Exception(a_ex);
    }
}

ObjThreadFunc* ReaderThreadFunc::Clone() const
{
    try{
        ObjThreadFunc* copy = new ReaderThreadFunc(m_cdrs, m_fileName);
        return copy;
    }
    catch (std::bad_alloc & a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}

SenderThreadFunc::SenderThreadFunc(MtWaitingQueue<Cdr>& a_cdrQueue, const std::string& a_ip, size_t a_port) 
: m_cdrsQueue(a_cdrQueue)
, m_ip(a_ip)
, m_port(a_port)
{
    
}

SenderThreadFunc::~SenderThreadFunc()
{
}

void SenderThreadFunc::operator()()
{
    try{
        CdrSender sender(m_cdrsQueue, m_ip, m_port);
        sender.Start();
    }catch(const std::exception& a_ex){
        StartLogger().Exception(a_ex);
    }
}

ObjThreadFunc* SenderThreadFunc::Clone() const
{
    try{
        ObjThreadFunc* copy = new SenderThreadFunc(m_cdrsQueue, m_ip, m_port);
        return copy;
    }
    catch (std::bad_alloc & a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}

} // namespace advcpp
