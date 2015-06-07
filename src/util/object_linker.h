//
//  object_linker.h
//  liborange
//
//  Created by zhoubao on 14-3-27.
//
//

#ifndef __OBJECT_LINKER_H__
#define __OBJECT_LINKER_H__

#ifndef _RELEASE

class RefHeader
{
public:

    RefHeader();
    RefHeader(const RefHeader &);
    virtual ~RefHeader();
    const RefHeader & operator = (const RefHeader &);
    
    static RefHeader * getRootRef();
    static void dumpAllLinkedObjects();
    
    RefHeader * getNextRef(){ return __next_ref_; }
    RefHeader * getPrevRef(){ return __prev_ref_; }
    
    virtual bool isSafeReferenceType() { return false; }
    
private:
    static void __linkRef(RefHeader *p);
    static void __unlinkRef(RefHeader *p);
    
    
    RefHeader *__next_ref_;
    RefHeader *__prev_ref_;
};

#endif

#endif //__OBJECT_LINKER_H__
