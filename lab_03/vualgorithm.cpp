#include "vualgorithm.h"
#include <cmath>

static bool needSwap = false;

VuAlgorithm::VuAlgorithm() {}

QString VuAlgorithm::getName()
{
    return "Ву";
}


static int sign(double val)
{
    return  (0 < val) - (0 > val);
}


static double frac(double f)
{
    return f - (int)(f);
}

static double rfrac(double f)
{
    return 1 - (f - (int)(f));
}

void VuAlgorithm::getLinePoints(QPoint first, QPoint second, QList<QPair<QPoint, int>> &pointsAlpha)
{
    if (first.x() == second.x() && first.y() == second.y())
    {
        QPair<QPoint, int> pointAlpha = QPair<QPoint, int>(first, 255);
        pointsAlpha.append(pointAlpha);
        return;
    }

    int x1 = first.x();
    int x2 = second.x();
    int y1 = first.y();
    int y2 = second.y();



    needSwap = abs(y2 - y1) > abs(x2 - x1);
    if (needSwap)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if (x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    int dx = x2 - x1;
    int dy = y2 - y1;

    double grad = dx == 0 ? 1 : (double)(dy) / (double)(dx);

    int xend = x1;
    int yend = y1 + grad * (xend - x1);
    double xgap = rfrac(x1 + 0.5);

    double  xpxl1 = xend;
    double ypxl1 = floor(yend);



    if (needSwap)
    {
        QPoint point = QPoint(ypxl1, xpxl1);
        pointsAlpha.append(QPair<QPoint, int>(point, 255 * rfrac(yend) * xgap));

        point = QPoint(ypxl1 + 1, xpxl1);
        pointsAlpha.append(QPair<QPoint, int>(point, 255 * frac(yend) * xgap));
    }
    else
    {
        QPoint point = QPoint(xpxl1, ypxl1);
        pointsAlpha.append(QPair<QPoint, int>(point, 255 * rfrac(yend) * xgap));

        point = QPoint(xpxl1, ypxl1 + 1);
        pointsAlpha.append(QPair<QPoint, int>(point, 255 * frac(yend) * xgap));
    }

    double interY = yend + grad;
    xend = round(x2);
    yend = y2 + grad * (xend - x2);
    xgap = rfrac(x2 + 0.5);

    double xpxl2 = xend;
    double ypxl2 = floor(yend);

    if (needSwap)
    {
        // Последняя
        QPoint point = QPoint(ypxl2, xpxl2);
        pointsAlpha.append(QPair<QPoint, int>(point, 255 * rfrac(yend) * xgap));

        point = QPoint(ypxl2 + 1, xpxl2);
        pointsAlpha.append(QPair<QPoint, int>(point, 255 * frac(yend) * xgap));
    }
    else
    {
        QPoint point = QPoint(xpxl2, ypxl2);
        pointsAlpha.append(QPair<QPoint, int>(point, 255 * rfrac(yend) * xgap));

        point = QPoint(xpxl2, ypxl2 + 1);
        pointsAlpha.append(QPair<QPoint, int>(point, 255 * frac(yend) * xgap));
    }


    int sx = sign(xpxl2 - xpxl1);
    for (int x = xpxl1 + 1; x != xpxl2; x += sx)
    {
        if (needSwap)
        {
            QPoint point = QPoint(floor(interY), x);
            pointsAlpha.append(QPair<QPoint, int>(point, 255 * rfrac(interY)));

            point = QPoint(floor(interY) + 1, x);
            pointsAlpha.append(QPair<QPoint, int>(point, 255 * frac(interY)));
        }
        else
        {
            QPoint point = QPoint(x, floor(interY));
            pointsAlpha.append(QPair<QPoint, int>(point, 255 * rfrac(interY)));

            point = QPoint(x, floor(interY) + 1);
            pointsAlpha.append(QPair<QPoint, int>(point, 255 * frac(interY)));
        }

        interY += grad;
    }

}
