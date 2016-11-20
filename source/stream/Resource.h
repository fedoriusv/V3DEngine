#pragma once

#include "stream/IStream.h"

namespace v3d
{
namespace stream
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Base Interface for Resource class
    */
    class IResource
    {
    public:

        IResource();
        virtual             ~IResource();

        virtual void        init(const IStreamPtr& stream) = 0;
        virtual bool        load() = 0;

        const IStreamPtr&   getStream() const;
        bool                isLoaded()  const;

        const std::string&  getResourseName() const;
        void                setResourseName(const std::string& name);

        const std::string&  getResourseFolder() const;
        void                setResourseFolder(const std::string& folder);

    protected:
        
        IResource(const IResource& other);
        IResource&          operator=(const IResource& other);

        void                setStream(const IStreamPtr& stream);
        void                swapContent(IResource& other);
        void                setLoaded(bool loaded);

    private:

        IStreamPtr          m_stream;
        std::string         m_resourceName;
        std::string         m_resourceFolder;
        bool                m_isLoaded;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace stream
} //namespace v3d
