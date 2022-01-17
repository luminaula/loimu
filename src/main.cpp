/**
 * Copyright (c) 2021 "Lumi"
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "gui.h"
#include "renderer.h"
#include <QApplication>

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    Lumi::Renderer::init(800, 800);
    Lumi::Window win(nullptr);
    win.show();
    QCoreApplication::setApplicationName("supkt");
    QCoreApplication::setOrganizationName("Lumi");

    return app.exec();
}