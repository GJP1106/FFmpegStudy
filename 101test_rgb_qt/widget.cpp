#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QImage>

static int w = 1280;
static int h = 720;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    resize(w, h);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *ev)
{
    QImage img(w, h, QImage::Format_RGB888);
    auto d = img.bits();
    unsigned char r = 255;
    for (int j = 0; j < h; j++) {
        r--;
        int b = j * w * 3;
        for (int i = 0; i < w * 3; i+=3){
            d[b + i] = r;  // R
            d[b + i + 1] = 0; // G
            d[b + i + 2] = 0; // B
        }
    }

    QPainter p;
    p.begin(this);
    p.drawImage(0, 0, img);

    p.end();
}
