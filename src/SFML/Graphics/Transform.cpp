////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2019 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/Transform.hpp>
#include <cmath>

namespace 
{
    float dot(const sf::Vector3f& a, const sf::Vector3f& b)
    {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }

    sf::Vector3f cross(const sf::Vector3f& a, const sf::Vector3f& b)
    {
        return sf::Vector3f(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
    }

    sf::Vector3f normalize(const sf::Vector3f& v)
    {
        float length = std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
        return sf::Vector3f(v.x / length, v.y / length, v.z / length);
    }
}

namespace sf
{
////////////////////////////////////////////////////////////
const Transform Transform::Identity;


////////////////////////////////////////////////////////////
Transform::Transform()
{
    // Identity matrix
    m_matrix[0] = 1.f; m_matrix[4] = 0.f; m_matrix[8]  = 0.f; m_matrix[12] = 0.f;
    m_matrix[1] = 0.f; m_matrix[5] = 1.f; m_matrix[9]  = 0.f; m_matrix[13] = 0.f;
    m_matrix[2] = 0.f; m_matrix[6] = 0.f; m_matrix[10] = 1.f; m_matrix[14] = 0.f;
    m_matrix[3] = 0.f; m_matrix[7] = 0.f; m_matrix[11] = 0.f; m_matrix[15] = 1.f;
}


////////////////////////////////////////////////////////////
Transform::Transform(float a00, float a01, float a02, float a03,
                     float a10, float a11, float a12, float a13,
                     float a20, float a21, float a22, float a23,
                     float a30, float a31, float a32, float a33)
{
    m_matrix[0] = a00; m_matrix[4] = a01; m_matrix[8]  = a02; m_matrix[12] = a03;
    m_matrix[1] = a10; m_matrix[5] = a11; m_matrix[9]  = a12; m_matrix[13] = a13;
    m_matrix[2] = a20; m_matrix[6] = a21; m_matrix[10] = a22; m_matrix[14] = a23;
    m_matrix[3] = a30; m_matrix[7] = a31; m_matrix[11] = a32; m_matrix[15] = a33;
}

////////////////////////////////////////////////////////////
const float* Transform::getMatrix() const
{
    return m_matrix;
}


////////////////////////////////////////////////////////////
Transform Transform::getInverse() const
{
    // Compute the inverse
    float inverted_matrix[16];

    inverted_matrix[0]  =  m_matrix[5]  * m_matrix[10] * m_matrix[15] -
                           m_matrix[5]  * m_matrix[11] * m_matrix[14] -
                           m_matrix[9]  * m_matrix[6]  * m_matrix[15] +
                           m_matrix[9]  * m_matrix[7]  * m_matrix[14] +
                           m_matrix[13] * m_matrix[6]  * m_matrix[11] -
                           m_matrix[13] * m_matrix[7]  * m_matrix[10];

    inverted_matrix[1]  = -m_matrix[1]  * m_matrix[10] * m_matrix[15] +
                           m_matrix[1]  * m_matrix[11] * m_matrix[14] +
                           m_matrix[9]  * m_matrix[2]  * m_matrix[15] -
                           m_matrix[9]  * m_matrix[3]  * m_matrix[14] -
                           m_matrix[13] * m_matrix[2]  * m_matrix[11] +
                           m_matrix[13] * m_matrix[3]  * m_matrix[10];

    inverted_matrix[2]  =  m_matrix[1]  * m_matrix[6]  * m_matrix[15] -
                           m_matrix[1]  * m_matrix[7]  * m_matrix[14] -
                           m_matrix[5]  * m_matrix[2]  * m_matrix[15] +
                           m_matrix[5]  * m_matrix[3]  * m_matrix[14] +
                           m_matrix[13] * m_matrix[2]  * m_matrix[7]  -
                           m_matrix[13] * m_matrix[3]  * m_matrix[6];

    inverted_matrix[3]  = -m_matrix[1]  * m_matrix[6]  * m_matrix[11] +
                           m_matrix[1]  * m_matrix[7]  * m_matrix[10] +
                           m_matrix[5]  * m_matrix[2]  * m_matrix[11] -
                           m_matrix[5]  * m_matrix[3]  * m_matrix[10] -
                           m_matrix[9]  * m_matrix[2]  * m_matrix[7]  +
                           m_matrix[9]  * m_matrix[3]  * m_matrix[6];

    inverted_matrix[4]  = -m_matrix[4]  * m_matrix[10] * m_matrix[15] +
                           m_matrix[4]  * m_matrix[11] * m_matrix[14] +
                           m_matrix[8]  * m_matrix[6]  * m_matrix[15] -
                           m_matrix[8]  * m_matrix[7]  * m_matrix[14] -
                           m_matrix[12] * m_matrix[6]  * m_matrix[11] +
                           m_matrix[12] * m_matrix[7]  * m_matrix[10];

    inverted_matrix[5]  =  m_matrix[0]  * m_matrix[10] * m_matrix[15] -
                           m_matrix[0]  * m_matrix[11] * m_matrix[14] -
                           m_matrix[8]  * m_matrix[2]  * m_matrix[15] +
                           m_matrix[8]  * m_matrix[3]  * m_matrix[14] +
                           m_matrix[12] * m_matrix[2]  * m_matrix[11] -
                           m_matrix[12] * m_matrix[3]  * m_matrix[10];

    inverted_matrix[6]  = -m_matrix[0]  * m_matrix[6]  * m_matrix[15] +
                           m_matrix[0]  * m_matrix[7]  * m_matrix[14] +
                           m_matrix[4]  * m_matrix[2]  * m_matrix[15] -
                           m_matrix[4]  * m_matrix[3]  * m_matrix[14] -
                           m_matrix[12] * m_matrix[2]  * m_matrix[7]  +
                           m_matrix[12] * m_matrix[3]  * m_matrix[6];

    inverted_matrix[7]  =  m_matrix[0]  * m_matrix[6]  * m_matrix[11] -
                           m_matrix[0]  * m_matrix[7]  * m_matrix[10] -
                           m_matrix[4]  * m_matrix[2]  * m_matrix[11] +
                           m_matrix[4]  * m_matrix[3]  * m_matrix[10] +
                           m_matrix[8]  * m_matrix[2]  * m_matrix[7]  -
                           m_matrix[8]  * m_matrix[3]  * m_matrix[6];

    inverted_matrix[8]  =  m_matrix[4]  * m_matrix[9]  * m_matrix[15] -
                           m_matrix[4]  * m_matrix[11] * m_matrix[13] -
                           m_matrix[8]  * m_matrix[5]  * m_matrix[15] +
                           m_matrix[8]  * m_matrix[7]  * m_matrix[13] +
                           m_matrix[12] * m_matrix[5]  * m_matrix[11] -
                           m_matrix[12] * m_matrix[7]  * m_matrix[9];

    inverted_matrix[9]  = -m_matrix[0]  * m_matrix[9]  * m_matrix[15] +
                           m_matrix[0]  * m_matrix[11] * m_matrix[13] +
                           m_matrix[8]  * m_matrix[1]  * m_matrix[15] -
                           m_matrix[8]  * m_matrix[3]  * m_matrix[13] -
                           m_matrix[12] * m_matrix[1]  * m_matrix[11] +
                           m_matrix[12] * m_matrix[3]  * m_matrix[9];

    inverted_matrix[10] =  m_matrix[0]  * m_matrix[5]  * m_matrix[15] -
                           m_matrix[0]  * m_matrix[7]  * m_matrix[13] -
                           m_matrix[4]  * m_matrix[1]  * m_matrix[15] +
                           m_matrix[4]  * m_matrix[3]  * m_matrix[13] +
                           m_matrix[12] * m_matrix[1]  * m_matrix[7]  -
                           m_matrix[12] * m_matrix[3]  * m_matrix[5];

    inverted_matrix[11] = -m_matrix[0]  * m_matrix[5]  * m_matrix[11] +
                           m_matrix[0]  * m_matrix[7]  * m_matrix[9]  +
                           m_matrix[4]  * m_matrix[1]  * m_matrix[11] -
                           m_matrix[4]  * m_matrix[3]  * m_matrix[9]  -
                           m_matrix[8]  * m_matrix[1]  * m_matrix[7]  +
                           m_matrix[8]  * m_matrix[3]  * m_matrix[5];

    inverted_matrix[12] = -m_matrix[4]  * m_matrix[9]  * m_matrix[14] +
                           m_matrix[4]  * m_matrix[10] * m_matrix[13] +
                           m_matrix[8]  * m_matrix[5]  * m_matrix[14] -
                           m_matrix[8]  * m_matrix[6]  * m_matrix[13] -
                           m_matrix[12] * m_matrix[5]  * m_matrix[10] +
                           m_matrix[12] * m_matrix[6]  * m_matrix[9];

    inverted_matrix[13] =  m_matrix[0]  * m_matrix[9]  * m_matrix[14] -
                           m_matrix[0]  * m_matrix[10] * m_matrix[13] -
                           m_matrix[8]  * m_matrix[1]  * m_matrix[14] +
                           m_matrix[8]  * m_matrix[2]  * m_matrix[13] +
                           m_matrix[12] * m_matrix[1]  * m_matrix[10] -
                           m_matrix[12] * m_matrix[2]  * m_matrix[9];

    inverted_matrix[14] = -m_matrix[0]  * m_matrix[5]  * m_matrix[14] +
                           m_matrix[0]  * m_matrix[6]  * m_matrix[13] +
                           m_matrix[4]  * m_matrix[1]  * m_matrix[14] -
                           m_matrix[4]  * m_matrix[2]  * m_matrix[13] -
                           m_matrix[12] * m_matrix[1]  * m_matrix[6]  +
                           m_matrix[12] * m_matrix[2]  * m_matrix[5];

    inverted_matrix[15] =  m_matrix[0]  * m_matrix[5]  * m_matrix[10] -
                           m_matrix[0]  * m_matrix[6]  * m_matrix[9]  -
                           m_matrix[4]  * m_matrix[1]  * m_matrix[10] +
                           m_matrix[4]  * m_matrix[2]  * m_matrix[9]  +
                           m_matrix[8]  * m_matrix[1]  * m_matrix[6]  -
                           m_matrix[8]  * m_matrix[2]  * m_matrix[5];

    // Compute the determinant
    float det = m_matrix[0] * inverted_matrix[0] +
                m_matrix[1] * inverted_matrix[4] +
                m_matrix[2] * inverted_matrix[8] +
                m_matrix[3] * inverted_matrix[12];

    // Compute the inverse if the determinant is not zero
    // (don't use an epsilon because the determinant may *really* be tiny)
    if (det != 0.f)
    {
        det = 1.f / det;

        for (int i = 0; i < 16; ++i)
            inverted_matrix[i] = inverted_matrix[i] * det;

        return Transform(inverted_matrix[0], inverted_matrix[4], inverted_matrix[8],  inverted_matrix[12],
                         inverted_matrix[1], inverted_matrix[5], inverted_matrix[8],  inverted_matrix[13],
                         inverted_matrix[2], inverted_matrix[6], inverted_matrix[10], inverted_matrix[14],
                         inverted_matrix[3], inverted_matrix[7], inverted_matrix[11], inverted_matrix[15]);
    }
    else
    {
        return Identity;
    }
}


////////////////////////////////////////////////////////////
Transform Transform::getTranspose() const
{
    return Transform(m_matrix[0],  m_matrix[1],  m_matrix[2],  m_matrix[3],
                     m_matrix[4],  m_matrix[5],  m_matrix[6],  m_matrix[7],
                     m_matrix[8],  m_matrix[9],  m_matrix[10], m_matrix[11],
                     m_matrix[12], m_matrix[13], m_matrix[14], m_matrix[15]);
}


////////////////////////////////////////////////////////////
Vector3f Transform::transformPoint(float x, float y, float z) const
{
    return Vector3f(m_matrix[0] * x + m_matrix[4] * y + m_matrix[8]  * z + m_matrix[12],
                    m_matrix[1] * x + m_matrix[5] * y + m_matrix[9]  * z + m_matrix[13],
                    m_matrix[2] * x + m_matrix[6] * y + m_matrix[10] * z + m_matrix[14]);
}


////////////////////////////////////////////////////////////
Vector3f Transform::transformPoint(const Vector3f& point) const
{
    return transformPoint(point.x, point.y, point.z);
}


////////////////////////////////////////////////////////////
FloatRect Transform::transformRect(const FloatRect& rectangle) const
{
    // Transform the 4 corners of the rectangle
    const Vector3f points[] =
    {
        transformPoint(rectangle.left, rectangle.top),
        transformPoint(rectangle.left, rectangle.top + rectangle.height),
        transformPoint(rectangle.left + rectangle.width, rectangle.top),
        transformPoint(rectangle.left + rectangle.width, rectangle.top + rectangle.height)
    };

    // Compute the bounding rectangle of the transformed points
    float left = points[0].x;
    float top = points[0].y;
    float right = points[0].x;
    float bottom = points[0].y;
    for (int i = 1; i < 4; ++i)
    {
        if      (points[i].x < left)   left = points[i].x;
        else if (points[i].x > right)  right = points[i].x;
        if      (points[i].y < top)    top = points[i].y;
        else if (points[i].y > bottom) bottom = points[i].y;
    }

    return FloatRect(left, top, right - left, bottom - top);
}


////////////////////////////////////////////////////////////
FloatBox Transform::transformBox(const FloatBox& box) const
{
    // Transform the 8 corners of the box
    const Vector3f points[] =
    {
        transformPoint(box.left, box.top, box.front),
        transformPoint(box.left, box.top + box.height, box.front),
        transformPoint(box.left + box.width, box.top, box.front),
        transformPoint(box.left + box.width, box.top + box.height, box.front),
        transformPoint(box.left, box.top, box.front + box.depth),
        transformPoint(box.left, box.top + box.height, box.front + box.depth),
        transformPoint(box.left + box.width, box.top, box.front + box.depth),
        transformPoint(box.left + box.width, box.top + box.height, box.front + box.depth)
    };

    // Compute the bounding box of the transformed points
    float left   = points[0].x;
    float top    = points[0].y;
    float front  = points[0].z;
    float right  = points[0].x;
    float bottom = points[0].y;
    float back   = points[0].z;
    for (int i = 1; i < 4; ++i)
    {
        if      (points[i].x < left)   left   = points[i].x;
        else if (points[i].x > right)  right  = points[i].x;
        if      (points[i].y < top)    top    = points[i].y;
        else if (points[i].y > bottom) bottom = points[i].y;
        if      (points[i].z < front)  front  = points[i].z;
        else if (points[i].z > back)   back   = points[i].z;
    }
    return FloatBox(left, top, front, right - left, bottom - top, back - front);
}

////////////////////////////////////////////////////////////
FloatRect Transform::transformBoxToRect(const FloatBox& box) const
{
    // Transform the 8 corners of the box
    const Vector3f points[] =
    {
        transformPoint(box.left, box.top, box.front),
        transformPoint(box.left, box.top + box.height, box.front),
        transformPoint(box.left + box.width, box.top, box.front),
        transformPoint(box.left + box.width, box.top + box.height, box.front),
        transformPoint(box.left, box.top, box.front + box.depth),
        transformPoint(box.left, box.top + box.height, box.front + box.depth),
        transformPoint(box.left + box.width, box.top, box.front + box.depth),
        transformPoint(box.left + box.width, box.top + box.height, box.front + box.depth)
    };

    float xMin, xMax, yMin, yMax;
    xMin = xMax = points[0].x;
    yMin = yMax = points[0].y;
    for (int i = 1; i < 8; i++)
    {
        xMin = std::min(points[i].x, xMin);
        xMax = std::max(points[i].x, xMax);
        yMin = std::min(points[i].y, yMin);
        yMax = std::max(points[i].y, yMax);

    }
    return FloatRect(xMin, yMin, xMax - xMin, yMax - yMin);
}

////////////////////////////////////////////////////////////
Transform& Transform::combine(const Transform& transform)
{
    const float* a = m_matrix;
    const float* b = transform.m_matrix;

    *this = Transform(a[0] * b[0]  + a[4] * b[1]  + a[8]  * b[2]  + a[12] * b[3],
                      a[0] * b[4]  + a[4] * b[5]  + a[8]  * b[6]  + a[12] * b[7],
                      a[0] * b[8]  + a[4] * b[9]  + a[8]  * b[10] + a[12] * b[11],
                      a[0] * b[12] + a[4] * b[13] + a[8]  * b[14] + a[12] * b[15],
                      a[1] * b[0]  + a[5] * b[1]  + a[9]  * b[2]  + a[13] * b[3],
                      a[1] * b[4]  + a[5] * b[5]  + a[9]  * b[6]  + a[13] * b[7],
                      a[1] * b[8]  + a[5] * b[9]  + a[9]  * b[10] + a[13] * b[11],
                      a[1] * b[12] + a[5] * b[13] + a[9]  * b[14] + a[13] * b[15],
                      a[2] * b[0]  + a[6] * b[1]  + a[10] * b[2]  + a[14] * b[3],
                      a[2] * b[4]  + a[6] * b[5]  + a[10] * b[6]  + a[14] * b[7],
                      a[2] * b[8]  + a[6] * b[9]  + a[10] * b[10] + a[14] * b[11],
                      a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15],
                      a[3] * b[0]  + a[7] * b[1]  + a[11] * b[2]  + a[15] * b[3],
                      a[3] * b[4]  + a[7] * b[5]  + a[11] * b[6]  + a[15] * b[7],
                      a[3] * b[8]  + a[7] * b[9]  + a[11] * b[10] + a[15] * b[11],
                      a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15]);

    return *this;
}


////////////////////////////////////////////////////////////
Transform& Transform::translate(float x, float y, float z)
{
    Transform translation(1, 0, 0, x,
                          0, 1, 0, y,
                          0, 0, 1, z,
                          0, 0, 0, 1);

    return combine(translation);
}


////////////////////////////////////////////////////////////
Transform& Transform::translate(const Vector3f& offset)
{
    return translate(offset.x, offset.y, offset.z);
}


////////////////////////////////////////////////////////////
Transform& Transform::rotate(float angle)
{
    float rad = angle * 3.141592654f / 180.f;
    float cos = std::cos(rad);
    float sin = std::sin(rad);

    Transform rotation(cos, -sin, 0, 0,
                       sin,  cos, 0, 0,
                       0,    0,   1, 0,
                       0,    0,   0, 1);

    return combine(rotation);
}


////////////////////////////////////////////////////////////
Transform& Transform::rotate(float angle, float centerX, float centerY)
{
    float rad = angle * 3.141592654f / 180.f;
    float cos = std::cos(rad);
    float sin = std::sin(rad);

    Transform rotation(cos, -sin, 0, centerX * (1 - cos) + centerY * sin,
                       sin,  cos, 0, centerY * (1 - cos) - centerX * sin,
                       0,    0,   1, 0,
                       0,    0,   0, 1);

    return combine(rotation);
}


////////////////////////////////////////////////////////////
Transform& Transform::rotate(float angle, const Vector2f& center)
{
    return rotate(angle, center.x, center.y);
}


////////////////////////////////////////////////////////////
Transform& Transform::rotate(float angle, const Vector3f& axis)
{
    // Normalize axis
    float norm = std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);

    if(norm == 0.f)
        return *this;

    float x = axis.x / norm;
    float y = axis.y / norm;
    float z = axis.z / norm;

    float rad = angle * 3.141592654f / 180.f;
    float cos = std::cos(rad);
    float sin = std::sin(rad);

    Transform rotation(x * x * (1 - cos) + cos,     x * y * (1 - cos) - z * sin, x * z * (1 - cos) + y * sin, 0,
                       y * x * (1 - cos) + z * sin, y * y * (1 - cos) + cos,     y * z * (1 - cos) - x * sin, 0,
                       x * z * (1 - cos) - y * sin, y * z * (1 - cos) + x * sin, z * z * (1 - cos) + cos,     0,
                       0,                           0,                           0,                           1);

    return combine(rotation);
}


////////////////////////////////////////////////////////////
Transform& Transform::scale(float scaleX, float scaleY, float scaleZ)
{
    Transform scaling(scaleX, 0,      0,      0,
                      0,      scaleY, 0,      0,
                      0,      0,      scaleZ, 0,
                      0,      0,      0,      1);

    return combine(scaling);
}


////////////////////////////////////////////////////////////
Transform& Transform::scale(float scaleX, float scaleY, float centerX, float centerY)
{
    Transform scaling(scaleX, 0,      0, centerX * (1 - scaleX),
                      0,      scaleY, 0, centerY * (1 - scaleY),
                      0,      0,      1, 0,
                      0,      0,      0, 1);

    return combine(scaling);
}


////////////////////////////////////////////////////////////
Transform& Transform::scale(float scaleX, float scaleY, float scaleZ, float centerX, float centerY, float centerZ)
{
    Transform scaling(scaleX, 0,      0,      centerX * (1 - scaleX),
                      0,      scaleY, 0,      centerY * (1 - scaleY),
                      0,      0,      scaleZ, centerZ * (1 - scaleZ),
                      0,      0,      0,      1);

    return combine(scaling);
}


////////////////////////////////////////////////////////////
Transform& Transform::scale(const Vector3f& factors)
{
    return scale(factors.x, factors.y, factors.z);
}


////////////////////////////////////////////////////////////
Transform& Transform::scale(const Vector3f& factors, const Vector3f& center)
{
    return scale(factors.x, factors.y, factors.z, center.x, center.y, center.z);
}

////////////////////////////////////////////////////////////
Transform Transform::ortho(float left, float right, float bottom, float top, float near, float far)
{
    Transform tran;
    float* mat = const_cast<float*>(tran.getMatrix());
    mat[0] = 2.0f / (right - left);
    mat[5] = 2.0f / (top - bottom);
    mat[10] = -2.0f / (far - near);
    mat[12] = -(right + left) / (right - left);
    mat[13] = -(top + bottom) / (top - bottom);
    mat[14] = -(far + near) / (far - near);

    return tran;
}

////////////////////////////////////////////////////////////
Transform Transform::perspective(float fov, float aspect, float near, float far)
{
    Transform tran;
    float* mat = const_cast<float*>(tran.getMatrix());
    float tanHalfFov = std::tan(fov / 2.0f);
    mat[0] = 1.0f / (aspect*tanHalfFov);
    mat[5] = 1.0f / (tanHalfFov);
    mat[10] = -(far + near) / (far - near);
    mat[15] = 0.0f;
    mat[11] = -1.0f;
    mat[14] = -(2.0f*far*near) / (far - near);

    return tran;
}

////////////////////////////////////////////////////////////
Transform Transform::lookAt(Vector3f position, Vector3f target, Vector3f up)
{
    const sf::Vector3f f = normalize(target - position);
    const sf::Vector3f s = normalize(cross(f, up));
    const sf::Vector3f u = cross(s, f);

    Transform tran;
    float* mat = const_cast<float*>(tran.getMatrix());
    mat[0] = s.x;
    mat[4] = s.y;
    mat[8] = s.z;
    mat[1] = u.x;
    mat[5] = u.y;
    mat[9] = u.z;
    mat[2] = -f.x;
    mat[6] = -f.y;
    mat[10] = -f.z;
    mat[12] = -dot(s, position);
    mat[13] = -dot(u, position);
    mat[14] = dot(f, position);
    return tran;
}


////////////////////////////////////////////////////////////
Transform operator *(const Transform& left, const Transform& right)
{
    return Transform(left).combine(right);
}


////////////////////////////////////////////////////////////
Transform& operator *=(Transform& left, const Transform& right)
{
    return left.combine(right);
}


////////////////////////////////////////////////////////////
Vector3f operator *(const Transform& left, const Vector3f& right)
{
    return left.transformPoint(right);
}


////////////////////////////////////////////////////////////
bool operator ==(const Transform& left, const Transform& right)
{
    const float* a = left.getMatrix();
    const float* b = right.getMatrix();

    return ((a[0]  == b[0])  && (a[1]  == b[1])  && (a[3]  == b[3]) &&
            (a[4]  == b[4])  && (a[5]  == b[5])  && (a[7]  == b[7]) &&
            (a[12] == b[12]) && (a[13] == b[13]) && (a[15] == b[15]));
}


////////////////////////////////////////////////////////////
bool operator !=(const Transform& left, const Transform& right)
{
    return !(left == right);
}

} // namespace sf
