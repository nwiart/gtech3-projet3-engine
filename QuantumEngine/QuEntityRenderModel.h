#pragma once

#include "QuEntity.h"
#include "Model.h"

class QuEntityRenderModel : public QuEntity
{

	public:
		QuEntityRenderModel();
		~QuEntityRenderModel();

		inline void SetModel(Model* model) { m_model = model; }
		inline Model* GetModel() { return m_model; }

		virtual void ExecuteProcedure();

	private :
		Model* m_model;


};

