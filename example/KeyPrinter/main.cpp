
#include <kbd_monitor/KeyboardMonitor.hpp>

#include <atomic>
#include <csignal>
#include <iostream>
#include <thread>

namespace
{
volatile std::sig_atomic_t signalStatus = 0;

void SignalHandler(const int signal)
{
    signalStatus = signal;
}

void Wait()
{
    using namespace std::chrono_literals;
    constexpr auto waitTime = 1s;
    while (true)
    {
        if (signalStatus)
            break;

        std::this_thread::sleep_for(waitTime);
    }
}

void AssignSignalHandler()
{
    signal(SIGINT, SignalHandler);
    signal(SIGTERM, SignalHandler);
}

} // namespace

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " /dev/input/..." << std::endl;
        return 0;
    }
    AssignSignalHandler();

    kbd_monitor::KeyboardMonitor monitor { argv[1] };
    if (!monitor.Start())
    {
        std::cerr << "Failed to start monitor." << std::endl;
        return 1;
    }

    Wait();

    return 0;
}
