#ifndef SFML_ORBIT_CAMERA_HPP
#define SFML_ORBIT_CAMERA_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Camera.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace sf
{
////////////////////////////////////////////////////////////
/// \brief 3D camera that defines what is shown on screen
/// OrbitCamera is a arcball camera only allowed 
/// to turn around the target
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API OrbitCamera : public Camera
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Construct the camera from its field of view and clipping planes
    ///
    /// This constructor creates a camera with the given
    /// field of view, near clipping plane distance and far
    /// clipping plane distance.
    ///
    /// The camera will be targeted to default target (0, 0, 0) at
    /// default distance 5.0
    ///
    /// \param fov  Field of view of the camera in degrees
    /// \param near Near clipping plane distance of the camera
    /// \param far  Far clipping plane distance of the camera
    ///
    ////////////////////////////////////////////////////////////
    OrbitCamera(float fov, float near, float far);

    ////////////////////////////////////////////////////////////
    /// \brief Set the distance between camera and target
    ///
    /// \param distance The distance between camera and target 
    ///
    /// \see getDistance
    /// 
    ////////////////////////////////////////////////////////////

    void setDistance(float distance);

    ////////////////////////////////////////////////////////////
    /// \brief Get the distance between camera and target 
    ///
    /// \see setDirection
    ///
    ////////////////////////////////////////////////////////////
    float getDistance() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the euler angle around y axis
    ///
    /// \param yaw  The yaw angle in degree
    ///
    /// \see changeYaw  getYaw
    /// 
    ////////////////////////////////////////////////////////////
    void setYaw(float yaw);

    ////////////////////////////////////////////////////////////
    /// \brief Change the eular angle around y aixs
    ///
    /// \param angle The changed angle in degree
    ///
    /// \see setYaw  getYaw
    /// 
    ////////////////////////////////////////////////////////////
    void changeYaw(float angle);

    ////////////////////////////////////////////////////////////
    /// \brief Get the eular angle around y axis
    ///
    /// \see setYaw  changeYaw
    /// 
    ////////////////////////////////////////////////////////////
    float getYaw() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the euler angle around x axis
    ///
    /// \param pitch  The pitch angle in degree
    ///
    /// \see changePitch  getPitch
    /// 
    ////////////////////////////////////////////////////////////
    void setPitch(float pitch);

    ////////////////////////////////////////////////////////////
    /// \brief Change the eular angle around x aixs
    ///
    /// \param angle The changed angle in degree
    ///
    /// \see setPitch  getPitch
    /// 
    ////////////////////////////////////////////////////////////
    void changePitch(float angle);

    ////////////////////////////////////////////////////////////
    /// \brief Get the eular angle around x axis
    ///
    /// \see setPitch  changePitch
    /// 
    ////////////////////////////////////////////////////////////
    float getPitch() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the view transform of the camera
    ///
    /// This function is meant for internal use only.
    ///
    /// \return View transform of the camera
    ///
    /// \see getTransform, getInverseTransform
    ///
    ////////////////////////////////////////////////////////////
    const Transform& getViewTransform() const override;

private:

    void computeRotation();
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    float m_distance;
    float m_yaw;
    float m_pitch;
    glm::quat m_rotation;

};
}
#endif // SFML_ORBIT_CAMERA_HPP
