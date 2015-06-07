//
//  variant.h
//  orange
//
//  Created by zhoubao on 14-6-9.
//
//

#ifndef __orange__variant__
#define __orange__variant__

#include "smartptr.h"
#include "log_tool.h"

namespace ora
{
    
    class VarProxy : public IReferenceCount
    {
    public:
        VarProxy();
        virtual ~VarProxy();
        
        virtual const std::type_info & getType() const = 0;
        virtual const char * getTypeName() const = 0;
        
    };
    typedef SmartPtr<VarProxy> VarProxyPtr;
    
    
    template<typename T>
    class VarProxyImp : public VarProxy
    {
    public:
        VarProxyImp()
        {}
        
        VarProxyImp(const T & v)
            : value_(v)
        {}
        
        virtual const std::type_info & getType() const { return typeid(value_); }
        virtual const char * getTypeName() const{ return typeid(value_).name(); }

        /** the real value*/
        T value_;
    };
    
    
    struct TPOD
    {
        union
        {
            bool        boolv;
            char        charv;
            short       shortv;
            int         intv;
            long        longv;
            float       floatv;
            double      doublev;
        };
    };
    
    
    class Variant
    {
    public:
        
        Variant()
        {
        }
        
        template<typename T>
        Variant(const T & v)
        {
            setv(v);
        }
        
        
#define DEF_GETSET(TYPE, NAME) \
    inline void setv(TYPE  v){ v2_.NAME = v; }\
    inline bool getv(TYPE &v){ v = v2_.NAME; return true; }

        DEF_GETSET(bool, boolv);
        DEF_GETSET(char, charv);
        DEF_GETSET(unsigned char, charv);
        DEF_GETSET(short, shortv);
        DEF_GETSET(unsigned short, shortv);
        DEF_GETSET(int, intv);
        DEF_GETSET(unsigned int, intv);
        DEF_GETSET(long, longv);
        DEF_GETSET(unsigned long, longv);
        DEF_GETSET(float, floatv);
        DEF_GETSET(double, doublev);
        
#undef DEF_GETSET
        
        void setv(const char *v){ v3_ = v; }
        bool getv(const char *&v){ v = v3_.c_str(); return true; }
        
        void setv(const std::string & v){ v3_ = v; }
        bool getv(std::string &v){ v= v3_; return true; }
        
        
        template<typename T>
        void setv(const T & v)
        {
            v_ = new VarProxyImp<T>(v);
        }
        
        template<typename T>
        bool getv(T & v)
        {
            if(!v_) return false;
            
            typedef VarProxyImp<T> TProxy;
            
            //check type
            if(v_->getType() != typeid(T))
                return false;
            
            TProxy * p = v_.cast<TProxy>();
            if(!p) return false;
            
            v = p->value_;
            return true;
        }
        
        template<typename T>
        T getv2(const T & defv=T())
        {
            T v;
            if(!getv(v))
            {
                const char * valueType = v_ ? v_->getTypeName() : "nullptr";
                
                ORA_LOG_WARN("Variant::getv2 - type '%s' miss match with '%s'!",
                             typeid(T).name(), valueType);
                
                return defv;
            }
            
            return v;
        }
        
    private:
        VarProxyPtr v_;
        TPOD        v2_; //优化内建数据类型
        std::string v3_;
    };
    
    
    typedef std::vector<Variant> VariantVector;

    //extern from variant.cpp
    extern VariantVector NullArgument;
    
#if defined(WIN32) && (_MSC_VER <= 1700) //for visual studio 2012

    inline void build_arguments(VariantVector & arg_list)
    {
        arg_list.clear();
    }

    template<typename A1>
    void build_arguments(VariantVector & arg_list, const A1 & a1)
    {
        arg_list.clear();
        arg_list.push_back(Variant(a1));
    }

    template<typename A1, typename A2>
    void build_arguments(VariantVector & arg_list, const A1 & a1, const A2 & a2)
    {
        arg_list.clear();
        arg_list.push_back(Variant(a1));
        arg_list.push_back(Variant(a2));
    }

    template<typename A1, typename A2, typename A3>
    void build_arguments(VariantVector & arg_list, const A1 & a1, const A2 & a2, const A3 & a3)
    {
        arg_list.clear();
        arg_list.push_back(Variant(a1));
        arg_list.push_back(Variant(a2));
        arg_list.push_back(Variant(a3));
    }

    template<typename A1, typename A2, typename A3, typename A4>
    void build_arguments(VariantVector & arg_list, const A1 & a1, const A2 & a2, const A3 & a3, const A4 & a4)
    {
        arg_list.clear();
        arg_list.push_back(Variant(a1));
        arg_list.push_back(Variant(a2));
        arg_list.push_back(Variant(a3));
        arg_list.push_back(Variant(a4));
    }


    inline bool parse_arguments(VariantVector & arg_list)
    {
        return true;
    }

    template<typename A1>
    bool parse_arguments(VariantVector & arg_list, A1 * a1)
    {
        if(arg_list.size() < 1) return false;
        
        if(!arg_list[0].getv(*a1)) return false;
        return true;
    }

    template<typename A1, typename A2>
    bool parse_arguments(VariantVector & arg_list, A1 * a1, A2 * a2)
    {
        if(arg_list.size() < 2) return false;
        
        if(!arg_list[0].getv(*a1)) return false;
        if(!arg_list[1].getv(*a2)) return false;
        return true;
    }

    template<typename A1, typename A2, typename A3>
    bool parse_arguments(VariantVector & arg_list, A1 * a1, A2 * a2, A3 * a3)
    {
        if(arg_list.size() < 3) return false;
        
        if(!arg_list[0].getv(*a1)) return false;
        if(!arg_list[1].getv(*a2)) return false;
        if(!arg_list[2].getv(*a3)) return false;
        return true;
    }

    template<typename A1, typename A2, typename A3, typename A4>
    bool parse_arguments(VariantVector & arg_list, A1 * a1, A2 * a2, A3 * a3, A4 * a4)
    {
        if(arg_list.size() < 4) return false;
        
        if(!arg_list[0].getv(*a1)) return false;
        if(!arg_list[1].getv(*a2)) return false;
        if(!arg_list[2].getv(*a3)) return false;
        if(!arg_list[4].getv(*a4)) return false;
        return true;
    }

#else

    inline void build_arguments_i(VariantVector & arg_list) {}
    inline bool parse_arguments_i(VariantVector & args, size_t i) { return true; }
    

    template<typename ...TARGS>
    void build_arguments(VariantVector & arg_list, TARGS... args)
    {
        arg_list.clear();
        build_arguments_i(arg_list, args...);
    }
    
    template<typename T,  typename ...TARGS>
    void build_arguments_i(VariantVector & arg_list, const T & firstV, TARGS ...args)
    {
        arg_list.push_back(Variant(firstV));
        build_arguments_i(arg_list, args...);
    }
    
    
    
    /** parse arguments from argument list. each argument must be a pointer.
     *  if anyone of arguments convert failed, the function return false.
     */
    template<typename ...TARGS>
    bool parse_arguments(VariantVector & arg_list, TARGS... args)
    {
        return parse_arguments_i(arg_list, 0, args...);
    }
    
    /** parse arguments from ist to end.*/
    template<typename T, typename ...TARGS>
    bool parse_arguments_i(VariantVector & arg_list, size_t i, T firstV, TARGS ...args)
    {
        //T must be a pointer.
        ASSERT_1(firstV != nullptr);

        if(i >= arg_list.size()) return true;
        
        if(!arg_list[i].getv(*firstV)) return false;
        
        return parse_arguments_i(arg_list, i + 1, args...);
    }
#endif
    
    
    
} // end namespace ora

#endif /* defined(__orange__variant__) */
