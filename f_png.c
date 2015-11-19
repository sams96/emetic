#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <zlib.h>
#include <png.h>

#define PNG_SIGBYTES 8

/*
 * Checks the signature of the png file being read at stream
 */
static GLuint readpng_checksig (FILE * stream)
{
	GLubyte sig[PNG_SIGBYTES];
	fread(sig, 1, PNG_SIGBYTES, stream);
	return !png_check_sig(sig, PNG_SIGBYTES);
}

/*
 * Reads png file
 */
GLuint read_png(const char * filename, GLuint * width, GLuint * height)
{
	/* Open File */
	FILE * fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Error opening file %s\n", filename);
		return 1;
	}

	/* Check Signature */
	if (readpng_checksig(fp)) {
		fprintf(stderr, "PNG signature for %s is invalid\n", filename);
		goto cleanup;
	}

	/* Create png structs */
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
			NULL, NULL);
	if (!png)
		goto cleanup;

	png_infop info = png_create_info_struct(png);
	if (!info) {
		png_destroy_read_struct(&png, (png_infopp) NULL,
				(png_infopp) NULL);
		goto cleanup;
	}

	png_infop end_info = png_create_info_struct(png);
	if (!end_info) {
		png_destroy_read_struct(&png, &info, (png_infopp) NULL);
		goto cleanup;
	}

	/* Set libpng error jump point */
	if (setjmp(png_jmpbuf(png))) {
		goto cleanup1;
	}

	/* Actually read the png */
	png_init_io(png, fp);
	png_set_sig_bytes(png, 8);
	png_read_info(png, info);

	GLint bit_depth, colour_type;
	png_uint_32 tmpw, tmph;
	png_get_IHDR(png, info, &tmpw, &tmph, &bit_depth, &colour_type, NULL,
			NULL, NULL);

	if (width) *width = tmpw;
	if (height) *height = tmph;

	char * s;
	if (colour_type == PNG_COLOR_TYPE_RGB)
		s = "RGB";
	else if (colour_type == PNG_COLOR_TYPE_RGBA)
		s = "RGBA";
	else
		s = "Unknown";
	printf("width = %i,\t height = %i\n", tmpw, tmph);
	printf("bit depth = %i,\t colour type = %s\n", bit_depth, s);

	png_read_update_info(png, info);
	size_t rowbytes = png_get_rowbytes(png, info);
	rowbytes += 3 - ((rowbytes - 1) % 4);

	png_byte * img_data = NULL;
	img_data = malloc(rowbytes * tmph * sizeof(png_byte) + 15);
	if (img_data == NULL) {
		fprintf(stderr, "Failed to allocate memory for %s\n", filename);
		goto cleanup1;
	}

	png_bytep * row_ptrs = malloc(tmph * sizeof(png_bytep));
	if (row_ptrs == NULL) {
		fprintf(stderr, "Failed to allocate memory for %s\n", filename);
		goto cleanup2;
	}

	GLuint i;
	for (i = 0; i < tmph; i++)
		row_ptrs[tmph - 1 - i] = img_data + i * rowbytes;

	png_read_image(png, row_ptrs);

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmpw, tmph, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, img_data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);

	free(row_ptrs);
cleanup2:
	free(img_data);
cleanup1:
	png_destroy_read_struct(&png, &info, &end_info);
cleanup:
	fclose(fp);
	return 0;
}
