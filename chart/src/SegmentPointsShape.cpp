// Copyright (c) 2019 Keysight Technologies. All rights reserved.

#include <SegmentPointsShape.hpp>

namespace Xsa::Presentation::Graph
{
    SegmentPointsShape::SegmentPointsShape()
    {
        SegmentCount = 1;
        XStart = 0.0;
        XDelta = 1.0;
    }

    void SegmentPointsShape::SetYData(float* pYData, int yDataSize, short* pColorIndexes, int colorIndexesSize)
    {
        if (yDataSize == 0 || colorIndexesSize == 0 || !pYData || !pColorIndexes)
            return;
        if (yDataSize % 2 != 0)
            return throw gcnew InvalidOperationException();

        int pointSize = yDataSize;
        _vertices->resize(pointSize);
        sf::VertexArray& vertices = *_vertices;
        sf::ColorArray& colors = *_colors;

        float xPos = 0;
        for (int i = 0; i < pointSize; i++)
        {
            vertices[i].color = colors[pColorIndexes[i]];
            vertices[i].position.x = (float)(xPos * XDelta + XStart);
            vertices[i].position.y = pYData[i];
            xPos++;
            if (xPos == SegmentCount)
                xPos = 0;
        }
    }
}
