// Copyright (c) 2019 Keysight Technologies. All rights reserved.

#include <SegmentPointsShape.hpp>

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        void SegmentPointsShape::SetData(float* pYData, int yDataSize, short* pSeg, int segIndexesSize)
        {
            if (yDataSize == 0 || segIndexesSize == 0 || !pYData || !pSeg)
                return;
            if (yDataSize % 2 != 0)
                return throw gcnew InvalidOperationException();

            int pointSize = yDataSize;
            _vertices->resize(pointSize);
            sf::VertexArray& vertices = *_vertices;

            float xPos = 0;
            for (int i = 0; i < pointSize; i++)
            {
                vertices[i].color = ColorUtil::ColorFrom(SegColors[pSeg[i]]);
                vertices[i].position.x = xPos;
                vertices[i].position.y = pYData[i];
                xPos++;
                if (xPos == SegmentCount)
                    xPos = 0;
            }
        }
    }
}
}
