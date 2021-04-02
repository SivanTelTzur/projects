#include "create_parser.hpp"
#include "nokia_parser.hpp"
#include "logger.hpp"

#include <exception>

namespace advcpp
{

IParser& CreateParser()
{
    try{
        IParser* create = new NokiaParser();
        return *create;
    }
    catch (std::bad_alloc & a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}

} // namespace advcpp
