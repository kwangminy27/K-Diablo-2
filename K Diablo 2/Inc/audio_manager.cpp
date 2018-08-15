#include "stdafx.h"
#include "audio_manager.h"

#include "path_manager.h"

using namespace std;
using namespace filesystem;
using namespace DirectX;

bool AudioManager::Initialize()
{
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	AUDIO_ENGINE_FLAGS audio_engine_flags = AudioEngine_Default;
#ifdef _DEBUG
	audio_engine_flags = audio_engine_flags | AudioEngine_Debug;
#endif
	audio_engine_ = std::make_unique<AudioEngine>(audio_engine_flags);

	if (!_LoadSoundDataSheet(L"sound.csv"))
		return false;

	return true;
}

void AudioManager::Update()
{
	audio_engine_->Update();
}

void AudioManager::Suspend()
{
	audio_engine_->Suspend();
}

void AudioManager::Resume()
{
	audio_engine_->Resume();
}

void AudioManager::AddSoundEffectInstance(string const& _tag, unique_ptr<SoundEffectInstance> _sound_effect_instance)
{
	sound_effect_instance_map_.insert(make_pair(_tag, move(_sound_effect_instance)));
}

shared_ptr<SoundEffect> const& AudioManager::FindSoundEffect(string const& _tag)
{
	auto iter = sound_effect_map_.find(_tag);

	if (iter == sound_effect_map_.end())
		return sound_effect_nullptr_;

	return iter->second;
}

void AudioManager::RemoveSoundEffectInstance(string const& _tag)
{
	sound_effect_instance_map_.erase(_tag);
}

void AudioManager::_Release()
{
}

bool AudioManager::_LoadSoundDataSheet(wstring const& _file_name, string const& _path_tag)
{
	path path_buffer = PathManager::GetSingleton()->FindPath(_path_tag);

	if (path_buffer.empty())
		return false;

	wstring full_path = path_buffer.wstring() + _file_name;

	ifstream file{ full_path };

	if (file.fail())
		return false;

	while (true)
	{
		string line{};
		stringstream line_stream{};

		getline(file, line);

		if (line.empty())
			break;

		line_stream.str(line);

		string tag{};
		string sound_file_name{};

		getline(line_stream, tag, ',');
		getline(line_stream, sound_file_name, ',');

		wstring wstring_sound_file_name{ sound_file_name.begin(), sound_file_name.end() };

		if (!_LoadSoundEffect(tag, wstring_sound_file_name))
			return false;
	}

	return true;
}

bool AudioManager::_LoadSoundEffect(string const& _tag, wstring const& _file_name, string const& _path_tag)
{
	if (FindSoundEffect(_tag))
		return false;

	path path_buffer = PathManager::GetSingleton()->FindPath(_path_tag);

	if (path_buffer.empty())
		return false;

	wstring full_path = path_buffer.wstring() + _file_name;

	auto sound_effect = std::make_unique<SoundEffect>(audio_engine_.get(), full_path.c_str());

	sound_effect_map_.insert(make_pair(_tag, move(sound_effect)));

	return true;
}