#ifndef ALGORITHMDRAWFACTORY_H
#define ALGORITHMDRAWFACTORY_H

#include "algorithmdrawbase.h"
class AlgorithmDrawFactory
{
public:
    AlgorithmDrawFactory();
    std::shared_ptr<AlgorithmDrawBase> newAlgorithm(AlgorithmDrawType type);
};

#endif // ALGORITHMDRAWFACTORY_H
