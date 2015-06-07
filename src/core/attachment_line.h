//
//  attackment_line.cpp
//  orange_client
//
//  Created by Kevin on 14-11-11.
//
//

#include "scene_node.h"
#include "graphics/vertex.h"
#include "graphics/mesh.h"

namespace ora{

	typedef SmartPtr<class AttachmentLine> AttachmentLinePtr;
	class AttachmentLine : public SceneNode
	{
	public:
		AttachmentLine();
		virtual ~AttachmentLine();

		virtual void addVertex(const Vector3 & vertex, uint32 color);
		virtual void clearVertex();
		virtual void display();
		virtual void undisplay();
		virtual void draw();

	private:
		typedef VertexXYZColor VertexFormat;
		typedef std::vector<VertexFormat> AttachmentLineVertex;

		AttachmentLineVertex	verties_;
		MeshPtr					lineMesh_;

	};

}; // end of namespace ora