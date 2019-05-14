////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/OrbitCamera.hpp>
#include <cmath>
#include <glm/gtx/quaternion.hpp>

namespace sf
{
constexpr glm::vec3 UP_AXIS(0, 1, 0);
constexpr glm::vec3 RIGHT_AXIS(1, 0, 0);
constexpr glm::quat DEFAULT_ROTATION(1, 0, 0, 0);
constexpr float DEFAULT_DISTANCE = 5.0f;

////////////////////////////////////////////////////////////
OrbitCamera::OrbitCamera(float fov, float near, float far)
    : Camera{ fov, near, far }
    , m_yaw{ 0.0f }
    , m_pitch{ 0.0f }
    , m_distance{ DEFAULT_DISTANCE }
    , m_rotation{ DEFAULT_ROTATION }
{
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
void OrbitCamera::setYaw(float yaw)
{
    m_yaw = yaw;
    computeRotation();
}

////////////////////////////////////////////////////////////
void OrbitCamera::changeYaw(float angle)
{
    m_yaw += angle;
    computeRotation();
}

////////////////////////////////////////////////////////////
float OrbitCamera::getYaw() const
{
    return m_yaw;
}

////////////////////////////////////////////////////////////
void OrbitCamera::setPitch(float pitch)
{
    m_pitch = pitch;
    computeRotation();
}

////////////////////////////////////////////////////////////
void OrbitCamera::changePitch(float angle)
{
    m_pitch += angle;
    computeRotation();
}

////////////////////////////////////////////////////////////
float OrbitCamera::getPitch() const
{
    return m_pitch;
}

////////////////////////////////////////////////////////////
void OrbitCamera::computeRotation()
{
    m_rotation = glm::quat(glm::vec3(glm::radians(m_pitch), glm::radians(m_yaw), 0.0));
    m_viewTransformUpdated = false;
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
        glm::mat4 viewMat = glm::translate(glm::vec3(0.0, 0.0, -m_distance)) * glm::toMat4(m_rotation) * glm::translate(-target);

        viewMat = glm::scale(viewMat, scale);
        viewMat = glm::translate(viewMat, -pos);
        m_viewTransform = Transform(viewMat);
        m_viewTransformUpdated = true;
    }

    return m_viewTransform;
}
}
