#ifndef G8CE4DD1_E0E3_4BAB_A342_1CFAB6AA0B96_H__
#define G8CE4DD1_E0E3_4BAB_A342_1CFAB6AA0B96_H__

#include "cdr_struct.hpp"

namespace advcpp
{

class IParser
{
public:
    virtual ~IParser() = 0;
    virtual void Parser(const std::string& a_line, Cdr& a_cdr) = 0;
};

inline IParser::~IParser()
{
}

} // namespace advcpp

#endif // G8CE4DD1_E0E3_4BAB_A342_1CFAB6AA0B96_H__
