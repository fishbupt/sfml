////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/OrbitCamera.hpp>
#include <cmath>

namespace sf
{
const static Vector3f kXAxis(1, 0, 0);
const static Vector3f kZAxis(0, 0, 1);
const static Vector3f kYAxis(0, 1, 0);
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
    if (m_azimuth >= 180.0f) m_azimuth -= 360.0f;
    if (m_azimuth < -180.0f) m_azimuth += 360.0f;
    computeRotation();
}

////////////////////////////////////////////////////////////
void OrbitCamera::changeAzimuth(float angle)
{
    setAzimuth(m_azimuth + angle);
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
    setElevation(m_elevation + angle);
}

////////////////////////////////////////////////////////////
float OrbitCamera::getElevation() const
{
    return m_elevation;
}

////////////////////////////////////////////////////////////
void OrbitCamera::computeRotation()
{

    m_rotation = Transform::Identity;
    m_rotation.rotate(m_elevation, kXAxis);
    m_rotation.rotate(m_azimuth, kYAxis);

    m_viewTransformUpdated = false;
    m_invViewTransformUpdated = false;
}

////////////////////////////////////////////////////////////
const Transform& OrbitCamera::getViewTransform() const
{
    // Recompute the view matrix if needed
    if (!m_viewTransformUpdated)
    {
        m_viewTransform = Transform::Identity;
        m_viewTransform.translate(0, 0, -m_distance);
        m_viewTransform.combine(m_rotation);
        m_viewTransform.scale(m_scale);

        m_viewTransformUpdated = true;
    }

    return m_viewTransform;
}
}
