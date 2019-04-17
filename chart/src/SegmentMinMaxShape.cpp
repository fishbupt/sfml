// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#include <SegmentMinMaxShape.hpp>
#include <limits>

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {

        void SegmentMinMaxPointsShape::SetData(SpanWrapper<float> yData, SpanWrapper<short> segIndexes)
        {
            if (yData.Length == 0 || segIndexes.Length == 0)
                return;
            if (segIndexes.Length != yData.Length)
                return throw gcnew InvalidOperationException();

            int pointSize = yData.Length;
            _vertices->resize(pointSize);
            _linesVertices->resize(SegmentCount * 2);

            float minValue = std::numeric_limits<float>::min();
            float maxValue = std::numeric_limits<float>::max();

            sf::VertexArray& vertices = *_vertices;
            sf::VertexArray& linesVertices = *_linesVertices;
            pin_ptr<float> pData = yData.ToPointer();
            pin_ptr<short> pIdx = segIndexes.ToPointer();
            for (int i = 0; i < SegmentCount; i++)
            {
                linesVertices[2 * i].position.x = static_cast<float>(i);
                linesVertices[2 * i].position.y = maxValue;
                linesVertices[2 * i].color = ColorUtil::ColorFrom(TraceColor);
                linesVertices[2 * i + 1].position.x = static_cast<float>(i);
                linesVertices[2 * i + 1].position.y = minValue;
                linesVertices[2 * i + 1].color = ColorUtil::ColorFrom(TraceColor);
            }

            int xPos = 0;
            for (int i = 0; i < pointSize; i++)
            {
                vertices[i].color = ColorUtil::ColorFrom(SegColors[pIdx[i]]);
                vertices[i].position.x = static_cast<float>(xPos);
                float yVal = pData[i];
                vertices[i].position.y = yVal;

                // set MinMax pair
                if (yVal < linesVertices[2 * xPos].position.y)
                {
                    linesVertices[2 * xPos].position.y = yVal;
                }
                if (yVal > linesVertices[2 * xPos + 1].position.y)
                {
                    linesVertices[2 * xPos + 1].position.y = yVal;
                }

                xPos++;
                if (xPos == SegmentCount)
                    xPos = 0;
            }
        }

        void SegmentMinMaxPointsShape::Draw(sf::RenderTarget* target, sf::RenderStates states)
        {
            SegmentPointsShape::Draw(target, states);
            if (IsVisible && _linesVertices->getVertexCount() != 0)
            {
                target->draw(*_linesVertices.get(), states);
            }
        }
    }
}
}
