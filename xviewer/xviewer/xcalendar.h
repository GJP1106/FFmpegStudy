#pragma once
#include <QCalendarWidget.h>
class XCalendar : public QCalendarWidget
{
public:
	XCalendar(QWidget* p);
	void paintCell(QPainter *painter,
		const QRect &rec,
		const QDate &date) const override;
};

