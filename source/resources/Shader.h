#pragma once

#include "utils/RefCounted.h"
#include "utils/IntrusivePtr.h"
#include "stream/Resource.h"
#include "renderer/ShaderDataTypes.h"

namespace v3d
{

namespace scene
{
    class ShaderManager;
} //namespace scene

namespace renderer
{
    class RenderPass;
} //namespace renderer

namespace resources
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EShaderType : s16
    {
        eShaderUnknown = -1,
        eVertex,
        eFragment,
        eGeometry,
        eTessellationControl,
        eTessellationEvaluation,
        eCompute,

        eShaderCount
    };

    using ShaderDefinesList = std::map<const std::string, std::string>;
    using Bytecode = std::vector<u32>;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Shader resource class
    */
    class Shader : public utils::CRefCounted, public stream::IResource
    {
    public:

        enum EShaderDataRepresent : u16
        {
            eBytecode,
            eSource,
        };

        Shader();
        ~Shader();

        void                        init(const stream::IStreamPtr stream) override;
        bool                        load() override;
        stream::IStreamPtr          serialize();

        const std::string&          getName() const;

        const std::string*          getSource() const;
        const Bytecode*             getBytecode() const;

        EShaderDataRepresent        getShaderKind() const;
        EShaderType                 getType() const;

        static const std::string&   getShaderTypeNameByType(EShaderType type);
        static EShaderType          getShaderTypeByName(const std::string& name);

    private:

        Shader(EShaderType type, const std::string& source);
        Shader(EShaderType type, const Bytecode& bytecode);

        void                        setType(EShaderType type);

        friend                      scene::ShaderManager;
        friend                      renderer::RenderPass;

        std::string                 m_name;

        union
        {
            std::string*            m_source;
            Bytecode*               m_bytecode;
        };

        EShaderDataRepresent        m_dataRepresent;
        EShaderType                 m_shaderType;

        static const std::string    s_shaderTypeName[EShaderType::eShaderCount];
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    using ShaderPtr = utils::TIntrusivePtr<Shader>;
    using ShaderList = std::vector<ShaderPtr>;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace resources
} //namespace v3d
