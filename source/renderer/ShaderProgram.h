#ifndef _V3D_SHADER_PROGRAM_H_
#define _V3D_SHADER_PROGRAM_H_

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

    /**
    * Shader Program Interface.
    */
    class CShaderProgram
    {
    public:

        CShaderProgram();
        virtual                     ~CShaderProgram();

        bool                        isEnable()      const;
        void                        setEnable(bool enable);

        bool                        getCompileStatus() const;

        virtual bool                create()    = 0;
        virtual void                destroy()   = 0;
        virtual void                bind()      = 0;
        virtual void                unbind()    = 0;

    protected:

        friend                      CRenderPass;
        friend                      CRenderer;

        void                        attachShader(const ShaderPtr& shader);
        void                        detachShader(const ShaderPtr& shader);

        void                        addShaderData(const ShaderDataPtr& data);
        void                        addVaryingsAttibutes(const std::vector<const c8*>& list);

        void                        getShaderIDArray(std::vector<u32>& shaders);
        bool                        create(const std::string& vertex, const std::string& fragment, u32 arg = 0, ...);

        virtual bool                applyUniform       (CShaderUniform* uniform)                    = 0;
        virtual void                applyUniformInt    (s32 location, s32 value)                    = 0;
        virtual void                applyUniformFloat  (s32 location, f32 value)                    = 0;
        virtual void                applyUniformVector2(s32 location, const core::Vector2D& vector) = 0;
        virtual void                applyUniformVector3(s32 location, const core::Vector3D& vector) = 0;
        virtual void                applyUniformVector4(s32 location, const core::Vector4D& vector) = 0;
        virtual void                applyUniformMatrix3(s32 location, const core::Matrix3D& matrix) = 0;
        virtual void                applyUniformMatrix4(s32 location, const core::Matrix4D& matrix) = 0;

        bool                        m_enable;
        bool                        m_compileStatus;

        ShaderDataList              m_shaderDataList;
        ShaderList                  m_shaderList;
        std::vector<const c8*>      m_varyingsList;
};

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderProgram> ShaderProgramPtr;

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_SHADER_PROGRAM_H_
