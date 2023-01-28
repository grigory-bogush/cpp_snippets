#include <GL/gl.h>
#include <GL/glut.h>

void renderTriangle() {
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.5, -0.5);
        glVertex2f(0.5, -0.5);
        glVertex2f(0.0, 0.5);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    renderTriangle();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutCreateWindow("Triangle");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
