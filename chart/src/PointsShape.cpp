#include "PointsShape.hpp"
#include <Utils.hpp>

namespace Xsa::Presentation::Graph
{
    PointsShape::PointsShape()
        : _vertices(new sf::VertexArray(sf::PrimitiveType::Points))
    {
        IsVisible = true;
    }

    PointsShape::~PointsShape()
    {
        this->!PointsShape();
    }

    PointsShape::!PointsShape()
    {}

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
            auto& vertex = vertices[i];
            vertex.color = color;
            vertex.position.x = pData[2 * i];
            vertex.position.y = pData[2 * i + 1];
        }
    }

    void PointsShape::SetXYData(float* xData, float* yData, int dataSize)
    {
        if (dataSize == 0 || !xData || !yData)
            return;

        int pointSize = dataSize;
        _vertices->resize(pointSize);
        sf::VertexArray& vertices = *_vertices;
        sf::Color color = ColorUtil::ColorFrom(TraceColor);
        for (int i = 0; i < pointSize; i++)
        {
            auto& vertex = vertices[i];
            vertex.color = color;
            vertex.position.x = xData[i];
            vertex.position.y = yData[i];
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
            auto& vertex = vertices[i];
            vertex.color = color;
            vertex.position.x = pData[3 * i];
            vertex.position.y = pData[3 * i + 1];
            vertex.position.z = pData[3 * i + 2];
        }
    }

}
