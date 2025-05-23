#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string.h>
using namespace cv;
using namespace std;

typedef struct {
    __int32 t, col, row, _unk;
    __int64 step, data_ptr;
} Img;

// DLL OCR 函式型別定義
typedef __int64(__cdecl* CreateOcrInitOptions_t)(__int64*);
typedef __int64(__cdecl* OcrInitOptionsSetUseModelDelayLoad_t)(__int64, char);
typedef __int64(__cdecl* CreateOcrPipeline_t)(__int64, __int64, __int64, __int64*);
typedef __int64(__cdecl* CreateOcrProcessOptions_t)(__int64*);
typedef __int64(__cdecl* OcrProcessOptionsSetMaxRecognitionLineCount_t)(__int64, __int64);
typedef __int64(__cdecl* RunOcrPipeline_t)(__int64, Img*, __int64, __int64*);
typedef __int64(__cdecl* GetOcrLineCount_t)(__int64, __int64*);
typedef __int64(__cdecl* GetOcrLine_t)(__int64, __int64, __int64*);
typedef __int64(__cdecl* GetOcrLineContent_t)(__int64, __int64*);

void ocr(Img img) {
    HINSTANCE hDLL = LoadLibraryA("ocr.dll");
    if (!hDLL) { cerr << "Failed to load DLL\n"; return; }

    // 函式指標
    auto CreateOcrInitOptions = (CreateOcrInitOptions_t)GetProcAddress(hDLL, "CreateOcrInitOptions");
    auto OcrInitOptionsSetUseModelDelayLoad = (OcrInitOptionsSetUseModelDelayLoad_t)GetProcAddress(hDLL, "OcrInitOptionsSetUseModelDelayLoad");
    auto CreateOcrPipeline = (CreateOcrPipeline_t)GetProcAddress(hDLL, "CreateOcrPipeline");
    auto CreateOcrProcessOptions = (CreateOcrProcessOptions_t)GetProcAddress(hDLL, "CreateOcrProcessOptions");
    auto OcrProcessOptionsSetMaxRecognitionLineCount = (OcrProcessOptionsSetMaxRecognitionLineCount_t)GetProcAddress(hDLL, "OcrProcessOptionsSetMaxRecognitionLineCount");
    auto RunOcrPipeline = (RunOcrPipeline_t)GetProcAddress(hDLL, "RunOcrPipeline");
    auto GetOcrLineCount = (GetOcrLineCount_t)GetProcAddress(hDLL, "GetOcrLineCount");
    auto GetOcrLine = (GetOcrLine_t)GetProcAddress(hDLL, "GetOcrLine");
    auto GetOcrLineContent = (GetOcrLineContent_t)GetProcAddress(hDLL, "GetOcrLineContent");

    __int64 ctx = 0, pipeline = 0, opt = 0, instance = 0;
    CreateOcrInitOptions(&ctx);
    OcrInitOptionsSetUseModelDelayLoad(ctx, 0);

    std::string key_str = "";
    std::cout << "請輸入 OCR 金鑰: ";
    std::cin >> key_str;
    const char* key = key_str.c_str();

    CreateOcrPipeline((__int64)"ocr.model", (__int64)key, ctx, &pipeline);

    CreateOcrProcessOptions(&opt);
    OcrProcessOptionsSetMaxRecognitionLineCount(opt, 1000);
    RunOcrPipeline(pipeline, &img, opt, &instance);

    __int64 lineCount;
    GetOcrLineCount(instance, &lineCount);
    printf("Recognized %lld lines:\n", lineCount);

    for (__int64 i = 0; i < lineCount; ++i) {
        __int64 line = 0, content = 0;
        GetOcrLine(instance, i, &line);
        GetOcrLineContent(line, &content);
        printf("%lld: %s\n", i, (char*)content);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: ocr.exe <image.png>\n";
        return 0;
    }


    Mat img = imread(argv[1], IMREAD_UNCHANGED);
    if (img.empty()) { cerr << "Cannot read image.\n"; return -1; }

    Mat img_rgba;
    if (img.channels() == 3) cvtColor(img, img_rgba, COLOR_BGR2BGRA);
    else if (img.channels() == 4) img_rgba = img;
    else { cerr << "Unsupported image format.\n"; return -1; }

    Img ig = {
        3,
        img_rgba.cols,
        img_rgba.rows,
        0,
        (__int64)img_rgba.step,
        (__int64)img_rgba.data
    };

    ocr(ig);
    return 0;
}
