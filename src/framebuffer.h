#pragma once

#include <stddef.h>
#include <stdint.h>
#include <vector>

struct Framebuffer{
	size_t width,height;
	std::vector<uint32_t> data;
};

