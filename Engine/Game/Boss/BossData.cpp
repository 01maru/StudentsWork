#include "BossData.h"
#include "ImGuiManager.h"
#include <fstream>
#include <sstream>
#include <cassert>

using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] ImGui
//-----------------------------------------------------------------------------

void BossData::ImGuiBeam()
{
	//ImGuiManager* imGui = ImGuiManager::GetInstance();

	//imGui->InputFloat("walkSpd", walkSpd_);
	//imGui->InputFloat("runSpd", runSpd_);
	//imGui->InputFloat("jumpingSpdDec", jumpingDecSpd_);
	//imGui->InputFloat("MoveRotMaxAngle", maxAngle_);
	//imGui->InputFloat("MoveRotMinAngle", minAngle_);
}

void BossData::ImGuiStateUpdate(int32_t nowState)
{
	switch (nowState)
	{
	case BossData::BulletState:
		break;
	case BossData::WayBulletsState:
		break;
	case BossData::BeamState:
		ImGuiBeam();
		break;
	case BossData::JumpAtState:
		break;
	case BossData::RockFallState:
		break;
	case BossData::BumpState:
		break;
	case BossData::Tornado:
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Save
//-----------------------------------------------------------------------------

void BossData::SaveData()
{
	std::string filePath = "Resources/Levels/BossParam.txt";

	std::ofstream outPutFile;
	outPutFile.open(filePath);

	//	HP
	outPutFile << "MAXHP " << maxHP_ << std::endl;

	//	Move
	outPutFile << "MoveLen " << lenMin_ << " " << lenMax_ << std::endl;
	outPutFile << "MoveSPD " << moveSpd_ << std::endl;

	for (int32_t i = 0; i < StateNum; i++)
	{
		auto atPriority = priority_.atState[i];
		outPutFile << "Priority " << i << " "
			<< atPriority[FirstForm].range[AttackPriority::Close] << " "
			<< atPriority[SecondForm].range[AttackPriority::Close] << " "
			<< atPriority[FirstForm].range[AttackPriority::Medium] << " "
			<< atPriority[SecondForm].range[AttackPriority::Medium] << " "
			<< atPriority[FirstForm].range[AttackPriority::Long] << " "
			<< atPriority[SecondForm].range[AttackPriority::Long] << std::endl;
	}

	outPutFile.close();
}

//-----------------------------------------------------------------------------
// [SECTION] Load
//-----------------------------------------------------------------------------

void BossData::LoadData()
{
	std::string filePath = "Resources/Levels/BossParam.txt";

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
		if (key == "MoveLen")
		{
			line_stream >> lenMin_;
			line_stream >> lenMax_;
			continue;
		}
		if (key == "MoveSPD")
		{
			line_stream >> moveSpd_;
			continue;
		}

		if (key == "Priority")
		{
			int32_t idx;
			line_stream >> idx;
			line_stream >> priority_.atState[idx][FirstForm].range[AttackPriority::Close];
			line_stream >> priority_.atState[idx][SecondForm].range[AttackPriority::Close];
			line_stream >> priority_.atState[idx][FirstForm].range[AttackPriority::Medium];
			line_stream >> priority_.atState[idx][SecondForm].range[AttackPriority::Medium];
			line_stream >> priority_.atState[idx][FirstForm].range[AttackPriority::Long];
			line_stream >> priority_.atState[idx][SecondForm].range[AttackPriority::Long];

			continue;
		}

		//if (key == "Beam")
		//{

		//	continue;
		//}
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

int32_t BossData::GetLenMin()
{
	return lenMin_;
}

int32_t BossData::GetLenMax()
{
	return lenMax_;
}

float BossData::GetMoveSpd()
{
	return moveSpd_;
}

int32_t BossData::GetBumpRate()
{
    return bumpRateTime_;
}

int32_t BossData::GetBumpLifeTime()
{
    return bumpLifeTime_;
}

float BossData::GetBumpHeight()
{
    return bumpHeight_;
}

float BossData::GetBumpScale()
{
    return bumpScale_;
}

int32_t BossData::GetBulletRate()
{
    return bulletRateTime_;
}

int32_t BossData::GetBulletMaxNum()
{
    return bulletMaxNum_;
}

float BossData::GetBulletSpd()
{
    return bulletSpd_;
}

int32_t BossData::GetBulletSplitNum()
{
    return splitBulletMaxNum_;
}

float BossData::GetBulletAngle()
{
    return bulletWayAngle_;
}

//-----------------------------------------------------------------------------
// [SECTION] BEAM
//-----------------------------------------------------------------------------

int32_t BossData::GetBeamChargeTime()
{
	return beamChargeTime_;
}

int32_t BossData::GetBeamAttackTime()
{
	return beamAttackTime_;
}

int32_t BossData::GetBeamAvoidTime()
{
	return beamAvoidTime_;
}

int32_t BossData::GetBeamEndTime()
{
	return beamEndTime_;
}

int32_t BossData::GetBeamDamage()
{
	return beamDamage_;
}

//-----------------------------------------------------------------------------
// [SECTION] ROCK FALL
//-----------------------------------------------------------------------------

int32_t BossData::GetRockWaitTime()
{
	return rockWaitTime_;
}

//-----------------------------------------------------------------------------
// [SECTION] TORNADO
//-----------------------------------------------------------------------------

int32_t BossData::GetTornadoWaitTime()
{
	return tornadoWaitTime_;
}

int32_t BossData::GetTornadoAtTime()
{
	return tornadoAtTime_;
}

int32_t BossData::GetTornadoEndTime()
{
	return tornadoEndTime_;
}

int32_t BossData::GetTornadoStanTime()
{
	return tornadoStanTime_;
}

float BossData::GetTornadoMaxR()
{
	return tornadoMaxR_;
}

float BossData::GetTornadoRotSpd()
{
	return tornadoRotSpd_;
}

float BossData::GetSuctionValue()
{
	return suctionV_;
}
