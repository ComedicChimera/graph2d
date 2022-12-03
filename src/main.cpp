#include <iostream>
#include <string>
#include <stdexcept>

#include "parser.hpp"
#include "renderer.hpp"

int main(int argc, char* argv[]) {
    std::string input;
    for (int i = 1; i < argc; i++) {
        if (i > 1)
            input.push_back(' ');

        input += argv[i];
    }

    try {
        Parser parser(input);
        auto expr = parser.parse();

        initRenderer(expr, 800, 800);

        runRenderLoop();
    } catch (ConfigError& cexc) {
        std::cout << cexc.what() << '\n';  
    } catch (std::exception& exc) {
        std::cout << exc.what() << '\n';
    } 
    
    return 0;
}
