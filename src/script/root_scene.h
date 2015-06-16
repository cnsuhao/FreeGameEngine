//
//  root_scene.h
//  client
//
//  Created by zhoubao on 14-9-23.
//
//

#ifndef __client__root_scene__
#define __client__root_scene__

#include "core/mesh_node.h"
#include "graphics/render_target.h"

typedef SmartPtr<class RootScene> RootScenePtr;

namespace ora
{

class RootScene : public ora::SceneNode
{
public:
    RootScene();
    ~RootScene();
    
    static RootScene * create();

    virtual bool init() override;
    virtual void tick(float elapse) override;
    virtual void draw() override;

    void renderAxis();
    void testRendertarget();

private:
    ora::RenderTargetPtr testRenderTarget_;
};

}// end namespace ora
    
#endif /* defined(__client__root_scene__) */
