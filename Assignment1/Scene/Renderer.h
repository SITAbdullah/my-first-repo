#pragma once

/*!***************************************************************************
 * \file   Renderer.h
 * \brief  Singleton controller that owns the active Model(s) and is the
 *         only class main.cpp / NativeTemplate.cpp talk to. Does not know
 *         about Triangle or Square directly (only Model*) so new shapes
 *         can be added without touching this header.
 *****************************************************************************/

#include "Model.h"
#include <vector>

#ifdef PLATFORM_ANDROID
#include <android/asset_manager.h>
#endif

class Renderer {
public:
    static Renderer& Instance();
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

#ifdef PLATFORM_ANDROID
    void setAssetManager(AAssetManager* mgr);
#endif

    bool initializeRenderer();
    void resize(int w, int h);
    void render();

private:
    Renderer() = default;
    ~Renderer();

    void createModels();
    void initializeModels();
    void clearModels();

#ifdef PLATFORM_ANDROID
    AAssetManager* assetMgr = nullptr;
#endif

    std::vector<Model*> models; // replaces the single Model* model
};