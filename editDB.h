#pragma once
#include "preprocess.h"
#include "./utils/utils.h"

// Thêm file vào data
int addFileToMetadata(char*& direct, AppData& data);

// Xóa file khỏi data
void deleteFileFromMetaData(char*& direct);

// Cập nhật file meta data
void updateMetaData();
