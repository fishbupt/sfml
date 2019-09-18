// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include <ScatterChart.hpp>

namespace Xsa::Presentation::Graph
{
    public ref class SymbolChart : public ScatterChart
    {
    public:

        PointsShape^ CreateShape() override;
    };
}
