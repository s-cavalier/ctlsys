#include "IOLayer.hpp"
#include <iostream>
#include <linux/input.h>
#include <iomanip>
#include <chrono>
#include <thread>

int main() {
    try {
        // Adjust to your actual keyboard event device (e.g. /dev/input/event3)
        const char* device = "/dev/input/event3";
        IOLayer::InputStream in(device);

        std::cout << "Listening for keyboard events from " << device << "...\n";
        std::cout << "Press Ctrl+C to stop.\n\n";

        input_event ev{};
        while (true) {
            if (in >> ev) {  // read one input_event
                if (ev.type == EV_KEY) {
                    std::cout << "KEY " << std::setw(3) << ev.code
                              << " state: " << (ev.value ? "pressed" : "released")
                              << " time: " << ev.time.tv_sec << "." << ev.time.tv_usec
                              << "\n";
                    std::cout.flush();
                }
            } else {
                // Sleep briefly to avoid busy waiting when no data
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                in.clear(); // clear eof/fail bits for next read
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
