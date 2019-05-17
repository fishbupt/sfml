////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Cuboid.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
Cuboid::Cuboid(const Vector3f& size)
    :m_borderVertices(Lines, 24)
{
    setSize(size);
}


////////////////////////////////////////////////////////////
void Cuboid::setSize(const Vector3f& size)
{
    m_size = size;
    update();
    updateBorder();
    generateNormals();
}


////////////////////////////////////////////////////////////
const Vector3f& Cuboid::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
unsigned int Cuboid::getFaceCount() const
{
    return 12;
}


////////////////////////////////////////////////////////////
Polyhedron::Face Cuboid::getFace(unsigned int index) const
{
    float left   = m_size.x / -2.f;
    float top    = m_size.y /  2.f;
    float front  = m_size.z /  2.f;
    float right  = m_size.x /  2.f;
    float bottom = m_size.y / -2.f;
    float back   = m_size.z / -2.f;

    const Color& color = getColor();

    switch (index)
    {
        default:
        // Front 1
        case 0:  { Face face = {Vertex(Vector3f(left, top, front), color), Vertex(Vector3f(left, bottom, front), color), Vertex(Vector3f(right, bottom, front), color)}; return face; }
        // Front 2
        case 1:  { Face face = {Vertex(Vector3f(left, top, front), color), Vertex(Vector3f(right, bottom, front), color), Vertex(Vector3f(right, top, front), color)}; return face; }
        // Right 1
        case 2:  { Face face = {Vertex(Vector3f(right, top, front), color), Vertex(Vector3f(right, bottom, front), color), Vertex(Vector3f(right, bottom, back), color)}; return face; }
        // Right 2
        case 3:  { Face face = {Vertex(Vector3f(right, top, front), color), Vertex(Vector3f(right, bottom, back), color), Vertex(Vector3f(right, top, back), color)}; return face; }
        // Back 1
        case 4:  { Face face = {Vertex(Vector3f(right, top, back), color), Vertex(Vector3f(right, bottom, back), color), Vertex(Vector3f(left, bottom, back), color)}; return face; }
        // Back 2
        case 5:  { Face face = {Vertex(Vector3f(right, top, back), color), Vertex(Vector3f(left, bottom, back), color), Vertex(Vector3f(left, top, back), color)}; return face; }
        // Left 1
        case 6:  { Face face = {Vertex(Vector3f(left, top, back), color), Vertex(Vector3f(left, bottom, back), color), Vertex(Vector3f(left, bottom, front), color)}; return face; }
        // Left 2
        case 7:  { Face face = {Vertex(Vector3f(left, top, back), color), Vertex(Vector3f(left, bottom, front), color), Vertex(Vector3f(left, top, front), color)}; return face; }
        // Top 1
        case 8:  { Face face = {Vertex(Vector3f(left, top, back), color), Vertex(Vector3f(left, top, front), color), Vertex(Vector3f(right, top, front), color)}; return face; }
        // Top 2
        case 9:  { Face face = {Vertex(Vector3f(left, top, back), color), Vertex(Vector3f(right, top, front), color), Vertex(Vector3f(right, top, back), color)}; return face; }
        // Bottom 1
        case 10: { Face face = {Vertex(Vector3f(left, bottom, front), color), Vertex(Vector3f(left, bottom, back), color), Vertex(Vector3f(right, bottom, back), color)}; return face; }
        // Bottom 2
        case 11: { Face face = {Vertex(Vector3f(left, bottom, front), color), Vertex(Vector3f(right, bottom, back), color), Vertex(Vector3f(right, bottom, front), color)}; return face; }
    }
}

////////////////////////////////////////////////////////////
void Cuboid::setBorderColor(const Color& color)
{
    m_borderColor = color;

    for (int i = 0; i < m_borderVertices.getVertexCount(); i++)
    {
        m_borderVertices[i].color = m_borderColor;
    }
}

////////////////////////////////////////////////////////////
const Color& Cuboid::getBorderColor() const
{
    return m_borderColor;
}

void Cuboid::updateBorder()
{
    float left = m_size.x / -2.f;
    float top = m_size.y / 2.f;
    float front = m_size.z / 2.f;
    float right = m_size.x / 2.f;
    float bottom = m_size.y / -2.f;
    float back = m_size.z / -2.f;
    int i = 0;
    m_borderVertices[i++] = Vertex({ left, top, front });
    m_borderVertices[i++] = Vertex({ right, top, front });

    m_borderVertices[i++] = Vertex({ left, top, back });
    m_borderVertices[i++] = Vertex({ right, top, back });

    m_borderVertices[i++] = Vertex({ left, bottom, front });
    m_borderVertices[i++] = Vertex({ right, bottom, front });

    m_borderVertices[i++] = Vertex({ left, bottom, back });
    m_borderVertices[i++] = Vertex({ right, bottom, back });

    m_borderVertices[i++] = Vertex({ left, top, front });
    m_borderVertices[i++] = Vertex({ left, top, back });

    m_borderVertices[i++] = Vertex({ right, top, front });
    m_borderVertices[i++] = Vertex({ right, top, back });

    m_borderVertices[i++] = Vertex({ left, bottom, front });
    m_borderVertices[i++] = Vertex({ left, bottom, back });

    m_borderVertices[i++] = Vertex({ right, bottom, front });
    m_borderVertices[i++] = Vertex({ right, bottom, back });

    m_borderVertices[i++] = Vertex({ left, top, front });
    m_borderVertices[i++] = Vertex({ left, bottom, front });

    m_borderVertices[i++] = Vertex({ left, top, back });
    m_borderVertices[i++] = Vertex({ left, bottom, back });

    m_borderVertices[i++] = Vertex({ right, top, front });
    m_borderVertices[i++] = Vertex({ right, bottom, front });

    m_borderVertices[i++] = Vertex({ right, top, back });
    m_borderVertices[i++] = Vertex({ right, bottom, back });
}

void Cuboid::draw(RenderTarget& target, RenderStates states) const 
{
    Polyhedron::draw(target, states);
    states.transform *= getTransform();
    target.draw(m_borderVertices, states);
}

} // namespace sf
