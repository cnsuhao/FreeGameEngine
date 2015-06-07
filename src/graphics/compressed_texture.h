//
//  CompressedTexture.h
//  orange_client
//
//  Created by Kevin on 14/10/28.
//
//

#ifndef __orange_client__CompressedTexture__
#define __orange_client__CompressedTexture__

#include "texture.h"

namespace ora{

    class CompressedTextureImp : public Texture
    {
    public:
        CompressedTextureImp();
        virtual ~CompressedTextureImp();
        
        virtual bool load(const std::string & filename);

    private:
        void updateParameter();

        virtual void doLoading() override;
        bool loadPVR(const std::string & data);
        bool loadETC(const std::string & data);
    };

#if defined(USE_PVRTC) || defined(USE_ETC)
    typedef CompressedTextureImp CompressedTexture;
#else
	typedef Texture CompressedTexture;
#endif
	typedef SmartPtr<CompressedTexture> CompressedTexturePtr;
    
}; // end of namespace ora;

#endif /* defined(__orange_client__CompressedTexture__) */
