#ifndef _F3D_ENGINE_H_
#define _F3D_ENGINE_H_

#include "Fedoria3D.h"

namespace f3d
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class CEngine : public Singleton<CEngine>
	{
	public:
										CEngine();
		virtual							~CEngine();
		
		platform::CPlatformPtr			getPlatform()          const;
		event::CInputEventHandlerPtr	getInputEventHandler() const;
		scene::CSceneManagerPtr			getSceneManager()      const;
		platform::CWindowPtr			getWindow()            const;
		renderer::CRendererPtr			getRenderer()          const;

		bool							init();
		bool							begin();
		bool							end();

	private:

		platform::CPlatformPtr			m_platform;
		event::CInputEventHandlerPtr	m_inputEventHandler;
		scene::CSceneManagerPtr			m_scene;

	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif //_F3D_ENGINE_H_
