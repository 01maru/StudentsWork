#include "PlayerData.h"
#include "ImGuiManager.h"
#include <fstream>
#include <sstream>
#include <cassert>

using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] ImGuiUpdate
//-----------------------------------------------------------------------------

void PlayerData::ImGuiHPUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->InputInt("MaxHP", maxHP_);
}

void PlayerData::ImGuiMoveUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->InputFloat("walkSpd", walkSpd_);
	imgui->InputFloat("runSpd", runSpd_);
	imgui->InputFloat("jumpingSpdDec", jumpingDecSpd_);
}

void PlayerData::ImGuiAvoidUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->InputFloat("AvoidMaxSpd", avoidMaxSpd_);
	imgui->InputInt("AvoidAccTime", avoidAccTime_);
	imgui->InputInt("AvoidDecTime", avoidDecTime_);
	imgui->InputInt("AvoidCoolTime", avoidCoolTime_);
}

void PlayerData::ImGuiAttackUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->InputInt("SlowATCoolTime", slowATCoolTime_);
	imgui->InputInt("BulletRate", bulletRate_);
}

void PlayerData::ImGuiJumpUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->InputFloat("FallAcc", fallAcc);
	imgui->InputFloat("FallVYMin", fallVYMin);
	imgui->InputFloat("JumpFirstSpd", jumpFirstSpd_);
}

//-----------------------------------------------------------------------------
// [SECTION] Save
//-----------------------------------------------------------------------------

void PlayerData::SaveData()
{
	std::string filePath = "Resources/Levels/PlayerParam.txt";

	std::ofstream outPutFile;
	outPutFile.open(filePath);

	//	HP
	outPutFile << "MAXHP " << maxHP_ << std::endl;

	//	Move
	outPutFile << "WalkSPD " << walkSpd_ << std::endl;
	outPutFile << "RunSPD " << runSpd_ << std::endl;
	outPutFile << "JumpDecSPD " << jumpingDecSpd_ << std::endl;

	//	Avoid
	outPutFile << "AvoidMaxSpd " << avoidMaxSpd_ << std::endl;
	outPutFile << "AvoidDecTime " << avoidDecTime_ << std::endl;
	outPutFile << "AvoidAccTime " << avoidAccTime_ << std::endl;
	outPutFile << "AvoidCT " << avoidCoolTime_ << std::endl;

	//	Jump
	outPutFile << "FallAcc " << fallAcc << std::endl;
	outPutFile << "FallMin " << fallVYMin << std::endl;
	outPutFile << "JumpFstSpd " << jumpFirstSpd_ << std::endl;

	outPutFile.close();
}

//-----------------------------------------------------------------------------
// [SECTION] Load
//-----------------------------------------------------------------------------

void PlayerData::LoadData()
{
	std::string filePath = "Resources/Levels/PlayerParam.txt";

	std::ifstream file;
	file.open(filePath);
	//	ファイルが開けなかったら
	assert(!file.fail());

	std::string line;

	// データの上から1行ずつ読み込む
	while (std::getline(file, line))
	{
		std::istringstream line_stream(line);

		//	行の先頭文字格納変数
		std::string key;
		//	半角スペース区切りで行の先頭文字列取得
		getline(line_stream, key, ' ');


		//	HP
		if (key == "MAXHP")
		{
			line_stream >> maxHP_;
			return;
		}

		//	Move
		if (key == "WalkSPD")
		{
			line_stream >> walkSpd_;
			return;
		}
		if (key == "RunSPD")
		{
			line_stream >> runSpd_;
			return;
		}
		if (key == "JumpDecSPD")
		{
			line_stream >> jumpingDecSpd_;
			return;
		}

		//	Avoid
		if (key == "AvoidMaxSpd")
		{
			line_stream >> avoidMaxSpd_;
			return;
		}
		if (key == "AvoidDecTime")
		{
			line_stream >> avoidDecTime_;
			return;
		}
		if (key == "AvoidAccTime")
		{
			line_stream >> avoidAccTime_;
			return;
		}
		if (key == "AvoidCT")
		{
			line_stream >> avoidCoolTime_;
			return;
		}

		//	Jump
		if (key == "FallAcc")
		{
			line_stream >> fallAcc;
			return;
		}
		if (key == "FallMin")
		{
			line_stream >> fallVYMin;
			return;
		}
		if (key == "JumpFstSpd")
		{
			line_stream >> jumpFirstSpd_;
			return;
		}
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

float PlayerData::GetWalkSpd()
{
	return walkSpd_;
}

float PlayerData::GetRunSpd()
{
	return runSpd_;
}

float PlayerData::GetJumpingSpdDec()
{
	return jumpingDecSpd_;
}

int32_t PlayerData::GetAvoidAccTime()
{
	return avoidAccTime_;
}

int32_t PlayerData::GetAvoidDecTime()
{
	return avoidDecTime_;
}

int32_t PlayerData::GetBulletRate()
{
	return bulletRate_;
}
