/******************************************************************************
BigWorld Technology 
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef VISUAL_MESH_HPP
#define VISUAL_MESH_HPP

#include "mmath.h"
#include "primitive_file_structs.hpp"
#include "visual_type.h"
#include "vertex_formats.hpp"
#include "index_buffer.hpp"

class QuickFileWriter;

namespace MeshExporter
{
    typedef SmartPointer<class VisualMesh> VisualMeshPtr;

    typedef std::vector<Moo::PrimitiveGroup> PGVector;
    typedef std::vector<Moo::VertexXYZNUV> VertexVector;
    typedef std::vector<Moo::VertexXYZNUV2> UV2VertexVector;
    typedef std::vector<Moo::VertexXYZNDUV> DiffuseVertexVector;
    typedef std::vector<Moo::VertexXYZNUVI> IndexedVertexVector;
    typedef std::vector<Moo::VertexXYZNUVITB> IndexedTBVertexVector;
    typedef std::vector<Moo::VertexXYZNUVTB> TBVertexVector;
    
    typedef std::vector<std::pair<std::string, std::string> > CopyList;

    /**
    *	This class is used to convert 3dsMax objects into BigWorld visual
    *	mesh objects.
    */
    class VisualMesh : public ReferenceCount
    {
    public:
        // Constructor / destructor
        VisualMesh();
        virtual ~VisualMesh();

        // Public interface
        bool				init( INode* node, std::string exportFromPath, std::string exportToPath, bool checkMats = true );

        void				resources( std::vector<std::string>& retResources );
        void				resourcesToCopy( CopyList & retResources );

        void				snapVertices( bool snapVertices ) { snapVertices_ = snapVertices; }
        bool				checkDXMaterials( const std::string& fxPostfix = "" );
        bool				largeIndices() const { return largeIndices_; }

        bool				dualUV() const { return dualUV_; }
        void				dualUV( bool value ){ dualUV_ = value; }

        bool				vertexColours() const { return vertexColours_; }
        void				vertexColours( bool value ){ vertexColours_ = value; }

        const BoundingBox&	bb() const { return bb_; };

        virtual void		add(VisualMesh & destMesh, int forceMeshIndex = -1, bool worldSpace = true );
       
        std::string			getIdentifier() { return this->identifier_; }

        virtual bool		save(const std::string& primitiveFile, const BoundingBox & bb);

    protected:

        int		addVertex( const BloatVertex& bv );

        void	createNormals( );
        void	getNormalsFromModifier( INode* pNode );
        float	normalAngle( const Triangle& tri, uint32 index );
        void	addNormal( const Point3& normal, int realIndex, int smoothingGroup, int index );

        void	gatherMaterials( INode* node, std::string exportFromPath, const std::string & exportToPath );
        void	gatherMtl( Mtl* node, std::string exportFromPath, const std::string & exportToPath );
        void	addMaterial( StdMat* mtl, std::string exportFromPath, const std::string & exportToPath );
        void	addDXMaterial( Mtl* mtl );
        int		findOrAddMaterial( const Material & mat );

        void	flipTriangleWindingOrder();
        void	sortTriangles();
        void	removeDuplicateVertices();

        bool	createPrimitiveGroups( PGVector& primGroups, Moo::IndicesHolder& indices );
        bool	createMeshParticlePrimGroups( PGVector& primGroups );
        void	createVertexList( VertexVector& vertices );
        void	createVertexList( UV2VertexVector& vertices );
        void	createVertexList( DiffuseVertexVector& vertices );
        void	createVertexList( IndexedVertexVector& vertices );
        void	createVertexList( TBVertexVector& vertices );
        void	createVertexList( IndexedTBVertexVector& vertices );

        //禁止拷贝复制
        VisualMesh( const VisualMesh& );
        VisualMesh& operator=( const VisualMesh& );

    protected:
        typedef std::vector<int>	RemapVector;
        RemapVector					materialRemap_;
        std::vector< Point3 >		vertexPositions_; //顶点位置

        bool			vertexColours_;//顶点是否带颜色
        bool			dualUV_;//是否双层纹理

        BVVector		vertices_;//顶点数据
        TriangleVector	triangles_;//面数据
        MaterialVector	materials_;//材质数据

        std::string identifier_;
        BoundingBox bb_;

        Matrix3		world_;//时间矩阵
        bool		snapVertices_;
        bool		largeIndices_;//索引是否较大（大于uint16）
        float		visibility_;
    };

}//end namespace MeshExporter

#endif // VISUAL_MESH_HPP
