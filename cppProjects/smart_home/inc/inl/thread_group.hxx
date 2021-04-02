#ifndef EA9A3DCD_3242_4D40_9CC5_F81CCE754003
#define EA9A3DCD_3242_4D40_9CC5_F81CCE754003

#include "thread_group.hpp"
#include "logger.hpp"
#include <algorithm>

namespace advcpp
{


template <typename Functor>
class JoinMembersAndDo{
public:
    JoinMembersAndDo(Functor a_func): m_func(a_func){}

    void operator()(Thread* a_currThread)
    {
        try{
            a_currThread->Join();
            delete a_currThread;
            m_func();
        }
        catch(const std::exception& a_ex){
            StartLogger().Exception(a_ex);
            StartLogger().Message("Join failed - JoinMembers");
        }
    }
private:
    Functor& m_func;
};

template<typename Functor>
void ThreadGroup::JoinAll(Functor a_func) 
{
    try{
        for_each (m_vec.begin(), m_vec.end(), JoinMembersAndDo<Functor>(a_func));    
    }
    catch(const std::exception& a_ex){
        StartLogger().Exception(a_ex);
        StartLogger().Message("JoinMembers failed ");
    }
    m_vec.clear();
 
}

     
} // namespace advcpp


#endif // EA9A3DCD_3242_4D40_9CC5_F81CCE754003