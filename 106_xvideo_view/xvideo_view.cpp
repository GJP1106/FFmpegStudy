//#include "xvideo_view.h"
#include "xsdl.h"

XVideoView * XVideoView::Create(RenderType type)
{
	switch (type)
	{
	case XVideoView::SDL:
		return new XSDL();
	default:
		break;
	}
	return nullptr;
}
