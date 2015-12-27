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

        CShaderProgramGL();
        ~CShaderProgramGL();

        bool                create()    override;
        void                destroy()   override;
        void                bind()      override;
        void                unbind()    override;

    public:

        static void         createProgram(u32& shaderProgram);
        static void         deleteProgram(u32 shaderProgram);
        static bool         useProgram(u32 shaderProgram);
        static bool         linkProgram(u32 shaderProgram);
        static bool         validateProgram(u32 shaderProgram);

        static void         attachShader(u32 shaderProgram, u32 shader);
        static void         detachShader(u32 shaderProgram, u32 shader);

        static void         bindAttrib(u32 shaderProgram, u32 type, const std::string& name);
        static s32          getAttribLocation(u32 shaderProgram, const std::string& name);

        static s32          getUniformLocation(u32 shaderProgram, const std::string& name);

    private:

        bool                init(const std::vector<u32>& shaders);

        bool                setUniform(const UniformPtr& uniform) override;

        void                setUniformInt    (s32 location, s32 value)                    override;
        void                setUniformFloat  (s32 location, f32 value)                    override;
        void                setUniformVector2(s32 location, const core::Vector2D& vector) override;
        void                setUniformVector3(s32 location, const core::Vector3D& vector) override;
        void                setUniformVector4(s32 location, const core::Vector4D& vector) override;
        void                setUniformMatrix3(s32 location, const core::Matrix3D& matrix) override;
        void                setUniformMatrix4(s32 location, const core::Matrix4D& matrix) override;

        static u32          s_currentShader;

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_PROGRAM_GL_H_
