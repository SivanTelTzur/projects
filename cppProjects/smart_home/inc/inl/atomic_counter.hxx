#ifndef C21E366D_E473_4316_AF7A_2B1AA0B16945_H__
#define C21E366D_E473_4316_AF7A_2B1AA0B16945_H__

#include "atomic_counter.hpp"

namespace advcpp
{

template<typename T>
AtomicCounter<T>::AtomicCounter(T a_initialVal)
: m_value(a_initialVal) 
{     
}

template<typename T>
AtomicCounter<T>::AtomicCounter(const AtomicCounter& a_copyFrom)
: m_value(a_copyFrom.Get()) 
{
    
}


template<typename T>
void AtomicCounter<T>::Inc() 
{
    ++m_value;    
}

template<typename T>
void AtomicCounter<T>::Dec() 
{
    --m_value;        
}

template<typename T>
void AtomicCounter<T>::Reset(T a_val) 
{
    m_value.Reset();     
}

template<typename T>
void AtomicCounter<T>::Set(T a_val) 
{
    m_value.Set(a_val);     
}

template<typename T>
AtomicCounter<T>::operator T() const
{
    return m_value.Get();    
}

template<typename T>
T AtomicCounter<T>::Get() const
{
    return m_value.Get(); 
}

template<typename T>
bool AtomicCounter<T>::operator==(AtomicCounter& a_counter) 
{
    return m_value == a_counter;   
}

template<typename T>
bool AtomicCounter<T>::operator!=(AtomicCounter& a_counter) 
{
    return m_value != a_counter;    
}

//void AtomicCounter<T>::ExecuteIf(Predicate& a_pred, Func a_actionFunc) 
//{
    //mutex.lock()
    //condVar.wait(a_pred, &mutex);
    //a_actionFunc();
    //mutex.unlock();    
//}

} // namespace advcpp

#endif // C21E366D_E473_4316_AF7A_2B1AA0B16945_H__
