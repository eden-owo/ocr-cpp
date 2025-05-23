#include <opencv2/opencv.hpp>
#include <iostream>
#include "ocr_module.h"
#include "folder.h"

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {

    cout << "Please select img folder." << endl;
    std::string img_folder = ReadFolder();
    cout << "Please select output folder." << endl;
    std::string output_folder = ReadFolder();

    std::vector<std::string> imageFiles = ListFilesInFolder(img_folder);

    //if (argc < 2) {
    //    cout << "Usage: ocr.exe <image.png>\n";
    //    return 0;
    //}

    for (const auto& filePath : imageFiles) {
        std::cout << "Processing: " << filePath << std::endl;

        Mat img = imread(argv[1], IMREAD_UNCHANGED);
        if (img.empty()) {
            cerr << "Cannot read image.\n";
            return -1;
        }

        Mat img_rgba;
        if (img.channels() == 3) {
            cvtColor(img, img_rgba, COLOR_BGR2BGRA);
        }
        else if (img.channels() == 4) {
            img_rgba = img;
        }
        else {
            cerr << "Unsupported image format.\n";
            return -1;
        }

        Img ig = {
            3,
            img_rgba.cols,
            img_rgba.rows,
            0,
            (__int64)img_rgba.step,
            (__int64)img_rgba.data
        };

        run_ocr(ig);
    }
    std::cout << "No images remaining. Processing finished." << std::endl;
    return 0;
}
