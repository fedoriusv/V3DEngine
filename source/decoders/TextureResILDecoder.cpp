#include "TextureResILDecoder.h"
#include "stream/StreamManager.h"
#include "Engine.h"
#ifdef USE_DEVIL
#   include "IL/il.h"
#endif //USE_DEVIL

using namespace v3d;
using namespace v3d::stream;
using namespace v3d::decoders;
using namespace v3d::renderer;

CTextureResILDecoder::CTextureResILDecoder()
    : CResourceDecoder()
{
}

CTextureResILDecoder::CTextureResILDecoder(std::initializer_list<std::string> supportedExtensions)
    : CResourceDecoder(supportedExtensions)
{
}

CTextureResILDecoder::~CTextureResILDecoder()
{
}

stream::ResourcePtr CTextureResILDecoder::decode(const stream::IStreamPtr& stream)
{
    std::function<wchar_t*(const char*)> charToWChar = [](const char* text)
    {
        size_t size = strlen(text) + 1;
        wchar_t* wa = new wchar_t[size];
        mbstowcs(wa, text, size);
        return wa;
    };

    std::string file = std::static_pointer_cast<FileStream>(stream)->getName();

#ifdef USE_DEVIL
    ilInit();
    ILboolean success = ilLoadImage(charToWChar(file.c_str()));
    ASSERT(success == IL_TRUE && "Invalid Texture");
    if (!success)
    {
        return nullptr;
    }

    renderer::TexturePtr texture = RENDERER->makeSharedTexture();

    stream::IStreamPtr data = CStreamManager::createMemoryStream();
    data->seekBeg(0);
    u16 width = ilGetInteger(IL_IMAGE_WIDTH);
    data->write(width);
    u16 height = ilGetInteger(IL_IMAGE_HEIGHT);
    data->write(height);
    u16 depth = ilGetInteger(IL_IMAGE_DEPTH);
    data->write(depth);
    ILenum format = ilGetInteger(IL_IMAGE_FORMAT);
    data->write((s32)convertILFormat(format));
    ILenum type = ilGetInteger(IL_IMAGE_TYPE);
    data->write((s32)convertILType(type));

    ilConvertImage(format, type);

    u32 size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
    void* bytes = (ILubyte*)malloc(size);
    memcpy(bytes, ilGetData(), size);
    data->write(size);
    data->write(bytes, size);

    texture->init(data);

    return texture;
#endif //USE_DEVIL

    return nullptr;
}

EImageFormat CTextureResILDecoder::convertILFormat(u32 format)
{
#ifdef USE_DEVIL
    switch (format)
    {
    case IL_COLOR_INDEX:
        return EImageFormat::eColorIndex;

    case IL_ALPHA:
        return EImageFormat::eAlpha;

    case IL_RGB:
        return EImageFormat::eRGB;

    case IL_RGBA:
        return EImageFormat::eRGBA;

    case IL_BGR:
        return EImageFormat::eBGR;

    case IL_BGRA:
        return EImageFormat::eBGRA;

    case IL_LUMINANCE:
        return EImageFormat::eLuminance;

    case IL_LUMINANCE_ALPHA:
        return EImageFormat::eLuminanceAlpha;

    default:
        ASSERT(true && "Invalid IL Format");
        return EImageFormat::eRGB;
    }
#endif //USE_DEVIL

    return EImageFormat::eRGB;
}

EImageType CTextureResILDecoder::convertILType(u32 type)
{
#ifdef USE_DEVIL
    switch (type)
    {
    case IL_BYTE:
        return EImageType::eByte;

    case IL_UNSIGNED_BYTE:
        return EImageType::eUnsignedByte;

    case IL_SHORT:
        return EImageType::eShort;

    case IL_UNSIGNED_SHORT:
        return EImageType::eUnsignedShort;

    case IL_INT:
        return EImageType::eInt;

    case IL_UNSIGNED_INT:
        return EImageType::eUnsignedInt;

    case IL_FLOAT:
        return EImageType::eFloat;

    case IL_DOUBLE:
        return EImageType::eDouble;

    case IL_HALF:
        return EImageType::eHalf;

    default:
        ASSERT(true && "Invalid IL Type");
        return EImageType::eUnsignedByte;
    }
#endif //USE_DEVIL

    return EImageType::eUnsignedByte;
}
