#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <assert.h>

#define WIDTH 800
#define HEIGHT 800
#define SCOPE 2.25
#define ITERS 10

typedef struct
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} PIXEL;

typedef struct
{
	double x, i;
} COMPLEX;

typedef struct
{
	int w, h;
	int midh;
	int midw;
	double delta_x, delta_i;
	PIXEL *pixels;
} IMAGE;

IMAGE image;

char in_set(COMPLEX *num, int i_count);

int main(int argc, char **argv)
{
	if (argc < 3)
		exit(-1);

	image.w = atoi(argv[1]);
	image.h = atoi(argv[2]);

	if (image.h % 2 == 0)
		--image.h;
	if (image.w % 2 == 0)
		--image.w;

	image.midh = image.h / 2;
	image.midw = image.w / 2;

/*	image.delta_x = (double)image.w / SCOPE;
	image.delta_i = (double)image.h / SCOPE;*/

	image.delta_x = SCOPE / (double)image.w;
	image.delta_i = SCOPE / (double)image.h;
	
	FILE *fp = fopen("mandelbrot.ppm", "wb");
	(void) fprintf(fp, "P6\n%d %d\n255\n", image.w, image.h);

	image.pixels = (PIXEL*)malloc(sizeof(PIXEL) * image.w * image.h);

	COMPLEX com;
	
	double r, i;
	for (int x = 0; x < image.w; ++x)
	{
		if (x == 0)
			 r = -image.delta_x * image.midw;
		for (int y = 0; y < image.h; ++y)
		{
			if (y == 0)
				i = -image.delta_i * image.midh;
			com.x = r;
			com.i = i;
			int val = in_set(&com,  ITERS);
			if (val == -1)
			{
				image.pixels[image.w * y + x].r = 255;
				image.pixels[image.w * y + x].g = 255;
				image.pixels[image.w * y + x].b = 255;
			}
			else
			{
				int brightness = (double)((double)(ITERS - val) / ITERS) * 255;
				image.pixels[image.w * y + x].r = brightness;
				image.pixels[image.w * y + x].g = 0;
				image.pixels[image.w * y + x].b = 0;
			}
		        i += (double)image.delta_i;
		}
	        r += (double)image.delta_x;
	}
	
	for (int y = 0; y < image.h; y++)
	{
		for (int x = 0; x < image.w; x++)
		{
			(void) fwrite(&image.pixels[y * image.w + x], 1, 3, fp);
		}
	}

	free(image.pixels);

	fclose(fp);

	system("xdg-open mandelbrot.ppm");
	
	return EXIT_SUCCESS;
}

void square(COMPLEX *num)
{
	double real = num->x * num->x - num->i * num->i;
	double imaginary = 2 * num->x * num->i;
	num->x = real;
	num->i = imaginary;
}


char in_set(COMPLEX *num, int i_count)
{
	while (i_count > 0)
	{
		if (num->x > 1 || num->i > 1 || num->x < -1 || num->i < -1)
			return i_count;
		--i_count;
		square(num);
	}
	return -1;
}
