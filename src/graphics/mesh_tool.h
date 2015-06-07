#ifndef _MESH_TOOL_H
#define _MESH_TOOL_H

#include "mesh.h"

namespace ora
{

    MeshPtr createCube(const Vector3 & radius,
                       const std::string & shader = "shaders/texture_only.glsl",
                       const std::string & texture = "cocos.png");

    MeshPtr createCubeFrame(const Vector3 & radius,
                            const ora::Color & cr,
                            const std::string & shader = "shaders/color_only.glsl");

}//end namespace ora

#endif //_MESH_TOOL_H

