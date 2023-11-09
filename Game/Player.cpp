#include "Player.h"
#include "stdafx.h"

EntityController* Player::m_Controller;

HUDGame* Player::hud;


Player::Player()
{
}

Player::~Player()
{
}

EntityController* Player::GetEntityController()
{
	return m_Controller;
}

void Player::SetEntityController(EntityController* controller)
{
	m_Controller = controller;
}

void Player::SetHUD(HUDGame* HUDtoSet)
{
	hud = HUDtoSet;
}

HUDGame* Player::GetHUD()
{
	return hud;
}


