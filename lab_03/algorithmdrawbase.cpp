#include "algorithmdrawbase.h"
#include "qpainter.h"

AlgorithmDrawBase::AlgorithmDrawBase() {
    this->painter = new QPainter();
}

AlgorithmDrawBase::~AlgorithmDrawBase() {
    delete this->painter;
}

void AlgorithmDrawBase::setColor(QColor color)
{
    lineColor = color;
}

QPainter & AlgorithmDrawBase::GetPainter()
{
    return *(this->painter);
}

void AlgorithmDrawBase::SetLineColor(QPainter & painter)
{
    this->painter->setPen(lineColor);
}
