#ifndef _V3D_SINGLETON_H_
#define _V3D_SINGLETON_H_

namespace v3d
{
namespace utils
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    template <class T>
    class TSingleton
    {
    public:

        static T*   getInstance();
        static void freeInstance();

    protected:

                    TSingleton();
        virtual     ~TSingleton();

    private:

        static T*   s_instance;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    template <class T>
    T* TSingleton<T>::s_instance = nullptr;

    template <class T>
    TSingleton<T>::TSingleton()
    {
    }

    template <class T>
    TSingleton<T>::~TSingleton()
    {
    }

    template <class T>
    T *TSingleton<T>::getInstance()
    {
        if (s_instance == nullptr)
        {
            s_instance = new T;
        }

        return s_instance;
    }

    template <class T>
    void TSingleton<T>::freeInstance()
    {
        if (s_instance)
        {
            delete s_instance;
        }

        s_instance = nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace utils
} //namespace v3d

#endif //_V3D_SINGLETON_H_
