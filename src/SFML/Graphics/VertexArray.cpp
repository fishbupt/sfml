////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
VertexArray::VertexArray() :
VertexContainer(0),
m_vertices     (),
m_primitiveType(Points)
{
}


////////////////////////////////////////////////////////////
VertexArray::VertexArray(PrimitiveType type, unsigned int vertexCount) :
VertexContainer(0),
m_vertices     (vertexCount),
m_primitiveType(type)
{
}


////////////////////////////////////////////////////////////
unsigned int VertexArray::getVertexCount() const
{
    return static_cast<unsigned int>(m_vertices.size());
}


////////////////////////////////////////////////////////////
Vertex& VertexArray::operator [](unsigned int index)
{
    return m_vertices[index];
}


////////////////////////////////////////////////////////////
const Vertex& VertexArray::operator [](unsigned int index) const
{
    return m_vertices[index];
}


////////////////////////////////////////////////////////////
void VertexArray::clear()
{
    m_vertices.clear();
}


////////////////////////////////////////////////////////////
void VertexArray::resize(unsigned int vertexCount)
{
    m_vertices.resize(vertexCount);
}


////////////////////////////////////////////////////////////
void VertexArray::append(const Vertex& vertex)
{
    m_vertices.push_back(vertex);
}


////////////////////////////////////////////////////////////
void VertexArray::setPrimitiveType(PrimitiveType type)
{
    m_primitiveType = type;
}


////////////////////////////////////////////////////////////
PrimitiveType VertexArray::getPrimitiveType() const
{
    return m_primitiveType;
}


////////////////////////////////////////////////////////////
FloatBox VertexArray::getBounds() const
{
    if (!m_vertices.empty())
    {
        float left   = m_vertices[0].position.x;
        float top    = m_vertices[0].position.y;
        float front  = m_vertices[0].position.z;
        float right  = m_vertices[0].position.x;
        float bottom = m_vertices[0].position.y;
        float back   = m_vertices[0].position.z;

        for (std::size_t i = 1; i < m_vertices.size(); ++i)
        {
            Vector3f position = m_vertices[i].position;

            // Update left and right
            if (position.x < left)
                left = position.x;
            else if (position.x > right)
                right = position.x;

            // Update top and bottom
            if (position.y < top)
                top = position.y;
            else if (position.y > bottom)
                bottom = position.y;

            // Update front and back
            if (position.z < front)
                front = position.z;
            else if (position.z > back)
                back = position.z;
        }

        return FloatBox(left, top, front, right - left, bottom - top, back - front);
    }
    else
    {
        // Array is empty
        return FloatBox();
    }
}


////////////////////////////////////////////////////////////
void VertexArray::draw(RenderTarget& target, RenderStates states) const
{
    if (!m_vertices.empty())
        target.draw(&m_vertices[0], static_cast<unsigned int>(m_vertices.size()), m_primitiveType, states);
}

} // namespace sf
