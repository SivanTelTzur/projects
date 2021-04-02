#ifndef DB4B5FB2_D44A_4AF1_9BE4_98D8D071FA9C_H__
#define DB4B5FB2_D44A_4AF1_9BE4_98D8D071FA9C_H__

#include "waiting_queue.hpp"

namespace advcpp
{

template <typename T>
class IsEmptyPred
{
public:
    explicit IsEmptyPred(MtWaitingQueue<T>& a_waitingQueue);
    //~IsFullPred(); default

    bool operator()();

private:
    MtWaitingQueue<T>& m_waitingQueue;
};

template<typename T>
IsEmptyPred<T>::IsEmptyPred(MtWaitingQueue<T>& a_waitingQueue) 
: m_waitingQueue(a_waitingQueue)
{
    
}

template<typename T>
bool IsEmptyPred<T>::operator()() 
{
    return m_waitingQueue.IsEmptyNoGuard();
}

    
} // namespace advcpp

#endif // DB4B5FB2_D44A_4AF1_9BE4_98D8D071FA9C_H__
