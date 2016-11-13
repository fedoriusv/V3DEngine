#ifndef _V3D_SHADER_SOURCE_H_
#define _V3D_SHADER_SOURCE_H_

#include "stream/Resource.h"

namespace v3d
{

namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class IShader;
    class ShaderProgram;

    using ShaderDefinesList = std::map<const std::string, std::string>;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EShaderType
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
    * ShaderSource class
    */
    class CShaderSource
    {

    public:

        CShaderSource();
        ~CShaderSource();

        CShaderSource(CShaderSource&&) = default;
        CShaderSource&                  operator=(CShaderSource&&) = default;


        CShaderSource(const CShaderSource&) = delete;
        CShaderSource&                  operator=(const CShaderSource&) = delete;

        const std::string&              getHeader() const;
        const std::string&              getBody() const;

        const std::string&              getName() const;
        EShaderType                     getType() const;

        std::size_t                     getHash() const;

        void                            setDefines(const ShaderDefinesList& defines = {});

        static const std::string&       getShaderTypeNameByType(EShaderType type);
        static EShaderType              getShaderTypeByName(const std::string& name);

    private:

        friend                          renderer::ShaderProgram;
        friend                          renderer::IShader;

        static std::string              buildHeader(const ShaderDefinesList& list, const std::string& body);
        static std::size_t              calculateHash(const std::string& header, const std::string& body);

        void                            setType(EShaderType type);
        void                            setHeader(const std::string& header);
        void                            setBody(const std::string& body);
        void                            setName(const std::string& name);

        EShaderType                     m_type;

        std::string                     m_header;
        std::string                     m_body;

        std::size_t                     m_hash;
        std::string                     m_name;

        static const std::string        s_shaderTypeName[eShaderCount];
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_SHADER_SOURCE_H_
