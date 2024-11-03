#include "FileDescriptor.hpp"

#include <unistd.h>

#include <iostream>
#include <ostream>

namespace kbd_monitor::details
{
FileDescriptor::FileDescriptor(FileDescriptor&& other)
{
    std::swap(other._fd, _fd);
}

FileDescriptor& FileDescriptor::operator=(FileDescriptor&& other)
{
    std::swap(other._fd, _fd);
    return *this;
}

FileDescriptor::~FileDescriptor()
{
    std::cerr << "~FileDescriptor()" << std::endl;
    if (!static_cast<bool>(*this))
        return;

    ::close(_fd);
}

FileDescriptor::operator int() const noexcept
{
    return _fd;
}

FileDescriptor::operator bool() const noexcept
{
    return _fd != invalidDescriptor;
}

int FileDescriptor::operator*() const noexcept
{
    return _fd;
}
} // namespace kbd_monitor::details