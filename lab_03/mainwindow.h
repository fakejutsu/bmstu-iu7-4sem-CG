#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_backgroundColor1_clicked();

    void on_lineColor_activated(int index);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_lineColor_2_activated(int index);

private:
    QColor lineColor = Qt::black;
    Ui::MainWindow *ui;
    void drawLineScene();
    int getLineCoords(QPoint & first, QPoint &second);
    QColor colorFromString(QString color);
    void drawSpectre();
    QColor getCurrentLineColor();
    QPixmap *scenePixmap;
};
#endif // MAINWINDOW_H
