#include "util.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include <SDL2/SDL_image.h>

static GLuint load_shader(char *path, GLenum type);

GLuint load_program(char *vertex_path, char *tess_control_path, char *tess_evaluation_path, char *geometry_path, char *fragment_path)
{
	GLuint vertex_shader, tess_control_shader, tess_evaluation_shader, geometry_shader, fragment_shader;

	GLuint program = glCreateProgram();

	vertex_shader = load_shader(vertex_path, GL_VERTEX_SHADER);
	glAttachShader(program, vertex_shader);

	if (tess_control_path != NULL) {
		tess_control_shader = load_shader(tess_control_path, GL_TESS_CONTROL_SHADER);
		glAttachShader(program, tess_control_shader);
	}
	if (tess_evaluation_path != NULL) {
		tess_evaluation_shader = load_shader(tess_evaluation_path, GL_TESS_EVALUATION_SHADER);
		glAttachShader(program, tess_evaluation_shader);
	}
	if (geometry_path != NULL) {
		geometry_shader = load_shader(geometry_path, GL_GEOMETRY_SHADER);
		glAttachShader(program, geometry_shader);
	}
	if (fragment_path != NULL) {
		fragment_shader = load_shader(fragment_path, GL_FRAGMENT_SHADER);
		glAttachShader(program, fragment_shader);
	}

	glLinkProgram(program);

	GLint info_length;
	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_length);
	if (info_length > 0) {
		char *buffer = (char*)malloc(info_length + 1);
		glGetProgramInfoLog(program, info_length, NULL, buffer);
		printf("%s", buffer);
	}

	glDetachShader(program, vertex_shader);
	glDetachShader(program, fragment_shader);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);


	return program;
}

GLuint load_texture(char *path)
{
	SDL_Surface *image = NULL;
	image = IMG_Load(path);
	if (image == NULL) {
		printf("File %s could not be loaded.\n", path);
	}

	SDL_Surface *surface = SDL_CreateRGBSurface(0,
						image->w, image->h,
						32,
						0x000000ff,
						0x0000ff00,
						0x00ff0000,
						0xff000000);
	SDL_BlitSurface(image, NULL, surface, NULL);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_FreeSurface(surface);
	SDL_FreeSurface(image);

	return texture;
}

void handle_gl_errors()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		switch (error) {
			case GL_INVALID_ENUM:
				printf("Invalid enum.\n");
				break;
			case GL_INVALID_VALUE:
				printf("Invalid value.\n");
				break;
			case GL_INVALID_OPERATION:
				printf("Invalid operation.\n");
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				printf("Invalid framebuffer operation.\n");
				break;
			case GL_OUT_OF_MEMORY:
				printf("Out of memory.\n");
				break;
			case GL_STACK_UNDERFLOW:
				printf("Underflow.\n");
				break;
			case GL_STACK_OVERFLOW:
				printf("Overflow.\n");
				break;
		}
	}
}

char *load_file(char *path)
{
	FILE *file = fopen(path, "rb");
	assert(file);
	
	int length;
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *buffer = (char*)calloc(length + 1, sizeof(char));
	assert(buffer);

	int read_length = fread(buffer, sizeof(char), length, file);
	assert(read_length == length);

	buffer[length] = '\0';

	fclose(file);

	return buffer;
}

static GLuint load_shader(char *path, GLenum type)
{
	GLuint shader_id = glCreateShader(type);
	assert(shader_id != 0);

	char *file = load_file(path);
	assert(file != NULL);

	glShaderSource(shader_id, 1, (const char**)&file, NULL);
	free(file);

	glCompileShader(shader_id);

	GLint info_length;
	GLint result;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_length);
	if (info_length > 0) {
		char *buffer = (char*)malloc(info_length + 1);
		glGetShaderInfoLog(shader_id, info_length, NULL, buffer);
		printf("From Shader file %s:\n", path);
		printf("%s", buffer);
	}
	assert(result);

	return shader_id;
}
