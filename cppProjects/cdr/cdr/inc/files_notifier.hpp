#ifndef AF5BD7DF_E9EC_4BB3_A583_863F05CB66A2_H__
#define AF5BD7DF_E9EC_4BB3_A583_863F05CB66A2_H__

#include "waiting_queue.hpp"
#include "noncopyable.hpp"

#include <string>
#include <exception>
#include <sys/inotify.h> // struct inotify_event
#include <unistd.h> //close
#include <limits.h> // NAME_MAX

namespace advcpp
{
class FilesNotifier : private NonCopyable
{
public:
    static const size_t EVENT_SIZE = sizeof(struct inotify_event);
    static const size_t BUFFER_SIZE = EVENT_SIZE + NAME_MAX + 1;

    FilesNotifier(MtWaitingQueue<std::string>& a_filesQueue, const std::string& a_dirName);
    ~FilesNotifier();

    void Run();

private:
    int initFileDescriptor(); //returns file descriptor
    int addWatch();
    void startRead();
    
private:
    MtWaitingQueue<std::string>& m_filesQueue;
    const std::string m_dirName;
    int m_fd;
    int m_wd;
};

class FileNotifierReadFailed : public std::exception {
public:
    virtual const char* what() const throw();
};


class FileNotifierPathName : public std::exception {
public:
    virtual const char* what() const throw();
};

class FileNotifierAccessDenied : public std::exception {
public:
    virtual const char* what() const throw();
};

class FileNotifierEmFile : public std::exception {
public:
    virtual const char* what() const throw();
};

class FileNotifierENoMem : public std::exception {
public:
    virtual const char* what() const throw();
};

} // namespace advcpp


#endif // AF5BD7DF_E9EC_4BB3_A583_863F05CB66A2_H__
