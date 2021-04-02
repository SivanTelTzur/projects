#ifndef AFF794FC_57EC_4F50_9BF6_1F624B9DE591_H__
#define AFF794FC_57EC_4F50_9BF6_1F624B9DE591_H__

#include "inet_client.hpp"

#include <string>
namespace advcpp
{

INetClient& CreateTcpClient(const std::string& a_ip, size_t a_port);

} // namespace advcpp

#endif // AFF794FC_57EC_4F50_9BF6_1F624B9DE591_H__