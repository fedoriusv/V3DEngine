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
        ~CShaderProgramGL();

        bool                create()    override;
        void                destroy()   override;
        void                bind()      override;
        void                unbind()    override;

        static void         createProgram(u32& shaderProgram);
        static void         deleteProgram(u32 shaderProgram);
        static bool         useProgram(u32 shaderProgram);

        static void         attachShader(u32 shaderProgram, u32 shader);
        static void         detachShader(u32 shaderProgram, u32 shader);

        static void         bindAttrib(u32 shaderProgram, CShaderAttribute::EShaderAttribute type, const std::string& name);
        static s32          getAttrib(u32 shaderProgram, const std::string& name);

    private:

        bool                init(const std::vector<u32>& shaders);

        bool                setUniform(EDataType type, const u32 shader, const std::string& attribute, void* value) override;
        void                setUniformInt    (const s32 location, const s32 value)              override;
        void                setUniformFloat  (const s32 location, const f32 value)              override;
        void                setUniformVector2(const s32 location, const core::Vector2D& vector) override;
        void                setUniformVector3(const s32 location, const core::Vector3D& vector) override;
        void                setUniformVector4(const s32 location, const core::Vector4D& vector) override;
        void                setUniformMatrix3(const s32 location, const core::Matrix3D& matrix) override;
        void                setUniformMatrix4(const s32 location, const core::Matrix4D& matrix) override;

        s32                 getUniformID(const u32 shader, const std::string& name)             override;

        static u32          s_currentShader;

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_PROGRAM_GL_H_
