#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QImage image;
    bool loaded = false;
    bool just_decoded = false;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void loadImage();
    void encodeMsg();
    void decodeMsg();
    void saveImage();
    void msgChanged();
};
#endif // MAINWINDOW_H
