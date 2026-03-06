#include "../include/window.h"
#include <iostream>

OpenWindow::OpenWindow(int w, int  h, const std::string& t) : width(w), height(h), title(t) {
    if (!glfwInit()) {
        exit(-1);
    }

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if (!window) {
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    glewInit();
}

OpenWindow::~OpenWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool OpenWindow::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void OpenWindow::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenWindow::display() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void OpenWindow::maximize() {
    glfwMaximizeWindow(window);
}



