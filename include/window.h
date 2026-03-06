#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class OpenWindow {
private:
    GLFWwindow* window;
    int width;
    int height;
    std::string title;

public:
    OpenWindow(int w, int h, const std::string& t);
    ~OpenWindow();

    bool shouldClose() const;
    void clear(float r, float g, float b, float a);
    void display();
    void maximize();
};