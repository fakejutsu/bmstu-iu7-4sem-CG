#include "ddaalgorithm.h"

DDAAlgorithm::DDAAlgorithm() {}

QString DDAAlgorithm::getName()
{
    return "ЦДА";
}

void DDAAlgorithm::getLinePoints(QPoint first, QPoint second, QList<QPair<QPoint, int>> &pointsAlpha)
{
    if (first.x() == second.x() && first.y() == second.y())
    {
        QPair<QPoint, int> pointAlpha = QPair<QPoint, int>(first, 255);
        pointsAlpha.append(pointAlpha);
        return;
    }

    double x = first.x();
    double y = first.y();

    double dx = second.x() - first.x();
    double dy = second.y() - first.y();

    int l = abs(dy);
    if (abs(dx) > abs(dy))
    {
        l = abs(dx);
    }

    dx /= l;
    dy /= l;

    for (int i = 0; i < l + 1; ++i)
    {
        QPoint point = QPoint(round(x), round(y));
        QPair<QPoint, int> pointAlpha = QPair<QPoint, int>(point, 255);
        pointsAlpha.append(pointAlpha);

        x += dx;
        y += dy;
    }
}
