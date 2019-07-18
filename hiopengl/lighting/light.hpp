//
//  light.hpp
//  hiopengl
//
//  Created by tienv on 2019-07-18.
//  Copyright © 2019 tienv. All rights reserved.
//

#ifndef light_hpp
#define light_hpp

#include <stdio.h>
#include <GLFW/glfw3.h>

#endif /* light_hpp */

namespace Light {
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void processInput(GLFWwindow *window);
    int main();
}
