#ifndef _F3D_FEDORIA_ENGINE_H_
#define _F3D_FEDORIA_ENGINE_H_

#include "Fedoria3D.h"

namespace f3d
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class CFedoriaEngine : public Singleton<CFedoriaEngine>
	{
	public:
		CFedoriaEngine();
		~CFedoriaEngine();
		
		platform::CPlatformPtr			getPlatform()          const;
		event::CInputEventHandlerPtr	getInputEventHandler() const;
		scene::CSceneManagerPtr			getSceneManager()      const;

		bool							begin();
		bool							end();

	private:

		void							init();

		platform::CPlatformPtr			m_platform;
		event::CInputEventHandlerPtr	m_inputEventHandler;
		scene::CSceneManagerPtr			m_scene;

	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif //_F3D_FEDORIA_ENGINE_H_
