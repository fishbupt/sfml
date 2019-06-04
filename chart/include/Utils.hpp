// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include <SFML/Graphics/Color.hpp>

#using "WindowsBase.dll"
#using "PresentationFramework.dll"
#using "PresentationCore.dll"

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Media;

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        ref class ColorUtil
        {
        public:
            static Color ColorFrom(sf::Color color)
            {
                return Color::FromArgb(color.a, color.r, color.g, color.b);
            }

            static sf::Color ColorFrom(Color color)
            {
                return sf::Color(color.R, color.G, color.B, color.A);
            }
        };

        ref class MathUtil
        {
        public:
            /// <summary>
            /// Check value is in range [low, high)
            /// </summary>
            /// <param name="value"></param>
            /// <param name="low"></param>
            /// <param name="high"></param>
            /// <returns></returns>
            static bool IsInRange(const float& value,  const float& low, const float& high)
            {
                return !(value < low) && (value < high);
            }
        };
    }
}
}
