//
//  hockey.hpp
//  hiopengl
//
//  Created by tienv on 2019-07-31.
//  Copyright © 2019 tienv. All rights reserved.
//

#ifndef hockey_hpp
#define hockey_hpp

#include <stdio.h>
#include <GLFW/glfw3.h>

#endif /* hockey_hpp */

namespace IceHockey {
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void processInput(GLFWwindow *window);
    int main();
}
