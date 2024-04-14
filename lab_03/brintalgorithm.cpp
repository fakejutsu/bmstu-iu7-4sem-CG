#include "brintalgorithm.h"

BrIntAlgorithm::BrIntAlgorithm() {}

static int sign(double val)
{
    return  (0 < val) - (0 > val);
}

QString BrIntAlgorithm::getName()
{
    return "Брезенхем с целыми числами";
}

void BrIntAlgorithm::getLinePoints(QPoint first, QPoint second, QList<QPair<QPoint, int>> &pointsAlpha)
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


    int e = 2 * dy - dx;

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

            e -= 2 * dx;
        }


            if (needSwap)
            {
                y += sy;
            }
            else
            {
                x += sx;
            }

            e += 2 * dy;
    }
}
