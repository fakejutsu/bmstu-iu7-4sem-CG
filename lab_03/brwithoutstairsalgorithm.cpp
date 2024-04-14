#include "brwithoutstairsalgorithm.h"

BrWithoutStairsAlgorithm::BrWithoutStairsAlgorithm() {}

const int maxI = 255;

static int sign(double val)
{
    return  (0 < val) - (0 > val);
}

static void setTransparency(int I, QPainter & painter)
{
    QColor color = painter.pen().color();
    color.setAlpha(I);
    QPen pen = QPen(color);
    painter.setPen(pen);
}

QString BrWithoutStairsAlgorithm::getName()
{
    return "Брезенхем с устранением ступенчатости";
}

void BrWithoutStairsAlgorithm::getLinePoints(QPoint first, QPoint second, QList<QPair<QPoint, int>> &pointsAlpha)
{
    if (first.x() == second.x() && first.y() == second.y())
    {
        QPair<QPoint, int> pointAlpha = QPair<QPoint, int>(first, 255);
        pointsAlpha.append(pointAlpha);
        return;
    }

    int x = first.x();
    int y = first.y();

    int dx = second.x() - first.x();
    int dy = second.y() - first.y();

    int sx = sign(dx);
    int sy = sign(dy);

    dx = abs(dx);
    dy = abs(dy);


    bool needSwap = (dx < dy);
    if (needSwap)
    {
        int t = dx;
        dx = dy;
        dy = t;
    }

    double m = maxI * double(dy) / double(dx);
    double e = double(maxI) * 0.5;


    double w = maxI - m;

    QPoint point = QPoint(x, y);
    QPair<QPoint, int> pointAlpha = QPair<QPoint, int>(point, round(e));
    pointsAlpha.append(pointAlpha);

    for (int i = 0; i < dx; ++i)
    {
        if (e < w)
        {
            if (needSwap)
            {
                y += sy;
            }
            else
            {
                x += sx;
            }

            e += m;
        }

        else
        {
            y += sy;
            x += sx;
            e -= w;
        }

        QPoint point = QPoint(x, y);
        QPair<QPoint, int> pointAlpha = QPair<QPoint, int>(point, round(e));
        pointsAlpha.append(pointAlpha);
    }
}
