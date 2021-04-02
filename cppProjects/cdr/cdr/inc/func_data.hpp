#ifndef DB3CAE51_6073_422D_AB7D_EB09DF320A42_H__
#define DB3CAE51_6073_422D_AB7D_EB09DF320A42_H__

#include "functor.hpp"
#include "waiting_queue.hpp"
#include "cdr_struct.hpp"
#include "subscriber_data.hpp"

namespace advcpp
{
class SubscriberMngFunc : public ObjThreadFunc{
public:
    SubscriberMngFunc(MtWaitingQueue<Subscriber>& a_subQueue, SubscriberData& a_subData);
    ~SubscriberMngFunc();

    virtual void operator()();
    virtual ObjThreadFunc* Clone() const;

private:
    MtWaitingQueue<Subscriber>& m_subQueue;
    SubscriberData& m_subData;
};


class DataMngFunc : public ObjThreadFunc{
public:
    DataMngFunc(MtWaitingQueue<Cdr>& a_cdrsQueue);
    ~DataMngFunc();

    virtual void operator()();
    virtual ObjThreadFunc* Clone() const;

private:
    MtWaitingQueue<Cdr>& m_cdrsQueue;
    
};

} // namespace advcpp

#endif // DB3CAE51_6073_422D_AB7D_EB09DF320A42_H__
