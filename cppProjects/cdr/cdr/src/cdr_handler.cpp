#include "cdr_handler.hpp"
#include "thread.hpp"
#include "cdr_functors.hpp"
#include "logger.hpp"

#include <iostream> //cerr
#include <unistd.h> //usleep

namespace advcpp
{
std::string CdrHandler::DIR_NAME = "./files"; 
const std::string CdrHandler::IP = "127.0.0.1"; 

CdrHandler::CdrHandler()
: m_cdrs(1000)
, m_files(10)
{

}

CdrHandler::~CdrHandler()
{

}

void CdrHandler::Run()
{
    startNotifierThread();
    startCdrSenderNetTheard();

    for (;;){
        startReaderThread();
        SleepMicroSeconds(1000000);
    }
    
}

void CdrHandler::startNotifierThread() 
{
    try{
        NotifierThreadFunc notifyFunc(m_files, DIR_NAME);
        Thread notifierThread(notifyFunc);
        notifierThread.Detach();
    } 
    catch (const std::exception& a_ex){

        std::cerr << a_ex.what() << std::endl;
    }
    catch (...){
        std::cerr << "startNotifierThread throw unknown " << std::endl;    
    }
}

void CdrHandler::startReaderThread() 
{
    if (!m_files.IsEmpty()){ //maybe add semaphore for controlling the num of active threads
        {
            try{
                std::string fileName =  m_files.Dequeue();
                std::cerr << "Handeling file : "  << fileName << std::endl;//DELETE
                ReaderThreadFunc readerFunc(m_cdrs, fileName);
                Thread reader(readerFunc);
                reader.Detach();
            } 
            catch (const std::exception& a_ex){
                StartLogger().Exception(a_ex);
            }
            catch (...){
                StartLogger().Message("startReaderThread throw unknown ");
            }
        }
    }  
}

void CdrHandler::startCdrSenderNetTheard() 
{
    try{
        SenderThreadFunc senderFunc(m_cdrs, IP, PORT);
        Thread senderThread(senderFunc);
        senderThread.Detach();
    } 
    catch (const std::exception& a_ex){
        StartLogger().Exception(a_ex);
    }
    catch (...){
        StartLogger().Message("start Sender Thread throw unknown");
    }    
}

void CdrHandler::SleepMicroSeconds(unsigned int a_microSeconds)
{
    useconds_t givenTime = a_microSeconds;
    usleep(givenTime);
}

} // namespace advcpp
