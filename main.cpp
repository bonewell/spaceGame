#include <iostream>

#include "application.hpp"

int main()
try {
    constexpr int kWidth = 640;
    constexpr int kHeight = 480;

    Application{{kWidth, kHeight}}.run();
    std::cout << "Game over!\n";
}
catch (SdlError const& error) {
    std::cout << error.what() << '\n';
    return -1;
}
catch (...) {
    std::cout << "Unknown error\n";
    return -1;
}
