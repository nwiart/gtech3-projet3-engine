#pragma once

#include "QuEntity.h"


class QuEntityCamera : public QuEntity
{
public:

	QuEntityCamera();

	inline float getFOV() const { return m_fov; }
	inline void setFOV(float f) { m_fov = f; }


private:

	virtual void ExecuteProcedure() override;


private:

	float m_fov;
};
