#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 300
#define WINDOW_TITLE "sparse-buffer-1"

int main(int argc, const char** argv)
{
  if (!glfwInit()) {
    printf("Unable to initialize GLFW.\n");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, 0, 0);

  if (!window) {
    printf("Unable to create GLFW window.\n");
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    printf("Unable to load glad.\n");
    exit(EXIT_FAILURE);
  }

  if (!GLAD_GL_ARB_sparse_buffer) {
    printf("GL_ARB_sparse_buffer extension not supported.\n");
    exit(EXIT_FAILURE);
  }

  printf("Initialized OpenGL %d.%d core context.\n", GLVersion.major, GLVersion.minor);

  GLuint buffer_id;
  glCreateBuffers(1, &buffer_id);
  glNamedBufferStorage(buffer_id, 2 * 1000 * 1000 * 1000, 0, GL_DYNAMIC_STORAGE_BIT | GL_SPARSE_STORAGE_BIT_ARB);

  GLint page_size;
  glGetIntegerv(GL_SPARSE_BUFFER_PAGE_SIZE_ARB, &page_size);

  const GLsizeiptr alloc_size = page_size * 20;

  // Crashes or doesn't do anything.
  glNamedBufferPageCommitmentARB(buffer_id, 0, alloc_size, GL_TRUE);

  // This works.
  //glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer_id);
  //glBufferPageCommitmentARB(GL_SHADER_STORAGE_BUFFER, 0, alloc_size, GL_TRUE);
  //glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

  // Write values in the buffer.
  char clear_value = 5;
  glClearNamedBufferData(buffer_id, GL_R8I, GL_RED_INTEGER, GL_BYTE, &clear_value);

  // Let's extract the last page and check its results.
  char* data = malloc(page_size);
  glGetNamedBufferSubData(buffer_id, page_size * 19, page_size, data);

  int error = 0;
  for (int i = 0; i < page_size; ++i) {
    error |= (data[i] != clear_value);
    printf("%d", data[i]);
  }
  printf("\n");
  free(data);

  glDeleteBuffers(1, &buffer_id);

  glfwDestroyWindow(window);
  glfwTerminate();

  if (error) {
    printf("Error: page faulty.\n");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
