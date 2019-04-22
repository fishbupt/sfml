// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include <PointsShape.hpp>
#include <Utils.hpp>

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        // clang-format off
        public ref class SegmentPointsShape : public PointsShape
        // clang-format on
        {
        public:
            SegmentPointsShape()
                : PointsShape()
            {
                SegmentCount = 1;
            }

            property int SegmentCount;

            virtual void SetData(float* pYData, int yDataSize, short* pSeg, int segIndexesSize) override;
        };
    }
}
}
