#ifndef FUNCTOR_H__
#define FUNCTOR_H__

namespace advcpp
{

class ObjThreadFunc
{
public:
    virtual ~ObjThreadFunc();
    virtual void operator()() = 0;
    virtual ObjThreadFunc* Clone() const = 0;

};

} //advcpp

#endif //FUNCTOR_H__
