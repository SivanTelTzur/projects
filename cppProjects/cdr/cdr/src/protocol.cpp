#include "protocol.hpp"

#include <string>
#include <iostream>

namespace advcpp
{   
namespace cdr_protocol
{

void Pack(char* a_buffer, const Cdr& a_cdrToSend)
{    
    for (size_t i = 0; i < sizeof(a_cdrToSend.pValues) / sizeof(char*) ; ++i){
        *a_buffer++ = '|';//a_cdrToSend.pValues[i]->size();
        a_buffer += (a_cdrToSend.pValues[i]->copy(a_buffer, a_cdrToSend.pValues[i]->size(), 0));
    }
    *a_buffer = '\0';
    //std::cerr << a_buffer << std::endl;
}

void Unpack(const std::string& a_buffer, Cdr& a_cdrToFill)
{
    char delimiter = '|';
    size_t pos = a_buffer.find_first_of(delimiter);

    size_t start = pos + 1;
    size_t i = 0;
    pos = a_buffer.find_first_of(delimiter, pos + 1);

    while (pos != std::string::npos && i < sizeof(a_cdrToFill.pValues) / sizeof(a_cdrToFill.pValues[0])){ //TODO CHANGE HARD CODED
        a_cdrToFill.pValues[i++]->assign(a_buffer.begin() + start, a_buffer.begin() + pos);

        start = pos + 1;
        pos = a_buffer.find_first_of(delimiter, pos + 1);
    }

    a_cdrToFill.pValues[i]->assign(a_buffer.begin() + start, a_buffer.end());
}    


} // cdr_protocol

} //advcpp



//for (size_t i = 0; i < sizeof(a_cdrToFill.pValues) / sizeof(a_cdrToFill.pValues[0]) ; ++i){
    //    
    //   // int len = *a_buffer++;
    //   // a_cdrToFill.pValues[i]->append(a_buffer, len);
    //   // a_buffer += len;
    //    //std::cerr << "***" <<*a_cdrToFill.pValues[i] << "###"<< std::endl;
    //}

//void Protocol::Pack(char* a_buffer, Cdr& a_cdrToSend) const
//{
//    char * bufferInx = a_buffer;
//    for (size_t i = 0; i < sizeof(a_cdrToSend.pValues) / sizeof(a_cdrToSend.pValues[0]) ; ++i){
//        size_t len = a_cdrToSend.pValues[i]->size();
//        *bufferInx = len;
//        ++bufferInx;
//        bufferInx += (a_cdrToSend.pValues[i]->copy(bufferInx, len, 0));
//    }
//    *bufferInx = '\0';
//    //std::cerr << a_buffer << std::endl;
//}

//void Protocol::Unpack(const char* a_buffer, Cdr& a_cdrToFill) const
//{
//    for (size_t i = 0; i < sizeof(a_cdrToFill.pValues) / sizeof(a_cdrToFill.pValues[0]) ; ++i){
//        size_t len = *a_buffer + 1;
//        ++a_buffer;
//        a_cdrToFill.pValues[i]->assign(a_buffer, len);
//        a_buffer += (len - 1);
//        //std::cerr << "***" <<*a_cdrToFill.pValues[i] << "###"<< std::endl;
//    }
//}
