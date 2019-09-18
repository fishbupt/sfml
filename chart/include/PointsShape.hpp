// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <cliext/vector>
#include <Drawable.hpp>
#include <ScopedPtr.hpp>
#include <Utils.hpp>
#include <ColorArray.hpp>

#using "PresentationCore.dll"

using namespace System::Windows::Media;
using namespace System;

namespace Xsa::Presentation::Graph
{
    public ref class PointsShape : public Drawable
    {
    public:
        PointsShape();

        virtual ~PointsShape();

        !PointsShape();

        property bool IsVisible;

        property Color TraceColor;



        virtual void Draw(sf::RenderTarget* target, sf::RenderStates states);

        /// <summary>
        /// Set trace data in (x,y) format.
        /// The color for trace is set by TraceColor
        /// </summary>
        /// <param name="data">The trace data.</param>
        virtual void SetXYData(float* pData, int dataSize);

        /// <summary>
        /// Set trace data in x, y array seperately
        /// </summary>
        /// <param name="xData">data contains x array</param>
        /// <param name="yData">data contains y array</param>
        /// <param name="dataSize">size of data</param>
        virtual void SetXYData(float* xData, float* yData, int dataSize);

        /// <summary>
        /// Set trace data in (x,y,z) format.
        /// The color for trace is set by TraceColor
        /// </summary>
        /// <param name="data">The trace data.</param>
        virtual void SetXYZData(float* pData, int dataSize);

    protected:
        scoped_ptr<sf::VertexArray> _vertices;
    };
}
