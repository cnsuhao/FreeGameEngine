//
//  gizmo.h
//  util
//
//  Created by Kevin on 15-1-5.
//
//

#include "scene_node.h"
#include "graphics/mesh.h"

namespace ora{
	
	struct VertexXYZColor;

    typedef SmartPtr<class Gizmo> GizmoPtr;
	
	class Gizmo : public SceneNode
	{
	public:
		typedef VertexXYZColor VertexFormat;
		typedef std::vector<VertexFormat> GizmoVertices;

		Gizmo();
		virtual ~Gizmo();

        virtual void draw();
		virtual void createGizmo(float xlength, float ylength, float zlength, uint32 color);
		
		BoundingBox localBoundingBox();

    protected:
        virtual bool genVertices(GizmoVertices& vertices, float xlength, float ylength, float zlength, uint32 color);
        virtual bool genMesh(GizmoVertices vertices);

	private:
		MeshPtr		meshPtr_;
		float		xlength_, ylength_, zlength_;
	};

}; // end of namespace ora