// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once

#include <PointsShape.hpp>

namespace Xsa::Presentation::Graph
{

    public ref class ColorfulPointsShape : public PointsShape

    {
    public:
        ColorfulPointsShape();

        property array<Color>^ Colors
        {
            array<Color>^ get();
            void set(array<Color>^ value);
        }

        /// <summary>
        /// Set trace data in (x,y) format
        /// The color for trace is set by Colors
        /// </summary>
        /// <param name="data">The trace data</param>
        /// <param name="colorIndexes">The indexes for Colors</param>
        virtual void SetXYData(float* pData, int dataSize, short* pColorIndexes, int colorIndexesSize);


        /// <summary>
        /// Set trace data in x, y array seperately
        /// </summary>
        /// <param name="xData">data contains x array</param>
        /// <param name="yData">data contains y array</param>
        /// <param name="colorIndexes">The indexes for Colors</param>
        virtual void SetXYData(float* xData, float* yData, int dataSize, short* pColorIndexes, int colorIndexsSize);

        /// Set trace data in (x,y,z) format
        /// The color for trace is set by SegColors
        /// </summary>
        /// <param name="data">The trace data</param>
        /// <param name="segIndexes">The indexes for SegColors</param>
        virtual void SetXYZData(float* pData, int dataSize, short* pSegIndexes, int colorIndexesSize);

    protected:
        scoped_ptr<sf::ColorArray>  _colors;
        int _invalidColorIndex = 0; // The index to represent data will not be used
    };
}
