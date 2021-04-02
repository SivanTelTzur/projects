#ifndef FB34BDC5_727D_4C4E_8C0F_886D58325EE1_H__
#define FB34BDC5_727D_4C4E_8C0F_886D58325EE1_H__

#include "functor.hpp"
#include "waiting_queue.hpp"
#include "subscriber_data.hpp"

namespace advcpp
{
class QueryGatewayFunc : public ObjThreadFunc{
public:
    QueryGatewayFunc(SubscriberData& a_subData);
    ~QueryGatewayFunc();

    virtual void operator()();
    virtual ObjThreadFunc* Clone() const;

private:
    SubscriberData& m_subData;
};

} // namespace advcpp

#endif // FB34BDC5_727D_4C4E_8C0F_886D58325EE1_H__
