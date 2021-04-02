#include "reader.hpp"
#include "create_parser.hpp"


#include <iostream>
#include <exception>
#include <stdio.h> //rename


namespace advcpp
{

Reader::Reader(MtWaitingQueue<Cdr>& a_cdrQueue, const std::string& a_source)
: m_queue(a_cdrQueue)
, m_fileName(a_source)
, m_fp(m_fileName.c_str(), std::ifstream::in)
{
    std::cerr << "reader ctor  "<<m_fileName << "****"<< std::endl;
    if (!m_fp.is_open()){
        std::cerr << " fail to open file" << std::endl;
    }

}

Reader::~Reader()
{
    m_fp.close();
    moveToDone();
}

void Reader::Start()
{
    IParser& parser = CreateParser();
    // ignore the first header line
    std::string line;
    std::getline(m_fp, line);

    while (!m_fp.eof())
    {
        std::getline(m_fp, line);

        if (!line.size()){
            break;
        }

        Cdr cdr;
        parser.Parser(line, cdr);
        m_queue.Enqueue(cdr);
    }
    delete &parser;
}



void Reader::moveToDone()
{
    size_t pos = m_fileName.find_last_of("/");
    std::string newFile = "./files/done/" + m_fileName.substr(pos + 1);
    std::cerr << newFile << std::endl;
    int result = rename(m_fileName.c_str(), newFile.c_str());
    if (result){
        perror("unable to remove file ");
    }
}

} // namespace advcpp
