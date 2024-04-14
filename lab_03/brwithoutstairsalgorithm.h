#ifndef BRWITHOUTSTAIRSALGORITHM_H
#define BRWITHOUTSTAIRSALGORITHM_H

#include "algorithmdrawbase.h"
class BrWithoutStairsAlgorithm : public AlgorithmDrawBase
{
public:
    BrWithoutStairsAlgorithm();
    void getLinePoints(QPoint first, QPoint second, QList<QPair<QPoint, int>> &pointsAlpha) override;
    QString getName() override;
};

#endif // BRWITHOUTSTAIRSALGORITHM_H
