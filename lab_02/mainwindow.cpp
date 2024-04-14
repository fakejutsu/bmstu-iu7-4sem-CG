#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "picture.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Picture & pic = Picture::getInstance();
    Picture::applyToView(*(ui->graphicsView));    
    Picture::drawStart(ui->graphicsView);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_transferButton_clicked()
{
    bool result = true;
    bool r = true;

    int dx = ui->dx_transfer->toPlainText().toDouble(&r);
    result &= r;

    int dy = ui->dy_transfer->toPlainText().toDouble(&r);
    result &= r;

    if (!result)
    {
        QMessageBox::critical(this, "Данные ввели некорректно", "Данные для переноса введены некорректно. Изображение не было преобразовано");
        return;
    }

    double transferMatrix[3][3];
    Picture::getTransferMatrix(dx, dy, transferMatrix);
    Picture::applyMatrix(transferMatrix);
    Picture::applyToView(*(ui->graphicsView));
}


void MainWindow::on_scaleButton_clicked()
{
    bool result = true;
    bool r = true;

    int cx = ui->scale_x->toPlainText().toDouble(&r);
    result &= r;

    int cy = ui->scale_y_2->toPlainText().toDouble(&r);
    result &= r;

    double kx = ui->scale_kx->toPlainText().toDouble(&r);
    result &= r;

    double ky = ui->scale_ky->toPlainText().toDouble(&r);
    result &= r;

    if (!result)
    {
        QMessageBox::critical(this, "Данные ввели некорректно", "Данные для масштабирования введены некорректно. Изображение не было преобразовано");
        return;
    }


    double scaleMatrix[3][3];
    Picture::getScaleMatrix(cx, cy, kx, ky, scaleMatrix);
    Picture::applyMatrix(scaleMatrix);
    Picture::applyToView(*(ui->graphicsView));
}


void MainWindow::on_rotateButton_clicked()
{
    bool result = true;
    bool r = true;

    int cx = ui->rotate_x->toPlainText().toDouble(&r);
    result &= r;

    int cy = ui->rotate_y->toPlainText().toDouble(&r);
    result &= r;

    int angle = ui->rotate_angle->toPlainText().toDouble(&r);
    result &= r;

    if (!result)
    {
        QMessageBox::critical(this, "Данные ввели некорректно", "Данные для поворота введены некорректно. Изображение не было преобразовано");
        return;
    }

    double rotateMatrix[3][3];
    Picture::getRotateMatrix(cx, cy, -angle, rotateMatrix);
    Picture::applyMatrix(rotateMatrix);
    Picture::applyToView(*(ui->graphicsView));
}


void MainWindow::on_undo_clicked()
{
    Picture::undo();
    Picture::applyToView(*(ui->graphicsView));
}



void MainWindow::on_reset_clicked()
{
    Picture::reset();
    Picture::applyToView(*(ui->graphicsView));
}

