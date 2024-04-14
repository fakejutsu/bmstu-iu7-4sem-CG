#ifndef INTERNALALGORITHM_H
#define INTERNALALGORITHM_H

#include "algorithmdrawbase.h"
class InternalAlgorithm : public AlgorithmDrawBase
{
public:
    InternalAlgorithm();
    void getLinePoints(QPoint first, QPoint second, QList<QPair<QPoint, int>> &pointsAlpha) override;
};

#endif // INTERNALALGORITHM_H
