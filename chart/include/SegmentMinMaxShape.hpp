// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include <SegmentPointsShape.hpp>
#include <Utils.hpp>

namespace Xsa
{
namespace Presentation
{
    namespace Graph
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

            virtual void SetData(SpanWrapper<float> yData, SpanWrapper<short> segIndexes) override;

            virtual void Draw(sf::RenderTarget* target, sf::RenderStates states) override;

        private:
            scoped_ptr<sf::VertexArray> _linesVertices;
        };
    }
}
}
