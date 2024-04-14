#include "mainwindow.h"
#include "algorithmdrawfactory.h"
#include "qglobal.h"
#include "ui_mainwindow.h"
#include "qgraphicsitem.h"
#include <QElapsedTimer>
#include <QtCharts>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto view = ui->graphicsView;
    view->setScene(new QGraphicsScene(view));
    view->scene()->setSceneRect(0, 0, 5000, 5000);

    int counter = 0;

    for (auto button: ui->buttonGroup->buttons())
    {
        if (button->objectName() == "InternalButton")
        {
            ui->buttonGroup->setId(button, AlgorithmDrawType::INTERNAL);
        }
        else if (button->objectName() == "DDAButton")
        {
            ui->buttonGroup->setId(button, AlgorithmDrawType::DDA);
        }
        else if (button->objectName() == "BrFloatButton")
        {
            ui->buttonGroup->setId(button, AlgorithmDrawType::BR_FLOAT);
        }
        else if (button->objectName() == "BrIntButton")
        {
            ui->buttonGroup->setId(button, AlgorithmDrawType::BR_INT);
        }
        else if (button->objectName() == "BrWithoutStairsButton")
        {
            ui->buttonGroup->setId(button, AlgorithmDrawType::BR_WITHOUT_STAIRS);
        }
        else
        {
            ui->buttonGroup->setId(button, AlgorithmDrawType::VU);
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui->graphicsView->scene();
    delete scenePixmap;
    delete ui;
}

int MainWindow::getLineCoords(QPoint & first, QPoint &second)
{
    bool isOk = false;
    int x1 = ui->x1->toPlainText().toFloat(&isOk);
    if (!isOk || x1 < 0)
    {
        return 1;
    }

    int y1 = ui->y1->toPlainText().toFloat(&isOk);
    if (!isOk || y1 < 0)
    {
        return 1;
    }

    int x2 = ui->x1_2->toPlainText().toFloat(&isOk);
    if (!isOk || x2 < 0)
    {
        return 1;
    }

    int y2 = ui->y1_2->toPlainText().toFloat(&isOk);
    if (!isOk || y2 < 0)
    {
        return 1;
    }

    first.setX(x1); first.setY(y1);
    second.setX(x2); second.setY(y2);

    return 0;
}

void MainWindow::on_pushButton_clicked()
{
    drawLineScene();
}

static void rotatePoint(QPoint & point, QPoint const & oPoint, int gradAngle)
{
    double radians = (2 * M_PI) / 360.0 * gradAngle;

    point.setX(point.x() - oPoint.x());
    point.setY(point.y() - oPoint.y());

    double new_x = round(point.x() * cos(radians) - point.y() * sin(radians));
    double new_y = round(point.x() * sin(radians) + point.y() * cos(radians));

    point.setX(new_x);
    point.setY(new_y);

    point.setX(point.x() + oPoint.x());
    point.setY(point.y() + oPoint.y());
}

void MainWindow::drawSpectre()
{
    scenePixmap = new QPixmap(1000, 1000);
    scenePixmap->fill(Qt::transparent);

    QPoint baseSecond;
    QPoint first, second;
    int res = getLineCoords(first, second);
    if (res)
    {
        QMessageBox::critical(this, "Некорректно введены координаты", "Пожалуйста, введите корректные значения координат начала и конца отрезка");
        return;
    }
    baseSecond = second;

    int gradStep = ui->spectreAngle->toPlainText().toInt();

    if (gradStep <= 0)
    {
        QMessageBox::critical(this, "Ошибка", "Некорректное значение шага поворота");
        return;
    }


    int algorithmType = ui->buttonGroup->checkedId();

    gradStep %= 360;

    if (gradStep % 360 == 0)
    {
        QMessageBox::critical(this, "Ошибка", "Некорректное значение шага поворота");
        return;
    }


    AlgorithmDrawFactory factory = AlgorithmDrawFactory();
    std:: shared_ptr<AlgorithmDrawBase> algorithm = factory.newAlgorithm(AlgorithmDrawType(algorithmType));
    algorithm->setColor(lineColor);

    std::shared_ptr<QPixmap> p_pixmap = std::make_shared<QPixmap>(1000, 1000);
    p_pixmap->fill(Qt::transparent);

    QPalette palette = ui->colorButton->palette();
    QColor lineColor = palette.color(QPalette::Button);

    QPainter painter = QPainter();

    painter.begin(scenePixmap);

    for (int i = 0; i < 360.0 / (gradStep) + 1; ++i)
    {
        if (algorithmType == AlgorithmDrawType::INTERNAL)
        {
            painter.setPen(lineColor);
            painter.drawLine(first, second);
        }
        else
        {
            std::shared_ptr<QList<QPair<QPoint, int>>> pointsAlpha = std::make_shared<QList<QPair<QPoint, int>>>();
            algorithm->getLinePoints(first, second, *pointsAlpha);

            for (auto pointAlpha : *pointsAlpha)
            {
                lineColor.setAlpha(pointAlpha.second);
                painter.setPen(lineColor);

                painter.drawPoint(pointAlpha.first);
            }
        }

        QPoint temp = baseSecond;
        rotatePoint(temp, first, gradStep * i);
        second = temp;
    }
    painter.end();

    scenePixmap->save("D:/fakejutsu projects/kg/lab_03/lab_03/lastDrawLine.png", nullptr, 100);

    ui->graphicsView->scene()->addPixmap(*scenePixmap);

}


void MainWindow::drawLineScene()
{
    int algorithmType = ui->buttonGroup->checkedId();
    QPoint f1 = QPoint(), f2 = QPoint();

    int res = getLineCoords(f1, f2);;
    if (res)
    {
        QMessageBox::critical(this, "Некорректно введены координаты", "Пожалуйста, введите корректные значения координат начала и конца отрезка");
        return;
    }


    AlgorithmDrawFactory factory = AlgorithmDrawFactory();
    std:: shared_ptr<AlgorithmDrawBase> algorithm = factory.newAlgorithm(AlgorithmDrawType(algorithmType));

    std::shared_ptr<QPixmap> p_pixmap = std::make_shared<QPixmap>(1000, 1000);
    p_pixmap->fill(Qt::transparent);


    QPalette palette = ui->colorButton->palette();
    QColor lineColor = palette.color(QPalette::Button);

    QPainter painter = QPainter();

    int stairsCount = 0;

    painter.begin(p_pixmap.get());

    if (algorithmType == AlgorithmDrawType::INTERNAL)
    {
        painter.setPen(lineColor);
        painter.drawLine(f1, f2);
    }
    else
    {

        std::shared_ptr<QList<QPair<QPoint, int>>> pointsAlpha = std::make_shared<QList<QPair<QPoint, int>>>();
        algorithm->getLinePoints(f1, f2, *pointsAlpha);
        QPoint prevPoint = QPoint(-1, -1);

        for (auto pointAlpha : *pointsAlpha)
        {
            lineColor.setAlpha(pointAlpha.second);
            painter.setPen(lineColor);

            painter.drawPoint(pointAlpha.first);
        }

        for (int j = 0; j < pointsAlpha->size(); j = algorithm->getName() == "Ву" ? j + 2 : j + 1)
        {
            QPoint point = pointsAlpha->at(j).first;
            if (point.x() != prevPoint.x() && point.y() != prevPoint.y())
            {
                ++stairsCount;
            }

            prevPoint = point;
        }




    }

    painter.end();

    QImage image = p_pixmap->toImage();
    QString isEnd = "Отрезок \"попал\" в конечную точку";
    QRgb pixelColor = image.pixel(f2.x(), f2.y());
    if (qAlpha(pixelColor) == 0)
    {
        isEnd = "Отрезок не попал";
    }

    isEnd += "\n";
    if (algorithmType == AlgorithmDrawType::INTERNAL)
    {
        isEnd += "Для библиотечной функции не отображается кол-во ступенек";
    }
    else
    {
        isEnd += "Кол-во ступенек: " + QString::number(stairsCount - 1);
    }

    ui->textEdit->setText(isEnd);

    ui->graphicsView->scene()->addPixmap(*(p_pixmap.get()));
}




void MainWindow::on_pushButton_2_clicked()
{
    ui->graphicsView->scene()->clear();

}


void MainWindow::on_spinBox_valueChanged(int value)
{
}


QColor MainWindow::colorFromString(QString colorString){
    if (colorString == "Красный")
    {
        return Qt::red;
    }
    else if (colorString == "Жёлтый")
    {
        return Qt::yellow;
    }
    else if (colorString == "Оранжевый")
    {
        //todo

        return QColorConstants::Svg::orange;
    }
    else if (colorString == "Фиолетовый")
    {
        return QColorConstants::Svg::purple;
    }
    else if (colorString == "Зелёный")
    {
        return Qt::green;
    }
    else if (colorString == "Белый")
    {
        return Qt::white;
    }
    else if (colorString == "Синий")
    {
        return Qt::blue;
    }
    else
    {
        return Qt::black;
    }
}

void MainWindow::on_backgroundColor1_clicked()
{

}


void MainWindow::on_lineColor_activated(int index)
{
    QString colorString = ui->lineColor->currentText();
    QColor color = colorFromString(colorString);

    lineColor = color;

    QString buttonStyle = "QPushButton { background-color: %1; }";
    buttonStyle = buttonStyle.arg(color.name());
    ui->colorButton->setStyleSheet(buttonStyle);
    ui->colorButton->setAutoFillBackground(true);
    ui->colorButton->repaint();

}


void MainWindow::on_pushButton_3_clicked()
{
    drawSpectre();
}


void MainWindow::on_pushButton_4_clicked()
{
    int algorithms[] = {AlgorithmDrawType::DDA, AlgorithmDrawType::BR_INT, AlgorithmDrawType::BR_FLOAT, AlgorithmDrawType::BR_WITHOUT_STAIRS, AlgorithmDrawType::VU};
    const int algorithmsCount = sizeof(algorithms) / sizeof(int);

    qint64 algorithmTime[algorithmsCount];

    AlgorithmDrawFactory factory = AlgorithmDrawFactory();


    QBarSeries *series = new QBarSeries();
    QBarSet *barset = new QBarSet("Алгоритмы");
    QStringList categories;
    std::shared_ptr<QList<QPair<QPoint, int>>> pointsAlpha = std::make_shared<QList<QPair<QPoint, int>>>();

    for (int i = 0; i < algorithmsCount; ++i)
    {
        std:: shared_ptr<AlgorithmDrawBase> algorithm = factory.newAlgorithm(AlgorithmDrawType(algorithms[i]));

        qint64 cleanWasted = 0;
        QElapsedTimer timer;
        QElapsedTimer cleanTimer;
        timer.start();
        for (int j = 0; j < 100; ++j)
        {

            algorithm->getLinePoints(QPoint(0, 0), QPoint(300, 400), *pointsAlpha);
            //qDebug() << cleanWasted << '\n';

        }

        qDebug() << timer.nsecsElapsed() << '-' << cleanWasted << '\n';
        qint64 elapsedTime = timer.nsecsElapsed() - cleanWasted;
        timer.invalidate();

        pointsAlpha->clear();

        algorithmTime[i] = elapsedTime;
        // Добавление в график

        *barset << (double) (elapsedTime / 1000);



        categories << algorithm->getName();
    }



    series->append(barset);

    QChart *chart = new QChart();
    chart->addSeries(series);


    chart->setTitle("Скорость работы алгоритмов");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Скорость построения 100 отрезков длиной 500, мс");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);



    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);


    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->show();
}


void MainWindow::on_pushButton_5_clicked()
{
    std::shared_ptr<QList<QPair<QPoint, int>>> pointsAlpha = std::make_shared<QList<QPair<QPoint, int>>>();

    int algorithms[] = {AlgorithmDrawType::DDA, AlgorithmDrawType::BR_FLOAT, AlgorithmDrawType::BR_INT,  AlgorithmDrawType::BR_WITHOUT_STAIRS, AlgorithmDrawType::VU};
    const int algorithmsCount = sizeof(algorithms) / sizeof(int);

    qint64 algorithmStairs[algorithmsCount];

    AlgorithmDrawFactory factory = AlgorithmDrawFactory();


    QStringList categories;

    QChart *chart = new QChart();


    for (int i = 0; i < algorithmsCount; ++i)
    {

        QLineSeries *series = new QLineSeries(chart);

        QPen pen = QPen();
        pen.setStyle((Qt::PenStyle)(Qt::SolidLine + i));
        pen.setColor(Qt::red + i);
        //series->setPen(pen);

        QPoint first = QPoint(0, 0);
        QPoint second = QPoint(200, 0);

        std:: shared_ptr<AlgorithmDrawBase> algorithm = factory.newAlgorithm(AlgorithmDrawType(algorithms[i]));




        int gradStep = 5;
        for (int h = 0; h < 90 / (gradStep) + 1; ++h)
        {
            int stairsCount = 0;
            pointsAlpha->clear();
            algorithm->getLinePoints(first, second, *pointsAlpha);

            QPoint prevPoint = QPoint(-1, -1);
            for (int j = 0; j < pointsAlpha->size(); j = algorithm->getName() == "Ву" ? j + 2 : j + 1)
            {
                QPoint point = pointsAlpha->at(j).first;
                if (point.x() != prevPoint.x() && point.y() != prevPoint.y())
                {
                    ++stairsCount;
                }

                prevPoint = point;
            }

            series->append(h * gradStep, stairsCount);
            rotatePoint(second, first, gradStep);
        }

        // Добавление в график

        series->setName(algorithm->getName());
        chart->addSeries(series);

    }


    chart->setTitle("Зависимость кол-ва ступенек от угла наклона отрезка");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, 90);
    chart->axes(Qt::Horizontal).first()->setTitleText("Угол наклона, градусы");
    chart->axes(Qt::Vertical).first()->setRange(0, 250);
    chart->axes(Qt::Vertical).first()->setTitleText("Кол-во ступенек у отрезка длины 200");



    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->show();
}


void MainWindow::on_pushButton_6_clicked()
{
    std::shared_ptr<QList<QPair<QPoint, int>>> pointsAlpha = std::make_shared<QList<QPair<QPoint, int>>>();

    int algorithms[] = {AlgorithmDrawType::DDA, AlgorithmDrawType::BR_FLOAT, AlgorithmDrawType::BR_INT,  AlgorithmDrawType::BR_WITHOUT_STAIRS, AlgorithmDrawType::VU};
    const int algorithmsCount = sizeof(algorithms) / sizeof(int);

    qint64 algorithmStairs[algorithmsCount];

    AlgorithmDrawFactory factory = AlgorithmDrawFactory();


    QStringList categories;

    QChart *chart = new QChart();


    for (int i = 0; i < algorithmsCount; ++i)
    {

        QLineSeries *series = new QLineSeries(chart);

        QPen pen = QPen();
        pen.setStyle((Qt::PenStyle)(Qt::SolidLine + i));
        pen.setColor(Qt::red + i);
        //series->setPen(pen);

        QPoint first = QPoint(0, 0);
        QPoint second = QPoint(200, 0);

        std:: shared_ptr<AlgorithmDrawBase> algorithm = factory.newAlgorithm(AlgorithmDrawType(algorithms[i]));




        int gradStep = 5;
        for (int h = 0; h < 90 / (gradStep) + 1; ++h)
        {
            int stairsCount = 0;
            pointsAlpha->clear();
            algorithm->getLinePoints(first, second, *pointsAlpha);

            QPoint prevPoint = QPoint(-1, -1);
            int cur_len = 1;
            int max_len = 1;
            for (int j = 0; j < pointsAlpha->size(); j = algorithm->getName() == "Ву" ? j + 2 : j + 1)
            {
                QPoint point = pointsAlpha->at(j).first;
                if (point.x() != prevPoint.x() && point.y() != prevPoint.y())
                {
                    cur_len = 1;
                }
                else
                {
                    ++cur_len;
                }

                max_len = std::max(max_len, cur_len);

                prevPoint = point;
            }

            series->append(h * gradStep, max_len);
            rotatePoint(second, first, gradStep);
        }

        // Добавление в график

        series->setName(algorithm->getName());
        chart->addSeries(series);

    }


    chart->setTitle("Максимальная длина ступеньки от угла");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, 90);
    chart->axes(Qt::Horizontal).first()->setTitleText("Угол наклона, градусы");
    chart->axes(Qt::Vertical).first()->setRange(0, 250);
    chart->axes(Qt::Vertical).first()->setTitleText("Максимальная длина ступеньки для отрезка длины 200");



    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->show();
}


void MainWindow::on_lineColor_2_activated(int index)
{
    QString colorString = ui->lineColor_2->currentText();
    QColor color = colorFromString(colorString);

    QBrush brush(color);
    ui->graphicsView->scene()->setBackgroundBrush(brush);
}

