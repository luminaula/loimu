#pragma once

#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QWidget>

class Window : public QWidget {
    Q_OBJECT
  public:
    Window(QWidget *parent);
  private slots:
    void paintEvent(QPaintEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

  private:
    QPixmap pix_map;
    QImage image;
    QLabel *render_obj;
};