#include "mesh.h"
#include "render_device.h"
#include "effect_constant.h"
#include "mesh_mgr.h"
#include "vertex.h"

#include "util/section_factory.h"
#include "util/file_tool.h"
#include "util/log_tool.h"
#include "util/quick_file_reader.h"

#include "mesh_structs.h"

namespace ora
{
    namespace
    {
        const uint32 MeshVersion = 1;
        const uint32 MeshMagic = 'm' | 'e' << 8 | 's' << 16 | 'h' << 24;


    }

    SubMesh::SubMesh()
        : start_(0)
        , count_(0)
        , mtlID_(-1)
        , useIndex_(true)
    {

    }

    SubMesh::~SubMesh()
    {

    }

    void SubMesh::setPrimitive(PrimitiveType pt,
        uint32 start, uint32 count, int mtlID,
        bool useIndex)
    {
        primitiveType_ = pt;
        start_ = start;
        count_ = count;
        mtlID_ = mtlID;
        useIndex_ = useIndex;
    }

    void SubMesh::draw()
    {
        if(count_ == 0) return;

        if(useIndex_)
            renderDev()->drawIndexedPrimitive(primitiveType_, start_, count_);
        else
            renderDev()->drawPrimitive(primitiveType_, start_, count_);
    }


    /////////////////////////////////////////////////////////////
    /// Mesh
    /////////////////////////////////////////////////////////////

    Mesh::Mesh()
    {

    }

    Mesh::~Mesh()
    {
    }

    bool Mesh::isEctypal() const
    {
        return bool(source_);
    }

    bool Mesh::load(const std::string & resource)
    {
        resource_ = resource;

        std::string datas;
        if (!readFile(datas, resource_, true))
        {
            ORA_LOG_ERROR("Failed to load file %s", resource_.c_str());
            return false;
        }

        QuickFileReader reader(datas.c_str(), datas.length());

        //read header
        uint32 magic, version;
        reader >> magic >> version;
        if(magic != MeshMagic)
        {
            ORA_LOG_ERROR("Invalid Mesh type! %d", magic);
            return false;
        }
        if(version != MeshVersion)
        {
            ORA_LOG_ERROR("Invalid Mesh version! %d", version);
            return false;
        }

        //read indices
        IndexHeader ih;
        reader >> ih;
        if(strcmp(ih.indexFormat, "list") == 0)
        {
            std::vector<uint16> indices;
            reader.readVector(indices, ih.nIndices);

            indexBuffer_ = new IndexBufferEx<uint16>(BufferUsage::Static, ih.nIndices, &indices[0]);
        }
        else
        {
            std::vector<uint32> indices(ih.nIndices);
            reader.readVector(indices, ih.nIndices);

            indexBuffer_ = new IndexBufferEx<uint32>(BufferUsage::Static, ih.nIndices, &indices[0]);
        }

        //read primitive group
        PGVector primGroups;
        reader.readVector(primGroups, ih.nTriangleGroups);

        //read vertices
        VertexHeader vh;
        reader >> vh;

        typedef VertexXYZNUV MeshVertex;
        vertexDecl_ = VertexDeclMgr::instance()->get(MeshVertex::getType());

        std::vector<MeshVertex> vertices;
        reader.readVector(vertices, vh.nVertices);
        vertexBuffer_ = new VertexBufferEx<MeshVertex>(BufferUsage::Static, vh.nVertices, &vertices[0]);

        reader >> aabb_.min >> aabb_.max;

        //load material
        materials_.resize(ih.nTriangleGroups);
        std::string materialFile = removeFileExt(resource_) + ".mtl";
        SectionPtr root = SectionFactory::loadDS(materialFile);
        if(!root)
        {
            ORA_LOG_ERROR("Failed to load material file '%s'", materialFile.c_str());
            return false;
        }

        std::string texturePath = getFilePath(resource_);
        
        SectionPtrVector sectionMaterials;
        root->reads("material", sectionMaterials);
        for (SectionPtr p : sectionMaterials)
        {
            int pgid = p->getInt();

            MaterialPtr mtl = new Material();
            mtl->load(p, texturePath);
            materials_[pgid] = mtl;
        }

        //generate sub mesh
        subMeshs_.resize(ih.nTriangleGroups);
        for (int i = 0; i < ih.nTriangleGroups; ++i)
        {
            const PrimitiveGroup & pg = primGroups[i];
            SubMeshPtr pMesh = new SubMesh();
            pMesh->setPrimitive(PrimitiveType::TriangleList, pg.startIndex, pg.nPrimitives * 3, i);

            subMeshs_[i] = pMesh;
        }

        return true;
    }

    MeshPtr Mesh::clone()
    {
        MeshPtr mesh = new Mesh();
        mesh->source_ = this;
        mesh->resource_ = this->resource_;
        mesh->vertexBuffer_ = this->vertexBuffer_;
        mesh->indexBuffer_ = this->indexBuffer_;
        mesh->vertexDecl_ = this->vertexDecl_;
        mesh->subMeshs_ = this->subMeshs_;
        mesh->aabb_ = this->aabb_;

        for(auto mtl : this->materials_)
        {
            mesh->materials_.push_back(mtl->clone());
        }

        return mesh;
    }

    void Mesh::draw()
    {
        if(!vertexBuffer_ || !vertexDecl_) return;

        vertexBuffer_->bind();
        if(indexBuffer_)
            indexBuffer_->bind();

        vertexDecl_->bind();

        for(SubMeshPtr ptr : subMeshs_)
        {
            MaterialPtr mtl = getMaterial(ptr->getMaterial());
            if(mtl && mtl->begin())
            {
                ptr->draw();
                mtl->end();
            }
        }

        vertexDecl_->unbind();
        if(indexBuffer_)
            indexBuffer_->unbind();
        vertexBuffer_->unbind();
    }

    void Mesh::setVertexBuffer(VertexBufferPtr vertex)
    {
        vertexBuffer_ = vertex;
    }

    void Mesh::setIndexBuffer(IndexBufferPtr index)
    {
        indexBuffer_ = index;
    }

    void Mesh::setVertexDecl(VertexDeclarationPtr decl)
    {
        vertexDecl_ = decl;
    }

    void Mesh::setSubMeshes(const Mesh::SubMeshes & subMeshes)
    {
        subMeshs_ = subMeshes;
    }

    void Mesh::addSubMesh(SubMeshPtr subMesh)
    {
        subMeshs_.push_back(subMesh);
    }

    const Mesh::SubMeshes & Mesh::getSubMeshes() const
    {
        return subMeshs_;
    }

    size_t Mesh::getNbSubMesh() const
    {
        return subMeshs_.size();
    }

    SubMeshPtr Mesh::getSubMesh(size_t i) const
    {
        return subMeshs_.at(i);
    }

    MaterialPtr Mesh::getMaterial(int mtlID) const
    {
        if(mtlID < 0 || mtlID >= int(materials_.size()))
            return nullptr;

        return materials_[mtlID];
    }

    void Mesh::setMaterials(const Mesh::Materials & materials)
    {
        materials_ = materials;
    }

    int Mesh::addMaterial(MaterialPtr mtl)
    {
        int index = int(materials_.size());
        materials_.push_back(mtl);
        return index;
    }

}//end namespace ora
