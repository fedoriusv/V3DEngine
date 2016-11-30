#pragma once

#include "utils/Cloneable.h"
#include "utils/RefCounted.h"
#include "resources/Shader.h"
#include "ShaderUniform.h"
#include "ShaderAttribute.h"
#include "ConstantBuffer.h"
#include "Texture.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class RenderPass;
    class ShaderProgram;
    class Texture;
    class IRenderer;

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
          /*eLinked    = 1 << 4,
            eValidated = 1 << 5,*/ 
            eDeleted   = 1 << 6
        };

        struct ShaderParameters
        {
            struct Channel
            {
                std::string                         name;
                u32                                 location;

                u32                                 rowCount;
                u32                                 colCount;
                renderer::ShaderDataType::EDataType type;
            };

            struct TextureParameter
            {
                std::string                         name;
                u32                                 set;
                u32                                 binding;
                renderer::ETextureTarget            target;
                bool                                depth;
            };

            struct UniformParameter
            {
                std::string                         name;
                s32                                 block;
                u32                                 index;

                u32                                 rowCount;
                u32                                 colCount;
                u32                                 array;
                u32                                 size;
                u32                                 offset;
                renderer::ShaderDataType::EDataType type;
            };

            struct UniformBlockParameter
            {
                std::string                         name;
                s32                                 id;
                u32                                 set;
                u32                                 binding;
                u32                                 size;
            };

            std::unordered_map<std::string, Channel>            channelsIn;
            std::unordered_map<std::string, Channel>            channelsOut;
            std::unordered_map<std::string, TextureParameter>   samplers;
            std::unordered_map<std::string, UniformParameter>   uniforms;
            std::vector<UniformBlockParameter>                  constantBuffers;
        };

        struct ShaderData
        {
            UniformList     uniforms;
            UniformList     builtinUniforms;
            AttributeList   attributes;
            AttributeList   builtinAttributes;
        };

        ShaderProgram(const resources::ShaderList& shaders, const resources::ShaderDefinesList& defines = {});
        virtual                                     ~ShaderProgram();

        ShaderProgram(const ShaderProgram& program) = delete;
        ShaderProgram& operator=(const ShaderProgram& program) = delete;

        virtual bool                                    setDefine(const std::string& name, const std::string& value = "");
        virtual bool                                    setUndefine(const std::string& name);

        template<typename T>
        void                                            applyUniform(const std::string& name, const T& value);
        template<typename T>
        void                                            applyAttribute(const std::string& name, const T& data);
        virtual void                                    applyTexture(const std::string& name, const TexturePtr texure);

        virtual void                                    attachShader(const resources::ShaderPtr shader);
        virtual void                                    detachShader(const resources::ShaderPtr shader);

        bool                                            compile();

        virtual u16                                     getFlags() const;
        virtual bool                                    isFlagPresent(EProgramFlags flag);

        virtual ShaderProgramPtr                        clone() const override;

    protected:

        ShaderProgram();

        virtual void                                    applyUniform(const std::string& name, const void* value, u32 size);
        virtual void                                    applyAttribute(const std::string& name, const void* value, u32 size);

        virtual void                                    addUniform(ShaderUniform* uniform);
        virtual void                                    addAttribute(ShaderAttribute* attribute);

        virtual const resources::ShaderDefinesList&     getMacroDefinitions() const;
        virtual const resources::ShaderList&            getShaders() const;

        virtual void                                    setMacroDefinitions(const resources::ShaderDefinesList& list);
        virtual void                                    setShaderParams(ShaderParameters& params);

        //Render thread
        friend                                          RenderThread;
        virtual bool                                    compile(const resources::ShaderDefinesList& defines, const resources::ShaderList& shaders, ShaderProgram::ShaderParameters& outParameters);
        virtual void                                    destroy();

    private:

        friend                                          RenderPass;

        ShaderProgram* const                            m_impl;
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
