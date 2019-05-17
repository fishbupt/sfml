////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/OrbitCamera.hpp>
#include <cmath>
#include <glm/gtx/quaternion.hpp>

namespace sf
{
constexpr glm::vec3 kXAxis(1, 0, 0);
constexpr glm::vec3 kZAxis(0, 0, 1);
constexpr glm::vec3 kYAxis(0, 1, 0);
constexpr float kDefaultDistance = 5.0f;

////////////////////////////////////////////////////////////
OrbitCamera::OrbitCamera(float fov, float near, float far)
    : Camera{ fov, near, far }
    , m_azimuth(0.0f)
    , m_elevation(0.0f)
    , m_distance{ kDefaultDistance }
{
    computeRotation();
}

////////////////////////////////////////////////////////////
void OrbitCamera::setDistance(float distance)
{
    m_distance = distance;
    m_viewTransformUpdated = false;
}

////////////////////////////////////////////////////////////
float OrbitCamera::getDistance() const
{
    return m_distance;
}

////////////////////////////////////////////////////////////
void OrbitCamera::setAzimuth(float azimuth)
{
    m_azimuth = azimuth;
    computeRotation();
}

////////////////////////////////////////////////////////////
void OrbitCamera::changeAzimuth(float angle)
{
    m_azimuth += angle;
    computeRotation();
}

////////////////////////////////////////////////////////////
float OrbitCamera::getAzimuth() const
{
    return m_azimuth;
}

////////////////////////////////////////////////////////////
void OrbitCamera::setElevation(float elevation)
{
    m_elevation = elevation;
    m_elevation = std::max(-90.0f, m_elevation);
    m_elevation = std::min(90.0f, m_elevation);
    computeRotation();
}

////////////////////////////////////////////////////////////
void OrbitCamera::changeElevation(float angle)
{
    m_elevation += angle;
    m_elevation = std::max(-90.0f, m_elevation);
    m_elevation = std::min(90.0f, m_elevation);
    computeRotation();
}

////////////////////////////////////////////////////////////
float OrbitCamera::getElevation() const
{
    return m_elevation;
}

////////////////////////////////////////////////////////////
void OrbitCamera::computeRotation()
{
    glm::mat4 identityMat{ 1.0f };
    m_rotation = glm::rotate(identityMat, glm::radians(-m_elevation), kXAxis);
    m_rotation = glm::rotate(m_rotation, glm::radians(m_azimuth), kYAxis);
    m_viewTransformUpdated = false;
    m_invViewTransformUpdated = false;
}

////////////////////////////////////////////////////////////
const Transform& OrbitCamera::getViewTransform() const
{
    // Recompute the view matrix if needed
    if (!m_viewTransformUpdated)
    {

        glm::vec3 pos = glm::vec3(m_center.x, m_center.y, m_center.z);
        glm::vec3 scale = glm::vec3(m_scale.x, m_scale.y, m_scale.z);
        glm::vec3 target(0, 0, 0);
        glm::mat4 viewMat = glm::translate(glm::vec3(0.0, 0.0, -m_distance)) * m_rotation * glm::translate(-target);

        viewMat = glm::scale(viewMat, scale);
        //viewMat = glm::translate(viewMat, -pos);
        m_viewTransform = Transform(viewMat);
        m_viewTransformUpdated = true;
    }

    return m_viewTransform;
}
}
