// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include <ColorfulScatterChart.hpp>

namespace Xsa::Presentation::Graph
{
    public ref class SegmentChart : public ColorfulScatterChart
    {
    public:
        SegmentChart();

        PointsShape^ CreateShape() override;
    };
}
