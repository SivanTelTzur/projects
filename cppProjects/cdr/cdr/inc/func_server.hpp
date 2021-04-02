#ifndef L931F5F0_FEEE_4A57_BB41_AC62A691F4A2_H__
#define L931F5F0_FEEE_4A57_BB41_AC62A691F4A2_H__

#include "functor.hpp"
#include "waiting_queue.hpp"
#include "cdr_struct.hpp"
#include "inet_server.hpp"

#include <string>

namespace advcpp
{
class ServerTcpFunc : public ObjThreadFunc{
public:
    ServerTcpFunc(size_t a_port, MtWaitingQueue<std::string>& a_msgQueue);
    ~ServerTcpFunc();

    virtual void operator()();
    virtual ObjThreadFunc* Clone() const;

private:
    MtWaitingQueue<std::string>& m_msgQueue;
    const size_t m_port;
};


class ServerRcvFunc : public ObjThreadFunc{
public:
    ServerRcvFunc(INetServer& a_server);
    ~ServerRcvFunc();

    virtual void operator()();
    virtual ObjThreadFunc* Clone() const;

private:
    INetServer& m_server;
};

class ReceiverFunc : public ObjThreadFunc{
public:
    ReceiverFunc(size_t a_port, MtWaitingQueue<Cdr>& a_cdrsQueue);
    ~ReceiverFunc();

    virtual void operator()();
    virtual ObjThreadFunc* Clone() const;

private:
    MtWaitingQueue<Cdr>& m_cdrsQueue;
    const size_t m_port;
};

} // namespace advcpp

#endif // L931F5F0_FEEE_4A57_BB41_AC62A691F4A2_H__
