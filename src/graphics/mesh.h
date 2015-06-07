#ifndef H__MESH_H
#define H__MESH_H

#include "vertex_buffer.h"
#include "vertex_declaration.h"
#include "material.h"
#include "aabb.h"

namespace ora
{
    typedef SmartPtr<class Mesh> MeshPtr;

    class SubMesh : public IReferenceCount
    {
    public:
        SubMesh();
        ~SubMesh();

        void draw();

        int getMaterial() const { return mtlID_; }

        /** 设置图元数据
         *  useIndex表示是否使用索引缓冲区绘制。
         *  useIndex为true，则start表示索引buffer起始位置，count表示索引的个数。
         *  useIndex为false，则start表示顶点buffer起始位置，count表示顶点个数。
        */
        void setPrimitive(PrimitiveType pt,
            uint32 start, uint32 count, int mtlID,
            bool useIndex=true);

    private:
        uint32          start_;
        uint32          count_;
        PrimitiveType   primitiveType_;
        int             mtlID_;
        bool            useIndex_;
    };
    typedef SmartPtr<SubMesh> SubMeshPtr;


    class Mesh : public IRes
    {
    public:
        typedef std::vector<MaterialPtr> Materials;
        typedef std::vector<SubMeshPtr> SubMeshes;

        Mesh();
        ~Mesh();

        bool load(const std::string & resource) override;
        
        MeshPtr clone();
        bool isEctypal() const;
        MeshPtr getSource() const { return source_; }

        void draw();

        void setVertexBuffer(VertexBufferPtr vertex);
        void setIndexBuffer(IndexBufferPtr index);
        void setVertexDecl(VertexDeclarationPtr decl);

        const SubMeshes & getSubMeshes() const;
        void setSubMeshes(const SubMeshes & subMeshes);
        void addSubMesh(SubMeshPtr subMesh);
        size_t getNbSubMesh() const;
        SubMeshPtr getSubMesh(size_t i) const;

        MaterialPtr getMaterial(int mtlID) const;
        const Materials & getMaterials() const { return materials_; }
        Materials & getMaterials() { return materials_; }
        void setMaterials(const Materials & materials);
        int addMaterial(MaterialPtr mtl);

        const std::string & getResource() const
        {
            return resource_;
        }
        void setResource(const std::string & name)
        {
            resource_ = name;
        }

        const AABB & getAABB() const
        {
            return aabb_;
        }

        VertexBufferPtr getVertexBuffer() const
        {
            return vertexBuffer_;
        }
        VertexDeclarationPtr getVertexDecl() const
        {
            return vertexDecl_;
        }
        IndexBufferPtr getIndexBuffer() const
        {
            return indexBuffer_;
        }

    private:
        MeshPtr                 source_;
        std::string             resource_;
        VertexDeclarationPtr    vertexDecl_;
        VertexBufferPtr         vertexBuffer_;
        IndexBufferPtr          indexBuffer_;
        SubMeshes               subMeshs_;
        Materials               materials_;
        AABB                    aabb_;
    };



}//end namespace ora

#endif //H__MESH_H