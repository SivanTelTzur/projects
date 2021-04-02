#include "../inc/conditionalVariableException.hpp"

namespace advcpp{

const char* PthreadContInitFailed::what() const throw() {return "Pthread Cont Init Failed" ; }

const char* PthreadContDestroyFailed::what() const throw() {return "Pthread Cont Destroy Failed" ; }

const char* PthreadContWaitFailed::what() const throw() {return "Pthread Cont wait Failed" ; }

} //advcpp