#ifndef EA5AF743_26FA_4A7A_9AB0_DEE405AE6049_H__
#define EA5AF743_26FA_4A7A_9AB0_DEE405AE6049_H__

#include "functor.hpp"
#include "sharedptr.hpp"
#include "inet_server.hpp"
#include "logger.hpp"
#include "serverHandler.hpp"


namespace advcpp
{

class ServerRecvFunc : public ObjThreadFunc{
public:
    ServerRecvFunc(SharedPtr<INetServer> a_server);
    ~ServerRecvFunc();

    virtual void operator()();
    virtual ObjThreadFunc* Clone() const;

private:
    SharedPtr<INetServer> m_server;
};


class ServerRecvFuncWithHandler : public ObjThreadFunc{
public:
    ServerRecvFuncWithHandler(ServerHandler& a_server);
    ~ServerRecvFuncWithHandler();

    virtual void operator()();
    virtual ObjThreadFunc* Clone() const;

private:
    ServerHandler& m_server;
};

inline ServerRecvFunc::ServerRecvFunc(SharedPtr<INetServer> a_server)
: m_server(a_server)
{
}

inline ServerRecvFunc::~ServerRecvFunc()
{
}

inline void ServerRecvFunc::operator()()
{
    try{
        m_server->Run();

    }catch(const std::exception& a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}

inline ObjThreadFunc* ServerRecvFunc::Clone() const
{
    try{
        ServerRecvFunc* copy = new ServerRecvFunc(m_server);
        return copy;
    }
    catch (std::bad_alloc & a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}

inline ServerRecvFuncWithHandler::ServerRecvFuncWithHandler(ServerHandler& a_server)
: m_server(a_server)
{
}

inline ServerRecvFuncWithHandler::~ServerRecvFuncWithHandler()
{
}

inline void ServerRecvFuncWithHandler::operator()()
{
    try{
        m_server.Run();

    }catch(const std::exception& a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}

inline ObjThreadFunc* ServerRecvFuncWithHandler::Clone() const
{
    try{
        ServerRecvFuncWithHandler* copy = new ServerRecvFuncWithHandler(m_server);
        return copy;
    }
    catch (std::bad_alloc & a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}
} // namespace advcpp

#endif // EA5AF743_26FA_4A7A_9AB0_DEE405AE6049_H__