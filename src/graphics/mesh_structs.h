#ifndef MESH_FILE_STRUCTS_HPP
#define MESH_FILE_STRUCTS_HPP

#include "base.h"

namespace ora
{
    /**
     *	Header structure for the vertex values stored in a file.
     */
    struct VertexHeader
    {
        char	vertexFormat[ 64 ];
        int		nVertices;
    };


    /**
     *	Header structure for the index values stored in a file.
     */
    struct IndexHeader
    {
        char	indexFormat[ 64 ];
        int		nIndices;
        int		nTriangleGroups;
    };


    /**
     *	The primitive group structure maps to a set of indices within the index
     *	buffer.
     */
    struct PrimitiveGroup
    {
        int		startIndex;
        int		nPrimitives;
        int		startVertex;
        int		nVertices;
    };


    typedef std::vector<PrimitiveGroup> PGVector;

}//end namespace ora

#endif // MESH_FILE_STRUCTS_HPP
