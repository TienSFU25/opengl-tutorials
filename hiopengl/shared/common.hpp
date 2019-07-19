//
//  common.hpp
//  hiopengl
//
//  Created by tienv on 2019-07-13.
//  Copyright © 2019 tienv. All rights reserved.
//

#ifndef common_hpp
#define common_hpp

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#endif /* common_hpp */

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

namespace Utils {
    unsigned int loadTexture(const char* path);
}
