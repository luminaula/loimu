#pragma once

#include "color.h"
#include "particlegrid.h"
#include <QImage>

namespace Renderer {

void init(int width_, int height_);
void deinit();
void to_qimage(QImage &image);

} // namespace Renderer