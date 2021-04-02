#include "threadpool_exceptions.hpp"


namespace advcpp{

const char* ThreadPoolException::what() const throw() { return "queue finished" ;}

const char* ThreadPoolClosedForNewTasks::what() const throw() { return "Threadpool Is closed " ;}

} // advcpp