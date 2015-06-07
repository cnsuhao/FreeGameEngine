#pragma once

#include "max.h"
#include <stdmat.h>

#include "file_stream.h"

namespace MeshExporter
{


    inline bool operator < ( Point2 p1, Point2 p2 )
    {
        if( p1.x < p2.x )	return true;
        if( p1.x > p2.x )	return false;
        if( p1.y < p2.y )	return true;
        if( p1.y > p2.y )	return false;
        return false;
    }

    inline bool operator > ( Point2 p1, Point2 p2 )
    {
        if( p1.x > p2.x )	return true;
        if( p1.x < p2.x )	return false;
        if( p1.y > p2.y )	return true;
        if( p1.y < p2.y )	return false;
        return false;
    }

    inline bool operator < ( Point3 p1, Point3 p2 )
    {
        if( p1.x < p2.x )	return true;
        if( p1.x > p2.x )	return false;
        if( p1.y < p2.y )	return true;
        if( p1.y > p2.y )	return false;
        if( p1.z < p2.z )	return true;
        if( p1.z > p2.z )	return false;
        return false;
    }

    inline bool operator > ( Point3 p1, Point3 p2 )
    {
        if( p1.x > p2.x )	return true;
        if( p1.x < p2.x )	return false;
        if( p1.y > p2.y )	return true;
        if( p1.y < p2.y )	return false;
        if( p1.z > p2.z )	return true;
        if( p1.z < p2.z )	return false;
        return false;
    }

    inline bool operator < ( Point4 p1, Point4 p2 )
    {
        if( p1.x < p2.x )	return true;
        if( p1.x > p2.x )	return false;
        if( p1.y < p2.y )	return true;
        if( p1.y > p2.y )	return false;
        if( p1.z < p2.z )	return true;
        if( p1.z > p2.z )	return false;
        if( p1.w < p2.w )	return true;
        if( p1.w > p2.w )	return false;
        return false;
    }

    inline bool operator > ( Point4 p1, Point4 p2 )
    {
        if( p1.x > p2.x )	return true;
        if( p1.x < p2.x )	return false;
        if( p1.y > p2.y )	return true;
        if( p1.y < p2.y )	return false;
        if( p1.z > p2.z )	return true;
        if( p1.z < p2.z )	return false;
        if( p1.w > p2.w )	return true;
        if( p1.w < p2.w )	return false;
        return false;
    }

    /**
    *	The vertex format captured from 3dsMax.
    */
    struct BloatVertex
    {
        Point3	pos;
        Point3	normal;
        Point4  colour;
        Point2	uv;
        Point2	uv2;
        int		vertexIndex;
        int		smoothingGroup;
        Point3	binormal;
        Point3	tangent;
        int		meshIndex;			// for creating MeshParticles

        bool operator == ( const BloatVertex& bv ) const ;
        bool operator < ( const BloatVertex& bv ) const;
        Vector3 v3Pos() const	{ return Vector3(pos.x,pos.y,pos.z); }
    };

    /**
    *	The triangle format captured from 3dsMax.
    */
    struct Triangle
    {
        int		index[3];
        int		realIndex[3];
        int		smoothingGroup;
        int		materialIndex;
    };

    /**
    *	The material format captured from 3dsMax.
    */
    struct Material
    {
        enum MapIDMeang
        {
            MAP_NONE,
            MAP_TEXTURE,
            MAP_MFM,
        };

        std::string identifier;
        std::string mapIdentifier;//材质或者纹理的文件名称
        std::string copyFrom;//没经过处理的纹理文件
        int mapIdMeaning;	// 0 = none, 1 = bitmap, 2 = mfm  1-表示mapIdentifier是纹理文件，2-表示mapIdentifier是材质文件
        float selfIllum;
        Color ambient;
        Color diffuse;
        Color specular;
        bool inUse;

        typedef std::pair< std::string, std::string > TextureOverride;
        typedef std::pair< std::string, Point4 > VectorOverride;
        typedef std::pair< std::string, BOOL > BoolOverride;
        typedef std::pair< std::string, float > FloatOverride;
        typedef std::pair< std::string, int > IntOverride;

        typedef std::vector< TextureOverride > TextureOverrides;
        typedef std::vector< VectorOverride > VectorOverrides;
        typedef std::vector< BoolOverride > BoolOverrides;
        typedef std::vector< FloatOverride > FloatOverrides;
        typedef std::vector< IntOverride > IntOverrides;

        TextureOverrides textureOverrides_;
        VectorOverrides vectorOverrides_;
        BoolOverrides boolOverrides_;
        FloatOverrides floatOverrides_;
        IntOverrides intOverrides_;

        std::string fxFile_;

        enum SkinType
        {
            NO_SKIN,
            SOFT_SKIN
        };


        Material();

        //return all resources used
        void resources( std::vector<std::string>& retResources );

        bool operator==( const Material& m ) const;

        void save(FileSreamPtr stream, uint32 pgid);

        bool checkFilename( std::string& fxName, const std::string& fxPostfix );
    };

    struct MorphVertex
    {
        Point3	delta;
        int		vertexIndex;
    };

    struct MorphTarget
    {
        std::string	identifier;
        int			channelIndex;
        std::vector< MorphVertex > verts;
    };

    struct ExportMorphHeader
    {
        int version;
        int nMorphTargets;
    };

    struct ExportMorphTarget
    {
        char	identifier[64];
        int		channelIndex;
        int		nVertices;
    };

    struct ExportMorphVertex
    {
        float	delta[3];
        int		index;
    };

    // Type definitions
    typedef std::vector< BloatVertex >	BVVector;
    typedef std::vector< Triangle >		TriangleVector;
    typedef std::vector< Material >		MaterialVector;

}//end namespace MeshExporter
