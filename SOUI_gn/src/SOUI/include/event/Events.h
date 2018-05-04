﻿/**
* Copyright (C) 2014-2050 
* All rights reserved.
* 
* @file       Events.h
* @brief      
* @version    v1.0      
* @author     SOUI group   
* @date       2014/08/02
* 
* Describe    SOUI系统中使用的事件系统
*/

#pragma once

#include <sobject/Sobject.hpp>

namespace SOUI
{
    class SWindow;
    enum SOUI_EVENTS
    {
        EVT_INIT=8000,
        EVT_EXIT,
        EVT_TIMER,

        //基本窗口事件
        EVT_SETFOCUS=8100,
        EVT_KILLFOCUS,
        EVT_CREATE,
        EVT_DESTROY,
        EVT_SIZE,
        EVT_VISIBLECHANGED,
        EVT_STATECHANGED,
		EVT_LBUTTONDOWN,
		EVT_LBUTTONUP,

        //两个窗口鼠标状态事件
        EVT_MOUSE_HOVER=9000,
        EVT_MOUSE_LEAVE,
        
        EVT_CMD=10000,
        EVT_CTXMENU,
        
        //增加两个滚动VIEW的事件
        EVT_SCROLLVIEW_ORIGINCHANGED,
        EVT_SCROLLVIEW_SIZECHANGED,

        EVT_SCROLL,
        EVT_OFEVENT,    //消息二次包装
        EVT_OFPANEL,    //一个itemPanel中的消息的二次包装

        //增加4个itempanel相关的事件,经过EVT_OFPANEL到达应用层
        EVT_ITEMPANEL_CLICK,
        EVT_ITEMPANEL_DBCLICK,
        EVT_ITEMPANEL_RCLICK,
        EVT_ITEMPANEL_HOVER,
        EVT_ITEMPANEL_LEAVE,
        
        EVT_TAB_SELCHANGING=11000,
        EVT_TAB_SELCHANGED,
        EVT_TAB_ITEMHOVER,
        EVT_TAB_ITEMLEAVE,

        EVT_LB_SELCHANGING=12000,
        EVT_LB_SELCHANGED,
		EVT_LB_DBCLICK,

        EVT_LC_SELCHANGING=13000,
        EVT_LC_SELCHANGED,
        EVT_LC_ITEMDELETED,
		EVT_LC_DBCLICK,

		EVT_TV_SELCHANGING = 14000,
        EVT_TV_SELCHANGED,
        
        EVT_TC_SELCHANGING=15000,
        EVT_TC_SELCHANGED,
        EVT_TC_EXPAND,
        EVT_TC_CHECKSTATE,
        EVT_TC_DBCLICK,    //treectrl的叶子节点双击事件, add by zhaosheng
        
        EVT_LV_SELCHANGING = 15100,
        EVT_LV_SELCHANGED,
        EVT_LV_ITEMCLICK,
        
        EVT_RE_NOTIFY=16000,
        EVT_RE_MENU,
        
        EVT_SLIDER_POS=17000,

        EVT_HEADER_CLICK=18000,
        EVT_HEADER_ITEMCHANGING,
        EVT_HEADER_ITEMCHANGED,
        EVT_HEADER_ITEMSWAP,

        EVT_CB_SELCHANGE=19000,
		EVT_CB_DROPDOWN,
        
        EVT_CALENDAR_SELDAY=20000,
        EVT_CALENDAR_SETDATE,
		EVT_CALENDAREX_CHANGED,
		EVT_DATETIME_CHANGED,
        
        EVT_SPIN_VALUE2STRING = 21000,
        
        EVT_SPLIT_PANE_MOVED  = 22000,
        
		EVT_HOT_KEY_SET = 23000,
		//图片动画开始，结束事件
		EVT_ANI_START	= 22100,
		EVT_ANI_STOP,

		EVT_SELECTMENU = 22150,

        EVT_EXTERNAL_BEGIN=10000000,
    };


    /*!
    \brief
    Base class used as the argument to all subscribers Event object.

    The base EventArgs class does not contain any useful information, it is intended
    to be specialised for each type of event that can be generated by objects within
    the system.  The use of this base class allows all event subscribers to have the
    same function signature.

    The \a handled field is used to signal whether an event was actually handled or not.  While
    the event system does not look at this value, code at a higher level can use it to determine
    how far to propagate an event.
    */
    class SOUI_EXP EventArgs : public TObjRefImpl<IObjRef>, public SObjectImpl<IObject>
    {
        SOUI_CLASS_NAME_DECL(EventArgs,L"eventargs")
    public:
        /*************************************************************************
        Construction
        *************************************************************************/
		EventArgs(SObject *pSender);
		~EventArgs(void) override ;

        /*************************************************************************
        Data members
        *************************************************************************/
        bool bubbleUp; /**< indicating whether the event should bubble up along the event handle chain, event handler can set the value to true to cancel the procedure  */
        UINT handled; /**< handlers should increment this if they handled the event. */

        int  idFrom; /**< 事件发送者ID */
        LPCWSTR nameFrom; /**< 事件发送者name */
        
        SObject *sender;  /**< 产生事件的原始SOUI对象，可能和idFrom and nameFrom指定的对象不同 */
    };
    

    /*!
    \brief 一个事件对象的模板类，用来实现EventArgs两个虚函数
    */
    template<class T>
    class TplEventArgs : public EventArgs
    {
    public:
        TplEventArgs(SObject *pSender):EventArgs(pSender)
        {
        }

        virtual int GetID() const {return T::EventID;}
        virtual LPCWSTR GetName() const {return T::GetClassName();}
    };

    class SOUI_EXP EventCmnArgs : public EventArgs
    {
    public:
        EventCmnArgs(SObject *pSender,int _nID):EventArgs(pSender),nID(_nID)
        {

        }
		int GetID() const override;

    protected:
        int nID;
    };

    #define EVENTID(x) x::EventID,x::GetClassName()

    class SOUI_EXP EventInit : public TplEventArgs<EventInit>
    {
        SOUI_CLASS_NAME_DECL(EventInit,L"on_init")
    public:
        EventInit(SObject *pSender):TplEventArgs<EventInit>(pSender){}
        enum{EventID=EVT_INIT};
    };

    class SOUI_EXP EventExit : public TplEventArgs<EventExit>
    {
        SOUI_CLASS_NAME_DECL(EventExit,L"on_exit")
    public:
        EventExit(SObject *pSender):TplEventArgs<EventExit>(pSender){}
        enum{EventID=EVT_EXIT};
    };

    class SOUI_EXP EventTimer : public TplEventArgs<EventTimer>
    {
        SOUI_CLASS_NAME_DECL(EventTimer,L"on_timer")
    public:
		EventTimer(SObject *pSender, UINT _uID);
        enum{EventID=EVT_TIMER};

        UINT uID;
    };

    class SOUI_EXP EventSwndCreate : public TplEventArgs<EventSwndCreate>
    {
        SOUI_CLASS_NAME_DECL(EventSwndCreate,L"on_create")
    public:
        EventSwndCreate(SObject *pSender):TplEventArgs<EventSwndCreate>(pSender){}
        enum{EventID=EVT_CREATE};
    };

    class SOUI_EXP EventSwndDestroy : public TplEventArgs<EventSwndDestroy>
    {
        SOUI_CLASS_NAME_DECL(EventSwndDestroy,L"on_destroy")
    public:
        EventSwndDestroy(SObject *pSender):TplEventArgs<EventSwndDestroy>(pSender){}
        enum{EventID=EVT_DESTROY};
    };

    class SOUI_EXP EventSwndSize : public TplEventArgs<EventSwndSize>
    {
        SOUI_CLASS_NAME_DECL(EventSwndSize,L"on_size")
    public:
		EventSwndSize(SObject *pSender, CSize _szWnd);
        enum{EventID=EVT_SIZE};
        CSize szWnd;
    };

    class SOUI_EXP EventSwndStateChanged : public TplEventArgs<EventSwndStateChanged>
    {
        SOUI_CLASS_NAME_DECL(EventSwndStateChanged,L"on_state_changed")
    public:
		EventSwndStateChanged(SObject *pSender);
        enum{EventID=EVT_STATECHANGED};
        
        bool CheckState(DWORD dwState)
        {
            return (dwOldState & dwState)!= (dwNewState & dwState);
        }
        
        DWORD dwOldState;
        DWORD dwNewState;
    };

    class SOUI_EXP EventSwndVisibleChanged : public TplEventArgs<EventSwndVisibleChanged>
    {
        SOUI_CLASS_NAME_DECL(EventSwndVisibleChanged,L"on_visible_changed")
    public:
        EventSwndVisibleChanged(SObject *pSender):TplEventArgs<EventSwndVisibleChanged>(pSender){}
        enum{EventID=EVT_VISIBLECHANGED};
    };

    class SOUI_EXP EventSwndMouseLeave : public TplEventArgs<EventSwndMouseLeave>
    {
        SOUI_CLASS_NAME_DECL(EventSwndMouseLeave,L"on_mouse_leave")
    public:
        EventSwndMouseLeave(SObject *pSender):TplEventArgs<EventSwndMouseLeave>(pSender){}
        enum{EventID=EVT_MOUSE_LEAVE};
    };

    class SOUI_EXP EventSwndMouseHover : public TplEventArgs<EventSwndMouseHover>
    {
        SOUI_CLASS_NAME_DECL(EventSwndMouseHover,L"on_mouse_hover")
    public:
        EventSwndMouseHover(SObject *pSender):TplEventArgs<EventSwndMouseHover>(pSender){}
        enum{EventID=EVT_MOUSE_HOVER};
    };

    class SOUI_EXP EventItemPanelRclick : public TplEventArgs<EventItemPanelRclick>
    {
        SOUI_CLASS_NAME_DECL(EventItemPanelRclick,L"on_itemparem_rclick")
    public:
		EventItemPanelRclick(SObject *pSender, WPARAM wp, LPARAM lp);
        enum{EventID=EVT_ITEMPANEL_RCLICK};
        WPARAM wParam;
        LPARAM lParam;
    };

    class SOUI_EXP EventItemPanelDbclick : public TplEventArgs<EventItemPanelDbclick>
    {
        SOUI_CLASS_NAME_DECL(EventItemPanelDbclick,L"on_itempanel_dbclick")
    public:
		EventItemPanelDbclick(SObject *pSender, WPARAM wp, LPARAM lp);
        enum{EventID=EVT_ITEMPANEL_DBCLICK};
        WPARAM wParam;
        LPARAM lParam;
    };

    class SOUI_EXP EventItemPanelClick : public TplEventArgs<EventItemPanelClick>
    {
        SOUI_CLASS_NAME_DECL(EventItemPanelClick,L"on_itempanel_click")
    public:
		EventItemPanelClick(SObject *pSender, WPARAM wp, LPARAM lp);
        enum{EventID=EVT_ITEMPANEL_CLICK};
        WPARAM wParam;
        LPARAM lParam;
    };

    class SOUI_EXP EventItemPanelHover : public TplEventArgs<EventItemPanelHover>
    {
        SOUI_CLASS_NAME_DECL(EventItemPanelHover,L"on_itempanel_hover")
    public:
		EventItemPanelHover(SObject *pSender, WPARAM wp, LPARAM lp);
        enum{EventID=EVT_ITEMPANEL_HOVER};
        WPARAM wParam;
        LPARAM lParam;
    };
    
    //注：在EventItemPanelLeave中从IItemPanel中通过GetItemIndex获取表项索引时需要检查索引有效性。
    class SOUI_EXP EventItemPanelLeave : public TplEventArgs<EventItemPanelLeave>
    {
        SOUI_CLASS_NAME_DECL(EventItemPanelLeave,L"on_itempanel_leave")
    public:
		EventItemPanelLeave(SObject *pSender);
        enum{EventID=EVT_ITEMPANEL_LEAVE};
    };
    
	class SOUI_EXP EventLButtonDown : public TplEventArgs<EventLButtonDown>
	{
		SOUI_CLASS_NAME_DECL(EventLButtonDown, L"on_mouse_lbutton_down")
	public:
		EventLButtonDown(SObject *pSender);
		enum { EventID = EVT_LBUTTONDOWN };
		POINT           pt;
	};

	class SOUI_EXP EventLButtonUp : public TplEventArgs<EventLButtonUp>
	{
		SOUI_CLASS_NAME_DECL(EventLButtonUp, L"on_mouse_lbutton_up")
	public:
		EventLButtonUp(SObject *pSender);
		enum { EventID = EVT_LBUTTONUP };
		POINT           pt;
	};

    class SOUI_EXP EventCmd : public TplEventArgs<EventCmd>
    {
        SOUI_CLASS_NAME_DECL(EventCmd,L"on_command")
    public:
		EventCmd(SObject *pSender);
        enum{EventID=EVT_CMD};
    };

    class SOUI_EXP EventCtxMenu : public TplEventArgs<EventCtxMenu>
    {
        SOUI_CLASS_NAME_DECL(EventCtxMenu,L"on_conext_menu")
    public:
		EventCtxMenu(SObject *pSender);
        enum{EventID=EVT_CTXMENU};
        POINT           pt;
        BOOL            bCancel;
    };

    class SOUI_EXP EventSetFocus : public TplEventArgs<EventSetFocus>
    {
        SOUI_CLASS_NAME_DECL(EventSetFocus,L"on_set_focus")
    public:
		EventSetFocus(SObject *pSender);
        enum{EventID=EVT_SETFOCUS};
    };

    class SOUI_EXP EventKillFocus : public TplEventArgs<EventKillFocus>
    {
        SOUI_CLASS_NAME_DECL(EventKillFocus,L"on_kill_focus")
    public:
		EventKillFocus(SObject *pSender);
        enum{EventID=EVT_KILLFOCUS};
    };
    
    class SOUI_EXP EventScrollViewOriginChanged : public TplEventArgs<EventScrollViewOriginChanged>
    {
        SOUI_CLASS_NAME_DECL(EventScrollViewOriginChanged,L"on_scrollview_origin_changed")
    public:
		EventScrollViewOriginChanged(SObject *pSender);
        enum{EventID=EVT_SCROLLVIEW_ORIGINCHANGED};

        CPoint ptOldOrigin;
        CPoint ptNewOrigin;
    };

    class SOUI_EXP EventScrollViewSizeChanged : public TplEventArgs<EventScrollViewSizeChanged>
    {
        SOUI_CLASS_NAME_DECL(EventScrollViewSizeChanged,L"on_scrollview_size_changed")
    public:
		EventScrollViewSizeChanged(SObject *pSender);
        enum{EventID=EVT_SCROLLVIEW_SIZECHANGED};

        CSize szOldViewSize;
        CSize szNewViewSize;
    };

    class SOUI_EXP EventScroll : public TplEventArgs<EventScroll>
    {
        SOUI_CLASS_NAME_DECL(EventScroll,L"on_scroll")
    public:
		EventScroll(SObject *pSender);
        enum{EventID=EVT_SCROLL};
        UINT        uSbCode;
        int         nPos;
        BOOL        bVertical;
    };

    class SOUI_EXP EventOfEvent : public TplEventArgs<EventOfEvent>
    {
        SOUI_CLASS_NAME_DECL(EventOfEvent,L"on_event_of_event")
    public:
		EventOfEvent(SObject *pSender, EventArgs *_pOrgEvt);
        enum{EventID=EVT_OFEVENT};
        EventArgs * pOrgEvt;
    };

    class SItemPanel;
    class SOUI_EXP EventOfPanel : public TplEventArgs<EventOfPanel>
    {
        SOUI_CLASS_NAME_DECL(EventOfPanel,L"on_event_of_panel")
    public:
		EventOfPanel(SObject * pSender, SItemPanel *_pPanel, EventArgs *_pOrgEvt);
        enum{EventID=EVT_OFPANEL};

        SItemPanel *pPanel;
        EventArgs * pOrgEvt;
    };
    
    
    class SOUI_EXP EventTabSelChanging : public TplEventArgs<EventTabSelChanging>
    {
        SOUI_CLASS_NAME_DECL(EventTabSelChanging,L"on_tab_sel_changing")
    public:
		EventTabSelChanging(SObject *pSender);
        enum{EventID=EVT_TAB_SELCHANGING};
        UINT        uOldSel;
        UINT        uNewSel;
        BOOL        bCancel;
    };

    class SOUI_EXP EventTabSelChanged : public TplEventArgs<EventTabSelChanged>
    {
        SOUI_CLASS_NAME_DECL(EventTabSelChanged,L"on_tab_sel_changed")
    public:
		EventTabSelChanged(SObject *pSender);
        enum{EventID=EVT_TAB_SELCHANGED};
        UINT        uOldSel;
        UINT        uNewSel;
    };

    class SOUI_EXP EventTabItemHover : public TplEventArgs<EventTabItemHover>
    {
        SOUI_CLASS_NAME_DECL(EventTabItemHover,L"on_tab_item_hover")
    public:
		EventTabItemHover(SObject *pSender);
        enum{EventID=EVT_TAB_ITEMHOVER};
        int iHover;
    };

    class SOUI_EXP EventTabItemLeave : public TplEventArgs<EventTabItemLeave>
    {
        SOUI_CLASS_NAME_DECL(EventTabItemLeave,L"on_tab_item_leave")
    public:
		EventTabItemLeave(SObject *pSender);
        enum{EventID=EVT_TAB_ITEMLEAVE};
        int iLeave;
    };


    class SOUI_EXP EventLBSelChanging : public TplEventArgs<EventLBSelChanging>
    {
        SOUI_CLASS_NAME_DECL(EventLBSelChanging,L"on_listbox_sel_changing")
    public:
		EventLBSelChanging(SObject *pSender);
        enum{EventID=EVT_LB_SELCHANGING};

        int nNewSel;
        int nOldSel;
        BOOL bCancel;
    };

    class SOUI_EXP EventLBSelChanged : public TplEventArgs<EventLBSelChanged>
    {
        SOUI_CLASS_NAME_DECL(EventLBSelChanged,L"on_listbox_sel_changed")
    public:
		EventLBSelChanged(SObject *pSender);
        enum{EventID=EVT_LB_SELCHANGED};
        int nNewSel;
        int nOldSel;
    };

	class SOUI_EXP EventLBDbClick : public TplEventArgs<EventLBDbClick>
	{
		SOUI_CLASS_NAME_DECL(EventLBDbClick, L"on_listbox_item_dbclick")
	public:
		EventLBDbClick(SObject *pSender);
		enum { EventID = EVT_LB_DBCLICK };
		int nCurSel;
	};

    class SOUI_EXP EventLVSelChanged : public TplEventArgs<EventLVSelChanged>
    {
        SOUI_CLASS_NAME_DECL(EventLVSelChanged,L"on_listview_select_changed")
    public:
		EventLVSelChanged(SObject *pSender);
        enum{EventID=EVT_LV_SELCHANGED};

        int iOldSel;    //原选中项
        int iNewSel;    //新选中项
    };
    
    class SOUI_EXP EventLVSelChanging : public TplEventArgs<EventLVSelChanging>
    {
        SOUI_CLASS_NAME_DECL(EventLVSelChanging,L"on_listview_select_changing")
    public:
		EventLVSelChanging(SObject *pSender);
        enum{EventID=EVT_LV_SELCHANGING};

        int iOldSel;    //原选中项
        int iNewSel;    //新选中项

        BOOL bCancel;
    };
    
	class SOUI_EXP EventTVSelChanged : public TplEventArgs<EventTVSelChanged>
	{
		SOUI_CLASS_NAME_DECL(EventTVSelChanged,L"on_treeview_select_changed")
	public:
		EventTVSelChanged(SObject *pSender);
		enum{EventID=EVT_TV_SELCHANGED};

		ULONG_PTR hOldSel;    //原选中项
		ULONG_PTR hNewSel;    //新选中项
	};

    class SOUI_EXP EventTVSelChanging : public TplEventArgs<EventTVSelChanging>
    {
        SOUI_CLASS_NAME_DECL(EventTVSelChanging,L"on_treeview_select_changing")
    public:
		EventTVSelChanging(SObject *pSender);
        enum{EventID=EVT_TV_SELCHANGING};
        ULONG_PTR hOldSel;    //原选中项
        ULONG_PTR hNewSel;    //新选中项

        BOOL bCancel;
    };


    class SOUI_EXP EventRENotify : public TplEventArgs<EventRENotify>
    {
        SOUI_CLASS_NAME_DECL(EventRENotify,L"on_richedit_notify")
    public:
		EventRENotify(SObject *pSender);
        enum{EventID=EVT_RE_NOTIFY};
        DWORD iNotify;
        LPVOID pv;
        HRESULT hr;
    };

    class SOUI_EXP EventREMenu : public TplEventArgs<EventREMenu>
    {
        SOUI_CLASS_NAME_DECL(EventREMenu,L"on_richedit_menu")
    public:
		EventREMenu(SObject *pSender);
        enum{EventID=EVT_RE_MENU};
        UINT uCmd;
    };
    
    class SOUI_EXP EventSliderPos : public TplEventArgs<EventSliderPos>
    {
        SOUI_CLASS_NAME_DECL(EventSliderPos,L"on_slider_pos")
    public:
		EventSliderPos(SObject *pSender);
        enum{EventID=EVT_SLIDER_POS};
        int     nPos;
    };

    //点击表头
    class SOUI_EXP EventHeaderClick : public TplEventArgs<EventHeaderClick>
    {
        SOUI_CLASS_NAME_DECL(EventHeaderClick,L"on_header_click")
    public:
		EventHeaderClick(SObject *pSender);
        enum{EventID=EVT_HEADER_CLICK};
        int   iItem;
    };

    //表头宽度改变中
    class SOUI_EXP EventHeaderItemChanging : public TplEventArgs<EventHeaderItemChanging>
    {
        SOUI_CLASS_NAME_DECL(EventHeaderItemChanging,L"on_header_item_changing")
    public:
		EventHeaderItemChanging(SObject *pSender);
        enum{EventID=EVT_HEADER_ITEMCHANGING};
        int   iItem;
        int   nWidth;
    };

    //表头宽度改变
    class SOUI_EXP EventHeaderItemChanged : public TplEventArgs<EventHeaderItemChanged>
    {
        SOUI_CLASS_NAME_DECL(EventHeaderItemChanged,L"on_header_item_changed")
    public:
		EventHeaderItemChanged(SObject *pSender);
        enum{EventID=EVT_HEADER_ITEMCHANGED};
        int   iItem;
        int   nWidth;
    };

    //拖动表项调整位置
    class SOUI_EXP EventHeaderItemSwap : public TplEventArgs<EventHeaderItemSwap>
    {
        SOUI_CLASS_NAME_DECL(EventHeaderItemSwap,L"on_header_item_swap")
    public:
		EventHeaderItemSwap(SObject *pSender);
        enum{EventID=EVT_HEADER_ITEMSWAP};
        int   iOldIndex;
        int      iNewIndex;
    };

    class SOUI_EXP EventCBSelChange : public TplEventArgs<EventCBSelChange>
    {
        SOUI_CLASS_NAME_DECL(EventCBSelChange,L"on_combobox_sel_change")
    public:
		EventCBSelChange(SObject *pSender, int _nCurSel);
        enum{EventID=EVT_CB_SELCHANGE};
        int nCurSel;
    };

	class SDropDownWnd;
	class SOUI_EXP EventCBDropdown :public TplEventArgs<EventCBDropdown>
	{
		SOUI_CLASS_NAME_DECL(EventCBDropdown, L"on_combobox_dropdown")
	public:
		EventCBDropdown(SObject *pSender);
		enum { EventID = EVT_CB_DROPDOWN };

		SDropDownWnd *pDropDown;
	};


    class SOUI_EXP EventLCSelChanging : public TplEventArgs<EventLCSelChanging>
    {
        SOUI_CLASS_NAME_DECL(EventLCSelChanging,L"on_listctrl_sel_changing")
    public:
		EventLCSelChanging(SObject *pSender);
        enum{EventID=EVT_LC_SELCHANGING};
        int nNewSel;
        int nOldSel;
        BOOL bCancel;
    };

    class SOUI_EXP EventLCSelChanged : public TplEventArgs<EventLCSelChanged>
    {
        SOUI_CLASS_NAME_DECL(EventLCSelChanged,L"on_listctrl_sel_changed")
    public:
		EventLCSelChanged(SObject *pSender);
        enum{EventID=EVT_LC_SELCHANGED};
        int nNewSel;
        int nOldSel;
    };

	class SOUI_EXP EventLCDbClick : public TplEventArgs<EventLCDbClick>
	{
		SOUI_CLASS_NAME_DECL(EventLCDbClick, L"on_listctrl_item_dbclick")
	public:
		EventLCDbClick(SObject *pSender);
		enum { EventID = EVT_LC_DBCLICK };
		int nCurSel;
	};

	class SOUI_EXP EventLCItemDeleted : public TplEventArgs<EventLCItemDeleted>
	{
        SOUI_CLASS_NAME_DECL(EventLCItemDeleted,L"on_listctrl_del_item")
	public:
		EventLCItemDeleted(SObject *pSender);
		enum{ EventID = EVT_LC_ITEMDELETED };
		int		nItem;
		DWORD   dwData;
	};

    class SOUI_EXP EventCalendarSelDay : public TplEventArgs<EventCalendarSelDay>
    {
        SOUI_CLASS_NAME_DECL(EventCalendarSelDay,L"on_calendar_sel_day")
    public:
		EventCalendarSelDay(SObject *pSender);
        enum{EventID=EVT_CALENDAR_SELDAY};
        WORD   wOldDay;
        WORD   wNewDay;
    };
    
    
    class SOUI_EXP EventCalendarSetDate : public TplEventArgs<EventCalendarSetDate>
    {
        SOUI_CLASS_NAME_DECL(EventCalendarSetDate,L"on_calendar_set_date")
    public:
		EventCalendarSetDate(SObject *pSender);
        enum{EventID=EVT_CALENDAR_SETDATE};
    };

	class SOUI_EXP EventCalendarExChanged : public TplEventArgs<EventCalendarExChanged>
	{
		SOUI_CLASS_NAME_DECL(EventCalendarExChanged, L"on_calendarex_changed")
	public:
		EventCalendarExChanged(SObject *pSender);
		enum { EventID = EVT_CALENDAREX_CHANGED };
		WORD iNewDay;
		WORD iNewMonth;
		WORD iNewYear;
		int nBtnType;			// 按钮类型
	};

	class SOUI_EXP EventDateTimeChanged : public TplEventArgs<EventDateTimeChanged>
	{
		SOUI_CLASS_NAME_DECL(EventDateTimeChanged, L"on_datetime_changed")
	public:
		EventDateTimeChanged(SObject *pSender);
		enum { EventID = EVT_DATETIME_CHANGED };
		SYSTEMTIME newTime;
	};

    class SOUI_EXP EventTCSelChanging : public TplEventArgs<EventTCSelChanging>
    {
        SOUI_CLASS_NAME_DECL(EventTCSelChanging,L"on_treectrl_sel_changing")
    public:
		EventTCSelChanging(SObject *pSender);
        enum{EventID=EVT_TC_SELCHANGING};
        HSTREEITEM hOldSel;
        HSTREEITEM hNewSel;
        BOOL bCancel;
    };

    class SOUI_EXP EventTCSelChanged : public TplEventArgs<EventTCSelChanged>
    {
        SOUI_CLASS_NAME_DECL(EventTCSelChanged,L"on_treectrl_sel_changed")
    public:
		EventTCSelChanged(SObject *pSender);
        enum{EventID=EVT_TC_SELCHANGED};
        HSTREEITEM hOldSel;
        HSTREEITEM hNewSel;
    };

    class SOUI_EXP EventTCCheckState : public TplEventArgs<EventTCCheckState>
    {
        SOUI_CLASS_NAME_DECL(EventTCCheckState,L"on_treectrl_item_check")
    public:
		EventTCCheckState(SObject *pSender);
        enum{EventID=EVT_TC_CHECKSTATE};
        HSTREEITEM  hItem;
        UINT        uCheckState;
    };

    class SOUI_EXP EventTCExpand : public TplEventArgs<EventTCExpand>
    {
        SOUI_CLASS_NAME_DECL(EventTCExpand,L"on_treectrl_item_expand")
    public:
		EventTCExpand(SObject *pSender);
        enum{EventID=EVT_TC_EXPAND};
        HSTREEITEM  hItem;
        BOOL bCollapsed;
    };

    //双击treectrl的叶子节点 add by zhaosheng
    class SOUI_EXP EventTCDbClick : public TplEventArgs<EventTCDbClick>
    {
        SOUI_CLASS_NAME_DECL(EventTCDbClick,L"on_treectrl_item_dbclick")
    public:
		EventTCDbClick(SObject *pSender);
        enum{EventID=EVT_TC_DBCLICK};
        HSTREEITEM hItem;  //双击选中的节点
        BOOL bCancel;
    };

    class SOUI_EXP EventSpinValue2String : public TplEventArgs<EventSpinValue2String>
    {
        SOUI_CLASS_NAME_DECL(EventSpinValue2String,L"on_spin_value2string")
    public:
		EventSpinValue2String(SObject *pSender);
		~EventSpinValue2String() override;
        enum{EventID=EVT_SPIN_VALUE2STRING};
        
        int      nValue;
        SStringT strValue;
    };

    class SOUI_EXP EventSplitPaneMoved : public TplEventArgs<EventSplitPaneMoved>
    {
        SOUI_CLASS_NAME_DECL(EventSplitPaneMoved,L"on_split_pane_moved")
    public:
		EventSplitPaneMoved(SObject *pSender);
        enum{EventID=EVT_SPLIT_PANE_MOVED};
        
        CRect rcPane;
    };
    
	class SOUI_EXP EventAnimateStart: public TplEventArgs<EventAnimateStart>
	{
		SOUI_CLASS_NAME_DECL(EventAnimateStart,L"on_animate_start")
	public:
		EventAnimateStart(SObject *pSender);
		enum{EventID=EVT_ANI_START};
	};

	class SOUI_EXP EventAnimateStop: public TplEventArgs<EventAnimateStop>
	{
		SOUI_CLASS_NAME_DECL(EventAnimateStop,L"on_animate_stop")
	public:
		EventAnimateStop(SObject *pSender);
		enum{EventID=EVT_ANI_STOP};
	};

	class SMenuEx;
	class SOUI_EXP EventSelectMenu : public TplEventArgs<EventSelectMenu>
	{
		SOUI_CLASS_NAME_DECL(EventSelectMenu, L"on_select_menu")
	public:
		EventSelectMenu(SObject *pSender);
		enum { EventID = EVT_SELECTMENU };

		UINT m_id;
		SMenuEx *m_pMenu;
	};

    class SOUI_EXP EventSetHotKey :public TplEventArgs<EventSetHotKey> {
        SOUI_CLASS_NAME_DECL(EventSetHotKey, L"on_hot_key_set_event")
    public:
		EventSetHotKey(SObject *pSender);
        enum {
            EventID = EVT_HOT_KEY_SET,
        };
        WORD vKey;
        WORD wModifiers;
    };

} // End of  CEGUI namespace section