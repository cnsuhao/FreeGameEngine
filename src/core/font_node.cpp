//
//  FontNode.cpp
//  orange_client
//
//  Created by Kevin on 14-9-16.
//
//

#include "font_node.h"
#include "graphics/mesh.h"
#include "graphics/vertex.h"
#include "graphics/effect_mgr.h"
#include "graphics/texture_mgr.h"
#include "util/section_factory.h"
#include "graphics/render_device.h"
#include "action_alpha.h"
#include "action_container.h"
#include "action_motion.h"
#include "world.h"
#include "resmgr/bwresource.hpp"

namespace ora{
    
    /*static*/ FontNode* FontNode::create()
    {
        return new FontNode();
    }
    
	FontNode::FontNode() :target_(nullptr)
	{
		mtl_ = new Material();
		mtl_->loadFromFile("shaders/material/a1.mtl", "font");
    }
    
    FontNode::~FontNode()
    {
    
    }
    
    bool FontNode::genVertices(
		const std::string& text,
		const std::string& font,
		FontNodeVertices& vertices,
        FontNodeIndices & indices)
    {
        std::size_t size = text.size();

		FontConfig* fontConfig = FontNodeMgr::instance()->getFontNodeConfig(font);

		if ( !fontConfig )
			return false;

		float width = 0.f;
		for (size_t pos = 0; pos < size; ++pos)
		{
			char s = text[pos];
			FontConfig::iterator iter = fontConfig->find(int(s));
			if (iter == fontConfig->end())
				continue;
			FontNodeInfo info = (*iter).second;
			width += info.w;
		}
		
		float wk = 2 / float(renderDev()->getViewPort().width_);
		float hk = 2 / float(renderDev()->getViewPort().height_);
		float left = wk * width * -0.5f;
        
        for (size_t pos = 0; pos < size; ++pos)
        {    
			char s = text[pos];
			FontConfig::iterator iter = fontConfig->find(s);
			if (iter == fontConfig->end())
				continue;
			FontNodeInfo info = (*iter).second;

            VertexFormat vertex;
			
			float w = info.w * wk;
			float h = info.h * hk;
			float lt = left;

			float lu = info.lu;
			float ru = info.ru;
			float tv = info.tv;
			float bv = info.bv;
            
			float y = 0.f;
            float z = 0.f;
            
            // 0--3
            // |\ |
            // | \|
            // 1--2
            vertex.position = Vector3(lt, y + h, z); // 0
            vertex.uv = Vector2(lu, tv);
            vertices.push_back(vertex);
            
            vertex.position = Vector3(lt, y, z); // 1
            vertex.uv = Vector2(lu, bv);
            vertices.push_back(vertex);
            
            vertex.position = Vector3(lt + w, y, z); // 2
            vertex.uv = Vector2(ru, bv);
            vertices.push_back(vertex);
            
            vertex.position = Vector3(lt + w, y + h, z); // 3
            vertex.uv = Vector2(ru, tv);
            vertices.push_back(vertex);
            
            uint16 base[6] = {0, 2, 1, 0, 3, 2};
            for ( int i = 0; i < sizeof(base) / sizeof(uint16); ++i )
            {
                indices.push_back(base[i] + pos * 4);
            }
            
			left += info.w * wk;
        }
        
		return true;
    }
    
    bool FontNode::genMesh(FontNodeVertices vertices, FontNodeIndices & indices)
    {
        MeshPtr mesh = new Mesh();
        
        auto vb = new VertexBufferEx<VertexFormat>(
                                                      BufferUsage::Static,
                                                      vertices.size(),
                                                      &vertices[0]
                                                      );
        
        auto decl = VertexDeclMgr::instance()->get(VertexFormat::getType());
		mesh->setVertexDecl(decl);
        
        IndexBufferPtr ib = new IndexBufferEx<uint16>(
                                                      BufferUsage::Static,
                                                      indices.size(),
                                                      &indices[0]
                                                      );
        
		int mtlID = mesh->addMaterial(mtl_);
        
        SubMeshPtr subMesh = new SubMesh();
        subMesh->setPrimitive(PrimitiveType::TriangleList, 0, indices.size(), mtlID, true);
		mesh->addSubMesh(subMesh);
        
		mesh->setVertexBuffer(vb);
		mesh->setVertexDecl(decl);
		mesh->setIndexBuffer(ib);

		meshPtr_ = mesh;

		return true;
    }
    
	void FontNode::setText(const std::string & text, const std::string & font)
	{
		if (font_ != font)
		{
			std::string filename = "fonts/" + font + ".font";
			SectionPtr section = SectionFactory::instance()->loadXml(filename);
			if ( section )
			{
				SectionPtr root = section->read("root");
				SectionPtr generatedSection = root->read("generated");
				std::string textureFile = "fonts/" + generatedSection->read("map")->getString();

				TexturePtr tex;

                //try the given path first.
                if(!textureFile.empty() && BWResource::fileExists(textureFile))
                {
                    tex = TextureMgr::instance()->get(textureFile);
                }

				TextureStage & textureStage = mtl_->getTextureStage(0);
                textureStage.setTexture(tex);
				mtl_->setTextureStage(0, textureStage);
			}
			font_ = font;
		}

		FontNodeVertices vertices;
		FontNodeIndices indices;

		if (!genVertices(text, font, vertices, indices))
			return;

        genMesh(vertices, indices);
	}

	void FontNode::draw()
	{
		SceneNode::draw();

        if (meshPtr_)
        {
            actionAlphaToken = color_;

            Matrix mat = getWorldMatrix();
            renderDev()->pushWorld(mat);
            meshPtr_->draw();
            renderDev()->popWorld();
        }
	}
	
	void FontNode::target(SceneNode* t)
	{
		target_ = t;
		Vector4 posInProj;
		renderDev()->getViewProjection().applyPoint( posInProj, t->getPosition() );
		posInProj *= 1.0f / posInProj.w;
		this->setPosition(Vector3(posInProj.x, posInProj.y, posInProj.z));
		World::instance()->getRoot()->addChild(this);
		//target_->addChild(this); 
	}
	////////////////////////////////////////////////////
	//16进制字符串转换成浮点数
	float strHexToFloat(std::string str)
	{
		int f;
		sscanf(str.c_str(), "%x", &f);
		return float(f);
	}
    
	IMPLEMENT_SINGLETON(FontNodeMgr);

	FontNodeMgr::FontNodeMgr()
	{
	
	}

	FontNodeMgr::~FontNodeMgr()
	{
		for ( FontNodePool::value_type value : fontNodePool_ )
		{
			delete value.second;
		}

		fontNodePool_.clear();
	}

	FontConfig* FontNodeMgr::getFontNodeConfig(const std::string & font)
	{
		FontNodePool::iterator iter = fontNodePool_.find(font);
		if ( iter == fontNodePool_.end() )
		{	
			std::string filename = "fonts/" + font + ".font";
			SectionPtr section = SectionFactory::instance()->loadXml(filename);
			if ( section )
			{
				SectionPtr root = section->read("root");
				SectionPtr generatedSection = root->read("generated");
				float h = generatedSection->read("height")->getFloat();

				std::string uvs = generatedSection->read("uvs")->getString();
				std::string widths = generatedSection->read("widths")->getString();
				size_t len = uvs.length()/8 - 1; //最后有一组重复数据

				uint32 startChar = root->read("creation")->read("startChar")->getUint();

				FontConfig * fontCofig = new FontConfig();
				
				uint32 width = generatedSection->read("mapDimensionsWidth")->getUint();
				uint32 height = generatedSection->read("mapDimensionsHeight")->getUint();

				for (size_t i = 0; i < len; ++i)
				{
					float w = strHexToFloat(widths.substr(i*4, 4));
					float x = strHexToFloat(uvs.substr(i*8, 4));
					float y = strHexToFloat(uvs.substr(i*8+4, 4));
					(*fontCofig)[startChar+i] = FontNodeInfo(w, h, x/width, (w+x)/width, y/height, (y+h)/height);
				}

				fontNodePool_[font] = fontCofig;

				return fontCofig;
			}
			else
			{
				return nullptr;
			}
		}
		else
		{
			return iter->second;
		}
	}

}; // end of namespace ora