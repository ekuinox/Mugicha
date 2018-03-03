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

AudioController::AudioController(Params _params)
{
	for (const auto& _param : _params)
	{
		audios[_param.first] = Audio(_param.second);
	}
	init();
}

AudioController::~AudioController()
{
	uninit();
}

HRESULT AudioController::init()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	/**** Create XAudio2 ****/
	auto hr = XAudio2Create(&Xaudio2, 0/* , XAUDIO2_DEFAULT_PROCESSOR*/);
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

	/**** Initalize Sound ****/

	DWORD chunk_size;
	DWORD chunk_position;
	DWORD filetype;

	for (auto&& audio : audios)
	{
		auto file = CreateFile(audio.second.param.filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (file == INVALID_HANDLE_VALUE) return HRESULT_FROM_WIN32(GetLastError());
		if (SetFilePointer(file, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) return HRESULT_FROM_WIN32(GetLastError());

		//check the file type, should be fourccWAVE or 'XWMA'
		find_chunk(file, fourccRIFF, chunk_size, chunk_position);
		read_chunk_data(file, &filetype, sizeof(DWORD), chunk_position);
		if (filetype != fourccWAVE) return S_FALSE;

		find_chunk(file, fourccFMT, chunk_size, chunk_position);
		read_chunk_data(file, &audio.second.wfx, chunk_size, chunk_position);

		//fill out the audio data buffer with the contents of the fourccDATA chunk
		find_chunk(file, fourccDATA, chunk_size, chunk_position);
		audio.second.data_buffer = new BYTE[chunk_size];
		read_chunk_data(file, audio.second.data_buffer, chunk_size, chunk_position);

		CloseHandle(file);

		// 	サブミットボイスで利用するサブミットバッファの設定
		audio.second.buffer.AudioBytes = chunk_size;
		audio.second.buffer.pAudioData = audio.second.data_buffer;
		audio.second.buffer.Flags = XAUDIO2_END_OF_STREAM;
		audio.second.buffer.LoopCount = (audio.second.param.loop ? XAUDIO2_LOOP_INFINITE : 0);

		Xaudio2->CreateSourceVoice(&audio.second.source_voice, &(audio.second.wfx.Format));
	}
	
	return hr;
}

void AudioController::uninit()
{
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

	mastering_voice->DestroyVoice();

	if (Xaudio2) Xaudio2->Release();

	CoUninitialize();
}

void AudioController::play(const char *label)
{
	// ソースボイス作成
	Xaudio2->CreateSourceVoice(&(audios[label].source_voice), &(audios[label].wfx.Format));
	
	// ボイスキューに新しいオーディオバッファーを追加
	audios[label].source_voice->SubmitSourceBuffer(&(audios[label].buffer));
	
	// 再生
	audios[label].source_voice->Start(0);
}

void AudioController::stop(const char * label)
{
	if (audios[label].source_voice == NULL) return;

	XAUDIO2_VOICE_STATE xa2state;
	audios[label].source_voice->GetState(&xa2state);
	if (xa2state.BuffersQueued) audios[label].source_voice->Stop(0);
}

void AudioController::pause(const char *label)
{
	// 未実装
}

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
