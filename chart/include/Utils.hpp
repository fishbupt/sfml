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

        // clang-format off
        template<typename T>
        public value struct SpanWrapper
        // clang-format on
        {
        public:
            SpanWrapper(System::IntPtr ptr, int length)
                : _pointer(ptr)
                , _length(length)
            {
            }
            property int Length
            {
                int get()
                {
                    return _length;
                }
            }
            T* ToPointer()
            {
                return reinterpret_cast<T*>(_pointer.ToPointer());
            }

        private:
            System::IntPtr _pointer;
            int _length;
        };
    }
}
}
