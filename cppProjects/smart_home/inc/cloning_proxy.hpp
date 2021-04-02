#ifndef D8A52D92F_F2EF_4FB9_82BE_2515D7D826ES_H__
#define D8A52D92F_F2EF_4FB9_82BE_2515D7D826ES_H__

#include "semaphore.hpp"

namespace advcpp
{
template<class T>
class CloningProxy
{
public:
    explicit CloningProxy(const T& a_obj);
    ~CloningProxy();

    T* Clone() const;

    void Wait();
    void Signal();
private:
    const T& m_object;
    Semaphore m_sem;
};

} // namespace advcpp

#include "inl/cloning_proxy.hxx"
#endif //D8A52D92F_F2EF_4FB9_82BE_2515D7D826ES_H__