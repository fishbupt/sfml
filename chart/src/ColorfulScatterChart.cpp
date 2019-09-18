// Copyright (c) 2019 Keysight Technologies. All rights reserved.

#include <ColorfulScatterChart.hpp>
#include <ColorfulPointsShape.hpp>

namespace Xsa::Presentation::Graph
{
    ColorfulScatterChart::ColorfulScatterChart()
    {
        _symbolColors = gcnew array<Color>(0);
    }

    array<Color>^ ColorfulScatterChart::SymbolColors::get()
    {
        return _symbolColors;
    }

    void ColorfulScatterChart::SymbolColors::set(array<Color>^ value)
    {
        if (_symbolColors != value)
        {
            _symbolColors = value;
            for each (ColorfulPointsShape ^ shape in DataShapes)
            {
                shape->Colors = _symbolColors;
            }
        }
    }

    PointsShape^ ColorfulScatterChart::CreateShape()
    {
        return gcnew ColorfulPointsShape();
    }

    void ColorfulScatterChart::InitShape(int shapeIndex)
    {
        ScatterChart::InitShape(shapeIndex);
        ColorfulPointsShape^ shape = (ColorfulPointsShape^)DataShapes[shapeIndex];
        shape->Colors = SymbolColors;
    }
}
