#pragma once

namespace kbd_monitor::details
{

class FileDescriptor
{
public:
    explicit FileDescriptor(const int fd)
        : _fd(fd)
    {
    }

    FileDescriptor() = default;
    FileDescriptor(const FileDescriptor&) = delete;
    FileDescriptor& operator=(const FileDescriptor&) = delete;
    FileDescriptor(FileDescriptor&&);
    FileDescriptor& operator=(FileDescriptor&&);
    ~FileDescriptor();

    explicit operator int() const noexcept;
    explicit operator bool() const noexcept;
    int operator*() const noexcept;

private:
    constexpr static int invalidDescriptor { -1 };
    int _fd { invalidDescriptor };
};

} // namespace kbd_monitor::details