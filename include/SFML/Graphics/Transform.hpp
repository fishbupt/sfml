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

#ifndef SFML_TRANSFORM_HPP
#define SFML_TRANSFORM_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Box.hpp>
#include <SFML/System/Vector3.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Define a 4x4 transform matrix
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API Transform
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an identity transform (a transform that does nothing).
    ///
    ////////////////////////////////////////////////////////////
    Transform();

    ////////////////////////////////////////////////////////////
    /// \brief Construct a transform from a 3x3 matrix
    ///
    /// \param a00 Element (0, 0) of the matrix
    /// \param a01 Element (0, 1) of the matrix
    /// \param a02 Element (0, 2) of the matrix
    /// \param a03 Element (0, 3) of the matrix
    /// \param a10 Element (1, 0) of the matrix
    /// \param a11 Element (1, 1) of the matrix
    /// \param a12 Element (1, 2) of the matrix
    /// \param a13 Element (1, 3) of the matrix
    /// \param a20 Element (2, 0) of the matrix
    /// \param a21 Element (2, 1) of the matrix
    /// \param a22 Element (2, 2) of the matrix
    /// \param a23 Element (2, 3) of the matrix
    /// \param a30 Element (3, 0) of the matrix
    /// \param a31 Element (3, 1) of the matrix
    /// \param a32 Element (3, 2) of the matrix
    /// \param a33 Element (3, 3) of the matrix
    ///
    ////////////////////////////////////////////////////////////
    Transform(float a00, float a01, float a02, float a03,
              float a10, float a11, float a12, float a13,
              float a20, float a21, float a22, float a23,
              float a30, float a31, float a32, float a33);

    ////////////////////////////////////////////////////////////
    /// \brief Return the transform as a 4x4 matrix
    ///
    /// This function returns a pointer to an array of 16 floats
    /// containing the transform elements as a 4x4 matrix, which
    /// is directly compatible with OpenGL functions.
    ///
    /// \code
    /// sf::Transform transform = ...;
    /// glLoadMatrixf(transform.getMatrix());
    /// \endcode
    ///
    /// \return Pointer to a 4x4 matrix
    ///
    ////////////////////////////////////////////////////////////
    const float* getMatrix() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the inverse of the transform
    ///
    /// If the inverse cannot be computed, an identity transform
    /// is returned.
    ///
    /// \return A new transform which is the inverse of self
    ///
    ////////////////////////////////////////////////////////////
    Transform getInverse() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the transpose of the transform
    ///
    /// \return A new transform which is the transpose of self
    ///
    ////////////////////////////////////////////////////////////
    Transform getTranspose() const;

    ////////////////////////////////////////////////////////////
    /// \brief Transform a 3D point
    ///
    /// \param x X coordinate of the point to transform
    /// \param y Y coordinate of the point to transform
    /// \param z Z coordinate of the point to transform
    ///
    /// \return Transformed point
    ///
    ////////////////////////////////////////////////////////////
    Vector3f transformPoint(float x, float y, float z = 0) const;

    ////////////////////////////////////////////////////////////
    /// \brief Transform a 3D point
    ///
    /// \param point Point to transform
    ///
    /// \return Transformed point
    ///
    ////////////////////////////////////////////////////////////
    Vector3f transformPoint(const Vector3f& point) const;

    ////////////////////////////////////////////////////////////
    /// \brief Transform a rectangle
    ///
    /// Since SFML doesn't provide support for oriented rectangles,
    /// the result of this function is always an axis-aligned
    /// rectangle. Which means that if the transform contains a
    /// rotation, the bounding rectangle of the transformed rectangle
    /// is returned.
    ///
    /// \param rectangle Rectangle to transform
    ///
    /// \return Transformed rectangle
    ///
    ////////////////////////////////////////////////////////////
    FloatRect transformRect(const FloatRect& rectangle) const;

    ////////////////////////////////////////////////////////////
    /// \brief Transform a box
    ///
    /// Since SFML doesn't provide support for oriented boxes,
    /// the result of this function is always an axis-aligned
    /// box. Which means that if the transform contains a
    /// rotation, the bounding box of the transformed box
    /// is returned.
    ///
    /// \param box Box to transform
    ///
    /// \return Transformed box
    ///
    ////////////////////////////////////////////////////////////
    FloatBox transformBox(const FloatBox& box) const;

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with another one
    ///
    /// The result is a transform that is equivalent to applying
    /// *this followed by \a transform. Mathematically, it is
    /// equivalent to a matrix multiplication.
    ///
    /// \param transform Transform to combine with this transform
    ///
    /// \return Reference to *this
    ///
    ////////////////////////////////////////////////////////////
    Transform& combine(const Transform& transform);

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a translation
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.translate(100, 200, 300).rotate(45);
    /// \endcode
    ///
    /// \param x Offset to apply on X axis
    /// \param y Offset to apply on Y axis
    /// \param z Offset to apply on Z axis
    ///
    /// \return Reference to *this
    ///
    /// \see rotate, scale
    ///
    ////////////////////////////////////////////////////////////
    Transform& translate(float x, float y, float z = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a translation
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.translate(sf::Vector3f(100, 200, 300)).rotate(45);
    /// \endcode
    ///
    /// \param offset Translation offset to apply
    ///
    /// \return Reference to *this
    ///
    /// \see rotate, scale
    ///
    ////////////////////////////////////////////////////////////
    Transform& translate(const Vector3f& offset);

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a rotation
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.rotate(90).translate(50, 20);
    /// \endcode
    ///
    /// \param angle Rotation angle, in degrees
    ///
    /// \return Reference to *this
    ///
    /// \see translate, scale
    ///
    ////////////////////////////////////////////////////////////
    Transform& rotate(float angle);

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a rotation
    ///
    /// The center of rotation is provided for convenience as a second
    /// argument, so that you can build rotations around arbitrary points
    /// more easily (and efficiently) than the usual
    /// translate(-center).rotate(angle).translate(center).
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.rotate(90, 8, 3).translate(50, 20);
    /// \endcode
    ///
    /// \param angle Rotation angle, in degrees
    /// \param centerX X coordinate of the center of rotation
    /// \param centerY Y coordinate of the center of rotation
    ///
    /// \return Reference to *this
    ///
    /// \see translate, scale
    ///
    ////////////////////////////////////////////////////////////
    Transform& rotate(float angle, float centerX, float centerY);

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a rotation
    ///
    /// The center of rotation is provided for convenience as a second
    /// argument, so that you can build rotations around arbitrary points
    /// more easily (and efficiently) than the usual
    /// translate(-center).rotate(angle).translate(center).
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.rotate(90, sf::Vector2f(8, 3)).translate(sf::Vector2f(50, 20));
    /// \endcode
    ///
    /// \param angle Rotation angle, in degrees
    /// \param center Center of rotation
    ///
    /// \return Reference to *this
    ///
    /// \see translate, scale
    ///
    ////////////////////////////////////////////////////////////
    Transform& rotate(float angle, const Vector2f& center);

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a rotation
    ///
    /// The axis of rotation is provided for convenience as a second
    /// argument, so that you can build rotations around arbitrary axes.
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.rotate(90, sf::Vector3f(8, 3, 5)).translate(sf::Vector2f(50, 20));
    /// \endcode
    ///
    /// \param angle Rotation angle, in degrees
    /// \param axis  Axis of rotation
    ///
    /// \return Reference to *this
    ///
    /// \see translate, scale
    ///
    ////////////////////////////////////////////////////////////
    Transform& rotate(float angle, const Vector3f& axis);

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a scaling
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.scale(2, 1).rotate(45);
    /// \endcode
    ///
    /// \param scaleX Scaling factor on the X axis
    /// \param scaleY Scaling factor on the Y axis
    /// \param scaleZ Scaling factor on the Z axis
    ///
    /// \return Reference to *this
    ///
    /// \see translate, rotate
    ///
    ////////////////////////////////////////////////////////////
    Transform& scale(float scaleX, float scaleY, float scaleZ = 1);

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a scaling
    ///
    /// The center of scaling is provided for convenience as a second
    /// argument, so that you can build scaling around arbitrary points
    /// more easily (and efficiently) than the usual
    /// translate(-center).scale(factors).translate(center).
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.scale(2, 1, 8, 3).rotate(45);
    /// \endcode
    ///
    /// \param scaleX Scaling factor on X axis
    /// \param scaleY Scaling factor on Y axis
    /// \param centerX X coordinate of the center of scaling
    /// \param centerY Y coordinate of the center of scaling
    ///
    /// \return Reference to *this
    ///
    /// \see translate, rotate
    ///
    ////////////////////////////////////////////////////////////
    Transform& scale(float scaleX, float scaleY, float centerX, float centerY);

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a scaling
    ///
    /// The center of scaling is provided for convenience as a second
    /// argument, so that you can build scaling around arbitrary points
    /// more easily (and efficiently) than the usual
    /// translate(-center).scale(factors).translate(center).
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.scale(2, 1, 8, 3, 2, 6).rotate(45);
    /// \endcode
    ///
    /// \param scaleX Scaling factor on X axis
    /// \param scaleY Scaling factor on Y axis
    /// \param scaleZ Scaling factor on Z axis
    /// \param centerX X coordinate of the center of scaling
    /// \param centerY Y coordinate of the center of scaling
    /// \param centerZ Z coordinate of the center of scaling
    ///
    /// \return Reference to *this
    ///
    /// \see translate, rotate
    ///
    ////////////////////////////////////////////////////////////
    Transform& scale(float scaleX, float scaleY, float scaleZ, float centerX, float centerY, float centerZ);

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a scaling
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.scale(sf::Vector3f(2, 1, 3)).rotate(45);
    /// \endcode
    ///
    /// \param factors Scaling factors
    ///
    /// \return Reference to *this
    ///
    /// \see translate, rotate
    ///
    ////////////////////////////////////////////////////////////
    Transform& scale(const Vector3f& factors);

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a scaling
    ///
    /// The center of scaling is provided for convenience as a second
    /// argument, so that you can build scaling around arbitrary points
    /// more easily (and efficiently) than the usual
    /// translate(-center).scale(factors).translate(center).
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.scale(sf::Vector2f(2, 1), sf::Vector2f(8, 3)).rotate(45);
    /// \endcode
    ///
    /// \param factors Scaling factors
    /// \param center Center of scaling
    ///
    /// \return Reference to *this
    ///
    /// \see translate, rotate
    ///
    ////////////////////////////////////////////////////////////
    Transform& scale(const Vector3f& factors, const Vector3f& center);

    ////////////////////////////////////////////////////////////
    // Static member data
    ////////////////////////////////////////////////////////////
    static const Transform Identity; ///< The identity transform (does nothing)

private:

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    float m_matrix[16]; ///< 4x4 matrix defining the transformation
};

////////////////////////////////////////////////////////////
/// \relates sf::Transform
/// \brief Overload of binary operator * to combine two transforms
///
/// This call is equivalent to calling Transform(left).combine(right).
///
/// \param left Left operand (the first transform)
/// \param right Right operand (the second transform)
///
/// \return New combined transform
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API Transform operator *(const Transform& left, const Transform& right);

////////////////////////////////////////////////////////////
/// \relates sf::Transform
/// \brief Overload of binary operator *= to combine two transforms
///
/// This call is equivalent to calling left.combine(right).
///
/// \param left Left operand (the first transform)
/// \param right Right operand (the second transform)
///
/// \return The combined transform
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API Transform& operator *=(Transform& left, const Transform& right);

////////////////////////////////////////////////////////////
/// \relates sf::Transform
/// \brief Overload of binary operator * to transform a point
///
/// This call is equivalent to calling left.transformPoint(right).
///
/// \param left Left operand (the transform)
/// \param right Right operand (the point to transform)
///
/// \return New transformed point
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API Vector3f operator *(const Transform& left, const Vector3f& right);

} // namespace sf


#endif // SFML_TRANSFORM_HPP


////////////////////////////////////////////////////////////
/// \class sf::Transform
/// \ingroup graphics
///
/// A sf::Transform specifies how to translate, rotate, scale,
/// shear, project, whatever things. In mathematical terms, it defines
/// how to transform a coordinate system into another.
///
/// For example, if you apply a rotation transform to a sprite, the
/// result will be a rotated sprite. And anything that is transformed
/// by this rotation transform will be rotated the same way, according
/// to its initial position.
///
/// Transforms are typically used for drawing. But they can also be
/// used for any computation that requires to transform points between
/// the local and global coordinate systems of an entity (like collision
/// detection).
///
/// Example:
/// \code
/// // define a translation transform
/// sf::Transform translation;
/// translation.translate(20, 50);
///
/// // define a rotation transform
/// sf::Transform rotation;
/// rotation.rotate(45);
///
/// // combine them
/// sf::Transform transform = translation * rotation;
///
/// // use the result to transform stuff...
/// sf::Vector2f point = transform.transformPoint(10, 20);
/// sf::FloatRect rect = transform.transformRect(sf::FloatRect(0, 0, 10, 100));
/// \endcode
///
/// \see sf::Transformable, sf::RenderStates
///
////////////////////////////////////////////////////////////
