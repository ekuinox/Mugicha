#pragma once

#pragma comment(lib, "xaudio2.lib")
#include <xaudio2.h>
#include <map>
#include <vector>

/*
* AudioController
* XAudio2ÇàµÇ¶ÇÈÇÊÇ§Ç…
*/

class AudioController
{
public:
	using PARAM = struct _PARAM {
		LPCSTR filename; // file
		bool loop; // ÉãÅ[ÉvÇ≥ÇπÇÈÇ©
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

	using Params = std::map<const char*, PARAM>;

	AudioController(Params _params);
	~AudioController();
	HRESULT init();
	void uninit();
	void play(const char* label);
	void stop(const char* label);
	void pause(const char* label);
private:
	std::map<const char*, Audio> audios;
	std::vector<const char*> nowplayings;

	IXAudio2 *Xaudio2;
	IXAudio2MasteringVoice *mastering_voice;

	HRESULT add_audio(const char* label, Audio _audio);
	HRESULT find_chunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
	HRESULT read_chunk_data(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset);
};

