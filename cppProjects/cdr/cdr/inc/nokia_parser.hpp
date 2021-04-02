#ifndef K5B9FCCEC_F3F4_4E04_87A0_A580ED80CDC6_H__
#define K5B9FCCEC_F3F4_4E04_87A0_A580ED80CDC6_H__

#include "parser_interface.hpp"

namespace advcpp
{
class NokiaParser : public IParser{
public:
    NokiaParser();
    ~NokiaParser();
    virtual void Parser(const std::string& a_line, Cdr& a_cdr);

private:
    void tokenizeALine(const std::string& a_line, Cdr& a_cdr);
private:
    const std::string m_delimiter;
};

} // namespace advcpp


#endif // K5B9FCCEC_F3F4_4E04_87A0_A580ED80CDC6_H__
