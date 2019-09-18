// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include <SegmentPointsShape.hpp>
#include <Utils.hpp>

namespace Xsa::Presentation::Graph
{
    // clang-format off
    public ref class SegmentMinMaxPointsShape : public SegmentPointsShape
        // clang-format on
    {
    public:
        SegmentMinMaxPointsShape()
            : SegmentPointsShape()
            , _linesVertices(new sf::VertexArray(sf::PrimitiveType::Lines))
        {
        }

        void SetYData(float* pYData, int yDataSize, short* pColorIndexes, int colorIndexesSize) override;

        void Draw(sf::RenderTarget* target, sf::RenderStates states) override;

    private:
        scoped_ptr<sf::VertexArray> _linesVertices;
    };
}
