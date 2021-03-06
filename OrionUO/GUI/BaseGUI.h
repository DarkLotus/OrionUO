﻿/***********************************************************************************
**
** BaseGUI.h
**
** Базовый класс компоненты пользовательского интерфейса
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef BASEGUI_H
#define BASEGUI_H
//----------------------------------------------------------------------------------
#include "../Globals.h"
#include "../RenderObject.h"
#include "../GLEngine/GLEngine.h"
//----------------------------------------------------------------------------------
class CBaseGUI : public CRenderObject
{
	//!Тип компоненты
	SETGET(GUMP_OBJECT_TYPE, Type);

	//!Возможность перемещения гампа, если компонента зажата
	SETGET(bool, MoveOnDrag);

	//!Флаг использования PartialHue
	SETGET(bool, PartialHue);

	//!Метод использования функции выбора
	//!			true - проверяет нахождение курсора мышки в пределах полигона (для CGUIPolygonal и компонент с шириной и высотой, либо берет габариты картинки исходного состояния m_Graphic)
	//!			false - стандартная функция проверки пикселей
	SETGET(bool, CheckPolygone);

	//!Включение выключение обработки компоненты (компонента отображается)
	SETGET(bool, Enabled);

	//!Показать/скрыть компоненту и ее обработку
	SETGET(bool, Visible);

	//!Флаг, отвечающий только за выбор компоненты, без ее отображения в гампе
	SETGET(bool, SelectOnly);

	//!Флаг, отвечающий только за отрисовку компоненты, без ее выбора
	SETGET(bool, DrawOnly);

public:
	CBaseGUI(const GUMP_OBJECT_TYPE &type, const uint &serial, const ushort &graphic, const ushort &color, const int &x, const int &y);
	virtual ~CBaseGUI();

	/*
	GOT_SCOPE,				//!Рамка (лепится из 4 кусков)
	GOT_SLIDER,				//!Ползунок
	GOT_HTMLGUMP,			//!ХТМЛ гамп
	GOT_XFMHTMLGUMP,		//!ХТМЛ гамп из клилока (цветной там же)
	GOT_XFMHTMLTOKEN,		//
	*/

	//!Выявление поля g_EntryPointer в данной компоненте
	virtual bool EntryPointerHere() { return false; }

	//!Установить данные для шейдера
	virtual void SetShaderMode() { }

	//!Получить ИД картинки для отображения
	virtual ushort GetDrawGraphic() { return m_Graphic; }

	//!Отрисовать компоненту
	//!		checktrans - использовать трафарет +прозрачность
	virtual void Draw(const bool &checktrans = false) {}

	//!Проверить компоненту на возможность выбора
	virtual bool Select() { return false; }

	//!Получить конечные габариты компоненты
	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(); }

	//!Это компонента пользовательского интерфейса
	virtual bool IsGUI() { return true; }

	//!Это HTMLGump компонента
	virtual bool IsHTMLGump() { return false; }

	//!Это компонента HTMLGump'а (кнопки скроллинга, скроллер, фон, область вывода)
	virtual bool IsControlHTML() { return false; }

	//!Возможность обрабатывать события компонента, если она зажата, но мышка находится где-то еще
	virtual bool IsPressedOuthit() { return false; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
