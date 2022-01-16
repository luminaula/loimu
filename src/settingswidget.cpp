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
	QColor start_color;
	start_color.setRed(255);
	start_color.setGreen(90);
	start_color.setBlue(10);
	color_picker->setCurrentColor(start_color);
	layout->addWidget(color_picker);

	QLabel *delumination_text = new QLabel;
	QSlider *delumination_slider = new QSlider;
	QHBoxLayout *delumination_layout = new QHBoxLayout;



	delumination_text->setText("Fade");
	delumination_slider->setOrientation(Qt::Orientation::Horizontal);
	delumination_slider->setRange(0,400);
	connect(delumination_slider, &QSlider::valueChanged,this,[=](int value){
		float delumination_value = (float)value / 10000.0f;
		Settings::set_delumination(delumination_value);
	});
	delumination_slider->setValue(100);
	delumination_layout->addWidget(delumination_text);
	delumination_layout->addWidget(delumination_slider);
	layout->addLayout(delumination_layout);

	QSlider *desaturation_slider = new QSlider;
	QLabel *desaturation_text = new QLabel;
	QHBoxLayout *desaturation_layout = new QHBoxLayout;
	desaturation_text->setText("Smoke");
	desaturation_slider->setOrientation(Qt::Orientation::Horizontal);
	desaturation_slider->setRange(0,200);
	connect(desaturation_slider,&QSlider::valueChanged,this,[=](int value){
		float desaturation_value = (float)value / 10000.0f;
		Settings::set_desaturation(desaturation_value);
	});
	desaturation_slider->setValue(100);
	desaturation_layout->addWidget(desaturation_text);
	desaturation_layout->addWidget(desaturation_slider);
	layout->addLayout(desaturation_layout);


	QSlider *hue_jitter_slider = new QSlider;
	QLabel *hue_jitter_text = new QLabel;
	QHBoxLayout *hue_jitter_layout = new QHBoxLayout;
	hue_jitter_text->setText("Color variance");
	hue_jitter_slider->setOrientation(Qt::Orientation::Horizontal);
	hue_jitter_slider->setRange(0,3000);
	connect(hue_jitter_slider,&QSlider::valueChanged,this,[=](int value){
		float jitter_value = (float)value / 10000.0f;
		Settings::set_hue_jitter(jitter_value);
	});
	hue_jitter_slider->setValue(1000);
	hue_jitter_layout->addWidget(hue_jitter_text);
	hue_jitter_layout->addWidget(hue_jitter_slider);
	layout->addLayout(hue_jitter_layout);

	QSlider *scatter_slider = new QSlider;
	QLabel *scatter_text = new QLabel;
	QHBoxLayout *scatter_layout = new QHBoxLayout;
	scatter_text->setText("Scatter");
	scatter_slider->setOrientation(Qt::Orientation::Horizontal);
	scatter_slider->setRange(0,2000);
	connect(scatter_slider,&QSlider::valueChanged,this,[=](int value){
		float scatter_value = (float)value / 100.0f;
		Settings::set_scatter(scatter_value);
	});
	scatter_slider->setValue(500);
	scatter_layout->addWidget(scatter_text);
	scatter_layout->addWidget(scatter_slider);
	layout->addLayout(scatter_layout);

	QSlider *y_slider = new QSlider;
	QLabel *y_text = new QLabel;
	QHBoxLayout *y_layout = new QHBoxLayout;
	y_text->setText("Force Y");
	y_slider->setOrientation(Qt::Orientation::Horizontal);
	y_slider->setRange(25,100);
	connect(y_slider,&QSlider::valueChanged,this,[=](int value){
		float direction = (float)value / 10.0f;
		Settings::set_force_y(direction);
	});
	y_slider->setValue(30);
	y_layout->addWidget(y_text);
	y_layout->addWidget(y_slider);
	layout->addLayout(y_layout);

	QSlider *x_slider = new QSlider;
	QLabel *x_text = new QLabel;
	QHBoxLayout *x_layout = new QHBoxLayout;
	x_text->setText("Force X");
	x_slider->setOrientation(Qt::Orientation::Horizontal);
	x_slider->setRange(-100,100);
	connect(x_slider,&QSlider::valueChanged,this,[=](int value){
		float direction = (float)value / 10.0f;
		Settings::set_force_x(direction);
	});
	x_slider->setValue(0);
	x_layout->addWidget(x_text);
	x_layout->addWidget(x_slider);
	layout->addLayout(x_layout);


	setLayout(layout);

}