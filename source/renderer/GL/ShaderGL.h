#ifndef _F3D_SHADER_GL_H_
#define _F3D_SHADER_GL_H_

#include "renderer/Shader.h"

namespace f3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShaderGL : public CShader
    {
    public:

        CShaderGL();
        virtual ~CShaderGL();

        void    create(const std::string& body, EShaderType type)    override;
        void    load(const std::string& file, EShaderType type)      override;
        void    destroy()                                            override;

    private:

        void    initShader(u32& shader, const EShaderType type, void* body);
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SHADER_GL_H_