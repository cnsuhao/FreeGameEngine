//
//  gizmo.cpp
//  util
//
//  Created by Kevin on 15-1-5.
//
//

#include "gizmo.h"
#include "world.h"
#include "graphics/vertex.h"
#include "graphics/render_device.h"

namespace ora{

	Gizmo::Gizmo():
		xlength_(0.f),
		ylength_(0.f),
		zlength_(0.f)
	{}

	Gizmo::~Gizmo()
	{}

	void Gizmo::draw()
	{
		SceneNode::draw();
		Matrix mat = getWorldMatrix();
		renderDev()->pushWorld(mat);
		meshPtr_->draw();
		renderDev()->popWorld();
	}

	bool Gizmo::genVertices(Gizmo::GizmoVertices& vertices, float xlength, float ylength, float zlength, uint32 color)
	{	
		vertices[0].position = Vector3(0.0f, ylength, 0.0f);
		vertices[1].position = Vector3(0.0f, 0.0f, 0.0f);
		vertices[2].position = Vector3(xlength, 0.0f, 0.0f);
		vertices[3].position = Vector3(xlength, ylength, 0.0f);
		
		vertices[4].position = Vector3(0.0f, ylength, zlength);
		vertices[5].position = Vector3(0.0f, 0.0f, zlength);
		vertices[6].position = Vector3(xlength, 0.0f, zlength);
		vertices[7].position = Vector3(xlength, ylength, zlength);

		for(size_t i = 0; i < 8; ++i)
		{
			vertices[i].color = color;
		}

		return true;
	}

	bool Gizmo::genMesh(Gizmo::GizmoVertices vertices)
	{
		MeshPtr mesh = new Mesh();
        
        auto vb = new VertexBufferEx<Gizmo::VertexFormat>(
                                                      BufferUsage::Static,
                                                      vertices.size(),
                                                      &vertices[0]
                                                      );
        
        auto decl = VertexDeclMgr::instance()->get(Gizmo::VertexFormat::getType());
		mesh->setVertexDecl(decl);
        
		MaterialPtr mtl = new Material();
		mtl->loadFromFile("shaders/material/a1.mtl", "color_only");
		int mtlID = mesh->addMaterial(mtl);
        
		uint16 indices[] =
		{
			// front
			0,1,3,
			3,1,2,

			// right
			3,2,7,
			7,2,6,

			// back
			4,7,6,
			4,6,5,

			// left
			4,5,0,
			0,5,1,

			// top
			0,4,7,
			7,0,2,

			// bottom
			5,1,6,
			6,1,2,
		};

        IndexBufferPtr ib = new IndexBufferEx<uint16>(
                                                      BufferUsage::Static,
                                                      sizeof(indices)/sizeof(uint16),
                                                      &indices[0]
                                                      );
        SubMeshPtr subMesh = new SubMesh();
        subMesh->setPrimitive(PrimitiveType::TriangleList, 0, sizeof(indices)/sizeof(uint16), mtlID, true);
		mesh->addSubMesh(subMesh);
        
		mesh->setVertexBuffer(vb);
		mesh->setVertexDecl(decl);
		mesh->setIndexBuffer(ib);

		meshPtr_ = mesh;
		return true;
	}

	void Gizmo::createGizmo(float xlength, float ylength, float zlength, uint32 color)
	{
		xlength_ = xlength; ylength_ = ylength; zlength_ = zlength;
		Gizmo::GizmoVertices vertices;
		for(size_t i = 0; i < 9; ++i)
		{
			vertices.push_back(Gizmo::VertexFormat());
		}

		if (!genVertices(vertices, xlength, ylength, zlength, color))
			return;

        genMesh(vertices);
	}

	BoundingBox Gizmo::localBoundingBox()
	{
		BoundingBox bb;

		Vector3 minBound(0.f, 0.f, 0.f);
		Vector3 maxBound(xlength_, ylength_, zlength_);

		bb.setBounds(minBound, maxBound);
		return bb;
	}

}; // end of namespace ora