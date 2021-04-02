#ifndef CB0856E9_18DD_4083_9D05_F401F305A147_H__
#define CB0856E9_18DD_4083_9D05_F401F305A147_H__

#include <string>
namespace advcpp
{

struct Config{
    std::string m_ip;
    std::string m_type;
    std::string m_room;
    std::string m_floor;
    std::string m_log;
    std::string m_config;
    int m_socket;
    void clearAll(){
        m_ip.clear();
        m_type.clear();
        m_room.clear();
        m_floor.clear();
        m_log.clear();
        m_config.clear();
        m_socket = 0;
    }
};

} // namespace advcpp


#endif // CB0856E9_18DD_4083_9D05_F401F305A147_H__