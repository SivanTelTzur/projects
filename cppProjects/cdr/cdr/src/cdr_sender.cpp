#include "cdr_sender.hpp"

#include "create_tcp_client.hpp"
#include "protocol.hpp"
#include "logger.hpp"
#include "sleep_micro_sec.hpp"
#include "string.h" // memset

#include <exception>

namespace advcpp
{

CdrSender::CdrSender(MtWaitingQueue<Cdr>& a_cdrQueue, const std::string& a_ip, size_t a_port) 
: m_queue(a_cdrQueue)
, m_clientNet(CreateTcpClient(a_ip, a_port))
{
    
}

CdrSender::~CdrSender() 
{
    delete &m_clientNet;    
}

void CdrSender::Start() 
{
    for (;;){
        Cdr cdr = m_queue.Dequeue();
        cdr.UpdateMemAddr(); //update stack address of cdr struct members

        char buffer[BUFFER_SIZE];
        memset(buffer , 0, sizeof(buffer));
        cdr_protocol::Pack(buffer, cdr);

        try{
            m_clientNet.Send(buffer);
            SleepMicroSeconds(20);
        }
        catch (const std::exception& a_ex){
            StartLogger().Message("send cdr failed ");
            StartLogger().Exception(a_ex);
            //throw;
        }
    }
}


} // namespace advcpp
