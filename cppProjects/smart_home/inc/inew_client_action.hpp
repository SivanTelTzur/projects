#ifndef C85F425D_0B7C_4E89_BD53_623325AB8603_H__
#define C85F425D_0B7C_4E89_BD53_623325AB8603_H__

#include <string>

namespace advcpp
{
class INewClientHandlerFunc
{
public:
    virtual ~INewClientHandlerFunc() = 0;

    //virtual void Run() = 0;
    virtual void NewClient(int a_clientSocket, const std::string& a_ip) = 0;
private:

};

inline INewClientHandlerFunc::~INewClientHandlerFunc()
{
    
}    
    
} // namespace advcpp

#endif // C85F425D_0B7C_4E89_BD53_623325AB8603_H__
