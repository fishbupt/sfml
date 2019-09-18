////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Camera.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

namespace sf
{
////////////////////////////////////////////////////////////
Camera::Camera(float fov, float near, float far)
    : View()
    , m_fieldOfView(fov)
    , m_nearPlane(near)
    , m_farPlane(far)
    , m_orthoCamera(true)
    , m_direction(0, 0, -1)
    , m_upVector(0, 1, 0)
    , m_scale(1, 1, 1)
{
}

////////////////////////////////////////////////////////////
Camera::~Camera()
{
}

////////////////////////////////////////////////////////////
void Camera::setFieldOfView(float fov)
{
    m_fieldOfView = fov;
    m_transformUpdated = false;
    m_invTransformUpdated = false;
}

////////////////////////////////////////////////////////////
float Camera::getFieldOfView() const
{
    return m_fieldOfView;
}

////////////////////////////////////////////////////////////
void Camera::setNearClippingPlane(float distance)
{
    m_nearPlane = distance;
    m_transformUpdated = false;
    m_invTransformUpdated = false;
}

////////////////////////////////////////////////////////////
float Camera::getNearClippingPlane() const
{
    return m_nearPlane;
}

////////////////////////////////////////////////////////////
void Camera::setFarClippingPlane(float distance)
{
    m_farPlane = distance;
    m_transformUpdated = false;
    m_invTransformUpdated = false;
}

////////////////////////////////////////////////////////////
float Camera::getFarClippingPlane() const
{
    return m_farPlane;
}

////////////////////////////////////////////////////////////
void Camera::setDirection(float x, float y, float z)
{
    setDirection(Vector3f(x, y, z));
}

////////////////////////////////////////////////////////////
void Camera::setDirection(const Vector3f& direction)
{
    m_direction = direction;
    m_viewTransformUpdated = false;
    m_invViewTransformUpdated = false;
}

////////////////////////////////////////////////////////////
const Vector3f& Camera::getDirection() const
{
    return m_direction;
}

////////////////////////////////////////////////////////////
void Camera::setUpVector(float x, float y, float z)
{
    setUpVector(Vector3f(x, y, z));
}

////////////////////////////////////////////////////////////
void Camera::setUpVector(const Vector3f& upVector)
{
    m_upVector = upVector;
    m_viewTransformUpdated = false;
    m_invViewTransformUpdated = false;
}

////////////////////////////////////////////////////////////
const Vector3f& Camera::getUpVector() const
{
    return m_upVector;
}

////////////////////////////////////////////////////////////
void Camera::setScale(float factorX, float factorY, float factorZ)
{
    setScale(Vector3f(factorX, factorY, factorZ));
}

////////////////////////////////////////////////////////////
void Camera::setScale(const Vector3f& factors)
{
    m_scale = factors;
    m_viewTransformUpdated = false;
    m_invViewTransformUpdated = false;
}

////////////////////////////////////////////////////////////
const Vector3f& Camera::getScale() const
{
    return m_scale;
}

////////////////////////////////////////////////////////////
void Camera::scale(float factorX, float factorY, float factorZ)
{
    m_scale.x *= factorX;
    m_scale.y *= factorY;
    m_scale.z *= factorZ;
    m_viewTransformUpdated = false;
    m_invViewTransformUpdated = false;
}

////////////////////////////////////////////////////////////
void Camera::scale(const Vector3f& factor)
{
    scale(factor.x, factor.y, factor.z);
}

////////////////////////////////////////////////////////////
void Camera::setPosition(const sf::Vector3f& position)
{
    setCenter(position);
    m_viewTransformUpdated = false;
    m_invViewTransformUpdated = false;
}

////////////////////////////////////////////////////////////
void Camera::useOrthographicProjection()
{
    m_orthoCamera = true;
    m_transformUpdated = false;
}

////////////////////////////////////////////////////////////
void Camera::usePerspectiveProjection()
{
    m_orthoCamera = false;
    m_transformUpdated = false;
}

////////////////////////////////////////////////////////////
const Transform& Camera::getTransform() const
{
    // Recompute the perspective projection matrix if needed
    if (!m_transformUpdated)
    {
        if (m_orthoCamera)
        {
            m_transform = Transform::ortho(-2.0f, 2.0f, -2.0f, 2.0f, m_nearPlane, m_farPlane);
        }
        else
        {
            m_transform = Transform::perspective(m_fieldOfView * (float)M_PI / 180.0f, 1.0f, m_nearPlane, m_farPlane);
        }
        m_transformUpdated = true;
    }

    return m_transform;
}

////////////////////////////////////////////////////////////
const Transform& Camera::getViewTransform() const
{
    // Recompute the view matrix if needed
    if (!m_viewTransformUpdated)
    {
        m_viewTransform = Transform::lookAt({ 0,0,0 }, m_direction, m_upVector);
        m_viewTransform.scale(m_scale);
        m_viewTransform.translate(-m_center);
        m_viewTransformUpdated = true;
    }

    return m_viewTransform;
}

////////////////////////////////////////////////////////////
bool Camera::isOrthographicProjection() const
{
    return m_orthoCamera;
}

} // namespace sf
