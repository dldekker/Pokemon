#ifndef UTIL_H
#define UTIL_H

#include <GL/glew.h>

GLuint load_program(char *vertex_path, char *tess_control_path, char *tess_evaluation_path, char *geometry_path, char *fragment_path);
GLuint load_texture(char *path);
void handle_gl_errors();
char *load_file(char *path);

#endif
