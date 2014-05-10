#ifndef _F3D_RESOURCE_H_
#define _F3D_RESOURCE_H_

#include "common.h"
#include "Object.h"
#include "stream/IStream.h"

namespace f3d
{
namespace stream
{
	class CResource : public CObject
	{
	public:

		CResource();
		virtual ~CResource();

		virtual bool init(IStream* stream) = 0;

	private:

	};
}
}

#endif //_F3D_RESOURCE_H_