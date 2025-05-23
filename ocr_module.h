#pragma once
#include <Windows.h>
#include <opencv2/opencv.hpp>

typedef struct {
    __int32 t, col, row, _unk;
    __int64 step, data_ptr;
} Img;

void run_ocr(const Img& img);