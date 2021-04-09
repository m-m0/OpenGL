/*
shader:
a programm **that runs on the gpu** and not on the cpu
a block of code
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource {
  std::string VertexSource;
  std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
    // open file
    std::ifstream stream(filepath);

    enum class ShaderType {
      NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while(getline(stream, line)) {
      // npos, invalid string position
      if (line.find("#shader") != std::string::npos) {
        if (line.find("vertex") != std::string::npos) {
          //set mode to vertex
          type = ShaderType::VERTEX;
        } else if (line.find("fragment") != std::string::npos){
          //set mode to fragment
          type = ShaderType::FRAGMENT;
        }
      } else {
        ss[(int)type] << line << '\n';
      }
    }
    return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(const std::string& source, unsigned int type) {
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str(); //&source[0] ist dats gleiche
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);

  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    // alloca dynamically allocates on the stack
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compile"  << (type== GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
    std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;

  }

  return id;
}


static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
  unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

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

    ShaderProgramSource source = ParseShader("../res/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);




        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
