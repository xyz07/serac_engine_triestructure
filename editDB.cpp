#include "editDB.h"

// Thêm file vào data
// Trả về 0: Thêm thành công
// Trả về 1: File đã được thêm từ trước
// Trả về 2: Lỗi đường dẫn
int addFileToMetadata(char*& direct, AppData& data) {
    trimmed(direct);

    // Check xem đường dẫn có đúng không
    FILE* check = fopen(direct, "r");
    if (!check) return 2;
    else fclose(check);

    // Check xem file đã được thêm vào từ trước chưa
    FILE* fileIndex = fopen("./stored/index.txt", "r");
    char buffer[MAXLEN_DIRECT + 1];
    while (!feof(fileIndex)) {
        fgets(buffer, MAXLEN_DIRECT, fileIndex);
        if (feof(fileIndex)) break;
        lastCharOfFileName(buffer);
        if (strcmp(buffer, direct) == 0) {
            fclose(fileIndex);
            return 1;
        }
    }
    fclose(fileIndex);

    // Thêm file vào index và meta data
    fileIndex = fopen("./stored/index.txt", "a");
    fprintf(fileIndex, "%s\n", direct);
    fclose(fileIndex);

    FILE* meta = fopen("./stored/metaData.txt", "a");
    preprocessFile(direct, data, meta);
    fclose(meta);

    return 0;
}

// Xóa file khỏi data
void deleteFileFromMetaData(char*& direct) {
    trimmed(direct);

    // Tạo lại file index, bỏ qua dòng chứa đường dẫn đến file muốn xóa
    // Tức là ta xóa đường dẫn của file này khỏi file index
    FILE* newFile = fopen("./stored/index(2).txt", "w");
    FILE* originFile = fopen("./stored/index.txt", "r");

    char buffer[MAXLEN_DIRECT + 1];
    while (!feof(originFile)) {
        fgets(buffer, MAXLEN_DIRECT, originFile);
        if (feof(originFile)) break;

        lastCharOfFileName(buffer);
        if (strcmp(buffer, direct) == 0) continue;
        fprintf(newFile, "%s\n", buffer);
    }

    fclose(newFile);
    fclose(originFile);

    remove("./stored/index.txt");
    rename("./stored/index(2).txt", "./stored/index.txt");
}

void updateMetaData() {
    // Loại những file người dùng thêm vào mà đã bị xóa thủ công khỏi máy tính của họ
    FILE* newIndex = fopen("./stored/index(2).txt", "w");
    FILE* originIndex = fopen("./stored/index.txt", "r");
    FILE* fileOfUser = fopen("./stored/fileOfUser.txt","w");
    char* buffer = (char*)malloc(MAXLEN_DIRECT + 1 * sizeof(char));

    FILE* temp;
    while (!feof(originIndex)) {
        // Đọc địa chỉ
        fgets(buffer, MAXLEN_DIRECT, originIndex);
        if (feof(originIndex)) break;
        lastCharOfFileName(buffer);

        // Nếu đây là file mà người dùng đã thêm, kiểm tra xem file này còn tồn tại ở máy người dùng không
        if (buffer[0] != '.') {
            temp = fopen(buffer, "r");
            if (temp) {
                // Nếu file bình thường, ghi đường dẫn ra fileOfUser.txt
                fprintf(fileOfUser, "%s\n", buffer);
                fclose(temp);
            }
            else {
                continue;
            }
        }

        fprintf(newIndex, "%s\n", buffer);
    }
    fclose(newIndex);
    fclose(originIndex);
    fclose(fileOfUser);
    free(buffer);
    buffer = NULL;
    remove("./stored/index.txt");
    rename("./stored/index(2).txt", "./stored/index.txt");

    // Update file meta data
    // Thao tác này sẽ xóa đi nội dung chính của những file không có tên trong file index ra khỏi meta data
    FILE* originMeta = fopen("./stored/metaData.txt", "r");
    FILE* newMeta = fopen("./stored/metaData(2).txt", "w");
    FILE* index = fopen("./stored/index.txt", "r");

    char direct[MAXLEN_DIRECT + 1];
    char label[MAXLEN_DIRECT + 1];
    buffer = (char*)malloc(2001 * sizeof(char));
    while (!feof(originMeta) && !feof(index)) {
        fgets(direct, MAXLEN_DIRECT, originMeta);
        fgets(label, MAXLEN_DIRECT, index);

        // So sánh đường dẫn trong file meta và file index
        while (strcmp(direct, label) != 0) {
            skipLine(originMeta);
            fgets(direct, MAXLEN_DIRECT, originMeta);
            if (feof(originMeta)) break;
        }

        if (feof(originMeta)) break;

        // Copy nội dung chính của file này sang file meta data mới
        fprintf(newMeta, "%s", direct);
        do {
            fgets(buffer, 2000, originMeta);
            fprintf(newMeta, "%s", buffer);
            if (buffer[strlen(buffer) - 1] == '\n') break;
        } while (1);
    }
    free(buffer);
    buffer = NULL;

    fclose(newMeta);
    fclose(originMeta);
    fclose(index);

    // Xóa file meta cũ
    remove("./stored/metaData.txt");
    rename("./stored/metaData(2).txt", "./stored/metaData.txt");
}
