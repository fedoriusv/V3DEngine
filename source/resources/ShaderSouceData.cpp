#include "ShaderSouceData.h"
#include "utils/Logger.h"

namespace v3d
{
namespace resources
{

CShaderSourceData::CShaderSourceData()
    : m_body("")
    , m_name("")
{
}

CShaderSourceData::~CShaderSourceData()
{
}

void CShaderSourceData::init(const stream::IStreamPtr& stream)
{
    CResource::setStream(stream);
}

bool CShaderSourceData::load()
{
    const stream::IStreamPtr& stream = CResource::getStream();
    if (!stream)
    {
        LOG_ERROR("CShaderSourceData::load: Empty Stream with name [%s]", CResource::getResourseName().c_str());
        return false;
    }

    m_name = CResource::getResourseName();
    
    stream->seekBeg(0);
    stream->read(m_body);

    return true;
}


const std::string& CShaderSourceData::getName() const
{
    return m_name;
}

const std::string& CShaderSourceData::getBody() const
{
    return m_body;
}

} //namespace resources
} //namespace v3d
