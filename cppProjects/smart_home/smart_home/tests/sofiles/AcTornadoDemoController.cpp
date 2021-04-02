#include "AcTornadoDemoController.hpp"

#include <arpa/inet.h> // htons
#include <cstring>

namespace advcpp
{

extern "C"
IAgent* CreateMeAnAgent(IRouter& a_router, IServer& a_server, ISubscription& a_subscription) 
{
    return new AcTornadoDemoController(a_router, a_server, a_subscription);    
}

AtomicValue<int> AcTornadoDemoController::m_counter(0);

AcTornadoDemoController::AcTornadoDemoController(IRouter& a_router, IServer& a_server, ISubscription& a_subscription) 
: m_server(a_server)
, m_router(a_router)
, m_subscription(a_subscription)
, m_deviceData()
{
        
}

AcTornadoDemoController::~AcTornadoDemoController() 
{
    
}

void AcTornadoDemoController::InitAgent(const Config& a_config, SharedPtr<IAgent> a_pThis) 
{
    m_deviceData = a_config;
    if (m_deviceData.m_config.size() < 0 ){
        return;
    }
    subscribeEventsFromConfig(a_pThis);
    //Topic topic = {"LOW_TEMP", {"room-1-b", "1"}};
    //Subscribe(topic, a_pThis);   
}

void AcTornadoDemoController::Subscribe(Topic a_topic, SharedPtr<IAgent> a_pThis) 
{
    if (m_deviceData.m_config.size() < 0 ){
        return;
    }
    subscribeEventsFromConfig(a_pThis);    
}

void AcTornadoDemoController::Notify(const std::string& a_msg) 
{

}

void AcTornadoDemoController::testMe() const
{
    
}

void AcTornadoDemoController::Update(const Event a_event) const
{
    unsigned int bufferSize;
    char* msg = PackEvent(&bufferSize, a_event); 
    m_server.Send(msg, bufferSize, m_deviceData.m_socket);
    delete msg;    
}

void AcTornadoDemoController::UpdateSocket(int a_socketUpdate) 
{
    m_deviceData.m_socket = a_socketUpdate;
}

char* AcTornadoDemoController::PackEvent(unsigned int* a_buffersize, const Event a_event) const 
{
    *a_buffersize = a_event.m_timestamp.size() 
                    + a_event.m_topic.m_eventType.size() + a_event.m_payLoad.size() +
                    + a_event.m_topic.m_location.m_room.size() + a_event.m_topic.m_location.m_floor.size()
                    + 5 + sizeof(unsigned int); //5 stand for the size of each element 

    char* initialBuffer = new char[*a_buffersize];
    char* buffer = initialBuffer;
    *((unsigned int*)buffer) = htonl(*a_buffersize - sizeof(unsigned int));
    buffer = buffer + sizeof(unsigned int); 

    *buffer++ = a_event.m_timestamp.size(); 
    strncpy(buffer, a_event.m_timestamp.c_str(), a_event.m_timestamp.size());
    buffer = buffer + a_event.m_timestamp.size();

    *buffer++ = a_event.m_topic.m_eventType.size(); 
    strncpy(buffer, a_event.m_topic.m_eventType.c_str(), a_event.m_topic.m_eventType.size());
    buffer = buffer + a_event.m_topic.m_eventType.size();

    *buffer++ = a_event.m_payLoad.size(); 
    strncpy(buffer, a_event.m_payLoad.c_str(), a_event.m_payLoad.size());
    buffer = buffer + a_event.m_payLoad.size();

    *buffer++ = a_event.m_topic.m_location.m_room.size(); 
    strncpy(buffer, a_event.m_topic.m_location.m_room.c_str(), a_event.m_topic.m_location.m_room.size());
    buffer = buffer + a_event.m_topic.m_location.m_room.size();

    *buffer++ = a_event.m_topic.m_location.m_floor.size(); 
    strncpy(buffer, a_event.m_topic.m_location.m_floor.c_str(), a_event.m_topic.m_location.m_floor.size());
    buffer = buffer + a_event.m_topic.m_location.m_floor.size();

    buffer = buffer - *a_buffersize;
    return buffer;
}

void AcTornadoDemoController::subscribeEventsFromConfig(SharedPtr<IAgent> a_pThis) 
{
    std::string configData = m_deviceData.m_config;
    char delimeter = '"';
    size_t found = configData.find("event:");
    while (found != std::string::npos){

        size_t startPos = configData.find_first_of(delimeter, found);
        size_t endPos = configData.find_first_of(delimeter, startPos + 1); //end of Event pos

        Topic topic;
        topic.m_eventType.append(configData, startPos + 1, endPos - startPos - 1);

        found = endPos + 1;
        startPos = configData.find_first_of(delimeter, found);
        endPos = configData.find_first_of(delimeter, startPos + 1); //end of room pos

        if (startPos == endPos){
            topic.m_location.m_room = "ANY";
        }else{
            topic.m_location.m_room.append(configData, startPos + 1, endPos - startPos - 1);
        }

        found = endPos + 1;
        startPos = configData.find_first_of(delimeter, found);
        endPos = configData.find_first_of(delimeter, startPos + 1); //end of floor pos
        if (startPos == endPos){
            topic.m_location.m_floor = "ANY";
        }else{
            topic.m_location.m_floor.append(configData, startPos + 1, endPos - startPos - 1);
        }

        m_subscription.Subscribe(topic, a_pThis);
        found = configData.find("event:", found); //next event
    }
 
}




//void AcTornadoDemoController::fillEvent(const std::string& a_msg, Event& a_toFill) const 
//{
//    Event event;
//    const char* buffer = a_msg.c_str();
//
//    size_t currentLen = *buffer++;//(size_t)(*(buffer));
//    event.m_timestamp.append(buffer, currentLen);
//    buffer = buffer + currentLen;
//
//    currentLen = *buffer++;
//    event.m_topic.m_eventType.append(buffer, currentLen);
//    buffer = buffer + currentLen;
//
//    currentLen = *buffer++;
//    event.m_payLoad.append(buffer, currentLen);
//    buffer = buffer + currentLen;
//
//    event.m_topic.m_location.m_room = this->m_deviceData.m_room;
//    event.m_topic.m_location.m_floor = this->m_deviceData.m_floor;
//
//    a_toFill = event;
//}
//


} // namespace advcpp
