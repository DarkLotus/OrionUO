﻿/***********************************************************************************
**
** SelectTownScreen.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef SELECTTOWNSCREEN_H
#define SELECTTOWNSCREEN_H
//----------------------------------------------------------------------------------
#include "BaseScreen.h"
#include "../Gumps/GumpScreenSelectTown.h"
#include "../CityList.h"
//----------------------------------------------------------------------------------
class CSelectTownScreen : public CBaseScreen
{
private:
	CGumpScreenSelectTown m_SelectTownGump;

public:
	CSelectTownScreen();
	~CSelectTownScreen();

	//!Идентификаторы событий для плавного перехода
	static const uchar ID_SMOOTH_STS_QUIT = 1;
	static const uchar ID_SMOOTH_STS_GO_SCREEN_CHARACTER = 2;
	static const uchar ID_SMOOTH_STS_GO_SCREEN_GAME_CONNECT = 3;

	//!Указатель на выбранный город
	CCityItem m_City;

	/*!
	Инициализация
	@return 
	*/
	void Init();

	/*!
	Обработка события после плавного затемнения экрана
	@param [__in_opt] action Идентификатор действия
	@return 
	*/
	void ProcessSmoothAction(uchar action = 0xFF);
};
//----------------------------------------------------------------------------------
extern CSelectTownScreen g_SelectTownScreen;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
