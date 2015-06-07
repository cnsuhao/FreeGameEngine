
#include "util/log_tool.h"
#include "math/matrix.hpp"
#include "math/quat.hpp"

namespace
{
    struct foo
    {
        char c1;
        short s;
        char c2;
        int i;
    };
    
    struct foo2
    {
        char c1;
        short s;
        char c2;
        short i1;
        short i2;
    };

    struct bar
    {
        char c1; 
        char c2;
        short s;
        int i;
    };

#pragma pack(1)
    struct foo_pack
    {
        char c1;
        short s;
        char c2;
        int i;
    };
#pragma pack()

}

namespace ora
{

    void checkStructAlign()
    {
        ORA_LOG_DEBUG("sizeof foo is %d", sizeof(foo));
        ORA_LOG_DEBUG("sizeof bar is %d", sizeof(bar));
        ORA_LOG_DEBUG("sizeof foo2 is %d", sizeof(foo2));
        ORA_LOG_DEBUG("sizeof foo_pack is %d", sizeof(foo_pack));
        
        if(sizeof(Vector2) != 8)
            ORA_LOG_WARN("sizeof Vector2 is %d", sizeof(Vector2));
            
        if(sizeof(Vector3) != 12)
            ORA_LOG_WARN("sizeof Vector3 is %d", sizeof(Vector3));
            
        if(sizeof(Vector4) != 16)
            ORA_LOG_WARN("sizeof Vector4 is %d", sizeof(Vector4));
            
        if(sizeof(Quaternion) != 16)
            ORA_LOG_WARN("sizeof Quaternion is %d", sizeof(Quaternion));
            
        if(sizeof(Matrix) != 64)
            ORA_LOG_WARN("sizeof Matrix is %d", sizeof(Matrix));
    }

} // end namespace ora
