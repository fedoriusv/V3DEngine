#ifndef _F3D_SHADER_H_
#define _F3D_SHADER_H_

#include "Object.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EShaderType
    {
        eTypeUnknown = -1,
        eTypeVertex,
        eTypeFragment,
        eTypeGeometry,
        eTypeCompute,

        eShaderTypeCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShader : public CObject
    {
    public:
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

    protected:

        char*                       readShader(const std::string& file);
        void                        clearShader();

        u32                         m_shaderID;

        EShaderType                 m_shaderType;
        bool                        m_compileStatus;
        void*                       m_data;

    private:

        static const std::string    s_shaderTypeName[EShaderType::eShaderTypeCount];

    };

	/////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShader>				ShaderPtr;
    typedef std::vector<ShaderPtr>					ShaderList;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SHADER_H_