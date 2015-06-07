
#include "render_helper.h"
#include "render_device.h"
#include "mesh_tool.h"
#include "effect_mgr.h"

namespace ora
{
    IMPLEMENT_SINGLETON(RenderHelper);

    RenderHelper::RenderHelper()
    {
    }

    RenderHelper::~RenderHelper()
    {
    }

    MeshPtr RenderHelper::getCubMesh()
    {
        if (!cubMesh_)
        {
            cubMesh_ = createCubeFrame(Vector3(0.5f, 0.5f, 0.5f), Color(1, 1, 1, 1));
        }
        return cubMesh_;
    }

    void RenderHelper::drawAABB(const AABB & aabb, uint32 color)
    {
        MeshPtr mesh = getCubMesh();

        Matrix matTrans;
        matTrans.setTranslate(aabb.getCenter());

        Matrix matScale;
        matScale.setScale(aabb.getDiameter());

        matTrans.preMultiply(matScale);

        renderDev()->pushWorld(matTrans);
        mesh->draw();
        renderDev()->popWorld();
    }

    void RenderHelper::drawLine(const Vector3 & start, const Vector3 & end, uint32 color)
    {
        //if(almostEqual(start, end)) return;

        VertexXYZColor vertices [] = {
            {start, color},
            {end, color},
        };

        drawLines(vertices, 2);
    }

    void RenderHelper::drawLines(const std::vector<VertexXYZColor> & vertices)
    {
        drawLines(vertices.data(), vertices.size());
    }

    void RenderHelper::drawLines(const VertexXYZColor* vertices, size_t size)
    {
        if(size < 2) return;

        if(!lineMesh_)
        {
            lineMesh_ = new Mesh();
            
            auto buffer = new VertexBufferEx<VertexXYZColor>(
                BufferUsage::Dynamic, size, vertices);
            lineMesh_->setVertexBuffer(buffer);

            auto decl = VertexDeclMgr::instance()->get(VertexXYZColor::getType());
            lineMesh_->setVertexDecl(decl);

            ora::MaterialPtr mtl = new ora::Material();
            mtl->setEffect(ora::EffectMgr::instance()->get("shaders/color_only.glsl"));
            int mtlID = lineMesh_->addMaterial(mtl);

            SubMeshPtr subMesh = new SubMesh();
            subMesh->setPrimitive(PrimitiveType::LineList, 0, size, mtlID, false);
            lineMesh_->addSubMesh(subMesh);
        }
        else
        {
            auto buffer = lineMesh_->getVertexBuffer();
            buffer->resize(size, vertices);

            SubMeshPtr subMesh = lineMesh_->getSubMesh(0);
            subMesh->setPrimitive(PrimitiveType::LineList, 0, size, 0, false);
        }

        lineMesh_->draw();
    }

    void RenderHelper::drawTriangle(const Vector3 & a, const Vector3 & b, const Vector3 & c, uint32 color)
    {
        std::vector<VertexXYZColor> vertices;

        VertexXYZColor vertex;
        vertex.color.setARGB(color);

        vertex.position = a;
        vertices.push_back(vertex);

        vertex.position = b;
        vertices.push_back(vertex);

        vertex.position = c;
        vertices.push_back(vertex);

        drawTriangles(vertices);
    }

    void RenderHelper::drawTriangles(const std::vector<VertexXYZColor> & vertices)
    {
        if(vertices.empty()) return;

        if(!triangleMesh_)
        {
            triangleMesh_ = new Mesh();
            
            auto buffer = new VertexBufferEx<VertexXYZColor>(
                BufferUsage::Dynamic, vertices.size(), &vertices[0]);
            triangleMesh_->setVertexBuffer(buffer);

            auto decl = VertexDeclMgr::instance()->get(VertexXYZColor::getType());
            triangleMesh_->setVertexDecl(decl);

            ora::MaterialPtr mtl = new ora::Material();
            mtl->setEffect(ora::EffectMgr::instance()->get("shaders/color_only.glsl"));
            int mtlID = triangleMesh_->addMaterial(mtl);

            SubMeshPtr subMesh = new SubMesh();
            subMesh->setPrimitive(PrimitiveType::TriangleList, 0, vertices.size(), mtlID, false);
            triangleMesh_->addSubMesh(subMesh);
        }
        else
        {
            auto buffer = triangleMesh_->getVertexBuffer();
            buffer->resize(vertices.size(), &vertices[0]);

            SubMeshPtr subMesh = lineMesh_->getSubMesh(0);
            subMesh->setPrimitive(PrimitiveType::TriangleList, 0, vertices.size(), 0, false);
        }

        triangleMesh_->draw();
    }

} // end namespace ora