#ifndef CONDITIONALVARIABLEEXCEPTION_HPP_
#define CONDITIONALVARIABLEEXCEPTION_HPP_

#include <exception>

namespace advcpp{

class PthreadContInitFailed: public std::exception{
public:
    virtual const char* what() const throw();
};

class PthreadContDestroyFailed: public std::exception{
public:
    virtual const char* what() const throw();
};

class PthreadContWaitFailed: public std::exception{
public:
    virtual const char* what() const throw() ;
};

} // advcpp

#endif //CONDITIONALVARIABLEEXCEPTION_HPP_