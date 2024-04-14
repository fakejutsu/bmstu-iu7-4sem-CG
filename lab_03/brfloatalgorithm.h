#ifndef BRFLOATALGORITHM_H
#define BRFLOATALGORITHM_H

#include "algorithmdrawbase.h"

class BrFloatAlgorithm : public AlgorithmDrawBase
{
public:
    BrFloatAlgorithm();
    void getLinePoints(QPoint first, QPoint second, QList<QPair<QPoint, int>> &pointsAlpha) override;
    QString getName() override;
};

#endif // BRFLOATALGORITHM_H
