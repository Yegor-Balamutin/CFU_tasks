#include <QFileDialog>
#include <QImage>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::loadImage);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::encodeMsg);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::decodeMsg);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::saveImage);
    connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, this, &MainWindow::msgChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Допустимые форматы (*.png)");
    if (fileName != ""){
        if (this->image.load(fileName)){
            this->image.convertTo(QImage::Format_ARGB32);
            this->loaded = true;
            ui->label->setText("Изображение загружено успешно.");
        }
        else {
            ui->label->setText("Изображение не загружено.");
        }
    }
}

unsigned int power_of_2(int a);

void MainWindow::encodeMsg()
{
    ui->label->setText("1");
    if (!this->loaded){
        ui->label->setText("Изображение не было загружено!");
        return;
    }
    QString msg = ui->plainTextEdit->toPlainText();
    QByteArray msg_u8 = msg.toUtf8();
    int width = this->image.width();
    int height = this->image.height();
    if (width * height * 3 < msg_u8.size() * 8 + 56){
        ui->label->setText("Ошибка. Не достаточно места!");
        return;
    }
    QString mark = "❤";
    int* arr = new int[32] {0};
    unsigned int size = msg_u8.size();
    for (int i = 31; i >= 0; i--){
        unsigned int a = power_of_2(i);
        if (size >= a){
            arr[i] = 1;
            size -= a;
        }
    }
    QByteArray len = "aaaa";
    int byte = 0;
    int bit = 7;
    for (int i = 31; i >= 0; i--){
        if (arr[i]) len[byte] |= (1 << bit);
        else len[byte] &= ~(1 << bit);
        bit--;
        if (bit == -1){
            byte++;
            bit = 7;
        }
    }
    delete [] arr;
    QByteArray full_msg;
    full_msg.append(mark.toUtf8());
    full_msg.append(len);
    full_msg.append(msg_u8);
    byte = 0;
    bit = 7;
    bool end = false;
    bool b;
    int* rgb = new int[3];
    for (int x = 0; x < width; x++){
        for (int y = 0; y < height; y++){
            QColor pixel = this->image.pixelColor(x, y);
            rgb[0] = pixel.red();
            rgb[1] = pixel.green();
            rgb[2] = pixel.blue();
            for (int i = 0; i < 3; i++){
                if (byte >= full_msg.size()){
                    end = true;
                    break;
                }
                b = bool((1 << bit)  &  full_msg[byte]);
                rgb[i] = (rgb[i] >> 1) << 1;
                if (b) rgb[i]++;

                bit--;
                if (bit == -1){
                    byte++;
                    bit = 7;
                }
            }
            pixel.setRed(rgb[0]);
            pixel.setGreen(rgb[1]);
            pixel.setBlue(rgb[2]);
            this->image.setPixelColor(x, y, pixel);
            if (end) break;
        }
        if (end) break;
    }
    delete[] rgb;
    ui->label->setText("Сообщение добавлено в картинку!");
}

void MainWindow::decodeMsg()
{
    if (!this->loaded){
        ui->label->setText("Изображение не было загружено!");
        return;
    }
    int width = this->image.width();
    int height = this->image.height();
    if (width * height < 56){
        ui->label->setText("Сообщение не обнаружено.");
        return;
    }
    QString mark = "❤";
    QByteArray three_bytes = "aaa";
    int* rgb = new int[3];
    int byte = 0;
    int bit = 7;
    int x, y;
    bool end = false;
    bool b;
    int last;
    for (x = 0; x < width; x++){
        for (y = 0; y < height; y++){
            QColor pixel = this->image.pixelColor(x, y);
            rgb[0] = pixel.red();
            rgb[1] = pixel.green();
            rgb[2] = pixel.blue();
            for (int i = 0; i < 3; i++){
                if (byte >= 3){
                    end = true;
                    last = i;
                    break;
                }
                b = bool(rgb[i] & 1);
                if (b) three_bytes[byte] |= (1 << bit);
                else three_bytes[byte] &= ~(1 << bit);
                bit--;
                if (bit == -1){
                    byte++;
                    bit = 7;
                }
            }
            if (end) break;
        }
        if (end) break;
    }
    if (three_bytes != mark.toUtf8()){
        ui->label->setText("Сообщение не обнаружено.");
        delete[] rgb;
        return;
    }
    int* arr = new int[32] {0};
    end = false;
    int n = 31;
    for (; x < width; x++){
        for (; y < height; y++){
            QColor pixel = this->image.pixelColor(x, y);
            rgb[0] = pixel.red();
            rgb[1] = pixel.green();
            rgb[2] = pixel.blue();
            while (last < 3){
                b = bool(rgb[last] & 1);
                if (b) arr[n] = 1;
                else arr[n] = 0;
                last++;
                n--;
                if (n < 0){
                    end = true;
                    break;
                }
            }
            if (end) break;
            last = 0;
        }
        if (end) break;
    }
    unsigned int size = 0;
    for (int i = 0; i <= 31; i++){
        if (arr[i]) size += power_of_2(i);
    }
    ui->label->setText(QString("Присутствует сообщение длиной %1 байт").arg(size));
    delete[] arr;
    QByteArray msg(size, 'a');
    byte = 0;
    bit = 7;
    end = false;
    for (; x < width; x++){
        for (; y < height; y++){
            if (last == 3){
                y++;
                last = 0;
            }
            QColor pixel = this->image.pixelColor(x, y);
            rgb[0] = pixel.red();
            rgb[1] = pixel.green();
            rgb[2] = pixel.blue();
            while (last < 3){
                if (byte >= size){
                    end = true;
                    break;
                }
                b = bool(rgb[last] & 1);
                if (b) msg[byte] |= (1 << bit);
                else msg[byte] &= ~(1 << bit);
                bit--;
                if (bit == -1){
                    byte++;
                    bit = 7;
                }
                last++;
            }
            if (end) break;
            last = 0;
        }
        if (end) break;
    }
    delete[] rgb;
    this->just_decoded = true;
    ui->plainTextEdit->setPlainText(QString::fromUtf8(msg));
    this->just_decoded = false;
}

void MainWindow::saveImage()
{
    if (!this->loaded){
        ui->label->setText("Изображение не было загружено!");
        return;
    }
    QString filePath = QFileDialog::getSaveFileName(this, "Сохранить картинку", "", "Допустимые форматы (*.png)");
    if (filePath != ""){
        if (this->image.save(filePath, "PNG")){
            ui->label->setText("Изображение сохранено успешно.");
        }
        else {
            ui->label->setText("Изображение не сохранено.");
        }
    }
}

void MainWindow::msgChanged()
{
    if (this->just_decoded) return;
    int width = this->image.width();
    int height = this->image.height();
    int max_bytes = width * height * 3 / 8;
    QString msg = ui->plainTextEdit->toPlainText();
    QByteArray msg_u8 = msg.toUtf8();
    int available = max_bytes - msg_u8.size();
    if (available >= 0) ui->label->setText(QString("Ещё можно ввести %1 байт").arg(available));
    else ui->label->setText("Слишком большое сообщение");
}

unsigned int power_of_2(int a){
    unsigned int result = 1;
    for (int i = 0; i < a; i++){
        result *= 2;
    }
    return result;
}
