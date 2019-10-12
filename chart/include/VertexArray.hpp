// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include<vector>
#include<ScopedPtr.hpp>
#include <Drawable.hpp>
#include <Vertex.hpp>
#include <PrimitiveType.hpp>
#include<SFML/Graphics/Vertex.hpp>
#include<SFML/Graphics/PrimitiveType.hpp>


namespace Xsa
{
namespace Presentation
{
namespace Graph
{
    /// <summary>
    /// Define a set of 3D primitives
    ///
    /// </summary>
    public ref class VertexArray : public Drawable
    {
    public:
        /// <summary>
        /// Default Construct
        /// Set primitiveType = Points, size = 0
        /// </summary>
        VertexArray();

        /// <summary>
        /// Construct the vertex array with a type
        /// Set size = 0.
        /// </summary>
        /// <param name="primitiveType"></param>
        VertexArray(PrimitiveType primitiveType);

        /// <summary>
        /// Construct the vertex array with a type and size
        /// </summary>
        /// <param name="primitiveType"></param>
        /// <param name="size"></param>
        VertexArray(PrimitiveType primitiveType, int size);

        ~VertexArray();

        !VertexArray();

        /// <summary>
        /// Constrol the vertex array should be drawn or not
        /// </summary>
        property bool IsVisible;

        /// <summary>
        /// Type of primitives to draw
        /// </summary>
        property PrimitiveType Type;

        /// <summary>
        /// The number of elements in the vertex array
        /// </summary>
        property int Size
        {
            int get();
            void set(int value);
        }

        /// <summary>
        /// The number of elements that the vertex array has currently allocated space for
        /// </summary>
        property int Capacity
        {
            int get();
            void set(int value);
        }

        /// <summary>
        /// Returns pointer to the underlying array
        /// </summary>
        /// <returns></returns>
        Vertex* AsPointer();

        /// <summary>
        /// Clear the vertex array.
        /// This method will release internal memory
        /// </summary>
        void Clear();

        virtual void Draw(sf::RenderTarget* target, sf::RenderStates states);

        void Draw(IntPtr target, IntPtr states);

    private:
        scoped_ptr<std::vector<sf::Vertex>> _vertices;
    };
} // namespace Xsa
} // namespace Presentation
} // namespace Graph
