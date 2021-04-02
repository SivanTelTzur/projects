#ifndef A8F40F0D_5D0F_4A51_BE99_4F317CA1C854_H__
#define A8F40F0D_5D0F_4A51_BE99_4F317CA1C854_H__

#include "Imsg_handler_func.hpp"
#include "waitableQueue.hpp"
#include "sensor_msg.hpp"
#include <iostream>

namespace advcpp
{

class TestRcvFunction : public IMsgHandlerFunc
{
public:
    ~TestRcvFunction (){};
    virtual void ProcessMsg(const std::string& a_buffer, int a_clientSocket, std::string a_ip); 

private:

};
inline void TestRcvFunction::ProcessMsg(const std::string& a_buffer, int a_clientSocket, std::string a_ip) 
{
    std::cout << "Ip : " << a_ip << std::endl;
    std::cout << "buffer : " << a_buffer << std::endl;   
    const char* buffer = a_buffer.c_str();
    char temp[256] = {'\0'};
    size_t currentLen = (size_t)(*(buffer));
    ++buffer;

    
    strncpy(temp, buffer, currentLen);
    temp[currentLen] = '\0';
    std::string timestamp(temp);
    
    std::cout << "time : " << timestamp << std::endl;

}

    
} // namespace advcpp


#endif // A8F40F0D_5D0F_4A51_BE99_4F317CA1C854_H__