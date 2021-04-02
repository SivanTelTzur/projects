#ifndef A179893B_4A54_4066_AD36_8A4B1E3FABCF_H__
#define A179893B_4A54_4066_AD36_8A4B1E3FABCF_H__

#include "atomic_value.hpp"
//#include "predicate.hpp"

namespace advcpp
{

template<typename T>
//{requires : T in an integer value (char, short, int, long)} //TODO all valid types specialization 
// copy Ctor is valid but assign operator not valid cause it's not atomic
class AtomicCounter {
public:
    typedef void (*Func)(void);
    explicit AtomicCounter(T a_initialVal = T(0));  

    //~AtomicCounter();//default
    AtomicCounter(const AtomicCounter& a_copyFrom);//default

    void Inc();
    void Dec(); 
    void Reset(T a_val);
    void Set(T a_val);

    bool operator==(AtomicCounter& a_counter);
    bool operator!=(AtomicCounter& a_counter);

    operator T() const;
    T Get() const;

    //void ExecuteIf(Predicate& a_pred, Func a_actionFunc);
private:
    AtomicCounter& operator=(const AtomicCounter  & a_AtomicCounter);//default
private:
    AtomicValue<T> m_value;
};

} // namespace advcpp

#include "inl/atomic_counter.hxx"

#endif // A179893B_4A54_4066_AD36_8A4B1E3FABCF_H__
