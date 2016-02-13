#include "TextureResILDecoder.h"
#include "stream/StreamManager.h"
#include "Engine.h"
#ifdef USE_DEVIL
#   include "IL/il.h"
#   pragma comment(lib, "ResIL.lib")
#endif //USE_DEVIL

using namespace v3d;
using namespace stream;
using namespace decoders;
using namespace renderer;
using namespace core;

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

stream::CResource* CTextureResILDecoder::decode(const stream::IStreamPtr& stream)
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

    ILuint texid;
    ilGenImages(1, &texid);
    ilBindImage(texid);

    ILboolean success = ilLoadImage(charToWChar(file.c_str()));
    ASSERT(success == IL_TRUE, "CTextureResILDecoder: Invalid Texture");
    if (!success)
    {
        ilDeleteImages(1, &texid);
        return nullptr;
    }

    //Convert specific formats
    EImageFormat oldFormat = convertILFormat(ilGetInteger(IL_IMAGE_FORMAT));
    EImageType oldType = convertILType(ilGetInteger(IL_IMAGE_TYPE));
    if (oldFormat == EImageFormat::eRGB && (oldType == EImageType::eByte || oldType == EImageType::eUnsignedByte))
    {
        ilConvertImage(IL_RGBA, ilGetInteger(IL_IMAGE_TYPE));
    }
    else if (oldFormat == EImageFormat::eRGB && (oldType == EImageType::eByte || oldType == EImageType::eUnsignedByte))
    {
        ilConvertImage(IL_BGRA, ilGetInteger(IL_IMAGE_TYPE));
    }

    CTexture* texture = RENDERER->createTexture();

    stream::IStreamPtr data = CStreamManager::createMemoryStream();
    data->seekBeg(0);

    u16 width = ilGetInteger(IL_IMAGE_WIDTH);
    u16 height = ilGetInteger(IL_IMAGE_HEIGHT);
    u16 depth = ilGetInteger(IL_IMAGE_DEPTH);
    Vector3DU sizeImage(width, height, depth);
    data->write(&sizeImage, sizeof(Vector3DU), 1);

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

    ilDeleteImages(1, &texid);

    return texture;
#endif //USE_DEVIL

    return nullptr;
}

EImageFormat CTextureResILDecoder::convertILFormat(u32 format)
{
#ifdef USE_DEVIL
    switch (format)
    {
    case IL_RGB:
        return EImageFormat::eRGB;

    case IL_RGBA:
        return EImageFormat::eRGBA;

    case IL_BGR:
        return EImageFormat::eBGR;

    case IL_BGRA:
        return EImageFormat::eBGRA;

    case IL_COLOR_INDEX:
    case IL_ALPHA:
    case IL_LUMINANCE:
    case IL_LUMINANCE_ALPHA:
        ASSERT(false, "CTextureResILDecoder: deprecated formats");
        return EImageFormat::eRGB;

    default:
        ASSERT(false, "CTextureResILDecoder: Invalid IL format");
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

    default:
        ASSERT(false, "CTextureResILDecoder: Invalid IL Type");
        return EImageType::eUnsignedByte;
    }
#endif //USE_DEVIL

    return EImageType::eUnsignedByte;
}
