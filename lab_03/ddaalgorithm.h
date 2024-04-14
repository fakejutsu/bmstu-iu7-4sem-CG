#ifndef DDAALGORITHM_H
#define DDAALGORITHM_H

#include "algorithmdrawbase.h"

class DDAAlgorithm : public AlgorithmDrawBase
{
public:
    DDAAlgorithm();
    void getLinePoints(QPoint first, QPoint second, QList<QPair<QPoint, int>> &pointsAlpha) override;
    QString getName() override;
};

#endif // DDAALGORITHM_H
