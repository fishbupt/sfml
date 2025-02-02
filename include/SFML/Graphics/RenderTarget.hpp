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

#ifndef SFML_RENDERTARGET_HPP
#define SFML_RENDERTARGET_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <map>


namespace sf
{
class Drawable;
class VertexBuffer;

////////////////////////////////////////////////////////////
/// \brief Base class for all render targets (window, texture, ...)
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API RenderTarget : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~RenderTarget();

    ////////////////////////////////////////////////////////////
    /// \brief Clear the entire target with a single color
    ///
    /// This function is usually called once every frame,
    /// to clear the previous contents of the target.
    ///
    /// \param color Fill color to use to clear the render target
    ///
    ////////////////////////////////////////////////////////////
    void clear(const Color& color = Color(0, 0, 0, 255));

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable depth testing
    ///
    /// Depth testing removes the need to draw in back to front
    /// order. The graphics hardware will make sure objects
    /// that are positioned in front of other objects will
    /// be seen no matter when they are drawn.
    ///
    /// \param enable True to enable, false to disable
    ///
    ////////////////////////////////////////////////////////////
    void enableDepthTest(bool enable);

    ////////////////////////////////////////////////////////////
    /// \brief Change the current active view
    ///
    /// The view is like a camera, it controls which part of
    /// the scene is visible, and how it is viewed in the
    /// render-target.
    /// The new view will affect everything that is drawn, until
    /// another view is set.
    /// The render target keeps its own copy of the view object,
    /// so it is not necessary to keep the original one alive
    /// after calling this function.
    /// To restore the original view of the target, you can pass
    /// the result of getDefaultView() to this function.
    ///
    /// \param view New view to use
    ///
    /// \see getView, getDefaultView
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    void setView(const T& view);

    ////////////////////////////////////////////////////////////
    /// \brief Get the view currently in use in the render target
    ///
    /// \return The view object that is currently used
    ///
    /// \see setView, getDefaultView
    ///
    ////////////////////////////////////////////////////////////
    const View& getView() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the default view of the render target
    ///
    /// The default view has the initial size of the render target,
    /// and never changes after the target has been created.
    ///
    /// \return The default view of the render target
    ///
    /// \see setView, getView
    ///
    ////////////////////////////////////////////////////////////
    const View& getDefaultView() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the viewport of a view, applied to this render target
    ///
    /// The viewport is defined in the view as a ratio, this function
    /// simply applies this ratio to the current dimensions of the
    /// render target to calculate the pixels rectangle that the viewport
    /// actually covers in the target.
    ///
    /// \param view The view for which we want to compute the viewport
    ///
    /// \return Viewport rectangle, expressed in pixels
    ///
    ////////////////////////////////////////////////////////////
    IntRect getViewport(const View& view) const;

    ////////////////////////////////////////////////////////////
    /// \brief Convert a point from target coordinates to world
    ///        coordinates, using the current view
    ///
    /// This function is an overload of the mapPixelToCoords
    /// function that implicitely uses the current view.
    /// It is equivalent to:
    /// \code
    /// target.mapPixelToCoords(point, target.getView());
    /// \endcode
    ///
    /// \param point Pixel to convert
    ///
    /// \return The converted point, in "world" coordinates
    ///
    /// \see mapCoordsToPixel
    ///
    ////////////////////////////////////////////////////////////
    Vector2f mapPixelToCoords(const Vector2i& point) const;

    ////////////////////////////////////////////////////////////
    /// \brief Convert a point from target coordinates to world coordinates
    ///
    /// This function finds the 2D position that matches the
    /// given pixel of the render-target. In other words, it does
    /// the inverse of what the graphics card does, to find the
    /// initial position of a rendered pixel.
    ///
    /// Initially, both coordinate systems (world units and target pixels)
    /// match perfectly. But if you define a custom view or resize your
    /// render-target, this assertion is not true anymore, ie. a point
    /// located at (10, 50) in your render-target may map to the point
    /// (150, 75) in your 2D world -- if the view is translated by (140, 25).
    ///
    /// For render-windows, this function is typically used to find
    /// which point (or object) is located below the mouse cursor.
    ///
    /// This version uses a custom view for calculations, see the other
    /// overload of the function if you want to use the current view of the
    /// render-target.
    ///
    /// \param point Pixel to convert
    /// \param view The view to use for converting the point
    ///
    /// \return The converted point, in "world" units
    ///
    /// \see mapCoordsToPixel
    ///
    ////////////////////////////////////////////////////////////
    Vector2f mapPixelToCoords(const Vector2i& point, const View& view) const;

    ////////////////////////////////////////////////////////////
    /// \brief Convert a point from world coordinates to target
    ///        coordinates, using the current view
    ///
    /// This function is an overload of the mapCoordsToPixel
    /// function that implicitely uses the current view.
    /// It is equivalent to:
    /// \code
    /// target.mapCoordsToPixel(point, target.getView());
    /// \endcode
    ///
    /// \param point Point to convert
    ///
    /// \return The converted point, in target coordinates (pixels)
    ///
    /// \see mapPixelToCoords
    ///
    ////////////////////////////////////////////////////////////
    Vector2i mapCoordsToPixel(const Vector3f& point) const;

    ////////////////////////////////////////////////////////////
    /// \brief Convert a point from world coordinates to target coordinates
    ///
    /// This function finds the pixel of the render-target that matches
    /// the given 2D point. In other words, it goes through the same process
    /// as the graphics card, to compute the final position of a rendered point.
    ///
    /// Initially, both coordinate systems (world units and target pixels)
    /// match perfectly. But if you define a custom view or resize your
    /// render-target, this assertion is not true anymore, ie. a point
    /// located at (150, 75) in your 2D world may map to the pixel
    /// (10, 50) of your render-target -- if the view is translated by (140, 25).
    ///
    /// This version uses a custom view for calculations, see the other
    /// overload of the function if you want to use the current view of the
    /// render-target.
    ///
    /// \param point Point to convert
    /// \param view The view to use for converting the point
    ///
    /// \return The converted point, in target coordinates (pixels)
    ///
    /// \see mapPixelToCoords
    ///
    ////////////////////////////////////////////////////////////
    Vector2i mapCoordsToPixel(const Vector3f& point, const View& view) const;

    ////////////////////////////////////////////////////////////
    /// \brief Draw a drawable object to the render-target
    ///
    /// \param drawable Object to draw
    /// \param states   Render states to use for drawing
    ///
    ////////////////////////////////////////////////////////////
    void draw(const Drawable& drawable, const RenderStates& states = RenderStates::Default);

    ////////////////////////////////////////////////////////////
    /// \brief Draw a vertex buffer to the render-target
    ///
    /// \param buffer Vertex buffer to draw
    /// \param states Render states to use for drawing
    ///
    ////////////////////////////////////////////////////////////
    void draw(const VertexBuffer& buffer, const RenderStates& states = RenderStates::Default);

    ////////////////////////////////////////////////////////////
    /// \brief Draw primitives defined by an array of vertices
    ///
    /// \param vertices    Pointer to the vertices
    /// \param vertexCount Number of vertices in the array
    /// \param type        Type of primitives to draw
    /// \param states      Render states to use for drawing
    ///
    ////////////////////////////////////////////////////////////
    void draw(const Vertex* vertices, unsigned int vertexCount,
              PrimitiveType type, const RenderStates& states = RenderStates::Default);

    ////////////////////////////////////////////////////////////
    /// \brief Return the size of the rendering region of the target
    ///
    /// \return Size in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual Vector2u getSize() const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Save the current OpenGL render states and matrices
    ///
    /// This function can be used when you mix SFML drawing
    /// and direct OpenGL rendering. Combined with PopGLStates,
    /// it ensures that:
    /// \li SFML's internal states are not messed up by your OpenGL code
    /// \li your OpenGL states are not modified by a call to a SFML function
    ///
    /// More specifically, it must be used around code that
    /// calls Draw functions. Example:
    /// \code
    /// // OpenGL code here...
    /// window.pushGLStates();
    /// window.draw(...);
    /// window.draw(...);
    /// window.popGLStates();
    /// // OpenGL code here...
    /// \endcode
    ///
    /// Note that this function is quite expensive: it saves all the
    /// possible OpenGL states and matrices, even the ones you
    /// don't care about. Therefore it should be used wisely.
    /// It is provided for convenience, but the best results will
    /// be achieved if you handle OpenGL states yourself (because
    /// you know which states have really changed, and need to be
    /// saved and restored). Take a look at the ResetGLStates
    /// function if you do so.
    ///
    /// \see popGLStates
    ///
    ////////////////////////////////////////////////////////////
    void pushGLStates();

    ////////////////////////////////////////////////////////////
    /// \brief Restore the previously saved OpenGL render states and matrices
    ///
    /// See the description of pushGLStates to get a detailed
    /// description of these functions.
    ///
    /// \see pushGLStates
    ///
    ////////////////////////////////////////////////////////////
    void popGLStates();

    ////////////////////////////////////////////////////////////
    /// \brief Reset the internal OpenGL states so that the target is ready for drawing
    ///
    /// This function can be used when you mix SFML drawing
    /// and direct OpenGL rendering, if you choose not to use
    /// pushGLStates/popGLStates. It makes sure that all OpenGL
    /// states needed by SFML are set, so that subsequent draw()
    /// calls will work as expected.
    ///
    /// Example:
    /// \code
    /// // OpenGL code here...
    /// glPushAttrib(...);
    /// window.resetGLStates();
    /// window.draw(...);
    /// window.draw(...);
    /// glPopAttrib(...);
    /// // OpenGL code here...
    /// \endcode
    ///
    ////////////////////////////////////////////////////////////
    void resetGLStates();

protected :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    RenderTarget();

    ////////////////////////////////////////////////////////////
    /// \brief Performs the common initialization step after creation
    ///
    /// The derived classes must call this function after the
    /// target is created and ready for drawing.
    ///
    ////////////////////////////////////////////////////////////
    void initialize();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    bool m_clearDepth; ///< Whether there is a depth buffer to clear

private:

    ////////////////////////////////////////////////////////////
    /// \brief Apply the current view
    ///
    ////////////////////////////////////////////////////////////
    void applyCurrentView();

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new blending mode
    ///
    /// \param mode Blending mode to apply
    ///
    ////////////////////////////////////////////////////////////
    void applyBlendMode(BlendMode mode);

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new transform
    ///
    /// \param transform Transform to apply
    ///
    ////////////////////////////////////////////////////////////
    void applyTransform(const Transform& transform);

    ////////////////////////////////////////////////////////////
    /// \brief Apply the current view transform
    ///
    ////////////////////////////////////////////////////////////
    void applyViewTransform();

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new texture
    ///
    /// \param texture Texture to apply
    ///
    ////////////////////////////////////////////////////////////
    void applyTexture(const Texture* texture);

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new shader
    ///
    /// \param shader Shader to apply
    ///
    ////////////////////////////////////////////////////////////
    void applyShader(const Shader* shader);

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new vertex buffer
    ///
    /// \param buffer Vertex buffer to apply
    ///
    ////////////////////////////////////////////////////////////
    void applyVertexBuffer(const VertexBuffer* buffer);

    ////////////////////////////////////////////////////////////
    /// \brief Activate the target for rendering
    ///
    /// This function must be implemented by derived classes to make
    /// their OpenGL context current; it is called by the base class
    /// everytime it's going to use OpenGL calls.
    ///
    /// \param active True to make the target active, false to deactivate it
    ///
    /// \return True if the function succeeded
    ///
    ////////////////////////////////////////////////////////////
    virtual bool activate(bool active) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Try to set up the non-legacy rendering pipeline if available
    ///
    /// This function checks the GLSL version to see if version
    /// 1.30 or greater is supported. If that is the case, it will
    /// set up the default shader used for rendering that will
    /// emulate the legacy pipeline using the non-legacy OpenGL API.
    ///
    ////////////////////////////////////////////////////////////
    void setupNonLegacyPipeline();

    ////////////////////////////////////////////////////////////
    /// \brief Render states cache
    ///
    ////////////////////////////////////////////////////////////
    struct StatesCache
    {
        bool      glStatesSet;        ///< Are our internal GL states set yet?
        bool      viewChanged;        ///< Has the current view changed since last draw?
        BlendMode lastBlendMode;      ///< Cached blending mode
        Uint64    lastTextureId;      ///< Cached texture
        Uint64    lastVertexBufferId; ///< Cached vertex buffer
    };

    ////////////////////////////////////////////////////////////
    // Types
    ////////////////////////////////////////////////////////////
    typedef std::map<unsigned int, unsigned int> ArrayAgeCount;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    View          m_defaultView;            ///< Default view
    View*         m_view;                   ///< Current view
    StatesCache   m_cache;                  ///< Render states cache
    bool          m_depthTest;              ///< Whether depth testing is enabled
    Shader*       m_defaultShader;          ///< Default non-legacy shader, only created if supported
    const Shader* m_currentNonLegacyShader; ///< Used during a draw call to set uniforms of the target shader
    const Shader* m_lastNonLegacyShader;    ///< Used during a draw call to check if shader changed since the last draw
    Uint64        m_id;                     ///< Unique number that identifies the render target
    ArrayAgeCount m_arrayAgeCount;          ///< Map tracking the age of vertex array objects
    IntRect       m_previousViewport;       ///< Cached viewport
    Color         m_previousClearColor;     ///< Cached clear color
};

#include <SFML/Graphics/RenderTarget.inl>

} // namespace sf


#endif // SFML_RENDERTARGET_HPP


////////////////////////////////////////////////////////////
/// \class sf::RenderTarget
/// \ingroup graphics
///
/// sf::RenderTarget defines the common behaviour of all the
/// 2D render targets usable in the graphics module. It makes
/// it possible to draw 2D entities like sprites, shapes, text
/// without using any OpenGL command directly.
///
/// A sf::RenderTarget is also able to use views (sf::View),
/// which are a kind of 2D cameras. With views you can globally
/// scroll, rotate or zoom everything that is drawn,
/// without having to transform every single entity. See the
/// documentation of sf::View for more details and sample pieces of
/// code about this class.
///
/// On top of that, render targets are still able to render direct
/// OpenGL stuff. It is even possible to mix together OpenGL calls
/// and regular SFML drawing commands. When doing so, make sure that
/// OpenGL states are not messed up by calling the
/// pushGLStates/popGLStates functions.
///
/// \see sf::RenderWindow, sf::RenderTexture, sf::View
///
////////////////////////////////////////////////////////////
