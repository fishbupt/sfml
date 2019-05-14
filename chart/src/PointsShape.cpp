#include "PointsShape.hpp"
#include <Utils.hpp>

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        void PointsShape::SegColors::set(array<Color> ^ value)
        {
            int count = value->Length;
            _segColors->resize(count);
            for (int i = 0; i < count; i++)
            {
                (*_segColors)[i] = ColorUtil::ColorFrom(value[i]);
                if (i == _invalidSegIndex)
                    (*_segColors)[i] = sf::Color::Transparent;
            }
        }

        void PointsShape::Draw(sf::RenderTarget* target, sf::RenderStates states)
        {
            if (IsVisible && _vertices->getVertexCount() != 0)
            {
                target->draw(*_vertices, states);
            }
        }

        void PointsShape::SetXYData(float* pData, int dataSize)
        {
            if (dataSize == 0 || !pData)
                return;
            if (dataSize % 2 != 0)
                return throw gcnew InvalidOperationException();

            int pointSize = dataSize / 2;
            _vertices->resize(pointSize);
            sf::VertexArray& vertices = *_vertices;
            sf::Color color = ColorUtil::ColorFrom(TraceColor);
            for (int i = 0; i < pointSize; i++)
            {
                vertices[i].color = color;
                vertices[i].position.x = pData[2 * i];
                vertices[i].position.y = pData[2 * i + 1];
            }
        }

        void PointsShape::SetXYData(float* pData, int dataSize, short* pSeg, int segIndexesSize)
        {
            if (dataSize == 0 || segIndexesSize == 0 || !pData || !pSeg)
                return;
            if (dataSize % 2 != 0)
                return throw gcnew InvalidOperationException();

            int pointSize = dataSize / 2;
            _vertices->resize(pointSize);
            sf::VertexArray& vertices = *_vertices;
            sf::ColorArray segColors = *_segColors;

            for (int i = 0; i < pointSize; i++)
            {
                vertices[i].color = segColors[pSeg[i]];
                vertices[i].position.x = pData[2 * i];
                vertices[i].position.y = pData[2 * i + 1];
            }
        }

        void PointsShape::SetData(float* pYData, int yDataSize, short* pSeg, int segIndexesSize)
        {
            if (yDataSize == 0 || segIndexesSize == 0 || !pYData || !pSeg)
                return;
            if (yDataSize % 2 != 0)
                return throw gcnew InvalidOperationException();

            int pointSize = yDataSize;
            _vertices->resize(pointSize);
            sf::VertexArray& vertices = *_vertices;
            sf::ColorArray& segColors = *_segColors;

            float xPos = 0;
            for (int i = 0; i < pointSize; i++)
            {
                vertices[i].color = segColors[pSeg[i]];
                vertices[i].position.x = xPos * XDelta + XStart;
                vertices[i].position.y = pYData[i];
                xPos++;
            }
        }

        void PointsShape::SetXYZData(float* pData, int dataSize)
        {
            if (dataSize == 0 || !pData)
                return;
            if (dataSize % 3 != 0)
                return throw gcnew InvalidOperationException();

            int pointSize = dataSize / 3;
            _vertices->resize(pointSize);
            sf::VertexArray& vertices = *_vertices;
            sf::Color color = ColorUtil::ColorFrom(TraceColor);
            for (int i = 0; i < pointSize; i++)
            {
                vertices[i].color = color;
                vertices[i].position.x = pData[3 * i];
                vertices[i].position.y = pData[3 * i + 1];
                vertices[i].position.z = pData[3 * i + 2];
            }
        }
    }
}
}
