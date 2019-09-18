// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#include <SegmentMinMaxShape.hpp>
#include <limits>

namespace Xsa::Presentation::Graph
{

    void SegmentMinMaxPointsShape::SetYData(float* pYData, int yDataSize, short* pColorIndexes, int colorIndexesSize)
    {
        if (yDataSize == 0 || colorIndexesSize == 0 || !pYData || !pColorIndexes)
            return;
        if (yDataSize % 2 != 0)
            return throw gcnew InvalidOperationException();

        int pointSize = yDataSize;
        _vertices->resize(pointSize);
        _linesVertices->resize(SegmentCount * 2);

        float minValue = std::numeric_limits<float>::lowest();
        float maxValue = std::numeric_limits<float>::max();

        sf::VertexArray& vertices = *_vertices;
        sf::VertexArray& linesVertices = *_linesVertices;
        sf::ColorArray& colors = *_colors;

        for (int i = 0; i < SegmentCount; i++)
        {
            float posX = (float)(i * XDelta + XStart);
            linesVertices[2 * i].position.x = posX;
            linesVertices[2 * i].position.y = maxValue;
            linesVertices[2 * i].color = ColorUtil::ColorFrom(TraceColor);
            linesVertices[2 * i + 1].position.x = posX;
            linesVertices[2 * i + 1].position.y = minValue;
            linesVertices[2 * i + 1].color = ColorUtil::ColorFrom(TraceColor);
        }

        int xPos = 0;
        for (int i = 0; i < pointSize; i++)
        {
            vertices[i].color = colors[pColorIndexes[i]];
            vertices[i].position.x = (float)(xPos * XDelta + XStart);
            float yVal = pYData[i];
            vertices[i].position.y = yVal;

            // set MinMax pairmax
            if (pColorIndexes[i] != _invalidColorIndex)
            {
                if (yVal < linesVertices[2 * xPos].position.y)
                {
                    linesVertices[2 * xPos].position.y = yVal;
                }
                if (yVal > linesVertices[2 * xPos + 1].position.y)
                {
                    linesVertices[2 * xPos + 1].position.y = yVal;
                }
            }
            xPos++;
            if (xPos == SegmentCount)
                xPos = 0;
        }

        // check min/max existence
        for (int i = 0; i < SegmentCount; i++)
        {
            if (linesVertices[2 * i].position.y > linesVertices[2 * i + 1].position.y)
            {
                linesVertices[2 * i].color = sf::Color::Transparent;
                linesVertices[2 * i + 1].color = sf::Color::Transparent;
            }
        }
    }

    void SegmentMinMaxPointsShape::Draw(sf::RenderTarget* target, sf::RenderStates states)
    {
        if (IsVisible && _linesVertices->getVertexCount() != 0)
        {
            target->draw(*_linesVertices, states);
        }
        SegmentPointsShape::Draw(target, states);
    }
}
