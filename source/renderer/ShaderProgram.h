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

    /**
    * Shader Program Interface.
    */
    class IShaderProgram
    {
    public:

        enum EProgramFlags
        {
            eInvalid   = 1 << 1,
            eCreated   = 1 << 2,
            eLinked    = 1 << 3,
            eValidated = 1 << 4, 
            eDeleted   = 1 << 5
        };

        IShaderProgram();
        virtual                             ~IShaderProgram();

        bool                                isEnable() const;
        void                                setEnable(bool enable);

        bool                                getCompileStatus() const;

        bool                                setDefine(const std::string& name, const std::string& value = "");
        bool                                setUndefine(const std::string& name);

        virtual bool                        create()    = 0;
        virtual void                        destroy()   = 0;
        virtual void                        bind()      = 0;
        virtual void                        unbind()    = 0;

        bool                                create(const std::string& vertex, const std::string& fragment, u32 arg = 0, ...);
        bool                                create(const std::string& compute);

        u16                                 getFlags() const;

        bool                                isFlagPresent(EProgramFlags flag);

    protected:

        void                                setFlag(EProgramFlags flag);
        void                                addFlag(EProgramFlags flag);

        friend                              CRenderPass;
        friend                              CRenderer;

        void                                attachShader(const ShaderWPtr& shader);
        void                                detachShader(const ShaderWPtr& shader);

        void                                addShaderData(const ShaderDataPtr& data);
        void                                addVaryingsAttibutes(const std::vector<const c8*>& list);
        void                                addDefines(const ShaderDefinesList& list = {});

        bool                                updateShaderList();

        virtual bool                        applyUniform       (CShaderUniform* uniform)                    = 0;
        virtual void                        applyUniformInt    (s32 location, s32 value)                    = 0;
        virtual void                        applyUniformFloat  (s32 location, f32 value)                    = 0;
        virtual void                        applyUniformVector2(s32 location, const core::Vector2D& vector) = 0;
        virtual void                        applyUniformVector3(s32 location, const core::Vector3D& vector) = 0;
        virtual void                        applyUniformVector4(s32 location, const core::Vector4D& vector) = 0;
        virtual void                        applyUniformMatrix3(s32 location, const core::Matrix3D& matrix) = 0;
        virtual void                        applyUniformMatrix4(s32 location, const core::Matrix4D& matrix) = 0;

        void                                clear();

        bool                                m_enable;
        u16                                 m_flags;

        ShaderDataList                      m_shaderDataList;
        ShaderList                          m_shaderList;
        ShaderDefinesList                   m_defines;
        std::vector<const c8*>              m_varyingsList;
};

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<IShaderProgram> ShaderProgramPtr;

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_SHADER_PROGRAM_H_
