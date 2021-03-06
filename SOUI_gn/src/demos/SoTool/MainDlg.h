﻿// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ImageMergerHandler.h"
#include "CodeLineCounterHandler.h"
#include "2UnicodeHandler.h"
#include "FolderScanHandler.h"
#include "CalcMd5Handler.h"
#include "CWindowHelperHander.h"

class CMainDlg : public SOUI::SHostWnd
{
public:
	CMainDlg();
	~CMainDlg();

	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	void OnSize(UINT nType, SOUI::CSize size);

	void OnBtnMsgBox();
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	BOOL OnTreeViewContextMenu(SOUI::EventArgs * pEvt);
protected:
	//soui消息
	EVENT_MAP_DECL()
	
	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX_DECL()
	
protected:
    CImageMergerHandler     m_imgMergerHandler;
    CCodeLineCounterHandler m_codeLineCounter;
    C2UnicodeHandler        m_2UnicodeHandler;
    CFolderScanHandler      m_folderScanHandler;
    CCalcMd5Handler         m_calcMd5Handler;
	CWindowHelperHander		m_windowHelperHander;
};
