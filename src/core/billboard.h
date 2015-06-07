//
//  billboard.h
//  liborange
//
//  Created by Kevin on 14-9-26.
//
//

#ifndef __liborange_billboard__
#define __liborange_billboard__

#include "scene_node.h"
#include "graphics/material.h"
#include "graphics/mesh.h"

namespace ora{

	class BillBoard : public SceneNode
	{
	public:
		BillBoard();
		virtual ~BillBoard();

		virtual void draw();

		void setMesh(MeshPtr mesh) { meshPtr_ = mesh; }
		MeshPtr getMesh() const{ return meshPtr_; }

	protected:
		float		width_;
		float		height_;

	private:
		MeshPtr		meshPtr_;
	};

}; // end of namespace ora

#endif // __liborange_billboard__