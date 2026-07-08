#define LOG_TAG "Renderer"

/**
 * Renderer.cpp
 *
 * Author: Muhammad Abdullah Bin Ahmad
 *
 * Creates, initializes, resizes, renders, and clears the scene models.
 */

#include "Renderer.h"
#include "Square.h"
#include "Triangle.h"

Renderer& Renderer::Instance()
{
    static Renderer instance;
    return instance;
}

Renderer::~Renderer()
{
    clearModels();
}

#ifdef PLATFORM_ANDROID
void Renderer::setAssetManager(AAssetManager* mgr)
{
    assetMgr = mgr;
}
#endif

bool Renderer::initializeRenderer()
{
#ifdef PLATFORM_ANDROID
    if (!assetMgr) {
        LOGE("Renderer: asset manager is null");
        return false;
    }
#endif

    createModels();
    initializeModels();
    return true;
}

void Renderer::resize(int w, int h)
{
    for (Model* model : models) {
        model->Resize(w, h);
    }
}

void Renderer::render()
{
    if (!models.empty()) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    for (Model* model : models) {
        model->Render();
    }
}

void Renderer::createModels()
{
    clearModels();

#ifdef PLATFORM_ANDROID
    models.push_back(new Triangle(assetMgr));
    models.push_back(new Square(assetMgr));
#else
    models.push_back(new Triangle());
    models.push_back(new Square());
#endif
}

void Renderer::initializeModels()
{
    for (Model* model : models) {
        model->InitModel();
    }
}

void Renderer::clearModels()
{
    for (Model* model : models) {
        delete model;
    }
    models.clear();
}
