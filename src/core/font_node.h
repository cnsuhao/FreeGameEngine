//
//  FontNode.h
//  orange_client
//
//  Created by Kevin on 14-9-16.
//
//

#ifndef __orange_client__FontNode__
#define __orange_client__FontNode__

#include "scene_node.h"
#include "graphics/material.h"
#include "graphics/mesh.h"

namespace ora{

    typedef SmartPtr<class Mesh> MeshPtr;
    
    struct VertexXYZUV;
    typedef VertexXYZUV VertexFormat;
    typedef std::vector<VertexFormat> FontNodeVertices;
    typedef std::vector<uint16> FontNodeIndices;
    
    typedef SmartPtr<class FontNode> FontNodePtr;
    
	class FontNode : public SceneNode
    {
    public:
        FontNode();
        ~FontNode();
        
        virtual void draw();
        
        static FontNode* create();
        void target(SceneNode* t);
		SceneNode* target() const { return target_; }

        void setText(const std::string & text, const std::string & font);
        
    protected:
        bool genVertices(const std::string& text,
						const std::string& font,
						FontNodeVertices& vertices,
                        FontNodeIndices & indices);
        bool genMesh(FontNodeVertices vertices, FontNodeIndices & indices);
        
    private:
		std::string	font_;
		MaterialPtr	mtl_;
		SceneNode*	target_;

		MeshPtr		meshPtr_;
    };
    
	struct FontNodeInfo{
		float w;
		float h;
		float lu;
		float ru;
		float tv;
		float bv;
		
		FontNodeInfo()
		{
			w = h = lu = ru = tv = bv = 0.f;
		}

		FontNodeInfo(float _w, float _h, float _lu, float _ru, float _tv, float _bv)
		{
			w = _w; h = _h; lu = _lu; ru = _ru; tv = _tv; bv = _bv;
		}

		std::size_t index;
	};
	typedef std::map<uint32, FontNodeInfo> FontConfig;
	typedef std::map<std::string, FontConfig*> FontNodePool;

	class FontNodeMgr : public ora::Singleton<FontNodeMgr>
	{
	public:
		FontNodeMgr();
		~FontNodeMgr();

		FontConfig*	getFontNodeConfig(const std::string & font);

	private:
		FontNodePool	fontNodePool_; 
	};

}; // end of namespace ora

#endif /* defined(__orange_client__FontNode__) */
