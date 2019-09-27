// Copyright (c) 2019 Keysight Technologies. All rights reserved.

#include <SymbolShape.hpp>
#include <cmath>

namespace Xsa::Presentation::Graph
{
    SymbolShape::SymbolShape()
        :PointsShape()
        , _trajectoryVertices(new sf::VertexArray(sf::PrimitiveType::LineStrip))
    {
        PointsPerSymbol = 1;
        IsOnlySymbols = false;
    }

    void SymbolShape::SetXYData(float* pData, int dataSize)
    {
        int dataPerSymbol = PointsPerSymbol * 2;
        if (dataSize == 0 || !pData)
            return;

        int pointSize = (int)std::ceil((float)dataSize / (float)dataPerSymbol);
        _vertices->resize(pointSize);
        sf::VertexArray& vertices = *_vertices;
        sf::Color symbolColor = ColorUtil::ColorFrom(SymbolColor);
        for (int i = 0; i < pointSize; i++)
        {
            auto& vertex = vertices[i];
            vertex.color = symbolColor;
            vertex.position.x = pData[dataPerSymbol * i];
            vertex.position.y = pData[dataPerSymbol * i + 1];
        }
        int lineSize = dataSize / 2;
        _trajectoryVertices->resize(lineSize);
        sf::VertexArray& lineVertices = *_trajectoryVertices;
        sf::Color color = ColorUtil::ColorFrom(TraceColor);
        for (int i = 0; i < lineSize; i++)
        {
            auto& vertex = lineVertices[i];
            vertex.color = color;
            vertex.position.x = pData[2 * i];
            vertex.position.y = pData[2 * i + 1];
        }
    }

    void SymbolShape::SetXYData(float* xData, float* yData, int dataSize)
    {
        int dataPerSymbol = PointsPerSymbol;
        if (dataSize == 0 || !xData || !yData)
            return;

        int pointSize = (int)std::ceil((float)dataSize / (float)dataPerSymbol);
        _vertices->resize(pointSize);
        sf::VertexArray& vertices = *_vertices;
        sf::Color symbolColor = ColorUtil::ColorFrom(SymbolColor);
        for (int i = 0; i < pointSize; i++)
        {
            auto& vertex = vertices[i];
            vertex.color = symbolColor;
            vertex.position.x = xData[dataPerSymbol * i];
            vertex.position.y = yData[dataPerSymbol * i];
        }
        int lineSize = dataSize;
        _trajectoryVertices->resize(lineSize);
        sf::VertexArray& lineVertices = *_trajectoryVertices;
        sf::Color color = ColorUtil::ColorFrom(TraceColor);
        for (int i = 0; i < lineSize; i++)
        {
            auto& vertex = lineVertices[i];
            vertex.color = color;
            vertex.position.x = xData[i];
            vertex.position.y = yData[i];
        }
    }

    void SymbolShape::Draw(sf::RenderTarget* target, sf::RenderStates states)
    {
        if (IsVisible && !IsOnlySymbols)
        {
            target->draw(*_trajectoryVertices, states);
        }
        PointsShape::Draw(target, states);
    }
}
