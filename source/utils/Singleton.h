#pragma once

#include "common.h"

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

        static T*               s_instance;
        static std::once_flag   s_onceFlag;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    template <class T>
    T* TSingleton<T>::s_instance = nullptr;

    template <class T>
    std::once_flag TSingleton<T>::s_onceFlag;

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
        std::call_once(TSingleton::s_onceFlag, []()
        {
            s_instance = new T;
        });

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
