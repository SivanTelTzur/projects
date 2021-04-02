#ifndef F16D81A1_215E_4157_B84E_2FF8EC365E7C_H__
#define F16D81A1_215E_4157_B84E_2FF8EC365E7C_H__

#include "Imsg_handler_func.hpp"
#include "waitableQueue.hpp"
#include "sensor_msg.hpp"

namespace advcpp
{

class ServerMsgsReceiver : public IMsgHandlerFunc
{
public:
    explicit ServerMsgsReceiver(WaitableQueue<SensorMsg>& a_msgQueue);
    ~ServerMsgsReceiver ();
    virtual void ProcessMsg(const std::string& a_buffer, int a_clientSocket, std::string a_ip); 

private:
    WaitableQueue<SensorMsg>& m_msgQueue;
};
    
} // namespace advcpp


#endif // F16D81A1_215E_4157_B84E_2FF8EC365E7C_H__