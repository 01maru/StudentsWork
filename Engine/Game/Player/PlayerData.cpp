#include "PlayerData.h"
#include "ImGuiManager.h"
#include <fstream>
#include <sstream>
#include <cassert>

using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] ImGuiUpdate
//-----------------------------------------------------------------------------

void PlayerData::ImGuiCamera()
{
	ImGuiManager* imGui = ImGuiManager::GetInstance();

	imGui->InputFloat3("CameraOffset", cameraOffset_);
}

void PlayerData::ImGuiHPUpdate()
{
	ImGuiManager* imGui = ImGuiManager::GetInstance();

	imGui->InputInt("MaxHP", maxHP_);
}

void PlayerData::ImGuiMoveUpdate()
{
	ImGuiManager* imGui = ImGuiManager::GetInstance();

	imGui->InputFloat("walkSpd", walkSpd_);
	imGui->InputFloat("runSpd", runSpd_);
	imGui->InputFloat("jumpingSpdDec", jumpingDecSpd_);
	imGui->InputFloat("MoveRotMaxAngle", maxAngle_);
	imGui->InputFloat("MoveRotMinAngle", minAngle_);
}

void PlayerData::ImGuiAvoidUpdate()
{
	ImGuiManager* imGui = ImGuiManager::GetInstance();

	imGui->InputFloat("AvoidMaxSpd", avoidMaxSpd_);
	imGui->InputInt("AvoidAccTime", avoidAccTime_);
	imGui->InputInt("AvoidDecTime", avoidDecTime_);
	imGui->InputInt("AvoidCoolTime", avoidCoolTime_);
}

void PlayerData::ImGuiAttackUpdate()
{
	ImGuiManager* imGui = ImGuiManager::GetInstance();

	imGui->InputInt("SlowATCoolTime", slowATCoolTime_);
	imGui->InputInt("BulletRate", bulletRate_);
}

void PlayerData::ImGuiJumpUpdate()
{
	ImGuiManager* imGui = ImGuiManager::GetInstance();

	imGui->InputFloat("FallAcc", fallAcc);
	imGui->InputFloat("FallVYMin", fallVYMin);
	imGui->InputFloat("JumpFirstSpd", jumpFirstSpd_);
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
	outPutFile << "MoveRotMaxAngle " << maxAngle_ << std::endl;
	outPutFile << "MoveRotMinAngle " << minAngle_ << std::endl;

	//	Camera
	outPutFile << "CameraOffset "
		<< cameraOffset_.x << " "
		<< cameraOffset_.y << " "
		<< cameraOffset_.z << std::endl;

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
			continue;
		}

		//	Move
		if (key == "WalkSPD")
		{
			line_stream >> walkSpd_;
			continue;
		}
		if (key == "RunSPD")
		{
			line_stream >> runSpd_;
			continue;
		}
		if (key == "JumpDecSPD")
		{
			line_stream >> jumpingDecSpd_;
			continue;
		}
		if (key == "MoveRotMaxAngle")
		{
			line_stream >> maxAngle_;
			continue;
		}
		if (key == "MoveRotMinAngle")
		{
			line_stream >> minAngle_;
			continue;
		}

		//	Camera
		if (key == "CameraOffset")
		{
			line_stream >> cameraOffset_.x;
			line_stream >> cameraOffset_.y;
			line_stream >> cameraOffset_.z;
			continue;
		}

		//	Avoid
		if (key == "AvoidMaxSpd")
		{
			line_stream >> avoidMaxSpd_;
			continue;
		}
		if (key == "AvoidDecTime")
		{
			line_stream >> avoidDecTime_;
			continue;
		}
		if (key == "AvoidAccTime")
		{
			line_stream >> avoidAccTime_;
			continue;
		}
		if (key == "AvoidCT")
		{
			line_stream >> avoidCoolTime_;
			continue;
		}

		//	Jump
		if (key == "FallAcc")
		{
			line_stream >> fallAcc;
			continue;
		}
		if (key == "FallMin")
		{
			line_stream >> fallVYMin;
			continue;
		}
		if (key == "JumpFstSpd")
		{
			line_stream >> jumpFirstSpd_;
			continue;
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

float PlayerData::GetAvoidMaxSpd()
{
	return avoidMaxSpd_;
}

float PlayerData::GetAcc()
{
	return acc_;
}

float PlayerData::GetAccDelta()
{
	return accDelta_;
}

float PlayerData::GetDecelRate()
{
	return decelRate_;
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
