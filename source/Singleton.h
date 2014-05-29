#ifndef _F3D_SINGLETON_H_
#define _F3D_SINGLETON_H_

namespace v3d
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	class Singleton
	{
	public:

		static T*	getInstance();
		static void freeInstance();
	
	protected:
					Singleton();
		virtual		~Singleton();
	
	private:

		static T*	s_pInstance;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	T* Singleton<T>::s_pInstance = nullptr;

	template <class T>
	Singleton<T>::Singleton()
	{
	}

	template <class T>
	Singleton<T>::~Singleton()
	{
	}

	template <class T>
	T *Singleton<T>::getInstance()
	{
		if( s_pInstance == nullptr )
		{
			s_pInstance = new T;
		}

		return s_pInstance;
	}

	template <class T>
	void Singleton<T>::freeInstance()
	{
		if( s_pInstance )	
		{
			delete s_pInstance;
		}

		s_pInstance = nullptr;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////

}
#endif //_F3D_SINGLETON_H_
