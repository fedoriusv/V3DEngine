#include "Object.h"

using namespace f3d;

CObject::CObject()
    : m_id(-1)
    , m_name("")
    , m_type(EObjectType::eTypeUnknown)
{
}

CObject::~CObject()
{
}

const std::string CObject::getName() const
{
	return m_name;
}

void CObject::setName(const std::string& name)
{
	m_name = name;
}

const EObjectType CObject::getObjectType() const
{
	return m_type;
}

const s32 CObject::getID() const
{
	return m_id;
}

void CObject::setID(const s32 id)
{
	m_id = id;
}
