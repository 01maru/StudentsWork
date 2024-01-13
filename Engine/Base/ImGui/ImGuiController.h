#pragma once

/**
* @file ImGuiController.h
* @brief いろいろなクラスのImGuiをまとめて管理するファイル
*/

namespace MNE
{

	class ImGuiController
	{
	private:
		bool activeVolumeManager_ = false;
		bool activeInputManager_ = false;
		bool activeTextureManager_ = false;
		bool activeUIManager_ = false;
		bool activeCameraManager_ = false;
		bool activeLightManager_ = false;
		bool activeParticleManager_ = false;
		bool activeModelManager_ = false;

	private:	//	関数
		ImGuiController() {};
		~ImGuiController() {};

	public:
		static ImGuiController* GetInstance();
		ImGuiController(const ImGuiController& obj) = delete;
		ImGuiController& operator=(const ImGuiController& obj) = delete;

		/**
		* @fn Update()
		* 更新処理関数
		*/
		void Update();

	#pragma region Getter

		/**
		* @fn GetActiveVolumeManager()
		* activeVolumeManager_のGetter関数
		* @return activeVolumeManager_の値
		*/
		bool GetActiveVolumeManager() { return activeVolumeManager_; }
		/**
		* @fn GetActiveInputManager()
		* activeInputManager_のGetter関数
		* @return activeInputManager_の値
		*/
		bool GetActiveInputManager() { return activeInputManager_; }
		/**
		* @fn GetActiveTextureManager()
		* activeTextureManager_のGetter関数
		* @return activeTextureManager_の値
		*/
		bool GetActiveTextureManager() { return activeTextureManager_; }
		/**
		* @fn GetActiveUIManager()
		* activeUIManager_のGetter関数
		* @return activeUIManager_の値
		*/
		bool GetActiveUIManager() { return activeUIManager_; }
		/**
		* @fn GetActiveCameraManager()
		* activeCameraManager_のGetter関数
		* @return activeCameraManager_の値
		*/
		bool GetActiveCameraManager() { return activeCameraManager_; }
		/**
		* @fn GetActiveLightManager()
		* activeLightManager_のGetter関数
		* @return activeLightManager_の値
		*/
		bool GetActiveLightManager() { return activeLightManager_; }
		/**
		* @fn GetActiveParticleManager()
		* activeParticleManager_のGetter関数
		* @return activeParticleManager_の値
		*/
		bool GetActiveParticleManager() { return activeParticleManager_; }
		/**
		* @fn GetActiveModelManager()
		* activeModelManager_のGetter関数
		* @return activeModelManager_の値
		*/
		bool GetActiveModelManager() { return activeModelManager_; }

	#pragma endregion
	};

}

