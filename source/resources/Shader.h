#pragma once

#include "utils/RefCounted.h"
#include "utils/IntrusivePtr.h"
#include "stream/Resource.h"
#include "renderer/ShaderDataTypes.h"

namespace v3d
{
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

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Shader resource class
    */
    class Shader : public stream::IResource, public utils::CRefCounted
    {
    public:

        enum EShaderDataRepresent : u16
        {
            eBytecode,
            eBytecodeSpirV,
            eSourceGLSL,
            eSourceHLSL
        };

        Shader();
        ~Shader();

        void                        init(const stream::IStreamPtr stream) override;
        bool                        load() override;

        const std::string&          getName() const;

        const std::string&          getSource() const;
        const std::vector<u32>&     getBytecode() const;

        EShaderDataRepresent        getShaderKind() const;

        static const std::string&   getShaderTypeNameByType(EShaderType type);
        static EShaderType          getShaderTypeByName(const std::string& name);

    private:

        std::string                 m_name;

        union
        {
            std::string*            m_source;
            std::vector<u32>*       m_bytecode;
        };

        EShaderDataRepresent        m_dataRepresent;
        EShaderType                 m_shaderType;

        static const std::string    s_shaderTypeName[EShaderType::eShaderCount];
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    using ShaderPtr = std::

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace resources
} //namespace v3d
