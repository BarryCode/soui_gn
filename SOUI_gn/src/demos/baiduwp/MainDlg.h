// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

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
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
    BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
    
protected:

    void OnHideOrShowLeftWndClick();
    void OnTabMainStyleClick();
    void OnTabPageRadioSwitch(int nID);
    void OnFileTypeRadioSwitch(int nID);
    void OnSharePanelRadioSwitch(int nID);
    
    //soui消息
    EVENT_MAP_DECL()
    
    //HostWnd真实窗口消息处理
    BEGIN_MSG_MAP_EX_DECL()
private:
    BOOL            m_bLayoutInited;
    int             m_MainStyle;//0为tile，1为list
    BOOL             m_LeftWndShow;
};
