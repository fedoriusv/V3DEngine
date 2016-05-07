#ifndef _V3D_SHADER_GL_H_
#define _V3D_SHADER_GL_H_

#include "renderer/Shader.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Inherited class for unit shader. GL render only.
    */
    class CShaderGL : public IShader
    {
    public:

        CShaderGL();
        ~CShaderGL();

        void            destroy() override;

        u32             getShaderID() const;

    private:

        bool            create(CShaderSource&& data) override;

        u32             createShader(EShaderType type);
        bool            compileShader(u32 shader, const std::string& body, EShaderType type);

        u32             m_id;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_SHADER_GL_H_
