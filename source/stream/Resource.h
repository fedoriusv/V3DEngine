#ifndef _F3D_RESOURCE_H_
#define _F3D_RESOURCE_H_

#include "Object.h"
#include "stream/IStream.h"

namespace v3d
{
namespace stream
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CResource : public CObject
    {
    public:

        CResource();
        virtual             ~CResource();

        virtual void        init(IStream* stream) = 0;
        virtual bool        load()                = 0;

        IStream*            getStream() const;
        bool                isLoaded()  const;

        const std::string&  getResourseName() const;
        void                setResourseName(const std::string& name);

        const std::string&  getResourseFolder() const;
        void                setResourseFolder(const std::string& folder);

    protected:
        
        CResource(const CResource& other);
        CResource&          operator=(const CResource& other);

        void                setStream(stream::IStream* stream);
        void                swapContent(CResource* other);
        void                setLoaded(bool loaded);

    private:

        IStream*            m_stream;
        std::string         m_resourceName;
        std::string         m_resourceFolder;
        bool                m_isLoaded;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_RESOURCE_H_