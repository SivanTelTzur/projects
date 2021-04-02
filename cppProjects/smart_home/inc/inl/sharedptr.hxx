#ifndef C2004A46_CB92_40C2_B5D7_F227AAE5FDEB_H__
#define C2004A46_CB92_40C2_B5D7_F227AAE5FDEB_H__

#include "sharedptr.hpp"

#include <assert.h>
#include <new>

namespace advcpp
{


template<typename T>
SharedPtr<T>::SharedPtr(T* a_ptr) 
: m_ptr(a_ptr)
, m_counter(0)  
{
    if (m_ptr == 0){
        return;
    }
    try{
        m_counter = new AtomicCounter<size_t>(1);
    }
    catch(const std::bad_alloc& a_ex){
        delete a_ptr;
        throw;
    }
}

template<typename T>
SharedPtr<T>::~SharedPtr() 
{
    if (0 == m_ptr){ //case null
        return;
    }
    m_counter->Dec();

    if (0 == m_counter->Get()){
        delete m_ptr;
        delete m_counter;
    }    
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& a_copyFrom) 
: m_ptr(a_copyFrom.m_ptr)
, m_counter(a_copyFrom.m_counter) 
{ 
    if (0 != m_ptr){
        m_counter->Inc();    
    }
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& a_assign) 
{
    if (this != &a_assign){
        //handle the old object
        prepareForAssign();

        m_ptr = a_assign.m_ptr;
        m_counter = a_assign.m_counter;

        if (0 != m_counter){
            m_counter->Inc();   
        }
    }   
    return *this; 
}


template<typename T>
SharedPtr<T>& SharedPtr<T>::Reset() 
{
    prepareForAssign();

    m_ptr = 0;
    m_counter = 0;

    return *this ;    
}


template<typename T>
template<typename S>
SharedPtr<T>::SharedPtr(const SharedPtr<S>& a_copyFrom)
: m_ptr(a_copyFrom.m_ptr)
, m_counter(a_copyFrom.getCounterPointer())
{
    if (0 != m_ptr){
        m_counter->Inc();    
    }  
}

template<typename T>
template<typename S>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<S>& a_assign) 
{   
    //handle the old object
    prepareForAssign();

    m_ptr = a_assign.m_ptr;
    m_counter = a_assign.getCounterPointer();

    if (0 != m_counter){
        m_counter->Inc();   
    }
       
    return *this;
}

template<typename T>
T* SharedPtr<T>::operator->() 
{
    return m_ptr; 
}

template<typename T>
const T* SharedPtr<T>::operator->() const
{
    return m_ptr;    
}

template<typename T>
T& SharedPtr<T>::operator*() 
{
    return *m_ptr; //if null it will get sigfault!
}

template<typename T>
const T& SharedPtr<T>::operator*() const
{
    return *m_ptr; //if null it will get sigfault!    
}

template<typename T>
bool SharedPtr<T>::operator!() const 
{
    return !m_ptr;      
}

template<typename T>
bool SharedPtr<T>::operator==(const SharedPtr& a_compareTo) const
{
    return (m_ptr == a_compareTo.m_ptr);
}

template<typename T>
template<typename S>
bool SharedPtr<T>::operator==(const SharedPtr<S>& a_compareTo) const
{
    return (m_ptr == a_compareTo.m_ptr);    
}

template<typename T>
bool SharedPtr<T>::operator!=(const SharedPtr& a_compareTo) const
{
    return (m_ptr != a_compareTo.m_ptr);    
}


template<typename T>
template<typename S>
bool SharedPtr<T>::operator!=(const SharedPtr<S>& a_compareTo) const
{
    return (m_ptr != a_compareTo.m_ptr);     
}

template<typename T>
SharedPtr<T>::operator UnspecifiedBoolType() const
{
   return m_ptr? &SharedPtr<T>::m_ptr : 0;
}

template<typename T>
void SharedPtr<T>::prepareForAssign() 
{
    if (0 != m_ptr){
        if (1 == m_counter->Get()){ //last pointer holds the object
            delete m_ptr;
            delete m_counter;
        }
        else{
            m_counter->Dec(); 
        } 
    }
}

template<typename T>
AtomicCounter<size_t>* SharedPtr<T>::getCounterPointer() const
{
    return m_counter;
}

template<typename T>
size_t SharedPtr<T>::GetCounter() const
{
    return (0 != m_counter) ? m_counter->Get() : 0;    
}

} // namespace advcpp
 

#endif // C2004A46_CB92_40C2_B5D7_F227AAE5FDEB_H__