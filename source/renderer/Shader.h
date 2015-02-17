#ifndef _V3D_SHADER_H_
#define _V3D_SHADER_H_

#include "common.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShader
    {
    public:

        enum EShaderType
        {
            eShaderUnknown = -1,
            eVertex,
            eFragment,
            eGeometry,
            eCompute,

            eShaderCount
        };

        CShader();
        virtual                     ~CShader();

        virtual bool                create(const std::string& body, EShaderType type) = 0;
        virtual bool                load  (const std::string& file, EShaderType type) = 0;
        virtual void                destroy()                                         = 0;

        u32                         getShaderID()       const;
        EShaderType                 getShaderType()     const;
        bool                        getCompileStatus()  const;

        static const std::string&   getShaderTypeNameByType(EShaderType type);
        static EShaderType          getShaderTypeByName(const std::string& name);

        const std::string           getName() const;
        void                        setName(const std::string& name);

    protected:

        char*                       read(const std::string& file);
        void                        clear();

        u32                         m_shaderID;

        EShaderType                 m_shaderType;
        bool                        m_compileStatus;
        void*                       m_data;
        std::string                 m_name;

    private:

        static const std::string    s_shaderTypeName[EShaderType::eShaderCount];

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShader>    ShaderPtr;
    typedef std::vector<ShaderPtr>      ShaderList;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_H_
