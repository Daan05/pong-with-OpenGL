#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "shaders_class.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void update();

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
bool gameOver = false;

float p1_offset = 0.0f;
float p2_offset = 0.0f;
float p1_points = 0;
float p2_points = 0;

float ball_x = 0.0f;
float ball_y = 0.0f;
float ball_x_vel = 0.01f;
float ball_y_vel = 0.0f;
float ball_radius = 0.02;

float gamespeed = 1.0f;

const float MAX_ANGLE = 3.14159265358979323846 / 3;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions                // colors
        // ball - 0_12
         0.000f,  0.000f, 0.000f,   1.0f, 1.0f, 1.0f, // center
         0.019f,  0.009f, 0.000f,   1.0f, 1.0f, 1.0f, // outside
         0.014f,  0.024f, 0.000f,   1.0f, 1.0f, 1.0f, // 
         0.005f,  0.033f, 0.000f,   1.0f, 1.0f, 1.0f, // 
        -0.005f,  0.033f, 0.000f,   1.0f, 1.0f, 1.0f, // 
        -0.014f,  0.024f, 0.000f,   1.0f, 1.0f, 1.0f, // 
        -0.019f,  0.009f, 0.000f,   1.0f, 1.0f, 1.0f, // 
        -0.019f, -0.009f, 0.000f,   1.0f, 1.0f, 1.0f, // 
        -0.014f, -0.024f, 0.000f,   1.0f, 1.0f, 1.0f, // 
        -0.005f, -0.033f, 0.000f,   1.0f, 1.0f, 1.0f, // 
         0.005f, -0.033f, 0.000f,   1.0f, 1.0f, 1.0f, // 
         0.014f, -0.024f, 0.000f,   1.0f, 1.0f, 1.0f, // 
         0.019f, -0.009f, 0.000f,   1.0f, 1.0f, 1.0f, // 
         
        // handles - 13_20
        // left handle
        -1.00f,  0.2f, 0.0f,        1.0f, 0.0f, 0.0f,  // top left
        -1.00f, -0.2f, 0.0f,        1.0f, 0.0f, 0.0f,  // bottom left
        -0.98f, -0.2f, 0.0f,        1.0f, 0.0f, 0.0f,  // bottom right
        -0.98f,  0.2f, 0.0f,        1.0f, 0.0f, 0.0f,  // top right
        // right handle
         1.00f,  0.2f, 0.0f,        0.0f, 0.0f, 1.0f,  // top right
         1.00f, -0.2f, 0.0f,        0.0f, 0.0f, 1.0f,  // bottom right
         0.98f, -0.2f, 0.0f,        0.0f, 0.0f, 1.0f,  // bottom left
         0.98f,  0.2f, 0.0f,        0.0f, 0.0f, 1.0f,  // top left

         // score - 21_35
         // uncolered
         // p1
        -0.9f,  0.90f, 0.0f,         0.2f, 0.2f, 0.2f,
        -0.9f,  0.82f, 0.0f,         0.2f, 0.2f, 0.2f,
        -0.6f,  0.82f, 0.0f,         0.2f, 0.2f, 0.2f,
        -0.6f,  0.90f, 0.0f,         0.2f, 0.2f, 0.2f,
        // p2                          
         0.9f,  0.90f, 0.0f,         0.2f, 0.2f, 0.2f,
         0.9f,  0.82f, 0.0f,         0.2f, 0.2f, 0.2f,
         0.6f,  0.90f, 0.0f,         0.2f, 0.2f, 0.2f,
         0.6f,  0.82f, 0.0f,         0.2f, 0.2f, 0.2f,

         // colored
         // p1
        -0.9f,  0.90f, 0.0f,         1.0f, 0.0f, 0.2f,
        -0.9f,  0.82f, 0.0f,         1.0f, 0.0f, 0.2f,
        -0.9f,  0.82f, 0.0f,         1.0f, 0.0f, 0.4f,
        -0.9f,  0.90f, 0.0f,         1.0f, 0.0f, 0.4f,
        // p2                          
         0.6f,  0.90f, 0.0f,         0.2f, 0.0f, 1.0f,
         0.6f,  0.82f, 0.0f,         0.2f, 0.0f, 1.0f,
         0.6f,  0.90f, 0.0f,         0.4f, 0.0f, 1.0f,
         0.6f,  0.82f, 0.0f,         0.4f, 0.0f, 1.0f

    };

    unsigned int indices[] = {
        // ball
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 6,
        0, 6, 7,
        0, 7, 8,
        0, 8, 9,
        0, 9, 10,
        0, 10, 11,
        0, 11, 12,
        0, 12, 1,

        // left handle
        13, 14, 15, // first triangle
        15, 16, 13, // second triangle
        // right handle
        17, 18, 19, // third triangle
        19, 20, 17,  // fourth triangle

        // left score
        21, 22, 23,
        21, 23, 24,
        // right score
        25, 26, 27,
        26, 27, 28,
        // left score
        29, 30, 31,
        29, 31, 32,
        // right score
        33, 34, 35,
        34, 35, 36
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        update();

        if (!gameOver)
        {
            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        
            // render container
            ourShader.use();
            ourShader.setFloat("p1_offset", p1_offset);
            ourShader.setFloat("p2_offset", p2_offset);
            ourShader.setFloat("p1_points", p1_points);
            ourShader.setFloat("p2_points", p2_points);
            ourShader.setFloat("ball_x", ball_x);
            ourShader.setFloat("ball_y", ball_y);

            glBindVertexArray(VAO);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // check if one of players has 5 points
            if (p1_points == 5 || p2_points == 5)
            {
                // game freezes
                gameOver = true;
            }
            glfwSwapBuffers(window);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // keys to move the handles
    // P1
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)       { p1_offset += gamespeed * 0.01f; }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)       { p1_offset -= gamespeed * 0.01f; }
    // P2
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)      { p2_offset += gamespeed * 0.01f; }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)    { p2_offset -= gamespeed * 0.01f; }

    // R to restart
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        // reset all vars
        p1_offset = 0.0f;
        p2_offset = 0.0f;
        p1_points = 0;
        p2_points = 0;

        ball_x = -0.5f;
        ball_y = 0.0f;
        ball_x_vel = 0.01f;
        ball_y_vel = 0.0f;
        gamespeed = 1.0f;

        gameOver = false;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void update()
{
    p1_offset = fmax(-0.8, fmin(p1_offset, 0.8));
    p2_offset = fmax(-0.8, fmin(p2_offset, 0.8));

    ball_x += ball_x_vel;
    ball_y += ball_y_vel;

    if (ball_y > 1.0 - ball_radius && ball_y_vel > 0.0f) { ball_y_vel *= -1; } // hit top of screen
    else  if (ball_y < -1.0 + ball_radius && ball_y_vel < 0.0f) { ball_y_vel *= -1; } // hit bottom of screen
    else if (ball_x < -0.98f && fabs(p1_offset - ball_y) < 0.22f && ball_x_vel < 0.0f) // ball hit p1 handle
    {
        float relativeIntersectY = ball_y - p1_offset;
        float normalizedRelativeIntersectionY = (relativeIntersectY / 0.2);
        float bounceAngle = normalizedRelativeIntersectionY * MAX_ANGLE;
        ball_x_vel = (gamespeed * 0.01) * std::cosf(bounceAngle);
        ball_y_vel = (gamespeed * 0.01) * std::sinf(bounceAngle);
    } 
    else if (ball_x > 0.98f && fabs(p2_offset - ball_y) < 0.22f && ball_x_vel > 0.0f) // ball hit p2 handle
    {  
        float relativeIntersectY = ball_y - p2_offset;
        float normalizedRelativeIntersectionY = (relativeIntersectY / 0.2);;
        float bounceAngle = normalizedRelativeIntersectionY * MAX_ANGLE;
        ball_x_vel = (gamespeed * 0.01) * std::cos(bounceAngle) * -1;
        ball_y_vel = (gamespeed * 0.01) * std::sin(bounceAngle);
    } 

    else if (ball_x < -1.0 && ball_x_vel < 0.0f) // ball out of bounds - point for p2
    {
        p2_points++;
        if (p2_points == 5) { return; }
        
        p1_offset = 0.0f;
        p2_offset = 0.0f;

        ball_x = -0.5f;
        ball_y = 0.0f;
        ball_x_vel = 0.01f;
        ball_y_vel = 0.0f;

        gamespeed = 1.0f;
    } 
    else if (ball_x > 1.0 && ball_x_vel > 0.0f) // ball out of bounds - point for p1
    {
        p1_points++;
        if (p1_points == 5) { return; }

        p1_offset = 0.0f;
        p2_offset = 0.0f;

        ball_x = 0.5f;
        ball_y = 0.0f;
        ball_x_vel = -0.01f;
        ball_y_vel = 0.0f;

        gamespeed = 1.0f;
    }

    gamespeed += 0.0001;
}
