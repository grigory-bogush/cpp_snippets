#include <GL/gl.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 triangle_vertecies[3] = {
    {-0.5, -0.5, 1.0f},
    {0.5, -0.5, 1.0f},
    {0.0, 0.5, 1.0f},
};
float angle_rad = 1.0f;// 1/PI of a circle
glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle_rad, glm::vec3(0.0f, 0.0f, 1.0f));

void renderTriangle() {

    // Transform the triangle's vertices
    for (int i = 0; i < 3; i++) {
        glm::vec4 vertex = glm::vec4(triangle_vertecies[i], 1.0f);
        vertex = rotationMatrix * vertex;
        triangle_vertecies[i] = glm::vec3(vertex);
    }


    glBegin(GL_TRIANGLES);
        for (auto vertex : triangle_vertecies) {
            glVertex3f(vertex[0], vertex[1], vertex[2]);
        }
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
