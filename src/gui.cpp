
#include "gui.h"
#include "renderer.h"
#include <QPixmap>
#include <QVBoxLayout>
#include <iostream>

Window::Window(QWidget *parent) : QWidget(parent), image() {
    setFixedSize(800, 800);
    render_obj = new QLabel(this);
    render_obj->setPixmap(pix_map);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(render_obj);
    setLayout(layout);
}

void Window::paintEvent(QPaintEvent *event) {
    Renderer::to_qimage(image);
    render_obj->setPixmap(QPixmap::fromImage(image));
    render_obj->update();
}
