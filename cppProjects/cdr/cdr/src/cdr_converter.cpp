#include "cdr_converter.hpp"

#include <stdlib.h> 
namespace advcpp
{
namespace cdr_converter
{
    
Subscriber CdrToSubscriber(const Cdr& a_cdr) 
{
    Subscriber newSub;
    newSub.imsi = a_cdr.imsi;
    newSub.msisdn = a_cdr.msisdn;
    if (a_cdr.usageType == "MOC"){
        newSub.voiceOut = std::strtol(a_cdr.duration.c_str(), 0 ,10);
    } else if (a_cdr.usageType == "MTC"){
        newSub.voiceIn = std::strtol(a_cdr.duration.c_str(), 0 ,10);
    } else if (a_cdr.usageType == "SMS-O"){
        newSub.smsIn = 1;
    } else if (a_cdr.usageType == "SMS-MT"){
        newSub.smsOut = 1;
    }else {
        newSub.dataIn = std::strtol(a_cdr.byteRcv.c_str(), 0, 10);
        newSub.dataOut = std::strtol(a_cdr.byteTransmit.c_str(), 0, 10);
    }

    return newSub;
}

} // namespace cdr_converter

} // namespace advcpp
