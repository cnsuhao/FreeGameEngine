//
//  vertex.h
//  liborange
//
//  Created by zhoubao on 14-2-26.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef __liborange__Vertex__
#define __liborange__Vertex__

#include "base.h"
#include "color.h"

namespace ora
{
#define DEF_VERTEX_TYPE(name) static const char * getType(){ return #name; }

    struct VertexXYZ
    {
        Vector3 position;

        DEF_VERTEX_TYPE(oxyz)
    };

    struct VertexXYZColor
    {
        Vector3 position;
        Color color;

        DEF_VERTEX_TYPE(oxyzc)
    };

    struct VertexXYZColorUV
    {
        Vector3 position;
        Color   color;
        Vector2 uv;

        DEF_VERTEX_TYPE(oxyzcuv);
    };

    struct VertexXYZN
    {
        Vector3 position;
        Vector3 normal;

        DEF_VERTEX_TYPE(oxyzn)
    };

    struct VertexXYZUV
    {
        Vector3 position;
        Vector2 uv;

        DEF_VERTEX_TYPE(oxyzuv)
    };

    struct VertexXYZNUV
    {
        Vector3 position;
        Vector3 normal;
        Vector2 uv;

        DEF_VERTEX_TYPE(oxyznuv)
    };

}//end namespace ora

#endif /* defined(__liborange__Vertex__) */
