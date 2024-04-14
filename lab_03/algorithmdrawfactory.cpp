#include "algorithmdrawfactory.h"
#include "brfloatalgorithm.h"
#include "brintalgorithm.h"
#include "brwithoutstairsalgorithm.h"
#include "ddaalgorithm.h"
#include "internalalgorithm.h"
#include "vualgorithm.h"

AlgorithmDrawFactory::AlgorithmDrawFactory() {}

std::shared_ptr<AlgorithmDrawBase> AlgorithmDrawFactory::newAlgorithm(AlgorithmDrawType type)
{
    switch (type)
    {
        case AlgorithmDrawType::DDA:
            return std::make_shared<DDAAlgorithm>();

        case AlgorithmDrawType::BR_FLOAT:
            return std::make_shared<BrFloatAlgorithm>();;

        case AlgorithmDrawType::BR_INT:
            return std::make_shared<BrIntAlgorithm>();

        case AlgorithmDrawType::BR_WITHOUT_STAIRS:
            return std::make_shared<BrWithoutStairsAlgorithm>();

        case AlgorithmDrawType::VU:
            return std::make_shared<VuAlgorithm>();

        default:
            return std::make_shared<DDAAlgorithm>();
    }
}
