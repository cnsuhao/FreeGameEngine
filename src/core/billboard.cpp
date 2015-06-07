//
//  billboard.h
//  liborange
//
//  Created by Kevin on 14-9-26.
//
//

#include "billboard.h"
#include "graphics/render_device.h"
#include "world.h"

namespace ora{

	BillBoard::BillBoard() :
		width_(0.f)
		, height_(0.f)
	{
	
	}

	BillBoard::~BillBoard()
	{
	
	}

	void BillBoard::draw()
	{
		SceneNode::draw();

		if (meshPtr_)
		{
#if 0
			const Matrix & worldMatrix = getWorldMatrix();
#else
			Matrix worldMatrix;
			Quaternion rotation;
			Vector3 scale;
			Vector3 translate;
			getWorldMatrix().decompose(rotation, scale, translate);
			worldMatrix.setScale(scale);
			worldMatrix.setTranslate(translate);
#endif
			Viewport viewPort = renderDev()->getViewPort();
			uint32 screenWidth = viewPort.width_;
			uint32 screenHeight = viewPort.height_;

			float scaleX = width_ / screenWidth;
			float scaleY = height_ / screenHeight;

			Vector3 targetPos = worldMatrix.translation();
			Vector3 cameraPos = World::getInstance()->getCamera()->getPosition();

			float fov = renderDev()->getCamera().getFov();
			float aspect = renderDev()->getCamera().getAspect();

			float length = (targetPos - cameraPos).length();
			float nearH = tanf(fov * 0.5f) * length * 2;

			float heightInZn = scaleY * nearH;
			float widthInZn = scaleX * nearH * aspect;

			Matrix mat;
			mat.setScale(Vector3(widthInZn / width_, heightInZn / height_, 1.f));

			mat.postMultiply(worldMatrix);
			mat.translation(Vector3::zero());

			Matrix invView = renderDev()->getInvView();
			invView.translation(Vector3::zero());

			mat.postMultiply(invView);
			mat.translation(worldMatrix.translation());

			renderDev()->pushWorld(mat);
			meshPtr_->draw();
			renderDev()->popWorld();
		}
	}

}; // end of namespace ora
