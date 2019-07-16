// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include<SFML/Graphics/Octahedron.hpp>
#include<SFML/Graphics/Transformable.hpp>
#include <Utils.hpp>
#include <ScopedPtr.hpp>
#include <Drawable.hpp>

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        // clang-format off
        public ref class Marker 
        // clang-format on
        {
        public:
            Marker();

            ~Marker();

            !Marker();

            /// <summary>
            /// Whether the marker is displayed
            /// </summary>
            property bool Visible;

            /// <summary>
            /// X-axis value of the marker
            /// </summary>
            property float X;

            /// <summary>
            /// Y-axis value of the marker
            /// </summary>
            property float Y;

            /// <summary>
            /// Z-axis value of the marker
            /// </summary>
            property float Z;

            /// <summary>
            /// The color of the marker if it is active
            /// </summary>
            property Color ActiveColor;

            /// <summary>
            /// The color of the marker if it is inactive
            /// </summary>
            property Color InactiveColor;

            /// <summary>
            /// Whether the marker is active
            /// ActiveColor will be used if Active is true,
            /// otherwise InactiveColor is used
            /// </summary>
            property bool Active;

            /// <summary>
            /// Set Scale to change the size of marker
            /// </summary>
            /// <param name="scaleX">Scaling factor in x dimension</param>
            /// <param name="scaleY">Scaling factor in y dimension</param>
            /// <param name="scaleZ">Scaling factor in z dimension</param>
            void Scale(float scaleX, float scaleY, float scaleZ);

            void Draw(sf::RenderTarget* target, sf::RenderStates states, sf::Transform transform);
        private:
            scoped_ptr<sf::Octahedron> _shape;
        };
    }
} // Presentation
} // Xsa
