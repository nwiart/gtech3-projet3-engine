#include "Player.h"
#include "stdafx.h"

EntityController* Player::m_Controller;


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


