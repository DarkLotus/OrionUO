﻿/***********************************************************************************
**
** ConnectionScreen.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "ConnectionScreen.h"
#include "../OrionUO.h"
#include "../CharacterList.h"
#include "../Network/Packets.h"
//----------------------------------------------------------------------------------
CConnectionScreen g_ConnectionScreen;
//----------------------------------------------------------------------------------
CConnectionScreen::CConnectionScreen()
: CBaseScreen(m_ConnectionGump), m_ConnectionFailed(false), m_Connected(false),
m_Completed(false), m_ErrorCode(0)
{
}
//----------------------------------------------------------------------------------
CConnectionScreen::~CConnectionScreen()
{
}
//----------------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void CConnectionScreen::Init()
{
	m_ConnectionFailed = false;
	m_Connected = false;
	m_Completed= false;
	m_ErrorCode = 0;
	m_Type = CST_LOGIN;

	g_ScreenEffectManager.UseSunrise();
	m_SmoothScreenAction = 0;

	m_Gump.PrepareTextures();
	m_Gump.WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
void CConnectionScreen::OnChangeConnectionFailed(const bool &val)
{
	m_Gump.WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
void CConnectionScreen::OnChangeConnected(const bool &val)
{
	m_Gump.WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
void CConnectionScreen::OnChangeCompleted(const bool &val)
{
	m_Gump.WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
void CConnectionScreen::OnChangeErrorCode(const int &val)
{
	m_Gump.WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
void CConnectionScreen::OnChangeType(const CONNECTION_SCREEN_TYPE &val)
{
	m_Gump.WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
/*!
Обработка события после плавного затемнения экрана
@param [__in_opt] action Идентификатор действия
@return 
*/
void CConnectionScreen::ProcessSmoothAction(uchar action)
{
	if (action == 0xFF)
		action = m_SmoothScreenAction;

	if (action == ID_SMOOTH_CS_GO_SCREEN_MAIN)
		g_Orion.InitScreen(GS_MAIN);
	else if (action == ID_SMOOTH_CS_GO_SCREEN_CHARACTER)
		g_Orion.InitScreen(GS_CHARACTER);
	else if (action == ID_SMOOTH_CS_GO_SCREEN_PROFESSION)
		g_Orion.InitScreen(GS_PROFESSION_SELECT);
	else if (action == ID_SMOOTH_CS_SEND_DELETE)
		CPacketDeleteCharacter(g_CharacterList.Selected).Send();
}
//----------------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@return 
*/
void CConnectionScreen::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	switch (wParam)
	{
		case VK_RETURN:
		{
			if (m_Type == CST_CHARACTER_LIST)
				CreateSmoothAction(ID_SMOOTH_CS_SEND_DELETE);
			else if (m_Type == CST_SELECT_PROFESSOIN)
				CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_PROFESSION);
			else if (m_Type == CST_GAME)
			{
				if (m_ErrorCode > 0)
					CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_CHARACTER);
			}
			else if (m_Type == CST_CONLOST || m_ConnectionFailed)
				CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_MAIN);

			break;
		}
		case VK_ESCAPE:
		{
			if (m_Type == CST_CHARACTER_LIST)
				CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_CHARACTER);
			else if (m_Type == CST_SELECT_PROFESSOIN)
				CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_PROFESSION);
			else if (m_Type == CST_GAME)
			{
				if (m_ErrorCode > 0)
					CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_CHARACTER);
			}
			else if (m_Type == CST_CONLOST || m_ConnectionFailed)
				CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_MAIN);

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
