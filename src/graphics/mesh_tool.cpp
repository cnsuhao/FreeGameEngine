#include "mesh_tool.h"
#include "vertex.h"
#include "effect_mgr.h"
#include "texture_mgr.h"

namespace ora
{

    MeshPtr createCube(const Vector3 & radius, const std::string & shader, const std::string & texture)
    {
        MeshPtr mesh = new Mesh();

        const int numVertices = 8;
        VertexXYZUV vertices[numVertices];

        vertices[0].position.set(-radius.x, -radius.y, radius.z);
        vertices[0].uv.set(0.0f, 1.0f);

        vertices[1].position.set(-radius.x, radius.y, radius.z);
        vertices[1].uv.set(0.0f, 0.0f);

        vertices[2].position.set(radius.x, radius.y, radius.z);
        vertices[2].uv.set(1.0f, 0.0f);

        vertices[3].position.set(radius.x, -radius.y, radius.z);
        vertices[3].uv.set(1.0f, 1.0f);

        for (int i = 0; i < 4; ++i)
        {
            vertices[i + 4] = vertices[i];
            vertices[i + 4].position.z = -radius.z;
        }

        vertices[4].uv.set(1, 1);
        vertices[5].uv.set(1, 0);
        vertices[6].uv.set(0, 0);
        vertices[7].uv.set(0, 1);

        VertexBufferPtr vb = new VertexBufferEx<VertexXYZUV>(
            BufferUsage::Static, numVertices, &vertices[0]);

        VertexDeclarationPtr decl = VertexDeclMgr::instance()->get(VertexXYZUV::getType());

        //create index buffer

        const int numIndices = 6 * 2 * 3;
        uint16 indices[numIndices] =
        {
            0, 2, 1, 0, 3, 2, //front
            3, 6, 2, 3, 7, 6, //right
            7, 5, 6, 7, 4, 5, //back
            4, 1, 5, 4, 0, 1, //left
            1, 6, 5, 1, 2, 6, //top
            4, 3, 0, 4, 7, 3, //bottom
        };

        IndexBufferPtr ib = new IndexBufferEx<uint16>(
            BufferUsage::Static, numIndices, &indices[0]);

        //create material

        MaterialPtr mtl = new Material();
        mtl->setEffect(EffectMgr::instance()->get(shader));

        TextureStage stage;
        stage.setTexture(CompressedTextureMgr::instance()->get(texture));
        stage.setUWrap(TextureWrap::Repeat);
        stage.setVWrap(TextureWrap::Repeat);
        mtl->addTextureStage(stage);

        //create a sub mesh

        SubMeshPtr subMesh = new SubMesh();
        int mtlID = mesh->addMaterial(mtl);
        subMesh->setPrimitive(PrimitiveType::TriangleList, 0, numIndices, mtlID);
        mesh->addSubMesh(subMesh);

        //create mesh

        mesh->setVertexBuffer(vb);
        mesh->setVertexDecl(decl);
        mesh->setIndexBuffer(ib);

        return mesh;
    }

    /////////////////////////////////////////////////////////////////////

    MeshPtr createCubeFrame(const Vector3 & radius,
                                  const Color & cr,
                                  const std::string & shader)
    {
        MeshPtr mesh = new Mesh();

        typedef VertexXYZColor VertexType;

        const int numVertices = 8;
        VertexType vertices[numVertices];

        vertices[0].position.set(-radius.x, -radius.y, radius.z);
        vertices[1].position.set(-radius.x, radius.y, radius.z);
        vertices[2].position.set(radius.x, radius.y, radius.z);
        vertices[3].position.set(radius.x, -radius.y, radius.z);

        for (int i = 0; i < 4; ++i)
        {
            vertices[i + 4] = vertices[i];
            vertices[i + 4].position.z = -radius.z;
        }

        for (int i = 0; i < numVertices; ++i)
        {
            vertices[i].color = cr;
        }

        VertexBufferPtr vb = new VertexBufferEx<VertexType>(
            BufferUsage::Static, numVertices, &vertices[0]);

        VertexDeclarationPtr decl = VertexDeclMgr::instance()->get(VertexType::getType());

        //create index buffer

        const int numIndices = 12 * 2;
        uint16 indices[numIndices] =
        {
            0, 1, 1, 2, 2, 3, 3, 0,
            4, 5, 5, 6, 6, 7, 7, 4,
            1, 5, 0, 4, 2, 6, 3, 7,
        };

        IndexBufferPtr ib = new IndexBufferEx<uint16>(
            BufferUsage::Static, numIndices, &indices[0]);

        //create material

        MaterialPtr mtl = new Material();
        mtl->setEffect(EffectMgr::instance()->get(shader));

        //create a sub mesh

        SubMeshPtr subMesh = new SubMesh();
        mesh->addSubMesh(subMesh);
        int mtlID = mesh->addMaterial(mtl);
        subMesh->setPrimitive(PrimitiveType::LineList, 0, numIndices, mtlID);

        //create mesh

        mesh->setVertexBuffer(vb);
        mesh->setVertexDecl(decl);
        mesh->setIndexBuffer(ib);

        return mesh;
    }


}//end namespace ora