#include "engine.h"
#include <QDebug>

// Chuẩn bị dữ liệu
void Engine::prepareData() {
    readCodeTable(this->data);
    readStopword(this->data);
}

// Thêm file vào database
void Engine::addFile() {
    char* direct = _strdup(this->addFileDirect);
    this->errorFlag = addFileToMetadata(direct, this->data);
    qDebug() << this->errorFlag << "\n";
    free(direct);
}

// Xóa file trong database
void Engine::delFile() {
    char* direct = _strdup(this->delFileDirect);
    deleteFileFromMetaData(direct);
    free(direct);
    this->updateCount++;
}

// Cập nhật dữ liệu
void Engine::updateData() {
    updateMetaData();
}

// Tìm kiếm
void Engine::searchData() {
    this->result = searchResult(this->userInput, this->data);
    this->nResult = result.size;
}

// Xóa kết quả tìm kiếm đang lưu
void Engine::clearSearchResult() {
    this->nResult = 0;
    this->rank = 0;
    this->userInput = NULL;
    removeResult(this->result);
}

// Kết thúc, free vùng nhớ
void Engine::endProg() {
    freeData(this->data);
}

