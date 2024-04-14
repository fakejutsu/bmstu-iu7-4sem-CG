#ifndef ALGORITHMDRAWBASE_H
#define ALGORITHMDRAWBASE_H

#include "qpainter.h"
#include "qpixmap.h"

enum AlgorithmDrawType
{
    INTERNAL,
    DDA,
    BR_FLOAT,
    BR_INT,
    BR_WITHOUT_STAIRS,
    VU
};

class AlgorithmDrawBase
{
public:
    AlgorithmDrawBase();
    ~AlgorithmDrawBase();

    virtual void getLinePoints(QPoint first, QPoint second, QList<QPair<QPoint, int>> &pointsAlpha) = 0;
    virtual QString getName() = 0;
    void SetLineColor(QPainter & painter);
    void setColor(QColor color);
protected:
    QPainter & GetPainter();
private:
    QColor lineColor;
    QPainter* painter;

};

#endif // ALGORITHMDRAWBASE_H
