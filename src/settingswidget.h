#pragma once

#include <QWidget>
#include <QColorDialog>


class SettingsWidget : public QWidget{
	Q_OBJECT
	public:
		SettingsWidget(QWidget *parent = nullptr);
	private slots:
	private:
		QColorDialog *color_picker;
};