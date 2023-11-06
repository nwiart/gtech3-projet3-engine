#pragma once

#include "QuEntity.h"

class Model;


class QuEntityRenderModel : public QuEntity
{
	public:

		QuEntityRenderModel();
		~QuEntityRenderModel();

		inline void SetModel(Model* model) { m_model = model; }
		inline Model* GetModel() const { return m_model; }

		virtual void ExecuteProcedure();


	private:

		Model* m_model;
};
