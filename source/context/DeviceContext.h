#pragma once

#include "common.h"
#include "GraphicsCaps.h"
#include "platform/Window.h"
#include "renderer/Texture.h"
#include "renderer/Buffer.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Interface Context stored all information about device context.
    */
    class DeviceContext
    {
    public:

        explicit                    DeviceContext(const platform::WindowPtr window);
        virtual                     ~DeviceContext();

        virtual void                checkForErrors(const std::string& location = "") = 0; //TODO: need create debug file
        virtual bool                create() = 0;
        virtual void                destroy() = 0;

        virtual bool                setVSync(bool use) = 0;
        virtual bool                present() = 0;

        bool                        isVSync() const;

        const core::Dimension2D&    getWindowSize() const;
        const platform::WindowPtr   getWindow() const;

        const std::string&          getShaderVersion() const;
        s32                         getRenderMajorVersion() const;
        s32                         getRenderMinorrVersion() const;

        u32                         getSamplesCount() const;
        u32                         getTextureUnitsCount() const;
        f32                         getMaxAnisotropySize() const;
        u32                         getMaxPatchVertices() const;
        u64                         getMemoryBlockSize() const;

    protected:

        friend                      platform::Platform;
        friend                      Texture;
        friend                      Buffer;

        virtual Texture*            createTexture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 level) = 0;
        virtual Texture*            createCubeTexture(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[6], u32 level) = 0;
        virtual Buffer*             createBuffer(EBufferTarget target, EDataUsageType type) = 0;

        virtual void                fillGrapthicCaps() = 0;

        std::string                 m_shaderVersion;
        s32                         m_renderMajorVersion;
        s32                         m_renderMinorVersion;

        GpraphicsCaps               m_graphicsCaps;

    protected:

        bool                        m_isVSync;

    private:

        const platform::WindowPtr   m_window;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    using ContextPtr  =             std::shared_ptr<DeviceContext>;
    using ContextWPtr =             std::weak_ptr<DeviceContext>;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d
