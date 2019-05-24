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
#include "ColorArray.hpp"

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
                , _segColors(new sf::ColorArray())
            {
                IsVisible = true;
                XStart = 0.0;
                XDelta = 1.0;
            }

            property bool IsVisible;

            property Color TraceColor;

            /// <summary>
            /// X value of the first x data value
            /// The default value is 0
            /// </summary>
            property double XStart;

            /// <summary>
            /// X delta between each x data value
            /// The default value is 1
            /// </summary>
            property double XDelta;

            // clang-format off
            property array<Color> ^ SegColors
            {
                void set(array<Color> ^ value);
            }

            virtual void Draw(sf::RenderTarget* target, sf::RenderStates states);
            // clang-format on

            /// <summary>
            /// Set trace data in (x,y) format.
            /// The color for trace is set by TraceColor
            /// </summary>
            /// <param name="data">The trace data.</param>
            virtual void SetXYData(float* pData, int dataSize);

            /// <summary>
            /// Set trace data in (x,y) format
            /// The color for trace is set by SegColors
            /// </summary>
            /// <param name="data">The trace data</param>
            /// <param name="segIndexes">The indexes for SegColors</param>
            virtual void SetXYData(float* pData, int dataSize, short* pSegIndexes, int segIndexesSize);

            /// <summary>
            /// Set trace data in y format. x is generated linearly from 0 to length - 1
            /// The color for trace is set by SegColors
            /// </summary>
            /// <param name="yData">The trace data for y-axis</param>
            /// <param name="segIndexes">The indexes for SegColors</param>
            virtual void SetData(float* pYData, int yDataSize, short* pSegIndexes, int segIndexesSize);

        protected:
            scoped_ptr<sf::VertexArray> _vertices;
            scoped_ptr<sf::ColorArray> _segColors;
            uint32_t _invalidSegIndex = 0; // The index used to indicate data will not be used
        };
    }
}
}
