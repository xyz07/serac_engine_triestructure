#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define _CRT_SECURE_NO_WARNINGS
#include <QMainWindow>
#include <stdio.h>
#include <QPushButton>
#include <time.h>
#include <QListWidgetItem>
#include <QDesktopServices>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
#include <QObject>
#include <time.h>
#include <QDate>
#include <QTime>
#include <QSplashScreen>
#include <QLabel>
#include <QMovie>
#include <windows.h>
#include "qtabbar.h"
#include "ui_mainwindow.h"
#include "engine/engine.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Engine engine;
    clock_t start, end;
    QPushButton* selected = NULL;
    MainWindow(QWidget *parent = nullptr);

    // Load dữ liệu ban đầu: Bảng mã, stopword
    void startLoading();
    // Hiển thị kết quả tìm kiếm
    void showResult();

    ~MainWindow();
public slots:
    void showInitialLoading();

    // Mở file khi người dùng double click vào kết quả tìm kiếm
    void openFile(QListWidgetItem* item);

signals:
    void loadComplete();

private slots:
    // Mở tab SEARCH
    void on_SEARCH_clicked();


    // Mở tab HISTORY
    void on_HISTORY_clicked();

    // Mở tab EDIT
    void on_EDIT_clicked();

    // Mở tab ABOUT
    void on_ABOUT_clicked();

    // Thoát chương trình
    void on_EXIT_clicked();

    // Event: Bấm nút "Tìm" ở tab SEARCH
    void on_searchBtn_clicked();

    // Event: Hiển thị thêm kết quả tìm kiếm
    void on_showMore_clicked();

    // Event: Xóa tất cả lịch sử
    void on_historyDeleteAll_clicked();

    // Event: Xóa các dòng lịch sử tìm kiếm đã chọn
    void on_historyDeleteSome_clicked();

    // Event: Bấm nút "thêm" ở tab EDIT (Thêm file vào DB)
    void on_addFileBtn_clicked();

    // Event: Bấm nút "Xóa" ở tab EDIT (Xóa file khỏi DB)
    void on_delFileBtn_clicked();

private:
    Ui::MainWindow *ui;
};

// Đợi hàm khác thực hiện (không có animation)
void waiting(std::function<void()>p);

// Hiển thị animation và đợi hàm khác thực hiện
void waitingWithAnimation(std::function<void()>p);

// Giới hạn độ dài hiển thị của chuỗi
QString limitLen(QString s, int maxLen);

#endif // MAINWINDOW_H
