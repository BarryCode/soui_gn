#pragma once
#include "RichEditObj.h"
#include "core\SwndContainerImpl.h"
#include <atlcomcli.h>
#include <oleidl.h>
#include <objidl.h>
#include <richole.h>

//////////////////////////////////////////////////////////////////////////
//
class OleTimerHandler : public SOUI::ITimelineHandler
{
public:

	BOOL RegisterHandler(SOUI::ITimelineHandler *pHandler)
    {
		SOUI::SPOSITION pos = m_lstHandler.Find(pHandler);
        if(pos) return FALSE;
        m_lstHandler.AddTail(pHandler);
        return TRUE;
    }

	BOOL UnregisterHandler(SOUI::ITimelineHandler *pHandler)
    {
		SOUI::SPOSITION pos = m_lstHandler.Find(pHandler);
        if(!pos) return FALSE;
        m_lstHandler.RemoveAt(pos);
        return TRUE;
    }

    void OnNextFrame()
    {
        /*
         * ����д����Ķ�ʱ��ע������,ÿ�ζ�Ҫ����,�ܷ�CPU,�������������ȥ��
         * ע����OnNextFrame���治��ע���ȡ����ʱ��,����ѭ�������
        */
        //SList<ITimelineHandler*> lstCopy;
        //CopyList(m_lstHandler,lstCopy);

		SOUI::SPOSITION pos = m_lstHandler.GetHeadPosition();
        while(pos)
        {
			SOUI::ITimelineHandler * pHandler = m_lstHandler.GetNext(pos);
            pHandler->OnNextFrame();
        }
    }

protected:
	SOUI::SList<SOUI::ITimelineHandler*>    m_lstHandler;
};

//////////////////////////////////////////////////////////////////////////
// OleWindow 
//
class OleWindow : public SOUI::SwndContainerImpl
{
public:
    OleWindow();
    virtual ~OleWindow();

    //
    // ISwndContainer methods
    //
	BOOL            OnFireEvent(SOUI::EventArgs &evt);
	SOUI::CRect           GetContainerRect();
    HWND            GetHostHwnd();
	const SOUI::SStringW& GetTranslatorContext();
	SOUI::IRenderTarget*  OnGetRenderTarget(const SOUI::CRect & rc, DWORD gdcFlags);
	void            OnReleaseRenderTarget(SOUI::IRenderTarget * pRT, const SOUI::CRect &rc, DWORD gdcFlags);
	void            OnRedraw(const SOUI::CRect &rc);
    BOOL            IsTranslucent() const;
    BOOL            IsSendWheel2Hover() const;
    BOOL            OnCreateCaret(SWND swnd,HBITMAP hBmp,int nWidth,int nHeight) {return FALSE;}
    BOOL            OnShowCaret(BOOL bShow) {return FALSE;}
    BOOL            OnSetCaretPos(int x,int y) {return FALSE;}
    BOOL            UpdateWindow();
	void            UpdateTooltip();
	BOOL            RegisterTimelineHandler(SOUI::ITimelineHandler *pHandler);
	BOOL            UnregisterTimelineHandler(SOUI::ITimelineHandler *pHandler);
	SOUI::SMessageLoop *  GetMsgLoop();
	SOUI::IScriptModule * GetScriptModule() { return NULL; }

    //
    // internal public methods
    //
    void        SetHostRichEdit(IRichEditObjHost *);
    LRESULT     HandleEvent(UINT msg, WPARAM wParam, LPARAM lParam);
	void        SetOleWindowRect(SOUI::CRect rcWindow)    { m_rcOleWindow = rcWindow; }
    void        SetDelayDraw(BOOL bDelayDraw)       { m_bDelayDraw  = bDelayDraw; }

protected:
	void        RealDraw(SOUI::CRect rc);

private:
	SOUI::ISwndContainer*         m_pHostContainer;
    IRichEditObjHost *      m_pHostRichEdit;
    int                     m_nWindowID;   

	SOUI::CRect                   m_rcOleWindow;      // ole��richedit�����λ��   
    BOOL                    m_bDelayDraw;       // ����GIF����Ҫ�����ӳ�ˢ��

    static OleTimerHandler  m_timerHandler;     // ��ʱ��
    static BOOL             m_bTiemrRegistered;
};

//////////////////////////////////////////////////////////////////////////
// RichEditOleBase 
//
class RichEditOleBase : public RichEditObj
    ,public IOleObject
    ,public IViewObject2
{
public:
    RichEditOleBase();
    virtual ~RichEditOleBase();

    //
    // IUnknown meghods
    //
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppvObject);
    ULONG   STDMETHODCALLTYPE AddRef(void);
    ULONG   STDMETHODCALLTYPE Release(void);

    //
    // IOleObject methods
    //
    HRESULT STDMETHODCALLTYPE SetHostNames(LPCOLESTR, LPCOLESTR)        { return S_OK; }
    HRESULT STDMETHODCALLTYPE Close(DWORD dwSaveOption)                 { return S_OK; }
    HRESULT STDMETHODCALLTYPE SetMoniker(DWORD dwWhich, IMoniker *pmk)  { return E_NOTIMPL;	}
    HRESULT STDMETHODCALLTYPE GetMoniker(DWORD, DWORD, IMoniker **ppmk) { return E_NOTIMPL;	}
    HRESULT STDMETHODCALLTYPE InitFromData(IDataObject *, BOOL, DWORD)  { return E_NOTIMPL;	}
    HRESULT STDMETHODCALLTYPE GetClipboardData(DWORD, IDataObject **)   { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE DoVerb(LONG iVerb, LPMSG lpmsg, IOleClientSite *pActiveSite,LONG lindex, HWND hwndParent, LPCRECT lprcPosRect)
                                                                        { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE EnumVerbs(IEnumOLEVERB **ppEnumOleVerb)   { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE Update( void)                             { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE IsUpToDate( void)                         { return E_NOTIMPL;	}
    HRESULT STDMETHODCALLTYPE GetUserType(DWORD, LPOLESTR *pszUserType) { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE EnumAdvise(IEnumSTATDATA **ppenumAdvise)  { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE GetMiscStatus(DWORD , DWORD *pdwStatus)   { return E_NOTIMPL;	}
    HRESULT STDMETHODCALLTYPE SetColorScheme(LOGPALETTE *pLogpal)       { return E_NOTIMPL;	}

    //���½ӿ���Ҫʵ��
    HRESULT STDMETHODCALLTYPE GetUserClassID(CLSID *pClsid);
    HRESULT STDMETHODCALLTYPE Advise(IAdviseSink *pAdvSink,DWORD *pdwConnection);
    HRESULT STDMETHODCALLTYPE Unadvise(DWORD dwConnection);
    HRESULT STDMETHODCALLTYPE SetClientSite(IOleClientSite *pClientSite);
    HRESULT STDMETHODCALLTYPE GetClientSite(IOleClientSite **ppClientSite);
    HRESULT STDMETHODCALLTYPE GetExtent(DWORD dwDrawAspect, SIZEL *psizel);
    HRESULT STDMETHODCALLTYPE SetExtent(DWORD dwDrawAspect, SIZEL *psizel) ;

    //
    // IViewObject2 methods
    //
    HRESULT STDMETHODCALLTYPE GetColorSet( DWORD, LONG, void *, DVTARGETDEVICE *,  HDC, LOGPALETTE **)    
                                                                            { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE Freeze( DWORD , LONG , void *, DWORD *)       { return S_OK; }
    HRESULT STDMETHODCALLTYPE Unfreeze(DWORD dwFreeze)                      { return S_OK; }

    // ���½ӿ���Ҫʵ��
    HRESULT STDMETHODCALLTYPE SetAdvise( DWORD aspects, DWORD advf, IAdviseSink *pAdvSink);
    HRESULT STDMETHODCALLTYPE GetAdvise(DWORD *pAspects, DWORD *pAdvf, IAdviseSink **ppAdvSink);
    HRESULT STDMETHODCALLTYPE Draw(
        DWORD dwDrawAspect, LONG lindex,  
        void *pvAspect, 
        DVTARGETDEVICE *ptd, 
        HDC hdcTargetDev,
        HDC hdcDraw, 
        LPCRECTL lprcBounds,
        LPCRECTL lprcWBounds,
        BOOL ( STDMETHODCALLTYPE *pfnContinue )(ULONG_PTR dwContinue), 
        ULONG_PTR dwContinue);

    HRESULT STDMETHODCALLTYPE GetExtent(
        DWORD dwDrawAspect, 
        LONG lindex, 
        DVTARGETDEVICE *ptd, 
        LPSIZEL lpsizel);

    //
    // RichEditObj methods
    //
    void            SetDirty(BOOL bDirty);
    LRESULT         ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    BOOL            InsertIntoHost(IRichEditObjHost * pHost);
	BOOL            OnUpdateToolTip(SOUI::CPoint pt, SOUI::SwndToolTipInfo &tipInfo);
    BOOL            NeedToProcessMessage() {return TRUE; }

    // internal  methods
    virtual BOOL    InitOleWindow(IRichEditObjHost* pHost);
    virtual void    PreInsertObject(REOBJECT& object){}
    void            CalculateExtentSize(const SIZE& sizeNature);

protected:
    HRESULT         InsertOleObject(IRichEditObjHost*);
    void            InvertBorder(HDC hdc, LPRECT lprcBorder);

    SOUI_ATTRS_BEGIN()
        ATTR_INT(L"selectable", m_bCanSelect, FALSE)
    SOUI_ATTRS_END()

protected:
    //ULONG                       m_dwRef;
    CComPtr<IOleClientSite>     m_spClientSite;
    CComPtr<IOleAdviseHolder>   m_spOleAdviseHolder;
    CComPtr<IAdviseSink>        m_spAdviseSink;
    SIZEL                       m_sizeExtent;
    OleWindow                   m_oleWindow;
    SIZE                        m_sizeNatural;
    GUID                        m_guidOle;
	SOUI::SStringW                    m_strXmlLayout;
    BOOL                        m_bCanSelect;
};
