//
//  hockey.cpp
//  hiopengl
//
//  Created by tienv on 2019-07-31.
//  Copyright © 2019 tienv. All rights reserved.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "hockey.hpp"
#include "shader_s.h"
#include "Camera.h"
#include "common.hpp"
#include "Playa.hpp"

#include <iostream>

namespace IceHockey {
    // camera
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;
    
    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    // rink sizing
    float normalizedRinkWidth = 0.9;
    float normalizedRinkHeight = 0.6;
    
    std::vector<float> initCircleVertices() {
        std::vector<float> circleVertices = {
            0.0f, 0.0f, 0.1f
        };
        
        float CIRCLE_RADIUS = 0.1f;
        int circle_points = 50;
        double dtheta = 2*  3.1416f/circle_points ;
        double theta = 0.0;
        
        for (int i = 0; i < circle_points; i++)
        {
            circleVertices.push_back(CIRCLE_RADIUS * cos(theta));
            circleVertices.push_back(CIRCLE_RADIUS * sin(theta));
            circleVertices.push_back(0.1f);
            theta += dtheta;
        }
        
        // add the first point again, to complete circle
        circleVertices.push_back(CIRCLE_RADIUS * cos(0));
        circleVertices.push_back(CIRCLE_RADIUS * sin(0));
        circleVertices.push_back(0.1f);
        
        return circleVertices;
    }
    
    int main()
    {        
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
        
        // glfw window creation
        // --------------------
        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, IceHockey::framebuffer_size_callback);
        glfwSetCursorPosCallback(window, IceHockey::mouse_callback);
        glfwSetScrollCallback(window, IceHockey::scroll_callback);
        
        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }
        
        // configure global opengl state
        // -----------------------------
        glEnable(GL_DEPTH_TEST);
        
        // build and compile our shader zprogram
        // ------------------------------------
        std::string loc =  "/Users/user/Documents/361/opengl/hiopengl/hiopengl/icemonkey/";
        std::string F1 = loc + "rink.vs";
        std::string F2 = loc + "rink.fs";
        std::string F3 = loc + "circle.fs";
        
        Shader lightingShader(F1, F2);
        Shader circleShader(F1, F3);

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        std::vector<float> planeVertices = {
            -normalizedRinkWidth, -normalizedRinkHeight,  0.0f,  0.0f, 0.0f, // bottom-left
            normalizedRinkWidth, -normalizedRinkHeight,  0.0f,  1.0f, 0.0f, // bottom-right
            normalizedRinkWidth,  normalizedRinkHeight,  0.0f,  1.0f, 1.0f, // top-right
            normalizedRinkWidth,  normalizedRinkHeight,  0.0f,  1.0f, 1.0f, // top-right
            -normalizedRinkWidth,  normalizedRinkHeight,  0.0f,  0.0f, 1.0f, // top-left
            -normalizedRinkWidth, -normalizedRinkHeight,  0.0f,  0.0f, 0.0f, // bottom-left
        };
        
        int numAttribPerVertex = 5;
        int numPlaneVertices = (int)planeVertices.size() / numAttribPerVertex;
        
        // configure plane VAO (and VBO)
        unsigned int planeVBO, planeVAO;
        glGenVertexArrays(1, &planeVAO);
        glGenBuffers(1, &planeVBO);
        
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, planeVertices.size() * sizeof(float), &planeVertices[0], GL_STATIC_DRAW);
        
        glBindVertexArray(planeVAO);
        
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // texture attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        // circle vertex
        std::vector<float> circleVertices = initCircleVertices();
        int numCircleVertices = (int)circleVertices.size() / 3;
        std::cout << numCircleVertices << std::endl;
        
        // circle uses different verties and VAO
        unsigned int circleVBO, circleVAO;
        glGenBuffers(1, &circleVBO);
        glGenVertexArrays(1, &circleVAO);

        glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
        glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), &circleVertices[0], GL_STATIC_DRAW);

        glBindVertexArray(circleVAO);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // load textures
        unsigned int someTexture = Utils::loadTexture("/Users/user/Documents/361/opengl/hiopengl/hiopengl/icemonkey/plain-wallpaper.jpg");

        std::vector<Pair> destinations;
        std::vector<Pair> destinations2;

        Pair a = {.X = 1.0f, .Y = 0.0f};
        Pair b = {.X = 0.0f, .Y = 1.0f};
        
        Pair s2 = {.X = -0.75f, .Y = 0.5f};
        Pair c = {.X = -1.0f, .Y = 0.0f};
        Pair d = {.X = 0.0f, .Y = -1.0f};
        
        destinations.push_back(a);
        destinations.push_back(b);
        destinations2.push_back(c);
        destinations2.push_back(d);
        
        Playa someGuy;
        someGuy.destinations = destinations;
        Playa someOtherGuy(s2);
        someOtherGuy.destinations = destinations2;
        
        std::vector<Playa> allPlayas;
        allPlayas.push_back(someGuy);
        allPlayas.push_back(someOtherGuy);
        
        // render loop
        // -----------
        while (!glfwWindowShouldClose(window))
        {
            // per-frame time logic
            // --------------------
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            for (auto& element : allPlayas) {
                element.move(deltaTime);
            }
            
            // input
            // -----
            IceHockey::processInput(window);
            
            // render
            // ------
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // be sure to activate shader when setting uniforms/drawing objects
            lightingShader.use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, someTexture);

            // mvp
            glm::mat4 model = glm::mat4(1.0f);
            lightingShader.setMat4("model", model);
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            lightingShader.setMat4("projection", projection);
            lightingShader.setMat4("view", view);
            
            glBindVertexArray(planeVAO);
            glDrawArrays(GL_TRIANGLES, 0, numPlaneVertices);
            
            // draw the circles
            circleShader.use();
            circleShader.setMat4("projection", projection);
            circleShader.setMat4("view", view);

            for (auto& playa: allPlayas) {
                glm::mat4 circleModel = glm::mat4(1.0f);
                circleModel = glm::translate(circleModel, playa.getTranslationMatrix());
                circleShader.setMat4("model", circleModel);
            
                glBindVertexArray(circleVAO);
                glDrawArrays(GL_TRIANGLE_FAN, 0, numCircleVertices);
            }
            
            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        
        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &planeVAO);
        glDeleteVertexArrays(1, &circleVAO);
        glDeleteBuffers(1, &circleVBO);
        glDeleteBuffers(1, &planeVBO);
        
        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        glfwTerminate();
        return 0;
    }
    
    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void processInput(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    
    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }
    
    
    // glfw: whenever the mouse moves, this callback is called
    // -------------------------------------------------------
    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
        
        lastX = xpos;
        lastY = ypos;
        
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
    
    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    // ----------------------------------------------------------------------
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        camera.ProcessMouseScroll(yoffset);
    }
    
}
