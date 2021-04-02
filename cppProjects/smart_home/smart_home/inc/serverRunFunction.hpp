#ifndef CA332330_2FA1_42A8_9772_B31B8A0D2C73_H__
#define CA332330_2FA1_42A8_9772_B31B8A0D2C73_H__

#include "functor.hpp"

namespace advcpp
{
class ServerHandler;
class ServerRecvFunc : public ObjThreadFunc{
public:
    ServerRecvFunc(ServerHandler& a_server);
    ~ServerRecvFunc();

    virtual void operator()();
    virtual ObjThreadFunc* Clone() const;

private:
    ServerHandler& m_server;
};

} // namespace advcpp

#endif // CA332330_2FA1_42A8_9772_B31B8A0D2C73_H__
