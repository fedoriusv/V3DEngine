#include "Resource.h"
#include "common.h"

using namespace v3d;
using namespace v3d::stream;

IResource::IResource()
    : m_stream(nullptr)
    , m_resourceName("")
    , m_resourceFolder("")
    , m_isLoaded(false)
{
}

IResource::~IResource()
{
}

IResource::IResource(const IResource& other)
    : m_stream(nullptr)
{
    m_resourceName = other.m_resourceName;
    m_resourceFolder = other.m_resourceFolder;
    m_isLoaded = other.m_isLoaded;
}

IResource& IResource::operator=(const IResource& other)
{
    if (this == &other)
    {
        return *this;
    }

    m_stream = other.m_stream;
    m_resourceName = other.m_resourceName;
    m_resourceFolder = other.m_resourceFolder;
    m_isLoaded = other.m_isLoaded;

    return *this;
}

const IStreamPtr& IResource::getStream() const
{
    return m_stream;
}

bool IResource::isLoaded() const
{
    return m_isLoaded;
}

const std::string& IResource::getResourseName() const
{
    return m_resourceName;
}

void IResource::setResourseName(const std::string& name)
{
    m_resourceName = name;
}

const std::string& IResource::getResourseFolder() const
{
    return m_resourceFolder;
}

void IResource::setResourseFolder(const std::string& folder)
{
    m_resourceFolder = folder;
}

void IResource::setStream(const stream::IStreamPtr& stream)
{
    m_stream = stream;
}

void IResource::swapContent(IResource& other)
{
    std::swap(m_resourceName, other.m_resourceName);
    std::swap(m_resourceFolder, other.m_resourceFolder);
    std::swap(m_stream, other.m_stream);
    std::swap(m_isLoaded, other.m_isLoaded);
}

void IResource::setLoaded(bool loaded)
{
    m_isLoaded = loaded;
}
