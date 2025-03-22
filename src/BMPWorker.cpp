#include "BMPWorker.hpp"

#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

BMPWorker::BMPWorker(const std::string& inputFile,
                     const std::string& outputFile) :
    inputFilename(inputFile),
    outputFilename(outputFile)
{}

void BMPWorker::run() {
    loadBMP();
    displayInConsole();
    drawX();
    displayInConsole();
    saveBMP();
}

void BMPWorker::loadBMP() {
    std::ifstream file(inputFilename, std::ios::binary);
    if (!file)
        assert(false);
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
    if (header.bfType != 0x4d42)
        assert(false);
    if (infoHeader.biCompression != 0)
        assert(false);
    if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32)
        assert(false);
    file.seekg(header.bfOffBits, std::ios::beg);
    pixels.resize(infoHeader.biHeight, std::vector<Pixel>(infoHeader.biWidth));
    int bytesPerPixel = infoHeader.biBitCount >> 3;
    int padding = (4 - (infoHeader.biWidth * bytesPerPixel) % 4) % 4;
    for (int i = 0; i < infoHeader.biWidth; ++i) {
        file.read(reinterpret_cast<char*>(pixels[i].data()),
                  infoHeader.biWidth * bytesPerPixel);
        if (infoHeader.biBitCount == 24) {
            for (auto& pixel : pixels[i])
                pixel.alpha = 255;
        }
        file.ignore(padding);
    }
}

void BMPWorker::saveBMP() {
    std::ofstream file(outputFilename, std::ios::binary);
    if (!file)
        assert(false);
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));
    int bytesPerPixel = infoHeader.biBitCount >> 3;
    int padding = (4 - (infoHeader.biWidth * bytesPerPixel) % 4) % 4;
    for (const auto& line : pixels) {
        file.write(reinterpret_cast<const char*>(line.data()),
                   infoHeader.biWidth * bytesPerPixel);
        file.write("\0\0\0\0", padding);
    }
}

void BMPWorker::displayInConsole() {
    for (int i = pixels.size() - 1; i >=0; --i) {
        for (const auto& pixel : pixels[i]) {
            int red = static_cast<int>(pixel.red);
            int green = static_cast<int>(pixel.green);
            int blue = static_cast<int>(pixel.blue);
            if (red == 255 && green == 255 && blue == 255) {
                std::cout << 'w';
            } else {
                std::cout << 'b';
            }
        }
    }
    std::cout << std::endl;
}

// алгоритм Брезенхема
void BMPWorker::drawLine(const std::pair<int, int>& point1,
                         const std::pair<int, int>& point2) {
    auto [x1, y1] = point1;
    auto [x2, y2] = point2;
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int error = dx - dy;
    while (true) {
        if (y1 >= 0 && y1 < static_cast<int>(pixels.size())) {
            if (x1 >= 0 && x1 < static_cast<int>(pixels[0].size())) {
                pixels[y1][x1] = {0, 0, 0, 255};
            }
        }
        if (x1 == x2 && y1 == y2)
            break;
        int error2 = 2 * error;
        if (error2 > -dy) {
            error -= dy;
            x1 += sx;
        }
        if (error2 < dx) {
            error += dx;
            y1 += sy;
        }
    }
}

void BMPWorker::drawX() {
    int lineCnt = pixels.size();
    drawLine({0, 0}, {lineCnt - 1, pixels[lineCnt - 1].size() - 1});
    drawLine({0, pixels[0].size() - 1}, {lineCnt - 1, 0});
}