#include <iostream>

#include "BMPWorker.hpp"

int main() {
    std::cout << ">> Enter input BMP file name:\t";
    std::string inputFilename;
    std::cin >> inputFilename;
    std::cout << ">> Enter output BMP file name:\t";
    std::string outputFilename;
    std::cin >> outputFilename;
    BMPWorker bmpWorker(inputFilename, outputFilename);
    bmpWorker.run();
}