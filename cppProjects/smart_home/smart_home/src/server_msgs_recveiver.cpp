#include "server_msgs_recveiver.hpp"
#include <iostream>

namespace advcpp
{


ServerMsgsReceiver::ServerMsgsReceiver(WaitableQueue<SensorMsg>& a_msgQueue) 
: m_msgQueue(a_msgQueue)
{   
}

ServerMsgsReceiver ::~ServerMsgsReceiver ()
{
}

void ServerMsgsReceiver::ProcessMsg(const std::string& a_buffer, int a_clientSocket, std::string a_ip) 
{
    SensorMsg newMsg = {a_ip, a_buffer};
    m_msgQueue.Enqueue(newMsg);   
}

} // namespace advcpp
