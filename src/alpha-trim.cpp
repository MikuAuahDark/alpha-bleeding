#include <iostream>
#include <fstream>
#include <vector>
#include <stdint.h>

#include "png.h"

unsigned char* alpha_trim(unsigned char*, int, int, int&, int&, int&, int&);

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cout << "Usage: alpha-bleeding <input> <output>" << std::endl;
		return 0;
	}

	const char *input = argv[1];
	const char *output = argv[2];

	if (strcmp(input, output) && std::ifstream(output).good())
	{
		std::cout << "Output file already exists!" << std::endl;
		return 0;
	}

	int w, h, c;

	unsigned char *data = png_load(input, &w, &h, &c);

	if (data == 0)
	{
		std::cout << "Error loading image. Must be PNG format." << std::endl;
		return 1;
	}

	if (c != 4)
	{
		std::cout << "The image must be 32 bits (RGB with alpha channel)." << std::endl;
		delete[] data;
		return 0;
	}

	int min_x = 1000000000;
	int min_y = 1000000000;
	int out_w = 1;
	int out_h = 1;
	unsigned char *out_data = alpha_trim(data, w, h, min_x, min_y, out_w, out_h);
	if (min_x == 1000000000) {
		min_x = 0;
		min_y = 0;
	}
	std::cout << min_x << ",";
	std::cout << min_y << ",";
	std::cout << w << ",";
	std::cout << h << std::endl;
	png_save(output, out_w, out_h, out_data);

	delete[] out_data;
	delete[] data;

	return 0;
}

unsigned char* alpha_trim(unsigned char *image, int width, int height, int& min_x, int& min_y, int& out_w, int& out_h)
{
	const size_t N = width * height;
	int max_x = -1;
	int max_y = -1;

	for (int i = 0, j = 3; i < N; i++, j += 4)
	{
		if (image[j] != 0)
		{
			int x = i % width;
			int y = i / width;
			if (x < min_x) {
				min_x = x;
			}
			if (x > max_x) {
				max_x = x;
			}
			if (y < min_y) {
				min_y = y;
			}
			if (y > max_y) {
				max_y = y;
			}
		}
	}
	if (max_x == -1) {
		min_x = 0;
		min_y = 0;
		max_x = 0;
		max_y = 0;
	}

	out_w = max_x - min_x + 1;
	out_h = max_y - min_y + 1;

	unsigned char* ret = new unsigned char[out_w * out_h * 4];
	for (int x = 0; x < out_w; x++) {
		for (int y = 0; y < out_h; y++) {
			int old_idx = (x + min_x + (y + min_y) * width) * 4;
			int new_idx = (x + y * out_w) * 4;
			for (int i=0; i<4; i++) {
				ret[new_idx+i] = image[old_idx+i];
			}
		}
	}
	return ret;
}
