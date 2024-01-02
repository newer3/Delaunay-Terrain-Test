#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <glm.hpp>
#include <algorithm>

#include"shader.h"
#include"camera.h"
#include"DeTriangulate.h"
#include"KDTreeSort.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

std::vector<glm::vec3> readData(const std::string& filename);

// settings
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);


    Shader ourShader("shader.vs", "shader.fs");
    std::vector<glm::vec3>points = readData("C:/Users/29206/Desktop/23学习记录/terraintest/data/terrain data.txt");
    /*KDTree* tree = new KDTree();
    tree->buildKdTree(points);
    tree->getSortPoints();
    points = tree->sortData;*/
    //std::cout << "排序后点云数据" << std::endl;
   // for (int i = 0; i < points.size(); i++) std::cout << points[i].x << " " << points[i].y << std::endl;

    DT mesh;
    mesh.genMesh(points);
    float* vertices = new float[points.size() * 3];
    for (int i = 0; i < points.size(); i++) {
        vertices[i * 3] = points[i].x * 5;
        vertices[i * 3 + 1] = points[i].y * 5;
        vertices[i * 3 + 2] = points[i].z;
    }
    int* indices = new int[mesh.result.size() * 3];
    for (int i = 0; i < mesh.result.size(); i++) {
        indices[i * 3] = mesh.result[i][0];
        indices[i * 3 + 1] = mesh.result[i][1];
        indices[i * 3 + 2] = mesh.result[i][2];
    }
    std::cout << "顶点数量:" << points.size() << std::endl;
    std::cout << "三角面片数量:" << mesh.result.size() << std::endl;
    std::cout << "合理化次数:" << mesh.rationcount << "   " << "定位次数:" << mesh.localcount << std::endl;
    //for (int j = 0; j < points.size() * 3; j++) std::cout << vertices[j] << std::endl;
    //for (int j = 0; j < mesh.result.size() * 3; j++) std::cout << indices[j] << std::endl;
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, points.size() * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.result.size() * 3 * sizeof(int), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);





    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ourShader.use();


        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        // draw our first triangle
        glBindVertexArray(VAO);
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        ourShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, mesh.result.size() * 3, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    delete[] vertices;
    delete[] indices;

    glfwTerminate();
    return 0;

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

std::vector<glm::vec3> readData(const std::string& filename) {
    std::vector<glm::vec3> points;
    std::ifstream file(filename);
    if (file.is_open()) {
        glm::vec3 point;
        while (file >> point.x >> point.y >> point.z) {
            if (point.x <= 81 && point.x >= 80 && point.y <= 31) {
                point.x -= 80;
                point.y -= 30;
                points.push_back(point);
            }

        }
        file.close();
        std::cout << "Data successfully read from " << filename << std::endl;
    }
    else {
        std::cerr << "Unable to open file for reading: " << filename << std::endl;
    }
    float minZ = 5000;
    float maxZ = 0;
    for (int i = 0; i < points.size(); i++) {
        if (points[i].z > maxZ) maxZ = points[i].z;
        if (points[i].z < minZ) minZ = points[i].z;
    }
    for (int i = 0; i < points.size(); i++) {
        (points[i].x - 0.5) * 2;
        (points[i].y - 0.5) * 2;
        points[i].z = (points[i].z - minZ) / (maxZ - minZ);
    }

    return points;
}