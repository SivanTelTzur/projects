#ifndef E70A4793_E41F_4793_B5A9_9C8BD9466B0E_H__
#define E70A4793_E41F_4793_B5A9_9C8BD9466B0E_H__

#include "functor.hpp"
#include "waiting_queue.hpp"
#include "cdr_struct.hpp"

#include <string>

namespace advcpp
{
class NotifierThreadFunc : public ObjThreadFunc{
public:
    NotifierThreadFunc(MtWaitingQueue<std::string>& a_filesQueue, const std::string& a_dirName);
    ~NotifierThreadFunc();

    virtual void operator()();
    virtual ObjThreadFunc* Clone() const;

private:
    MtWaitingQueue<std::string>& m_filesQueue;
    const std::string m_dirName;
};


class ReaderThreadFunc : public ObjThreadFunc{
public:
    ReaderThreadFunc(MtWaitingQueue<Cdr>& a_cdrQueue, const std::string& a_source);
    ~ReaderThreadFunc();

    virtual void operator()();
    virtual ObjThreadFunc* Clone() const;

private:
    MtWaitingQueue<Cdr>& m_cdrs;
    const std::string m_fileName;
};

class SenderThreadFunc : public ObjThreadFunc{
public:
    SenderThreadFunc(MtWaitingQueue<Cdr>& a_cdrQueue, const std::string& a_ip, size_t a_port);
    ~SenderThreadFunc();

    virtual void operator()();
    virtual ObjThreadFunc* Clone() const;

private:
    MtWaitingQueue<Cdr>& m_cdrsQueue;
    const std::string m_ip;
    const size_t m_port;
};

} // namespace advcpp

#endif // E70A4793_E41F_4793_B5A9_9C8BD9466B0E_H__
