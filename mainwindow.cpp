#include "mainwindow.h"
#include <functional>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setFixedSize(1024, 720);
    ui->setupUi(this);

    // Ẩn thanh tab
    QTabBar *tabBar = ui->tabs->findChild<QTabBar *>();
    tabBar->hide();

    // Set tab hiện tại là tab ABOUT
    this->selected = ui->ABOUT;
    ui->ABOUT->setStyleSheet("background-color: rgb(0, 55, 255);border-radius: 15px;color: white;cursor: pointer;");

    // Cho phép multi choice trong mục lịch sử tìm kiếm
    ui->historyResult->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // Double click vào item trong searchResult thì mở file đó lên
    connect(ui->searchResult, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(openFile(QListWidgetItem*)));
}

// Thoát khỏi chương trình
MainWindow::~MainWindow()
{
    // Cập nhật lại file meta data
    auto execute = std::bind(&Engine::updateData, this->engine);
    waitingWithAnimation(execute);

    // Xóa lịch sử, kết quả tìm kiếm (nếu còn lưu), cửa sổ ghi danh sách file đã thêm
    ui->historyResult->clear();
    ui->searchResult->clear();
    ui->fileOfUser->clear();

    // Free vùng nhớ đã cấp phát
    this->engine.endProg();

    delete ui;
}

// Load dữ liệu ban đầu: Bảng mã, stopword. Cập nhật lại dữ liệu. Load danh sách file user đã thêm
void MainWindow::startLoading() {
    this->engine.prepareData();
    this->engine.updateData();

    // Load danh sách file user đã thêm (nếu có)
    FILE* file = fopen("./stored/fileOfUser.txt", "r");
    if (file) {
        char* buffer = (char*)malloc((MAXLEN_DIRECT + 1) * sizeof(char));
        do {
            fgets(buffer, MAXLEN_DIRECT, file);
            if (feof(file) || !lastCharOfFileName(buffer)) break;
            if (buffer[0] == '\0') break;

            QListWidgetItem* item = new QListWidgetItem;
            QString direct = buffer;
            item->setWhatsThis(direct);
            direct = limitLen(direct, 75);
            item->setText("   " + direct);
            ui->fileOfUser->addItem(item);
        } while(1);
        free(buffer);
        fclose(file);
    }
}

// Hiển thị kết quả tìm kiếm
void MainWindow::showResult() {
    char* buffer = (char*)malloc(500 * sizeof(char));

    // 1 lần hiển thị không quá threshold kết quả
    Node* p = this->engine.result.tail;
    for (int i = 1; i <= this->engine.threshold && p; i++) {
        this->engine.rank++;

        QListWidgetItem* item = new QListWidgetItem;
        QString direct = (*(StringCount**)p->data)->str;
        item->setWhatsThis(direct);
        char* temp = (*(StringCount**)p->data)->str;
        int len = 52;
        if (strlen(temp) >= len) {
            temp[len - 3] = '.';
            temp[len - 2] = '.';
            temp[len - 1] = '.';
            temp[len] = '\0';
        }

        sprintf(buffer, "  %d.\tScore: %8d\t%s", this->engine.rank, (*(StringCount**)p->data)->count, temp);

        QString text = QString::fromUtf8(buffer);
        item->setText(text);

        ui->searchResult->addItem(item);

        free(temp);
        removeTail(this->engine.result);
        p = this->engine.result.tail;
    }

    free(buffer);
}

// Load dữ liệu cần thiết khi mở app
void MainWindow::showInitialLoading()
{
    auto execute = std::bind(&MainWindow::startLoading, this);
    waitingWithAnimation(execute);

    emit this->loadComplete();
}

// Mở file khi double click vào 1 kết quả tìm kiếm
void MainWindow::openFile(QListWidgetItem* item) {
    QString direct = item->whatsThis();
    QDesktopServices::openUrl(QUrl::fromLocalFile(direct));
}

// Mở tab ABOUT
void MainWindow::on_ABOUT_clicked()
{
    ui->searchStatus->clear();
    this->engine.clearSearchResult();

    if (!this->selected) {
        this->selected = ui->ABOUT;
    }
    else {
        this->selected->setStyleSheet("border: 2px solid rgb(0, 55, 255);border-radius: 15px;color: rgb(0, 55, 255);cursor: pointer;");
        this->selected = ui->ABOUT;
    }
    ui->ABOUT->setStyleSheet("background-color: rgb(0, 55, 255);border-radius: 15px;color: white;cursor: pointer;");
    ui->tabs->setCurrentIndex(0);
}

// Mở tab SEARCH
void MainWindow::on_SEARCH_clicked()
{
    if (!this->selected) {
        this->selected = ui->SEARCH;
    }
    else {
        this->selected->setStyleSheet("border: 2px solid rgb(0, 55, 255);border-radius: 15px;color: rgb(0, 55, 255);cursor: pointer;");
        this->selected = ui->SEARCH;
    }
    ui->SEARCH->setStyleSheet("background-color: rgb(0, 55, 255);border-radius: 15px;color: white;cursor: pointer;");
    ui->tabs->setCurrentIndex(1);

    // Reset lại các biến (từ lần search trước)
    ui->searchInput->setText("");
    ui->searchResult->clear();
    ui->searchStatus->clear();

    // Search
    this->engine.clearSearchResult();
}

// Mở tab HISTORY
void MainWindow::on_HISTORY_clicked()
{
    ui->searchStatus->clear();
    this->engine.clearSearchResult();

    if (!this->selected) {
        this->selected = ui->HISTORY;
    }
    else {
        this->selected->setStyleSheet("border: 2px solid rgb(0, 55, 255);border-radius: 15px;color: rgb(0, 55, 255);cursor: pointer;");
        this->selected = ui->HISTORY;
    }
    ui->HISTORY->setStyleSheet("background-color: rgb(0, 55, 255);border-radius: 15px;color: white;cursor: pointer;");
    ui->tabs->setCurrentIndex(2);
}

// Mở tab EDIT
void MainWindow::on_EDIT_clicked()
{
    ui->searchStatus->clear();
    this->engine.clearSearchResult();

    if (!this->selected) {
        this->selected = ui->EDIT;
    }
    else {
        this->selected->setStyleSheet("border: 2px solid rgb(0, 55, 255);border-radius: 15px;color: rgb(0, 55, 255);cursor: pointer;");
        this->selected = ui->EDIT;
    }
    ui->EDIT->setStyleSheet("background-color: rgb(0, 55, 255);border-radius: 15px;color: white;cursor: pointer;");
    ui->tabs->setCurrentIndex(3);

    // Reset lại các biến (từ lần chỉnh sửa trước rồi chuyển tab)
    ui->directFileAdded->clear();
    ui->updateNoti->clear();
}

// Thoát chương trình
void MainWindow::on_EXIT_clicked()
{
    this->close();
}

// Event: Bấm nút "Tìm" ở tab SEARCH
void MainWindow::on_searchBtn_clicked()
{
    ui->searchResult->clear();
    ui->searchStatus->clear();
    this->engine.clearSearchResult();

    // Đọc input
    QString input = ui->searchInput->text();
    int len = input.length();
    this->engine.userInput = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
    input.toWCharArray(this->engine.userInput);
    this->engine.userInput[len] = L'\0';

    // Tìm kiếm
    clock_t start, end;
    start = clock();
    auto execute = std::bind(&Engine::searchData, &(this->engine));
    waiting(execute);
    end = clock();

    // Thời gian, số lượng search được
    QString searchTime = QString::number(double(end - start)/CLOCKS_PER_SEC);
    ui->searchStatus->setText("Tìm thấy " + QString::number(this->engine.nResult) + " kết quả trong " + searchTime + " giây.");

    // Show kết quả
    if (this->engine.nResult == 0) {
        ui->searchResult->addItem("\tKhông tìm thấy dữ liệu liên quan... :(");
    }
    else this->showResult();

    // Thêm vào lịch sử tìm kiếm
    input = input.trimmed();
    input = limitLen(input, 45);
    QString day = (QDate::currentDate()).toString();
    QString time = (QTime::currentTime()).toString();
    QString item ="     " +  day + " - " + time + "       " + input;
    ui->historyResult->addItem(item);
}

// Event: Hiển thị thêm kết quả tìm kiếm
void MainWindow::on_showMore_clicked()
{
    this->showResult();
}

// Event: Xóa tất cả lịch sử
void MainWindow::on_historyDeleteAll_clicked()
{
    ui->historyResult->clear();
}

// Event: Xóa các dòng lịch sử tìm kiếm đã chọn
void MainWindow::on_historyDeleteSome_clicked()
{
    qDeleteAll(ui->historyResult->selectedItems());
}

// Event: Bấm nút "thêm" ở tab EDIT (Thêm file vào DB)
void MainWindow::on_addFileBtn_clicked()
{
    // Đọc đường dẫn đến file
    QString input = ui->directFileAdded->text();
    input = input.replace('\\', '/');

    QByteArray temp = input.toLocal8Bit();
    this->engine.addFileDirect = temp.data();

    // Đợi thêm file
    auto execute = std::bind(&Engine::addFile, &(this->engine));
    waiting(execute);

    // Thêm vào list file mà user đã thêm
    qDebug() << this->engine.errorFlag << "\n";
    if (this->engine.errorFlag == 0) {
        QListWidgetItem* item = new QListWidgetItem;
        input = input.trimmed();
        item->setWhatsThis(input);
        input = limitLen(input, 75);
        item->setText("   " + input);
        ui->fileOfUser->addItem(item);
    }

    // Thông báo cho người dùng: Thêm thành công hoặc lỗi
    QString noti;
    if (this->engine.errorFlag == 2) noti = "Lỗi! Không tìm thấy file muốn thêm.";
    else noti = "Đã thêm file";
    ui->addStatus->setText(noti);
    ui->addStatus->show();
    QTimer::singleShot(1500, ui->addStatus, &QWidget::hide);
}

// Event: Bấm nút "Xóa" ở tab EDIT (Xóa file khỏi DB)
void MainWindow::on_delFileBtn_clicked()
{
    if (ui->fileOfUser->selectedItems().length() == 0) return;
    ui->updateNoti->clear();
    this->engine.updateCount++;

    // Lấy đường dẫn đến file
    QString input = ui->fileOfUser->selectedItems()[0]->whatsThis();
    input = input.replace('\\', '/');
    if (input.length() == 0) return;
    QByteArray temp = input.toLocal8Bit();
    this->engine.delFileDirect = temp.data();

    // Xóa khỏi list file đã thêm
    qDeleteAll(ui->fileOfUser->selectedItems());

    // Đợi xóa file
    auto execute = std::bind(&Engine::delFile, this->engine);
    waiting(execute);

    // Nếu đến thời điểm update data thì update
    if (this->engine.updateCount >= 5) {
        // Đợi update
        clock_t start, end;
        start = clock();
        auto execute = std::bind(&Engine::updateData, this->engine);
        waitingWithAnimation(execute);
        end = clock();

        // Thông báo thời gian update
        QString time = QString::number((double) (end - start) / CLOCKS_PER_SEC);
        ui->updateNoti->setText("Dữ liệu đã được cập nhật định kỳ sau " + time + " giây.");
        ui->updateNoti->setStyleSheet("text-align: center");
        ui->updateNoti->show();
        QTimer::singleShot(3000, ui->updateNoti, &QWidget::hide);

        // Reset biến count
        this->engine.updateCount = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////

// Hiển thị animation và đợi hàm khác thực hiện
void waitingWithAnimation(std::function<void()>p) {
//    QMovie *movie = new QMovie("C:/Users/Admin/Downloads/gg_loading.gif");
    QMovie *movie = new QMovie("./loading.gif");
    movie->setScaledSize(QSize(400, 200));
    QLabel *processLabel = new QLabel(NULL);
    movie->start();
    processLabel->setMovie(movie);
    processLabel->setWindowTitle("Đang cập nhật dữ liệu...");
    processLabel->show();

    QFuture<void> future = QtConcurrent::run([=]() {
        p();
    });
    QFutureWatcher<void> watcher;
    QEventLoop loop;
    QObject::connect(&watcher, SIGNAL(finished()), &loop, SLOT(quit()),  Qt::QueuedConnection);
    watcher.setFuture(future);
    loop.exec();
    watcher.cancel();

    processLabel->hide();
}

// Đợi hàm khác thực hiện (không có animation)
void waiting(std::function<void()>p) {
    QFuture<void> future = QtConcurrent::run([=]() {
        p();
    });
    QFutureWatcher<void> watcher;
    QEventLoop loop;
    QObject::connect(&watcher, SIGNAL(finished()), &loop, SLOT(quit()),  Qt::QueuedConnection);
    watcher.setFuture(future);
    loop.exec();
    watcher.cancel();
}

// Giới hạn độ dài hiển thị của chuỗi
QString limitLen(QString s, int maxLen) {
    if (s.length() >= maxLen) {
        s[maxLen - 3] = '.';
        s[maxLen - 2] = '.';
        s[maxLen - 1] = '.';
        s = s.left(maxLen);
    }
    return s;
}
