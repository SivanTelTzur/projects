#ifndef B08B09D8_C33E_4A86_9B75_9418DDBA9A4A_H__
#define B08B09D8_C33E_4A86_9B75_9418DDBA9A4A_H__

#include "inet_server.hpp"
#include "waiting_queue.hpp"

#include <string>
#include <unistd.h> //size_t

namespace advcpp
{

INetServer& CreateTcpServer(size_t a_port, MtWaitingQueue<std::string>& a_msgQueue);

} // namespace advcpp

#endif // B08B09D8_C33E_4A86_9B75_9418DDBA9A4A_H__