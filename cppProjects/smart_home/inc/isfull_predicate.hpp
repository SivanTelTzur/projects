#ifndef E301BF94_6DBD_4A8A_B01E_5EC8BB684854_H__
#define E301BF94_6DBD_4A8A_B01E_5EC8BB684854_H__

#include "waiting_queue.hpp"
namespace advcpp
{
template <typename T>
class IsFullPred
{
public:
    explicit IsFullPred(MtWaitingQueue<T>& a_waitingQueue);

    bool operator()();

private:
    MtWaitingQueue<T>& m_waitingQueue;

};

template<typename T>
IsFullPred<T>::IsFullPred(MtWaitingQueue<T>& a_waitingQueue) 
: m_waitingQueue(a_waitingQueue)
{
}

template<typename T>
bool IsFullPred<T>::operator()() 
{
    return m_waitingQueue.IsFullNoGuard();
}


} //advcpp

#endif // E301BF94_6DBD_4A8A_B01E_5EC8BB684854_H__
