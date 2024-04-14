#ifndef PICTURE_H
#define PICTURE_H

#include <QGraphicsScene>
#include <QGraphicsView>

class Picture
{
public:
    static inline Picture& getInstance()
    {
        static Picture instance;
        return instance;
    }

    static void drawStart(QGraphicsView * view);
    static void applyToView(QGraphicsView &view);
    static void drawBig(QGraphicsView * view);
    static void getRotateMatrix(double x, double y, double angle, double matrix[3][3]);
    static void getTransferMatrix(double x, double y, double matrix[3][3]);
    static void getScaleMatrix(double x, double y, double kx, double ky, double matrix[3][3]);
    static void productMatrix(double matrixA[3][3], double matrixB[3][3]);
    static void applyMatrix(double matrix[3][3]);
    static void undo();
    static void reset();
    static double centerX;
    static double centerY;

private:
    Picture();
    Picture(const Picture &);
    Picture& operator= (Picture &);
    ~Picture();

    static QGraphicsScene *curScene;
    static QGraphicsScene *baseScene;


    static double curMatrix[3][3];
    static double prevMatrix[3][3];

    static void drawEllipsis(double x, double y, double a, double b);
    static void drawHalfEllipsis(double x, double y, double a, double b);
    static void drawLine(double x1, double y1, double x2, double y2);
    static void drawTriangle(double x1, double y1, double x2, double y2, double x3, double y3);
    static void drawCircle(double x, double y, double r);
    static void drawRectangle(int ulx, int uly, int w, int h);



    static double toRadians(double angle);

    static double getMinAngleCurve(double x, double y, double a, double b);


};

#endif // PICTURE_H
