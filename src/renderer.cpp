#include "renderer.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graph.hpp"

static GLFWwindow* window;
static int windowWidth, windowHeight;

static unsigned int vbo, vao;
static unsigned int shaderProgram;

static std::vector<float> mesh;

/* -------------------------------------------------------------------------- */

static void framebufferSizeCallback(GLFWwindow* window, int newWidth, int newHeight) {
    glViewport(0, 0, newWidth, newHeight);
    windowWidth = newWidth;
    windowHeight = newHeight;
}

static void initBuffers() {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(float), mesh.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);
}

static unsigned int loadShader(GLenum shaderKind, const std::string& shaderPath) {
    std::string shaderSource;

    std::ifstream file(shaderPath);
    if (!file)
        throw ConfigError("failed to load shader");

    std::string line;
    while (std::getline(file, line)) {
        shaderSource += line += '\n';
    }
    
    unsigned int shader = glCreateShader(shaderKind);
    const char* shaderSourcePtr = shaderSource.c_str();
    glShaderSource(shader, 1, &shaderSourcePtr, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        throw ConfigError(std::format("failed to compile shader at {}: {}", shaderPath, infoLog));
    }

    return shader;
}

static void initShaders() {
    unsigned int vertexShader = loadShader(GL_VERTEX_SHADER, "./src/shaders/vertex.glsl");
    unsigned int fragmentShader = loadShader(GL_FRAGMENT_SHADER, "./src/shaders/fragment.glsl");

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        throw ConfigError(std::format("failed to compile program: {}", infoLog));
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);
}

/* -------------------------------------------------------------------------- */

void initRenderer(std::unique_ptr<ASTNode>& expr, int width, int height) {
    windowWidth = width;
    windowHeight = height;

    if (!glfwInit()) {
        throw ConfigError("failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, "Graph2D", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw ConfigError("failed to create window");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw ConfigError("failed to initialize GLAD");
    }    

    glViewport(0, 0, windowWidth, windowHeight);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    mesh = buildGraphMesh(expr, -20, 20, -20, 20);
    if (mesh.size() == 0) {
        glfwTerminate();
        throw ConfigError("failed to create graph mesh: nothing to draw");
    }

    initBuffers();
    initShaders();

    unsigned int uResolution = glGetUniformLocation(shaderProgram, "uResolution");
    glUniform1f(uResolution, RESOLUTION);
}

void runRenderLoop() {
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_LINES, 0, mesh.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /* ---------------------------------------------------------------------- */

    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);

    glDeleteProgram(shaderProgram);

    glfwTerminate();
}