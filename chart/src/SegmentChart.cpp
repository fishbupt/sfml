// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#include <SegmentChart.hpp>
#include <SegmentPointsShape.hpp>

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        SegmentChart::SegmentChart()
            : ScatterChart()
        {
            IsPolorCoordinate = false;
        }

        PointsShape ^ SegmentChart::CreateShape()
        {
            return gcnew SegmentPointsShape();
        }
    }
}
}
