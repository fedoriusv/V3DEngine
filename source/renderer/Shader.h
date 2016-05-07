#ifndef _V3D_SHADER_H_
#define _V3D_SHADER_H_

#include "common.h"
#include "utils/Cloneable.h"
#include "ShaderSouce.h"

namespace tinyxml2
{
    class XMLElement;
} //namespace tinyxml2

namespace v3d
{

namespace scene
{
    class CShaderManager;
} //namespace scene

namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderPass;

    /**
    * Base class for shader interface.
    */
    class IShader: public utils::TCloneable<std::shared_ptr<IShader>>
    {
    public:

        enum EShaderFlags
        {
            eInvalid    = 1  << 1,
            eLoaded     = 1  << 2,
            eCreated    = 1  << 3,
            eCompiled   = 1  << 4,
            eDeleted    = 1  << 5
        };

        IShader();

        IShader(const IShader&) = delete;
        IShader& operator=(const IShader&) = delete;

        virtual                     ~IShader();

        bool                        create(EShaderType type, const std::string& body, const ShaderDefinesList& defines = {});
        virtual void                destroy() = 0;

        EShaderType                 getType()  const;
        u16                         getFlags() const;

        bool                        isFlagPresent(EShaderFlags flag);

        const std::string&          getName() const;

        std::shared_ptr<IShader>    clone() override;

    protected:

        void                        setName(const std::string& name);
        void                        setType(EShaderType type);

        void                        setFlag(EShaderFlags flag);
        void                        addFlag(EShaderFlags flag);

        CShaderSource               m_data;
        u16                         m_flags;

    private:

        friend                      scene::CShaderManager;
        friend                      IShaderProgram;
        friend                      CRenderPass;

        const CShaderSource&        getShaderSource() const;

        virtual bool                create(CShaderSource&& data) = 0;

        static bool                 parse(const tinyxml2::XMLElement* root, CShaderSource& data);

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<IShader>            ShaderPtr;
    typedef std::weak_ptr<IShader>              ShaderWPtr;
    typedef std::vector<ShaderWPtr>             ShaderList;

    typedef std::pair<std::size_t, ShaderPtr>   ShaderHashPair;
    typedef std::map<std::size_t, ShaderPtr>    ShaderHashMap;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_SHADER_H_
