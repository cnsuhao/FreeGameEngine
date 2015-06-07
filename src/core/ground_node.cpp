//
//  root_scene.cpp
//  client
//
//  Created by zhoubao on 14-9-23.
//
//

#include "ground_node.h"
#include "graphics/render_device.h"
#include "graphics/vertex.h"

namespace ora
{

    GroundNode::GroundNode()
    {}

    GroundNode::~GroundNode()
    {
    }

    void GroundNode::setBackGround(const std::string & material, const std::string & key)
    {
        backGroundMtr_ = material;
        backGroundKey_ = key;
        background_ = nullptr;

        MaterialPtr mtl = new Material();
        if (!mtl->loadFromFile(material, key))
            return;

        if (mtl->getNumTextureStage() == 0)
            return;

        TexturePtr tex = mtl->getTextureStage(0).getTexture();
        if (!tex)
            return;

        //计算等比缩放
        uint32 width = tex->getWidth();
        uint32 height = tex->getHeight();
        float aspectTex = float(width) / float(height);
        float aspectVp = renderDev()->getViewPort().aspect();

        float w, h;
        if (aspectTex > aspectVp)
        {
            w = 1.0f;
            h = aspectVp / aspectTex;
        }
        else
        {
            h = 1.0f;
            w = aspectTex / aspectVp;
        }

        float z = 0.0f;
        VertexXYZUV vertices [] =
        {
            { Vector3(-w,  h, z), Vector2(0, 0) },
            { Vector3( w,  h, z), Vector2(1, 0) },
            { Vector3(-w, -h, z), Vector2(0, 1) },
            { Vector3( w, -h, z), Vector2(1, 1) },
        };
        VertexBufferPtr vb = new VertexBufferEx<VertexXYZUV>(
            BufferUsage::Static, 4, vertices);

        VertexDeclarationPtr decl = VertexDeclMgr::instance()->get(VertexXYZUV::getType());

        SubMeshPtr subMesh = new SubMesh();
        subMesh->setPrimitive(PrimitiveType::TriangleStrip, 0, 4, 0, false);

        background_ = new Mesh();
        background_->setVertexBuffer(vb);
        background_->setVertexDecl(decl);
        background_->addMaterial(mtl);
        background_->addSubMesh(subMesh);
    }

    void GroundNode::draw()
    {
        if (background_)
        {
            renderDev()->pushWorld(getWorldMatrix());
            background_->draw();
            renderDev()->popWorld();
        }

        SceneNode::draw();
    }

} // end namespace ora
