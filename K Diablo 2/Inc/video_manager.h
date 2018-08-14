#pragma once

#include "singleton.h"

class VideoManager : public Singleton<VideoManager>
{
	friend class Singleton<VideoManager>;
public:
	bool Initialize();

	void RenderVideo(std::wstring const& _file_name);

private:
	VideoManager() = default;
	VideoManager(VideoManager const&) = delete;
	VideoManager& operator=(VideoManager const&) = delete;

	virtual void _Release() override;

	Microsoft::WRL::ComPtr<IGraphBuilder> graph_builder_{};
	Microsoft::WRL::ComPtr<IMediaControl> media_control_{};
	Microsoft::WRL::ComPtr<IMediaEvent> media_event_{};
	Microsoft::WRL::ComPtr<IVideoWindow> video_window_{};
};