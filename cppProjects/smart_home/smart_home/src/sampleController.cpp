#include "sampleController.hpp"
#include "sleep_micro_sec.hpp"
#include "create_tcp_client.hpp"
#include "logger.hpp"

namespace advcpp
{

Controller::Controller(const std::string& a_ip, size_t a_port) 
: m_net(CreateTcpClient(a_ip, a_port)){
    
}


Controller::~Controller() 
{
    delete m_net;    
}

void Controller::Run() 
{
    int i = 1000   ;
    while (i ){ //while (m_active){
        std::string recvMsg = "";
        try{
            m_net->Recv(recvMsg);
        } catch (const std::exception& a_ex){
            StartLogger().Exception(a_ex);
            StartLogger().Message("receive msg failed controller");
        }
        recvData(recvMsg);
        --i;
    }    
}

void Controller::Stop() 
{
    
}


void Controller::recvData(std::string a_msg) 
{
    StartLogger().Message(a_msg);    
}
    
} // namespace advcpp
