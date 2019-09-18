
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Light.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Err.hpp>
#include <cmath>
#include <sstream>
#include <vector>


namespace
{
    // Static light data and its mutex
    sf::Mutex mutex;
    unsigned int count = 0;
    std::vector<bool> usedIds;
    std::set<const sf::Light*> enabledLights;
    bool lightingEnabled = false;
    sf::VertexBuffer* lightUniformBuffer = NULL;
    bool lightUniformBufferNeedUpload = true;
}


namespace sf
{

    ////////////////////////////////////////////////////////////
    Light::Light() :
        m_light(-1),
        m_position(0, 0, 0),
        m_directional(false),
        m_color(Color::White),
        m_ambientIntensity(0.f),
        m_diffuseIntensity(1.f),
        m_specularIntensity(1.f),
        m_constantAttenuation(1.f),
        m_linearAttenuation(0.f),
        m_quadraticAttenuation(0.f),
        m_enabled(false)
    {
        getId();

        if (m_light < 0)
            return;

        setNeedUniformUpload();

        if (hasShaderLighting())
            return;

        GLfloat position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glCheck(glLightfv(GL_LIGHT0 + m_light, GL_POSITION, position));

        GLfloat ambientColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glCheck(glLightfv(GL_LIGHT0 + m_light, GL_AMBIENT, ambientColor));

        GLfloat diffuseColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glCheck(glLightfv(GL_LIGHT0 + m_light, GL_DIFFUSE, diffuseColor));

        GLfloat specularColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glCheck(glLightfv(GL_LIGHT0 + m_light, GL_SPECULAR, specularColor));

        glCheck(glLightf(GL_LIGHT0 + m_light, GL_CONSTANT_ATTENUATION, 1.0f));
        glCheck(glLightf(GL_LIGHT0 + m_light, GL_LINEAR_ATTENUATION, 0.0f));
        glCheck(glLightf(GL_LIGHT0 + m_light, GL_QUADRATIC_ATTENUATION, 0.0f));

        glCheck(glDisable(GL_LIGHT0 + m_light));
    }


    ////////////////////////////////////////////////////////////
    Light::Light(const Light& copy) :
        m_light(-1),
        m_position(copy.m_position),
        m_directional(copy.m_directional),
        m_color(copy.m_color),
        m_ambientIntensity(copy.m_ambientIntensity),
        m_diffuseIntensity(copy.m_diffuseIntensity),
        m_specularIntensity(copy.m_specularIntensity),
        m_constantAttenuation(copy.m_constantAttenuation),
        m_linearAttenuation(copy.m_linearAttenuation),
        m_quadraticAttenuation(copy.m_quadraticAttenuation),
        m_enabled(false)
    {
        getId();

        if (m_light < 0)
            return;

        setNeedUniformUpload();

        // If this is a directional light source, normalize the direction vector
        if (m_directional)
        {
            float norm = std::sqrt(m_position.x * m_position.x + m_position.y * m_position.y + m_position.z * m_position.z);

            m_position /= norm;
        }

        if (hasShaderLighting())
            return;

        GLfloat position[] = { m_position.x, m_position.y, m_position.z, m_directional ? 0.0f : 1.0f };
        glCheck(glLightfv(GL_LIGHT0 + m_light, GL_POSITION, position));

        GLfloat ambientColor[] = { static_cast<float>(m_color.r) / 255.f * m_ambientIntensity,
                                  static_cast<float>(m_color.g) / 255.f * m_ambientIntensity,
                                  static_cast<float>(m_color.b) / 255.f * m_ambientIntensity,
                                  static_cast<float>(m_color.a) / 255.f * m_ambientIntensity };
        glCheck(glLightfv(GL_LIGHT0 + m_light, GL_AMBIENT, ambientColor));

        GLfloat diffuseColor[] = { static_cast<float>(m_color.r) / 255.f * m_diffuseIntensity,
                                  static_cast<float>(m_color.g) / 255.f * m_diffuseIntensity,
                                  static_cast<float>(m_color.b) / 255.f * m_diffuseIntensity,
                                  static_cast<float>(m_color.a) / 255.f * m_diffuseIntensity };
        glCheck(glLightfv(GL_LIGHT0 + m_light, GL_DIFFUSE, diffuseColor));

        GLfloat specularColor[] = { static_cast<float>(m_color.r) / 255.f * m_specularIntensity,
                                   static_cast<float>(m_color.g) / 255.f * m_specularIntensity,
                                   static_cast<float>(m_color.b) / 255.f * m_specularIntensity,
                                   static_cast<float>(m_color.a) / 255.f * m_specularIntensity };
        glCheck(glLightfv(GL_LIGHT0 + m_light, GL_SPECULAR, specularColor));

        glCheck(glLightf(GL_LIGHT0 + m_light, GL_CONSTANT_ATTENUATION, m_constantAttenuation));
        glCheck(glLightf(GL_LIGHT0 + m_light, GL_LINEAR_ATTENUATION, m_linearAttenuation));
        glCheck(glLightf(GL_LIGHT0 + m_light, GL_QUADRATIC_ATTENUATION, m_quadraticAttenuation));

        glCheck(glDisable(GL_LIGHT0 + m_light));
    }


    ////////////////////////////////////////////////////////////
    Light::~Light()
    {
        disable();

        if (m_light >= 0)
        {
            Lock lock(mutex);
            usedIds[m_light] = false;
        }
    }


    ////////////////////////////////////////////////////////////
    void Light::setDirectional(bool directional)
    {
        m_directional = directional;

        if (m_light < 0)
            return;

        setNeedUniformUpload();

        // If this becomes a directional light source, normalize the direction vector
        if (m_directional)
        {
            float norm = std::sqrt(m_position.x * m_position.x + m_position.y * m_position.y + m_position.z * m_position.z);

            m_position /= norm;
        }

        if (hasShaderLighting())
            return;

        GLfloat position[] = { m_position.x, m_position.y, m_position.z, m_directional ? 0.0f : 1.0f };
        glCheck(glLightfv(GL_LIGHT0 + m_light, GL_POSITION, position));
    }


    ////////////////////////////////////////////////////////////
    void Light::setPosition(float x, float y, float z)
    {
        setPosition(Vector3f(x, y, z));
    }


    ////////////////////////////////////////////////////////////
    void Light::setPosition(const Vector3f& position)
    {
        m_position = position;

        if (m_light < 0)
            return;

        setNeedUniformUpload();

        // If this is a directional light source, normalize the direction vector
        if (m_directional)
        {
            float norm = std::sqrt(m_position.x * m_position.x + m_position.y * m_position.y + m_position.z * m_position.z);

            m_position /= norm;
        }

        if (hasShaderLighting())
            return;

        GLfloat glPosition[] = { m_position.x, m_position.y, m_position.z, m_directional ? 0.0f : 1.0f };
        glCheck(glLightfv(GL_LIGHT0 + m_light, GL_POSITION, glPosition));
    }


    ////////////////////////////////////////////////////////////
    const Vector3f& Light::getPosition() const
    {
        return m_position;
    }


    ////////////////////////////////////////////////////////////
    void Light::setDirection(float x, float y, float z)
    {
        setPosition(Vector3f(x, y, z));
    }


    ////////////////////////////////////////////////////////////
    void Light::setDirection(const Vector3f& position)
    {
        setPosition(position);
    }


    ////////////////////////////////////////////////////////////
    const Vector3f& Light::getDirection() const
    {
        return m_position;
    }


    ////////////////////////////////////////////////////////////
    void Light::setColor(const Color& color)
    {
        m_color = color;

        if (m_light < 0)
            return;

        setNeedUniformUpload();

        if (hasShaderLighting())
            return;

        GLfloat ambientColor[] = { static_cast<float>(m_color.r) / 255.f * m_ambientIntensity,
                                  static_cast<float>(m_color.g) / 255.f * m_ambientIntensity,
                                  static_cast<float>(m_color.b) / 255.f * m_ambientIntensity,
                                  static_cast<float>(m_color.a) / 255.f * m_ambientIntensity };
        glCheck(glLightfv(GL_LIGHT0 + m_light, GL_AMBIENT, ambientColor));

        GLfloat diffuseColor[] = { static_cast<float>(m_color.r) / 255.f * m_diffuseIntensity,
                                  static_cast<float>(m_color.g) / 255.f * m_diffuseIntensity,
                                  static_cast<float>(m_color.b) / 255.f * m_diffuseIntensity,
                                  static_cast<float>(m_color.a) / 255.f * m_diffuseIntensity };
        glCheck(glLightfv(GL_LIGHT0 + m_light, GL_DIFFUSE, diffuseColor));

        GLfloat specularColor[] = { static_cast<float>(m_color.r) / 255.f * m_specularIntensity,
                                   static_cast<float>(m_color.g) / 255.f * m_specularIntensity,
                                   static_cast<float>(m_color.b) / 255.f * m_specularIntensity,
                                   static_cast<float>(m_color.a) / 255.f * m_specularIntensity };
        glCheck(glLightfv(GL_LIGHT0 + m_light, GL_SPECULAR, specularColor));
    }


    ////////////////////////////////////////////////////////////
    const Color& Light::getColor() const
    {
        return m_color;
    }


    ////////////////////////////////////////////////////////////
    void Light::setAmbientIntensity(float intensity)
    {
        m_ambientIntensity = intensity;

        if (m_light < 0)
            return;

        setNeedUniformUpload();

        if (hasShaderLighting())
            return;

        GLfloat ambientColor[] = { static_cast<float>(m_color.r) / 255.f * m_ambientIntensity,
                                  static_cast<float>(m_color.g) / 255.f * m_ambientIntensity,
                                  static_cast<float>(m_color.b) / 255.f * m_ambientIntensity,
                                  static_cast<float>(m_color.a) / 255.f * m_ambientIntensity };
        glCheck(glLightfv(GL_LIGHT0 + m_light, GL_AMBIENT, ambientColor));
    }


    ////////////////////////////////////////////////////////////
    float Light::getAmbientIntensity() const
    {
        return m_ambientIntensity;
    }


    ////////////////////////////////////////////////////////////
    void Light::setDiffuseIntensity(float intensity)
    {
        m_diffuseIntensity = intensity;

        if (m_light < 0)
            return;

        setNeedUniformUpload();

        if (hasShaderLighting())
            return;

        GLfloat diffuseColor[] = { static_cast<float>(m_color.r) / 255.f * m_diffuseIntensity,
                                  static_cast<float>(m_color.g) / 255.f * m_diffuseIntensity,
                                  static_cast<float>(m_color.b) / 255.f * m_diffuseIntensity,
                                  static_cast<float>(m_color.a) / 255.f * m_diffuseIntensity };
        glCheck(glLightfv(GL_LIGHT0 + m_light, GL_DIFFUSE, diffuseColor));
    }


    ////////////////////////////////////////////////////////////
    float Light::getDiffuseIntensity() const
    {
        return m_diffuseIntensity;
    }


    ////////////////////////////////////////////////////////////
    void Light::setSpecularIntensity(float intensity)
    {
        m_specularIntensity = intensity;

        if (m_light < 0)
            return;

        setNeedUniformUpload();

        if (hasShaderLighting())
            return;

        GLfloat specularColor[] = { static_cast<float>(m_color.r) / 255.f * m_specularIntensity,
                                   static_cast<float>(m_color.g) / 255.f * m_specularIntensity,
                                   static_cast<float>(m_color.b) / 255.f * m_specularIntensity,
                                   static_cast<float>(m_color.a) / 255.f * m_specularIntensity };
        glCheck(glLightfv(GL_LIGHT0 + m_light, GL_SPECULAR, specularColor));
    }


    ////////////////////////////////////////////////////////////
    float Light::getSpecularIntensity() const
    {
        return m_specularIntensity;
    }


    ////////////////////////////////////////////////////////////
    void Light::setConstantAttenuation(float attenuation)
    {
        m_constantAttenuation = attenuation;

        if (m_light < 0)
            return;

        setNeedUniformUpload();

        if (hasShaderLighting())
            return;

        glCheck(glLightf(GL_LIGHT0 + m_light, GL_CONSTANT_ATTENUATION, m_constantAttenuation));
    }


    ////////////////////////////////////////////////////////////
    float Light::getConstantAttenuation() const
    {
        return m_constantAttenuation;
    }


    ////////////////////////////////////////////////////////////
    void Light::setLinearAttenuation(float attenuation)
    {
        m_linearAttenuation = attenuation;

        if (m_light < 0)
            return;

        setNeedUniformUpload();

        if (hasShaderLighting())
            return;

        glCheck(glLightf(GL_LIGHT0 + m_light, GL_LINEAR_ATTENUATION, m_linearAttenuation));
    }


    ////////////////////////////////////////////////////////////
    float Light::getLinearAttenuation() const
    {
        return m_linearAttenuation;
    }


    ////////////////////////////////////////////////////////////
    void Light::setQuadraticAttenuation(float attenuation)
    {
        m_quadraticAttenuation = attenuation;

        if (m_light < 0)
            return;

        setNeedUniformUpload();

        if (hasShaderLighting())
            return;

        glCheck(glLightf(GL_LIGHT0 + m_light, GL_QUADRATIC_ATTENUATION, m_quadraticAttenuation));
    }


    ////////////////////////////////////////////////////////////
    float Light::getQuadraticAttenuation() const
    {
        return m_quadraticAttenuation;
    }


    ////////////////////////////////////////////////////////////
    void Light::move(float offsetX, float offsetY, float offsetZ)
    {
        move(Vector3f(offsetX, offsetY, offsetZ));
    }


    ////////////////////////////////////////////////////////////
    void Light::move(const Vector3f& offset)
    {
        setPosition(m_position + offset);
    }


    ////////////////////////////////////////////////////////////
    bool Light::isDirectional() const
    {
        return m_directional;
    }


    ////////////////////////////////////////////////////////////
    void Light::enable()
    {
        if (m_light < 0)
            return;

        setNeedUniformUpload();

        Lock lock(mutex);
        enabledLights.insert(this);

        if (hasShaderLighting())
            return;

        glCheck(glEnable(GL_LIGHT0 + m_light));
    }


    ////////////////////////////////////////////////////////////
    void Light::disable()
    {
        if (m_light < 0)
            return;

        setNeedUniformUpload();

        Lock lock(mutex);
        enabledLights.erase(this);

        if (hasShaderLighting())
            return;

        glCheck(glDisable(GL_LIGHT0 + m_light));
    }


    ////////////////////////////////////////////////////////////
    size_t Light::getMaximumLights()
    {
        //sf::priv::ensureExtensionsInit();

        if (hasShaderLighting())
        {
            // hasShaderLighting() guarantees this will be a sane value
            return (Shader::getMaximumUniformComponents() - 256) / 128;
        }

        GLint maxLights = 0;
        glCheck(glGetIntegerv(GL_MAX_LIGHTS, &maxLights));

        return static_cast<unsigned int>(maxLights);
    }


    ////////////////////////////////////////////////////////////
    void Light::enableLighting()
    {
        //sf::priv::ensureExtensionsInit();

        Lock lock(mutex);
        lightingEnabled = true;

        if (hasShaderLighting())
            return;

        glCheck(glEnable(GL_LIGHTING));
    }


    ////////////////////////////////////////////////////////////
    void Light::disableLighting()
    {
        //sf::priv::ensureExtensionsInit();
        
        Lock lock(mutex);
        lightingEnabled = false;

        if (hasShaderLighting())
            return;

        glCheck(glDisable(GL_LIGHTING));
    }


    ////////////////////////////////////////////////////////////
    bool Light::isLightingEnabled()
    {
        Lock lock(mutex);
        return lightingEnabled;
    }


    ////////////////////////////////////////////////////////////
    bool Light::hasShaderLighting()
    {
        Lock lock(mutex);

        static bool checked = false;
        static bool shaderLightingSupported = false;
        if (!checked)
        {
            checked = true;

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
                auto maxUniformComponents = Shader::getMaximumUniformComponents();

                GLint maxLegacyLights = 0;
                glCheck(glGetIntegerv(GL_MAX_LIGHTS, &maxLegacyLights));

                unsigned int requiredUniformComponents = maxLegacyLights * 128 + 256;

                if (maxUniformComponents >= requiredUniformComponents)
                    shaderLightingSupported = true;
            }
        }

        return shaderLightingSupported;
    }


    ////////////////////////////////////////////////////////////
    void Light::increaseLightReferences()
    {
        //sf::priv::ensureExtensionsInit();

        Lock lock(mutex);

        if (!count && Shader::isUniformBufferAvailable())
        {
            lightUniformBuffer = new VertexBuffer;
            setNeedUniformUpload();
        }

        count++;
    }


    ////////////////////////////////////////////////////////////
    void Light::decreaseLightReferences()
    {
        //sf::priv::ensureExtensionsInit();

        Lock lock(mutex);
        count--;

        if (!count)
        {
            delete lightUniformBuffer;
            lightUniformBuffer = NULL;
        }
    }


    ////////////////////////////////////////////////////////////
    Light& Light::operator =(const Light& right)
    {
        Light temp(right);

        std::swap(m_light, temp.m_light);
        std::swap(m_position, temp.m_position);
        std::swap(m_directional, temp.m_directional);
        std::swap(m_color, temp.m_color);
        std::swap(m_ambientIntensity, temp.m_ambientIntensity);
        std::swap(m_diffuseIntensity, temp.m_diffuseIntensity);
        std::swap(m_specularIntensity, temp.m_specularIntensity);
        std::swap(m_constantAttenuation, temp.m_constantAttenuation);
        std::swap(m_linearAttenuation, temp.m_linearAttenuation);
        std::swap(m_quadraticAttenuation, temp.m_quadraticAttenuation);
        std::swap(m_enabled, temp.m_enabled);

        return *this;
    }


    ////////////////////////////////////////////////////////////
    void Light::getId()
    {
        Lock lock(mutex);

        if (usedIds.empty())
            usedIds.resize(getMaximumLights(), false);

        for (std::size_t i = 0; i < usedIds.size(); ++i)
        {
            if (!usedIds[i])
            {
                m_light = static_cast<int>(i);
                usedIds[i] = true;
                return;
            }
        }

#ifdef SFML_DEBUG
        // Inform the user that they created too many lights
        // for the fixed function pipeline to handle
        err() << "Not enough OpenGL lights to support creating "
            << "more sf::Light objects."
            << std::endl;
#endif
    }


    ////////////////////////////////////////////////////////////
    void Light::addLightsToShader(Shader& shader)
    {
        if (!lightingEnabled)
        {
            shader.setUniform("sf_LightingEnabled", 0);
            return;
        }

        shader.setUniform("sf_LightingEnabled", 1);

        if (!Shader::isUniformBufferAvailable())
        {
            for (std::set<const sf::Light*>::const_iterator i = enabledLights.begin(); i != enabledLights.end(); ++i)
            {
                const Light& light = *(*i);

                if (light.m_shaderElement.empty())
                {
                    std::ostringstream shaderElement;
                    shaderElement << "sf_Lights[" << light.m_light << "]";
                    light.m_shaderElement = shaderElement.str();
                }

                shader.setUniform(light.m_shaderElement + ".ambientColor", Glsl::Vec4(light.m_color.r * light.m_ambientIntensity / 255.f,
                    light.m_color.g * light.m_ambientIntensity / 255.f,
                    light.m_color.b * light.m_ambientIntensity / 255.f,
                    light.m_color.a * light.m_ambientIntensity / 255.f));
                shader.setUniform(light.m_shaderElement + ".diffuseColor", Glsl::Vec4(light.m_color.r * light.m_diffuseIntensity / 255.f,
                    light.m_color.g * light.m_diffuseIntensity / 255.f,
                    light.m_color.b * light.m_diffuseIntensity / 255.f,
                    light.m_color.a * light.m_diffuseIntensity / 255.f));
                shader.setUniform(light.m_shaderElement + ".specularColor", Glsl::Vec4(light.m_color.r * light.m_specularIntensity / 255.f,
                    light.m_color.g * light.m_specularIntensity / 255.f,
                    light.m_color.b * light.m_specularIntensity / 255.f,
                    light.m_color.a * light.m_specularIntensity / 255.f));
                shader.setUniform(light.m_shaderElement + ".positionDirection", Glsl::Vec4(light.m_position.x,
                    light.m_position.y,
                    light.m_position.z,
                    light.m_directional ? 0.f : 1.f));
                shader.setUniform(light.m_shaderElement + ".attenuation", Glsl::Vec4(light.m_constantAttenuation,
                    light.m_linearAttenuation,
                    light.m_quadraticAttenuation,
                    1.f));
            }
        }
#if 0
        else if (lightUniformBuffer)
        {
            {
                Lock lock(mutex);

                if (lightUniformBufferNeedUpload)
                {
                    lightUniformBufferNeedUpload = false;

                    std::size_t bytesNeeded = enabledLights.size() * 20 * sizeof(float);

                    // Make sure we have enough space for our lighting data
                    lightUniformBuffer->resize((bytesNeeded / sizeof(Vertex)) + 1);
                    float* dataPointer = static_cast<float*>(lightUniformBuffer->getPointer());
                    std::size_t index = 0;

                    for (std::set<const sf::Light*>::const_iterator i = enabledLights.begin(); i != enabledLights.end(); ++i, ++index)
                    {
                        const Light& light = *(*i);

                        dataPointer[index * 20 + 0] = light.m_color.r * light.m_ambientIntensity / 255.f;
                        dataPointer[index * 20 + 1] = light.m_color.g * light.m_ambientIntensity / 255.f;
                        dataPointer[index * 20 + 2] = light.m_color.b * light.m_ambientIntensity / 255.f;
                        dataPointer[index * 20 + 3] = light.m_color.a * light.m_ambientIntensity / 255.f;
                        dataPointer[index * 20 + 4] = light.m_color.r * light.m_diffuseIntensity / 255.f;
                        dataPointer[index * 20 + 5] = light.m_color.g * light.m_diffuseIntensity / 255.f;
                        dataPointer[index * 20 + 6] = light.m_color.b * light.m_diffuseIntensity / 255.f;
                        dataPointer[index * 20 + 7] = light.m_color.a * light.m_diffuseIntensity / 255.f;
                        dataPointer[index * 20 + 8] = light.m_color.r * light.m_specularIntensity / 255.f;
                        dataPointer[index * 20 + 9] = light.m_color.g * light.m_specularIntensity / 255.f;
                        dataPointer[index * 20 + 10] = light.m_color.b * light.m_specularIntensity / 255.f;
                        dataPointer[index * 20 + 11] = light.m_color.a * light.m_specularIntensity / 255.f;
                        dataPointer[index * 20 + 12] = light.m_position.x;
                        dataPointer[index * 20 + 13] = light.m_position.y;
                        dataPointer[index * 20 + 14] = light.m_position.z;
                        dataPointer[index * 20 + 15] = light.m_directional ? 0.f : 1.f;
                        dataPointer[index * 20 + 16] = light.m_constantAttenuation;
                        dataPointer[index * 20 + 17] = light.m_linearAttenuation;
                        dataPointer[index * 20 + 18] = light.m_quadraticAttenuation;
                        dataPointer[index * 20 + 19] = 1.f;
                    }
                }
            }

            shader.setBlock("Lights", *lightUniformBuffer);
        }
#endif
        shader.setUniform("sf_LightCount", static_cast<int>(enabledLights.size()));
    }


    ////////////////////////////////////////////////////////////
    void Light::setNeedUniformUpload()
    {
        if (!Shader::isUniformBufferAvailable())
            return;

        Lock lock(mutex);

        lightUniformBufferNeedUpload = true;
    }

} // namespace sf
