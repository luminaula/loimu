#pragma once

#include "framebuffer.h"
#include "particlegrid.h"
#include <QImage>

namespace Renderer {

void init();
void to_qimage(QImage &image);

void update(ParticleGrid *grid);
} // namespace Renderer