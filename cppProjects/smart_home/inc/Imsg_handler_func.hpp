#ifndef DC3A396E_5082_439E_8A05_CAFB45DA50C7_H__
#define DC3A396E_5082_439E_8A05_CAFB45DA50C7_H__

#include <string> 

namespace advcpp
{
class IMsgHandlerFunc
{
public:
    virtual ~IMsgHandlerFunc() = 0;

    //virtual void Run() = 0;
    virtual void ProcessMsg(const std::string& a_buffer, int a_clientSocket, std::string a_ip) = 0;
private:

};

inline IMsgHandlerFunc::~IMsgHandlerFunc()
{
    
}    
} // namespace advcpp

#endif // DC3A396E_5082_439E_8A05_CAFB45DA50C7_H__