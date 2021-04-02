#ifndef B990DBB3_7226_4729_A5BD_6066AA75EA1D_H__
#define B990DBB3_7226_4729_A5BD_6066AA75EA1D_H__

#include "functor.hpp"
#include "sharedptr.hpp"
#include "logger.hpp"
#include "eventsRouter.hpp"

namespace advcpp
{
class RouterRunFunc : public ObjThreadFunc{
public:
    RouterRunFunc(EventsRouter& a_router);
    ~RouterRunFunc();

    virtual void operator()();
    virtual ObjThreadFunc* Clone() const;

private:
    EventsRouter& m_router;
};


inline RouterRunFunc::RouterRunFunc(EventsRouter& a_router)
: m_router(a_router)
{
}

inline RouterRunFunc::~RouterRunFunc()
{
}

inline void RouterRunFunc::operator()()
{
    try{
        m_router.Run();

    }catch(const std::exception& a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}

inline ObjThreadFunc* RouterRunFunc::Clone() const
{
    try{
        RouterRunFunc* copy = new RouterRunFunc(m_router);
        return copy;
    }
    catch (std::bad_alloc & a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}
    
} // namespace advcpp


#endif // B990DBB3_7226_4729_A5BD_6066AA75EA1D_H__