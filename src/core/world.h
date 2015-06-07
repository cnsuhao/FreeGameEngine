//
//  world.h
//  my3d_libs
//
//  Created by zhoubao on 14-3-31.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef __my3d_libs__World__
#define __my3d_libs__World__

#include "scene_node.h"
#include "camera.h"
#include "util/object.h"

namespace ora
{
    class World :
        public Object,
        public Singleton<World>
    {
    public:
        World();
        ~World();

		void    setRoot(SceneNodePtr root, bool cleanup = true);
		SceneNode * getRoot() const { return root_.get(); }

        bool    init();
        void    fini();
        void    update();

        void    setPause(bool pause) {  pause_ = pause ? 1 : 0; }
        bool    getPause() const { return pause_ != 0; }
        
        float   getElapse() const { return elapse_; }
        uint32  getElapse_ms() const { return elapse_ms_; }
        float   getFps() const { return fps_; }
        uint32  getTotalFrame() const { return totalFrame_; }
        
        void    setCamera(CameraPtr pCamera);
        Camera* getCamera() const { return camera_.get(); }

        float   getTimeScale() const { return timeScale_; }
        void    setTimeScale(float s) { timeScale_ = s; }

        Vector3 screenPosToWorld(const Vector2 & pos) const;
        Vector3 worldPosToProj(const Vector3 & pos) const;
        Vector2 worldPosToScreen(const Vector3 & pos) const;

        Vector2 getViewSize() const;

		void	setLineMode(bool b){ lineMode_ = b ? 1 : 0; };
		bool	getLineMode() const { return lineMode_ != 0; };

    private:
        void updateTime();
		void updateTime_ms();
        void updateCamera();
        
		void doUpdate();
		void doRender();

        int             pause_;
        SceneNodePtr    root_;
        float           elapse_;
        float           timeScale_;
        uint32          elapse_ms_;
        float           fps_;
        uint32          totalFrame_;
        CameraPtr       camera_;

		int	            lineMode_;
    };

}//end namespace ora


#endif /* defined(__my3d_libs__World__) */
