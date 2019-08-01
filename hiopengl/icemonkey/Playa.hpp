#ifndef PLAYA_H
#define PLAYA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>
#include <iostream>

struct Pair {
    float X;
    float Y;
};

const Pair defaultInitialPosition = {.X = 0.0f, .Y = 0.0f};

class Playa
{
public:
    std::vector<Pair> destinations;
    
    Playa(Pair initialPosition = defaultInitialPosition)
    {
        xPos = initialPosition.X;
        yPos = initialPosition.Y;
    }
    
    void move(float deltaTime) {
        if (counter < (int)destinations.size()) {
            if (!enRoute) {
                float startX = xPos;
                float startY = yPos;

                if (counter > -1) {
                    startX = destinations[counter].X;
                    startY = destinations[counter].Y;
                }
                
                totalDistanceX = destinations[counter + 1].X - startX;
                totalDistanceY = destinations[counter + 1].Y - startY;
                std::cout << "resetting counter to " << counter << ", moving X, Y by " << totalDistanceX << ", " << totalDistanceY << std::endl;
                
                totalXThisStep = 0.0f;
                totalYThisStep = 0.0f;
                enRoute = true;
                counter += 1;
            }

            checkAndMove(deltaTime);
        }
    }
    
    glm::vec3 getTranslationMatrix() {
        return glm::vec3(xPos, yPos, 0.0f);
    }

    
private:
    int counter = -1;
    float xPos = 0.0f;
    float yPos = 0.0f;
    float totalDistanceX = 0.0f;
    float totalDistanceY = 0.0f;
    float totalXThisStep = 0.0f;
    float totalYThisStep = 0.0f;
    float totalTime = 10.0f;
    bool enRoute = false;

    void checkAndMove(float deltaTime)
    {
        // move X, Y
        if (abs(totalDistanceX - totalXThisStep) > 0.01f || abs(totalDistanceY - totalYThisStep) > 0.01f) {
            float stepX = (deltaTime / totalTime) * totalDistanceX;
            float stepY = (deltaTime / totalTime) * totalDistanceY;
            
            totalXThisStep += stepX;
            totalYThisStep += stepY;
            xPos += stepX;
            yPos += stepY;
        } else {
            enRoute = false;
        }
    }
};
#endif
