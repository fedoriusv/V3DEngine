#ifndef _V3D_SHADER_GL_H_
#define _V3D_SHADER_GL_H_

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
        ~CShaderGL();

        bool            create()                                            override;
        bool            create(const std::string& shader, EShaderType type) override;
        void            destroy()                                           override;

    private:

        static bool     initShader(u32& shader, const EShaderType type, void* body);

        static u32      createShader(EShaderType type);
        static void     deleteShader(u32 shader);
        static void     sourceShader(u32 shader, void* body);
        static void     compileShader(u32 shader);
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_GL_H_
