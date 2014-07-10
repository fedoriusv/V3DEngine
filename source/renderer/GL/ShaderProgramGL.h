#ifndef _V3D_SHADER_PROGRAM_GL_H_
#define _V3D_SHADER_PROGRAM_GL_H_

#include "renderer/ShaderProgram.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShaderProgramGL : public CShaderProgram
    {
    public:

        CShaderProgramGL(const ShaderDataPtr& data);
        virtual             ~CShaderProgramGL();

        bool                create()    override;
        void                destroy()   override;
        void                bind()      override;
        void                unbind()    override;

    private:

        bool                create(const std::string& vertex, const std::string& fragment, u32 arg, ...) override;

        bool                initProgram(u32& shaderProgram, const std::vector<u32>& shaders);

        void                attachShader(u32 shaderProgram, u32 shader);
        void                detachShader(u32 shaderProgram, u32 shader);
        void                bindAttrib(u32 shaderProgram, CShaderAttribute::EShaderAttribute type, const std::string& name);
        s32                 getAttrib(u32 shaderProgram, const std::string& name);
        void                createProgram(u32& shaderProgram);
        void                deleteProgram(u32 shaderProgram);
        void                useProgram(u32 shaderProgram);

        bool                setUniform(CShaderUniform::EDataType type, const u32 shader, const std::string& attribute, void* value) override;

        s32                 setUniformInt(const u32 shader, const std::string& name, const u32 value)                               override;
        s32                 setUniformFloat(const u32 shader, const std::string& name, const f32 value)                             override;
        s32                 setUniformVector2(const u32 shader, const std::string& name, const core::Vector2D& vector)              override;
        s32                 setUniformVector3(const u32 shader, const std::string& name, const core::Vector3D& vector)              override;
        s32                 setUniformVector4(const u32 shader, const std::string& name, const core::Vector4D& vector)              override;
        s32                 setUniformMatrix3(const u32 shader, const std::string& name, const core::Matrix3D& matrix)              override;
        s32                 setUniformMatrix4(const u32 shader, const std::string& name, const core::Matrix4D& matrix)              override;

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_PROGRAM_GL_H_
