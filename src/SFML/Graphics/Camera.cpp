////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Camera.hpp>
#include <cmath>

#include <glm/vec3.hpp>                  // glm::vec3
#include <glm/vec4.hpp>                  // glm::vec4
#include <glm/mat4x4.hpp>                // glm::mat4
#include <glm/ext/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

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
        glm::mat4 projMat;
        if (m_orthoCamera)
        {
            projMat = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, m_nearPlane, m_farPlane);
        }
        else
        {
            projMat = glm::perspective(glm::radians(m_fieldOfView), 1.0f, m_nearPlane, m_farPlane);
        }
        m_transform = Transform(projMat);
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

        glm::vec3 pos = glm::vec3(m_center.x, m_center.y, m_center.z);
        glm::vec3 scale = glm::vec3(m_scale.x, m_scale.y, m_scale.z);
        glm::mat4 viewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(m_direction.x, m_direction.y, m_direction.z),
                                        glm::vec3(m_upVector.x, m_upVector.y, m_upVector.z));

        viewMat = glm::scale(viewMat, scale);
        viewMat = glm::translate(viewMat, -pos);
        m_viewTransform = Transform(viewMat);
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
