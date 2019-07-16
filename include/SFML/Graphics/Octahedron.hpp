#ifndef SFML_OCTAHEDRON_HPP
#define SFML_OCTAHEDRON_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Polyhedron.hpp>

namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Specialized polyhedron representing a octahedron
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API Octahedron : public Polyhedron
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// \param size Size of the octahedron
    ///
    ////////////////////////////////////////////////////////////
    explicit Octahedron(const Vector3f& size = Vector3f(0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// \brief Set the size of the octahedron
    ///
    /// \param size New size of the octahedron
    ///
    /// \see getSize
    ///
    ////////////////////////////////////////////////////////////
    void setSize(const Vector3f& size);

    ////////////////////////////////////////////////////////////
    /// \brief Get the size of the octahedron
    ///
    /// \return Size of the octahedron
    ///
    /// \see setSize
    ///
    ////////////////////////////////////////////////////////////
    const Vector3f& getSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of faces defining the octahedron
    ///
    /// \return Number of faces defining the octahedron
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int getFaceCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get a face of the octahedron
    ///
    /// The result is undefined if \a index is out of the valid range.
    ///
    /// \param index Index of the face to get, in range [0 .. getFaceCount() - 1]
    ///
    /// \return Index-th face of the octahedron
    ///
    ////////////////////////////////////////////////////////////
    virtual Face getFace(unsigned int index) const;

    void setBorderColor(const Color& color);

    const Color& getBorderColor() const;

    void draw(RenderTarget& target, RenderStates states) const override;

    void updateBorder();
private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Vector3f m_size; ///< Size of the octahedron
    Color m_borderColor;
    VertexArray m_borderVertices;
};

}

#endif // ! SFML_OCTAHEDRON_HPP
