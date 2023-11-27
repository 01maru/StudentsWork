#pragma once
#include "Vector3D.h"
#include "Particle.h"

class ParticleEmitter;

class EmitterType
{
public:
	~EmitterType() = default;
	virtual void Update(Particle* /*particle*/) {};

protected:
	ParticleEmitter* parent_;
	Vector3D dir_;
	Vector3D pos_;

public:
	void SetParent(ParticleEmitter* parent);
	void SetPosition(const Vector3D& pos) { pos_ = pos; }
	Vector3D GetDir() { return dir_; }
	Vector3D GetPosition() { return pos_; }
};

