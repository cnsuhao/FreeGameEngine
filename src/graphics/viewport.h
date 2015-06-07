#ifndef VIEWPORT_H_
#define VIEWPORT_H_

namespace ora
{

    class Viewport
    {
    public:
        Viewport();
        Viewport(int x, int y, int width, int height);
        ~Viewport();
        
        float aspect() const { return float(width_) / height_; }

    public:
        int x_;
        int y_;
        int width_;
        int height_;
    };

    bool operator == (const Viewport & a, const Viewport & b);

}//end namespace ora

#endif //VIEWPORT_H_