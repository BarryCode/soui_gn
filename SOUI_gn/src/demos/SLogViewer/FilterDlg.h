﻿#pragma once
#include <souicoll.h>
class CMainDlg;
using namespace SOUI;

interface IFilterChangeListener
{
	virtual void OnTagChange(const SArray<SStringW> & lstTags) =0;
	virtual void OnPidChange(const SArray<UINT> & lstPids) = 0;
	virtual void OnTidChange(const SArray<UINT> & lstTids) = 0;
};

class CFilterDlg : public SHostWnd , public IFilterChangeListener
{
public:
	CFilterDlg(CMainDlg *pMainDlg);
	~CFilterDlg(void);

	void UpdateTags(const SArray<SStringW> & lstTags);
	void UpdatePids(const SArray<UINT> & lstPids);
	void UpdateTids(const SArray<UINT> & lstTids);
	void ExcludeTag(const SStringW & strTag);
protected:

	void OnInit(EventArgs *e);
	void OnTabChanged(EventArgs *e);
	void OnBtnSelectAll();
	void OnBtnClearAll();

	EVENT_MAP_DECL()

	virtual void OnFinalMessage(HWND hWnd);

	virtual void OnTagChange(const SArray<SStringW> & lstTags);
	virtual void OnPidChange(const SArray<UINT> & lstPid);
	virtual void OnTidChange(const SArray<UINT> & lstTid);
	void OnlyTag(const SStringW & strTag);
	enum {
		FilterTag=0,
		FilterPid,
		FilterTid,

		FilterCount,
	};
	SListView * m_lvFilters[FilterCount];
	int			m_iTab;
	CMainDlg  * m_pMainDlg;
};
