#include <opencv2/opencv.hpp>
#include <iostream>
#include "ocr_module.h"

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: ocr.exe <image.png>\n";
        return 0;
    }

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
    return 0;
}
