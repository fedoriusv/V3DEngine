#ifndef _V3D_INTRUSIVE_PTR_H_
#define _V3D_INTRUSIVE_PTR_H_

namespace v3d
{
namespace utils
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    template <class T>
    class TIntrusivePtr
    {
    private:

        typedef TIntrusivePtr this_type;

    public:

        TIntrusivePtr();
        TIntrusivePtr(T* p, bool add_ref = true);
        TIntrusivePtr(TIntrusivePtr const& rhs);
        ~TIntrusivePtr();

        template<class U>
        TIntrusivePtr(TIntrusivePtr<U> const& rhs)
            : px(rhs.get())
        {
            if (px != 0) intrusive_ptr_add_ref(px);
        }

        template<class U> TIntrusivePtr& operator=(TIntrusivePtr<U> const& rhs)
        {
            this_type(rhs).swap(*this);
            return *this;
        }

        TIntrusivePtr&  operator=(TIntrusivePtr const& rhs);
        TIntrusivePtr&  operator=(T * rhs);
        T&              operator*()  const;
        T*              operator->() const;
        T*              get()        const;

        void            reset();
        void            reset(T* rhs);
        void            swap(TIntrusivePtr& rhs);

        operator bool()              const;

    private:

        T* m_px;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    template <class T>
    TIntrusivePtr<T>::TIntrusivePtr()
        : m_px(nullptr)
    {
    }

    template <class T>
    TIntrusivePtr<T>::TIntrusivePtr(T* p, bool add_ref)
        : m_px(p)
    {
        if (m_px != nullptr && add_ref)
        {
            intrusivePtrAddRef(m_px);
        }
    }

    template <class T>
    TIntrusivePtr<T>::TIntrusivePtr(TIntrusivePtr const& rhs)
        : m_px(rhs.m_px)
    {
        if (m_px != nullptr)
        {
            intrusivePtrAddRef(m_px);
        }
    }

    template <class T>
    TIntrusivePtr<T>::~TIntrusivePtr()
    {
        if (m_px != nullptr)
        {
            intrusivePtrRelease(m_px);
        }
    }

    //template <class T>
    //TIntrusivePtr<T>::TIntrusivePtr(TIntrusivePtr<U> const& rhs)
    //     : m_px(rhs.get())
    // {
    //         if (px != 0) intrusive_ptr_add_ref(px);
    // }

    template <class T>
    TIntrusivePtr<T>& TIntrusivePtr<T>::operator=(TIntrusivePtr const& rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    template <class T>
    TIntrusivePtr<T>& TIntrusivePtr<T>::operator=(T* rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    template <class T>
    void TIntrusivePtr<T>::reset()
    {
        this_type().swap(*this);
    }

    template <class T>
    void TIntrusivePtr<T>::reset(T* rhs)
    {
        this_type(rhs).swap(*this);
    }

    template <class T>
    T* TIntrusivePtr<T>::get() const
    {
        return m_px;
    }

    template <class T>
    T& TIntrusivePtr<T>::operator*() const
    {
        return *m_px;
    }

    template <class T>
    T* TIntrusivePtr<T>::operator->() const
    {
        return m_px;
    }

    template <class T>
    TIntrusivePtr<T>::operator bool() const
    {
        return m_px != nullptr;
    }

    template <class T>
    void TIntrusivePtr<T>::swap(TIntrusivePtr<T>& rhs)
    {
        T* tmp = m_px;
        m_px = rhs.m_px;
        rhs.m_px = tmp;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    template<class T, class U> 
    inline bool operator==(TIntrusivePtr<T> const& a, TIntrusivePtr<U> const& b)
    {
        return a.get() == b.get();
    }

    template<class T, class U>
    inline bool operator!=(TIntrusivePtr<T> const& a, TIntrusivePtr<U> const& b)
    {
        return a.get() != b.get();
    }

    template<class T, class U>
    inline bool operator==(TIntrusivePtr<T> const& a, U* b)
    {
        return a.get() == b;
    }

    template<class T, class U>
    inline bool operator!=(TIntrusivePtr<T> const& a, U* b)
    {
        return a.get() != b;
    }

    template<class T, class U>
    inline bool operator==(T* a, TIntrusivePtr<U> const& b)
    {
        return a == b.get();
    }

    template<class T, class U>
    inline bool operator!=(T* a, TIntrusivePtr<U> const& b)
    {
        return a != b.get();
    }

    template<class T>
    inline bool operator<(TIntrusivePtr<T> const& a, TIntrusivePtr<T> const& b)
    {
        return std::less<T*>()(a.get(), b.get());
    }

    template<class T>
    void swap(TIntrusivePtr<T>& lhs, TIntrusivePtr<T>& rhs)
    {
        lhs.swap(rhs);
    }

    template<class T>
    T * get_pointer(TIntrusivePtr<T> const& p)
    {
        return p.get();
    }

    template<class T, class U>
    TIntrusivePtr<T> static_pointer_cast(TIntrusivePtr<U> const& p)
    {
        return static_cast<T*>(p.get());
    }

    template<class T, class U>
    TIntrusivePtr<T> const_pointer_cast(TIntrusivePtr<U> const& p)
    {
        return const_cast<T*>(p.get());
    }

    template<class T, class U>
    TIntrusivePtr<T> dynamic_pointer_cast(TIntrusivePtr<U> const& p)
    {
        return dynamic_cast<T*>(p.get());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_INTRUSIVE_PTR_H_