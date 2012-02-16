#include "tga.h"

TGAFILE* openTGA(scene *curscene)
{
	FILE *out = fopen("scene.tga", "wb");

	unsigned char header[] = {
		0, 0,
		2,
		0, 0, 0, 0, 0,
		0, 0,
		0, 0,
		(unsigned char)(((int)curscene->resolution) & 0x00FF), (unsigned char)((((int)curscene->resolution) & 0xFF00) / 256),
		(unsigned char)(((int)curscene->resolution) & 0x00FF), (unsigned char)((((int)curscene->resolution) & 0xFF00) / 256),
		24,
		0
	};

	fwrite(header, 1, sizeof(header), out);

	return out;
}

void writeTGAColor(TGAFILE *out, scene *curscene, int x, int y, float red, float green, float blue)
{
	fseek(out, 18 + (((y * (int)curscene->resolution) + x) * 3), SEEK_SET);
	fputc((unsigned char)MIN(255.0, blue * 255.0), out);
	fputc((unsigned char)MIN(255.0, green * 255.0), out);
	fputc((unsigned char)MIN(255.0, red * 255.0), out);
}

void closeTGA(TGAFILE *out)
{
	fclose(out);
}
