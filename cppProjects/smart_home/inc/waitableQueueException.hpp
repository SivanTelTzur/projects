#ifndef WAITABLEQUEUEEXCEPTION_HPP_
#define WAITABLEQUEUEEXCEPTION_HPP_

#include <exception>

namespace advcpp{

class QueueFinishException: public std::exception{
public:
    virtual const char* what() const throw();
};

} // advcpp

#endif //WAITABLEQUEUEEXCEPTION_HPP_