#include "xviewer.h"
#include <QtWidgets/QApplication>
#include "xcamera_config.h"
#include <QDebug>
#define TEST_CAM_PATH "test.db"
int main(int argc, char *argv[])
{
#if 0
	auto *xc = XCameraConfig::Instance();
	xc->Load(TEST_CAM_PATH);
	{
		XCameraData cd;
		strcpy(cd.name, "camera1");
		strcpy(cd.save_path, ".\\camera1\\");
		strcpy(cd.url, "rtsp://127.0.0.1:8554/test");
		strcpy(cd.sub_url, "rtsp://127.0.0.1:8554/test");
		xc->Push(cd);
	}
	{
		XCameraData cd;
		strcpy(cd.name, "camera2");
		strcpy(cd.save_path, ".\\camera2\\");
		strcpy(cd.url, "rtsp://127.0.0.1:8555/test");
		strcpy(cd.sub_url, "rtsp://127.0.0.1:8555/test");
		xc->Push(cd);
	}
	int cam_count = xc->GetCamCount();
	for (int i = 0; i < cam_count; i++) {
		auto cam = xc->GetCam(i);
		qDebug() << cam.name;
	}
	qDebug() << "===============set====================";
	auto d1 = xc->GetCam(0);
	strcpy(d1.name, "camera_001");
	xc->SetCam(0, d1);
	cam_count = xc->GetCamCount();
	for (int i = 0; i < cam_count; i++) {
		auto cam = xc->GetCam(i);
		qDebug() << cam.name;
	}
	xc->Save(TEST_CAM_PATH);
	qDebug() << "===============del====================";
	xc->DelCam(1);
	cam_count = xc->GetCamCount();
	for (int i = 0; i < cam_count; i++) {
		auto cam = xc->GetCam(i);
		qDebug() << cam.name;
	}
	xc->DelCam(0);
#endif
	QApplication a(argc, argv);
	XViewer w;
	w.show();
	return a.exec();
}
