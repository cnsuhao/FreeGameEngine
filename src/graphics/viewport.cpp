#include "viewport.h"

namespace ora
{

    Viewport::Viewport()
        : x_(0)
        , y_(0)
        , width_(0)
        , height_(0)
    {}

    Viewport::Viewport(int x, int y, int width, int height)
        : x_(x)
        , y_(y)
        , width_(width)
        , height_(height)
    {}

    Viewport::~Viewport()
    {}

    bool operator == (const Viewport & a, const Viewport & b)
    {
        return (a.x_ == b.x_) && (a.y_ == b.y_) && (a.width_ == b.width_) && (a.height_ == b.height_);
    }
}