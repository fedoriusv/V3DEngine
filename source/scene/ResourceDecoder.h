#ifndef _F3D_RESOURCE_DECODER_H_
#define _F3D_RESOURCE_DECODER_H_

#include "common.h"
#include "stream/IStream.h"
#include "stream/Resource.h"

namespace f3d
{
namespace scene
{
	typedef std::shared_ptr<stream::CResource> ResourcePtr;
	
	class CResourceDecoder
	{
		public:
	
			CResourceDecoder();
			CResourceDecoder( const std::string supportedExtensions[] );

			virtual ~CResourceDecoder();

			virtual ResourcePtr				decode( stream::IStream& stream ) = 0;
			
			bool							isExtensionSupported( const std::string& extension ) const;
			void							setSupportedExtensions( const std::string supportedExtensions[] );
			const std::vector<std::string>&	getSupportedExtensions() const;

			
		protected:

			std::vector<std::string>		m_supportedExtensions;
	};
}
}

#endif