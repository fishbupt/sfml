// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include <ColorfulPointsShape.hpp>
#include <Utils.hpp>

namespace Xsa::Presentation::Graph
{
    public ref class SegmentPointsShape : public ColorfulPointsShape
    {
    public:
        SegmentPointsShape();

        /// <summary>
        /// X value of the first x data value
        /// The default value is 0
        /// </summary>
        property double XStart;

        /// <summary>
        /// X delta between each x data value
        /// The default value is 1
        /// </summary>
        property double XDelta;

        property int SegmentCount;

        /// <summary>
        /// Set trace data in y format. x is generated linearly from 0 to length - 1
        /// The color for trace is set by Colors
        /// </summary>
        /// <param name="yData">The trace data for y-axis</param>
        /// <param name="segIndexes">The indexes for Colors</param>
        virtual void SetYData(float* pYData, int yDataSize, short* pColorIndexes, int IndexesSize);
    };
}
