#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class BMPWorker {
public:
    BMPWorker() = delete;
    BMPWorker(const BMPWorker& other) = delete;
    BMPWorker(const std::string& inputFile, const std::string& outputFile);
    ~BMPWorker() = default;
    BMPWorker operator=(const BMPWorker& other) = delete;
    
    void run();

private:
    struct Pixel {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t alpha;
    };

    // устанавливает выравнивание между полями структуры в 1 байт
#pragma pack(push, 1)
    struct BMPHeader {
        uint16_t bfType;
        uint32_t bfSize;
        uint16_t bfReserved1;
        uint16_t bfReserved2;
        uint32_t bfOffBits;
    };
    struct BMPInfoHeader {
        uint32_t biSize;
        int32_t biWidth;
        int32_t biHeight;
        uint16_t biPlanes;
        uint16_t biBitCount;
        uint32_t biCompression;
        uint32_t biSizeImage;
        int32_t biXPelsPerMeter;
        int32_t biYPelsPerMeter;
        uint32_t biClrUsed;
        uint32_t biClrImportant;
    };
#pragma pack(pop)

    std::vector<std::vector<Pixel>> pixels;
    const std::string inputFilename;
    const std::string outputFilename;
    BMPHeader header;
    BMPInfoHeader infoHeader;
    void loadBMP();
    void saveBMP();
    void displayInConsole();
    void drawLine(const std::pair<int, int>& point1,
                  const std::pair<int, int>& point2);
    void drawX();
};