#ifndef _TRANS_H
#define _TRANS_H

typedef struct {
	GLfloat x;
	GLfloat y;
	GLfloat z;
} vec3;

/*
 * Deposits the values in a vec3 into the first 3 places in an array
 */
void vec3_array (vec3 in, GLfloat * array);

/*
 * Defines a matrix transformation to move the camera position
 */
void look_at (vec3 eye, vec3 centre, vec3 up, GLfloat * mat4);

/*
 * Defines a perspective projection matrix transformation
 */
void perspective (GLfloat fovy, GLfloat asp, GLfloat znear, GLfloat zfar,
		GLfloat * mat4);

/*
 * Defines a matrix transformation to rotate around the Z axis
 */
void rotatez (GLfloat ang, GLfloat * mat4);

/*
 * Fills a mat4 with zeros
 */
void zeros (GLfloat * mat4);

/*
 * Creates a 4x4 identity matrix
 */
void identity (GLfloat * mat4);

/*
 * Creates a matrix to move a vector by the vector pos
 */
void translate (vec3 pos, GLfloat * mat4);

#endif /* _TRANS_H */
