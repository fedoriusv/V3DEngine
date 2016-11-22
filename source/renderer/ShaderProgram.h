#pragma once

#include "utils/Cloneable.h"
#include "utils/RefCounted.h"
#include "Shader.h"
#include "Texture.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class RenderPass;
    class ShaderProgram;
    class Texture;

    using ShaderProgramPtr = utils::TIntrusivePtr<ShaderProgram>;

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Shader Program Interface
    * Client side. Provide transfer data to render thread
    */
    class ShaderProgram : public utils::CRefCounted, public utils::TCloneable<ShaderProgramPtr>
    {
    public:

        enum EProgramFlags : u16
        {
            eInvalid   = 1 << 1,
            eCreated   = 1 << 2,
            eCompiled  = 1 << 3,
/*            eLinked    = 1 << 4,
            eValidated = 1 << 5,*/ 
            eDeleted   = 1 << 6
        };

        struct ShaderParameters
        {
            //TODO:
        };

        ShaderProgram(const ShaderList& shaders, const ShaderDefinesList& defines = {});
        virtual                             ~ShaderProgram();

        ShaderProgram(const ShaderProgram& program) = delete;
        ShaderProgram& operator=(const ShaderProgram& program) = delete;

        virtual bool                        setDefine(const std::string& name, const std::string& value = "");
        virtual bool                        setUndefine(const std::string& name);

        template<typename T>
        void                                applyUniform(const std::string& name, const T& value);
        template<typename T>
        void                                applyAttribute(const std::string& name, const T& data);
        virtual void                        applyTexture(const std::string& name, const TexturePtr texure);

        virtual void                        attachShader(const ShaderPtr shader);
        virtual void                        detachShader(const ShaderPtr shader);

        bool                                compile();

        virtual u16                         getFlags() const;
        virtual bool                        isFlagPresent(EProgramFlags flag);

        virtual ShaderProgramPtr            clone() const override;

    protected:

        ShaderProgram();

        virtual void                        applyUniform(const std::string& name, const void* value, u32 size);
        virtual void                        applyAttribute(const std::string& name, const void* value, u32 size);

        virtual const ShaderDefinesList&    getMacroDefinitions() const;
        virtual const ShaderList&           getShaders() const;

        virtual void                        setMacroDefinitions(const ShaderDefinesList& list);
        virtual void                        setShaderParams(ShaderParameters& params);

        friend                              RenderThread;
        virtual bool                        compile(const ShaderDefinesList& defines, const ShaderList& shaders, ShaderParameters& outParameters);
        virtual void                        destroy();

    private:

        friend                              RenderPass;


        ShaderProgram* const                m_impl;
};

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    inline void ShaderProgram::applyUniform(const std::string& name, const T& value)
    {
        ShaderProgram::applyUniform(name, &value, sizeof(T));
    }

    template <typename T>
    inline void ShaderProgram::applyAttribute(const std::string& name, const T& data)
    {
        ShaderProgram::applyAttribute(name, &value, sizeof(T));
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d
