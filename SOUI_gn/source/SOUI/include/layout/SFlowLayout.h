#pragma once
#include "core/SWnd.h"


namespace SOUI
{  
	/// <summary>
	///		��ʽ���ֻ���
	/// </summary>
	class SOUI_EXP SFlowLayout : public SWindow
	{
		SOUI_CLASS_NAME(SFlowLayout, L"FlowLayout")
	public:
		SFlowLayout();
		bool DV_IsStateChangeRedraw(){return false;}

	public:
		SOUI_ATTRS_BEGIN()
			ATTR_INT(L"childpaddlen", m_iChildPaddLen, 0)
		SOUI_ATTRS_END()
	public:
		int                            m_iChildPaddLen;		///<�ӿؼ�֮��Ķ������	
	};

	/// <summary>
	///		��ֱ��ʽ����
	/// </summary>
	class SOUI_EXP SVBox : public SFlowLayout
	{
		SOUI_CLASS_NAME(SVBox, L"VBox")
	public:
		void UpdateChildrenPosition();
	};

	/// <summary>
	///		ˮƽ��ʽ����
	/// </summary>
	class SOUI_EXP SHBox:public SFlowLayout
	{
		SOUI_CLASS_NAME(SHBox, L"VBox")
	public:
		void UpdateChildrenPosition();
	};

	/// <summary>
	///		tab��ʽ���֣���С��ͬ
	/// </summary>
	class SOUI_EXP STableLayout : public SFlowLayout
	{
		SOUI_CLASS_NAME(STableLayout, L"TableLayout") 
	public:
		void UpdateChildrenPosition();

	public:
		SOUI_ATTRS_BEGIN()
			ATTR_INT(L"itemsize", m_szItem, 3)
		SOUI_ATTRS_END() 
	public:
		CSize                          m_szItem;
	};

	/// <summary>
	///		Warp��ʽ����
	/// </summary>
	class SOUI_EXP SWrapLayout : public STableLayout
	{
		SOUI_CLASS_NAME(STableLayout, L"WrapLayout")
	public:
		void UpdateChildrenPosition();

	public:
		SOUI_ATTRS_BEGIN()
			ATTR_INT(L"colcount", m_nColumns, 0) // ��û������sizeʱ�����������Զ����ÿ��
		SOUI_ATTRS_END()
	public:
		int                            m_nColumns;
	};

	// <summary>
	///		 SSplitLayout�ָ�ֵ�����ʵ�֣����ԣ�<see cref="DMAttr::DUISplitterAttr"/>
	/// ��:���������ڣ�A,B������,�������ⲿ����size�仯ʱ��ҪôA���ڱ仯��ҪôB���ڱ仯��2ѡһ
	/// </summary>
	class SSplitLayout:public SFlowLayout
	{
		SOUI_CLASS_NAME(SSplitLayout, L"splitlayout")
	public:
		SSplitLayout();
	public: 
		SOUI_MSG_MAP_BEGIN()
			MSG_WM_PAINT_EX(OnPaint)
			MSG_WM_LBUTTONDBLCLK(OnLButtonDown) //��˫����Ϣ����Ϊ����
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_MOUSEMOVE(OnMouseMove)
		SOUI_MSG_MAP_END() 
	public:
		void OnPaint(IRenderTarget *pRT);
		void OnLButtonDown(UINT nFlags, CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);

	public: 
		virtual BOOL CreateChildren(pugi::xml_node xmlNode); 
		void UpdateChildrenPosition();
		virtual BOOL OnSetCursor(const CPoint &pt);

	public:// ����      
		BOOL UpdateFixWidth();
		void UpdateDrag(int iDiff);
		CRect GetSliderRect();
	protected:
		HRESULT OnAttrVert(const SStringW& strValue, BOOL bLoading);
		HRESULT OnAttrFirstchange(const SStringW& strValue, BOOL bLoading);
		HRESULT OnAttrSliderSkin(const SStringW& strValue, BOOL bLoading);
		
	public: 
		SOUI_ATTRS_BEGIN()
			ATTR_CUSTOM(L"bvert", OnAttrVert)
			ATTR_CUSTOM(L"bfirstchange", OnAttrFirstchange)
			ATTR_INT(L"sliderwidth", m_iSliderWid, TRUE)
			ATTR_INT(L"firstchildwidth", m_iFirstChildWidth, TRUE)
			ATTR_CUSTOM(L"sliderskin", OnAttrSliderSkin)
			ATTR_COLOR(L"clrslider", m_ClrSlider, FALSE)
		SOUI_ATTRS_END() 
	public:
		bool                             m_bVert;                ///< �Ƿ���ֱ
		int                              m_iSliderWid;           ///< �м���϶���Ŀ��(ˮƽ)��߶�(��ֱ)
		ISkinObj                         *m_pSliderSkin;
		COLORREF                         m_ClrSlider;
		bool                             m_bFirstChange;
		int                              m_iFirstChildWidth;     ///< ��һ�����ڵĳ�ʼֵ
		int                              m_iFixWid;              ///< �̶��Ŀ�ȣ�Ϊ0��ʾδ��ʼ��,�����ʾsize�仯ʱ���̶�size���Ǹ�����
		bool                             m_bDrag;
		CPoint                           m_ptDrag;
		SStringT                         m_strPos;
	};
}
//namespace SOUI