#include <iostream>

#include <brute/brute.hpp>

int main() {
    try {
        Config config;
        config.windowTitle = "Brute Engine: Testbed";

        Brute engine(config);

        auto future_texture = engine.load_texture_async("sigma.png");

        engine.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
