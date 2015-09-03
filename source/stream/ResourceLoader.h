#ifndef _V3D_RESOURCE_LOADER_H_
#define _V3D_RESOURCE_LOADER_H_

#include "common.h"
#include "decoders/ResourceDecoder.h"

namespace v3d
{
namespace stream
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    template <class T>
    class TResourceLoader
    {
    public:

        TResourceLoader();
        virtual                         ~TResourceLoader();

        const T*                        get(const std::string& name);
        virtual const T*                load(const std::string& file, const std::string& alias = "") = 0;

        void                            unload(const std::string& name);
        void                            unload(const T* resource);
        void                            unloadAll();

        void                            registerDecoder(decoders::DecoderPtr decoder);
        void                            unregisterDecoder(decoders::DecoderPtr& decoder);

        void                            registerPath(const std::string& path);
        void                            unregisterPath(const std::string& path);

    protected:

        void                            insert(const T* resource, const std::string& key);
        const decoders::DecoderPtr      findDecoder(const std::string& extension);

        std::map<std::string, const T*> m_resources;
        decoders::DecoderList           m_decoders;
        std::vector<std::string>        m_pathes;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#include "ResourceLoader.inl"

#endif //_V3D_RESOURCE_LOADER_H_