#define LOG_TAG "Fan3D"
#include "Fan.h"
#include "ShaderHelper.h"

#include <algorithm>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>

Fan::~Fan()
{
    if (ibo) glDeleteBuffers(1, &ibo);
    if (vbo) glDeleteBuffers(1, &vbo);
    if (vao) glDeleteVertexArrays(1, &vao);
    if (program) glDeleteProgram(program);
}

void Fan::InitModel()
{
    static const GLfloat positions[] = {
        -0.5f, -0.5f,  0.5f,   0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,  -0.5f,  0.5f, -0.5f
    };
    static const GLfloat shades[] = { 0.85f, 0.90f, 1.00f, 0.95f, 0.60f, 0.65f, 0.75f, 0.70f };
    static const GLushort indices[] = {
        0,1,2, 0,2,3,  1,5,6, 1,6,2,  5,4,7, 5,7,6,
        4,0,3, 4,3,7,  3,2,6, 3,6,7,  4,5,1, 4,1,0
    };

#ifdef PLATFORM_ANDROID
    program = ShaderHelper::buildProgramFromAssets(
        assetManager, "shader/FanVertex.glsl", "shader/FanFragment.glsl");
#else
    program = ShaderHelper::buildProgramFromFile("FanVertex.glsl", "FanFragment.glsl");
#endif
    if (!program) {
        LOGE("Unable to create fan shader program");
        return;
    }

    mvpLocation = glGetUniformLocation(program, "MODELVIEWPROJECTIONMATRIX");
    colourLocation = glGetUniformLocation(program, "PARTCOLOR");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    const GLsizeiptr positionBytes = sizeof(positions);
    const GLsizeiptr shadeBytes = sizeof(shades);
    glBufferData(GL_ARRAY_BUFFER, positionBytes + shadeBytes, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, positionBytes, positions);
    glBufferSubData(GL_ARRAY_BUFFER, positionBytes, shadeBytes, shades);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0,
                          reinterpret_cast<const void*>(positionBytes));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);

    transform.TransformInit();
    transform.TransformSetMatrixMode(VIEW_MATRIX);
    transform.TransformLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    modelType = CubeType;
}

void Fan::DrawCube(const glm::vec3& colour)
{
    const glm::mat4* mvp = transform.TransformGetModelViewProjectionMatrix();
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(*mvp));
    glUniform3fv(colourLocation, 1, glm::value_ptr(colour));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);
}

void Fan::Render()
{
    if (!program) return;
    if (!isDragging) {
        dragBoost *= kFlingDecay;
        if (dragBoost < 0.01f) dragBoost = 0.0f;
    }
    if (fanOn) {
        spinAngle += kBaseSpeed + dragBoost;
        if (spinAngle >= 360.0f) spinAngle = std::fmod(spinAngle, 360.0f);
    }

    glUseProgram(program);
    glBindVertexArray(vao);
    transform.TransformSetMatrixMode(MODEL_MATRIX);
    transform.TransformLoadIdentity();
    transform.TransformTranslate(0.0f, 0.8f, -8.0f);
    transform.TransformRotate(glm::radians(20.0f), 0.0f, 1.0f, 0.0f);

    transform.TransformPushMatrix();
    transform.TransformTranslate(0.0f, -2.6f, 0.0f);
    transform.TransformScale(1.6f, 0.25f, 0.8f);
    DrawCube({0.45f, 0.28f, 0.12f});
    transform.TransformPopMatrix();

    transform.TransformPushMatrix();
    transform.TransformTranslate(0.0f, -1.21f, 0.0f);
    transform.TransformScale(0.15f, 2.53f, 0.15f);
    DrawCube({0.55f, 0.55f, 0.58f});
    transform.TransformPopMatrix();

    transform.TransformPushMatrix();
    transform.TransformTranslate(0.0f, 0.2f, 0.0f);
    transform.TransformScale(0.3f, 0.3f, 0.3f);
    DrawCube({0.20f, 0.20f, 0.22f});
    transform.TransformPopMatrix();

    static const glm::vec3 bladeColours[] = {
        {0.90f, 0.12f, 0.10f}, {0.10f, 0.30f, 0.90f},
        {1.00f, 0.48f, 0.05f}, {0.12f, 0.72f, 0.20f},
        {0.62f, 0.18f, 0.82f}
    };
    const float bladeSpacing = 360.0f / static_cast<float>(bladeCount);
    for (int i = 0; i < bladeCount; ++i) {
        transform.TransformPushMatrix();
        transform.TransformTranslate(0.0f, 0.2f, 0.15f);
        transform.TransformRotate(glm::radians(spinAngle + i * bladeSpacing), 0.0f, 0.0f, 1.0f);
        transform.TransformTranslate(0.0f, 0.55f, 0.0f);
        transform.TransformScale(0.22f, 0.8f, 0.05f);
        DrawCube(bladeColours[i]);
        transform.TransformPopMatrix();
    }
    glBindVertexArray(0);
}

void Fan::Resize(int w, int h)
{
    if (h <= 0) h = 1;
    transform.TransformSetMatrixMode(PROJECTION_MATRIX);
    transform.TransformLoadIdentity();
    transform.TransformSetPerspective(glm::radians(60.0f),
                                      static_cast<float>(w) / static_cast<float>(h),
                                      0.01f, 1000.0f, 0.0f);
    transform.TransformSetMatrixMode(VIEW_MATRIX);
    transform.TransformLoadIdentity();
}

void Fan::TouchEventDown(float x, float y)
{
    lastX = x;
    lastY = y;
    movedDistance = 0.0f;
    isDragging = true;
    pressStartTime = std::chrono::steady_clock::now();
    lastMoveTime = pressStartTime;
}

void Fan::TouchEventMove(float x, float y)
{
    const auto now = std::chrono::steady_clock::now();
    const float dx = x - lastX;
    const float dy = y - lastY;
    const float distance = std::sqrt(dx * dx + dy * dy);
    const float dtMs = std::chrono::duration<float, std::milli>(now - lastMoveTime).count();
    movedDistance += distance;
    if (dtMs > 0.0f) {
        const float velocity = distance / dtMs;
        dragBoost = std::min(velocity * kBoostScale, kMaxBoost);
        LOGI("Drag velocity %.3f px/ms, boost %.3f deg/frame", velocity, dragBoost);
    }
    lastX = x;
    lastY = y;
    lastMoveTime = now;
}

void Fan::TouchEventRelease(float x, float y)
{
    const auto now = std::chrono::steady_clock::now();
    const float dx = x - lastX;
    const float dy = y - lastY;
    movedDistance += std::sqrt(dx * dx + dy * dy);
    isDragging = false;
    const float heldMs = std::chrono::duration<float, std::milli>(now - pressStartTime).count();
    if (movedDistance < kTapThreshold && heldMs >= kLongPressMs) {
        dragBoost = 0.0f;
        bladeCount = bladeCount == 5 ? 3 : bladeCount + 1;
        LOGI("Blade count %d", bladeCount);
    } else if (movedDistance < kTapThreshold) {
        dragBoost = 0.0f;
        fanOn = !fanOn;
        LOGI("Fan %s", fanOn ? "ON" : "OFF");
    } else {
        LOGI("Fling boost %.3f deg/frame", dragBoost);
    }
}
