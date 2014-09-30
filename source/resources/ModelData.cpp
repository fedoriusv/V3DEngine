#include "ModelData.h"

using namespace v3d;
using namespace v3d::resources;

CModelData::CModelData()
{
}

CModelData::~CModelData()
{
}

void CModelData::init(const stream::IStreamPtr& stream)
{
    CResource::setStream(stream);
}

bool CModelData::load()
{
    return false;
}
