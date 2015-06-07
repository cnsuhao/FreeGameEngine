/******************************************************************************
BigWorld Technology 
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4530 )

#include "pch.hpp"
#include "mfxexp.hpp"

#include <ieditnormals.h>

#define OPTIMISATION (1)

#if MAX_RELEASE < 6000
#define MAX5
#endif

#ifndef MAX5
#include <idxmaterial.h>
#include <bmmlib.h>
#include <iparamm2.h>
#endif

#include <bitmap.h>
#include <iparamb2.h>

#include "visual_mesh.hpp"
#include "string_tool.h"
#include "quick_file_writer.hpp"
#include "utility.hpp"
#include "expsets.hpp"


extern bool validResource( const std::string& fileName, std::string& retResourceName );

#define TC_OPTIMISATION
#ifdef TC_OPTIMISATION
#include <hash_map>

inline uint64 hashKey( const Point3& v )
{
    uint64 x = uint64(v.x);
    uint64 y = uint64(v.y);
    uint64 z = uint64(v.z);
    x <<= 20;
    y <<= 10;
    return (x + y + z);
}
#endif//TC_OPTIMISATION



namespace MeshExporter
{
    static std::map<BloatVertex, std::set<BVVector::size_type> > verticeSetMap;
    static std::map<int, std::vector<BVVector::size_type> > verticeIndexSetMap;

    
    const uint32 MeshVersion = 1;
    const uint32 MeshMagic = 'm' | 'e' << 8 | 's' << 16 | 'h' << 24;


    //������(float3)ѹ����uint32
    static inline uint32 packNormal( const Point3&nn )
    {
        Point3 n = nn;
        n.Normalize();

        n.x = Math::clamp(-1.f, n.x, 1.f);
        n.y = Math::clamp(-1.f, n.y, 1.f);
        n.z = Math::clamp(-1.f, n.z, 1.f);

        if (ExportSettings::instance().useLegacyOrientation())
        {
            return	( ( ( (uint32)(-n.y * 511.0f) )  & 0x3ff ) << 22L ) |
                ( ( ( (uint32)(n.z * 1023.0f) ) & 0x7ff ) << 11L ) |
                ( ( ( (uint32)(-n.x * 1023.0f) ) & 0x7ff ) <<  0L );
        }
        else
        {
            return	( ( ( (uint32)(n.y * 511.0f) )  & 0x3ff ) << 22L ) |
                ( ( ( (uint32)(n.z * 1023.0f) ) & 0x7ff ) << 11L ) |
                ( ( ( (uint32)(n.x * 1023.0f) ) & 0x7ff ) <<  0L );
        }		
    }


    TimeValue timeNow()
    {
        return ExportSettings::instance().staticFrame() * GetTicksPerFrame();
    }


    // -----------------------------------------------------------------------------
    // Section: VisualMesh
    // -----------------------------------------------------------------------------

    /**
    *	Constructor.
    */
    VisualMesh::VisualMesh()
        : identifier_( "Scene Root" ),
        snapVertices_( false ),
        largeIndices_( false ),
        dualUV_( false ),
        vertexColours_( false ),
        visibility_( 1 )
    {
    }


    /**
    *	Destructor.
    */
    VisualMesh::~VisualMesh()
    {
    }


    /**
    *	Returns all resources used.
    *
    *	@param	retResources	The vector of all resources.
    */
    void VisualMesh::resources( std::vector<std::string>& retResources )
    {
        MaterialVector::iterator it = materials_.begin();
        MaterialVector::iterator end = materials_.end();
        while (it != end)
        {
            Material& m = *it++;
            m.resources( retResources );
        }
    }

    //Ҫ��������Դ��������
    void VisualMesh::resourcesToCopy( CopyList & retResources )
    {
        MaterialVector::iterator it = materials_.begin();
        MaterialVector::iterator end = materials_.end();
        while (it != end)
        {
            Material& m = *it++;
            if (m.copyFrom.size() > 0)
                retResources.push_back(std::pair<std::string, std::string>(m.copyFrom, m.mapIdentifier));
        }
    }

    //�������������ߵļнǣ�indexΪ�����ߵĹ��õ�
    /**
    * This method finds the angle between two edges in a triangle.
    *
    * @param tri	The triangle.
    * @param index	The vertex reference index to find the angle from.
    * @return		The angle (in radians) between the two edges.
    */
    float VisualMesh::normalAngle( const Triangle& tri, uint32 index )
    {
        Point3 v1 = vertices_[tri.index[(index + 1 ) % 3]].pos - vertices_[tri.index[index]].pos;
        Point3 v2 = vertices_[tri.index[(index + 2 ) % 3]].pos - vertices_[tri.index[index]].pos;


        float len = Length( v1 );
        if (len == 0)
            return 0;
        v1 /= len;

        len = Length( v2 );
        if (len == 0)
            return 0;
        v2 /= len;

        float normalAngle = DotProd( v1, v2 );

        normalAngle = min( 1.f, max( normalAngle, -1.f ) );

        return acosf( normalAngle );	
    }

    //������ķ��߶�����һ��ֵ
    /**
    *	This method adds a normal to all vertices that should be influenced by it.
    *
    *	@param normal the normal to add
    *	@param realIndex the index of the vertex in the max mesh.
    *	@param smoothingGroup the smoothinggroup of this normal.
    *	@param index the index of a vertex this normal has to influence.
    */
    void VisualMesh::addNormal( const Point3& normal, int realIndex, int smoothingGroup, int index )
    {
        if( verticeIndexSetMap.find( realIndex ) != verticeIndexSetMap.end() )
        {
            std::vector<BVVector::size_type>& verticeIndexSet = verticeIndexSetMap[ realIndex ];
            for( std::vector<BVVector::size_type>::iterator iter = verticeIndexSet.begin();
                iter != verticeIndexSet.end(); ++iter )
            {
                BloatVertex& bv = vertices_[ *iter ];
                if( bv.smoothingGroup & smoothingGroup || ((*iter) == index) )
                    bv.normal += normal;
            }
        }
    }

    //Ϊ���ж��㴴������
    /**
    *	This method creates vertex normals for all vertices in the vertex list.
    */
    void VisualMesh::createNormals( )
    {
        TriangleVector::iterator it = triangles_.begin();
        TriangleVector::iterator end = triangles_.end();

        while (it!=end)
        {
            Triangle& tri = *it++;
            Point3 v1 = vertices_[tri.index[1]].pos - vertices_[tri.index[0]].pos;
            Point3 v2 = vertices_[tri.index[2]].pos - vertices_[tri.index[0]].pos;
            Point3 normal = v1^v2;
            normal = Normalize( normal );

            addNormal( normal * normalAngle( tri, 0), tri.realIndex[0], tri.smoothingGroup, tri.index[0] );
            addNormal( normal * normalAngle( tri, 1), tri.realIndex[1], tri.smoothingGroup, tri.index[1] );
            addNormal( normal * normalAngle( tri, 2), tri.realIndex[2], tri.smoothingGroup, tri.index[2] );
        }

        //���ۼ�֮��ķ��ߵ�λ��
        BVVector::iterator vit = vertices_.begin();
        BVVector::iterator vend = vertices_.end();
        while (vit != vend)
        {
            BloatVertex& bv = *vit++;
            bv.normal = Normalize( bv.normal );
        }
    }


    /**
    *	This method extracts the normals from the Normals modifier for
    *	pNode.
    *
    *	@param	pNode	The node to extract normal information from.
    */
    void VisualMesh::getNormalsFromModifier( INode* pNode )
    {
        IEditNormalsMod* pNormsMod = MFXExport::findEditNormalsMod( pNode );
        int triangles = 0, changed = 0;
        if (pNormsMod)
        {
            int nFaces = pNormsMod->EnfnGetNumFaces(pNode);
            DEBUG_MSG( "MFXExport::preProcess - found edit normals modifier on object %s %d faces\n", pNode->GetName(), nFaces );

            for (int i = 0; i < nFaces; i++)
            {
                int faceDegree = pNormsMod->EnfnGetFaceDegree( i, pNode );
                std::vector< int > face;
                std::map< int, Point3* > vertexNormals;
                for (int j = 0; j < faceDegree; j++)
                {
                    face.push_back( pNormsMod->EnfnGetVertexID( i, j, pNode ) );
                    Point3* norm = NULL;
                    if (pNormsMod->EnfnGetFaceNormalSpecified( i, j, pNode ))
                    {
                        int nID = pNormsMod->EnfnGetNormalID( i, j, pNode );
                        norm = pNormsMod->EnfnGetNormal( nID, pNode, timeNow() );
                    }
                    vertexNormals[face.back()] = norm;
                }

                TriangleVector::iterator it = triangles_.begin();
                TriangleVector::iterator end = triangles_.end();
                int nMatching = 0;
                while (it != end)
                {
                    if (std::find( face.begin(), face.end(), it->realIndex[0] ) != face.end() &&
                        std::find( face.begin(), face.end(), it->realIndex[1] ) != face.end() &&
                        std::find( face.begin(), face.end(), it->realIndex[2] ) != face.end())
                    {
                        triangles++;
                        if (vertexNormals[it->realIndex[0]] != NULL)
                        {
                            int ind = vertices_.size();
                            vertices_.push_back( vertices_[it->index[0]] );
                            vertices_.back().normal = Normalize(*vertexNormals[it->realIndex[0]]);
                            it->index[0] = ind;
                            changed++;
                        }
                        if (vertexNormals[it->realIndex[1]] != NULL)
                        {
                            int ind = vertices_.size();
                            vertices_.push_back( vertices_[it->index[1]] );
                            vertices_.back().normal = Normalize(*vertexNormals[it->realIndex[1]]);
                            it->index[1] = ind;
                            changed++;
                        }
                        if (vertexNormals[it->realIndex[2]] != NULL)
                        {
                            int ind = vertices_.size();
                            vertices_.push_back( vertices_[it->index[2]] );
                            vertices_.back().normal = Normalize(*vertexNormals[it->realIndex[2]]);
                            it->index[2] = ind;
                            changed++;
                        }
                    }
                    it++;
                }
            }

            DEBUG_MSG( "MFXExport::preProcess changed %d triangles, %d verticies\n", triangles, changed );
        }
    }

    //�˷�����ȡmax���ʣ�����ת��Ϊ������ʽ
    /**
    *	This method gets a max material and stores the material
    *	data we want to export.
    *
    *	@param	mtl	The max material being added.
    */
    void VisualMesh::addMaterial( StdMat* mtl, std::string exportFromPath, const std::string & exportToPath  )
    {
        Material m;
        m.ambient = mtl->GetAmbient( timeNow() );
        m.diffuse = mtl->GetDiffuse( timeNow() );
        m.specular = mtl->GetSpecular( timeNow() );
        m.selfIllum = mtl->GetSelfIllum( timeNow() );
        m.identifier = mtl->GetName();

        m.mapIdMeaning = Material::MAP_NONE;
        if( mtl->MapEnabled( ID_DI ) )
        {
            //��ȡ������Դ����
            Texmap *map = mtl->GetSubTexmap( ID_DI );
            if (map && map->ClassID() == Class_ID( BMTEX_CLASS_ID, 0x00 ))
            {
                MCHAR bitmapName[300];
                std::string texname;
                if (BMMGetFullFilename(((BitmapTex *)map)->GetMapName(), bitmapName))
                {
                    texname = bitmapName;
                }	
                else
                    texname = ((BitmapTex *)map)->GetMapName();

                std::string submapName = unifySlashes( toLower( std::string( map->GetName() ) ) );
                std::string textureName = unifySlashes( texname );

                //mapIdentifier�ǲ�������
                if (submapName.length() > 4 &&
                    submapName.substr( submapName.length()-4 ) == ".mfm")
                {
                    m.mapIdentifier = submapName;
                    m.mapIdMeaning = Material::MAP_MFM;
                }
                else//mapIdentifier����������
                {
                    textureName = exportToPath + getFileName(textureName);
                    m.copyFrom = texname;

                    if (textureName.length())
                    {
                        m.mapIdentifier = textureName;
                        m.mapIdMeaning = Material::MAP_TEXTURE;
                    }
                }
            }
        }

        materialRemap_.push_back( this->findOrAddMaterial( m ) );
    }

    //�÷�����ȡdx���ʣ�����ת��Ϊ������ʽ
    /**
    *	This method gets a DirectX material and stores the material
    *	data we want to export.
    *
    *	@param	mtl	The DirectX material being added.
    */
    void VisualMesh::addDXMaterial( Mtl* mtl )
    {
#ifndef MAX5
        Material m;
        m.identifier = mtl->GetName();
        IDxMaterial * idxm = (IDxMaterial*)mtl->GetInterface(IDXMATERIAL_INTERFACE);
#ifndef MAX_RELEASE_R12 // GetEffectFilename was deprecated in max 2010
        m.fxFile_ = idxm->GetEffectFilename();
#else
        m.fxFile_ = idxm->GetEffectFile().GetFileName().data();
#endif
        IParamBlock2* pParamBlock = mtl->GetParamBlock(0);
        int nParams = pParamBlock->NumParams();
        // iterate over the parameters
        for (int i = 0; i < nParams; i++)
        {
            // The parameter definitions contains information about how to 
            // interpret the parameter data.
            ParamDef& def = pParamBlock->GetParamDef(i);

            // If lightDir or lightColor, continue
            std::string defName = std::string( def.int_name );
            if ( defName == "lightDir" || defName == "lightColor" )
                continue;

            switch (def.type)
            {
            case TYPE_BOOL:
                {
                    BOOL b = pParamBlock->GetInt( i );
                    m.boolOverrides_.push_back( std::make_pair( std::string( def.int_name ), b  ) );
                    break;
                }
            case TYPE_FLOAT:
                {
                    float f = pParamBlock->GetFloat( i );
                    m.floatOverrides_.push_back( std::make_pair( std::string( def.int_name ), f  ) );
                    break;
                }
            case TYPE_INT:
                {
                    int j = pParamBlock->GetInt( i );
                    m.intOverrides_.push_back( std::make_pair( std::string( def.int_name ), j ) );
                    break;
                }
            case TYPE_FRGBA:
                {
                    Point4 v = pParamBlock->GetPoint4( i );
                    m.vectorOverrides_.push_back( std::make_pair( std::string( def.int_name ), v ) );
                    break;
                }
            case TYPE_POINT4:
                {
                    Point4 v = pParamBlock->GetPoint4( i );
                    m.vectorOverrides_.push_back( std::make_pair( std::string( def.int_name ), v ) );
                    break;
                }
            case TYPE_BITMAP:
                {
                    PBBitmap* b = pParamBlock->GetBitmap( i );
                    if (b && b->bi.Name() != std::string("None"))
                        m.textureOverrides_.push_back( std::make_pair( std::string( def.int_name ), std::string( b->bi.Name() ) ) );
                    break;
                }
            }
        }

        materialRemap_.push_back( this->findOrAddMaterial( m ) );
#endif
    }

    void VisualMesh::gatherMtl( Mtl* mtl, std::string exportFromPath, const std::string & exportToPath )
    {
        if (mtl == NULL)//��Ч����
        {
            Material m;
            materialRemap_.push_back( this->findOrAddMaterial( m ) );
        }
        else if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))//��ͨ����
        {
            addMaterial( (StdMat*)mtl, exportFromPath, exportToPath  );
        }
#ifndef MAX5
        else if (mtl->GetInterface(IDXMATERIAL_INTERFACE))//dx ����
        {
            addDXMaterial( mtl );
        }
#endif
        else if (mtl->NumSubMtls())//���Ӳ���
        {
            //����ÿ���Ӳ���
            for (int i = 0; i < mtl->NumSubMtls(); i++) 
            {
                gatherMtl(mtl->GetSubMtl( i ), exportFromPath, exportToPath);
            }
        }
        else//�ղ���
        {
            Material m;
            m.identifier = mtl->GetName();
            materialRemap_.push_back( this->findOrAddMaterial( m ) );
        }
    }

    //�÷����ռ�ָ��node�е����в���
    /**
    *	This method gathers all the materials contained in a specific node.
    *
    *	@param	node	The node to gather materials from.
    */
    void VisualMesh::gatherMaterials( INode* node, std::string exportFromPath, const std::string & exportToPath )
    {
        gatherMtl(node->GetMtl(), exportFromPath, exportToPath);
        
        if (materialRemap_.empty())
        {
            materialRemap_.push_back( 0 );
        }
    }

    //�÷������һ������visual���ʵ����ʱ��У�����������
    /**
    *	This method finds or adds the given visual material to our list.
    *
    *	@param	mat	Material to be added.
    *	@return	Index of mat in materials_.
    */
    int VisualMesh::findOrAddMaterial( const Material & mat )
    {
        uint32 i;
        for (i = 0; i < materials_.size(); i++)
        {
            if (materials_[i] == mat) return i;
        }

        materials_.push_back( mat );
        return i;
    }

    //��ת�����λ���˳��
    /**
    *	This method swaps the triangle winding order for all triangles.
    */
    void VisualMesh::flipTriangleWindingOrder()
    {
        TriangleVector::iterator it = triangles_.begin();
        TriangleVector::iterator end = triangles_.end();

        while (it!=end)
        {
            Triangle& tri = *it++;
            std::swap( tri.index[1], tri.index[2] );
            std::swap( tri.realIndex[1], tri.realIndex[2] );
        }
    }


    BVVector* g_verts = 0;

    //������������
    bool triangleSorter( const Triangle& t1, const Triangle& t2 )
    {
        //return t1.materialIndex < t2.materialIndex;

        if (t1.materialIndex == t2.materialIndex)
            return (*g_verts)[t1.index[0]].meshIndex < (*g_verts)[t2.index[0]].meshIndex;
        else
            return t1.materialIndex < t2.materialIndex;
    }


    /**
    *	This method sorts the triangles.
    */
    void VisualMesh::sortTriangles()
    {
        //�Ȱ���������
        g_verts = &vertices_;
        std::sort( triangles_.begin(), triangles_.end(), triangleSorter );

        if (triangles_.size())
        {
            std::vector<int> vertexIndices(vertices_.size(), -1);
            BVVector vs;

            int mIndex = triangles_.front().materialIndex;
            for (int i = 0; i < triangles_.size(); i++)
            {
                Triangle& tri = triangles_[i];
                if (tri.materialIndex != mIndex)
                {
                    mIndex = tri.materialIndex;
                    vertexIndices.resize( 0 );
                    vertexIndices.resize( vertices_.size(), -1 );
                }
                for (int j = 0; j < 3; j++)
                {
                    int ind = tri.index[j];
                    if (vertexIndices[ind] == -1)
                    {
                        vertexIndices[ind] = vs.size();
                        vs.push_back( vertices_[ind] );
                    }
                    tri.index[j] = vertexIndices[ind];
                }
            }
            vertices_ = vs;
        }
    }

    //����ͼԪ�飬���������ΰ����ʷ���
    /**
    *	This method creates our primitive groups.
    *
    *	@param	primGroups	The output primitivegroups.
    *	@param	indices		The output indices.
    */
    bool VisualMesh::createPrimitiveGroups( PGVector& primGroups, Moo::IndicesHolder& indices )
    {
        if (triangles_.size())
        {
            int mIndex = triangles_.front().materialIndex;

            int firstVertex = triangles_.front().index[0];
            int lastVertex = triangles_.front().index[0];
            int firstTriangle = 0;

            int offset = 0;
            indices.setSize( triangles_.size() * 3, Moo::IndicesReference::bestFormat( vertices_.size() ) );

            for (int i = 0; i <= triangles_.size(); i++)
            {
                Triangle& tri = i == triangles_.size() ? triangles_.back() : triangles_[i];
                if (i == triangles_.size() || tri.materialIndex != mIndex)
                {
                    Moo::PrimitiveGroup pg;
                    pg.startVertex_ = firstVertex;
                    pg.nVertices_ = lastVertex - firstVertex + 1;
                    pg.startIndex_ = firstTriangle * 3;
                    pg.nPrimitives_ = i - firstTriangle;
                    primGroups.push_back( pg );	
                    firstVertex = tri.index[0];
                    lastVertex = tri.index[0];
                    firstTriangle = i;
                    materials_[ mIndex ].inUse = true;
                    mIndex = tri.materialIndex;
                }
                if (i!=triangles_.size())
                {
                    indices.set( offset, tri.index[0] );
                    ++offset;
                    indices.set( offset, tri.index[1] );
                    ++offset;
                    indices.set( offset, tri.index[2] );
                    ++offset;
                    firstVertex = min( firstVertex, min( tri.index[0], min( tri.index[1], tri.index[2] ) ) );
                    lastVertex  = max( lastVertex,  max( tri.index[0], max( tri.index[1], tri.index[2] ) ) );
                }
            }

            //Mesh particles add an extra 15 primitive groups, defined by the index
            //of the vertices.
            if (ExportSettings::instance().exportMode() == ExportSettings::MESH_PARTICLES)
            {
                return this->createMeshParticlePrimGroups(primGroups);		
            }
        }
        return true;
    }


    /**
    *	This method adds 15 primitive groups, based on meshIndex.
    *	This allows the mesh particles to be drawn all together (prim group 0)
    *	or one-by-one (prim group 1 - 15 incl.), which is used when doing
    *	slower, correct sorting of alpha-blended mesh particles.
    *
    *	@param	primGroups	The vector of prim groups.
    *	@return	Success or failure.
    */
    bool VisualMesh::createMeshParticlePrimGroups( PGVector& primGroups )
    {
        //Mesh particles should have one primitive group, representing the
        //entire mesh.
        //MF_ASSERT( primGroups.size() == 1 )
        if (primGroups.size() != 1)
        {
            MessageBox( GetForegroundWindow(),
                "VisualMesh::createMeshParticlePrimGroups - Mesh particles must use only 1 material/mesh.\n",
                "Visual Exporter", MB_OK | MB_ICONEXCLAMATION );
            return false;
        }

        if (triangles_.size())
        {
            Triangle& tri = triangles_[0];

            int firstVertex = tri.index[0];
            int lastVertex = firstVertex;
            int mIndex = vertices_[firstVertex].meshIndex;
            int firstTriangle = 0;

            for (int i = 0; i <= triangles_.size(); i++)
            {
                Triangle& tri = i == triangles_.size() ? triangles_.back() : triangles_[i];
                if (i == triangles_.size() || vertices_[tri.index[0]].meshIndex != mIndex)
                {
                    Moo::PrimitiveGroup pg;
                    pg.startVertex_ = firstVertex;
                    pg.nVertices_ = lastVertex - firstVertex + 1;
                    pg.startIndex_ = firstTriangle * 3;
                    pg.nPrimitives_ = i - firstTriangle;
                    primGroups.push_back( pg );

                    firstVertex = lastVertex = tri.index[0];
                    mIndex = vertices_[firstVertex].meshIndex;				
                    firstTriangle = i;
                }

                firstVertex = min( firstVertex, min( tri.index[0], min( tri.index[1], tri.index[2] ) ) );
                lastVertex  = max( lastVertex,  max( tri.index[0], max( tri.index[1], tri.index[2] ) ) );
            }		
        }

        int nPrimGroups = primGroups.size();

        //Mesh particles should now always have 16 primitive groups.
        //Made this an assert because the MeshParticleRenderer assumes
        //there are 16 primitivegroups.
        MF_ASSERT( primGroups.size() == 16, "createMeshParticlePrimGroups");

        return true;
    }

    //��������б�����λ�á����ߡ�����������Ϣ��
    /**
    *	This method creates our output vertex list.
    *
    *	@vertices The output vertex list.
    */
    void VisualMesh::createVertexList( VertexVector& vertices )
    {
        Moo::VertexXYZNUV v;
        BVVector::iterator it = vertices_.begin();
        BVVector::iterator end = vertices_.end();
        while (it!=end)
        {
            BloatVertex& bv = *it++;
            bv.pos *= ExportSettings::instance().unitScale();

            if (ExportSettings::instance().useLegacyOrientation())
            {
                v.pos_[0] = -bv.pos.x;
                v.pos_[1] = bv.pos.z;
                v.pos_[2] = -bv.pos.y;
                v.normal_[0] = -bv.normal.x;
                v.normal_[1] = bv.normal.z;
                v.normal_[2] = -bv.normal.y;
            }
            else
            {
                v.pos_[0] = bv.pos.x;
                v.pos_[1] = bv.pos.z;
                v.pos_[2] = bv.pos.y;
                v.normal_[0] = bv.normal.x;
                v.normal_[1] = bv.normal.z;
                v.normal_[2] = bv.normal.y;
            }

            v.uv_[0] = bv.uv.x;
            v.uv_[1] = bv.uv.y;

            vertices.push_back( v );
        }
    }

    //���������Ϣ������λ�á����ߡ��������ꡢ������ɫ��Ϣ��
    /**
    *	This method creates our output vertex list.
    *
    *	@vertices The output vertex list.
    */
    void VisualMesh::createVertexList( DiffuseVertexVector& vertices )
    {
        Moo::VertexXYZNDUV v;
        BVVector::iterator it = vertices_.begin();
        BVVector::iterator end = vertices_.end();
        while (it!=end)
        {
            BloatVertex& bv = *it++;
            bv.pos *= ExportSettings::instance().unitScale();

            if (ExportSettings::instance().useLegacyOrientation())
            {
                v.pos_[0] = -bv.pos.x;
                v.pos_[1] = bv.pos.z;
                v.pos_[2] = -bv.pos.y;
                v.normal_[0] = -bv.normal.x;
                v.normal_[1] = bv.normal.z;
                v.normal_[2] = -bv.normal.y;
            }
            else
            {
                v.pos_[0] = bv.pos.x;
                v.pos_[1] = bv.pos.z;
                v.pos_[2] = bv.pos.y;
                v.normal_[0] = bv.normal.x;
                v.normal_[1] = bv.normal.z;
                v.normal_[2] = bv.normal.y;
            }

            v.uv_[0] = bv.uv.x;
            v.uv_[1] = bv.uv.y;

            v.colour_ = D3DCOLOR_COLORVALUE( bv.colour.x, bv.colour.y, bv.colour.z, bv.colour.w );

            vertices.push_back( v );
        }
    }

    //��������б�����λ�á����ߡ���������0����������1
    /**
    *	This method creates our output vertex list.
    *
    *	@vertices The output vertex list.
    */
    void VisualMesh::createVertexList( UV2VertexVector& vertices )
    {
        Moo::VertexXYZNUV2 v;
        BVVector::iterator it = vertices_.begin();
        BVVector::iterator end = vertices_.end();
        while (it!=end)
        {
            BloatVertex& bv = *it++;
            bv.pos *= ExportSettings::instance().unitScale();

            if (ExportSettings::instance().useLegacyOrientation())
            {
                v.pos_[0] = -bv.pos.x;
                v.pos_[1] = bv.pos.z;
                v.pos_[2] = -bv.pos.y;
                v.normal_[0] = -bv.normal.x;
                v.normal_[1] = bv.normal.z;
                v.normal_[2] = -bv.normal.y;
            }
            else
            {
                v.pos_[0] = bv.pos.x;
                v.pos_[1] = bv.pos.z;
                v.pos_[2] = bv.pos.y;
                v.normal_[0] = bv.normal.x;
                v.normal_[1] = bv.normal.z;
                v.normal_[2] = bv.normal.y;
            }

            v.uv_[0] = bv.uv.x;
            v.uv_[1] = bv.uv.y;

            v.uv2_[0] = bv.uv2.x;
            v.uv2_[1] = bv.uv2.y;

            vertices.push_back( v );
        }
    }


    /**
    *	This method creates our output vertex list.
    *
    *	@vertices The output vertex list.
    */
    void VisualMesh::createVertexList( IndexedVertexVector& vertices )
    {
        Moo::VertexXYZNUVI v;

        BVVector::iterator lastit = vertices_.begin();
        BVVector::iterator it = vertices_.begin();
        BVVector::iterator end = vertices_.end();

        while (it!=end)
        {
            MF_ASSERT(lastit->meshIndex <= it->meshIndex, "createVertexList");
            lastit = it;

            BloatVertex& bv = *it++;

            bv.pos *= ExportSettings::instance().unitScale();

            if (ExportSettings::instance().useLegacyOrientation())
            {
                v.pos_[0] = -bv.pos.x;
                v.pos_[1] = bv.pos.z;
                v.pos_[2] = -bv.pos.y;
                v.normal_[0] = -bv.normal.x;
                v.normal_[1] = bv.normal.z;
                v.normal_[2] = -bv.normal.y;
            }
            else
            {
                v.pos_[0] = bv.pos.x;
                v.pos_[1] = bv.pos.z;
                v.pos_[2] = bv.pos.y;
                v.normal_[0] = bv.normal.x;
                v.normal_[1] = bv.normal.z;
                v.normal_[2] = bv.normal.y;
            }

            v.uv_[0] = bv.uv.x;
            v.uv_[1] = bv.uv.y;
            v.index_ = (float) bv.meshIndex * 3;

            vertices.push_back( v );		
        }
    }


    /**
    *	This method creates our output vertex list.
    *
    *	@vertices The output vertex list.
    */
    void VisualMesh::createVertexList( TBVertexVector& vertices )
    {
        Moo::VertexXYZNUVTB v;
        BVVector::iterator it = vertices_.begin();
        BVVector::iterator end = vertices_.end();
        while (it!=end)
        {
            BloatVertex& bv = *it++;
            bv.pos *= ExportSettings::instance().unitScale();

            if (ExportSettings::instance().useLegacyOrientation())
            {
                v.pos_[0] = -bv.pos.x;
                v.pos_[1] = bv.pos.z;
                v.pos_[2] = -bv.pos.y;
            }
            else
            {
                v.pos_[0] = bv.pos.x;
                v.pos_[1] = bv.pos.z;
                v.pos_[2] = bv.pos.y;
            }

            v.normal_ = packNormal( bv.normal );
            v.uv_[0] = bv.uv.x;
            v.uv_[1] = bv.uv.y;
            v.tangent_ = packNormal( bv.tangent );
            v.binormal_ = packNormal( bv.binormal );//�������������ƹⷽ��нǷ�Χ�İ�������
            vertices.push_back( v );
        }
    }


    /**
    *	This method creates our output vertex list.
    *
    *	@vertices The output vertex list.
    */
    void VisualMesh::createVertexList( IndexedTBVertexVector& vertices )
    {
        Moo::VertexXYZNUVITB v;

        BVVector::iterator lastit = vertices_.begin();
        BVVector::iterator it = vertices_.begin();
        BVVector::iterator end = vertices_.end();

        while (it!=end)
        {
            MF_ASSERT( lastit->meshIndex <= it->meshIndex, "createVertexList" );
            lastit = it;

            BloatVertex& bv = *it++;
            bv.pos *= ExportSettings::instance().unitScale();

            if (ExportSettings::instance().useLegacyOrientation())
            {
                v.pos_[0] = -bv.pos.x;
                v.pos_[1] = bv.pos.z;
                v.pos_[2] = -bv.pos.y;
            }
            else
            {
                v.pos_[0] = bv.pos.x;
                v.pos_[1] = bv.pos.z;
                v.pos_[2] = bv.pos.y;
            }

            v.normal_ = packNormal( bv.normal );
            v.uv_[0] = bv.uv.x;
            v.uv_[1] = bv.uv.y;
            v.tangent_ = packNormal( bv.tangent );
            v.binormal_ = packNormal( bv.binormal );
            v.index_ = (float) bv.meshIndex * 3;

            vertices.push_back( v );		
        }
    }


    struct PG
    {
        BVVector verts;
        TriangleVector tris;
    };

    //����float�������
    static bool closeEnough( float a, float b )
    {
        return fabs( a - b ) < 0.0001f;
    }

    //����������
    static bool pointCloseEnough( Point4 a, Point4 b )
    {
        return closeEnough( a.x, b.x )
            && closeEnough( a.y, b.y )
            && closeEnough( a.z, b.z )
            && closeEnough( a.w, b.w );
    }

    static bool pointCloseEnough( Point3 a, Point3 b )
    {
        return closeEnough( a.x, b.x )
            && closeEnough( a.y, b.y )
            && closeEnough( a.z, b.z );
    }

    static bool pointCloseEnough( Point2 a, Point2 b )
    {
        return closeEnough( a.x, b.x )
            && closeEnough( a.y, b.y );
    }

    //������������ȡ��нǽӽ�Ϊ0
    static bool directionCloseEnough( Point3 a, Point3 b )
    {
        // 0.02 == 2 * pi / 360 (roughly);
        return (DotProd( a, b ) > cos(0.02)) || (a == b);
    }

    //�ж����������Ƿ�������
    static bool closeEnough( const BloatVertex& a, const BloatVertex& b )
    {
        return  pointCloseEnough( a.pos, b.pos ) &&
            directionCloseEnough( a.normal, b.normal ) &&
            pointCloseEnough( a.uv, b.uv ) &&
            pointCloseEnough( a.uv2, b.uv2 ) &&
            pointCloseEnough( a.colour, b.colour ) &&
            //		a.smoothingGroup == b.smoothingGroup &&
            directionCloseEnough( a.binormal, b.binormal ) &&
            directionCloseEnough( a.tangent, b.tangent ) &&
            a.meshIndex == b.meshIndex;
    }



    /**
    *	This method removes duplicate vertices.
    */
    void VisualMesh::removeDuplicateVertices()
    {
        // Raymond : Merge TC's optimisation {
#ifdef TC_OPTIMISATION
        int j1 = 0;
        // In VS 2002, hash_multimap is inside namespace std
        // In VS 2003, hash_multimap is inside namespace stdext
        // I cannot find an elegant way for that, so MACRO is the rescue
#if	_MSC_VER < 1300
#error this file is intended to be compiled with VC 2002 or above
#endif//_MSC_VER < 1300

        std::hash_multimap <uint64, int> vertexMap;
        std::hash_multimap <uint64, int>::const_iterator lowerBound, upperBound, iter;
        std::hash_map <int, int> vertexRemap;

        typedef std::pair <uint64, int> Int64_Pair;
        typedef std::pair <int, int> Int_Pair;

        //������j1�ӽ��Ķ���found
        //vertexRemap(j1->found)������Ὣ����������Ϊj1�ĵ����Ϊfound
        //vertexMap ��¼���Ѿ�������Ķ��㣬�����Ĵ�����vertexMapΪ��������

        for (int j1 = 0; j1 < vertices_.size(); j1++)
        {
            BloatVertex& bv1 = vertices_[j1];
            uint64 key = hashKey( bv1.pos );
            int found = -1;

            //��lowerBound��upperBound֮����ҽӽ��ĵ�
            lowerBound = vertexMap.lower_bound( key );
            upperBound = vertexMap.upper_bound( key );
            for (iter = lowerBound; iter != upperBound; iter++)
            {
                int hashedIndex = iter->second;
                BloatVertex& bv2 = vertices_[ hashedIndex ];
                if (closeEnough( bv1, bv2 ))
                {
                    found = hashedIndex;
                    break;
                }
            }

            if (found == -1)
            {
                vertexMap.insert( Int64_Pair( key, j1 ) );
                vertexRemap.insert( Int_Pair( j1, j1 ) );
            }
            else
                vertexRemap.insert( Int_Pair( j1, found ) );
        }
#endif//TC_OPTIMISATION
        // }

        //���µ��������ε�����
        int nDupes = 0;
        // iterate through all triangles
        for (int i = 0; i < (int)triangles_.size(); ++i)
        {
            Triangle& tri = triangles_[i];
            // Raymond : Merge TC's optimisation {
#ifdef TC_OPTIMISATION
            tri.index[0] = vertexRemap[tri.index[0]];
            tri.index[1] = vertexRemap[tri.index[1]];
            tri.index[2] = vertexRemap[tri.index[2]];
#endif//TC_OPTIMISATION
#ifndef TC_OPTIMISATION
            // }


            // for each vertex
            for (int j = 0; j < 3; ++j)
            {
                // find if there is an earlier vertex that matches
                for (int k = 0; k < tri.index[j]; ++k)
                {
                    // make the triangle point at that one instead
                    if (closeEnough( vertices_[tri.index[j]], vertices_[k] ))
                        tri.index[j] = k;
                }
            }
            // Raymond : Merge TC's optimisation {
#endif//TC_OPTIMISATION
            // }
        }

        // (sortTriangles will removed the unreferenced vertices)
    }

    bool VisualMesh::save(const std::string& primitiveFile, const BoundingBox & bb)
    {
        //��ת������˳��max������ϵΪ��������ϵ����dx�෴��
        flipTriangleWindingOrder();

        //����������
        sortTriangles();

        //ɾ���ظ�������ǳ������Ķ���
        removeDuplicateVertices();
        sortTriangles();

        //�����ʽ������η���
        PGVector primGroups;
        Moo::IndicesHolder indices;
        if (!createPrimitiveGroups( primGroups, indices ))
            return false;

        QuickFileWriter writer;
        writer << MeshMagic << MeshVersion;

        //д����������
        Moo::IndexHeader ih; //����������
        largeIndices_ = indices.entrySize() != 2;
        sprintf_s( ih.indexFormat_, sizeof(ih.indexFormat_), largeIndices_ ? "list32" : "list" );//ȷ������������
        ih.nIndices_ = indices.size();//�������
        ih.nTriangleGroups_ = primGroups.size();//�������

        writer << ih;
        writer.write( indices.indices(), indices.size() * indices.entrySize() );
        writer << primGroups;//���������

        //д�붥������
        VertexVector vertices;
        createVertexList( vertices );
        Moo::VertexHeader vh;
        vh.nVertices_ = vertices.size();
        sprintf_s( vh.vertexFormat_, sizeof(vh.vertexFormat_), "xyznuv" );
        writer << vh;
        writer << vertices;

        //д���Χ��
        writer << bb.min_ << bb.max_;

        const std::string & bin = writer.getData();
        DEBUG_MSG("Save mesh with %d bytes.", bin.length());

        //������.mesh�ļ�
        if(!writeToFile(primitiveFile, bin, true))
        {
            ERROR_MSG("Failed write primitive file '%s'!", primitiveFile.c_str());
            return false;
        }
        else
        {
            DEBUG_MSG("Succed write primitive file '%s'!", primitiveFile.c_str());
        }

        //������Ϣ

        std::string mtlFile = primitiveFile;
        size_t pos = mtlFile.find_last_of('.');
        if(pos != mtlFile.npos)
        {
            mtlFile.erase(pos);
        }
        mtlFile += ".mtl";

        FileSreamPtr stream = new FileSream("root");

        MaterialVector::iterator it = materials_.begin();
        MaterialVector::iterator end = materials_.end();
        uint32 primGroup = 0;
        for (uint32 i = 0; i < materials_.size(); i++)
        {
            Material& mat = materials_[i];
            if (mat.inUse) mat.save( stream, primGroup++ );
        }

        if(!FileSream::saveStream(stream, mtlFile))
        {
            ERROR_MSG("Failed save material '%s'.", mtlFile.c_str());
        }

        return true;
    }

    //�˷������Լ���ӵ�Ŀ��visual mesh��
    /**
    *	This method adds this visual mesh to destination visual mesh.
    *
    *	@param	destMesh		The visual mesh we are being added to.
    *	@param	forceMeshIndex	Flag force mesh index.
    *	@param	worldSpace		Flag transform to world space.
    */
    void VisualMesh::add( VisualMesh & destMesh, int forceMeshIndex, bool worldSpace )
    {
        uint32 i;

        //�ϲ�����
        // find or add all our materials to the dest mesh
        RemapVector		reremap;
        for (i = 0; i < materials_.size(); i++)
        {
            reremap.push_back( destMesh.findOrAddMaterial( materials_[i] ) );
        }

        if (reremap.empty())
        {
            reremap.push_back( 0 );
        }

        //�ϲ�����
        // append our transformed vertices to its vertices
        uint32 startVertex = destMesh.vertices_.size();//����buffer���
        for (i = 0; i < vertices_.size(); i++)
        {
            BloatVertex bv = vertices_[i];
            if (worldSpace)
                bv.pos = world_.PointTransform( bv.pos );
            bv.normal = Normalize( world_.VectorTransform( bv.normal ) );
            if (forceMeshIndex != -1)
                bv.meshIndex = forceMeshIndex;
            destMesh.vertices_.push_back( bv );
        }

        //�ϲ���
        // append our remapped and shuffled triangles to its triangles
        for (i = 0; i < triangles_.size(); i++)
        {
            Triangle tr = triangles_[i];
            tr.index[0] += startVertex;
            tr.index[1] += startVertex;
            tr.index[2] += startVertex;
            tr.materialIndex = reremap[ tr.materialIndex ];
            destMesh.triangles_.push_back( tr );
        }
    }

    //�򶥵��б����һ������
    /**
    *	This method adds a vertex to our vertex list.
    *
    *	@param bv Vertex to add to the vertexlist.
    *	@return Index of the vertex in the vector.
    */
    int VisualMesh::addVertex( const BloatVertex& bv )
    {
        if( verticeSetMap.find( bv ) != verticeSetMap.end() )
        {
            std::set<BVVector::size_type>& verticeSet = verticeSetMap[ bv ];
            for( std::set<BVVector::size_type>::iterator iter = verticeSet.begin();
                iter != verticeSet.end(); ++iter )
            {
                if( vertices_[ *iter ] == bv )
                {
                    BloatVertex& mbv = vertices_[ *iter ];
                    mbv.smoothingGroup |= bv.smoothingGroup;
                    return *iter;
                }
            }
        }
        vertices_.push_back( bv );
        verticeSetMap[ bv ].insert( vertices_.size() - 1 );
        return vertices_.size() - 1;
    }


    /**
    *	This method checks the material suffix to check if the material type
    *	matches the visual type, i.e. it checks that non-skinned meshes do not
    *	use skinned materials and vice-verse.
    *
    *	@param fxPostfix	The suffix to check for.
    *	@return				Success or failure.
    */
    bool VisualMesh::checkDXMaterials( const std::string& fxPostfix )
    {
        uint32 i;
        for (i = 0; i < materials_.size(); i++)
        {
            if ( (ExportSettings::instance().exportMode() != ExportSettings::NORMAL) && materials_[i].fxFile_.length())
            {
                std::string fxName;
                if (!materials_[i].checkFilename(fxName,fxPostfix))
                {
                    MessageBox( GetForegroundWindow(),
                        "VisualMesh::Material::save - Skinned shader applied to a non-skinned mesh and the required shader variant doesn't exist.\n",
                        "Visual Exporter", MB_OK | MB_ICONEXCLAMATION );
                    return false;
                }
            }
        }
        return true;
    }


    /**
    *	This method initalises the visual mesh.
    *
    *	@param node			The node of the mesh we want to export.
    *	@param checkMats	Flag to check the material of the node.
    *	@return				Success or failure.
    */
    bool VisualMesh::init( INode* node, std::string exportFromPath, std::string exportToPath, bool checkMats )
    {
        verticeSetMap.clear();
        verticeIndexSetMap.clear();

        bool needDel = false;
        ConditionalDeleteOnDestruct<TriObject> triObject( MFXExport::getTriObject( node, timeNow(), needDel));
        if (! (&*triObject))
            return false;
        triObject.del( needDel );
        Mesh* mesh = &triObject->mesh;

        bool isStatic = ExportSettings::instance().exportMode() == ExportSettings::STATIC ||
            ExportSettings::instance().exportMode() == ExportSettings::STATIC_WITH_NODES;

        visibility_ = node->GetVisibility(timeNow()); 

        bool ret = false;
        if (mesh->getNumFaces() && mesh->getNumVerts())
        {
            identifier_ = trimWhitespaces( node->GetName() );

            gatherMaterials( node, exportFromPath, exportToPath );

            int mapCount = mesh->getNumMaps();

            bool hasUVs = mesh->getNumTVerts() && mesh->tvFace;		

            UVVert * vertexAlpha = mesh->mapVerts( MAP_ALPHA );
            TVFace * vertexAlphaFaces = mesh->mapFaces( MAP_ALPHA );

            UVVert * uv2MapVerts = mesh->mapVerts(2);
            TVFace * uv2MapFaces = mesh->mapFaces(2);

            dualUV_ = hasUVs && mapCount > 1 && uv2MapVerts && uv2MapFaces;

            vertexColours_ = (mesh->getNumVertCol() && mesh->vcFace) || (vertexAlpha && vertexAlphaFaces);
            if (checkMats && !checkDXMaterials(""))
                return false;

            if ( !hasUVs &&
                (!(ExportSettings::instance().exportMode() == ExportSettings::MESH_PARTICLES)) )
            {
                std::string warningMessage(
                    "VisualMesh::init - Bump mapping is enabled but mesh '" +
                    identifier_ +
                    "' has no UV coordinates.\n" );
                MessageBox( GetForegroundWindow(),
                    warningMessage.c_str(),
                    "Visual Exporter", MB_OK | MB_ICONEXCLAMATION );

                return false;
            }



            BloatVertex bv;
            bv.normal = Point3::Origin;
            bv.uv = Point2::Origin;
            bv.uv2 = Point2::Origin;
            bv.colour = Point4::Origin;
            bv.binormal = Point3::Origin;
            bv.tangent = Point3::Origin;

            Matrix3 meshMatrix = node->GetObjectTM( timeNow() );
            Matrix3 nodeMatrix = node->GetNodeTM( timeNow() );
            if( !ExportSettings::instance().allowScale() )
                nodeMatrix = normaliseMatrix( nodeMatrix );

            Matrix3 scaleMatrix = isStatic ? meshMatrix : meshMatrix * Inverse( nodeMatrix );

            vertexPositions_.assign( mesh->verts, mesh->verts + mesh->getNumVerts() );

            for (int i = 0; i < mesh->getNumFaces(); i++)
            {
                Face f = mesh->faces[ i ];

                Triangle tr;
                tr.materialIndex = materialRemap_[ f.getMatID() % materialRemap_.size() ];
                tr.smoothingGroup = f.smGroup;
                for (int j = 0; j < 3; j++)
                {
                    bv.pos = scaleMatrix * mesh->verts[ f.v[j] ];
                    bv.vertexIndex = f.v[j];
                    bv.smoothingGroup = f.smGroup;
                    if (hasUVs)
                    {
                        UVVert * mapVerts = mesh->mapVerts(1);
                        TVFace * mapFaces = mesh->mapFaces(1);

                        bv.uv = reinterpret_cast<Point2&>( mapVerts[ mapFaces[i].t[j] ] );
                        bv.uv.y = 1 - bv.uv.y;
                    }
                    if (dualUV_)
                    {
                        bv.uv2 = reinterpret_cast<Point2&>( uv2MapVerts[ uv2MapFaces[i].t[j] ] );
                        bv.uv2.y = 1 - bv.uv2.y;
                    }
                    if (vertexColours_)
                    {
                        Point3 colour = (mesh->getNumVertCol() && mesh->vcFace) ? reinterpret_cast<Point3&>( mesh->vertCol[ mesh->vcFace[i].t[j] ] ) :
                            Point3(1.f,1.f,1.f);
                        float alpha = vertexAlpha ? vertexAlpha[ vertexAlphaFaces[i].t[j] ].x : 1.0f;
                        bv.colour = Point4( colour, alpha );
                    }
                    bv.meshIndex = 0;

                    if (snapVertices_)
                        bv.pos = snapPoint3( bv.pos );
                    tr.index[j] = addVertex( bv );
                    tr.realIndex[j] = bv.vertexIndex;
                }
                triangles_.push_back( tr );
            }

            bool nodeMirrored = isStatic ? false : isMirrored( nodeMatrix );
            bool meshMirrored = isMirrored( meshMatrix );

            if (nodeMirrored ^ meshMirrored)
                flipTriangleWindingOrder();

            /*		if (isMirrored( meshMatrix ))
            flipTriangleOrder();

            if (isMirrored( nodeMatrix ))
            flipTriangleWindingOrder();*/

            for( BVVector::size_type i = 0; i < vertices_.size(); ++i )
                verticeIndexSetMap[ vertices_[ i ].vertexIndex ].push_back( i );

            createNormals( );
            getNormalsFromModifier( node );

            if (isStatic)
            {
                world_.IdentityMatrix();
            }
            else
            {
                world_ = nodeMatrix;
            }

            for (uint32 i = 0; i < vertices_.size(); i++)
            {
                if (snapVertices_)
                    bb_.addBounds( reinterpret_cast<const Vector3&>( snapPoint3( world_ * vertices_[ i ].pos ) ) );
                else
                    bb_.addBounds( reinterpret_cast<const Vector3&>( world_ * vertices_[ i ].pos ) );
            }

            if (nodeMirrored)
                flipTriangleWindingOrder();

            ret = true;
        }

        return ret;
    }

}//end namespace MeshExporter