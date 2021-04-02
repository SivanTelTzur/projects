#ifndef C93A5F61_D06F_4BF6_BB62_46A500653774_H__
#define C93A5F61_D06F_4BF6_BB62_46A500653774_H__

#include "singleton.hpp"
#include "mutex_guard.hpp"

#include <exception>

namespace advcpp
{

template<typename T>
T*  Singleton<T>::s_tInstance = 0;

template<typename T>
bool  Singleton<T>::s_isFirst = false;


template<typename T>
Singleton<T>::Singleton() 
{
    
}

template<typename T>
T& Singleton<T>::GetInstance() 
{
    T* temp;
    if (!(temp  = __sync_add_and_fetch(&Singleton::s_tInstance, 0))) { // m_tInstance == 0

        if (__sync_bool_compare_and_swap(&Singleton::s_isFirst, false, true)){ //first thread
            temp = new T;
            Singleton::s_tInstance = __sync_add_and_fetch(&temp, 0);   
        }
        else{ //all threads that came when m_tInstance was 0
            do{
                __asm volatile ("pause" ::: "memory");
            }while (!(temp  = __sync_add_and_fetch(&Singleton::s_tInstance, 0)));
        }
    }
    return *temp;
}

} // namespace advcpp


#endif // C93A5F61_D06F_4BF6_BB62_46A500653774_H__
