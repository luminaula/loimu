
#include "gui.h"
#include "renderer.h"
#include <QPixmap>
#include <QVBoxLayout>
#include <iostream>
#include "settingswidget.h"

Window::Window(QWidget *parent) : QWidget(parent), image() {
    render_obj = new QLabel(this);
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

void Window::closeEvent(QCloseEvent *event){
    Renderer::deinit();
}

