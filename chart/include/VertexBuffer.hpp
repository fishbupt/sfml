// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include <vector>
#include <ScopedPtr.hpp>
#include <Drawable.hpp>
#include <VertexArray.hpp>
#include <PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

namespace Xsa
{
namespace Presentation
{
namespace Graph
{
    /// <summary>
    /// Define a set of 3D primitives in GPU memory
    ///
    /// </summary>
public
    ref class VertexBuffer : public Drawable
    {
    public:
        ////////////////////////////////////////////////////////////
        /// \brief Usage specifiers
        ///
        /// If data is going to be updated once or more every frame,
        /// set the usage to Stream. If data is going to be set once
        /// and used for a long time without being modified, set the
        /// usage to Static. For everything else Dynamic should be a
        /// good compromise.
        ///
        ////////////////////////////////////////////////////////////
        enum class Usage
        {
            Stream,  ///< Constantly changing data
            Dynamic, ///< Occasionally changing data
            Static   ///< Rarely changing data
        };

        /// <summary>
        /// Default Construct
        /// </summary>
        VertexBuffer();

        ~VertexBuffer();

        !VertexBuffer();

        /// <summary>
        /// Type of primitives to draw
        /// </summary>
        property PrimitiveType Type;

        /// <summary>
        /// Usage specifier of this vertex buffer
        /// </summary>
        property Usage DrawUsage;

        /// <summary>
        /// The number of elements in the vertex array
        /// </summary>
        property int Size;


        /// <summary>
        /// Update the whole buffer from an array of vertices
        /// </summary>
        bool Update(VertexArray^ vertices);

        virtual void Draw(sf::RenderTarget* target, sf::RenderStates states);

        void Draw(IntPtr target, IntPtr states);

    private:
        scoped_ptr<sf::VertexBuffer> _ptrImpl;
    };

} // namespace Xsa
} // namespace Presentation
} // namespace Graph
