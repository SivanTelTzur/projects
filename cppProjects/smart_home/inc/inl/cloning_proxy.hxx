#include "cloning_proxy.hpp"

namespace advcpp
{
template<class T>
CloningProxy<T>::CloningProxy(const T& a_obj)
: m_object(a_obj)
, m_sem(1)
{
}

template<class T>
CloningProxy<T>::~CloningProxy()
{
}

template<class T>
T* CloningProxy<T>::Clone() const
{
    return m_object.Clone();
}

template<class T>
void CloningProxy<T>::Wait()
{
    m_sem.Acquire();
}

template<class T>
void CloningProxy<T>::Signal()
{
    m_sem.Release();
}

}//advcpp