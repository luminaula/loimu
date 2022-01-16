#pragma once

#include "particlegrid.h"
#include <QImage>

namespace Renderer {

void init(int width_, int height_);
void to_qimage(QImage &image);

void update(ParticleGrid *grid);
} // namespace Renderer