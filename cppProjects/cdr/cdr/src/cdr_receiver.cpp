#include "cdr_receiver.hpp"

#include "thread.hpp"
#include "func_server.hpp"
#include "protocol.hpp"
#include "logger.hpp"
#include "sleep_micro_sec.hpp"
#include "inet_server.hpp"

#include <exception>

namespace advcpp
{

CdrReceiver::CdrReceiver(size_t a_port, MtWaitingQueue<Cdr>& a_cdrQueue) 
: m_queue(a_cdrQueue)
, m_bufferQueue(MAX_QUEUE_SIZE)
, m_port(a_port)
{
    
}

CdrReceiver::~CdrReceiver() 
{   
}

void CdrReceiver::Start() 
{
    //create server thread 
    ServerTcpFunc serverFunc(m_port, m_bufferQueue);
    Thread serverThread(serverFunc);    

    for (;;){
        std::string buff = m_bufferQueue.Dequeue();
        {
            Cdr cdrToFill;
            try{
                cdr_protocol::Unpack(buff, cdrToFill);
            }
            catch (const std::exception& a_ex){
                StartLogger().Message( "protocol sent exception" );
                StartLogger().Exception(a_ex);
            }
            try{
                m_queue.Enqueue(cdrToFill);
            }
            catch (const std::exception& a_ex){
                StartLogger().Message("enqueue cdr failed receiver thread ");
                StartLogger().Exception(a_ex);
                throw;
            }
        }
    }
    delete &serverFunc; 
    serverThread.Cancel();
}

} // namespace advcpp
