#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>

/* 
 * Puts the thee floats in the string str into an array given by buff
 */
static int parse_vector (char * str, GLfloat * buff)
{
	char * _str = malloc(sizeof(char) * 80);
	int i = 0;
	char * wd = malloc(sizeof(char) * 20);

	if (_str == NULL || wd == NULL) 
		goto cleanup;

	strcpy(_str, str);
	strtok(_str, " ");

	while ((wd = strtok(NULL, " "))) {
		buff[i] = atof(wd);
		i++;
	}

cleanup:
	free(_str);
	free(wd);

	return 0;
}

static int parse_face (char * str, GLuint * buff)
{
	char * _str = malloc(sizeof(char) * 80);
	int i = 0;
	char * wd = malloc(sizeof(char) * 20);

	if (_str == NULL || wd == NULL) 
		goto cleanup;

	strcpy(_str, str);
	strtok(_str, " ");

	while ((wd = strtok(NULL, " "))) {
		buff[i] = atoi(strtok(wd, "/")) - 1;
		i++;
	}

cleanup:
	free(_str);
	free(wd);

	return 0;
}

/* 
 * Reads obj file and deposits vertices into float array
 */
size_t read_obj (const char * filename, GLfloat * vertices, GLuint * faces,
		GLuint * facec)
{
	char buf[80];
	int vertc = 0;
	GLfloat * _verts = vertices;
	GLuint * _faces = faces;

	FILE * m = fopen(filename, "r");
	if (m == NULL) {
		fprintf(stderr, "Could not open file %s\n", filename);
		return 1;
	}

	while (fgets(buf, sizeof(buf), m)) {
		if (buf[0] == 'v' && buf[1] == ' ') {
			parse_vector(buf, _verts);
			_verts += 3;
			vertc++;
		} else if (buf[0] == 'f') {
		}

	}

	fclose(m);

	return vertc;
}

int main ()
{
	/*
	size_t vertlen = 0;
	int i;
	GLuint * facec = 0;

	GLfloat * vertices = malloc(128 * sizeof(GLfloat));
	GLuint * faces = malloc(128 * sizeof(GLuint));
	vertlen = read_obj("models/sphere.obj", vertices, faces, facec);

	printf("%i\n", (int) vertlen);

	for (i = 0; i < (int) (vertlen * 3); i += 3) {
		printf("%f, ", *(vertices + i));
		printf("%f, ", *(vertices + i + 1));
		printf("%f\n", *(vertices + i + 2));
	}
	*/


	GLuint * faces = malloc(3 * sizeof(GLuint));
	parse_face("f 1//5 20//5 17//5", faces);

	printf("%i, %i, %i\n", (int) *faces, (int) *(faces+1), (int) *(faces+2));

	exit(EXIT_SUCCESS);
}
