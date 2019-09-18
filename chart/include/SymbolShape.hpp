// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once

#include <PointsShape.hpp>

namespace Xsa::Presentation::Graph
{
    public ref class SymbolShape : public PointsShape
    {
    public:

        SymbolShape();

        /// <summary>
        /// Points per symbol for drawing symbol enhancement
        /// </summary>
        property int PointsPerSymbol;

        /// <summary>
        /// Whether to draw trajectroy lines or not
        /// True - not to draw trajectroy lines.
        /// False - to draw trajectory lines.
        /// </summary>
        property bool IsOnlySymbols;

        property Color SymbolColor;

        void SetXYData(float* pData, int dataSize) override;

        void SetXYData(float* xData, float* yData, int dataSize) override;

        void Draw(sf::RenderTarget* target, sf::RenderStates states) override;

    private:
        scoped_ptr<sf::VertexArray> _trajectoryVertices;

    };
}

