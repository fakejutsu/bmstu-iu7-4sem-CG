#include "brfloatalgorithm.h"

BrFloatAlgorithm::BrFloatAlgorithm() {}

static int sign(double val)
{
    return  (0 < val) - (0 > val);
}

QString BrFloatAlgorithm::getName()
{
    return "Брезенхем с вещественными числами";
}

void BrFloatAlgorithm::getLinePoints(QPoint first, QPoint second, QList<QPair<QPoint, int>> &pointsAlpha)
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


    bool needSwap = (dx <= dy);
    if (needSwap)
    {
        int t = dx;
        dx = dy;
        dy = t;
    }

    double m = double(dy) / double(dx);


    double e = m - (0.5);

    for (int i = 0; i < dx + 1; ++i)
    {
        QPoint point = QPoint(x, y);
        QPair<QPoint, int> pointAlpha = QPair<QPoint, int>(point, 255);
        pointsAlpha.append(pointAlpha);

        if (e >= 0)
        {
            if (needSwap)
            {
                x += sx;
            }
            else
            {
                y += sy;
            }

            e -= 1;
        }

        if (e < 0)
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
    }
}
