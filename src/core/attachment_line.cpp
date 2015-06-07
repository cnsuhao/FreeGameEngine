//
//  attackment_line.cpp
//  orange_client
//
//  Created by Kevin on 14-11-11.
//
//

#include "attachment_line.h"
#include "graphics/render_device.h"
#include "graphics/effect_mgr.h"
#include "world.h"

namespace ora{

	AttachmentLine::AttachmentLine()
		:lineMesh_(nullptr)
	{
	
	}

	AttachmentLine::~AttachmentLine()
	{
	
	}

	void AttachmentLine::addVertex(const Vector3 & vertex, uint32 color)
	{
		VertexFormat v;
		v.position = vertex;
		v.color = color;
		verties_.push_back(v);
	}

	void AttachmentLine::clearVertex()
	{
		verties_.clear();
	}

	void AttachmentLine::display()
	{
		if (lineMesh_)
			return;

		lineMesh_ = new Mesh();

		auto buffer = new VertexBufferEx<VertexFormat>(
			BufferUsage::Dynamic,
			verties_.size(),
			&verties_[0]
		);
		lineMesh_->setVertexBuffer(buffer);

		auto decl = VertexDeclMgr::instance()->get(VertexFormat::getType());
		lineMesh_->setVertexDecl(decl);
		
		MaterialPtr mtl = new Material();
		mtl->setEffect(EffectMgr::instance()->get("shaders/color_only.glsl"));
		int mtlID = lineMesh_->addMaterial(mtl);

		SubMeshPtr subMesh = new SubMesh();
		subMesh->setPrimitive(PrimitiveType::LineList, 0, verties_.size(), mtlID, false);
		lineMesh_->addSubMesh(subMesh);

		World::instance()->getRoot()->addChild(this);

	}

	void AttachmentLine::undisplay()
	{
		World::instance()->getRoot()->removeChild(this);
		//lineMesh_ = nullptr;
	}

	void AttachmentLine::draw()
	{
		if (!lineMesh_)
			return;

		SceneNode::draw();

		Matrix mat = getWorldMatrix();
		renderDev()->pushWorld(mat);
		lineMesh_->draw();
		renderDev()->popWorld();
	}

}; // end of namespace ora