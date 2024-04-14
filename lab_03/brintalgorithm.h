#ifndef BRINTALGORITHM_H
#define BRINTALGORITHM_H

#include "algorithmdrawbase.h"

class BrIntAlgorithm : public AlgorithmDrawBase
{
public:
    BrIntAlgorithm();
    void getLinePoints(QPoint first, QPoint second, QList<QPair<QPoint, int>> &pointsAlpha) override;
    QString getName() override;
};

#endif // BRINTALGORITHM_H
