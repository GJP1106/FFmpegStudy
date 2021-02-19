#pragma once

#include <QtWidgets/QWidget>
#include "ui_xviewer.h"

class XViewer : public QWidget
{
	Q_OBJECT

public:
	XViewer(QWidget *parent = Q_NULLPTR);

private:
	Ui::XViewerClass ui;
};
