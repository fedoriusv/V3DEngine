#pragma once

#include "stream/Resource.h"
#include "renderer/ImageFormats.h"
#include "utils/RefCounted.h"
#include "utils/IntrusivePtr.h"

namespace v3d
{
namespace resources
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Image class.
    */
    class CImage : public stream::IResource, public utils::CRefCounted
    {
    public:

        CImage();
        CImage(renderer::EImageFormat format, renderer::EImageType type, const stream::IStreamPtr stream);
        CImage(renderer::EImageFormat format, renderer::EImageType type, const core::Dimension2D& size, const u8* data);

        CImage(const CImage&) = delete;
        ~CImage();

        CImage&                     operator=(const CImage& image);
        bool                        operator==(const CImage& image);

        void                        init(const stream::IStreamPtr stream) override;
        bool                        load() override;

        u32                         getByteCount() const;
        u32                         getPixelCount() const;
        u32                         getBytePerPixelCount() const;

        const u8*                   getPixel(u32 index) const;
        void                        setPixel(u32 index, const u8* data);
        void                        setPixel(const core::Point2D& point, const u8* data);

        void                        fill(const core::Rect32& rect, const u8* data);
        CImage*                     copyRect(const core::Rect32& rect) const;

        renderer::EImageFormat      getFormat() const;
        renderer::EImageType        getType() const;
        const u8*                   getData() const;

        const core::Dimension2D&    getSize() const;

    private:

        void                        setFormat(renderer::EImageFormat format);
        void                        setType(renderer::EImageType type);

        renderer::EImageFormat      m_format;
        renderer::EImageType        m_type;

        core::Dimension2D           m_size;

        u8*                         m_data;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CImage> ImagePtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace resources
} //namespace v3d
