#ifndef _V3D_MATERIAL_H_
#define _V3D_MATERIAL_H_

#include "Texture.h"
#include "stream/Resource.h"
#include "utils/Cloneable.h"
#include "utils/RefCounted.h"
#include "utils/IntrusivePtr.h"
#include "ShaderProgram.h"

namespace v3d
{
namespace renderer
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderer;
    class CMaterial;

    typedef utils::TIntrusivePtr<CMaterial> MaterialPtr;

    /**
    * Matrial class
    */
    class CMaterial : public stream::IResource, public utils::TCloneable<MaterialPtr>, public utils::CRefCounted
    {
    public:

        CMaterial();
        CMaterial(const CMaterial&);
        virtual                     ~CMaterial();

        void                        setAmbientColor (const core::Vector4D& color);
        void                        setDiffuseColor (const core::Vector4D& color);
        void                        setSpecularColor(const core::Vector4D& color);
        void                        setEmissionColor(const core::Vector4D& color);
        void                        setShininess(f32 value);
        void                        setGlossiness(f32 value);
        void                        setTransparency(f32 value);

        const core::Vector4D&       getAmbientColor()  const;
        const core::Vector4D&       getDiffuseColor()  const;
        const core::Vector4D&       getSpecularColor() const;
        const core::Vector4D&       getEmissionColor() const;
        f32                         getShininess()     const;
        f32                         getGlossiness()    const;
        f32                         getTransparency()  const;

        bool                        setTexture(u32 unit, const std::string& file);
        bool                        setTexture(u32 unit, const std::string files[6]);
        void                        setTexture(u32 unit, const TexturePtr& texture);
        
        const TexturePtr            getTexture(u32 unit) const;
        TexturePtr                  getTexture(u32 unit);
        u32                         getTextureCount()           const;

        const std::string&          getName() const;

        void                        init(const stream::IStreamPtr& stream)  override;
        bool                        load()                                  override;

        MaterialPtr                 clone() const                           override;

    private:

        struct SMaterialData
        {
            core::Vector4D          _ambient;
            core::Vector4D          _diffuse;
            core::Vector4D          _specular;
            core::Vector4D          _emission;
            f32                     _shininess;
            f32                     _glossiness;
            f32                     _transparency;

            SMaterialData& operator=(const SMaterialData&);
        };

    protected:

        friend                      CRenderer;

        SMaterialData               m_materialData;
        
        TextureUnits                m_texture;

        bool                        m_needUpdate;
        std::string                 m_name;

    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}
#endif //_V3D_MATERIAL_H_
