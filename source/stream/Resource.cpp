#include "Resource.h"
#include "common.h"

using namespace v3d;
using namespace v3d::stream;

CResource::CResource()
    : m_isLoaded(false)
    , m_stream(nullptr)
    , m_resourceName("")
    , m_resourceFolder("")
{
}

CResource::~CResource()
{
}

CResource::CResource(const CResource& other)
    : m_stream(nullptr)
{
    m_resourceName = other.m_resourceName;
    m_resourceFolder = other.m_resourceFolder;
    m_isLoaded = other.m_isLoaded;
}

CResource& CResource::operator=(const CResource& other)
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

const IStreamPtr& CResource::getStream() const
{
    return m_stream;
}

bool CResource::isLoaded() const
{
    return m_isLoaded;
}

const std::string& CResource::getResourseName() const
{
    return m_resourceName;
}

void CResource::setResourseName(const std::string& name)
{
    m_resourceName = name;
}

const std::string& CResource::getResourseFolder() const
{
    return m_resourceFolder;
}

void CResource::setResourseFolder(const std::string& folder)
{
    m_resourceFolder = folder;
}

void CResource::setStream(const stream::IStreamPtr& stream)
{
    m_stream = stream;
}

void CResource::swapContent(CResource& other)
{
    std::swap(m_resourceName, other.m_resourceName);
    std::swap(m_resourceFolder, other.m_resourceFolder);
    std::swap(m_stream, other.m_stream);
    std::swap(m_isLoaded, other.m_isLoaded);
}

void CResource::setLoaded(bool loaded)
{
    m_isLoaded = loaded;
}
