#ifndef _V3D_SHADER_GL_H_
#define _V3D_SHADER_GL_H_

#include "renderer/Shader.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Inherited class for shader management. GL render only.
    */
    class CShaderGL : public CShader
    {
    public:

        CShaderGL();
        ~CShaderGL();

        bool            create()                                            override;
        bool            create(const std::string& shader, EShaderType type) override;
        void            destroy()                                           override;

    private:

        static bool     initShader(u32& shader, const EShaderType type, const std::string& name, const std::string& body);

        static u32      createShader(EShaderType type);
        static void     deleteShader(u32& shader);
        static void     sourceShader(u32 shader, const std::string& body);
        static void     compileShader(u32 shader);
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_SHADER_GL_H_
