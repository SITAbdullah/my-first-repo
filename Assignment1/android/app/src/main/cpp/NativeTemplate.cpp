/**
 * NativeTemplate.cpp
 *
 * Thin JNI bridge between the Java/Android layer and the Renderer
 * singleton. Converts the Java AAssetManager into its native pointer
 * form and hands it to Renderer before initialization, since Triangle
 * (and later Square) need it to read .glsl shader files out of the APK.
 *
 * Native method signatures match MainActivity.java:
 *   package com.example.glpiframeworkintro
 *   class   MainActivity
 */

#define LOG_TAG "GLPIFrameworkIntroNative"

#include <jni.h>
#include <android/asset_manager_jni.h>
#include "Renderer.h"
#include "Platform.h"

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