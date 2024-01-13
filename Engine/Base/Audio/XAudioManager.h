#pragma once
#include <xaudio2.h>
#include <wrl.h>
#include <vector>
#include <string>
#include <map>

/**
* @file XAudioManager.h
* @brief 音再生に関連する機能をまとめたファイル
*/

namespace MNE
{
#pragma region Struct

	/**
	* @struct     SoundData
	* @brief      音データ
	**/
	struct SoundData {
		WAVEFORMATEX wfex{};
		BYTE* pBuffer = nullptr;
		size_t bufferSize = 0;

		float volume = 1.0f;
	};

	/**
	* @struct     SoundVoicePtr
	* @brief      再生中の音データ
	**/
	struct SoundVoicePtr {
		std::string dataKey;
		IXAudio2SourceVoice* ptr = nullptr;
	};

#pragma endregion

	class XAudioManager
	{
	private:
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		XAudioManager() {};
		~XAudioManager() {};

	public:
		static XAudioManager* GetInstance();
		//	コピーコンストラクタ無効
		XAudioManager(const XAudioManager& obj) = delete;
		//	代入演算子無効
		XAudioManager& operator=(const XAudioManager& obj) = delete;

	public:
		/**
		* @enum SoundType
		* 音量調節用の列挙体。再生時に指定することで格納するvectorを変える
		*/
		enum SoundType {
			Master,
			BGM,
			SE,
			None,
		};
	private:
		//	Imgui用
		bool isDebug_ = false;

		ComPtr<IXAudio2> xAudio2_;
		IXAudio2MasteringVoice* masterVoice_ = nullptr;

		//	soundのデータ配列
		std::map<std::string, SoundData, std::less<>> data_;

		std::vector<SoundVoicePtr> sePtr_;
		std::vector<SoundVoicePtr> bgmPtr_;
		std::vector<SoundVoicePtr> debugSoundPtr_;

		//	volume
		float masterVolume_ = 1.0f;
		float bgmVolume_ = 1.0f;
		float seVolume_ = 1.0f;

		float pitchRatio_ = 1.0f;

	private:	//	関数
		void UnloadSoundData(SoundData* soundData);
		void ChangeVolume(float volume, SoundType type);

		//	Stop
		void StopDebugSound();
		void StopSound(const std::string& soundName);

		//	Save&Load
		float LoadVolume(const std::string& filename);
		void LoadAllValumeData();
		void SaveVolume();
		
		void PlayDebugSoundWave(const std::string& soundName, SoundType type, bool loop = false, bool isDebug = false);
	public:

		/**
		* @fn Initialize()
		* 初期化用関数
		*/
		void Initialize();

		/**
		* @fn Finalize()
		* exe終了時に呼び出す関数
		*/
		void Finalize();

		/**
		* @fn ImguiUpdate(bool)
		* Debug用のImGuiを使う際に使用する関数
		* @param endLoading 非同期処理による書き換えが終わるまでデータを使用しないように回避するための引数
		*/
		void ImguiUpdate(bool endLoading);

		/**
		* @fn LoadSoundWave(const std::string&)
		* 音を読み込む際に使用する関数
		* @param filename 音データのファイル名(例 : bgm.wav)
		*/
		void LoadSoundWave(const std::string& filename);

		/**
		* @fn PlaySoundWave(const std::string&, SoundType, bool)
		* 音を再生する際に使用する関数
		* @param soundName 音データのファイル名(例 : bgm.wav)
		* @param type 再生する音の種類設定
		* @param loop ループ再生するかの設定(初期値はfalse)
		*/
		void PlaySoundWave(const std::string& soundName, SoundType type, bool loop = false);

		/**
		* @fn VolumeUpdate(SoundType, int32_t)
		* Optionなどでvolume変更する際に使用する関数
		* @param type 音量調節するTypeの設定
		* @param inputValue 音量の増加量0～100の値
		*/
		void VolumeUpdate(SoundType type, float volume);

		//void ChangeAllPitchRatio(float pitch);

#pragma region Stop

	/**
	* @fn StopAllSound()
	* 再生中の音をすべて停止させる関数
	*/
		void StopAllSound();

		/**
		* @fn StopAllSound()
		* BGMとして再生中の音を停止させる関数
		*/
		void StopBGM();

		/**
		* @fn StopAllSound()
		* SEとして再生中の音を停止させる関数
		*/
		void StopSE();

#pragma endregion

#pragma region Delete

		/**
		* @fn DeleteAllSound()
		* 読み込んだ音データをすべて削除する関数
		*/
		void DeleteAllSound();

		/**
		* @fn DeleteSoundData(const std::string&)
		* 読み込んだ音データを削除する関数
		* @param soundName 音データのファイル名(例 : bgm.wav)
		*/
		void DeleteSoundData(const std::string& soundName);

#pragma endregion

#pragma region Getter

		/**
		* @fn GetBGMVolume()
		* BGMの音量の割合を返す関数
		* @return 0.0f～1.0fに正規化されたBGMの音量を返す
		*/
		float GetBGMVolume() { return bgmVolume_; }

		/**
		* @fn GetSEVolume()
		* SEの音量の割合を返す関数
		* @return 0.0f～1.0fに正規化されたSEの音量を返す
		*/
		float GetSEVolume() { return seVolume_; }

		/**
		* @fn GetSEVolume()
		* SEの音量の割合を返す関数
		* @return 0.0f～1.0fに正規化されたSEの音量を返す
		*/
		float GetMasterVolume() { return masterVolume_; }

#pragma endregion
	};

}

