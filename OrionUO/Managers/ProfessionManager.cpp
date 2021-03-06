﻿/***********************************************************************************
**
** ProfessionManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "ProfessionManager.h"
#include "../OrionUO.h"
#include "../OrionWindow.h"
#include "../Skills.h"
#include "../Wisp/WispLogger.h"
#include "../Wisp/WispApplication.h"
#include "../Wisp/WispTextFileParser.h"
#include "../Wisp/WispMappedFile.h"
//----------------------------------------------------------------------------------
CProfessionManager g_ProfessionManager;
//----------------------------------------------------------------------------------
//!Ключи конфига
const string CProfessionManager::m_Keys[m_KeyCount] =
{
	"begin",
	"name",
	"truename",
	"desc",
	"toplevel",
	"gump",
	"type",
	"children",
	"skill",
	"stat",
	"str",
	"int",
	"dex",
	"end",
	"true",
	"category",
	"nameid",
	"descid"
};
//----------------------------------------------------------------------------------
CProfessionManager::CProfessionManager()
: CBaseQueue(), Selected(NULL)
{
}
//----------------------------------------------------------------------------------
CProfessionManager::~CProfessionManager()
{
}
//----------------------------------------------------------------------------------
/*!
Получить код кофига по строке
@param [__in] key Строка из файла
@return Код конфига
*/
int CProfessionManager::GetKeyCode(const string &key)
{
	string str = ToLowerA(key);
	int result = 0;

	IFOR(i, 0, m_KeyCount && !result)
	{
		if (str == m_Keys[i])
			result = i + 1;
	}

	return result;
}
//----------------------------------------------------------------------------------
/*!
Обработать часть файла (загрузка профессии)
@param [__in] file Указатель на файл
@return true при успешной обработке
*/
bool CProfessionManager::ParseFilePart(WISP_FILE::CTextFileParser &file)
{
	PROFESSION_TYPE type = PT_NO_PROF;
	std::vector<string> childrens;
	string name = "";
	string trueName = "";
	uint nameClilocID = 0;
	uint descriptionClilocID = 0;
	int descriptionIndex = 0;
	ushort gump = 0;
	bool topLevel = false;
	int skillCount = 0;
	int skillIndex[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
	int skillValue[4] = { 0 };
	int stats[3] = { 0 };

	bool exit = false;

	while (!file.IsEOF() && !exit)
	{
		STRING_LIST strings = file.ReadTokens();

		if (!strings.size())
			continue;

		int code = GetKeyCode(strings[0]);

		switch (code)
		{
			case PM_CODE_BEGIN:
			case PM_CODE_END:
			{
				exit = true;
				break;
			}
			case PM_CODE_NAME:
			{
				name = strings[1];
				break;
			}
			case PM_CODE_TRUENAME:
			{
				trueName = strings[1];
				break;
			}
			case PM_CODE_DESC:
			{
				descriptionIndex = atoi(strings[1].c_str());
				break;
			}
			case PM_CODE_TOPLEVEL:
			{
				topLevel = (GetKeyCode(strings[1]) == PM_CODE_TRUE);
				break;
			}
			case PM_CODE_GUMP:
			{
				gump = atoi(strings[1].c_str());

				g_Orion.ExecuteGump(gump);
				g_Orion.ExecuteGump(gump + 1);
				break;
			}
			case PM_CODE_TYPE:
			{
				if (GetKeyCode(strings[1]) == PM_CODE_CATEGORY)
					type = PT_CATEGORY;
				else
					type = PT_PROFESSION;

				break;
			}
			case PM_CODE_CHILDREN:
			{
				IFOR(j, 1, (int)strings.size())
					childrens.push_back(strings[j]);

				break;
			}
			case PM_CODE_SKILL:
			{
				if (strings.size() > 2 && skillCount < 4)
				{
					IFOR(j, 0, 54)
					{
						if (strings[1] == g_SkillName[j])
						{
							skillIndex[skillCount] = j;
							skillValue[skillCount] = atoi(strings[2].c_str());
							skillCount++;

							break;
						}
					}
				}

				break;
			}
			case PM_CODE_STAT:
			{
				if (strings.size() > 2)
				{
					code = GetKeyCode(strings[1]);
					int val = atoi(strings[2].c_str());

					if (code == PM_CODE_STR)
						stats[0] = val;
					else if (code == PM_CODE_INT)
						stats[1] = val;
					else if (code == PM_CODE_DEX)
						stats[2] = val;
				}

				break;
			}
			case PM_CODE_NAME_CLILOC_ID:
			{
				nameClilocID = atoi(strings[1].c_str());
				break;
			}
			case PM_CODE_DESCRIPTION_CLILOC_ID:
			{
				descriptionClilocID = atoi(strings[1].c_str());
				break;
			}
			default:
				break;
		}
	}

	CBaseProfession *obj = NULL;

	if (type == PT_CATEGORY)
	{
		CProfessionCategory *temp = new CProfessionCategory();

		IFOR(i, 0, (int)childrens.size())
			temp->AddChildren(childrens[i]);

		obj = temp;
	}
	else if (type == PT_PROFESSION)
	{
		CProfession *temp = new CProfession();

		temp->Str = stats[0];
		temp->Int = stats[1];
		temp->Dex = stats[2];

		IFOR(i, 0, 4)
		{
			temp->SetSkillIndex(i, (BYTE)skillIndex[i]);
			temp->SetSkillValue(i, (BYTE)skillValue[i]);
		}

		obj = temp;
	}

	bool result = (type != PT_NONE);

	if (obj != NULL)
	{
		obj->NameClilocID = nameClilocID;
		obj->SetName(name);
		obj->TrueName = trueName;
		obj->DescriptionClilocID = descriptionClilocID;
		obj->DescriptionIndex = descriptionIndex;
		obj->TopLevel = topLevel;
		obj->Gump = gump;
		obj->Type = type;

		if (topLevel)
			m_Items->Add(obj);
		else
		{
			CBaseProfession *parent = (CBaseProfession*)m_Items;

			while (parent != NULL)
			{
				result = AddChild(parent, obj);

				if (result)
					break;

				parent = (CBaseProfession*)parent->m_Next;
			}

			if (!result)
				delete obj;
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
/*!
Добавить ребенка в профессию
@param [__in] parent Ссылка на родителя
@param [__in] child Ссылка на детишку
@return true в случае успешного добавления
*/
bool CProfessionManager::AddChild(CBaseProfession *parent, CBaseProfession *child)
{
	bool result = false;

	if (parent->Type == PT_CATEGORY)
	{
		CProfessionCategory *cat = (CProfessionCategory*)parent;

		string check = string("|") + child->GetName() + "|";

		if (cat->Childrens.find(check) != string::npos)
		{
			cat->Add(child);
			result = true;
		}
		else
		{
			CBaseProfession *item = (CBaseProfession*)cat->m_Items;

			while (item != NULL)
			{
				result = AddChild(item, child);

				if (result)
					break;

				item = (CBaseProfession*)item->m_Next;
			}
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
/*!
Загрузка профессий из файла
@return true в случае успешной загрузки
*/
bool CProfessionManager::Load()
{
	bool result = false;

	CProfessionCategory *head = new CProfessionCategory();
	head->SetTrueName("parent");
	head->SetName("Parent");
	head->DescriptionIndex = -2;
	head->Type = PT_CATEGORY;
	head->Gump = 0x15A9;
	head->TopLevel = true;
	Add(head);

	WISP_FILE::CTextFileParser file(g_App.FilePath("prof.txt"), " \t,", "#;", "\"\"");

	if (!file.IsEOF())
	{
		while (!file.IsEOF())
		{
			std::vector<std::string> strings = file.ReadTokens();

			if (strings.size() > 0)
			{
				if (ToLowerA(strings[0]) == string("begin"))
				{
					result = ParseFilePart(file);

					if (!result)
						break;
				}

			}
		}

		g_Orion.ExecuteGump(0x15A9);
		g_Orion.ExecuteGump(0x15AA);

		CProfession *apc = new CProfession();
		apc->SetTrueName("advanced");
		apc->OnChangeName("Advanced");
		apc->Type = PT_PROFESSION;
		apc->Gump = 0x15A9;
		apc->DescriptionIndex = -1;
		apc->Str = 44;
		apc->Int = 10;
		apc->Dex = 11;
		apc->SetSkillIndex(0, 0xFF);
		apc->SetSkillIndex(1, 0xFF);
		apc->SetSkillIndex(2, 0xFF);
		apc->SetSkillIndex(3, 0xFF);
		apc->SetSkillValue(0, 50);
		apc->SetSkillValue(1, 50);
		apc->SetSkillValue(2, 0);
		apc->SetSkillValue(3, 0);

		head->Add(apc);

		LoadProfessionDescription();
	}
	else
	{
		LOG("Failed to load prof.txt\n");
		g_OrionWindow.ShowMessage("Failed to load prof.txt", "Failed to load");
	}

	return result;
}
//----------------------------------------------------------------------------------
/*!
Загрузка описания профессий
@return 
*/
void CProfessionManager::LoadProfessionDescription()
{
	WISP_FILE::CMappedFile file;

	if (file.Load(g_App.FilePath("professn.enu")))
	{
		char *ptr = (char*)file.Start;
		char *end = (char*)((uint)file.Start + file.Size);

		vector<string> list;

		while (ptr < end)
		{
			if (!memcmp(ptr, "TEXT", 4))
			{
				ptr += 8;

				while (ptr < end)
				{
					if (!(*(ptr - 1)) && !(*ptr)) //end of names section
					{
						ptr++;

						break;
					}

					list.push_back(ptr);
					ptr += strlen(ptr) + 1;
				}

				((CBaseProfession*)m_Items)->AddDescription(-2, "parent", ptr);
				ptr += strlen(ptr) + 1;

				IFOR(i, 0, (int)list.size())
				{
					if (!((CBaseProfession*)m_Items)->AddDescription(i - 1, list[i], ptr))
					{
						//LOG("Failed to add description! (%s)\n", list[i].c_str());
					}
					ptr += strlen(ptr) + 1;
				}

				break;
			}

			ptr++;
		}

		list.clear();

		file.Unload();
	}
	else
	{
		LOG("Failed to load professn.enu\n");
		g_OrionWindow.ShowMessage("Failed to load professn.enu", "Failed to load");
	}
}
//----------------------------------------------------------------------------------
/*!
Получить родителя профессии
@param [__in] obj Ссылка на объект профессии
@param [__in_opt] check Ссылка на проверочный объект профессии
@return Ссылка на найденного родителя объекта профессии
*/
CBaseProfession *CProfessionManager::GetParent(CBaseProfession *obj, CBaseProfession *check)
{
	if (check == NULL)
		check = (CBaseProfession*)m_Items;

	if (obj == m_Items)
		return obj;

	CBaseProfession *item = (CBaseProfession*)check->m_Items;
	CBaseProfession *result = NULL;

	while (item != NULL && result == NULL)
	{
		if (obj == item)
			result = check;
		else
			result = GetParent(obj, item);

		item = (CBaseProfession*)item->m_Next;
	}

	return result;
}
//----------------------------------------------------------------------------------
