// Copyright(c) 2019 Keysight Technologies.All rights reserved.
#pragma once
#include <SymbolChart.hpp>
#include <SymbolShape.hpp>

namespace Xsa::Presentation::Graph
{
    PointsShape^ SymbolChart::CreateShape()
    {
        return gcnew SymbolShape();
    }
}
