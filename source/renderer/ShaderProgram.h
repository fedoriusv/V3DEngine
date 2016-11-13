#pragma once

#include "Shader.h"
#include "ShaderData.h"
#include "utils/Cloneable.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class RenderPass;
    class IRenderer;
    class ShaderProgram;

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Shader Program Interface.
    */
    class ShaderProgram : public utils::TCloneable<ShaderProgramPtr>
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

        ShaderProgram();
        virtual                             ~ShaderProgram();

        bool                                isEnable() const;
        void                                setEnable(bool enable);

        bool                                setDefine(const std::string& name, const std::string& value = "");
        bool                                setUndefine(const std::string& name);

        bool                                applyUniform(ShaderUniform* uniform);
        void                                applyUniformInt(s32 location, s32 value);
        void                                applyUniformFloat(s32 location, f32 value);
        void                                applyUniformVector2(s32 location, const core::Vector2D& vector);
        void                                applyUniformVector3(s32 location, const core::Vector3D& vector);
        void                                applyUniformVector4(s32 location, const core::Vector4D& vector);
        void                                applyUniformMatrix3(s32 location, const core::Matrix3D& matrix);
        void                                applyUniformMatrix4(s32 location, const core::Matrix4D& matrix);

        virtual bool                        create()    = 0;
        virtual void                        destroy()   = 0;
        virtual void                        bind()      = 0;
        virtual void                        unbind()    = 0;

        bool                                create(const std::vector<u8>& vertexBin, const std::vector<u8>& fragmentBin, u32 arg = 0, ...);
        bool                                create(const std::vector<u8>& computeBin);
        bool                                create(const std::string& vertex, const std::string& fragment, u32 arg = 0, ...);
        bool                                create(const std::string& compute);

        u16                                 getFlags() const;

        bool                                isFlagPresent(EProgramFlags flag);

    protected:

        ShaderProgram(const ShaderProgram& program);
        ShaderProgram& operator=(const ShaderProgram& program);

        void                                setFlag(EProgramFlags flag);
        void                                addFlag(EProgramFlags flag);

        friend                              RenderPass;
        friend                              IRenderer;

        void                                attachShader(const ShaderWPtr& shader);
        void                                detachShader(const ShaderWPtr& shader);

        void                                addShaderData(const ShaderDataPtr& data);
        void                                addVaryingsAttibutes(const std::vector<const c8*>& list);
        void                                addDefines(const ShaderDefinesList& list = {});

        bool                                updateShaderList();

        void                                clear();

        bool                                m_enable;
        u16                                 m_flags;

        //TODO: add: ConstantBuffers
        ShaderDataList                      m_shaderDataList;
        ShaderList                          m_shaderList;
        ShaderDefinesList                   m_defines;
        std::vector<const c8*>              m_varyingsList; //?
};

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d
