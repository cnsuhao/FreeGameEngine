//
//  progress_bar.cpp
//  liborange
//
//  Created by Kevin on 14-2-21.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#include "progress_bar.h"
#include "graphics/vertex.h"

namespace ora{

	ProgressReference::ProgressReference():
		mtl_(nullptr),
		BillBoard()
	{
		uint16 base[6] = {0, 2, 1, 0, 3, 2};
		for ( int i = 0; i < sizeof(base) / sizeof(uint16); ++i )
		{
			indices.push_back(base[i]);
		}	
	}

	ProgressReference::~ProgressReference()
	{
	
	}
	
	bool ProgressReference::genVertices(ProgressBarVertices& vertices)
	{
		float w = width_;
		float h = height_;
		float lt = w * -.5f;

		float lu = 0.f;
		float ru = 1.f;
		float tv = 0.f;
		float bv = 1.f;
            
		float y = height_ * -.5f;
		float z = 0.f;
            
        VertexFormat vertex;
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

		return true;
	}
	
	bool ProgressReference::genMesh(ProgressBarVertices vertices)
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

		setMesh(mesh);

		return true;
	}

	
	void ProgressReference::setMaterial(const std::string & name)
	{
		if ( !mtl_ )
		{
			mtl_ = new Material();
		}

		mtl_->loadFromFile("shaders/material/a1.mtl", name);

		TexturePtr texture = mtl_->getTextureStage(0).getTexture();
		width_ = (float)texture->getWidth();
		height_ = (float)texture->getHeight();

		ProgressBarVertices vertices;

		if ( genVertices(vertices) )
			genMesh(vertices);
	}

	MaterialPtr ProgressReference::getMaterial() const
	{
		return mtl_;
	}

	void ProgressReference::draw()
	{
		progressReferenceToken = this->range_;
		BillBoard::draw();
	}

}; // end of namespace ora