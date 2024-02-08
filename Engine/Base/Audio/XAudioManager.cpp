#include "XAudioManager.h"
#include <fstream>
#include <assert.h>
#include <sstream>

#include "ImGuiController.h"
#include "ImGuiManager.h"
#include "MyMath.h"

using namespace MyMath;

#pragma region Struct

struct ChunkHeader {
	char id[4];
	int32_t size;
};
struct RiffHeader {
	ChunkHeader chunk;
	char type[4];
};
struct FormatChunk {
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};

struct VolumeData
{
	std::string name_;
	float volume_ = 1.0f;

	//	コンストラクタ
	VolumeData() {};
	VolumeData(const std::string& name, float volume) :name_(name), volume_(volume) {};
};

#pragma endregion

void MNE::XAudioManager::UnloadSoundData(SoundData* soundData)
{
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

MNE::XAudioManager* MNE::XAudioManager::GetInstance()
{
	static XAudioManager instance;
	return &instance;
}

void MNE::XAudioManager::Initialize()
{
	HRESULT result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);

	result = xAudio2_->CreateMasteringVoice(&masterVoice_);
}

void MNE::XAudioManager::Finalize()
{
	StopAllSound();
	masterVoice_->DestroyVoice();
	xAudio2_.Reset();

	DeleteAllSound();
}

float MNE::XAudioManager::LoadVolume(const std::string& filename)
{
	float volume = 1.0f;

	std::string filePath = "Resources/Sound/VolumeData.txt";

	//ファイル開く(開けなかったら新規作成)
	std::ifstream file;
	file.open(filePath.c_str(), std::ios_base::app);

	// 1行ずつ読み込む
	std::string line;
	while (getline(file, line)) {

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		if (key == filename) {
			//	dataがあったら
			line_stream >> volume;
		}
	}

	//ファイル閉じる
	file.close();

	return volume;
}

void MNE::XAudioManager::LoadAllValumeData()
{
	for (auto& itr : data_)
	{
		itr.second.volume = LoadVolume(itr.first);
	}
}

void MNE::XAudioManager::SaveVolume()
{
	std::string filePath = "Resources/Sound/VolumeData.txt";

	//ファイル開く(開けなかったら新規作成)
	std::ifstream file;
	file.open(filePath.c_str(), std::ios_base::app);

	std::vector<VolumeData> volumeData;
	std::vector<VolumeData> newVolumeData;

	// 1行ずつ読み込む
	std::string line;
	while (getline(file, line)) {

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		VolumeData data;
		line_stream >> data.name_;

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		line_stream >> data.volume_;
		
		volumeData.emplace_back(data);
	}

	for (auto itr = data_.begin(); itr != data_.end(); itr++)
	{
		bool pushBack = true;
		for (size_t j = 0; j < volumeData.size(); j++)
		{
			//	既にあったら
			if (itr->first == volumeData[j].name_) {
				volumeData[j].volume_ = itr->second.volume;
				pushBack = false;
				break;
			}
		}

		//	存在しなかったら
		if (pushBack) newVolumeData.emplace_back(VolumeData(itr->first, itr->second.volume));
	}

	//ファイル閉じる
	file.close();

	std::ofstream outPutFile;
	outPutFile.open(filePath);

	//	ファイルが開けなかったら
	if (outPutFile.fail()) { assert(0); }

	for (auto& data : volumeData)
	{
		outPutFile << data.name_ << " " << data.volume_ << std::endl;
	}
	for (auto& data : newVolumeData)
	{
		outPutFile << data.name_ << " " << data.volume_ << std::endl;
	}

	outPutFile.close();
}

void MNE::XAudioManager::ImguiUpdate(bool endLoading)
{
	if (!ImGuiController::GetInstance()->GetActiveVolumeManager()) return;

	ChangeVolume(masterVolume_, Master);
	ChangeVolume(bgmVolume_, BGM);
	ChangeVolume(seVolume_, SE);

	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->BeginWindow("VolumeManager", true);

	if (imguiMan->BeginMenuBar()) {
		if (imguiMan->BeginMenu("File")) {
			if (imguiMan->MenuItem("Save")) SaveVolume();
			if (imguiMan->MenuItem("Load")) LoadAllValumeData();
			imguiMan->EndMenu();
		}
		imguiMan->EndMenuBar();
	}

	imguiMan->CheckBox("IsDebug", isDebug_);

	if (isDebug_) {
		imguiMan->BeginChild();

		int32_t id = 0;

		if (endLoading) {
			for (auto itr = data_.begin(); itr != data_.end(); ++itr)
			{
				imguiMan->PushID(id);

				imguiMan->SetSliderFloat(itr->first.c_str(), itr->second.volume, 0.005f, 0.0f, 1.0f);

				if (imguiMan->SetButton("Play")) {
					PlayDebugSoundWave(itr->first, Master, false, true);
				}
				imguiMan->PopID();
				
				id++;
			}
		}
		imguiMan->EndChild();

		if (imguiMan->SetButton("StopAllSound")) StopDebugSound();
	}
	else {
		imguiMan->SetSliderFloat("Master", masterVolume_, 0.005f, 0.0f, 1.0f);
		imguiMan->SetSliderFloat("BGM", bgmVolume_, 0.005f, 0.0f, 1.0f);
		imguiMan->SetSliderFloat("SE", seVolume_, 0.005f, 0.0f, 1.0f);
	}

	imguiMan->EndWindow();
}

void MNE::XAudioManager::LoadSoundWave(const std::string& filename)
{
	//	既に読み込み済みだったら
	if (data_.count(filename) == 1) return;

	std::ifstream file;
	const std::string filePath = "Resources/Sound/" + filename;
	file.open(filePath, std::ios_base::binary);
	assert(file.is_open());

	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	FormatChunk format = {};

	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) { assert(0); }
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	if (strncmp(data.id, "JUNK", 4) == 0) {
		file.seekg(data.size, std::ios_base::cur);
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) { assert(0); }
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	file.close();

	SoundData soundData = {};
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;
	soundData.volume = LoadVolume(filename);

	//	データの挿入
	data_.emplace(filename, soundData);
}

void MNE::XAudioManager::PlayDebugSoundWave(const std::string& soundName, SoundType type, bool loop, bool isDebug)
{
	HRESULT result;

	//	音データがあったら
	assert(data_.count(soundName) != 0);

	SoundVoicePtr pSourceVoice;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice.ptr, &data_[soundName].wfex, 0U, 3.0f);

	pSourceVoice.dataKey = soundName;
	float typeVolume = 1.0f;
	switch (type)
	{
	case XAudioManager::Master:
		break;
	case XAudioManager::BGM:
		typeVolume = bgmVolume_;
		break;
	case XAudioManager::SE:
		typeVolume = seVolume_;
		break;
	}
	pSourceVoice.ptr->SetVolume(data_[soundName].volume * typeVolume);

	//	配列に挿入
	if (isDebug) {
		debugSoundPtr_.push_back(pSourceVoice);
	}
	else {
		switch (type)
		{
		case XAudioManager::Master:
			break;
		case XAudioManager::BGM:
			bgmPtr_.push_back(pSourceVoice);
			break;
		case XAudioManager::SE:
			sePtr_.push_back(pSourceVoice);
			break;
		}
	}

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = data_[soundName].pBuffer;
	buf.AudioBytes = (UINT32)data_[soundName].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	//	ループ再生
	if (loop) buf.LoopCount = XAUDIO2_LOOP_INFINITE;

	result = pSourceVoice.ptr->SubmitSourceBuffer(&buf);
	result = pSourceVoice.ptr->Start();
}

void MNE::XAudioManager::PlaySoundWave(const std::string& soundName, SoundType type, bool loop)
{
	PlayDebugSoundWave(soundName, type, loop);
}

void MNE::XAudioManager::ChangeVolume(float volume, SoundType type)
{
	switch (type)
	{
	case XAudioManager::Master:
		masterVolume_ = volume;
		masterVoice_->SetVolume(masterVolume_);
		break;
	case XAudioManager::BGM:
		bgmVolume_ = volume;
		for (auto& bgm : bgmPtr_)
		{
			bgm.ptr->SetVolume(bgmVolume_ * data_[bgm.dataKey].volume);
		}
		break;
	case XAudioManager::SE:
		seVolume_ = volume;
		for (auto& se : sePtr_)
		{
			se.ptr->SetVolume(seVolume_ * data_[se.dataKey].volume);
		}
		break;
	default:
		break;
	}
}

void MNE::XAudioManager::VolumeUpdate(SoundType type, float volume)
{
	ChangeVolume(volume, type);
}

//void MyXAudio::ChangeAllPitchRatio(float pitch)
//{
//	for (size_t i = 0; i < bgmsoundPtr.size(); i++)
//	{
//		bgmsoundPtr[i].ptr->SetFrequencyRatio(pitch);
//
//		if (pitch <= 1.0f)	bgmsoundPtr[i].ptr->SetVolume(bgmVolume * bgmsoundPtr[i].volume);
//		else				bgmsoundPtr[i].ptr->SetVolume(bgmVolume * bgmsoundPtr[i].volume * 0.5f);
//	}
//	for (size_t i = 0; i < soundEffectPtr.size(); i++)
//	{
//		soundEffectPtr[i].ptr->SetFrequencyRatio(pitch);
//	}
//}

void MNE::XAudioManager::StopAllSound()
{
	StopBGM();
	StopSE();
	StopDebugSound();
}

void MNE::XAudioManager::StopBGM()
{
	for (size_t i = 0; i < bgmPtr_.size(); i++)
	{
		bgmPtr_[i].ptr->Stop();
		bgmPtr_[i].ptr->DestroyVoice();
	}
	bgmPtr_.clear();
}

void MNE::XAudioManager::StopSE()
{
	for (size_t i = 0; i < sePtr_.size(); i++)
	{
		sePtr_[i].ptr->Stop();
		sePtr_[i].ptr->DestroyVoice();
	}
	sePtr_.clear();
}

void MNE::XAudioManager::StopSound(const std::string& soundName)
{
	for (size_t i = 0; i < debugSoundPtr_.size(); i++)
	{
		if (debugSoundPtr_[i].dataKey != soundName) continue;
		debugSoundPtr_[i].ptr->Stop();
	}
}

void MNE::XAudioManager::StopDebugSound()
{
	for (size_t i = 0; i < debugSoundPtr_.size(); i++)
	{
		debugSoundPtr_[i].ptr->Stop();
		debugSoundPtr_[i].ptr->DestroyVoice();
	}
	debugSoundPtr_.clear();
}

void MNE::XAudioManager::DeleteAllSound()
{
	for (auto& data : data_)
	{
		UnloadSoundData(&data.second);
	}
	data_.clear();
}

void MNE::XAudioManager::DeleteSoundData(const std::string& soundName)
{
	//	音データがあったら
	assert(data_.count(soundName) != 0);
	
	UnloadSoundData(&data_[soundName]);
	data_.erase(soundName);
}
