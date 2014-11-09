#ifndef _V3D_MODEL_F3D_DECODER_H_
#define _V3D_MODEL_F3D_DECODER_H_

#include "ResourceDecoder.h"

namespace v3d
{
namespace decoders
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CModelF3DDecoder : public CResourceDecoder
    {
    public:

        CModelF3DDecoder();
        explicit CModelF3DDecoder(std::initializer_list<std::string> supportedExtensions);

        virtual                 ~CModelF3DDecoder();

        stream::ResourcePtr     decode(const stream::IStreamPtr& stream) override;

        //int meshCount();
        //stream::ResourcePtr     decodeMesh(const stream::IStreamPtr& stream, int index);

        //stream::ResourcePtr list
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_MODEL_F3D_DECODER_H_
