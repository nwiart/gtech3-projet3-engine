#pragma once
#include <DirectXMath.h>
#include "QuEntity.h"
#include "EntityController.h"
#include "HUDGame.h"
class Player
{
public:	
	Player();
	~Player();

	  static EntityController* GetEntityController();
	  static void SetEntityController(EntityController*);

	  static EntityController* m_Controller;

	  static HUDGame* hud;
	  static void SetHUD(HUDGame*);
	  static HUDGame* GetHUD();


};

