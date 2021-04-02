#include "files_notifier.hpp"

#include <errno.h>

namespace advcpp
{
FilesNotifier::FilesNotifier(MtWaitingQueue<std::string>& a_filesQueue, const std::string& a_dirName)
: m_filesQueue(a_filesQueue)
, m_dirName(a_dirName)
, m_fd(initFileDescriptor())
, m_wd(addWatch())
{

}

FilesNotifier::~FilesNotifier()
{
    if (-1 == inotify_rm_watch(m_fd, m_wd)){
        assert(!"remove watch failed");//EBADF  || EINVAL
    }
    if (-1 == close(m_fd)){
        assert(errno != EBADF);
        assert(errno != EINTR);
        assert(errno != EIO);
    }
}

void FilesNotifier::Run()
{
    try{
        startRead();
    } catch (const FileNotifierReadFailed& a_ex){
        throw;
    }
}

int FilesNotifier::initFileDescriptor()
{
    int fd  =  inotify_init();
    if (-1 == fd){
        switch (errno) {
            case EMFILE:
                throw FileNotifierEmFile();
            case ENOMEM:
                throw FileNotifierENoMem();
        }
    }
    return fd;
}

int FilesNotifier::addWatch()
{
    int wd  =  inotify_add_watch(m_fd, m_dirName.c_str(), IN_CREATE | IN_MOVED_TO);
    if (-1 == wd){
        switch (errno) {
            case EBADF:
                assert(!"file descriptor can't be valid at this point");
            case EACCES:
            case EFAULT:
            case EEXIST:
            case ENAMETOOLONG:
            case ENOENT:
                throw FileNotifierPathName();
            case ENOMEM:
                throw FileNotifierENoMem();
            case ENOSPC:
                throw FileNotifierEmFile();
        }
    }
    return wd;
}

void FilesNotifier::startRead()
{

    for (; ;){   
        char buffer[BUFFER_SIZE] = {'\0'};
        int nBytes = read(m_fd, buffer, BUFFER_SIZE);
        if (-1 == nBytes){
            throw FileNotifierReadFailed();
        }

        struct inotify_event* event = (struct inotify_event*) buffer;
        if (event->len){
            if (event->mask & IN_CREATE || event->mask & IN_MOVED_TO){
                std::string newFileName = m_dirName + "/" + (event->name);
                m_filesQueue.Enqueue(newFileName);
            }
        }
    }
}

const char* FileNotifierReadFailed::what() const throw()
{
    return "Read failed";
}

const char* FileNotifierPathName::what() const throw()
{
    return "File pathname problem";
}

const char* FileNotifierAccessDenied::what() const throw()
{
    return "File access denied";
}

const char* FileNotifierEmFile::what() const throw()
{
    return "File limits error has ocurred";
}

const char* FileNotifierENoMem::what() const throw()
{
    return "Insufficient kernel memory is available";
}

} // namespace advcpp
