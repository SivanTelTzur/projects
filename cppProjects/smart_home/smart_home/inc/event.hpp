#ifndef DD648B17_474B_4411_B4AC_46499C0045AF_H__
#define DD648B17_474B_4411_B4AC_46499C0045AF_H__

#include <string>
namespace advcpp
{

typedef struct Location
{   
    std::string m_room;
    std::string m_floor;
} Location;

typedef struct Topic
{
    std::string m_eventType;
    Location m_location;
} Topic;


typedef struct Event{
    Topic m_topic;
    std::string m_timestamp;
    std::string m_payLoad;
} Event;  

inline bool operator==(const Location& a_x, const Location& a_y) {
    return (a_x.m_floor == a_y.m_floor && a_x.m_room == a_y.m_room);
}
inline bool operator==(const Topic& a_x, const Topic& a_y) {
    return (a_x.m_location == a_y.m_location && a_x.m_eventType == a_y.m_eventType);
}

} // namespace advcpp

#endif // DD648B17_474B_4411_B4AC_46499C0045AF_H__
