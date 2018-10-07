#include "image_mask.h"
#include "utils.h"

#include <QPainter>

ImageMask::ImageMask() {}
ImageMask::ImageMask(const QString &file, Id2Labels id_labels) {
	id = mat2QImage(cv::imread(file.toStdString()));
	map0to255id(id);
	color = idToColor(id, id_labels);
}
ImageMask::ImageMask(QSize s) {
	id = QImage(s, QImage::Format_RGB888);
	color = QImage(s, QImage::Format_RGB888);
	id.fill(QColor(255, 255, 255));
	color.fill(QColor(255, 255, 255));
}

void ImageMask::exchangeLabel(int x, int y, const Id2Labels& id_labels, ColorMask cm) {
	
	cv::Mat id_mat = qImage2Mat(id);
	cv::floodFill(id_mat, cv::Point(x, y), cv::Scalar(cm.id.red(), cm.id.green(), cm.id.blue()), 0, cv::Scalar(0, 0, 0), cv::Scalar(0,0,0));
	
	id = mat2QImage(id_mat);
	color = idToColor(id, id_labels);

	//cv::Mat color_debug = qImage2Mat(color);
	//cv::imshow("debug", color_debug);

}

void ImageMask::drawFillCircle(int x, int y, int pen_size, ColorMask cm) {
	QPen pen(QBrush(cm.id), 1.0);
	QPainter painter_id(&id);
	painter_id.setRenderHint(QPainter::Antialiasing, false);
	painter_id.setPen(pen);
	painter_id.setBrush(QBrush(cm.id));
	painter_id.drawEllipse(x, y, pen_size, pen_size);
	painter_id.end();

	QPainter painter_color(&color);
	QPen pen_color(QBrush(cm.color), 1.0);
	painter_color.setRenderHint(QPainter::Antialiasing, false);
	painter_color.setPen(pen_color);
	painter_color.setBrush(QBrush(cm.color));
	painter_color.drawEllipse(x, y, pen_size, pen_size);
	painter_color.end();
}

void ImageMask::drawFillLine(int x1, int y1, int x2, int y2, int pen_size, ColorMask cm) {
	QPen pen(QBrush(cm.id), pen_size, Qt::PenStyle::SolidLine);
	QPainter painter_id(&id);
	painter_id.setRenderHint(QPainter::Antialiasing, false);
	painter_id.setPen(pen);
	painter_id.setBrush(QBrush(cm.id));
	painter_id.drawLine(x1, y1, x2, y2);
	painter_id.end();

	QPainter painter_color(&color);
	QPen pen_color(QBrush(cm.color), pen_size, Qt::PenStyle::SolidLine);
	painter_color.setRenderHint(QPainter::Antialiasing, false);
	painter_color.setPen(pen_color);
	painter_color.setBrush(QBrush(cm.color));
	painter_color.drawLine(x1, y1, x2, y2);
	painter_color.end();
}

void ImageMask::drawPixel(int x, int y, ColorMask cm) {
	id.setPixelColor(x, y, cm.id);
	color.setPixelColor(x, y, cm.color);
}

void ImageMask::updateColor(const Id2Labels & labels) {
	idToColor(id, labels, &color);
}