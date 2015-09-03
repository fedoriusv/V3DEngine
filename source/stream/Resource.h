#ifndef _V3D_RESOURCE_H_
#define _V3D_RESOURCE_H_

#include "stream/IStream.h"

namespace v3d
{
namespace stream
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CResource
    {
    public:

        CResource();
        virtual             ~CResource();

        virtual void        init(const IStreamPtr& stream) = 0;
        virtual bool        load()                         = 0;

        const IStreamPtr&   getStream() const;
        bool                isLoaded()  const;

        const std::string&  getResourseName() const;
        void                setResourseName(const std::string& name);

        const std::string&  getResourseFolder() const;
        void                setResourseFolder(const std::string& folder);

    protected:
        
        CResource(const CResource& other);
        CResource&          operator=(const CResource& other);

        void                setStream(const IStreamPtr& stream);
        void                swapContent(CResource& other);
        void                setLoaded(bool loaded);

    private:

        IStreamPtr          m_stream;
        std::string         m_resourceName;
        std::string         m_resourceFolder;
        bool                m_isLoaded;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif //_V3D_RESOURCE_H_
