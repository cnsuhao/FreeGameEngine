//
//  singleton.h
//  dydg_b
//
//  Created by zhoubao on 14-2-12.
//
//

#ifndef dydg_b_singleton_h
#define dydg_b_singleton_h

#include "assert_tool.h"

namespace ora
{
    //需要使用单例类，必须从Singleton派生，并且用宏IMPLEMENT_SINGLETON，完成定义。
    template<typename T>
    class Singleton
    {
    public:

        Singleton()
        {
            ASSERT_2(s_pInstance == nullptr, "Singleton Class can has only one instance!");
            
            s_pInstance = static_cast<T*>(this);
        }

        ~Singleton()
        {
            if(s_pInstance == this)
                s_pInstance = nullptr;
        }

        static void initInstance()
        {
            ASSERT_2(s_pInstance == nullptr, "Singleton Class can has only one instance!");

            new T();
        }

        static void finiInstance()
        {
            if(!s_pInstance) return;

            delete s_pInstance;
            s_pInstance = nullptr;
        }

        static bool hasInstance()
        {
            return s_pInstance != nullptr;
        }
        
        inline static T * getInstance() { return s_pInstance; }

        inline static T * instance()
        {
            ASSERT_2(s_pInstance, "Singleton<T>::instance");

            return s_pInstance;
        }

    private:
        static T * s_pInstance;
    };

} // end namespace ora

#define IMPLEMENT_SINGLETON(T) \
template<> T * ::ora::Singleton<T>::s_pInstance = nullptr

#endif
