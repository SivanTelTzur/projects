#include "cdr_struct.hpp"

namespace advcpp
{

Cdr::Cdr()
{
    UpdateMemAddr();
}

void Cdr::UpdateMemAddr() 
{
    pValues[0] = &IMSI;
    pValues[1] = &IMEI;
    pValues[2] = &usageType;
    pValues[3] = &MSISDN;
    pValues[4] = &date;
    pValues[5] = &time;
    pValues[6] = &duration;
    pValues[7] = &byteRcv;
    pValues[8] = &byteTransmit;
    pValues[9] = &secondIMSI;
    pValues[10] = &secondMSISDN;        
}

    


} // namespace advcpp
