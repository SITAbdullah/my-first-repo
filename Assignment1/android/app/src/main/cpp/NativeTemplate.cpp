/**
 * NativeTemplate.cpp
 *
 * Author: Muhammad Abdullah Bin Ahmad
 *
 * Thin JNI bridge between the Java/Android layer and the shared scene renderer.
 *
 * Native method signatures match MainActivity.java:
 *   package com.example.glpiframeworkintro
 *   class   MainActivity
 */

#define LOG_TAG "GLPIFrameworkIntroNative"

#include <jni.h>
#include <android/asset_manager_jni.h>
#include "Platform.h"
#include "Renderer.h"

// ---------------------------------------------------------------------------
// JNI entry points
// ---------------------------------------------------------------------------

extern "C" JNIEXPORT jboolean JNICALL
Java_com_example_helloworldandroid_MainActivity_nativeInit(
        JNIEnv* env, jobject /*thiz*/, jobject assetManager)
{
    LOGI("nativeInit called");
    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
    Renderer::Instance().setAssetManager(mgr);
    return Renderer::Instance().initializeRenderer() ? JNI_TRUE : JNI_FALSE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_helloworldandroid_MainActivity_nativeResize(
        JNIEnv* /*env*/, jobject /*thiz*/, jint width, jint height)
{
    Renderer::Instance().resize(width, height);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_helloworldandroid_MainActivity_nativeRender(
        JNIEnv* /*env*/, jobject /*thiz*/)
{
    Renderer::Instance().render();
}
