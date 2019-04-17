// Copyright (c) 2019 Keysight Technologies. All rights reserved.

#include <SegmentPointsShape.hpp>

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        void SegmentPointsShape::SetData(SpanWrapper<float> yData, SpanWrapper<short> segIndexes)
        {
            if (yData.Length == 0 || segIndexes.Length == 0)
                return;
            if (segIndexes.Length != yData.Length)
                return throw gcnew InvalidOperationException();

            int pointSize = yData.Length;
            _vertices->resize(pointSize);
            sf::VertexArray& vertices = *_vertices;
            pin_ptr<float> pData = (float*)yData.ToPointer();
            pin_ptr<short> pIdx = (short*)segIndexes.ToPointer();

            float xPos = 0;
            for (int i = 0; i < pointSize; i++)
            {
                vertices[i].color = ColorUtil::ColorFrom(SegColors[pIdx[i]]);
                vertices[i].position.x = xPos;
                vertices[i].position.y = pData[i];
                xPos++;
                if (xPos == SegmentCount)
                    xPos = 0;
            }
        }
    }
}
}
