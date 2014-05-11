#ifndef _F3D_SCENE_MANAGER_H_
#define _F3D_SCENE_MANAGER_H_

#include "scene/Node.h"
#include "renderer/Renderer.h"

namespace f3d
{
namespace scene
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class CSceneManager
	{
	public:

								CSceneManager();
								~CSceneManager();

		void					init();

		void					draw();

		void					clear();
		bool					drop(CNode* node);

		void					addNode(CNode* node);

		CNode*					getObjectByID(const s32 id);
		CNode*					getObjectByName(const std::string& name);

	private:

		double					m_currentTime;
		double					m_deltaTime;
		double					m_lastTime;

		void					updateDeltaTime();
		void					update(double time);

		std::vector<CNode*>		m_objects;

		renderer::CRendererPtr	m_renderer;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::shared_ptr<CSceneManager>	CSceneManagerPtr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SCENE_MANAGER_H_