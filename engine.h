#pragma once
#include "../src/preprocess.h"
#include "../src/editDB.h"
#include "../src/search.h"

class Engine {
public:
    AppData data;   // Lưu các dữ liệu cần thiết

    int errorFlag;  // Lưu lại mã lỗi khi có lỗi xảy ra

    wchar_t* userInput = NULL;
    List result;
    int nResult;
    int threshold = 20;
    int rank = 0;

    char* addFileDirect = NULL;
    char* delFileDirect = NULL;
    int updateCount = 0;

    // Chuẩn bị dữ liệu
    void prepareData();

    // Thêm file vào database
    void addFile();

    // Xóa file khỏi database
    void delFile();

    // Cập nhật dữ liệu
    void updateData();

    // Tìm kiếm
    void searchData();

    // Xóa kết quả tìm kiếm đang lưu
    void clearSearchResult();

    // Giải phóng bộ nhớ
    void endProg();
};
