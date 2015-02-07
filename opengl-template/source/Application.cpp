/**
 * @file Application.cpp
 *
 * OpenGL Template
 */

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include "Application.h"
#include <iostream>
#include "ShaderSource.h"

Application::Application()
{
	// Initialisation

	fade_factor = 0.12f;

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	GLint result_ok = false;
	GLfloat vertexData[] = { 
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, 1.0f
	};

	GLushort indices[] { 0, 1, 2, 3 };

	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	textures[0] = make_texture("assets/textures/hello1.tga");
	textures[1] = make_texture("assets/textures/hello2.tga");

	assert(textures[0] != 0 || textures[1] != 0);

	program = glCreateProgram();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string shaderPath = ShaderSource::load("assets/shaders/trivial_vertex.glsl");
	const char* temp = shaderPath.c_str();

	glShaderSource(vertexShader, 1, &temp, nullptr);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result_ok);
	if (!result_ok) {
		fprintf(stderr, "Failed to compile %s:\n", shaderPath);
		show_info_log(vertexShader, glGetShaderiv, glGetShaderInfoLog);
		glDeleteShader(vertexShader);
	}

	shaderPath = ShaderSource::load("assets/shaders/trivial_fragment.glsl");
	temp = shaderPath.c_str();

	glShaderSource(fragmentShader, 1, &temp, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result_ok);
	if (!result_ok) {
		fprintf(stderr, "Failed to compile %s:\n", shaderPath);
		show_info_log(fragmentShader, glGetShaderiv, glGetShaderInfoLog);
		glDeleteShader(fragmentShader);
	}

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &result_ok);
	if (!result_ok) {
		fprintf(stderr, "Failed to link shader program:\n");
		show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
		glDeleteProgram(program);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	uniforms.fade_factor = glGetUniformLocation(program, "fade_factor");
	uniforms.textures[0] = glGetUniformLocation(program, "textures[0]");
	uniforms.textures[1] = glGetUniformLocation(program, "textures[1]");

	attributes.position = glGetAttribLocation(program, "position");
}

Application::~Application()
{
	// Deinitialisation
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &elementBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteProgram(program);
}

void Application::update()

{
	// Updating and drawing
	glUseProgram(program);

	glUniform1f(uniforms.fade_factor, fade_factor);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glUniform1i(uniforms.textures[0], 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glUniform1i(uniforms.textures[1], 1);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(attributes.position,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(GLfloat) * 2,
		(void*)0
		);
	glEnableVertexAttribArray(attributes.position);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glDrawElements(
		GL_TRIANGLE_STRIP, 
		4, 
		GL_UNSIGNED_SHORT, 
		(void*)0
		);

	glDisableVertexAttribArray(attributes.position);
}

GLuint Application::make_texture(const char *filename)
{
	GLuint texture;
	int width, height;
	void *pixels = read_tga(filename, &width, &height);

	if (!pixels)
		return 0;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(
		GL_TEXTURE_2D, 0,
		GL_RGB8,
		width, height, 0,
		GL_BGR, GL_UNSIGNED_BYTE,
		pixels
	);

	free(pixels);

	return texture;
}

short Application::le_short(unsigned char *bytes)
{
	return bytes[0] | ((char)bytes[1] << 8);
}

void* Application::read_tga(const char *filename, int *width, int *height)
{
	struct tga_header {
		char id_length;
		char color_map_type;
		char data_type_code;
		unsigned char color_map_origin[2];
		unsigned char color_map_length[2];
		char color_map_depth;
		unsigned char x_origin[2];
		unsigned char y_origin[2];
		unsigned char width[2];
		unsigned char height[2];
		char bits_per_pixel;
		char image_descriptor;
	} header;
	int i, color_map_size, pixels_size;
	FILE *f;
	size_t read;
	void *pixels;
	f = fopen(filename, "rb");
	if (!f) {
		fprintf(stderr, "Unable to open %s for reading\n", filename);
		return NULL;
	}
	read = fread(&header, 1, sizeof(header), f);
	if (read != sizeof(header)) {
		fprintf(stderr, "%s has incomplete tga header\n", filename);
		fclose(f);
		return NULL;
	}
	if (header.data_type_code != 2) {
		fprintf(stderr, "%s is not an uncompressed RGB tga file\n", filename);
		fclose(f);
		return NULL;
	}
	if (header.bits_per_pixel != 24) {
		fprintf(stderr, "%s is not a 24-bit uncompressed RGB tga file\n", filename);
		fclose(f);
		return NULL;
	}
	for (i = 0; i < header.id_length; ++i)
		if (getc(f) == EOF) {
			fprintf(stderr, "%s has incomplete id string\n", filename);
			fclose(f);
			return NULL;
		}
	color_map_size = le_short(header.color_map_length) * (header.color_map_depth / 8);
	for (i = 0; i < color_map_size; ++i)
		if (getc(f) == EOF) {
			fprintf(stderr, "%s has incomplete color map\n", filename);
			fclose(f);
			return NULL;
		}
	*width = le_short(header.width); *height = le_short(header.height);
	pixels_size = *width * *height * (header.bits_per_pixel / 8);
	pixels = malloc(pixels_size);
	read = fread(pixels, 1, pixels_size, f);
	fclose(f);
	if (read != pixels_size) {
		fprintf(stderr, "%s has incomplete image\n", filename);
		free(pixels);
		return NULL;
	}
	return pixels;
}

void Application::show_info_log(
	GLuint object,
	PFNGLGETSHADERIVPROC glGet__iv,
	PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
	)
{
	GLint log_length;
	char *log;

	glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
	log = (char*)malloc(log_length);
	glGet__InfoLog(object, log_length, NULL, log);
	fprintf(stderr, "%s", log);
	free(log);
}