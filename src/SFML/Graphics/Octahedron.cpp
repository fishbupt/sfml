////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Octahedron.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace sf
{
////////////////////////////////////////////////////////////
Octahedron::Octahedron(const Vector3f& size)
    :m_borderVertices(Lines, 24)
{
    setSize(size);
}


////////////////////////////////////////////////////////////
void Octahedron::setSize(const Vector3f& size)
{
    if (m_size == size)
        return;

    m_size = size;
    update();
    updateBorder();
    generateNormals();
}


////////////////////////////////////////////////////////////
const Vector3f& Octahedron::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
size_t Octahedron::getFaceCount() const
{
    return 8;
}


////////////////////////////////////////////////////////////
Polyhedron::Face Octahedron::getFace(unsigned int index) const
{
    Vector3f right{ m_size.x, 0, 0 };
    Vector3f left = -right;
    Vector3f top{ 0, m_size.y, 0 };
    Vector3f bottom = -top;
    Vector3f front = { 0, 0, m_size.z };
    Vector3f back = -front;
    Vector3f offset = { 0, m_size.y, 0 };
    right += offset;
    left += offset;
    top += offset;
    bottom += offset;
    front += offset;
    back += offset;

    const Color& color = getColor();

    switch (index)
    {
    default:
    case 0:
        return{ top, front, right };
    case 1:
        return{ top, back, right };
    case 2:
        return { top, front, left };
    case 3:
        return { top, back, left };
    case 4:
        return{ bottom, front, right };
    case 5:
        return { bottom, back, right };
    case 6:
        return { bottom, front, left };
    case 7:
        return{ bottom, back, left };
    }
}

////////////////////////////////////////////////////////////
void Octahedron::setBorderColor(const Color& color)
{
    m_borderColor = color;

    for (int i = 0; i < m_borderVertices.getVertexCount(); i++)
    {
        m_borderVertices[i].color = m_borderColor;
    }
}

////////////////////////////////////////////////////////////
const Color& Octahedron::getBorderColor() const
{
    return m_borderColor;
}

void Octahedron::updateBorder()
{
    Vector3f right{ m_size.x, 0, 0 };
    Vector3f left = -right;
    Vector3f top{ 0, m_size.y, 0 };
    Vector3f bottom = -top;
    Vector3f front = { 0, 0, m_size.z };
    Vector3f back = -front;
    Vector3f offset = { 0, m_size.y, 0 };
    right += offset;
    left += offset;
    top += offset;
    bottom += offset;
    front += offset;
    back += offset;

    const Color& color = getBorderColor();
    int i = 0;
    m_borderVertices[i++] = Vertex(top, color);
    m_borderVertices[i++] = Vertex(left, color);

    m_borderVertices[i++] = Vertex(top, color);
    m_borderVertices[i++] = Vertex(front, color);

    m_borderVertices[i++] = Vertex(top, color);
    m_borderVertices[i++] = Vertex(right, color);

    m_borderVertices[i++] = Vertex(top, color);
    m_borderVertices[i++] = Vertex(back, color);

    m_borderVertices[i++] = Vertex(left, color);
    m_borderVertices[i++] = Vertex(front, color);

    m_borderVertices[i++] = Vertex(front, color);
    m_borderVertices[i++] = Vertex(right, color);

    m_borderVertices[i++] = Vertex(right, color);
    m_borderVertices[i++] = Vertex(back, color);

    m_borderVertices[i++] = Vertex(back, color);
    m_borderVertices[i++] = Vertex(left, color);

    m_borderVertices[i++] = Vertex(bottom, color);
    m_borderVertices[i++] = Vertex(left, color);

    m_borderVertices[i++] = Vertex(bottom, color);
    m_borderVertices[i++] = Vertex(front, color);

    m_borderVertices[i++] = Vertex(bottom, color);
    m_borderVertices[i++] = Vertex(right, color);

    m_borderVertices[i++] = Vertex(bottom, color);
    m_borderVertices[i++] = Vertex(back, color);

}

void Octahedron::draw(RenderTarget& target, RenderStates states) const 
{
    Polyhedron::draw(target, states);
    states.transform *= getTransform();
    target.draw(m_borderVertices, states);
}

}
