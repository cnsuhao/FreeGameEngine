//
//  root_scene.cpp
//  client
//
//  Created by zhoubao on 14-9-23.
//
//

#include "cstdmf/pch.hpp"

#include "root_scene.h"
#include "core/camera.h"
#include "graphics/graphics.h"

#include <SDL/include/SDL.h>

#include "graphics/render_target.h"
#include "core/world.h"


RootScene::RootScene()
{}

RootScene::~RootScene()
{
}

bool RootScene::init()
{
    if(!ora::SceneNode::init()) return false;

    ora::RenderDevice *device = ora::renderDev();
    
    float aspect = device->getViewPort().aspect();
    device->getCamera().setPerspective(PI_QUAR, aspect, 1.0f, 500.0f);

    ora::LightContainerPtr lights = new ora::LightContainer();
    lights->setAmbientColor(ora::Color(0.6f, 0.6f, 0.6f, 1.0f));
    
    ora::DirLightPtr dirLight = new ora::DirLight();
    dirLight->setDirection(Vector3(1.0f, 1.0f, -1.0f));
    dirLight->setColor(ora::Color(0.8f, 0.8f, 0.8f, 1.0f));
    lights->addDirLight(dirLight);
    
    device->setLightContainer(lights);

    testRenderTarget_ = new ora::RenderTarget();
    testRenderTarget_->create(device->getViewPort().width_, device->getViewPort().height_);

    return true;
}

void RootScene::tick(float elapse)
{
    ora::SceneNode::tick(elapse);

    const Uint8 * states = SDL_GetKeyboardState(NULL);
    if (states[SDL_SCANCODE_F12])
    {
        testRendertarget();
    }
}

void RootScene::draw()
{
#ifndef _RELEASE
    renderAxis();
#endif

    ora::SceneNode::draw();
}

void RootScene::renderAxis()
{
    ora::renderDev()->pushWorld(Matrix::identity);

    float radius = 2.0f;

    static ora::VertexXYZColor vertices[] =
    {
        {Vector3(-radius, 0, 0), 0xff7f0000}, {Vector3(0, 0, 0), 0xff7f0000},
        {Vector3(0, -radius, 0), 0xff007f00}, {Vector3(0, 0, 0), 0xff007f00},
        {Vector3(0, 0, -radius), 0xff00007f}, {Vector3(0, 0, 0), 0xff00007f},

        {Vector3(0, 0, 0), 0xffff0000}, {Vector3(radius, 0, 0), 0xffff0000}, 
        {Vector3(0, 0, 0), 0xff00ff00}, {Vector3(0, radius, 0), 0xff00ff00}, 
        {Vector3(0, 0, 0), 0xff0000ff}, {Vector3(0, 0, radius), 0xff0000ff}, 
    };

    ora::RenderHelper::instance()->drawLines(vertices, 12);
    
    ora::renderDev()->popWorld();
}

void RootScene::testRendertarget()
{        
    if (testRenderTarget_ && ora::renderDev()->pushRenderTarget(testRenderTarget_))
    {
        ora::renderDev()->clear(ora::ClearState::Color | ora::ClearState::Depth, ora::ColorBlue, 1, 0);

        ora::renderDev()->pushWorld(Matrix::identity);
        ora::SceneNode::draw();
        ora::renderDev()->popWorld();

        const char * imageName = "test.tga";
        if (!testRenderTarget_->getTexture()->save(imageName))
        {
            ORA_LOG_ERROR("failed save image %s", imageName);
        }
        else
        {
            ORA_LOG_INFO("save image %s", imageName);
        }
        
        ora::renderDev()->popRenderTarget();
    }
}
