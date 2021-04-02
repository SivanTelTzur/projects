#ifndef C026E316_E7D7_4B7E_83C4_1EB162B81F0D_H__
#define C026E316_E7D7_4B7E_83C4_1EB162B81F0D_H__

#include <string> //string, size_t
namespace advcpp
{
struct Carrier
{
    Carrier()
    : mccMnc()
    , voiceOut(0)
    , voiceIn(0)
    , smsOut(0)
    , smsIn(0) 
    {}
    std::string mccMnc;
    long voiceOut;
    long voiceIn;
    long smsOut;
    long smsIn;
};

} // namespace advcpp
#endif // C026E316_E7D7_4B7E_83C4_1EB162B81F0D_H__