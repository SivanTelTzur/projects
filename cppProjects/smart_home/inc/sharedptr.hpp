#ifndef B5852F7C_267C_4AD5_B18C_691E5D46A89A_H__
#define B5852F7C_267C_4AD5_B18C_691E5D46A89A_H__

#include "atomic_counter.hpp"
#include <cstdlib>

namespace advcpp
{
template<typename T>
class SharedPtr{
        template<typename S>
        friend class SharedPtr;
public:
    explicit SharedPtr(T* a_ptr = 0);

    ~SharedPtr();

    SharedPtr(const SharedPtr& a_copyFrom);
    template <typename S> SharedPtr(const SharedPtr<S>& a_copyFrom);

    SharedPtr& operator=(const SharedPtr& a_assign);
    template <typename S> SharedPtr& operator=(const SharedPtr<S>& a_assign);

    SharedPtr& Reset();

    T& operator*();
    const T& operator*() const;

    T* operator->();
    const T* operator->() const;

    bool operator!()const ;

    bool operator==(const SharedPtr& a_compareTo) const;
    template <typename S>bool operator==(const SharedPtr<S>& a_compareTo) const;

    bool operator!=(const SharedPtr& a_compareTo) const;
    template <typename S> bool operator!=(const SharedPtr<S>& a_compareTo) const;

    typedef T* SharedPtr<T>::* UnspecifiedBoolType;
    operator UnspecifiedBoolType() const ;

    size_t GetCounter()const; //only for test !
private:
    AtomicCounter<size_t>* getCounterPointer() const;
    void prepareForAssign();
private:
    T* m_ptr;
    AtomicCounter<size_t>* m_counter;
};

} // namespace advcpp

#include "inl/sharedptr.hxx"

#endif // B5852F7C_267C_4AD5_B18C_691E5D46A89A_H__
