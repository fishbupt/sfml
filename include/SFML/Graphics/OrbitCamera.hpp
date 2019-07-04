#ifndef SFML_ORBIT_CAMERA_HPP
#define SFML_ORBIT_CAMERA_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Camera.hpp>
#include <SFML/Graphics/Transformable.hpp>

namespace sf
{
////////////////////////////////////////////////////////////
/// \brief 3D camera that defines what is shown on screen
/// OrbitCamera is a arcball camera only allowed 
/// to turn around the target
/// The Camera position is defined by two angles, the azimuth and
/// elevation. 
/// These angles are measured within a 3-D coordinate system that has its
/// origin at the center(0, 0, 0) of the chart
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
    /// \brief Set the Azimuth angle
    /// The Azimuth is the angle of ratation around the y-axis,
    /// as measured from the postive z-axis. Increasing this angle
    /// corresponds to countclockwise rotation about y-axis
    ///
    /// \param azimuth  The azimuth angle in degree
    ///
    /// \see changeYaw  getAzimuth
    /// 
    ////////////////////////////////////////////////////////////
    void setAzimuth(float azimuth);

    ////////////////////////////////////////////////////////////
    /// \brief Change the angle around z axis
    ///
    /// \param angle The changed angle in degree
    ///
    /// \see setYaw  getAzimuth
    /// 
    ////////////////////////////////////////////////////////////
    void changeAzimuth(float angle);

    ////////////////////////////////////////////////////////////
    /// \brief Get the angle around z axis
    ///
    /// \see setAzimuth  changeAzimuth
    /// 
    ////////////////////////////////////////////////////////////
    float getAzimuth() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the Elevation angle
    /// The Elevation is the minimum angle between the camera and
    /// x-z plane. Increasing the elevation from -90 to 90 degree
    /// corresponds to a rotation from the negative y-axis to
    /// the postive y-axis
    ///
    /// \param elevation  The elevation angle in degree
    ///
    /// \see changeElevation  getElevation
    /// 
    ////////////////////////////////////////////////////////////
    void setElevation(float elevation);

    ////////////////////////////////////////////////////////////
    /// \brief Change the angle around x-y plane
    ///
    /// \param angle The changed angle in degree
    ///
    /// \see setElevation  getElevation
    /// 
    ////////////////////////////////////////////////////////////
    void changeElevation(float angle);

    ////////////////////////////////////////////////////////////
    /// \brief Get the angle around x-y plane
    ///
    /// \see setElevation  changeElevation
    /// 
    ////////////////////////////////////////////////////////////
    float getElevation() const;

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
    float m_azimuth;        
    float m_elevation;     
    Transform m_rotation;

};
}
#endif // SFML_ORBIT_CAMERA_HPP
