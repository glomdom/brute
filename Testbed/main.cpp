#include <iostream>

#include <brute/brute.hpp>

int main() {
    try {
        Config config;
        config.windowTitle = "Brute Engine: Testbed";

        Brute engine(config);
        engine.run();

        // FIXME: wont work until dynamic resource management has been implemented!!
        //        but atleast it reads images properly :D
        auto texture = engine.load_texture("sigma.png");
        if (texture) {
            std::cout << "texture loaded: width=" << texture->get_width() << " height=" << texture->get_height() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}