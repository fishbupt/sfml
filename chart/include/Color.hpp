// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include <cstdint>
using namespace System;
using namespace System::Runtime::InteropServices;

namespace Xsa::Presentation::Graph
{
    using byte = uint8_t;
    /// <summary>
    /// Color is an utility class for manipulating 32 bits RGBA colors.
    /// As SFML use this color format.
    /// </summary>
    [StructLayout(LayoutKind::Sequential)]
    public value struct GlColor : IEquatable<GlColor>
    {
        /// <summary>
        /// Red component of the color
        /// </summary>
        property byte R;

        /// <summary>
        /// Green component of the color
        /// </summary>
        property byte G;

        /// <summary>
        /// Blue component of the color
        /// </summary>
        property byte B;

        /// <summary>
        /// Alpha component of the color
        /// </summary>
        property byte A;

        /// <summary>
        /// Construct the color from its red, green, blue and alpha components
        /// </summary>
        /// <param name="red">Red component</param>
        /// <param name="green">Green component</param>
        /// <param name="blue">Blue component</param>
        /// <param name="alpha">Alpha component</param>
        GlColor(byte red, byte green, byte blue, byte alpha)
        {
            R = red;
            G = green;
            B = blue;
            A = alpha;
        }

        /// <summary>
        /// Construct the color from its red, green, blue components
        /// Set A = 255
        /// </summary>
        /// <param name="red">Red component</param>
        /// <param name="green">Green component</param>
        /// <param name="blue">Blue component</param>
        /// <param name="alpha">Alpha component</param>
        GlColor(byte red, byte green, byte blue) : GlColor(red, green, blue, 255)
        {
        }

        /// <summary>
        /// Construct the color from 32 bits unsigned integer
        /// </summary>
        /// <param name="color">32 bits integer contains the RGBA component(in that order)</param>
        GlColor(uint32_t color)
        {
            R = (byte)(color >> 24);
            G = (byte)(color >> 16);
            B = (byte)(color >> 8);
            A = (byte)color;
        };

        /// <summary>
        /// Convert a color to 32 bits unsigned integer
        /// </summary>
        /// <returns>Color represents as a 32 bits unsigned integer</returns>
        uint32_t ToInteger()
        {
            return (uint32_t)((R << 24) | (G << 16) | (B << 8) | A);
        }

        /// <summary>
        /// Provide a string describing the object
        /// </summary>
        /// <returns>String describing of the object</returns>
        String^ ToString() override
        {
            return System::String::Format("[Color] R({0}) G({1}) B({2}) A({3})", R, G, B, A);
        }

        /// <summary>
        /// Compare two colors to check if they are equal
        /// </summary>
        /// <param name="other">Color to check</param>
        /// <returns>Two colors are equal</returns>
        virtual bool Equals(GlColor other) 
        {
            return ToInteger() == other.ToInteger();
        }

        /// <summary>
        /// Provide a integer describing the object
        /// </summary>
        /// <returns>Hash value of the object</returns>
        int GetHashCode() override
        {
            return (R << 24) | (G << 16) | (B << 8) | A;
        }

        /// <summary>
        /// Compare two colors to check is they are equal
        /// </summary>
        /// <param name="lhs"></param>
        /// <param name="rhs"></param>
        /// <returns>Colors are equal</returns>
        static bool operator ==(GlColor lhs, GlColor rhs)
        {
            return lhs.Equals(rhs);
        }

        /// <summary>
        /// Compare two colors to check is they are not equal
        /// </summary>
        /// <param name="lhs"></param>
        /// <param name="rhs"></param>
        /// <returns>Colors are not equal</returns>
        static bool operator !=(GlColor lhs, GlColor rhs)
        {
            return !lhs.Equals(rhs);
        }

    };
}
