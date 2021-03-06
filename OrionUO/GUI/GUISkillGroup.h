﻿/***********************************************************************************
**
** GUISkillGroup.h
**
** Компонента для группы скиллов
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUISKILLGROUP_H
#define GUISKILLGROUP_H
//----------------------------------------------------------------------------------
#include "BaseGUI.h"
#include "GUIButton.h"
#include "GUITextEntry.h"
#include "../GLEngine/GLTextTexture.h"
//----------------------------------------------------------------------------------
class CGUISkillGroup : public CBaseGUI
{
	//!Свернута или развернута группа
	SETGETE(bool, Minimized, OnChangeMinimized);

public:
	CGUISkillGroup(const uint &serial, const uint &minimizeSerial, class CSkillGroupObject *group, const int &x, const int &y);
	virtual ~CGUISkillGroup();

	//!Компоненты управления
	CGUIButton *m_Minimizer;
	CGUITextEntry *m_Name;

	void UpdateDataPositions();

	virtual WISP_GEOMETRY::CSize GetSize();

	virtual void PrepareTextures();

	virtual bool EntryPointerHere();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();

	//!Получить ссылку на выбранную компоненту или на себя
	CBaseGUI *SelectedItem();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
