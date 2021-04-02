#ifndef EECB3538_01AB_4E9A_8E99_01608B30B651_H__
#define EECB3538_01AB_4E9A_8E99_01608B30B651_H__

#include <string> //string, size_t
#include <ctime> //tm struct
#include <vector>
namespace advcpp
{
struct Cdr
{
    Cdr();
    void UpdateMemAddr();
    std::string imsi;
    std::string imei;
    std::string usageType;
    std::string msisdn;
    std::string date;
    std::string time;
    std::string duration;
    std::string byteRcv;
    std::string byteTransmit;
    std::string secondImsi;
    std::string secondMsisdn;
    std::string* pValues[11];
};

} // namespace advcpp

#endif // EECB3538_0strAB_4E9A_8E99_01608B30B651_H__
