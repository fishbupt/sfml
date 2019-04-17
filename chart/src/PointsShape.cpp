#include "PointsShape.hpp"
#include <Utils.hpp>

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        void PointsShape::Draw(sf::RenderTarget* target, sf::RenderStates states)
        {
            if (IsVisible && _vertices->getVertexCount() != 0)
            {
                target->draw(*_vertices, states);
            }
        }

        void PointsShape::SetXYData(SpanWrapper<float> data)
        {
            if (data.Length == 0)
                return;
            if (data.Length % 2 != 0)
                return throw gcnew InvalidOperationException();

            int pointSize = data.Length / 2;
            _vertices->resize(pointSize);
            sf::VertexArray& vertices = *_vertices;
            pin_ptr<float> pData = (float*)data.ToPointer();
            sf::Color color = ColorUtil::ColorFrom(TraceColor);
            for (int i = 0; i < pointSize; i++)
            {
                vertices[i].color = color;
                vertices[i].position.x = pData[2 * i];
                vertices[i].position.y = pData[2 * i + 1];
            }
        }

        void PointsShape::SetXYData(SpanWrapper<float> data, SpanWrapper<short> segIndexes)
        {
            if (data.Length == 0 || segIndexes.Length == 0)
                return;
            if (data.Length % 2 != 0)
                return throw gcnew InvalidOperationException();

            int pointSize = data.Length / 2;
            _vertices->resize(pointSize);
            sf::VertexArray& vertices = *_vertices;
            pin_ptr<float> pData = (float*)data.ToPointer();
            pin_ptr<short> pIdx = (short*)segIndexes.ToPointer();

            for (int i = 0; i < pointSize; i++)
            {
                vertices[i].color = ColorUtil::ColorFrom(SegColors[pIdx[i]]);
                vertices[i].position.x = pData[2 * i];
                vertices[i].position.y = pData[2 * i + 1];
            }
        }

        void PointsShape::SetData(SpanWrapper<float> yData, SpanWrapper<short> segIndexes)
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
            }
        }
    }
}
}
