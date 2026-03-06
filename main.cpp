#include "window.h"

int main() {
    OpenWindow window(800, 600, "Isometric");

    while (!window.shouldClose()) {
        window.clear(0.6f, 0.1f, 0.1f, 1.0f);
        window.display();
    }

    return 0;
}
