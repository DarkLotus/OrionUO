﻿/***********************************************************************************
**
** GUIHTMLButton.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUIHTMLButton.h"
#include "GUIHTMLGump.h"
//----------------------------------------------------------------------------------
CGUIHTMLButton::CGUIHTMLButton(CGUIHTMLGump *htmlGump, const uint &serial, const ushort &graphic, const ushort &graphicSelected, const ushort &graphicPressed, const int &x, const int &y)
: CGUIButton(serial, graphic, graphicSelected, graphicPressed, x, y),
m_HTMLGump(htmlGump)
{
}
//----------------------------------------------------------------------------------
CGUIHTMLButton::~CGUIHTMLButton()
{
}
//----------------------------------------------------------------------------------
void CGUIHTMLButton::SetShaderMode()
{
	glUniform1iARB(g_ShaderDrawMode, 0);
}
//----------------------------------------------------------------------------------
void CGUIHTMLButton::Scroll(const bool &up, const uint &delay)
{
	if (m_HTMLGump != NULL)
		m_HTMLGump->Scroll(up, delay);
}
//----------------------------------------------------------------------------------
