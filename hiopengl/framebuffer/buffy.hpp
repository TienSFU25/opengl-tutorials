//
//  buffy.hpp
//  hiopengl
//
//  Created by tienv on 2019-07-24.
//  Copyright © 2019 tienv. All rights reserved.
//

#ifndef buffy_hpp
#define buffy_hpp

#include <stdio.h>

#endif /* buffy_hpp */

namespace Buffy {
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void processInput(GLFWwindow *window);
    
    int main();
}
