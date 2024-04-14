#ifndef VUALGORITHM_H
#define VUALGORITHM_H

#include "algorithmdrawbase.h"
#include "qpixmap.h"

class VuAlgorithm : public AlgorithmDrawBase
{
public:
    VuAlgorithm();
    void getLinePoints(QPoint first, QPoint second, QList<QPair<QPoint, int>> &pointsAlpha) override;
    QString getName() override;
};

#endif // VUALGORITHM_H
