#include "preprocess.h"

// Tạo file index chứa tên tất cả các file txt trong database
// Chỉ xét đường dẫn không dấu
void createIndexs() {
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
    FILE* fileIndex = fopen("./stored/index.txt", "w");

    printf("Nhap duong dan den cac file van ban:\n(Ket thuc bang tu \"END\")\n");

    char buffer[MAXLEN_DIRECT + 1];
    wchar_t direct[MAXLEN_DIRECT + 1];
    do {
        // buffer là đường dẫn đến folder
        fgets(buffer, MAXLEN_DIRECT, stdin);
        if (!lastCharOfFileName(buffer)) continue;
        if (strlen(buffer) == 0) continue;
        
        if (strcmp(buffer, "END") == 0) break;

        // chuyển buffer thành wchar_t để dùng các hàm FindFirstFile, FindNextFile
        swprintf(direct, MAXLEN_DIRECT, L"%hs/*.txt", buffer);

        strcat(buffer, "/");

        hFind = FindFirstFile(direct, &FindFileData);
        if (hFind == INVALID_HANDLE_VALUE) continue;
        else {
            char temp[MAXLEN_DIRECT + 1];
            wcstombs(temp, FindFileData.cFileName, MAXLEN_DIRECT);
            fprintf(fileIndex, "%s%s\n", buffer, temp);

            while (FindNextFile(hFind, &FindFileData) != 0) {
                if (wcscmp(FindFileData.cFileName, L"index.txt") == 0) break;
                wcstombs(temp, FindFileData.cFileName, MAXLEN_DIRECT);
                fprintf(fileIndex, "%s%s\n", buffer, temp);
            }

            FindClose(hFind);
        }
        hFind = INVALID_HANDLE_VALUE;

    } while (true);

    fclose(fileIndex);
}

// Đọc những dữ liệu cần thiết để xử lý chữ tiếng Việt
void readCodeTable(AppData& data) {
    wchar_t unicode[] = L"./code table/unicode.txt";
    char utf8[] = "./code table/utf8.txt";

    data.UniFull = (wchar_t*)malloc(134 * sizeof(wchar_t));
    data.UniLower = (wchar_t*)malloc(134 * sizeof(wchar_t));
    data.UniMin = (wchar_t*)malloc(67 * sizeof(wchar_t));
    data.Utf8 = (char*)malloc(67 * sizeof(char));

    // Đọc bảng unicode
    FILE* file = _wfopen(unicode, L"r,ccs=UTF-16LE");
    if (!file) return;

    wchar_t c;
    int cnt = 0;
    while (!feof(file)) {
        c = fgetwc(file);
        if (c == L'\n') break;
        data.UniFull[cnt++] = c;
    }
    cnt = 0;
    while (!feof(file)) {
        c = fgetwc(file);
        if (c == L'\n') break;
        data.UniLower[cnt++] = c;
    }
    cnt = 0;
    while (!feof(file)) {
        c = fgetwc(file);
        if (feof(file) || c == L'\n') break;
        data.UniMin[cnt++] = c;
    }
    fclose(file);

    // Đọc bảng utf8
    file = fopen(utf8, "r");
    if (!file) return;

    cnt = 0;
    char _c;
    while (!feof(file)) {
        _c = fgetc(file);
        if (feof(file)) break;
        data.Utf8[cnt++] = _c;
    }
    fclose(file);
}

// Đọc file chứa các stopword
void readStopword(AppData& data) {
    wchar_t stopwordFile[] = L"./stopwords/stopword.txt";

    data.Stopwords = NULL;
    data.nStopword = 0;

    FILE* file = _wfopen(stopwordFile, L"r,ccs=UTF-16LE");
    if (!file) return;

    wchar_t buffer[51];

    while (!feof(file)) {
        fgetws(buffer, 50, file);
        int len = wcslen(buffer);
        if (feof(file) || (len > 0 && buffer[0] != L'\n')) {
            if (buffer[len - 1] == L'\n') {
                buffer[len - 1] = L'\0';
                len--;
            }

            data.nStopword++;
            data.Stopwords = (wchar_t**)realloc(data.Stopwords, data.nStopword * sizeof(wchar_t*));
            data.Stopwords[data.nStopword - 1] = _wcsdup(buffer);
            if (feof(file)) break;
        }
    }

    fclose(file);
}

// Tách chuỗi đầu vào thành các token
void getTokens(wchar_t**& tokens, int& nToken, wchar_t*& rawData, AppData& data) {
    wchar_t delim[] = L"\r\n !\"#&\'()*+,-./:;<=>?[\\]^_`{|}~"; // Không có @, đề phòng file có chứa email, không có %, $ vì nó liên quan đến đơn vị
    wchar_t* tok = wcstok(rawData, delim);
    while (tok) {
        toLower(tok, data.UniFull, data.UniLower);
        nToken++;
        tokens = (wchar_t**)realloc(tokens, nToken * sizeof(wchar_t*));
        tokens[nToken - 1] = _wcsdup(tok);
        tok = wcstok(NULL, delim);
    }
}

// Loại bỏ stopword từ mảng các token
void deleteStopword(wchar_t**& tokens, int& nToken, AppData& data) {
    wchar_t* temp;
    int lenTemp;

    wchar_t space[] = L" ";
    for (int i = 0; i < nToken; ) {
        temp = (wchar_t*)malloc(1 * sizeof(wchar_t));
        temp[0] = L'\0';
        lenTemp = 0;

        // Xác định các cụm từ và xét xem đó có phải là stopword không
        for (int j = 0; j <= 4; j++) {
            if (i + j >= nToken) break;
            addWstring(temp, lenTemp, tokens[i + j]);
            addWstring(temp, lenTemp, space);
        }
        temp[lenTemp - 1] = L'\0';

        int flag = -1;
        for (int j = 4; j >= 0; j--) {
            if (i + j >= nToken) continue;
            if (isStopWord(temp, data.Stopwords, data.nStopword)) {
                flag = j;
                break;
            }
            while (lenTemp > 0 && temp[lenTemp - 1] != L' ') lenTemp--;
            if (lenTemp == 0) break;
            temp[lenTemp - 1] = L'\0';
        }

        if (flag != -1) {
            for (int j = i; j <= i + flag; j++) {
                free(tokens[j]);
                tokens[j] = NULL;
            }
            i = i + flag + 1;
        }
        else i++;

        free(temp);
        temp = NULL;
    }
}

// Chuyển các token có dấu thành token không dấu
void getUtf8Tokens(char**& u8Tokens, int& nU8Token, wchar_t**& uniTokens, int& nUniToken, AppData& data) {
    char temp[100];
    for (int i = 0; i < nUniToken; i++) {
        if (uniTokens[i]) {
            UniToUtf8(temp, uniTokens[i], data.UniMin, data.Utf8);
            if (strlen(temp) > 0) {
                nU8Token++;
                u8Tokens = (char**)realloc(u8Tokens, nU8Token * sizeof(char*));
                u8Tokens[nU8Token - 1] = _strdup(temp);
            }
        }
    }
}

// Rút nội dung chính của 1 mảng các token không dấu
List getMainContent(char**& u8Tokens, int& nU8Token) {
    SkipList skipList = SkipList();
    char* temp = (char*)malloc(300 * sizeof(char));
    temp[0] = '\0';
    char space[] = " ";
    int total = 0;

    // Đếm số lần xuất hiện của các cụm từ (tạo từ không quá 3 token liên tiếp)
    for (int i = 0; i < nU8Token; i++) {
        for (int j = 0; j <= 2; j++) {
            if (i + j >= nU8Token) break;
            total++;
            strcat(temp, u8Tokens[i + j]);
            skipList.insert(temp);
            strcat(temp, space);
        }
        temp[0] = '\0';
    }
    free(temp);
    temp = NULL;

    // Khoảng [a, b] với a, b là chặn dưới, chặn trên của số lần xuất hiện
    // Xét chặn dưới là 0.42%, chặn trên là 45%
    double b = 0.45 * total, a = 0.0042 * total;

    // Xét các cụm từ trong skipList
    List list;
    init(list, sizeof(StringCount**));
    Column* head = skipList.head;
    while (head) {
        // Nếu số lần xuất hiện thuộc [a, b] hoặc chuỗi chỉ có không quá 6 cụm từ thì đưa vào list kết quả
        if (head->count >= 0 && (total <= 6 || (head->count >= a && head->count <= b))) {
            StringCount* word = (StringCount*)malloc(1 * sizeof(StringCount));
            word->str = _strdup(head->str);
            word->count = head->count;

            addTail(list, &word);
        }
        head = head->cells[0].next_column;
    }

    freeSkipList(skipList);
    return list;
}

// Xử lý 1 chuỗi văn bản có dấu, trả về list chứa nội dung chính
List preprocessString(wchar_t*& rawData, AppData& data) {
    // Tách token, xóa stopword, chuyển các token thành token không dấu
    wchar_t** tokens = NULL;
    int nToken = 0;
    getTokens(tokens, nToken, rawData, data);

    deleteStopword(tokens, nToken, data);

    char** u8Tokens = NULL;
    int nU8Token = 0;
    getUtf8Tokens(u8Tokens, nU8Token, tokens, nToken, data);

    // List chứa nội dung chính
    List list = getMainContent(u8Tokens, nU8Token);

    for (int i = 0; i < nU8Token; i++) {
        free(u8Tokens[i]);
    }
    free(u8Tokens);
    u8Tokens = NULL;

    for (int i = 0; i < nToken; i++) {
        free(tokens[i]);
    }
    free(tokens);
    tokens = NULL;

    return list;
}

// Xử lý 1 file txt, ghi ra file meta data
void preprocessFile(char*& directory, AppData& data, FILE*& metaData) {
    wchar_t wDirect[MAXLEN_DIRECT + 1];
    swprintf(wDirect, MAXLEN_DIRECT, L"%hs", directory);

    wchar_t* rawData = (wchar_t*)malloc(1 * sizeof(wchar_t));
    rawData[0] = L'\0';
    int lenRaw = 0;

    wchar_t* buffer = (wchar_t*)malloc(2001 * sizeof(wchar_t));

    // Đọc nội dung gốc của file
    FILE* fIn = _wfopen(wDirect, L"r,ccs=UTF-16LE");
    while (!feof(fIn)) {
        fgetws(buffer, 2000, fIn);
        if (!lastCharOfParaph(buffer)) break;
        if (buffer[0] == L'\0') continue;
        addWstring(rawData, lenRaw, buffer);
    }
    free(buffer);
    buffer = NULL;
    fclose(fIn);

    // Lấy ra nội dung chính
    List list = preprocessString(rawData, data);
    free(rawData);
    rawData = NULL;

    // Ghi ra file meta
    fprintf(metaData, "%s\n", directory);
    Node* p = list.head;
    while (p) {
        fprintf(metaData, "#%s#%d", (*(StringCount**)p->data)->str, (*(StringCount**)p->data)->count);
        free((*(StringCount**)p->data)->str);
        removeHead(list);
        p = list.head;
    }
    fprintf(metaData, "\n");
}

// Tạo file meta data cho app
void preprocessData(AppData& data) {
    // File siêu dữ liệu
    FILE* metaData = fopen("./stored/metaData.txt", "w");
    if (!metaData) return;

    clock_t start, end;
    start = clock();

    FILE* file = fopen("./stored/index.txt", "r");
    if (!file) return;

    char* fileDirect = (char*)malloc((MAXLEN_DIRECT + 1) * sizeof(char));
    while (!feof(file)) {
        // Xử lý từng file có tên trong index.txt
        fgets(fileDirect, MAXLEN_DIRECT, file);

        if (feof(file) || !lastCharOfFileName(fileDirect)) break;
        if (fileDirect[0] == '\0') continue;

        preprocessFile(fileDirect, data, metaData);
    }

    fclose(metaData);

    end = clock();
    wprintf(L"Time = %g\n", double(end - start) / CLOCKS_PER_SEC);

    free(fileDirect);
    fileDirect = NULL;
}

// Giải phóng bộ nhớ
void freeData(AppData& data) {
    free(data.UniFull); free(data.UniLower); free(data.UniMin); free(data.Utf8);
    data.UniFull = data.UniLower = data.UniMin = NULL;
    data.Utf8 = NULL;

    for (int i = 0; i < data.nStopword; i++) free(data.Stopwords[i]);
    free(data.Stopwords);
    data.Stopwords = NULL;
}
