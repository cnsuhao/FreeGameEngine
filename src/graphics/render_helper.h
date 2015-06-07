#ifndef H_RENDER_HELPER_H
#define H_RENDER_HELPER_H

#include "util/singleton.h"

#include "aabb.h"
#include "mesh.h"
#include "vertex.h"

namespace ora
{
    ///used for debug draw
    class RenderHelper : public Singleton<RenderHelper>
    {
    public:
        RenderHelper();
        ~RenderHelper();

        void drawAABB(const AABB & aabb, uint32 color=0xffffffff);

        void drawLine(const Vector3 & start, const Vector3 & end, uint32 color=0xffffffff);
        void drawLines(const std::vector<VertexXYZColor> & vertices);
        void drawLines(const VertexXYZColor* vertices, size_t size);

        void drawTriangle(const Vector3 & a, const Vector3 & b, const Vector3 & c, uint32 color=0xffffffff);
        void drawTriangles(const std::vector<VertexXYZColor> & vertices);

    private:

        MeshPtr getCubMesh();

    private:
        MeshPtr cubMesh_;
        MeshPtr lineMesh_;
        MeshPtr triangleMesh_;
    };

} // end namespace ora

#endif //H_RENDER_HELPER_H