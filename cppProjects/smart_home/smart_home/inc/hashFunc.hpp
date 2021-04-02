#ifndef E07D4BD8_3732_404F_BBCA_E5A4BD6E2C43_H__
#define E07D4BD8_3732_404F_BBCA_E5A4BD6E2C43_H__

#include "event.hpp"

#include <tr1/unordered_map>

namespace advcpp
{

struct HashFunction
{
    size_t operator()(const Topic& a_topic) const
    {
        size_t res = 17;
        res = res * 31 + std::tr1::hash<std::string>()(a_topic.m_location.m_floor);
        res = res * 31 + std::tr1::hash<std::string>()(a_topic.m_location.m_room);
        res = res * 31 + std::tr1::hash<std::string>()(a_topic.m_eventType);
        return res;
    }
};

} // namespace advcpp

#endif // E07D4BD8_3732_404F_BBCA_E5A4BD6E2C43_H__