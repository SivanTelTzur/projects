#ifndef E0AFE807_C888_479C_87A7_F50D64534D4C_H__
#define E0AFE807_C888_479C_87A7_F50D64534D4C_H__

namespace advcpp
{
template<typename T>
//{requires : T in an integer value (char, short, int, long)}
class AtomicValue {
public:
    explicit AtomicValue(T a_initialVal = T(0));  

    //~AtomicValue();//default
    AtomicValue(const AtomicValue& a_copyFrom);//default

    T operator++();//++g
    T operator++(int); //g++
    void operator+=(T a_val);



    T operator--(); //--g;
    T operator--(int); // g--
    void operator-=(T a_val);

    bool operator==(T a_val);
    bool operator!=(T a_val);
    void Set(T a_val);
    void Reset();

    operator T() const;
    T Get() const;

private:
    AtomicValue& operator=(const AtomicValue& a_AtomicValue);

private:
   mutable T m_value;
};

//template<>
//inline bool AtomicValue<bool>::Get() const
//{
//    return __sync_bool_compare_and_swap(&m_value, true , true);
//}


template<>
class AtomicValue<bool>
{
public:
    explicit AtomicValue(bool a_val);
    void Set(bool a_ex);
    bool Get() const;

    bool operator==(bool a_val) const;
private:
    mutable bool m_val;
};


template<> class AtomicValue<double>;

template<> class AtomicValue<float> ;
//template<> class AtomicValue<long long> ;

}//advcpp



#include "inl/atomic_value.hxx"

#endif // E0AFE807_C888_479C_87A7_F50D64534D4C_H__
