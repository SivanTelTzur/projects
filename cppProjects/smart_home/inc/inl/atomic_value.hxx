#ifndef DB5B62F3_B6E3_453F_B583_62B4F093B17BR_H__
#define DB5B62F3_B6E3_453F_B583_62B4F093B17BR_H__

#include "atomic_value.hpp"

#include <assert.h>

namespace advcpp
{

template<typename T>
AtomicValue<T>::AtomicValue(T a_initialVal)
: m_value(a_initialVal)
{
    assert(sizeof(T) <= 8);
}

template<typename T>
AtomicValue<T>::AtomicValue(const AtomicValue& a_copyFrom)
: m_value(__sync_add_and_fetch(&m_value, 0)) 
{  
}

template<typename T>
T AtomicValue<T>::operator++() 
{ 
    return __sync_add_and_fetch(&m_value, 1);    
}

template<typename T>
T AtomicValue<T>::operator--() 
{
    return __sync_sub_and_fetch(&m_value, 1);       
}

template<typename T>
T AtomicValue<T>::operator--(int) 
{
    return __sync_fetch_and_sub(&m_value, 1);
    
}

template<typename T>
T AtomicValue<T>::operator++(int) 
{
    return __sync_fetch_and_add(&m_value, 1);    
}

template<typename T>
T AtomicValue<T>::Get() const
{
    return __sync_add_and_fetch(&m_value,0);
}



template<typename T>
AtomicValue<T>::operator T() const
{
    return __sync_fetch_and_add(&m_value,0);        
}

template<typename T>
void AtomicValue<T>::operator-=(T a_val) 
{
    __sync_sub_and_fetch(&m_value, a_val);    
}

template<typename T>
void AtomicValue<T>::operator+=(T a_val) 
{
    __sync_add_and_fetch(&m_value, a_val);    
}

template<typename T>
bool AtomicValue<T>::operator==(T a_val) 
{
    return __sync_bool_compare_and_swap(&m_value, a_val , a_val);    
}

template<typename T>
bool AtomicValue<T>::operator!=(T a_val) 
{
    return (!(*this == a_val));    
}

template<typename T>
void AtomicValue<T>::Set(T a_val) 
{
     (void)__sync_lock_test_and_set(&m_value, a_val);
}

template<typename T>
void AtomicValue<T>::Reset() 
{
    Set(0);
}


inline  AtomicValue<bool>::AtomicValue(bool a_val) 
: m_val(a_val)
{
   
}

inline void AtomicValue<bool>::Set(bool a_val) 
{
    (void)__sync_lock_test_and_set(&m_val, a_val);        
}

inline bool AtomicValue<bool>::Get() const
{
    return __sync_bool_compare_and_swap(&m_val, true , true);    
}

inline bool AtomicValue<bool>::operator==(bool a_val) const
{
    return __sync_bool_compare_and_swap(&m_val, a_val , a_val);    
}  

} // namespace advcpp


#endif // DB5B62F3_B6E3_453F_B583_62B4F093B17BR_H__
