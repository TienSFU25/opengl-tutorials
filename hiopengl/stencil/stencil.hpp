//
//  stencil.hpp
//  hiopengl
//
//  Created by tienv on 2019-07-20.
//  Copyright © 2019 tienv. All rights reserved.
//

#ifndef stencil_hpp
#define stencil_hpp
#include <GLFW/glfw3.h>

#include <stdio.h>

#endif /* stencil_hpp */

namespace Stencil {
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void processInput(GLFWwindow *window);
    
    int main();
}
