#include "search.h"

int Max(int a, int b) {
    return a > b ? a : b;
}

// Tính trọng số của 1 từ
int calWeight(char* s) {
    int nWord = 1;
    int len = strlen(s);
    for (int i = 0; i < len; i++) {
        if (s[i] == ' ') nWord++;
    }
    if (nWord == 1) return 1;
    if (nWord == 2) return 100;
    return 1000;
}

// Tính index trong quá trình chạy thuật toán radix sort
int calIndex(void* p, int pow) {
    return ((*(StringCount**)p)->count / pow) % 10;
}

// Sắp xếp kết quả tìm kiếm theo score
void sortResult(List& result, int maxScore) {
    short D = 0;
    while (maxScore) {
        maxScore /= 10;
        D++;
    }
    radixSort(result, calIndex, D);
}

// Xóa kết quả tìm kiếm
void removeResult(List& list) {
    Node* p = list.head;
    while (p) {
        free((*(StringCount**)p->data)->str);
        removeHead(list);
        p = list.head;
    }
}

// Trả về kết quả tìm kiếm cho chuỗi input
List searchResult(wchar_t*& input, AppData& data) {
    trimmed(input);

    // Tạo list các file có liên quan (score > 0)
    List result;
    init(result, sizeof(StringCount**));

    List searchContent = preprocessString(input, data);
    if (isEmpty(searchContent)) return result;

    FILE* dataFile = fopen("./stored/metaData.txt", "r");
    FILE* indexFile = fopen("./stored/index.txt", "r");

    char* direct = (char*)malloc(MAXLEN_DIRECT + 1 * sizeof(char));
    char* label = (char*)malloc(MAXLEN_DIRECT + 1 * sizeof(char));

    int maxScore = 0;
    bool readNextDirect = true;
    char endline;
    do {
        // Đọc đường dẫn của 2 bên để đối chiếu
        fgets(direct, MAXLEN_DIRECT, dataFile);
        if (readNextDirect) fgets(label, MAXLEN_DIRECT, indexFile);
        if (feof(dataFile) || feof(indexFile)) break;

        // Nếu giống nhau --> file này chưa bị xóa
        if (strcmp(direct, label) == 0) {
            int score = 0;
            char buffer[100];
            int count;
            Node* p = searchContent.head;
            char* temp = (*(StringCount**)p->data)->str;

            // Đọc phần nội dung (nằm trên 1 dòng) và tính score
            while (fscanf(dataFile, "#%[^#]#%d", buffer, &count)) {
                if (!p) continue;

                while (strcmp(temp, buffer) == -1) {
                    p = p->next;
                    if (!p) break;
                    temp = (*(StringCount**)p->data)->str;
                }

                if (strcmp(temp, buffer) == 0) {
                    int weight = calWeight(buffer) * count;
                    score += weight;
                    p = p->next;
                    if (!p) continue;
                    temp = (*((StringCount**)p->data))->str;
                }
                continue;
            }
            endline = fgetc(dataFile);

            if (score > 0) {
                StringCount* tmp = (StringCount*)malloc(1 * sizeof(StringCount));
                lastCharOfFileName(direct);
                tmp->str = _strdup(direct);
                tmp->count = score;
                addTail(result, &tmp);
                maxScore = Max(maxScore, score);
            }

            readNextDirect = true;
        }
        // Nếu tên khác nhau (file bên meta data đã bị xóa), bỏ qua phần nội dung của file hiện tại trong metaData
        else {
            readNextDirect = false;
            skipLine(dataFile);
        }
    } while (1);

    // Sắp xếp theo score
    if (!isEmpty(result)) {
        sortResult(result, maxScore);
    }

    // Giải phóng skip list
    Node* p = searchContent.head;
    while (p) {
        free((*(StringCount**)p->data)->str);
        removeHead(searchContent);
        p = searchContent.head;
    }

    free(direct);
    free(label);
    direct = label = NULL;

    fclose(dataFile);
    fclose(indexFile);

    return result;
}
