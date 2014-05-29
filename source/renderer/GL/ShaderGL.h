#ifndef _F3D_SHADER_GL_H_
#define _F3D_SHADER_GL_H_

#include "renderer/Shader.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShaderGL : public CShader
    {
    public:

        CShaderGL();
        virtual ~CShaderGL();

        bool    create(const std::string& body, EShaderType type)    override;
        bool    load  (const std::string& file, EShaderType type)    override;
        void    destroy()                                            override;

    private:

        bool    initShader(u32& shader, const EShaderType type, void* body);
        u32     createShader(const EShaderType type);
        void    deleteShader(u32 shader);
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SHADER_GL_H_