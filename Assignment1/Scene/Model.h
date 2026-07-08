#pragma once

/**
 * Model.h
 *
 * Author: Muhammad Abdullah Bin Ahmad
 *
 * Shared lifecycle interface for any renderable object in the scene.
 */
class Model {
public:
    Model() {}
    virtual ~Model() {}

    virtual void InitModel() = 0;
    virtual void Render() = 0;
    virtual void Resize(int w, int h) {}
};
