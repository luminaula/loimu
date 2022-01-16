#include "settingswidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QSlider>
#include <QPushButton>
#include "settings.h"
#include "color.h"




SettingsWidget::SettingsWidget(QWidget *parent) : QWidget(parent){
	setFixedHeight(800);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setSpacing(0);
	layout->setMargin(0);

	color_picker = new QColorDialog;
	color_picker->setOptions(QColorDialog::NoButtons | QColorDialog::DontUseNativeDialog);
	connect(color_picker, &QColorDialog::currentColorChanged,this, [=](const QColor &color){
		int r,g,b;
		color.getRgb(&r,&g,&b);
		color_rgb rgb((float)r,(float)g,(float)b);
		Settings::set_color(color_hsl(rgb));
	});
	color_picker->setCurrentColor(Qt::red);
	layout->addWidget(color_picker);

	QLabel *delumination_text = new QLabel;
	QSlider *delumination_slider = new QSlider;
	QHBoxLayout *delumination_layout = new QHBoxLayout;



	delumination_text->setText("Delumination");
	delumination_slider->setOrientation(Qt::Orientation::Horizontal);
	connect(delumination_slider, &QSlider::valueChanged,this,[=](int value){
		float delumination_value = (float)value / 10000.0f;
		Settings::set_delumination(delumination_value);
	});
	delumination_slider->setValue(50);
	delumination_layout->addWidget(delumination_text);
	delumination_layout->addWidget(delumination_slider);
	layout->addLayout(delumination_layout);

	QSlider *desaturation_slider = new QSlider;
	QLabel *desaturation_text = new QLabel;
	QHBoxLayout *desaturation_layout = new QHBoxLayout;
	desaturation_text->setText("Desaturation");
	desaturation_slider->setOrientation(Qt::Orientation::Horizontal);
	connect(desaturation_slider,&QSlider::valueChanged,this,[=](int value){
		float desaturation_value = (float)value / 10000.0f;
		Settings::set_desaturation(desaturation_value);
	});
	desaturation_slider->setValue(50);
	desaturation_layout->addWidget(desaturation_text);
	desaturation_layout->addWidget(desaturation_slider);
	layout->addLayout(desaturation_layout);


	QSlider *hue_jitter_slider = new QSlider;
	QLabel *hue_jitter_text = new QLabel;
	QHBoxLayout *hue_jitter_layout = new QHBoxLayout;
	hue_jitter_text->setText("Hue jitter");
	hue_jitter_slider->setOrientation(Qt::Orientation::Horizontal);
	hue_jitter_slider->setRange(0,10000);
	connect(hue_jitter_slider,&QSlider::valueChanged,this,[=](int value){
		float jitter_value = (float)value / 10000.0f;
		Settings::set_hue_jitter(jitter_value);
	});
	hue_jitter_slider->setValue(500);
	hue_jitter_layout->addWidget(hue_jitter_text);
	hue_jitter_layout->addWidget(hue_jitter_slider);
	layout->addLayout(hue_jitter_layout);

	QSlider *y_slider = new QSlider;
	QLabel *y_text = new QLabel;
	QHBoxLayout *y_layout = new QHBoxLayout;
	y_text->setText("Direction Y");
	y_slider->setOrientation(Qt::Orientation::Horizontal);
	y_slider->setRange(10,100);
	connect(y_slider,&QSlider::valueChanged,this,[=](int value){
		float direction = (float)value / 10.0f;
		Settings::set_direction_y(direction);
	});
	y_slider->setValue(2);
	y_layout->addWidget(y_text);
	y_layout->addWidget(y_slider);
	layout->addLayout(y_layout);

	QSlider *x_slider = new QSlider;
	QLabel *x_text = new QLabel;
	QHBoxLayout *x_layout = new QHBoxLayout;
	x_text->setText("Direction X");
	x_slider->setOrientation(Qt::Orientation::Horizontal);
	connect(x_slider,&QSlider::valueChanged,this,[=](int value){
		float direction = 5.0f - (float)value / 10.0f;
		Settings::set_direction_x(direction);
	});
	x_slider->setValue(50);
	x_layout->addWidget(x_text);
	x_layout->addWidget(x_slider);
	layout->addLayout(x_layout);


	setLayout(layout);

}