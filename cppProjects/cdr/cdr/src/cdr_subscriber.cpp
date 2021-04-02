#include "cdr_subscriber.hpp"

#include <cstdlib> // strol 

namespace advcpp
{
Subscriber::Subscriber() 
: imsi(), msisdn() //usageType(),
, voiceIn(0), voiceOut(0)
, dataIn(0) , dataOut(0)
, smsIn(0), smsOut(0)
{}


void advcpp::Subscriber::AccumulateData(const Subscriber& a_toAdd) 
{
    if (msisdn != a_toAdd.msisdn){
        throw WrongSubscriberException();
    }
    voiceIn+= a_toAdd.voiceIn;
    voiceOut+= a_toAdd.voiceOut;;
    dataIn+= a_toAdd.dataIn;;
    dataOut+= a_toAdd.dataOut;;
    smsIn+= a_toAdd.smsIn;;
    smsOut+= a_toAdd.smsOut;;
}


const char* WrongSubscriberException::what() const throw()
{
    return "Tried to update the wrong Subscriber";
}

} // namespace advcpp
