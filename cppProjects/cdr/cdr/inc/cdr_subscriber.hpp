#ifndef D3C7CA91_5E1B_480D_9789_4E5C89BB8153_H__
#define D3C7CA91_5E1B_480D_9789_4E5C89BB8153_H__

#include "cdr_struct.hpp"

#include <exception>
#include <string> //string, size_t

namespace advcpp
{
struct Subscriber
{
    Subscriber();

    void AccumulateData(const Subscriber& a_toAdd);
    std::string imsi;
    std::string msisdn;
    //std::string usageType;
    long voiceIn;
    long voiceOut;
    long dataIn;
    long dataOut;
    long smsIn;
    long smsOut;
};


class WrongSubscriberException : public std::exception{
public:
    virtual const char* what() const throw();
};

}// namespace advcpp

#endif // D3C7CA91_5E1B_480D_9789_4E5C89BB8153_H__


