#include "FileDescriptor.hpp"

#include <kbd_monitor/KeyboardMonitor.hpp>

#include <array>
#include <iostream>
#include <vector>

extern "C"
{
#include <fcntl.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <unistd.h>
}

namespace
{
constexpr int ev_buf_size { 16 };
}

namespace kbd_monitor
{

KeyboardMonitor::KeyboardMonitor(const std::filesystem::path& keyboardDescriptor)
    : _keyboardDescriptor { keyboardDescriptor }
{
}

bool KeyboardMonitor::Start()
{
    Stop();

    if (!std::filesystem::exists(_keyboardDescriptor))
        return false;

    details::FileDescriptor fd { ::open(std::filesystem::absolute(_keyboardDescriptor).c_str(), O_RDONLY) };
    if (static_cast<int>(fd) < 0)
    {
        std::cerr << "Failed to open " << std::filesystem::absolute(_keyboardDescriptor) << std::endl;
        return false;
    }

    _running = true;
    _thread = std::thread { [this] { Worker(); } };

    return true;
}

void KeyboardMonitor::Stop()
{
    _running = false;
    if (_thread.joinable())
        _thread.join();
}

void KeyboardMonitor::Worker()
{
    std::cout << "KeyboardMonitor::Worker()" << std::endl;

    // ev.value
    static const std::array<std::string, 3> eventsNames { "RELEASED", "PRESSED ", "REPEATED" };

    while (_running)
    {
        const int fd = ::open(std::filesystem::absolute(_keyboardDescriptor).c_str(), O_RDONLY);
        if (fd < 0)
            continue;
        const auto fileDescriptor = details::FileDescriptor { fd };

        while (_running)
        {
            input_event ev {};
            const ssize_t n = ::read(static_cast<int>(fileDescriptor), &ev, sizeof(struct input_event));
            if (n == static_cast<ssize_t>(-1))
            {
                std::cerr << "pressed n == static_cast<ssize_t>(-1): " << n << std::endl;
                break;
            }
            if (n != sizeof(ev))
            {
                std::cerr << "pressed n != sizeof(ev): " << n << std::endl;
                break;
            }

            if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2)
                printf("%s 0x%04x (%d)\n", eventsNames[ev.value].c_str(), static_cast<int>(ev.code),
                       static_cast<int>(ev.code));
        }
    }
}
} // namespace kbd_monitor