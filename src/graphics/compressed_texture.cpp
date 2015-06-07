//
//  CompressedTextureImp.cpp
//  orange_client
//
//  Created by Kevin on 14/10/28.
//
//

#include "compressed_texture.h"

#include "render_config.h"
#include "render_device.h"

#include "util/log_tool.h"
#include "util/file_tool.h"

#include "cstdmf/guard.hpp"

#ifdef USE_ETC
#include "texture/etc1.h"
#endif

#ifdef USE_PVRTC
#define PVRTC_ARGB_4444     0x00000000
#define PVRTC_ARGB_1555     0x00000001
#define PVRTC_RGB_565       0x00000002
#define PVRTC_RGB_555       0x00000003
#define PVRTC_RGB_888       0x00000004
#define PVRTC_ARGB_8888     0x00000005
#define PVRTC_ARGB_8332     0x00000006
#define PVRTC_I_8           0x00000007
#define PVRTC_AI_88         0x00000008
#define PVRTC_1_BPP         0x00000009
#define PVRTC_VY1UY0        0x0000000A
#define PVRTC_Y1VY0U        0x0000000B
#define PVRTC_PVRTC2        0x0000000C
#define PVRTC_PVRTC4        0x0000000D
#define PVRTC_RGBA_4444     0x00000010
#define PVRTC_RGBA_5551     0x00000011
#define PVRTC_RGBA_8888     0x00000012
#define PVRTC_PVRTC2_GL     0x00000018
#define PVRTC_PVRTC4_GL     0x00000019
#define PVRTC_MIPMap        0x00000100
#define PVRTC_Twiddle       0x00000200
#define PVRTC_NormalMap     0x00000400
#endif

namespace ora {
    
    extern int g_texture_counter;
    
#pragma pack(push, 1)
    struct PVR_Header{
        uint32 headerSize;
        uint32 height;
        uint32 width;
        uint32 mipMapCount;
        uint32 flags;
        uint32 textureDataSize;
        uint32 bitCount;
        uint32 RBitMask;
        uint32 GBitMask;
        uint32 BBitMask;
        uint32 ABitMask;
        uint32 pvrTag;
        uint32 numSurfs;
    };
#pragma pack(pop)
    
    CompressedTextureImp::CompressedTextureImp()
    {
    
    }
    
    CompressedTextureImp::~CompressedTextureImp()
    {
    }
    
    bool CompressedTextureImp::load(const std::string &filename)
    {
        resource_ = filename;
        
        std::string buffer;
        
#if defined(USE_PVRTC)
        std::string name = removeFileExt(filename) + ".pvr";
#elif defined(USE_ETC)
        std::string name = removeFileExt(filename) + ".etc";
#else
        std::string name = filename;
#endif
        if (!readFile(buffer, name, true))
        {
            ORA_LOG_ERROR("Failed to open texture file '%s'", name.c_str());
            return false;
        }
        
#if defined(USE_PVRTC)
        return loadPVR(buffer);
#elif defined(USE_ETC)
        return loadETC(buffer);
#else
        return false;
#endif
    }


    bool CompressedTextureImp::loadPVR(const std::string & buffer)
    {
        bool ret = false;
#if defined(USE_PVRTC)
        const char * pData = buffer.c_str();
        size_t dataLen = buffer.size();
                
        // read the header size
        uint32 headerSize;
        if(dataLen < sizeof(headerSize))
            return false;
        memcpy(&headerSize, pData, sizeof(headerSize));
        
        // read pvr header
        if(dataLen < headerSize || sizeof(PVR_Header) < headerSize)
            return false;
            
        PVR_Header header;
        memset(&header, 0, sizeof(header)); // headerSize may less than sizeof(PVR_Header).
        memcpy(&header, pData, headerSize);
        pData += headerSize;

        // check the data length is valid.
        if (dataLen < header.headerSize + header.textureDataSize )
            return false;
            
        bool hasAlpha = header.ABitMask != 0;

        TextureFormat format = TextureFormat::Unknown;

        header.flags &= ~PVRTC_MIPMap;
        header.flags &= ~PVRTC_Twiddle;
        header.flags &= ~PVRTC_NormalMap;

        switch (header.flags) {
        case PVRTC_PVRTC4:
            format = hasAlpha ? TextureFormat::PVRTC4BPP_A : TextureFormat::PVRTC4BPP;
            break;
        case PVRTC_PVRTC2:
            format = hasAlpha ? TextureFormat::PVRTC2BPP_A : TextureFormat::PVRTC2BPP;
            break;
        case PVRTC_PVRTC4_GL:
            format = TextureFormat::PVRTC4BPP_A;
            break;
        case PVRTC_PVRTC2_GL:
            format = TextureFormat::PVRTC2BPP_A;

        default:
            format = TextureFormat::Unknown;
            break;
        }

        if (format != TextureFormat::Unknown)
        {
            ret = create(header.mipMapCount, header.width, header.height, format, pData, header.textureDataSize);
        }
#endif
        return ret;
    }

    bool CompressedTextureImp::loadETC(const std::string & buffer)
    {
        bool ret = false;
#ifdef USE_ETC
        const unsigned char *data = reinterpret_cast<const unsigned char *>(buffer.data());
        const etc1_byte* header = static_cast<const etc1_byte*>(data);

        //check the data
        if (!etc1_pkm_is_valid(header))
        {
            return  false;
        }

        width_ = etc1_pkm_get_width(header);
        height_ = etc1_pkm_get_height(header);

        if (0 == width_ || 0 == height_)
        {
            return false;
        }

        //old opengl version has no define for GL_ETC1_RGB8_OES, add macro to make compiler happy. 

        format_ = TextureFormat::ETC;
        const unsigned char *pPixelData = data + ETC_PKM_HEADER_SIZE;
        size_t dataSize = buffer.size() - ETC_PKM_HEADER_SIZE;
        ret = create(0, width_, height_, format_, pPixelData, dataSize);
#endif
        return ret;
    }

    void CompressedTextureImp::doLoading()
    {
        BW_GUARD;
        ASSERT_1(pLoadingInfo_ != nullptr);

        GLenum internalFormat = GLenum(format_);
        GLenum type = GL_UNSIGNED_BYTE;

        GL_ASSERT(glGenTextures(1, &handle_));

        if (pLoadingInfo_->pPixelData)
        {
            GL_ASSERT(glBindTexture(GL_TEXTURE_2D, handle_));
            GL_ASSERT(glCompressedTexImage2D(GL_TEXTURE_2D,
                pLoadingInfo_->levels,
                internalFormat,
                width_,
                height_,
                0,
                pLoadingInfo_->size,
                pLoadingInfo_->pPixelData));

            delete [] pLoadingInfo_->pPixelData;
        }

        delete pLoadingInfo_;
        pLoadingInfo_ = nullptr;
    }

    void CompressedTextureImp::updateParameter()
    {
        if (!parameterDirty_ || !handle_) return;
        parameterDirty_ = false;

        GLenum target = GLenum(target_);

#ifndef _RELEASE

        GLint handle = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &handle);
        ASSERT_1(handle == handle_);

#endif

        TextureQuality quality = quality_;
        if (quality == TextureQuality::Default)
            quality = RenderConfig::instance()->getTextureQuality();

        switch (quality)
        {
        case TextureQuality::Nearest:
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;

        case TextureQuality::Linear:
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;

        case TextureQuality::TwoLinear:
        case TextureQuality::ThreeLinear:
        case TextureQuality::FourLinear:
        case TextureQuality::Anisotropic:
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
            
        default:
            break;
        };

        glTexParameteri(target, GL_TEXTURE_WRAP_S, GLenum(uwrap_));
        glTexParameteri(target, GL_TEXTURE_WRAP_T, GLenum(vwrap_));
    }
    
    
}; // end of namespace ora
