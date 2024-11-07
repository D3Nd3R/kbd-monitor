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
    : _keyboardDescriptor { std::filesystem::absolute(keyboardDescriptor) }
{
}

bool KeyboardMonitor::Start()
{
    Stop();

    if (!std::filesystem::exists(_keyboardDescriptor))
    {
        std::cerr << "Could not open keyboard device " << _keyboardDescriptor << std::endl;
        return false;
    }

    if (!_callback)
    {
        std::cerr << "Callback for device " << _keyboardDescriptor << " not set" << std::endl;
        return false;
    }

    if (const details::FileDescriptor fd { ::open(_keyboardDescriptor.c_str(), O_RDONLY) }; static_cast<int>(fd) < 0)
    {
        std::cerr << "Failed to open " << _keyboardDescriptor << std::endl;
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

void KeyboardMonitor::RegisterEventCallback(std::function<void(Event&&)>&& callback)
{
    if (_running)
    {
        std::cerr << "Unable to register event callback after running" << std::endl;
        return;
    }

    _callback = std::move(callback);
}

void KeyboardMonitor::Worker()
{
    std::cout << "KeyboardMonitor::Worker()" << std::endl;

    while (_running)
    {
        const int fd = ::open(_keyboardDescriptor.c_str(), O_RDONLY);
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

            if (_callback && ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2)
                _callback(Event { EventType { ev.value }, ev.code });
        }
    }
}
} // namespace kbd_monitor