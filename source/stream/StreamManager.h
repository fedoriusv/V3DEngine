#ifndef _F3D_STREAM_MANAGER_H_
#define _F3D_STREAM_MANAGER_H_

#include "Singleton.h"
#include "common.h"

namespace f3d
{
namespace stream
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class IStream;

    class CStreamManager : public Singleton<CStreamManager>
    {
    public:

        CStreamManager();
        virtual     ~CStreamManager();

        IStream*    createFileStream(const std::string& file);

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_STREAM_MANAGER_H_