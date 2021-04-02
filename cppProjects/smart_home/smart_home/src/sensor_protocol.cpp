#include "sensor_protocol.hpp"

#include <arpa/inet.h> // htons

namespace advcpp
{
    
namespace Sensor_protocol
{
enum { DELIMITER_COUNT = 6
     , END_OF_LINE = 1};

void Pack(char* a_buffer, const struct Event& a_eventToSend)
{
    *((short*)a_buffer) = htons( a_eventToSend.m_type.size() + a_eventToSend.m_id.size()
                + a_eventToSend.m_room.size() + sizeof(a_eventToSend.m_floor)
                + a_eventToSend.m_log.size() + a_eventToSend.m_config.size()
                + DELIMITER_COUNT + sizeof(short int) + END_OF_LINE);
    a_buffer +=  sizeof(short int);
    *a_buffer++ = '|';
    a_buffer += a_eventToSend.m_type.copy(a_buffer, a_eventToSend.m_type.size(), 0);
    *a_buffer++ = '|';
    a_buffer += a_eventToSend.m_id.copy(a_buffer, a_eventToSend.m_id.size(), 0);
    *a_buffer++ = '|';
    a_buffer += a_eventToSend.m_room.copy(a_buffer, a_eventToSend.m_room.size(), 0);
    *a_buffer++ = '|';
    *a_buffer++ = a_eventToSend.m_floor;
    *a_buffer++ = '|';
    a_buffer += a_eventToSend.m_log.copy(a_buffer, a_eventToSend.m_log.size(), 0);
    *a_buffer++ = '|';
    a_buffer += a_eventToSend.m_config.copy(a_buffer, a_eventToSend.m_config.size(), 0);
    *a_buffer = '\n';
}
void Pack(char* a_buffer, const struct Config& a_configToSend);

void Unpack(const std::string& a_buffer, struct Config& a_config);
void Unpack(const std::string& a_buffer, struct Event& a_eventRcv)
{
    char delimiter = '|';
    size_t pos = a_buffer.find_first_of(delimiter); 

    size_t start = pos + 1;   
    pos = a_buffer.find_first_of(delimiter); 
    a_eventRcv.m_type.assign(a_buffer.begin() + start, a_buffer.begin() + pos);

    start = pos + 1; 
    pos = a_buffer.find_first_of(delimiter, pos + 1);
    a_eventRcv.m_id.assign(a_buffer.begin() + start, a_buffer.begin() + pos);
    
    start = pos + 1; 
    pos = a_buffer.find_first_of(delimiter, pos + 1);
    a_eventRcv.m_room.assign(a_buffer.begin() + start, a_buffer.begin() + pos);

    start = pos + 1; 
    pos = a_buffer.find_first_of(delimiter, pos + 1);
    a_eventRcv.m_floor = a_buffer.at(start);

    start = pos + 1; 
    pos = a_buffer.find_first_of(delimiter, pos + 1);
    if (pos != start){
        a_eventRcv.m_log.assign(a_buffer.begin() + start, a_buffer.begin() + pos);
    }

    start = pos + 1; 
    a_eventRcv.m_log.assign(a_buffer.begin() + start, a_buffer.end());
}


} // namespace Sensor_protocol

} // namespace advcpp
