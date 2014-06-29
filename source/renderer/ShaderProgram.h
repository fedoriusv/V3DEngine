#ifndef _V3D_SHADER_PROGRAM_H_
#define _V3D_SHADER_PROGRAM_H_

#include "Object.h"
#include "Shader.h"
#include "ShaderData.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderPass;
    class CRenderer;

    class CDebugGeometry;
    class CDebugLight;

    class CShaderProgram : public CObject
    {
    public:

        CShaderProgram(const ShaderDataPtr& data);
        virtual         ~CShaderProgram();

        u32             getShaderID()   const;
        bool            isEnable()      const;
        void            setEnable(bool enable);

        virtual bool    create()    = 0;
        virtual void    destroy()   = 0;
        virtual void    bind()      = 0;
        virtual void    unbind()    = 0;

    protected:

        friend          CRenderPass;
        friend          CRenderer;

        friend          CDebugGeometry;
        friend          CDebugLight;

        void            addShader(ShaderPtr shader);
        void            destroyShader(ShaderPtr shader);

        void            getShaderIDArray(std::vector<u32>& shaders);

        virtual bool    create(const std::string& vertex, const std::string& fragment, u32 arg = 0, ...)                        = 0;

        virtual bool    setUniform(CShaderUniform::EDataType type, const u32 shader, const std::string& attribute, void* value) = 0;

        virtual s32     setUniformInt(const u32 shader, const std::string& name, const u32 value)                               = 0;
        virtual s32     setUniformFloat(const u32 shader, const std::string& name, const f32 value)                             = 0;
        virtual s32     setUniformVector2(const u32 shader, const std::string& name, core::Vector2D& vector)                    = 0;
        virtual s32     setUniformVector3(const u32 shader, const std::string& name, core::Vector3D& vector)                    = 0;
        virtual s32     setUniformVector4(const u32 shader, const std::string& name, core::Vector4D& vector)                    = 0;
        virtual s32     setUniformMatrix3(const u32 shader, const std::string& name, const core::Matrix3D& matrix)              = 0;
        virtual s32     setUniformMatrix4(const u32 shader, const std::string& name, const core::Matrix4D& matrix)              = 0;

        u32             m_shaderProgID;

        bool            m_enable;
        bool            m_isActive;

        ShaderDataPtr   m_shaderData;
        ShaderList      m_shaderList;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderProgram> ShaderProgramPtr;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_PROGRAM_H_
