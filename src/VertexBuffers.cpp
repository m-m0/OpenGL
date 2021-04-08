/*
VertexBuffer?
  just a MemoryBuffer, Array of Memory in which we push bytes
  Differenc to normal Array -> its in our GPU / VRAM

Shader:
  A Programm that runs on the GPU (in a very specific/powerfull way)

OpenGL:
  Basically a State Machine
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* after a valid OpenGL context is defined we call glewInit() to initialize the extension entry point */
    if (glewInit() != GLEW_OK) {
      std::cout << "Error" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

    //-----------------------------------------new----------------------------------------------------
    // start defining VertexBuffer
    // each line is a vertex, x,y
    // vertex = point at the geometry, vertex is way more than a position, pos, colors, ...
    float positions[6] = {
      -0.5f, -0.5f,
      0.0f, 0.5f,
      0.5f, -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    // bind buffer gets drawn
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    //glDrawElements(GL_TRIANGLE, 3)
    //-------------------------------------------------

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //new--------------------------------------------------
        glDrawArrays(GL_TRIANGLES, 0, 3);




        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
