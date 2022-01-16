
#include "gui.h"
#include "renderer.h"
#include <QPixmap>
#include <QVBoxLayout>
#include <iostream>
#include "settingswidget.h"

Window::Window(QWidget *parent) : QWidget(parent), image() {
    render_obj = new QLabel(this);
    render_obj->setFixedSize(800,800);
    render_obj->setPixmap(pix_map);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(render_obj);
    layout->addWidget(new SettingsWidget);
    setLayout(layout);
}

void Window::paintEvent(QPaintEvent *event) {
    Renderer::to_qimage(image);
    render_obj->setPixmap(QPixmap::fromImage(image));
    render_obj->update();
}
