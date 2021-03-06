﻿// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "MainDlg.h"
#include "import_table_adapter.h"
#include "export_table_adapter.h"
#include "DragDrop.h"
using namespace SOUI;

EVENT_MAP_BEGIN(CMainDlg)
	EVENT_NAME_COMMAND(L"btn_close", OnClose)
	EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
	EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
	EVENT_NAME_COMMAND(L"btn_restore", OnRestore)
	EVENT_NAME_HANDLER(L"import_table_treeview", SOUI::EVT_CTXMENU, OnTreeViewContextMenu)
	CHAIN_EVENT_MAP_MEMBER(m_imgMergerHandler)
	CHAIN_EVENT_MAP_MEMBER(m_codeLineCounter)
	CHAIN_EVENT_MAP_MEMBER(m_2UnicodeHandler)
	CHAIN_EVENT_MAP_MEMBER(m_folderScanHandler)
	CHAIN_EVENT_MAP_MEMBER(m_calcMd5Handler)
EVENT_MAP_END()

//HostWnd真实窗口消息处理
BEGIN_MSG_MAP_EX(CMainDlg)
	MSG_WM_INITDIALOG(OnInitDialog)
	MSG_WM_CLOSE(OnClose)
	MSG_WM_SIZE(OnSize)
	CHAIN_MSG_MAP(SHostWnd)
	REFLECT_NOTIFICATIONS_EX()
END_MSG_MAP()

CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:dlg_main"))
{
}

CMainDlg::~CMainDlg()
{
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	//HRESULT hr = ;
	::RegisterDragDrop(m_hWnd, GetDropTarget());

    m_imgMergerHandler.OnInit(this);
    m_codeLineCounter.OnInit(this);
    m_2UnicodeHandler.OnInit(this);
    m_folderScanHandler.OnInit(this);
    m_calcMd5Handler.OnInit(this);
	m_windowHelperHander.OnInit(this);
	STreeView * pTreeView1 = FindChildByName2<STreeView>("import_table_treeview");
	STreeView *pTreeView2 = FindChildByName2<STreeView>("export_table_treeview");

	if (pTreeView1&&pTreeView2)
	{
		CImportTableTreeViewAdapter *pImportTableTreeViewAdapter = new CImportTableTreeViewAdapter();
		pTreeView1->SetAdapter(pImportTableTreeViewAdapter);
		pImportTableTreeViewAdapter->Release(); 
		
		CExportTableTreeViewAdapter *pExportTableTreeViewAdapter = new CExportTableTreeViewAdapter();
		pTreeView2->SetAdapter(pExportTableTreeViewAdapter);
		pExportTableTreeViewAdapter->Release();
		RegisterDragDrop(pTreeView1->GetSwnd(), new CTreeViewDropTarget(pTreeView1, pTreeView2));
		RegisterDragDrop(pTreeView2->GetSwnd(), new CTreeViewDropTarget(pTreeView1, pTreeView2));
	}
	return 0;
}

BOOL CMainDlg::OnTreeViewContextMenu(EventArgs *pEvt)
{
	EventCtxMenu *pEvt2 = sobj_cast<EventCtxMenu>(pEvt);
	POINT pt = pEvt2->pt;
	//选中鼠标点击行
	STreeView *pListview = sobj_cast<STreeView>(pEvt2->sender);
	CPoint pt2 = pt;
	ClientToScreen(&pt);
	SItemPanel *pItem = pListview->HitTest(pt2);
	int iItem = 0;
	if (pItem)
	{
		iItem = pItem->GetItemIndex();
		pListview->SetSel(iItem);
	}
	((CImportTableTreeViewAdapter*)pListview->GetAdapter())->HandleTreeViewContextMenu(pt, pItem, m_hWnd);
	return TRUE;
}
//TODO:消息映射
void CMainDlg::OnClose()
{
	DestroyWindow();
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}

