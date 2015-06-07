//
//  root_scene.h
//  client
//
//  Created by zhoubao on 14-9-23.
//
//

#ifndef __client__ground_node__
#define __client__ground_node__

#include "mesh_node.h"

namespace ora
{
    class GroundNode : public SceneNode
    {
    public:
        GroundNode();
        ~GroundNode();

        void setBackGround(const std::string & material, const std::string & key);

        const std::string & getBackGroundMtl(){ return backGroundMtr_; };
        const std::string & getBackGroundKey(){ return backGroundKey_; };

        virtual void draw() override;

    private:
        ora::MeshPtr    background_;
        std::string     backGroundMtr_;
        std::string     backGroundKey_;
    };

    typedef SmartPtr<GroundNode> GroundNodePtr;

} // end namespace ora

#endif /* defined(__client__ground_node__) */
