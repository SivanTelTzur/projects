#ifndef H50D3FDE_8E02_485B_B3FB_6DB382FFDEB6_H__
#define H50D3FDE_8E02_485B_B3FB_6DB382FFDEB6_H__

#include "cdr_struct.hpp"

namespace advcpp
{
namespace cdr_protocol
{
    void Pack(char* a_buffer, const Cdr& a_cdrToSend);
    void Unpack(const std::string& a_buffer, Cdr& a_cdrToFill);

} // cdr_protocol

}//advcpp

#endif // H50D3FDE_8E02_485B_B3FB_6DB382FFDEB6_H__
