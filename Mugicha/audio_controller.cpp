#include "audio_controller.h"

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#else //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

AudioController::AudioController()
	: Xaudio2(NULL), mastering_voice(NULL)
{
	if (FAILED(init()))
	{
		throw "error";
	}
}

AudioController::AudioController(Params _params)
	: Xaudio2(NULL), mastering_voice(NULL)
{
	if (SUCCEEDED(init()))
	{
		for (const auto& _param : _params) add_audio(_param.first, Audio(_param.second));
	}
}

AudioController::~AudioController()
{
#ifdef _DEBUG
	puts("Called AudioController Destructor");
#endif
	uninit();
}

HRESULT AudioController::init()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	/**** Create XAudio2 ****/
	auto hr = XAudio2Create(&Xaudio2, 0);
	if (FAILED(hr)) {
		CoUninitialize();
		return -1;
	}

	/**** Create Mastering Voice ****/
	hr = Xaudio2->CreateMasteringVoice(&mastering_voice);
	if (FAILED(hr))
	{
		if (Xaudio2) Xaudio2->Release();
		CoUninitialize();
		return -1;
	}
	
	return hr;
}

void AudioController::uninit()
{
	mastering_voice->DestroyVoice();

	for (const auto& audio : audios)
	{
		if (audio.second.source_voice)
		{
			audio.second.source_voice->Stop(0);
			audio.second.source_voice->FlushSourceBuffers();
			audio.second.source_voice->DestroyVoice(); // オーディオグラフからソースボイスを削除
			delete[] audio.second.data_buffer;
		}
	}



	if (Xaudio2) Xaudio2->Release();

	CoUninitialize();
}

void AudioController::play(std::string label)
{
	// 再生中なら再生しない
	if (audios[label].nowplaying) return;

	// ソースボイス作成
	Xaudio2->CreateSourceVoice(&audios[label].source_voice, &audios[label].wfx.Format);
	
	// ボイスキューに新しいオーディオバッファーを追加
	audios[label].source_voice->SubmitSourceBuffer(&audios[label].buffer);
	
	// 再生
	if (SUCCEEDED(audios[label].source_voice->Start(0))) audios[label].nowplaying = true;
}

bool AudioController::is_playing(std::string label)
{
	return audios[label].nowplaying;
}

void AudioController::stop(std::string label)
{
	if (audios[label].source_voice == NULL) return;
	if (!audios[label].nowplaying) return;

	XAUDIO2_VOICE_STATE xa2state;
	audios[label].source_voice->GetState(&xa2state);
	if (xa2state.BuffersQueued)
		if (SUCCEEDED(audios[label].source_voice->Stop(0)))
			audios[label].nowplaying = false;
}

void AudioController::pause(std::string label)
{
	// 未実装
}

HRESULT AudioController::add_audio(std::string label, Audio _audio)
{
	/**** Initalize Sound ****/

	DWORD chunk_size;
	DWORD chunk_position;
	DWORD filetype;

	memset(&_audio.wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&_audio.buffer, 0, sizeof(XAUDIO2_BUFFER));

	auto file = CreateFile(_audio.param.filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (file == INVALID_HANDLE_VALUE) return HRESULT_FROM_WIN32(GetLastError());
	if (SetFilePointer(file, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) return HRESULT_FROM_WIN32(GetLastError());

	//check the file type, should be fourccWAVE or 'XWMA'
	find_chunk(file, fourccRIFF, chunk_size, chunk_position);
	read_chunk_data(file, &filetype, sizeof(DWORD), chunk_position);
	if (filetype != fourccWAVE) return S_FALSE;

	find_chunk(file, fourccFMT, chunk_size, chunk_position);
	read_chunk_data(file, &_audio.wfx, chunk_size, chunk_position);

	//fill out the audio data buffer with the contents of the fourccDATA chunk
	find_chunk(file, fourccDATA, chunk_size, chunk_position);
	_audio.data_buffer = new BYTE[chunk_size];
	read_chunk_data(file, _audio.data_buffer, chunk_size, chunk_position);

	CloseHandle(file);

	// 	サブミットボイスで利用するサブミットバッファの設定
	_audio.buffer.AudioBytes = chunk_size;
	_audio.buffer.pAudioData = _audio.data_buffer;
	_audio.buffer.Flags = XAUDIO2_END_OF_STREAM;
	_audio.buffer.LoopCount = (_audio.param.loop ? XAUDIO2_LOOP_INFINITE : 0);

	Xaudio2->CreateSourceVoice(&_audio.source_voice, &_audio.wfx.Format);

	audios[label] =_audio;

	return S_OK;
}

void AudioController::reload()
{
	for (auto&& audio : audios)
	{
		//  まずフラグをチェック
		if (audio.second.nowplaying)
		{
			XAUDIO2_VOICE_STATE state;

			audio.second.source_voice->GetState(&state);

			if(state.BuffersQueued == 0) audio.second.nowplaying = false;
		}
	}
}

#ifdef _DEBUG
void AudioController::dump()
{
	for (const auto& audio : audios)
	{
		printf("LABEL: %s, FILE: %s\n", audio.first.c_str(), audio.second.param.filename);
	}
}
#endif

HRESULT AudioController::find_chunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;
	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;
		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}
		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}
		dwOffset += dwChunkDataSize;
		if (bytesRead >= dwRIFFDataSize) return S_FALSE;
	}
	return S_OK;
}

HRESULT AudioController::read_chunk_data(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}
