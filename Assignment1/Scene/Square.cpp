#define LOG_TAG "Square"

/*!***************************************************************************
 * \file   Square.cpp
 * \author zijian.tan@digipen.edu
 * \brief  Implementation of the Square Model. Renders a solid yellow square
 *         offset to the right of the triangle in clip space so both shapes
 *         are clearly visible simultaneously.
 *****************************************************************************/

#include "Square.h"
#include "ShaderHelper.h"

static const GLfloat SquarePositions[] = {
    0.62f,  0.85f,   // top-left
    0.62f,  0.45f,   // bottom-left
    0.97f,  0.45f,   // bottom-right
    0.97f,  0.85f,   // top-right
};

#ifdef PLATFORM_ANDROID
Square::Square(AAssetManager* assetMgr) : mgr(assetMgr) {}
#else
Square::Square() = default;
#endif

Square::~Square()
{
    glDeleteBuffers(1, &vboPos);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(programID);
}

void Square::InitModel()
{
    LOGI("Square::InitModel");

#ifdef PLATFORM_ANDROID
    programID = ShaderHelper::buildProgramFromAssets(mgr, "shader/SquareVertex.glsl",
                                                          "shader/SquareFragment.glsl");
#else
    programID = ShaderHelper::buildProgramFromFile("SquareVertex.glsl",
                                                    "SquareFragment.glsl");
#endif
    if (!programID) { LOGE("Square: could not create program"); return; }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vboPos);
    glBindBuffer(GL_ARRAY_BUFFER, vboPos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SquarePositions), SquarePositions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    LOGI("Square::InitModel done");
}

void Square::Render()
{
    glUseProgram(programID);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}

void Square::Resize(int w, int h)
{
    glViewport(0, 0, w, h);
}