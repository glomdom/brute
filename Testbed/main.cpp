#include <iostream>

#include <brute.hpp>

int main() {
    try {
        Config config;
        config.windowTitle = "Brute Engine: Testbed";

        Brute engine(config);
        engine.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}