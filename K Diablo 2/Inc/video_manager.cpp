#include "stdafx.h"
#include "video_manager.h"

#include "path_manager.h"

using namespace std;
using namespace filesystem;

bool VideoManager::Initialize()
{
	//RenderVideo(L"BlizNorth640x480.avi");

	return true;
}

void VideoManager::RenderVideo(wstring const& _file_name)
{
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)graph_builder_.ReleaseAndGetAddressOf());

	graph_builder_->QueryInterface(IID_IMediaControl, (void **)media_control_.ReleaseAndGetAddressOf());
	graph_builder_->QueryInterface(IID_IMediaEvent, (void **)media_event_.ReleaseAndGetAddressOf());
	graph_builder_->QueryInterface(IID_IVideoWindow, (void **)video_window_.ReleaseAndGetAddressOf());

	path path_buffer = PathManager::GetSingleton()->FindPath("VideoPath");
	wstring full_path = path_buffer.wstring() + _file_name;
	graph_builder_->RenderFile(full_path.c_str(), NULL);

	video_window_->put_FullScreenMode(OATRUE);
	media_control_->Run();

	long ev_code{};
	media_event_->WaitForCompletion(INFINITE, &ev_code);

	video_window_.Reset();
	media_event_.Reset();
	media_control_.Reset();
	graph_builder_.Reset();
}

void VideoManager::_Release()
{
}