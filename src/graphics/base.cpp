//
//  base.cpp
//  my3d_libs
//
//  Created by zhoubao on 14-3-28.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#include "base.h"

GLenum __gl_error_code = GL_NO_ERROR;

#if defined(ANDROID)
    
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray = nullptr;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays = nullptr;
PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays = nullptr;
PFNGLISVERTEXARRAYOESPROC glIsVertexArray = nullptr;

#endif

