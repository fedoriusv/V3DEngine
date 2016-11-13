#pragma once

#include "stream/Resource.h"

namespace v3d
{
namespace resources
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EShaderType : s32
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

    class ShaderData : public stream::IResource
    {

    public:

        enum EShaderDataType : u32
        {
            eBitecode,
            eBitecodeSpirV,
            eSourceGLSL,
            eSourceHLSL
        };

       /* struct SReflection
        {
            uniform_list;
            channel_input_list;
            channel_output_list;
            sample_list;

        };*/

        ShaderData();
        ~ShaderData();

        void                        init(const stream::IStreamPtr& stream) override;
        bool                        load() override;

        const std::string&  getName() const;
        const std::string&  getBody() const;

        static const std::string&   getShaderTypeNameByType(EShaderType type);
        static EShaderType          getShaderTypeByName(const std::string& name);

    private:

        std::string                 m_name;

        EShaderDataType             m_dataType;
        EShaderType                 m_type;
        //SReflection                 m_reflaction;

        union
        {
            std::vector<c8>*        m_source;
            std::vector<u32>*       m_bitecode;
        };

        u64                         m_hash;

        static const std::string    s_shaderTypeName[EShaderType::eShaderCount];
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace resources
} //namespace v3d
