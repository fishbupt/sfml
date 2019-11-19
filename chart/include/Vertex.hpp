// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include <Color.hpp>
#include <Vector3.hpp>
using namespace System;
using namespace System::Runtime::InteropServices;

namespace Xsa
{
namespace Presentation
{
namespace Graph
{
    /// <summary>
    /// Define a point with color and texture coordinates
    /// </summary>
    [StructLayout(LayoutKind::Sequential)]
    public value struct Vertex
    {
        /// <summary>
        /// 3D position of the vertex
        /// </summary>
        Vector3f Position;

        /// <summary>
        /// Color of the vertex in RGBA format
        /// </summary>
        GlColor Color;

        #ifdef ENABLE_TEXTURE
        /// <summary>
        /// Coordinates of the texture's pixel to map to the vertex
        /// </summary>
        Vector2f TexCoords;
        #endif // ENABLE_TEXTURE


        /// <summary>
        /// Construct the vertex from it's position
        /// </summary>
        /// <param name="position">Vertx position</param>
        Vertex(Vector3f% position) :
            Vertex(position, GlColor(255, 255, 255))
        {
        }

        /// <summary>
        /// Construct the vertex from it's position and color
        /// </summary>
        /// <param name="position">Vertex position</param>
        /// <param name="color">Vertex color</param>
        Vertex(Vector3f% position, GlColor% color) 
        {
            Position = position;
            Color = color;
        }

        #ifdef ENABLE_TEXTURE
        /// <summary>
        /// Construct the vertex from it's position, color and texture coordinates
        /// </summary>
        /// <param name="position">Vertex position</param>
        /// <param name="color">Vertex color</param>
        /// <param name="texCoords">Vertex texture coordinates</param>
        Vertex(Vector3f% position, GlColor% color, Vector2f% texCoords)
        {
            Position = position;
            Color = color;
            TexCoords = texCoords;
        }
        #endif // ENABLE_TEXTURE


        /// <summary>
        /// Provide a string describing the object
        /// </summary>
        /// <returns>String description of the object</returns>
        String^ ToString() override
        {
            #ifdef ENABLE_TEXTURE
            return String::Format("[Vertex] Position({0}) Color({1}) TexCoords({2})", Position, Color, TexCoords);
            #else
            return String::Format("[Vertex] Position({0}) Color({1}))", Position, Color);
            #endif // ENABLE_TEXTURE
        }
    };
} // namespace Xsa
} // namespace Presentation
} // namespace Graph
