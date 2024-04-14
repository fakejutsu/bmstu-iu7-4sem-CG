#include "picture.h"
#include <QGraphicsItem>



Picture::Picture() {
    curScene = new QGraphicsScene();
    curScene->setSceneRect(-100, -100, 500, 500);
    baseScene = new QGraphicsScene();
    baseScene->setSceneRect(-100, -100, 500, 500);

    memset(curMatrix, 0, sizeof(double) * 3 * 3);
    curMatrix[0][0] = 1;
    curMatrix[1][1] = 1;
    curMatrix[2][2] = 1;
    memmove(prevMatrix, curMatrix, sizeof(double) * 3 * 3);
}

void Picture::undo()
{
    memmove(curMatrix, prevMatrix, sizeof(double) * 3 * 3);
}

void Picture::reset()
{
    memset(curMatrix, 0, sizeof(double) * 3 * 3);
    curMatrix[0][0] = 1;
    curMatrix[1][1] = 1;
    curMatrix[2][2] = 1;
}

Picture::~Picture()
{
    delete curScene;
    delete baseScene;
}

void Picture::drawRectangle(int ulx, int uly, int w, int h)
{
    curScene->addLine(ulx, uly, ulx + w, uly);
    curScene->addLine(ulx, uly, ulx, uly + h);
    curScene->addLine(ulx + w, uly, ulx + w, uly + h);
    curScene->addLine(ulx, uly + h, ulx + w, uly + h);

    baseScene->addLine(ulx, uly, ulx + w, uly);
    baseScene->addLine(ulx, uly, ulx, uly + h);
    baseScene->addLine(ulx + w, uly, ulx + w, uly + h);
    baseScene->addLine(ulx, uly + h, ulx + w, uly + h);
}

void Picture::drawCircle(double x, double y, double r)
{
    drawEllipsis(x, y, r, r);
}

void Picture::drawEllipsis(double x, double y, double a, double b)
{
    double angleCurve = getMinAngleCurve(x, y, a, b);
    double angle = 0;

    double f_x = x + a, f_y = y, s_x, s_y;
    while (angle < M_PI * 2)
    {
        s_x = x + a * cos(angle);
        s_y = y + b * sin(angle);

        curScene->addLine(f_x, f_y, s_x, s_y);
        baseScene->addLine(f_x, f_y, s_x, s_y);

        angle += angleCurve;
        f_x = s_x;
        f_y = s_y;
    }
}

void Picture::drawHalfEllipsis(double x, double y, double a, double b)
{
    double angleCurve = getMinAngleCurve(x, y, a, b);
    double angle = 0;

    double f_x = x + a, f_y = y, s_x, s_y;
    while (angle < M_PI)
    {
        s_x = x + a * cos(angle);
        s_y = y - b * sin(angle);

        curScene->addLine(f_x, f_y, s_x, s_y);
        baseScene->addLine(f_x, f_y, s_x, s_y);

        angle += angleCurve;
        f_x = s_x;
        f_y = s_y;
    }
}

double Picture::getMinAngleCurve(double x, double y, double a, double b)
{
    double x_r = a * a / b;
    double y_r = b * b / a;

    return 1 / std::max(x_r, y_r);
}

static void applyMatrixToPoint(double &x, double &y, double matrix[3][3])
{
    double x_new = x * matrix[0][0] + y * matrix[1][0] + matrix[2][0];
    double y_new = x * matrix[0][1] + y * matrix[1][1] + matrix[2][1];

    x = x_new;
    y = y_new;
}


void Picture::applyToView(QGraphicsView &view)
{
    curScene->clear();
    for (QGraphicsItem *item : baseScene->items()) {
        QGraphicsLineItem *line = (QGraphicsLineItem*)item;

        QLineF oldLine = line->line();
        double x1 = oldLine.p1().x(), y1 = oldLine.p1().y(), x2 = oldLine.p2().x(), y2 = oldLine.p2().y();

        applyMatrixToPoint(x1, y1, curMatrix);
        applyMatrixToPoint(x2, y2, curMatrix);
        QLineF newLine = QLineF(x1, y1, x2, y2);

        QGraphicsLineItem *newItem = new QGraphicsLineItem(newLine);
        newItem->setLine(newLine);
        curScene->addItem(newItem);
    }

    double newCenterX = centerX;
    double newCenterY = centerY;
    applyMatrixToPoint(newCenterX, newCenterY, curMatrix);

    view.setScene(curScene);
    QBrush brush = QBrush(Qt::red);
    curScene->addEllipse(newCenterX - 10, newCenterY - 10, 10, 10, QPen(), brush);
    curScene->addSimpleText(QString("(%1,%2)").arg((round(newCenterX))).arg(round(newCenterY)))->setPos(newCenterX, newCenterY);

    brush = QBrush(Qt::blue);
    curScene->addEllipse(centerX - 10, centerY - 10, 10, 10, QPen(), brush);
    curScene->addSimpleText(QString("(%1,%2)").arg((round(centerX))).arg(round(centerY)))->setPos(centerX, centerY);

    curScene->update();
}


void Picture::drawStart(QGraphicsView * view)
{
    // Прямоугольник дома
    drawRectangle(0, 0, 300, 300);

    // Элипсовое окно
    drawEllipsis(230, 150, 40, 90);
    drawLine(230, 150 - 90, 230, 150 + 90);
    drawLine(230 - 40, 150, 230 + 40, 150);

    // Прямоугольное окно
    drawRectangle(40, 80, 120, 160);
    drawHalfEllipsis(100, 80, 60, 20);
    drawLine(100, 60, 100, 60 + 90 * 2);

    // Крыша
    drawTriangle(0, 0, 100, -120, 300, 0);

    // Окошко на крыше
    double r = 30;
    double x0 = 100, y0 = -50;
    drawCircle(x0, y0, r);


    double x1, x2, y1, y2;

    x1 = x0 - r + 5;
    x2 = x0 + 2;
    y1 = y0 + sqrt(r * r - pow((x0 - x1), 2));
    y2 = y0 - sqrt(r * r - pow((x0 - x2), 2));
    drawLine(x1, y1, x2, y2);

    x1 = x0 - r + 15;
    x2 = x0 + 15;
    y1 = y0 + sqrt(r * r - pow((x0 - x1), 2));
    y2 = y0 - sqrt(r * r - pow((x0 - x2), 2));
    drawLine(x1, y1, x2, y2);

    x1 = x0 - r + 26;
    x2 = x0 + 25;
    y1 = y0 + sqrt(r * r - pow((x0 - x1), 2));
    y2 = y0 - sqrt(r * r - pow((x0 - x2), 2));
    drawLine(x1, y1, x2, y2);

    x1 = x0 - r + 40;
    x2 = x0 + 30;
    y1 = y0 + sqrt(r * r - pow((x0 - x1), 2));
    y2 = y0 - sqrt(r * r - pow((x0 - x2), 2));
    drawLine(x1, y1, x2, y2);
}

void Picture::drawTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
{
    curScene->addLine(x1, y1, x2, y2);
    curScene->addLine(x1, y1, x3, y3);
    curScene->addLine(x2, y2, x3, y3);

    baseScene->addLine(x1, y1, x2, y2);
    baseScene->addLine(x1, y1, x3, y3);
    baseScene->addLine(x2, y2, x3, y3);
}

void Picture::drawLine(double x1, double y1, double x2, double y2)
{
    curScene->addLine(x1, y1, x2, y2);
    baseScene->addLine(x1, y1, x2, y2);
}

void Picture::drawBig(QGraphicsView * view)
{
    QList<QGraphicsItem*> items = curScene->items();
    qDebug() << items.size();
    for (QGraphicsItem *item : items)
    {
        //qDebug() << "line";
        QGraphicsLineItem *line = (QGraphicsLineItem *)item;
        QLineF oldLine = line->line();
        QLineF newLine = QLineF(oldLine.p1().x() * 1.25, oldLine.p1().y() * 1.25, oldLine.p2().x() * 1.25, oldLine.p2().y() * 1.25);
        line->setLine(newLine);
    }
}

double Picture::toRadians(double angle)
{
    return angle * M_PI / 180.0;
}

void Picture::getRotateMatrix(double x, double y, double angle, double matrix[3][3])
{
    double radians = toRadians(angle);

    getTransferMatrix(-x, -y, matrix);

    double secondMatrix[3][3];
    memset(secondMatrix, 0, sizeof(double) * 3 * 3);

    secondMatrix[0][0] = cos(radians);
    secondMatrix[1][0] = sin(radians);
    secondMatrix[0][1] = -sin(radians);
    secondMatrix[1][1] = cos(radians);
    secondMatrix[2][2] = 1;

    productMatrix(matrix, secondMatrix);

    getTransferMatrix(x, y, secondMatrix);

    productMatrix(matrix, secondMatrix);
}

void Picture::getTransferMatrix(double x, double y, double matrix[3][3])
{
    memset(matrix, 0, sizeof(double) * 3 * 3);
    matrix[0][0] = 1;
    matrix[1][1] = 1;
    matrix[2][2] = 1;
    matrix[2][0] = x;
    matrix[2][1] = y;
}

void Picture::applyMatrix(double matrix[3][3])
{
    memmove(prevMatrix, curMatrix, sizeof(double) * 3 * 3);
    productMatrix(curMatrix, matrix);
}

void Picture::getScaleMatrix(double x, double y, double kx, double ky, double matrix[3][3])
{
    getTransferMatrix(-x, -y, matrix);

    double secondMatrix[3][3];
    secondMatrix[0][0] = kx;
    secondMatrix[1][1] = ky;
    secondMatrix[2][2] = 1;

    productMatrix(matrix, secondMatrix);

    getTransferMatrix(x, y, secondMatrix);

    productMatrix(matrix, secondMatrix);
}
void Picture::productMatrix(double matrixA[3][3], double matrixB[3][3])
{
    double matrixTemp[3][3];
    memset(matrixTemp, 0, sizeof(double) * 3 * 3);

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            double sum = 0;
            for (int k = 0; k < 3; ++k)
            {
                sum += matrixA[i][k] * matrixB[k][j];
            }

            matrixTemp[i][j] = sum;
        }
    }

    memmove(matrixA, matrixTemp, sizeof(double) * 3 * 3);
}

QGraphicsScene* Picture::curScene = nullptr;
QGraphicsScene* Picture::baseScene = nullptr;
double Picture::curMatrix[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
double Picture::prevMatrix[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
double Picture::centerX = 150;
double Picture::centerY = 150;
