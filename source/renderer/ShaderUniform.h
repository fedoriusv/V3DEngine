#ifndef _V3D_SHADER_UNIFORM_H_
#define _V3D_SHADER_UNIFORM_H_

#include "common.h"

namespace v3d
{
namespace renderer
{
    class CShaderUniform
    {
    public:

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EDataType
    {
        eTypeNone = 0,
        eTypeInt,
        eTypeFloat,
        eTypeVector2,
        eTypeVector3,
        eTypeVector4,
        eTypeMatrix3,
        eTypeMatrix4,

        eTypeCount,
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

        CShaderUniform();
        virtual                             ~CShaderUniform();

        void                                setUniform(EDataType type, const std::string& attribute, void* value);

        EDataType                           getUniformType() const;
        void*                               getUniforValue() const;

    private:

        EDataType                           m_uniformType;
        void*                               m_uniformValue;
        std::string                         m_attribute;

        void*                               allocMemory(EDataType type, void* value);
        void                                deallocMemory();

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderUniform>         UniformPtr;
    typedef std::map<std::string, UniformPtr>       UniformList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_UNIFORM_H_
