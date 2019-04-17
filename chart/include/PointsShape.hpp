// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <cliext/vector>
#include "Drawable.hpp"
#include "ScopedPtr.hpp"
#include "Utils.hpp"

#using "PresentationCore.dll"

using namespace System::Windows::Media;
using namespace System;

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        // clang-format off
        public ref class PointsShape : public Drawable
        // clang-format on
        {
        public:
            PointsShape()
                : _vertices(new sf::VertexArray(sf::PrimitiveType::Points))
            {
                IsVisible = true;
            }

            property bool IsVisible;

            property Color TraceColor;

            property array<Color> ^ SegColors;

            virtual void Draw(sf::RenderTarget* target, sf::RenderStates states);

            /// <summary>
            /// Set trace data in (x,y) format.
            /// The color for trace is set by TraceColor
            /// </summary>
            /// <param name="data">The trace data.</param>
            virtual void SetXYData(SpanWrapper<float> data);

            /// <summary>
            /// Set trace data in (x,y) format
            /// The color for trace is set by SegColors
            /// </summary>
            /// <param name="data">The trace data</param>
            /// <param name="segIndexes">The indexes for SegColors</param>
            virtual void SetXYData(SpanWrapper<float> data, SpanWrapper<short> segIndexes);

            /// <summary>
            /// Set trace data in y format. x is generated linearly from 0 to length - 1
            /// The color for trace is set by SegColors
            /// </summary>
            /// <param name="yData">The trace data for y-axis</param>
            /// <param name="segIndexes">The indexes for SegColors</param>
            virtual void SetData(SpanWrapper<float> yData, SpanWrapper<short> segIndexes);

        protected:
            scoped_ptr<sf::VertexArray> _vertices;
        };
    }
}
}
