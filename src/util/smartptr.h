#ifndef MYTOOL_SMARTPTR_H
#define MYTOOL_SMARTPTR_H

#include "util_config.h"
#include "object_linker.h"
#include "assert_tool.h"

namespace ora
{

    ///引用计数基类。可以安全的用于智能指针。
    class IReferenceCount
#ifndef _RETAIL
        : public RefHeader //内存检查
#endif
    {
    public:
        IReferenceCount()
            : _referenceCount(0)
        {}

        IReferenceCount(const IReferenceCount &)
            : _referenceCount(0)
        {}

        virtual ~IReferenceCount()
        {}

        const IReferenceCount & operator = (const IReferenceCount &)
        {
            return *this;
        }

        void retain()
        {
            ++_referenceCount;
        }

        void release()
        {
            --_referenceCount;

            ASSERT_2(_referenceCount >= 0, "IReferenceCount::release");

            if (_referenceCount <= 0)
                delete this;
        }

        virtual bool isSafeReferenceType()
        {
            return true;
        }

    protected:
        unsigned int _referenceCount;
    };


    ///智能指针
    template<typename T>
    class SmartPtr
    {
    public:
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef const T * const_pointer;
        typedef const T & const_reference;
        typedef SmartPtr<T> this_type;

        SmartPtr()
            : m_ptr(nullptr)
        {}

        SmartPtr(pointer p)
            : m_ptr(p)
        {
            if (m_ptr != nullptr)
            {
                m_ptr->retain();
            }
        }

        SmartPtr(const this_type & o)
            : m_ptr(o.m_ptr)
        {
            if (m_ptr != nullptr)
            {
                m_ptr->retain();
            }
        }

        template<typename U>
        SmartPtr(U * p)
            : m_ptr(nullptr)
        {
            *this = p;
        }
        
        template<typename U>
        SmartPtr(const SmartPtr<U> & o)
            : m_ptr(nullptr)
        {
            *this = o.get();
        }
        

        ~SmartPtr()
        {
            if(m_ptr != nullptr) m_ptr->release();
        }

        pointer get() const
        {
            return m_ptr;
        }

        template<typename U>
        U* cast() const
        {
            return (U*) m_ptr;
        }

	    bool hasObject() const
	    {
		    return m_ptr != 0;
	    }

	    bool exists() const
	    {
		    return m_ptr != 0;
	    }

    public:

        operator bool () const
        {
            return m_ptr != nullptr;
        }

        pointer operator -> () const
        {
            ASSERT_2(m_ptr != nullptr, "SmartPtr::operator ->");
            return m_ptr;
        }

        reference operator * () const
        {
            ASSERT_2(m_ptr != nullptr, "SmartPtr::operator *");
            return *m_ptr;
        }

        const this_type & operator = (pointer p)
        {
            if(m_ptr != p)
            {
                if(m_ptr != nullptr) m_ptr->release();
                m_ptr = p;
                if (m_ptr != nullptr)
                {
                    m_ptr->retain();
                }
            }
            return *this;
        }

        template<typename U>
        const this_type & operator = (U *p)
        {
            if(m_ptr != p)
            {
                if(m_ptr != nullptr) m_ptr->release();

                if(p != nullptr)
                {
#ifdef _RETAIL
                    m_ptr = pointer(p);
#else
                    m_ptr = dynamic_cast<pointer>(p);
                    ASSERT_2(m_ptr, "SmartPtr::operator= - Invalid type cast!");
#endif
                    m_ptr->retain();
                }
                else
                {
                    m_ptr = nullptr;
                }
            }
            return *this;
        }

        const this_type & operator = (const this_type & o)
        {
            return *this = o.m_ptr;
        }

        template<typename U>
        const this_type & operator = (const SmartPtr<U> & o)
        {
            return *this = o.get();
        }

        bool operator == (const this_type & o) const
        {
            return m_ptr == o.m_ptr;
        }

        bool operator < (const this_type & o) const
        {
            return m_ptr < o.m_ptr;
        }

        bool operator > (const this_type & o) const
        {
            return m_ptr > o.m_ptr;
        }

#if 0
#ifdef IOS

		SmartPtr(std::nullptr_t)
			: m_ptr(nullptr)
		{
		}

		const this_type & operator = (std::nullptr_t)
		{
			if (m_ptr) m_ptr->release();
			m_ptr = nullptr;
			return *this;
		}

#endif
#endif

    private:
        pointer m_ptr;
    };


    template<typename T, typename U>
    bool operator == (const SmartPtr<T> & l, const U * r)
    {
        return l.get() == r;
    }

    template<typename T, typename U>
    bool operator == (const T *l, const SmartPtr<U> & r)
    {
        return l == r.get();
    }

    template<typename T, typename U>
    bool operator == (const SmartPtr<T> & l, const SmartPtr<U> & r)
    {
        return l.get() == r.get();
    }

    template<typename T, typename U>
    bool operator != (const SmartPtr<T> & l, const U * r)
    {
        return l.get() != r;
    }

    template<typename T, typename U>
    bool operator != (const T *l, const SmartPtr<U> & r)
    {
        return l != r.get();
    }

    template<typename T, typename U>
    bool operator != (const SmartPtr<T> & l, const SmartPtr<U> & r)
    {
        return l.get() != r.get();
    }

}// end namespace ora

using ora::SmartPtr;
using ora::IReferenceCount;

#endif //MYTOOL_SMARTPTR_H
