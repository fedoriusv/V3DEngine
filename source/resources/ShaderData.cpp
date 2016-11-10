#include "ShaderData.h"
#include "utils/Logger.h"

namespace v3d
{
namespace resources
{

    ShaderData::ShaderData()
    : m_body("")
    , m_name("")
{
}

    ShaderData::~ShaderData()
{
}

void ShaderData::init(const stream::IStreamPtr& stream)
{
    IResource::setStream(stream);
}

bool ShaderData::load()
{
    const stream::IStreamPtr& stream = IResource::getStream();
    if (!stream)
    {
        LOG_ERROR("CShaderSourceData::load: Empty Stream with name [%s]", IResource::getResourseName().c_str());
        return false;
    }

    m_name = IResource::getResourseName();
    
    stream->seekBeg(0);
    stream->read(m_body);

    return true;
}


const std::string& ShaderData::getName() const
{
    return m_name;
}

const std::string& ShaderData::getBody() const
{
    return m_body;
}

} //namespace resources
} //namespace v3d
