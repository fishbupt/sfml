// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#include <SegmentMinMaxChart.hpp>
#include <SegmentMinMaxShape.hpp>

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        SegmentMinMaxChart::SegmentMinMaxChart()
            : ScatterChart()
        {
            IsPolorCoordinate = false;
        }

        PointsShape ^ SegmentMinMaxChart::CreateShape()
        {
            return gcnew SegmentMinMaxPointsShape();
        }
    }
}
}
