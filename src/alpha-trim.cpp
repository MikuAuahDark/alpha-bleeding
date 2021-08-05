#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <limits>

#include "png.h"

unsigned char* alpha_trim(unsigned char*, int, int, int&, int&, int&, int&);

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cout << "Usage: alpha-trim <input> <output>" << std::endl;
		return 1;
	}

	const char *input = argv[1];
	const char *output = argv[2];

	if (strcmp(input, output) && std::ifstream(output).good())
	{
		std::cout << "Output file already exists!" << std::endl;
		return 1;
	}

	int w, h, c;

	unsigned char *data = png_load(input, &w, &h, &c);

	if (!data)
	{
		std::cout << "Error loading image. Must be PNG format." << std::endl;
		return 1;
	}

	if (c != 4)
	{
		std::cout << "The image must be 32 bits (RGB with alpha channel)." << std::endl;
		delete[] data;
		return 1;
	}

	constexpr int int_max = std::numeric_limits<int>::max();
	int min_x = int_max;
	int min_y = int_max;
	int out_w = 1;
	int out_h = 1;
	unsigned char *out_data = alpha_trim(data, w, h, min_x, min_y, out_w, out_h);
	if (min_x == int_max) {
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
	int max_x = std::numeric_limits<int>::min();
	int max_y = max_x;
	bool has_opaque = false;

	for (int i = 0, j = 3; i < N; i++, j += 4)
	{
		if (image[j] != 0)
		{
			int x = i % width;
			int y = i / width;
			min_x = std::min(x, min_x);
			max_x = std::max(x, max_x);
			min_y = std::min(y, min_y);
			max_y = std::max(y, max_y);
			has_opaque = true;
		}
	}

	if (!has_opaque)
	{
		min_x = 0;
		min_y = 0;
		max_x = 0;
		max_y = 0;
	}
	else
	{
		// Pad by 1px
		min_x = std::max(min_x - 1, 0);
		max_x = std::min(max_x + 1, width - 1);
		min_y = std::max(min_y - 1, 0);
		max_y = std::min(max_y + 1, height - 1);
		out_w = max_x - min_x + 1;
		out_h = max_y - min_y + 1;
	}

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
