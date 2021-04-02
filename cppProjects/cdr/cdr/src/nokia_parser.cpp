#include  "nokia_parser.hpp"
#include "assert.h"

namespace advcpp
{

NokiaParser::NokiaParser()
: m_delimiter("|")
{
}

NokiaParser::~NokiaParser()
{
}

void NokiaParser::Parser(const std::string& a_line, Cdr& a_cdr)
{
    tokenizeALine(a_line, a_cdr);
}

void NokiaParser::tokenizeALine(const std::string& a_line, Cdr& a_cdr)
{
    //ignored the first token
    size_t pos = a_line.find_first_of(m_delimiter);

    size_t start = pos + 1;
    pos = a_line.find_first_of(m_delimiter, pos + 1);
    size_t i = 0;

    while (pos != std::string::npos && i < sizeof(a_cdr.pValues) / sizeof(a_cdr.pValues[0])){ //TODO CHANGE HARD CODED

        if (start == pos){
            *a_cdr.pValues[i++] = "0"; 
        }else{
            a_cdr.pValues[i++]->assign(a_line.begin() + start, a_line.begin() + pos);
        }
        start = pos + 1;
        pos = a_line.find_first_of(m_delimiter, pos + 1);
    }
    if(start >= a_line.size()){
        *a_cdr.pValues[i++] = "0";
        return;
    }
    a_cdr.pValues[i]->assign(a_line.begin() + start, a_line.end());

}



} // namespace advcpp
