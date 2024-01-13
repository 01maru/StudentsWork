#pragma once
/**
 * @file Framework.h
 * @brief エンジンの軌道に必要な最低限の処理をまとめる用のファイル
 */

namespace MNE
{

	class Framework
	{
	protected:
		//	ゲームループ用フラグ
		bool gameroopFlag_ = true;

		/**
		* @fn Initialize()
		* 初期化用関数
		*/
		virtual void Initialize();

		/**
		* @fn Update()
		* 更新処理をまとめた関数
		*/
		virtual void Update();

		/**
		* @fn Draw()
		* 描画処理をまとめた関数
		*/
		virtual void Draw() = 0;

		/**
		* @fn Finalize()
		* exe終了時に呼び出す関数
		*/
		virtual void Finalize();
	public:		//	関数
		virtual ~Framework() = default;

		/**
		* @fn Run()
		* 実行する処理をまとめた関数
		*/
		void Run();
	};

}
