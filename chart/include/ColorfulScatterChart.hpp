// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include <ScatterChart.hpp>

namespace Xsa::Presentation::Graph
{
    public ref class ColorfulScatterChart : public ScatterChart
    {
    public:
        ColorfulScatterChart();

        property array<Color>^ SymbolColors
        {
            array<Color>^ get();
            void set(array<Color>^ value);
        }

        PointsShape^ CreateShape() override;

        void InitShape(int shapeIndex) override;

    private:
        array<Color>^ _symbolColors;
    };
}
