#pragma once

#include <atomic>
#include <filesystem>
#include <functional>
#include <thread>

namespace kbd_monitor
{

class KeyboardMonitor
{

public:
    enum EventType : int
    {
        None = -1,
        Release = 0,
        Press = 1,
        Repeat = 2,
    };

    struct Event
    {
        EventType type { None };
        int code { -1 };
    };

    using KeyEventCallback = std::function<void(Event&&)>;

public:
    explicit KeyboardMonitor(const std::filesystem::path& keyboardDescriptor);

    bool Start();
    void Stop();

    /// \brief Set callback to handle key event from keyboard
    /// \details Call before Start();
    /// @param callback
    void RegisterEventCallback(std::function<void(Event&&)>&& callback);

    // TODO: add callback

private:
    void Worker();

private:
    std::atomic_bool _running { false };
    std::filesystem::path _keyboardDescriptor;

    std::thread _thread;

    KeyEventCallback _callback { nullptr };
};

} // namespace kbd_monitor
