#ifndef _V3D_MATERIAL_H_
#define _V3D_MATERIAL_H_

#include "Texture.h"
#include "ShaderProgram.h"
#include "RenderTechnique.h"

namespace v3d
{
namespace renderer
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderer;

    class CMaterial : public CObject
    {
    public:

        CMaterial();
        virtual                     ~CMaterial();

        void                        setAmbientColor (const core::Vector4D& color);
        void                        setDiffuseColor (const core::Vector4D& color);
        void                        setSpecularColor(const core::Vector4D& color);
        void                        setEmissionColor(const core::Vector4D& color);
        void                        setShininess(const f32 value);
        void                        setTransparency(const f32 value);

        const core::Vector4D&       getAmbientColor()  const;
        const core::Vector4D&       getDiffuseColor()  const;
        const core::Vector4D&       getSpecularColor() const;
        const core::Vector4D&       getEmissionColor() const;
        f32                         getShininess()     const;
        f32                         getTransparency()  const;

        bool                        setTexture(const u32 layer, const std::string& file);
        bool                        setTexture(const u32 layer, const std::string* files[6]);
        void                        setTexture(const u32 layer, TexturePtr texture);
        
        const TexturePtr&           getTexture(const u32 layer) const;
        u32                         getTextureCount()           const;

        void                        destroyTexture(u32 layer);
        
        bool                        setRenderTechnique(const std::string& file);
        bool                        setRenderTechnique(const stream::IStreamPtr& stream);

        void                        setRenderTechnique(const RenderTechniquePtr& technique);
        const RenderTechniquePtr&   getRenderTechique() const;

    private:

        struct SMaterialData
        {
            core::Vector4D          _ambient;
            core::Vector4D          _diffuse;
            core::Vector4D          _specular;
            core::Vector4D          _emission;
            f32                     _shininess;
            f32                     _transparency;
        };

    protected:

        friend                      CRenderer;

        SMaterialData               m_materialData;
        
        TextureLayers               m_texture;
        RenderTechniquePtr          m_renderTechnique;

        bool                        m_needUpdate;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CMaterial> MaterialPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
#endif //_V3D_MATERIAL_H_
