#pragma once

/**
 * Fan.h
 *
 * A hierarchical table fan assembled from seven draws of one indexed cube:
 * base, pole, hub, and four independently coloured blades. The blade group
 * rotates about the hub. A tap toggles power; drag velocity boosts the spin and
 * decays after release. A stationary long press cycles 3, 4, and 5 blades.
 */

#include "Model.h"
#include "Platform.h"
#include "Transform.h"

#include <chrono>

#ifdef PLATFORM_ANDROID
#include <android/asset_manager.h>
#endif

class Fan final : public Model {
public:
#ifdef PLATFORM_ANDROID
    explicit Fan(AAssetManager* manager) : assetManager(manager) {}
#else
    Fan() = default;
#endif
    ~Fan() override;

    void InitModel() override;
    void Render() override;
    void Resize(int w, int h) override;
    void TouchEventDown(float x, float y) override;
    void TouchEventMove(float x, float y) override;
    void TouchEventRelease(float x, float y) override;

private:
    void DrawCube(const glm::vec3& colour);

    Transform transform;
    GLuint program = 0;
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ibo = 0;
    GLint mvpLocation = -1;
    GLint colourLocation = -1;

#ifdef PLATFORM_ANDROID
    AAssetManager* assetManager = nullptr;
#endif

    float spinAngle = 0.0f;
    bool fanOn = true;
    float dragBoost = 0.0f;
    float lastX = 0.0f;
    float lastY = 0.0f;
    float movedDistance = 0.0f;
    bool isDragging = false;
    int bladeCount = 4;
    std::chrono::steady_clock::time_point lastMoveTime{};
    std::chrono::steady_clock::time_point pressStartTime{};

    static constexpr float kBaseSpeed = 1.5f;
    static constexpr float kMaxBoost = 20.0f;
    static constexpr float kBoostScale = 8.0f;
    static constexpr float kTapThreshold = 12.0f;
    static constexpr float kFlingDecay = 0.98f;
    static constexpr float kLongPressMs = 600.0f;
};
