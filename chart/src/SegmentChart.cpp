// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#include <SegmentChart.hpp>
#include <SegmentPointsShape.hpp>

namespace Xsa::Presentation::Graph
{
    SegmentChart::SegmentChart()
        : ColorfulScatterChart()
    {
        IsPolorCoordinate = false;
    }

    PointsShape^ SegmentChart::CreateShape()
    {
        return gcnew SegmentPointsShape();
    }
}
