// Copyright (c) 2019 Keysight Technologies. All rights reserved.

#include <ColorfulPointsShape.hpp>
#include <Utils.hpp>

namespace Xsa::Presentation::Graph
{
    ColorfulPointsShape::ColorfulPointsShape()
        : _colors(new sf::ColorArray())
    {
    }

    void ColorfulPointsShape::Colors::set(array<Color>^ value)
    {
        int count = value->Length;
        _colors->resize(count);
        for (int i = 0; i < count; i++)
        {
            (*_colors)[i] = ColorUtil::ColorFrom(value[i]);
            if (i == _invalidColorIndex)
                (*_colors)[i] = sf::Color::Transparent;
        }
    }

    array<Color>^ ColorfulPointsShape::Colors::get()
    {
        int count = (int)_colors->size();
        array<Color>^ colors = gcnew array<Color>(count);
        for (int i = 0; i < count; i++)
        {
            colors[i] = ColorUtil::ColorFrom((*_colors)[i]);
        }
        return colors;
    }

    void ColorfulPointsShape::SetXYData(float* pData, int dataSize, short* pColorIndexes, int colorIndexesSize)
    {
        if (dataSize == 0 || colorIndexesSize == 0 || !pData || !pColorIndexes)
            return;
        if (dataSize % 2 != 0)
            return throw gcnew InvalidOperationException();

        int pointSize = dataSize / 2;
        _vertices->resize(pointSize);
        sf::VertexArray& vertices = *_vertices;
        sf::ColorArray colors = *_colors;

        for (int i = 0; i < pointSize; i++)
        {
            auto& vertex = vertices[i];
            vertex.color = colors[pColorIndexes[i]];
            vertex.position.x = pData[2 * i];
            vertex.position.y = pData[2 * i + 1];
        }
    }

    void ColorfulPointsShape::SetXYData(float* xData, float* yData, int dataSize, short* pColorIndexes, int colorIndexesSize)
    {
        if (dataSize == 0 || colorIndexesSize == 0 || !xData || !yData || !pColorIndexes)
            return;

        int pointSize = dataSize;
        _vertices->resize(pointSize);
        sf::VertexArray& vertices = *_vertices;
        sf::ColorArray colors = *_colors;

        for (int i = 0; i < pointSize; i++)
        {
            auto& vertex = vertices[i];
            vertex.color = colors[pColorIndexes[i]];
            vertex.position.x = xData[i];
            vertex.position.y = yData[i];
        }
    }

    void ColorfulPointsShape::SetXYZData(float* pData, int dataSize, short* pColorIndexes, int colorIndexesSize)
    {
        if (dataSize == 0 || colorIndexesSize == 0 || !pData || !pColorIndexes)
            return;
        if (dataSize % 3 != 0)
            return throw gcnew InvalidOperationException();

        int pointSize = dataSize / 3;
        _vertices->resize(pointSize);
        sf::VertexArray& vertices = *_vertices;
        sf::ColorArray colors = *_colors;

        for (int i = 0; i < pointSize; i++)
        {
            auto& vertex = vertices[i];
            vertex.color = colors[pColorIndexes[i]];
            vertex.position.x = pData[3 * i];
            vertex.position.y = pData[3 * i + 1];
            vertex.position.z = pData[3 * i + 2];
        }
    }
}
