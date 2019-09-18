// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#include <SegmentMinMaxChart.hpp>
#include <SegmentMinMaxShape.hpp>

namespace Xsa::Presentation::Graph
{
    SegmentMinMaxChart::SegmentMinMaxChart()
        : ColorfulScatterChart()
    {
        IsPolorCoordinate = false;
    }

    PointsShape^ SegmentMinMaxChart::CreateShape()
    {
        return gcnew SegmentMinMaxPointsShape();
    }
}
