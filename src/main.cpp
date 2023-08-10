#include <iostream>
#include <fstream>
#include <png++/png.hpp>
#include <cmath>
#include <string>
#include "../include/Renderer.hpp"
#include "utils/DisplayType.hpp"

#define WIDTH 800
#define HEIGHT 800

static void printHelp(){
	std::cout << "-s --script\t path of the configuration script" << std::endl; 
    std::cout << "-l --live\t live rendering" << std::endl;
	std::cout << "-h --help\t print help" << std::endl;
}

int main(int argc, char *argv[]) {
    std::string scriptPath = "";
    DisplayType displayType = DisplayType::Png;

    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if ((arg == "-h") || (arg == "--help")) {
                printHelp();
                return 0;
            } else if ((arg == "-s") || (arg == "--script")) {
                if (i + 1 < argc) {
                    scriptPath = argv[i+1];
                }
            } else if ((arg == "-l") || (arg == "--live")) {
                displayType = DisplayType::Live;
            }
        }
    }
    if (scriptPath == "") {
        std::cerr << "Please provide a job path" << std::endl;
        exit(1);
    }


    Renderer renderer;
    renderer.init(scriptPath, displayType);
    renderer.run();
    return 0;
}