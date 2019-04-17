// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include <ScatterChart.hpp>

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        // clang-format off
        public ref class SegmentChart : public ScatterChart
        // clang-format on
        {
        public:
            SegmentChart();

            virtual PointsShape ^ CreateShape() override;
        };
    }
}
}
