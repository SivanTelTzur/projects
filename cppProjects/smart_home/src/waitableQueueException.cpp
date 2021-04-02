#include "waitableQueueException.hpp"
///home/stas/pikachu/inc/threadException.hpp

namespace advcpp{

const char* QueueFinishException::what() const throw() { return "queue finished" ;}

} // advcpp