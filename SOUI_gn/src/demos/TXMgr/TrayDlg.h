// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#define  Update_Update_Speed		101
#define  Update_Down_Speed			102
#define  Update_Use_Speed			103


class CTrayDlg : public SOUI::SHostWnd
{
public:
	CTrayDlg();
	~CTrayDlg();


	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);


	void OnTimer(char nIDEvent);							// 定时器

	void OnUseBg();

protected:
	bool OnUseBgMouseHover(SOUI::EventArgs *e);
	bool OnUseBgMouseLeave(SOUI::EventArgs *e);

	virtual void OnFinalMessage(HWND hWnd);
	
	virtual void OnLButtonDown(UINT nFlags, SOUI::CPoint pt);

	virtual void OnLButtonUp(UINT nFlags, SOUI::CPoint pt);

protected:
	void OnRocketVisibleChange(SOUI::EventArgs *e);
	//soui消息
	EVENT_MAP_DECL()

	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX_DECL()
private:
	BOOL			m_bLayoutInited;

	int				m_nFullWidth;
	int				m_nFullHeight;
};
