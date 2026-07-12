#pragma once

/*!***************************************************************************
 * \file   Model.h
 * \brief  Abstract base class for all renderable models. Each concrete Model subclass owns its
 *         own shader program, VAO, and VBOs independently — no GPU resources are shared between models. 
 *         Each Model is responsible for its own initialization, rendering, and resizing.
 *****************************************************************************/

class Model {
public:
    Model() {}
    virtual ~Model() {}
    
    virtual void InitModel() = 0; // called once, before the first frame
    virtual void Render() = 0;    // called once per frame
    virtual void Resize(int w, int h) {} // called whenever the surface/window resizes
};