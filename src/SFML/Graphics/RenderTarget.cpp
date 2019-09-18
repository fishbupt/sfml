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
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/Graphics/Light.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/Window/Context.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Err.hpp>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <map>
#include <sstream>

// GL_QUADS is unavailable on OpenGL ES, thus we need to define GL_QUADS ourselves
#ifdef SFML_OPENGL_ES

#    define GL_QUADS 0

#endif // SFML_OPENGL_ES

namespace
{
// Mutex to protect ID generation and our context-RenderTarget-map
sf::Mutex mutex;

// Unique identifier, used for identifying RenderTargets when
// tracking the currently active RenderTarget within a given context
sf::Uint64 getUniqueId()
{
    sf::Lock lock(mutex);

    static sf::Uint64 id = 1; // start at 1, zero is "no RenderTarget"

    return id++;
}

// Map to help us detect whether a different RenderTarget
// has been activated within a single context
typedef std::map<sf::Uint64, sf::Uint64> ContextRenderTargetMap;
ContextRenderTargetMap contextRenderTargetMap;

// Check if a RenderTarget with the given ID is active in the current context
bool isActive(sf::Uint64 id)
{
    ContextRenderTargetMap::iterator iter = contextRenderTargetMap.find(sf::Context::getActiveContextId());

    if ((iter == contextRenderTargetMap.end()) || (iter->second != id))
        return false;

    return true;
}

// Convert an sf::BlendMode::Factor constant to the corresponding OpenGL constant.
sf::Uint32 factorToGlConstant(sf::BlendMode::Factor blendFactor)
{
    switch (blendFactor)
    {
    case sf::BlendMode::Zero:
        return GL_ZERO;
    case sf::BlendMode::One:
        return GL_ONE;
    case sf::BlendMode::SrcColor:
        return GL_SRC_COLOR;
    case sf::BlendMode::OneMinusSrcColor:
        return GL_ONE_MINUS_SRC_COLOR;
    case sf::BlendMode::DstColor:
        return GL_DST_COLOR;
    case sf::BlendMode::OneMinusDstColor:
        return GL_ONE_MINUS_DST_COLOR;
    case sf::BlendMode::SrcAlpha:
        return GL_SRC_ALPHA;
    case sf::BlendMode::OneMinusSrcAlpha:
        return GL_ONE_MINUS_SRC_ALPHA;
    case sf::BlendMode::DstAlpha:
        return GL_DST_ALPHA;
    case sf::BlendMode::OneMinusDstAlpha:
        return GL_ONE_MINUS_DST_ALPHA;
    }

    sf::err() << "Invalid value for sf::BlendMode::Factor! Fallback to sf::BlendMode::Zero." << std::endl;
    assert(false);
    return GL_ZERO;
}

// Convert an sf::BlendMode::BlendEquation constant to the corresponding OpenGL constant.
sf::Uint32 equationToGlConstant(sf::BlendMode::Equation blendEquation)
{
    switch (blendEquation)
    {
    case sf::BlendMode::Add:
        return GLEXT_GL_FUNC_ADD;
    case sf::BlendMode::Subtract:
        return GLEXT_GL_FUNC_SUBTRACT;
    case sf::BlendMode::ReverseSubtract:
        return GLEXT_GL_FUNC_REVERSE_SUBTRACT;
    }

    sf::err() << "Invalid value for sf::BlendMode::Equation! Fallback to sf::BlendMode::Add." << std::endl;
    assert(false);
    return GLEXT_GL_FUNC_ADD;
}
}

namespace sf
{
////////////////////////////////////////////////////////////
RenderTarget::RenderTarget()
    : m_defaultView()
    , m_view()
    , m_cache()
    , m_depthTest(false)
    , m_defaultShader(nullptr)
    , m_currentNonLegacyShader(nullptr)
    , m_lastNonLegacyShader(nullptr)
{
    m_cache.glStatesSet = false;
    Light::increaseLightReferences();
}

////////////////////////////////////////////////////////////
RenderTarget::~RenderTarget()
{
    Light::decreaseLightReferences();
}

////////////////////////////////////////////////////////////
void RenderTarget::enableDepthTest(bool enable)
{
    m_depthTest = enable;

    if (enable)
    {
        glCheck(glEnable(GL_DEPTH_TEST));
    }
    else
        glCheck(glDisable(GL_DEPTH_TEST));
}

////////////////////////////////////////////////////////////
void RenderTarget::clear(const Color& color)
{
    if (isActive(m_id) || setActive(true))
    {
        // Unbind texture to fix RenderTexture preventing clear
        applyTexture(NULL);

        glCheck(glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
        glCheck(glClear(GL_COLOR_BUFFER_BIT | (m_depthTest ? GL_DEPTH_BUFFER_BIT : 0)));
    }
}

////////////////////////////////////////////////////////////
const View& RenderTarget::getView() const
{
    return *m_view;
}

////////////////////////////////////////////////////////////
const View& RenderTarget::getDefaultView() const
{
    return m_defaultView;
}

////////////////////////////////////////////////////////////
IntRect RenderTarget::getViewport(const View& view) const
{
    float width = static_cast<float>(getSize().x);
    float height = static_cast<float>(getSize().y);
    const FloatRect& viewport = view.getViewport();

    return IntRect(static_cast<int>(0.5f + width * viewport.left), static_cast<int>(0.5f + height * viewport.top),
                   static_cast<int>(0.5f + width * viewport.width), static_cast<int>(0.5f + height * viewport.height));
}

////////////////////////////////////////////////////////////
Vector2f RenderTarget::mapPixelToCoords(const Vector2i& point) const
{
    return mapPixelToCoords(point, getView());
}

////////////////////////////////////////////////////////////
Vector2f RenderTarget::mapPixelToCoords(const Vector2i& point, const View& view) const
{
    // First, convert from viewport coordinates to homogeneous coordinates
    Vector2f normalized;
    IntRect viewport = getViewport(view);
    normalized.x = -1.f + 2.f * (point.x - viewport.left) / viewport.width;
    normalized.y = 1.f - 2.f * (point.y - viewport.top) / viewport.height;

    // Then transform by the inverse of the view matrix
    return view.getInverseTransform().transformPoint(normalized);
}

////////////////////////////////////////////////////////////
Vector2i RenderTarget::mapCoordsToPixel(const Vector3f& point) const
{
    return mapCoordsToPixel(point, getView());
}

////////////////////////////////////////////////////////////
Vector2i RenderTarget::mapCoordsToPixel(const Vector3f& point, const View& view) const
{
    // First, transform the point by the modelview and projection matrix
    Vector3f normalized = (view.getTransform() * view.getViewTransform()).transformPoint(point);

    // Then convert to viewport coordinates
    Vector2i pixel;
    IntRect viewport = getViewport(view);
    pixel.x = static_cast<int>((normalized.x + 1.f) / 2.f * viewport.width + viewport.left);
    pixel.y = static_cast<int>((-normalized.y + 1.f) / 2.f * viewport.height + viewport.top);

    return pixel;
}

////////////////////////////////////////////////////////////
void RenderTarget::draw(const Drawable& drawable, const RenderStates& states)
{
    drawable.draw(*this, states);
}

////////////////////////////////////////////////////////////
void RenderTarget::draw(const Vertex* vertices, std::size_t vertexCount, PrimitiveType type, const RenderStates& states)
{
    // Nothing to draw?
    if (!vertices || (vertexCount == 0))
        return;

// GL_QUADS is unavailable on OpenGL ES
#ifdef SFML_OPENGL_ES
    if (type == Quads)
    {
        err() << "sf::Quads primitive type is not supported on OpenGL ES platforms, drawing skipped" << std::endl;
        return;
    }
#endif

    if (isActive(m_id) || setActive(true))
    {
        // Check if the vertex count is low enough so that we can pre-transform them
        // only enable cache mode if non-legacy shader is disabled
        bool useVertexCache = (vertexCount <= StatesCache::VertexCacheSize) && !m_defaultShader;

        if (useVertexCache)
        {
            // Pre-transform the vertices and store them into the vertex cache
            for (std::size_t i = 0; i < vertexCount; ++i)
            {
                Vertex& vertex = m_cache.vertexCache[i];
                vertex.position = states.transform * vertices[i].position;
                vertex.color = vertices[i].color;
                vertex.texCoords = vertices[i].texCoords;
            }
        }

        setupDraw(useVertexCache, states);

        if (!m_defaultShader)
        {
            // Check if texture coordinates array is needed, and update client state accordingly
            bool enableTexCoordsArray = (states.texture || states.shader);
            if (!m_cache.enable || (enableTexCoordsArray != m_cache.texCoordsArrayEnabled))
            {
                if (enableTexCoordsArray)
                    glCheck(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
                else
                    glCheck(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
            }

            // If we switch between non-cache and cache mode or enable texture
            // coordinates we need to set up the pointers to the vertices' components
            if (!m_cache.enable || !useVertexCache || !m_cache.useVertexCache)
            {
                const char* data = reinterpret_cast<const char*>(vertices);

                // If we pre-transform the vertices, we must use our internal vertex cache
                if (useVertexCache)
                    data = reinterpret_cast<const char*>(m_cache.vertexCache);

                glCheck(glVertexPointer(3, GL_FLOAT, sizeof(Vertex), data + offsetof(Vertex, position)));
                glCheck(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), data + offsetof(Vertex, color)));
                if (enableTexCoordsArray)
                    glCheck(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), data + offsetof(Vertex, texCoords)));
                glCheck(glNormalPointer(GL_FLOAT, sizeof(Vertex), data + offsetof(Vertex, normal)));
            }
            else if (enableTexCoordsArray && !m_cache.texCoordsArrayEnabled)
            {
                // If we enter this block, we are already using our internal vertex cache
                const char* data = reinterpret_cast<const char*>(m_cache.vertexCache);

                glCheck(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), data + offsetof(Vertex, texCoords)));
            }
            drawPrimitives(type, 0, vertexCount);
            // Update the cache
            m_cache.useVertexCache = useVertexCache;
            m_cache.texCoordsArrayEnabled = enableTexCoordsArray;
        }
        else
        {
            Shader *shader = const_cast<Shader*>(m_currentNonLegacyShader);

            Light::addLightsToShader(*shader);

            int vertexLocation = shader->getVertexAttributeLocation("sf_Vertex");
            int colorLocation = shader->getVertexAttributeLocation("sf_Color");
            int texCoordLocation = shader->getVertexAttributeLocation("sf_MultiTexCoord0");
            int normalLocation = shader->getVertexAttributeLocation("sf_Normal");

            const char* data = reinterpret_cast<const char*>(vertices);

            if (vertexLocation >= 0)
            {
                glCheck(glEnableVertexAttribArrayARB(vertexLocation));
                glCheck(glVertexAttribPointerARB(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), data + offsetof(Vertex, position)));
            }

            if (colorLocation >= 0)
            {
                glCheck(glEnableVertexAttribArrayARB(colorLocation));
                glCheck(glVertexAttribPointerARB(colorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), data + offsetof(Vertex, color)));
            }

            if (texCoordLocation >= 0)
            {
                glCheck(glEnableVertexAttribArrayARB(texCoordLocation));
                glCheck(glVertexAttribPointerARB(texCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), data + offsetof(Vertex, texCoords)));
            }

            if (normalLocation >= 0)
            {
                glCheck(glEnableVertexAttribArrayARB(normalLocation));
                glCheck(glVertexAttribPointerARB(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), data + offsetof(Vertex, normal)));
            }

            // Draw the primitives
            drawPrimitives(type, 0, vertexCount);

            if (vertexLocation >= 0)
                glCheck(glDisableVertexAttribArrayARB(vertexLocation));

            if (colorLocation >= 0)
                glCheck(glDisableVertexAttribArrayARB(colorLocation));

            if (texCoordLocation >= 0)
                glCheck(glDisableVertexAttribArrayARB(texCoordLocation));

            if (normalLocation >= 0)
                glCheck(glDisableVertexAttribArrayARB(normalLocation));

        }
        cleanupDraw(states);

    }
}

////////////////////////////////////////////////////////////
void RenderTarget::draw(const VertexBuffer& vertexBuffer, const RenderStates& states)
{
    draw(vertexBuffer, 0, vertexBuffer.getVertexCount(), states);
}

////////////////////////////////////////////////////////////
void RenderTarget::draw(const VertexBuffer& vertexBuffer, std::size_t firstVertex, std::size_t vertexCount, const RenderStates& states)
{
    // VertexBuffer not supported?
    if (!VertexBuffer::isAvailable())
    {
        err() << "sf::VertexBuffer is not available, drawing skipped" << std::endl;
        return;
    }

    // Sanity check
    if (firstVertex > vertexBuffer.getVertexCount())
        return;

    // Clamp vertexCount to something that makes sense
    vertexCount = std::min(vertexCount, vertexBuffer.getVertexCount() - firstVertex);

    // Nothing to draw?
    if (!vertexCount || !vertexBuffer.getNativeHandle())
        return;

// GL_QUADS is unavailable on OpenGL ES
#ifdef SFML_OPENGL_ES
    if (vertexBuffer.getPrimitiveType() == Quads)
    {
        err() << "sf::Quads primitive type is not supported on OpenGL ES platforms, drawing skipped" << std::endl;
        return;
    }
#endif

    if (isActive(m_id) || setActive(true))
    {
        setupDraw(false, states);

        if (!m_defaultShader)
        {
            // Apply the vertex buffer
            auto vertexBufferId = vertexBuffer.m_cacheId;
            if (vertexBufferId != m_cache.lastVertexBufferId)
                applyVertexBuffer(&vertexBuffer);

            // Bind vertex buffer
            // VertexBuffer::bind(&vertexBuffer);

            // Always enable texture coordinates
            if (!m_cache.enable || !m_cache.texCoordsArrayEnabled)
                glCheck(glEnableClientState(GL_TEXTURE_COORD_ARRAY));

            glCheck(glVertexPointer(3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, position))));
            glCheck(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, color))));
            glCheck(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, texCoords))));
            glCheck(glNormalPointer(GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal))));

            drawPrimitives(vertexBuffer.getPrimitiveType(), firstVertex, vertexCount);

            // Unbind vertex buffer
            applyVertexBuffer(nullptr);

            // Update the cache
            m_cache.useVertexCache = false;
            m_cache.texCoordsArrayEnabled = true;
        }
        else
        {
            Shader *shader = const_cast<Shader*>(m_currentNonLegacyShader);

            Light::addLightsToShader(*shader);

            unsigned int arrayObject = 0;
            bool newArray = true;
            bool needUpload = false;

#if 0
            if (VertexBuffer::hasVertexArrayObjects())
            {
                // Lookup the current context (id, shader id) in the VertexBuffer
                std::pair<Uint64, Uint64> contextIdentifier(m_id, shader->m_id);
                VertexBuffer::ArrayObjects::iterator arrayObjectIter = vertexBuffer.m_arrayObjects.find(contextIdentifier);

                if (arrayObjectIter == vertexBuffer.m_arrayObjects.end())
                {
                    // VertexBuffer doesn't have a VAO in this context

                    // Create a new VAO
                    glCheck(glGenVertexArrays(1, &arrayObject));

                    // Register the VAO with the VertexBuffer
                    vertexBuffer.m_arrayObjects[contextIdentifier] = arrayObject;

                    // Mark the VAO age as 0
                    m_arrayAgeCount[arrayObject] = 0;
                }
                else
                {
                    // VertexBuffer has/had a VAO in this context

                    // Grab the VAO identifier from the VertexBuffer
                    arrayObject = arrayObjectIter->second;

                    // Still need to check if it still exists
                    ArrayAgeCount::iterator arrayAge = m_arrayAgeCount.find(arrayObject);

                    if (arrayAge != m_arrayAgeCount.end())
                    {
                        // VAO still exists in this context
                        newArray = false;

                        // Check if the VertexBuffer data needs to be re-uploaded
                        needUpload = vertexBuffer.m_needUpload;

                        // Mark the VAO age as 0
                        arrayAge->second = 0;
                    }
                    else
                    {
                        // VAO needs to be recreated in this context

                        // Create a new VAO
                        glCheck(glGenVertexArrays(1, &arrayObject));

                        // Register the VAO with the VertexBuffer
                        arrayObjectIter->second = arrayObject;

                        // Mark the VAO age as 0
                        m_arrayAgeCount[arrayObject] = 0;
                    }
                }

                glBindVertexArray(arrayObject);

                // Maximum array object age in draw calls before being purged
                // If an array object was not used to draw this many
                // calls, it will be considered expired and purged
                // from the context owned by this RenderTarget
                const static unsigned int maxArrayObjectAge = 10000;

                // Increment age counters and purge all expired VAOs
                for (ArrayAgeCount::iterator arrayAge = m_arrayAgeCount.begin(); arrayAge != m_arrayAgeCount.end();)
                {
                    arrayAge->second++;

                    if (arrayAge->second > maxArrayObjectAge)
                    {
                        glCheck(glDeleteVertexArrays(1, &(arrayAge->first)));
                        m_arrayAgeCount.erase(arrayAge++);
                        continue;
                    }

                    ++arrayAge;
                }
            }
#endif 

            int vertexLocation = -1;
            int colorLocation = -1;
            int texCoordLocation = -1;
            int normalLocation = -1;

            // If we are creating a new array object or buffer data
            // needs to be re-uploaded, we need to rebind even if
            // it is still currently bound
            if (newArray || needUpload)
            {
                // Apply the vertex buffer
                Uint64 vertexBufferId = vertexBuffer.m_cacheId;
                applyVertexBuffer(&vertexBuffer);
            }

            if (newArray)
            {
                vertexLocation = shader->getVertexAttributeLocation("sf_Vertex");
                colorLocation = shader->getVertexAttributeLocation("sf_Color");
                texCoordLocation = shader->getVertexAttributeLocation("sf_MultiTexCoord0");
                normalLocation = shader->getVertexAttributeLocation("sf_Normal");

                if (vertexLocation >= 0)
                {
                    glCheck(glEnableVertexAttribArrayARB(vertexLocation));
                    glCheck(glVertexAttribPointerARB(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position))));
                }

                if (colorLocation >= 0)
                {
                    glCheck(glEnableVertexAttribArrayARB(colorLocation));
                    glCheck(glVertexAttribPointerARB(colorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color))));
                }

                if (texCoordLocation >= 0)
                {
                    glCheck(glEnableVertexAttribArrayARB(texCoordLocation));
                    glCheck(glVertexAttribPointerARB(texCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords))));
                }

                if (normalLocation >= 0)
                {
                    glCheck(glEnableVertexAttribArrayARB(normalLocation));
                    glCheck(glVertexAttribPointerARB(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal))));
                }
            }

            // Draw the primitives
            drawPrimitives(vertexBuffer.getPrimitiveType(), firstVertex, vertexCount);

            //if (arrayObject)
            //    glBindVertexArray(0);

            if (vertexLocation >= 0)
                glCheck(glDisableVertexAttribArrayARB(vertexLocation));

            if (colorLocation >= 0)
                glCheck(glDisableVertexAttribArrayARB(colorLocation));

            if (texCoordLocation >= 0)
                glCheck(glDisableVertexAttribArrayARB(texCoordLocation));

            if (normalLocation >= 0)
                glCheck(glDisableVertexAttribArrayARB(normalLocation));
        }
        // Unbind vertex buffer
        applyVertexBuffer(nullptr);
        cleanupDraw(states);
    }
}

////////////////////////////////////////////////////////////
bool RenderTarget::setActive(bool active)
{
    // Mark this RenderTarget as active or no longer active in the tracking map
    {
        sf::Lock lock(mutex);

        Uint64 contextId = Context::getActiveContextId();

        ContextRenderTargetMap::iterator iter = contextRenderTargetMap.find(contextId);

        if (active)
        {
            if (iter == contextRenderTargetMap.end())
            {
                contextRenderTargetMap[contextId] = m_id;

                m_cache.enable = false;
            }
            else if (iter->second != m_id)
            {
                iter->second = m_id;

                m_cache.enable = false;
            }
        }
        else
        {
            if (iter != contextRenderTargetMap.end())
                contextRenderTargetMap.erase(iter);

            m_cache.enable = false;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////
void RenderTarget::pushGLStates()
{
    if (isActive(m_id) || setActive(true))
    {
#ifdef SFML_DEBUG
        // make sure that the user didn't leave an unchecked OpenGL error
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            err() << "OpenGL error (" << error << ") detected in user code, "
                  << "you should check for errors with glGetError()" << std::endl;
        }
#endif

        glCheck(glPushAttrib(GL_ALL_ATTRIB_BITS));
        if (!m_defaultShader)
        {
#ifndef SFML_OPENGL_ES
            glCheck(glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS));
#endif
            glCheck(glMatrixMode(GL_MODELVIEW));
            glCheck(glPushMatrix());
            glCheck(glMatrixMode(GL_PROJECTION));
            glCheck(glPushMatrix());
            glCheck(glMatrixMode(GL_TEXTURE));
            glCheck(glPushMatrix());
        }
    }

    resetGLStates();
}

////////////////////////////////////////////////////////////
void RenderTarget::popGLStates()
{
    if (isActive(m_id) || setActive(true))
    {
        if (m_defaultShader)
            applyShader(nullptr);

        if (!m_defaultShader)
        {
            glCheck(glMatrixMode(GL_PROJECTION));
            glCheck(glPopMatrix());
            glCheck(glMatrixMode(GL_MODELVIEW));
            glCheck(glPopMatrix());
            glCheck(glMatrixMode(GL_TEXTURE));
            glCheck(glPopMatrix());
#ifndef SFML_OPENGL_ES
            glCheck(glPopClientAttrib());
#endif
        }
        glCheck(glPopAttrib());
    }
}

////////////////////////////////////////////////////////////
void RenderTarget::resetGLStates()
{
    // Check here to make sure a context change does not happen after activate(true)
    bool shaderAvailable = Shader::isAvailable();
    bool vertexBufferAvailable = VertexBuffer::isAvailable();

// Workaround for states not being properly reset on
// macOS unless a context switch really takes place
#if defined(SFML_SYSTEM_MACOS)
    setActive(false);
#endif

    if (isActive(m_id) || setActive(true))
    {
        // Make sure that extensions are initialized
        priv::ensureExtensionsInit();

        // Make sure that the texture unit which is active is the number 0
        if (GLEXT_multitexture)
        {
            glCheck(GLEXT_glClientActiveTexture(GLEXT_GL_TEXTURE0));
            glCheck(GLEXT_glActiveTexture(GLEXT_GL_TEXTURE0));
        }

        // Define the default OpenGL states
        glCheck(glDisable(GL_LIGHTING));
        if (!m_depthTest)
            glCheck(glDisable(GL_DEPTH_TEST));
        glCheck(glDisable(GL_ALPHA_TEST));
        glCheck(glEnable(GL_BLEND));
        glCheck(glDisable(GL_CULL_FACE));

        if (!m_defaultShader)
        {
            glCheck(glEnable(GL_TEXTURE_2D));
            glCheck(glEnable(GL_COLOR_MATERIAL));
            glCheck(glEnable(GL_NORMALIZE));
            glCheck(glMatrixMode(GL_MODELVIEW));
            glCheck(glLoadIdentity());
            glCheck(glEnableClientState(GL_VERTEX_ARRAY));
            glCheck(glEnableClientState(GL_COLOR_ARRAY));
            glCheck(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
            glCheck(glEnableClientState(GL_NORMAL_ARRAY));
        }
        glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        m_cache.glStatesSet = true;

        // Apply the default SFML states
        applyBlendMode(BlendAlpha);
        applyTexture(nullptr);
        if (shaderAvailable)
        {
            if (!m_defaultShader)
                applyShader(nullptr);
            else
                applyShader(m_defaultShader.get());
        }

        if (vertexBufferAvailable)
            VertexBuffer::bind(nullptr);

        m_cache.texCoordsArrayEnabled = true;

        m_cache.useVertexCache = false;

        if (VertexBuffer::isAvailable())
            applyVertexBuffer(nullptr);

        m_cache.enable = true;
    }
}

////////////////////////////////////////////////////////////
void RenderTarget::initialize(bool tryToSetupNonLegacyPipeLine)
{
    // Setup the default and current views
    m_defaultView.reset(FloatRect(0, 0, static_cast<float>(getSize().x), static_cast<float>(getSize().y)));

    // Set GL states only on first draw, so that we don't pollute user's states
    m_cache.glStatesSet = false;

    // Generate a unique ID for this RenderTarget to track
    // whether it is active within a specific context
    m_id = getUniqueId();

    // Try to set up non-legacy pipeline if available
    if(tryToSetupNonLegacyPipeLine)
        setupNonLegacyPipeline();

    setView(m_defaultView);
}

////////////////////////////////////////////////////////////
void RenderTarget::applyCurrentView()
{
    // Set the viewport
    IntRect viewport = getViewport(*m_view);
    int top = getSize().y - (viewport.top + viewport.height);
    glCheck(glViewport(viewport.left, top, viewport.width, viewport.height));

    if (m_defaultShader)
    {
        Shader* shader = m_currentNonLegacyShader ? const_cast<Shader*>(m_currentNonLegacyShader) : m_defaultShader.get();

        shader->setUniform("sf_ProjectionMatrix", Glsl::Mat4(m_view->getTransform()));
        shader->setUniform("sf_ViewMatrix", Glsl::Mat4(m_view->getViewTransform()));
        shader->setUniform("sf_ViewerPosition", m_view->getCenter());
        shader->setUniform("sf_CheckBoundary", m_view->getCheckBoundary());
           
    }
    else
    {
        // Set the projection matrix
        glCheck(glMatrixMode(GL_PROJECTION));
        glCheck(glLoadMatrixf(m_view->getTransform().getMatrix()));

        // Go back to model-view mode
        glCheck(glMatrixMode(GL_MODELVIEW));
    }
    m_cache.viewChanged = false;
}

////////////////////////////////////////////////////////////
void RenderTarget::applyBlendMode(const BlendMode& mode)
{
    // Apply the blend mode, falling back to the non-separate versions if necessary
    if (GLEXT_blend_func_separate)
    {
        glCheck(GLEXT_glBlendFuncSeparate(factorToGlConstant(mode.colorSrcFactor), factorToGlConstant(mode.colorDstFactor),
                                          factorToGlConstant(mode.alphaSrcFactor), factorToGlConstant(mode.alphaDstFactor)));
    }
    else
    {
        glCheck(glBlendFunc(factorToGlConstant(mode.colorSrcFactor), factorToGlConstant(mode.colorDstFactor)));
    }

    if (GLEXT_blend_minmax && GLEXT_blend_subtract)
    {
        if (GLEXT_blend_equation_separate)
        {
            glCheck(GLEXT_glBlendEquationSeparate(equationToGlConstant(mode.colorEquation), equationToGlConstant(mode.alphaEquation)));
        }
        else
        {
            glCheck(GLEXT_glBlendEquation(equationToGlConstant(mode.colorEquation)));
        }
    }
    else if ((mode.colorEquation != BlendMode::Add) || (mode.alphaEquation != BlendMode::Add))
    {
        static bool warned = false;

        if (!warned)
        {
            err() << "OpenGL extension EXT_blend_minmax and/or EXT_blend_subtract unavailable" << std::endl;
            err() << "Selecting a blend equation not possible" << std::endl;
            err() << "Ensure that hardware acceleration is enabled if available" << std::endl;

            warned = true;
        }
    }

    m_cache.lastBlendMode = mode;
}

////////////////////////////////////////////////////////////
void RenderTarget::applyTransform(const Transform& transform)
{
    if (m_defaultShader)
    {
        Shader* shader = m_currentNonLegacyShader ? const_cast<Shader*>(m_currentNonLegacyShader) : m_defaultShader.get();

        shader->setUniform("sf_ModelMatrix", Glsl::Mat4(transform));

        const float* modelMatrix = transform.getMatrix();
        Transform normalMatrix(modelMatrix[0], modelMatrix[4], modelMatrix[8], 0.f, modelMatrix[1], modelMatrix[5], modelMatrix[9], 0.f,
                               modelMatrix[2], modelMatrix[6], modelMatrix[10], 0.f, 0.f, 0.f, 0.f, 1.f);

        if (sf::Light::isLightingEnabled())
            shader->setUniform("sf_NormalMatrix", Glsl::Mat4(normalMatrix.getInverse().getTranspose()));
    }
    else
    {
        // No need to call glMatrixMode(GL_MODELVIEW), it is always the
        // current mode (for optimization purpose, since it's the most used)
        if (transform == Transform::Identity)
            glCheck(glLoadIdentity());
        else
            glCheck(glLoadMatrixf((m_view->getViewTransform() * transform).getMatrix()));
    }
}

////////////////////////////////////////////////////////////
void RenderTarget::applyViewTransform()
{
    if (!m_defaultShader)
    {
        // No need to call glMatrixMode(GL_MODELVIEW), it is always the
        // current mode (for optimization purpose, since it's the most used)
        glCheck(glLoadMatrixf(m_view->getViewTransform().getMatrix()));
    }
}

////////////////////////////////////////////////////////////
void RenderTarget::applyTexture(const Texture* texture)
{
    if (m_defaultShader)
    {
        Shader* shader = m_currentNonLegacyShader ? const_cast<Shader*>(m_currentNonLegacyShader) : m_defaultShader.get();

        float xScale = 1.f;
        float yScale = 1.f;
        float yFlip = 0.f;

        if (texture)
        {
            // Setup scale factors that convert the range [0 .. size] to [0 .. 1]
            xScale = 1.f / texture->m_actualSize.x;
            yScale = 1.f / texture->m_actualSize.y;

            // If pixels are flipped we must invert the Y axis
            if (texture->m_pixelsFlipped)
            {
                yScale = -yScale;
                yFlip = static_cast<float>(texture->m_size.y) / texture->m_actualSize.y;
            }

            Transform textureMatrix(xScale, 0.f, 0.f, 0.f, 0.f, yScale, 0.f, yFlip, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f);

            shader->setUniform("sf_TextureMatrix", Glsl::Mat4(textureMatrix));
            shader->setUniform("sf_Texture0", *texture);
            shader->setUniform("sf_TextureEnabled", 1);
        }
        else
            shader->setUniform("sf_TextureEnabled", 0);
    }
    else
    {
        Texture::bind(texture, Texture::Pixels);
    }
    m_cache.lastTextureId = texture ? texture->m_cacheId : 0;
}

////////////////////////////////////////////////////////////
void RenderTarget::applyShader(const Shader* shader)
{
    Shader::bind(shader);
}

////////////////////////////////////////////////////////////
void RenderTarget::applyVertexBuffer(const VertexBuffer* buffer)
{
    VertexBuffer::bind(buffer);

    m_cache.lastVertexBufferId = buffer ? buffer->m_cacheId : 0;
}

////////////////////////////////////////////////////////////
void RenderTarget::setupNonLegacyPipeline()
{
    // Setup the default shader if non-legacy rendering is supported

    // Check if our shader lighting implementation is supported
    if (!Light::hasShaderLighting())
        return;

    double versionNumber = 0.0;
    std::istringstream versionStringStream(Shader::getSupportedVersion());
    versionStringStream >> versionNumber;

    // Disable non-legacy pipeline if requested
#if defined(SFML_LEGACY_GL)
    versionNumber = 0.0;
#endif

    // This will only succeed if the supported version is not GLSL ES
    if (versionNumber > 1.29)
    {
        m_defaultShader = std::make_unique<Shader>();

        std::stringstream vertexShaderSource;
        vertexShaderSource << "#version 130\n"
                              "\n"
                              "// Uniforms\n"
                              "uniform mat4 sf_ModelMatrix;\n"
                              "uniform mat4 sf_ViewMatrix;\n"
                              "uniform mat4 sf_ProjectionMatrix;\n"
                              "uniform mat4 sf_TextureMatrix;\n"
                              "uniform int sf_TextureEnabled;\n"
                              "uniform int sf_LightingEnabled;\n"
                              "uniform bool sf_CheckBoundary;\n"
                              "\n"
                              "// Vertex attributes\n"
                              "in vec3 sf_Vertex;\n"
                              "in vec4 sf_Color;\n"
                              "in vec2 sf_MultiTexCoord0;\n"
                              "in vec3 sf_Normal;\n"
                              "\n"
                              "// Vertex shader outputs\n"
                              "out vec4 sf_FrontColor;\n"
                              "out vec2 sf_TexCoord0;\n"
                              "out vec3 sf_FragWorldPosition;\n"
                              "out vec3 sf_FragNormal;\n"
                              "\n"
                              "void main()\n"
                              "{\n"
                              "    // Vertex position\n"
                              "    gl_Position = sf_ProjectionMatrix * sf_ViewMatrix * sf_ModelMatrix * vec4(sf_Vertex, 1.0);\n"
                              "\n"
                              "    // Vertex color\n"
                              "    sf_FrontColor = sf_Color;\n"
                              "    vec4 pos = sf_ModelMatrix * vec4(sf_Vertex, 1.0);\n"
                              "    if (sf_CheckBoundary && (pos.x > 1.0 || pos.x < -1.0 || pos.y > 1.0 || pos.y < -1.0 || pos.z > 1.0 || pos.z < -1.0))\n"
                              "        sf_FrontColor.a = 0.0;\n"
                              "\n"
                              "    // Texture data\n"
                              "    //if (sf_TextureEnabled == 1)\n"
                              "    //    sf_TexCoord0 = (sf_TextureMatrix * vec4(sf_MultiTexCoord0, 0.0, 1.0)).st;\n"
                              "\n"
                              "    // Lighting data\n"
                              "    //if (sf_LightingEnabled > 0)\n"
                              "    //{\n"
                              "    //    sf_FragNormal = sf_Normal;\n"
                              "    //    sf_FragWorldPosition = vec3(sf_ModelMatrix * vec4(sf_Vertex, 1.0));\n"
                              "    //}\n"
                              "}\n";

        std::stringstream fragmentShaderSource;
        fragmentShaderSource << "#version 130\n";

        if (Shader::isUniformBufferAvailable())
            fragmentShaderSource << "#extension GL_ARB_uniform_buffer_object : enable\n";

        fragmentShaderSource << "\n"
                                "// Light structure\n"
                                "struct Light\n"
                                "{\n"
                                "    vec4 ambientColor;\n"
                                "    vec4 diffuseColor;\n"
                                "    vec4 specularColor;\n"
                                "    vec4 positionDirection;\n"
                                "    vec4 attenuation;\n"
                                "};\n"
                                "\n"
                                "// Uniforms\n"
                                "uniform mat4 sf_ModelMatrix;\n"
                                "uniform mat4 sf_NormalMatrix;\n"
                                "uniform sampler2D sf_Texture0;\n"
                                "uniform int sf_TextureEnabled;\n"
                                "uniform int sf_LightCount;\n"
                                "uniform int sf_LightingEnabled;\n"
                                "uniform vec3 sf_ViewerPosition;\n"
                                "\n";

        if (Shader::isUniformBufferAvailable())
            fragmentShaderSource << "layout (std140) uniform Lights\n"
                                    "{\n"
                                    "    Light sf_Lights["
                                 << Light::getMaximumLights()
                                 << "];\n"
                                    "};\n";
        else
            fragmentShaderSource << "uniform Light sf_Lights[" << Light::getMaximumLights() << "];\n";

        fragmentShaderSource << "\n"
                                "// Fragment attributes\n"
                                "in vec4 sf_FrontColor;\n"
                                "in vec2 sf_TexCoord0;\n"
                                "in vec3 sf_FragWorldPosition;\n"
                                "in vec3 sf_FragNormal;\n"
                                "\n"
                                "// Fragment shader outputs\n"
                                "out vec4 sf_FragColor;\n"
                                "\n"
                                "vec4 computeLighting()\n"
                                "{\n"
                                "    // Early return in case lighting disabled\n"
                                "    if (sf_LightingEnabled == 0)\n"
                                "        return vec4(1.0, 1.0, 1.0, 1.0);\n"
                                "\n"
                                "    // TODO: Implement way to manipulate materials\n"
                                "    const float materialShininess = 1.0;\n"
                                "    const vec4 materialSpecularColor = vec4(0.0001, 0.0001, 0.0001, 1.0);\n"
                                "\n"
                                "    vec3 fragmentNormal = normalize((sf_NormalMatrix * vec4(sf_FragNormal, 1.0)).xyz);\n"
                                "    vec3 fragmentDistanceToViewer = normalize(sf_ViewerPosition - sf_FragWorldPosition);"
                                "\n"
                                "    vec4 totalIntensity = vec4(0.0, 0.0, 0.0, 0.0);\n"
                                "\n"
                                "    for (int index = 0; index < sf_LightCount; ++index)\n"
                                "    {\n"
                                "        vec3 rayDirection = normalize(sf_Lights[index].positionDirection.xyz);\n"
                                "        float attenuationFactor = 1.0;"
                                "\n"
                                "        if (sf_Lights[index].positionDirection.w > 0.0)\n"
                                "        {\n"
                                "            rayDirection = normalize(sf_FragWorldPosition - sf_Lights[index].positionDirection.xyz);\n"
                                "            float rayLength = length(sf_Lights[index].positionDirection.xyz - sf_FragWorldPosition);"
                                "            vec4 attenuationCoefficients = vec4(1.0, rayLength, rayLength * rayLength, 0.0);"
                                "            attenuationFactor = dot(sf_Lights[index].attenuation, attenuationCoefficients);\n"
                                "        }\n"
                                "\n"
                                "        vec4 ambientIntensity = sf_Lights[index].ambientColor;\n"
                                "\n"
                                "        float diffuseCoefficient = max(0.0, dot(fragmentNormal, -rayDirection));\n"
                                "        vec4 diffuseIntensity = sf_Lights[index].diffuseColor * diffuseCoefficient;\n"
                                "\n"
                                "        float specularCoefficient = 0.0;\n"
                                "        if(diffuseCoefficient > 0.0)"
                                "            specularCoefficient = pow(max(0.0, dot(fragmentDistanceToViewer, reflect(rayDirection, "
                                "fragmentNormal))), materialShininess);"
                                "        vec4 specularIntensity = specularCoefficient * materialSpecularColor * "
                                "sf_Lights[index].specularColor;"
                                "\n"
                                "        totalIntensity += ambientIntensity + (diffuseIntensity + specularIntensity) / attenuationFactor;\n"
                                "    }\n"
                                "\n"
                                "    return vec4(totalIntensity.rgb, 1.0);\n"
                                "}\n"
                                "\n"
                                "vec4 computeTexture()\n"
                                "{\n"
                                "    if (sf_TextureEnabled == 0)\n"
                                "        return vec4(1.0, 1.0, 1.0, 1.0);\n"
                                "\n"
                                "    return texture2D(sf_Texture0, sf_TexCoord0);\n"
                                "}\n"
                                "\n"
                                "void main()\n"
                                "{\n"
                                "    // Fragment color\n"
                                "    //vec4 color = sf_FrontColor * computeTexture() * computeLighting();\n"
                                "    //if(color.a == 0)\n"
                                "    if(sf_FrontColor.a == 0)\n"
                                "      discard;\n"
                                "    sf_FragColor = sf_FrontColor;\n"
                                "}\n";

        if (!m_defaultShader->loadFromMemory(vertexShaderSource.str(), fragmentShaderSource.str()))
        {
            err() << "Compiling default shader failed. Falling back to legacy pipeline..." << std::endl;
            m_defaultShader.reset(nullptr);
        }
    }
}
////////////////////////////////////////////////////////////
void RenderTarget::setupDraw(bool useVertexCache, const RenderStates& states)
{
    // First set the persistent OpenGL states if it's the very first call
    if (!m_cache.glStatesSet)
        resetGLStates();

    // Track if we need to set uniforms again for current shader
    bool shaderChanged = false;

    if (m_defaultShader)
    {
        m_currentNonLegacyShader = states.shader ? states.shader : m_defaultShader.get();
        shaderChanged = (m_currentNonLegacyShader != m_lastNonLegacyShader);

        applyTransform(states.transform);
    }
    else
    {
        if (useVertexCache)
        {
            // Since vertices are transformed, we must use an identity transform to render them
            if (!m_cache.enable || !m_cache.useVertexCache)
                glCheck(glLoadIdentity());
        }
        else
        {
            applyTransform(states.transform);
        }
    }
    // Apply the view
    if (!m_cache.enable || m_cache.viewChanged || shaderChanged)
        applyCurrentView();

    // Apply the blend mode
    if (!m_cache.enable || (states.blendMode != m_cache.lastBlendMode))
        applyBlendMode(states.blendMode);

    // Apply the texture
    if (!m_cache.enable || (states.texture && states.texture->m_fboAttachment))
    {
        // If the texture is an FBO attachment, always rebind it
        // in order to inform the OpenGL driver that we want changes
        // made to it in other contexts to be visible here as well
        // This saves us from having to call glFlush() in
        // RenderTextureImplFBO which can be quite costly
        // See: https://www.khronos.org/opengl/wiki/Memory_Model
        applyTexture(states.texture);
    }
    else
    {
        Uint64 textureId = states.texture ? states.texture->m_cacheId : 0;
        if (textureId != m_cache.lastTextureId)
            applyTexture(states.texture);
    }

    // Apply the shader
    if (states.shader)
        applyShader(states.shader);
    else if (m_defaultShader)
        applyShader(m_defaultShader.get());
}

////////////////////////////////////////////////////////////
void RenderTarget::drawPrimitives(PrimitiveType type, std::size_t firstVertex, std::size_t vertexCount)
{
    // Find the OpenGL primitive type
    static const GLenum modes[] = {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS};
    GLenum mode = modes[type];

    // Draw the primitives
    static const size_t MAX_ELEMENTS_VERTICES = 655360;
    auto remainderCount = vertexCount;
    for (int i = 0; i < vertexCount; i += MAX_ELEMENTS_VERTICES)
    {
        auto drawCount = std::min(remainderCount, MAX_ELEMENTS_VERTICES);
        glCheck(glDrawArrays(mode, static_cast<GLint>(firstVertex + i), static_cast<GLsizei>(drawCount)));
        remainderCount -= drawCount;
    }

}

////////////////////////////////////////////////////////////
void RenderTarget::cleanupDraw(const RenderStates& states)
{
    // Unbind the shader, if any
    if (states.shader && !m_defaultShader)
        applyShader(nullptr);
    if (m_defaultShader)
    {
        m_lastNonLegacyShader = m_currentNonLegacyShader;
        m_currentNonLegacyShader = nullptr;
    }
    // If the texture we used to draw belonged to a RenderTexture, then forcibly unbind that texture.
    // This prevents a bug where some drivers do not clear RenderTextures properly.
    if (states.texture && states.texture->m_fboAttachment)
        applyTexture(NULL);

    // Re-enable the cache at the end of the draw if it was disabled
    m_cache.enable = true;
}

} // namespace sf

////////////////////////////////////////////////////////////
// Render states caching strategies
//
// * View
//   If SetView was called since last draw, the projection
//   matrix is updated. We don't need more, the view doesn't
//   change frequently.
//
// * Transform
//   The transform matrix is usually expensive because each
//   entity will most likely use a different transform. This can
//   lead, in worst case, to changing it every 4 vertices.
//   To avoid that, when the vertex count is low enough, we
//   pre-transform them and therefore use an identity transform
//   to render them.
//
// * Blending mode
//   Since it overloads the == operator, we can easily check
//   whether any of the 6 blending components changed and,
//   thus, whether we need to update the blend mode.
//
// * Texture
//   Storing the pointer or OpenGL ID of the last used texture
//   is not enough; if the sf::Texture instance is destroyed,
//   both the pointer and the OpenGL ID might be recycled in
//   a new texture instance. We need to use our own unique
//   identifier system to ensure consistent caching.
//
// * Shader
//   Shaders are very hard to optimize, because they have
//   parameters that can be hard (if not impossible) to track,
//   like matrices or textures. The only optimization that we
//   do is that we avoid setting a null shader if there was
//   already none for the previous draw.
//
////////////////////////////////////////////////////////////
