#ifndef _V3D_SHADER_H_
#define _V3D_SHADER_H_

#include "common.h"

namespace tinyxml2
{
    class XMLElement;
}

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Base class for shader management.
    */
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

        virtual bool                create()                                            = 0;
        virtual bool                create(const std::string& shader, EShaderType type) = 0;
        virtual void                destroy()                                           = 0;

        bool                        parse(const tinyxml2::XMLElement* root);

        EShaderType                 getShaderType()     const;
        bool                        getCompileStatus()  const;

        static const std::string&   getShaderTypeNameByType(EShaderType type);
        static EShaderType          getShaderTypeByName(const std::string& name);

        const std::string&          getName() const;
        void                        setName(const std::string& name);

    protected:

        EShaderType                 m_type;
        std::string                 m_name;
        std::string                 m_data;

        bool                        m_compileStatus;

    private:

        static const std::string    s_shaderTypeName[eShaderCount];

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShader>            ShaderPtr;
    typedef std::vector<ShaderPtr>              ShaderList;
    typedef std::map<std::string, ShaderPtr>    ShaderMap;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_SHADER_H_
