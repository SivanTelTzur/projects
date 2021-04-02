#ifndef AE626169_C557_4A81_B3FC_F7651ACFC4E0_H__
#define AE626169_C557_4A81_B3FC_F7651ACFC4E0_H__


#include <exception>

namespace advcpp
{

class ThreadPoolException: public std::exception{
public:
    virtual const char* what() const throw();
}; 

  

class ThreadPoolClosedForNewTasks: public std::exception{
public:
    virtual const char* what() const throw();
};    
} // namespace advcpp


#endif // AE626169_C557_4A81_B3FC_F7651ACFC4E0_H__