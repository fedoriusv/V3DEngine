#ifndef _V3D_STREAM_MANAGER_H_
#define _V3D_STREAM_MANAGER_H_

#include "Singleton.h"
#include "common.h"

namespace v3d
{
namespace stream
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class FileStream;

    class CStreamManager : public Singleton<CStreamManager>
    {
    public:

        CStreamManager();
        virtual         ~CStreamManager();

        FileStream*    createFileStream(const std::string& file);

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_STREAM_MANAGER_H_
