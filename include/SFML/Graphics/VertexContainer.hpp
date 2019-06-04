#ifndef SFML_VERTEXCONTAINER_HPP
#define SFML_VERTEXCONTAINER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Box.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <vector>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Define a set of one or more primitives
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API VertexContainer : public Drawable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty vertex container.
    ///
    ////////////////////////////////////////////////////////////
    VertexContainer();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the vertex container with a type and an initial number of vertices
    ///
    /// \param type        Type of primitives
    /// \param vertexCount Initial number of vertices in the container
    ///
    ////////////////////////////////////////////////////////////
    explicit VertexContainer(PrimitiveType type, unsigned int vertexCount = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    /// \param copy instance to copy
    ///
    ////////////////////////////////////////////////////////////
    VertexContainer(const VertexContainer& copy);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~VertexContainer();

    ////////////////////////////////////////////////////////////
    /// \brief Return the vertex count
    ///
    /// \return Number of vertices in the container
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int getVertexCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get a read-write access to a vertex by its index
    ///
    /// This function doesn't check \a index, it must be in range
    /// [0, getVertexCount() - 1]. The behaviour is undefined
    /// otherwise.
    ///
    /// \param index Index of the vertex to get
    ///
    /// \return Reference to the index-th vertex
    ///
    /// \see getVertexCount
    ///
    ////////////////////////////////////////////////////////////
    virtual Vertex& operator [](unsigned int index);

    ////////////////////////////////////////////////////////////
    /// \brief Get a read-only access to a vertex by its index
    ///
    /// This function doesn't check \a index, it must be in range
    /// [0, getVertexCount() - 1]. The behaviour is undefined
    /// otherwise.
    ///
    /// \param index Index of the vertex to get
    ///
    /// \return Const reference to the index-th vertex
    ///
    /// \see getVertexCount
    ///
    ////////////////////////////////////////////////////////////
    virtual const Vertex& operator [](unsigned int index) const;

    ////////////////////////////////////////////////////////////
    /// \brief Clear the vertex container
    ///
    /// This function removes all the vertices from the container.
    /// It doesn't deallocate the corresponding memory, so that
    /// adding new vertices after clearing doesn't involve
    /// reallocating all the memory.
    ///
    ////////////////////////////////////////////////////////////
    virtual void clear();

    ////////////////////////////////////////////////////////////
    /// \brief Resize the vertex container
    ///
    /// If \a vertexCount is greater than the current size, the previous
    /// vertices are kept and new (default-constructed) vertices are
    /// added.
    /// If \a vertexCount is less than the current size, existing vertices
    /// are removed from the container.
    ///
    /// \param vertexCount New size of the container (number of vertices)
    ///
    ////////////////////////////////////////////////////////////
    virtual void resize(unsigned int vertexCount);

    ////////////////////////////////////////////////////////////
    /// \brief Add a vertex to the container
    ///
    /// \param vertex Vertex to add
    ///
    ////////////////////////////////////////////////////////////
    virtual void append(const Vertex& vertex);

    ////////////////////////////////////////////////////////////
    /// \brief Set the type of primitives to draw
    ///
    /// This function defines how the vertices must be interpreted
    /// when it's time to draw them:
    /// \li As points
    /// \li As lines
    /// \li As triangles
    /// \li As quads
    /// The default primitive type is sf::Points.
    ///
    /// \param type Type of primitive
    ///
    ////////////////////////////////////////////////////////////
    virtual void setPrimitiveType(PrimitiveType type);

    ////////////////////////////////////////////////////////////
    /// \brief Get the type of primitives drawn by the vertex container
    ///
    /// \return Primitive type
    ///
    ////////////////////////////////////////////////////////////
    virtual PrimitiveType getPrimitiveType() const;

    ////////////////////////////////////////////////////////////
    /// \brief Compute the bounding box of the vertex container
    ///
    /// This function returns the axis-aligned box that
    /// contains all the vertices of the container.
    ///
    /// \return Bounding box of the vertex container
    ///
    ////////////////////////////////////////////////////////////
    virtual FloatBox getBounds() const;

    ////////////////////////////////////////////////////////////
    /// \brief Overload of assignment operator
    ///
    /// \param right Instance to assign
    ///
    /// \return Reference to self
    ///
    ////////////////////////////////////////////////////////////
    VertexContainer& operator =(const VertexContainer& right);

protected :

    ////////////////////////////////////////////////////////////
    /// \brief Special constructor for derived classes
    ///
    /// This function is meant for internal use only.
    ///
    ////////////////////////////////////////////////////////////
    VertexContainer(int);

private :

    ////////////////////////////////////////////////////////////
    /// \brief Draw the vertex container to a render target
    ///
    /// \param target Render target to draw to
    /// \param states Current render states
    ///
    ////////////////////////////////////////////////////////////
    virtual void draw(RenderTarget& target, RenderStates states) const;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    VertexContainer* m_impl; ///< Platform/hardware specific implementation
};

} // namespace sf


#endif // SFML_VERTEXCONTAINER_HPP


////////////////////////////////////////////////////////////
/// \class sf::VertexContainer
/// \ingroup graphics
///
/// sf::VertexContainer is a very simple wrapper around a dynamic
/// container of vertices and a primitives type.
///
/// It inherits sf::Drawable, but unlike other drawables it
/// is not transformable.
///
/// sf::VertexContainer is a simple proxy class that automatically
/// selects the best method of vertex storage. It will make use
/// of an sf::VertexBuffer if it is available and fall back
/// to sf::VertexArray if not.
///
/// Be aware of the order when specifying vertices. By default,
/// outward facing faces have counter-clockwise winding and as
/// such any faces specified in clockwise order might not be
/// displayed.
///
/// Example:
/// \code
/// sf::VertexContainer lines(sf::LinesStrip, 4);
/// lines[0].position = sf::Vector3f(10, 0, 0);
/// lines[1].position = sf::Vector3f(20, 0, 0);
/// lines[2].position = sf::Vector3f(30, 5, 0);
/// lines[3].position = sf::Vector3f(40, 2, 0);
///
/// window.draw(lines);
/// \endcode
///
/// \see sf::Vertex, sf::VertexArray, sf::VertexBuffer
///
////////////////////////////////////////////////////////////
