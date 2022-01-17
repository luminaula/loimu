#pragma once

#include <QColorDialog>
#include <QWidget>

namespace Lumi {

class SettingsWidget : public QWidget {
    Q_OBJECT
  public:
    SettingsWidget(QWidget *parent = nullptr);
  private slots:
  private:
    QColorDialog *color_picker;
};
} // namespace Lumi