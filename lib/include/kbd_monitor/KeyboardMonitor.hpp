#pragma once

#include <atomic>
#include <filesystem>
#include <thread>

namespace kbd_monitor
{

class KeyboardMonitor
{
public:
    explicit KeyboardMonitor(const std::filesystem::path& keyboardDescriptor);

    bool Start();
    void Stop();

    // TODO: add callback

private:
    void Worker();

private:
    std::atomic_bool _running { false };
    std::filesystem::path _keyboardDescriptor;

    std::thread _thread;
};

} // namespace kbd_monitor
