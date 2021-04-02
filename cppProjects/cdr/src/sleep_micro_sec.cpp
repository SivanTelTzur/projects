#include "sleep_micro_sec.hpp"

#include <unistd.h> //usleep

namespace advcpp
{
void SleepMicroSeconds(unsigned int a_microSeconds) 
{
    useconds_t givenTime = a_microSeconds;
    usleep(givenTime);    
}

}//advcpp
