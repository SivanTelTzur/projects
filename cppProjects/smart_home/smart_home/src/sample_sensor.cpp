#include "sample_sensor.hpp"
#include "create_tcp_client.hpp"
#include "sleep_micro_sec.hpp"
#include "logger.hpp"
//#include "sampleProtocol"


#include <string>
#include <string.h>
#include <iostream>
#include <arpa/inet.h> // htons


namespace advcpp
{
Sensor::Sensor(const std::string& a_ip, size_t a_port) 
: m_shpNet(CreateTcpClient(a_ip, a_port))
, m_active(true)
{
    
}

void Sensor::Run() 
{
    int i = 100   ;
    while (i ){ //while (m_active){
        unsigned int len = 0 ;
        char* buffer = collectData(&len);
        //std::cerr << buffer << std::endl; 
        try{
            m_shpNet->Send(buffer, len);
        } catch (const std::exception& a_ex){
            StartLogger().Exception(a_ex);
        }
        SleepMicroSeconds(1000000);
        --i;
        delete buffer;  
    }   
}

char* Sensor::collectData(unsigned int* a_buffersize) 
{
    time_t ct = time(0);
    std::string timestamp(ctime(&ct));

    std::string type("LOW_TEMP");
    std::string payload("this is the temperatore sensor");

    unsigned int bufferLen = timestamp.size() + type.size() + payload.size() + 3 + sizeof(unsigned int);
    char* initialBuffer = new char[bufferLen];
    char* buffer = initialBuffer;
    *((unsigned int*)buffer) = htonl(bufferLen - sizeof(unsigned int));
    buffer = buffer + sizeof(unsigned int);

    *buffer++ = timestamp.size();
    strncpy(buffer, timestamp.c_str(), timestamp.size());
    buffer = buffer + timestamp.size();

    *buffer++ = type.size();
    strncpy(buffer, type.c_str(), type.size());
    buffer = buffer + type.size();
    
    *buffer++ = payload.size();
    strncpy(buffer, payload.c_str(), payload.size());
    buffer = buffer + payload.size();


    *a_buffersize = bufferLen;
    buffer = buffer - bufferLen;
    return buffer;

}


} // namespace advcpp

