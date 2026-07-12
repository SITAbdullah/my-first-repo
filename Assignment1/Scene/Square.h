#pragma once

/**
 * Square.h
 *
 * Author: Muhammad Abdullah Bin Ahmad
 *
 * Concrete Model implementation for a colored square rendered beside the
 * triangle. The square owns its shader program, VAO, and VBOs.
 */

#include "Platform.h"
#include "Model.h"

#ifdef PLATFORM_ANDROID
#include <android/asset_manager.h>
#endif

class Square : public Model {
public:
#ifdef PLATFORM_ANDROID
    explicit Square(AAssetManager* assetMgr);
#else
    Square();
#endif
    ~Square() override;

    void InitModel() override;
    void Render() override;
    void Resize(int w, int h) override;

private:
#ifdef PLATFORM_ANDROID
    AAssetManager* mgr = nullptr;
#endif
    GLuint programID = 0;
    GLuint vao = 0;
    GLuint vboPos = 0;
    GLuint vboColor = 0;
    GLint uRadianAngle = -1;
    float degree = 0.0f;
};
