#include "cdr_struct.hpp"

namespace advcpp
{

Cdr::Cdr()
{
    UpdateMemAddr();
}

void Cdr::UpdateMemAddr() 
{
    pValues[0] = &imsi;
    pValues[1] = &imei;
    pValues[2] = &usageType;
    pValues[3] = &msisdn;
    pValues[4] = &date;
    pValues[5] = &time;
    pValues[6] = &duration;
    pValues[7] = &byteRcv;
    pValues[8] = &byteTransmit;
    pValues[9] = &secondImsi;
    pValues[10] = &secondMsisdn;        
}

    


} // namespace advcpp
