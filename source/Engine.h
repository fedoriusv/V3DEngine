#ifndef _F3D_ENGINE_H_
#define _F3D_ENGINE_H_

#include "Valera3D.h"

namespace f3d
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class CEngine : public Singleton<CEngine>
	{
	public:

		CEngine();
		virtual							~CEngine();
		
		platform::PlatformPtr			getPlatform()          const;
		event::InputEventHandlerPtr 	getInputEventHandler() const;
		scene::SceneManagerPtr			getSceneManager()      const;
		platform::WindowPtr			    getWindow()            const;
		renderer::RendererPtr			getRenderer()          const;

		bool							init();
		bool							begin();
		bool							end();

	private:

		platform::PlatformPtr			m_platform;
		event::InputEventHandlerPtr	    m_inputEventHandler;
		scene::SceneManagerPtr			m_scene;

	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif //_F3D_ENGINE_H_
