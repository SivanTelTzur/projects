#ifndef AC7D7810_38C9_48A5_A1F1_EE38A8EE7F16_H__
#define AC7D7810_38C9_48A5_A1F1_EE38A8EE7F16_H__

#include <string> //string, size_t
#include <ctime> //tm struct
#include <vector>
namespace advcpp
{
struct Cdr
{
    Cdr();
    void UpdateMemAddr();
    std::string IMSI;
    std::string IMEI;
    std::string usageType;
    std::string MSISDN;
    std::string date;
    std::string time;
    std::string duration;
    std::string byteRcv;
    std::string byteTransmit;
    std::string secondIMSI;
    std::string secondMSISDN;
    std::string* pValues[11];
};

} // namespace advcpp

#endif // EECB3538_0strAB_4E9A_8E99_01608B30B651_H__
