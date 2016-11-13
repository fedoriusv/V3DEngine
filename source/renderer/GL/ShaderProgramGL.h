#ifndef _V3D_SHADER_PROGRAM_GL_H_
#define _V3D_SHADER_PROGRAM_GL_H_

#include "renderer/ShaderProgram.h"

#ifdef _OPENGL_RENDER_

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Inherited class for shader program management. GL render only.
    */
    class CShaderProgramGL final : public ShaderProgram
    {
    public:

        CShaderProgramGL();
        ~CShaderProgramGL();

        bool                create()    override;
        void                destroy()   override;
        void                bind()      override;
        void                unbind()    override;

        u32                 getShaderProgramID()   const;

        ShaderProgramPtr    clone() const override;

    private:

        CShaderProgramGL(const CShaderProgramGL& program);
        CShaderProgramGL&   operator=(const CShaderProgramGL& program);

        bool                createProgram(const std::vector<u32>& shaders);
        bool                linkProgram();
        bool                validateProgram();

        static bool         useProgram(u32 shaderProgram);
        static s32          getCurrentProgram();

        void                getShaderIDArray(std::vector<u32>& shaders);

        bool                applyUniform       (ShaderUniform* uniform)                    override;
        void                applyUniformInt    (s32 location, s32 value)                    override;
        void                applyUniformFloat  (s32 location, f32 value)                    override;
        void                applyUniformVector2(s32 location, const core::Vector2D& vector) override;
        void                applyUniformVector3(s32 location, const core::Vector3D& vector) override;
        void                applyUniformVector4(s32 location, const core::Vector4D& vector) override;
        void                applyUniformMatrix3(s32 location, const core::Matrix3D& matrix) override;
        void                applyUniformMatrix4(s32 location, const core::Matrix4D& matrix) override;

        u32                 m_id;

        static u32          s_currentShader;

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_OPENGL_RENDER_

#endif //_V3D_SHADER_PROGRAM_GL_H_
