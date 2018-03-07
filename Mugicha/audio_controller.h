#pragma once

#pragma comment(lib, "xaudio2.lib")
#include <xaudio2.h>
#include <map>
#include <vector>
#include <string>

/*
* AudioController
* XAudio2を扱えるように
*/

class AudioController
{
public:
	using PARAM = struct _PARAM {
		LPCSTR filename; // file
		bool loop; // ループさせるか
		_PARAM() {}
		_PARAM(LPCSTR _filename, bool _loop) : filename(_filename), loop(_loop) {}
	};
	using Audio = struct _Audio {
		PARAM param;
		IXAudio2SourceVoice* source_voice;
		WAVEFORMATEXTENSIBLE wfx; // WAV Format
		XAUDIO2_BUFFER buffer;
		BYTE* data_buffer;
		bool nowplaying;
		_Audio() {}
		_Audio(PARAM _param) : param(_param), nowplaying(false) {}
	};

	using Params = std::map<std::string, PARAM>;

	AudioController();
	AudioController(Params _params);
	~AudioController();
	HRESULT init();
	void uninit();
	void play(std::string label);
	bool is_playing(std::string label);
	void stop(std::string label);
	void pause(std::string label);
	HRESULT add_audio(std::string label, Audio _audio);
	void reload(); // 音楽が実際に再生されているかを確認し，nowplayingをfalseにする
#ifdef _DEBUG
	void dump();
#endif

private:
	std::map<std::string, Audio> audios;

	IXAudio2 *Xaudio2;
	IXAudio2MasteringVoice *mastering_voice;

	HRESULT find_chunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
	HRESULT read_chunk_data(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset);
};

