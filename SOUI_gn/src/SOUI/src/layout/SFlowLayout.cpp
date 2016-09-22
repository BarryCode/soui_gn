#include "souistd.h"
#include "layout/SFlowLayout.h"
#include "core/SwndLayoutBuilder.h"

namespace SOUI
{
	SFlowLayout::SFlowLayout()
	{
		m_iChildPaddLen = 0;
	}
	///--------------------------------SVBox----------------
	void SVBox::UpdateChildrenPosition()
	{
		do 
		{
			// 1.����ӿؼ��Ĳ��ֿռ�
			CRect rcLayout =GetChildrenLayoutRect();
			if (rcLayout.IsRectEmpty())
			{
				break;
			}
			// 2.�ж������ӿؼ��ǲ���Ϊ��
			if (0==GetChildrenCount())
			{
				break;
			}

			// 3.�����������õĴ�С
			SIZE szAvailable = rcLayout.Size();

			// 4.������С�Ĵ�С 
			int nAdjustNum	= 0;   //  û�����ø߶ȵ����Զ�����߶ȵĸ���
			int nCyFixedLen = 0;   //  �ܹ�ֵ�߶�
			int nEstimateNum = 0;  //  ��ֵ�߶ȵĸ���,������nAdjustNum

			CRect rc;
			SWindow *pChild = GetWindow(GSW_FIRSTCHILD);
			while (pChild)
			{
				if (!pChild->IsVisible(FALSE))
				{
					pChild = pChild->GetWindow(GSW_NEXTSIBLING);
					continue;
				}

				// 5. ���Ԥ���fixsize����������ֱ���֣�һ������ù̶��ĸ߶ȣ���Ӧxml��heightֵ
				CSize szMeasure(POS_INIT, POS_INIT);
				CRect rc =pChild->GetWindowRect();// �������˸߶ȣ����Զ���ֵ
				
				/*
				CRect &rcPadd = pChild->m_pDUIXmlInfo->m_rcPadd;
				if (POS_INIT == szMeasure.cy)
				{
					nAdjustNum++;// û�����ø߶ȵ����Զ�����߶ȵĸ���
					nCyFixedLen += rcPadd.top + rcPadd.bottom;
				}
				else
				{
					int nMinHeight = pChild->m_pDUIXmlInfo->m_nMinHeight;
					int nMaxHeight = pChild->m_pDUIXmlInfo->m_nMaxHeight;

					if (nMinHeight != -1
						&& szMeasure.cy<nMinHeight)
					{
						szMeasure.cy = nMinHeight;
					}
					if (nMaxHeight != -1
						&& szMeasure.cy>nMaxHeight)
					{
						szMeasure.cy = nMaxHeight;
					}

					nCyFixedLen += szMeasure.cy + rcPadd.top + rcPadd.bottom;
				}
				*/
				nEstimateNum++; 
				pChild = pChild->GetWindow(GSW_NEXTSIBLING);
			}

			// 8.�ӿؼ�֮��Ķ������
			nCyFixedLen += (nEstimateNum-1)*m_iChildPaddLen;

			// ��ʼ�ڷ��ӿؼ�
			int ncyAdjustItemLen	= 0;
			if (nAdjustNum>0)
			{
				// 9.�ж��Զ�����߶ȹ�����
				ncyAdjustItemLen = (szAvailable.cy>nCyFixedLen)?((szAvailable.cy-nCyFixedLen)/nAdjustNum):0;
			}

			SIZE szRemain = szAvailable;
			int iAdjustIndex = 0;
			int nCyFixedLenRemain = nCyFixedLen;
			int iPosY = rcLayout.top;
			int iPosX = rcLayout.left;

			pChild =  GetWindow(GSW_FIRSTCHILD);
			while (pChild)
			{
				if (!pChild->IsVisible(FALSE))
				{
					pChild=pChild->GetWindow(GSW_NEXTSIBLING);
					continue;
				}

				 
				int nMinHeight = -1;// pChild->m_pDUIXmlInfo->m_nMinHeight;
				int nMaxHeight = -1;//pChild->m_pDUIXmlInfo->m_nMaxHeight;
				int nMinWidth = -1;//pChild->m_pDUIXmlInfo->m_nMinWidth;
				int nMaxWidth = -1;//pChild->m_pDUIXmlInfo->m_nMaxWidth;
				CRect rcPadd = {0,0,0,0};// pChild->m_pDUIXmlInfo->m_rcPadd;
				szRemain.cy -= rcPadd.top;// �����ȼ�ȥtop

				// ����y
				CSize szMeasure(POS_INIT,POS_INIT);//�������ֵ
				szMeasure  =pChild->GetDesiredSize(NULL);
				if (POS_INIT == szMeasure.cy)// û�����ø߶ȵĿؼ�
				{
					iAdjustIndex++;
					szMeasure.cy = ncyAdjustItemLen;

					if (iAdjustIndex == nAdjustNum)
					{// ���һ��û�����ø߶ȵĿؼ�����Ϊǰ������������С����ʹ��cy!=ncyAdjustItemLen
					 // +rcPadd.top����Ϊǰ�����rcPadd.top,��nCyFixedLenRemain�а�����rcPadd.top+rcPadd.bottm
					 // ��ֻ�����һ��û���ø߶ȵĿؼ�����һ�������Ŀؼ�������Ҫ-nCyFixedLenRemain
						szMeasure.cy = szRemain.cy+rcPadd.top-nCyFixedLenRemain;
						szMeasure.cy = szMeasure.cy<=0?0:szMeasure.cy;
					}
					if (nMinHeight!=-1
						&&szMeasure.cy<nMinHeight)
					{
						szMeasure.cy = nMinHeight;
					}
					if (nMaxHeight!=-1
						&&szMeasure.cy>nMaxHeight)
					{
						szMeasure.cy = nMaxHeight;
					}
				}
				else
				{
					if (nMinHeight!=-1
						&&szMeasure.cy<nMinHeight)
					{
						szMeasure.cy = nMinHeight;
					}
					if (nMaxHeight!=-1
						&&szMeasure.cy>nMaxHeight)
					{
						szMeasure.cy = nMaxHeight;
					}
					nCyFixedLenRemain -= szMeasure.cy;
				}

				// ����x
				szMeasure.cx = pChild->GetWindowRect().Width();
				if (-1 == szMeasure.cx)
				{// ��ʼĬ��ֵ-1
					szMeasure.cx = szAvailable.cx-rcPadd.left-rcPadd.right;
				}
				else if (szMeasure.cx<0)
				{
					szMeasure.cx = 0;
				}
				if (nMinWidth!=-1
					&&szMeasure.cx<nMinWidth)
				{
					szMeasure.cx = nMinWidth;
				}
				if (nMaxWidth!=-1
					&&szMeasure.cx>nMaxWidth)
				{
					szMeasure.cx = nMaxWidth;
				}

				//CRect &rcWindow  = rcLayout;// ������С
				CRect rcArrange(iPosX+rcPadd.left,iPosY+rcPadd.top,iPosX+rcPadd.left+szMeasure.cx, iPosY+rcPadd.top+szMeasure.cy);
				pChild->Move(&rcArrange);

				pChild=pChild->GetWindow(GSW_NEXTSIBLING);
				iPosY += szMeasure.cy + (NULL == pChild?0:m_iChildPaddLen) + rcPadd.top+rcPadd.bottom;
				szRemain.cy -= szMeasure.cy + (NULL == pChild?0:m_iChildPaddLen) + rcPadd.bottom;// ��ǰ���ȥtop��
				nCyFixedLenRemain -= (NULL == pChild?0:m_iChildPaddLen) + rcPadd.top+rcPadd.bottom;
			 
			}
		} while (false); 
	}

	///--------------------------------SHBox----------------
	void SHBox::UpdateChildrenPosition()
	{
		do 
		{
			// 1.����ӿؼ��Ĳ��ֿռ�
			CRect rcLayout = GetChildrenLayoutRect();
			if (rcLayout.IsRectEmpty())
			{
				break;
			}

			// 3.�����������õĴ�С
			SIZE szAvailable = rcLayout.Size();

			// 4.������С�Ĵ�С
			int nAdjustNum	= 0;   //  û�����ÿ�ȵ����Զ������ȵĸ���
			int nCxFixedLen = 0;   //  �ܹ�ֵ���
			int nEstimateNum = 0;  //  ��ֵ��ȵĸ���,������nAdjustNum

			SWindow *pChild = GetWindow(GSW_FIRSTCHILD);
			while (pChild)
			{
				if (!pChild->IsVisible(FALSE))
				{
					pChild=pChild->GetWindow(GSW_NEXTSIBLING);
					continue;
				}

			
				// 5. ���Ԥ���fixsize��������ˮƽ���֣�һ������ù̶��Ŀ�ȣ���Ӧxml��widthֵ
				CSize szMeasure(POS_INIT,POS_INIT); //�������������ֵ
				szMeasure = pChild->GetDesiredSize(NULL);
				CRect rcPadd = {0,0,0,0};// pChild->m_pDUIXmlInfo->m_rcPadd;
				if (POS_INIT == szMeasure.cx)
				{
					nCxFixedLen += rcPadd.left+rcPadd.right;
					nAdjustNum++;// û�����ÿ�ȵ����Զ������ȵĸ���
				}
				else
				{
					int nMinWidth = -1;// pChild->m_pDUIXmlInfo->m_nMinWidth;
					int nMaxWidth = -1;// pChild->m_pDUIXmlInfo->m_nMaxWidth;
					if (nMinWidth!=-1
						&&szMeasure.cx<nMinWidth)
					{
						szMeasure.cx = nMinWidth;
					}
					if (nMaxWidth!=-1
						&&szMeasure.cx>nMaxWidth)
					{
						szMeasure.cx = nMaxWidth;
					}
					nCxFixedLen += szMeasure.cx+rcPadd.left+rcPadd.right;
				}
				
				nEstimateNum++;
				pChild = pChild->GetWindow(GSW_NEXTSIBLING);
			}

			// 8.�ӿؼ�֮��Ķ������
			nCxFixedLen += (nEstimateNum-1)*m_iChildPaddLen;

			// ��ʼ�ڷ��ӿؼ�
			int ncxAdjustItemLen	= 0;
			if (nAdjustNum>0)
			{
				// 9.�ж��Զ������ȹ�����
				ncxAdjustItemLen = (szAvailable.cx>nCxFixedLen)?((szAvailable.cx-nCxFixedLen)/nAdjustNum):0;
			}

			SIZE szRemain = szAvailable;
			int iAdjustIndex = 0;
			int nCxFixedLenRemain = nCxFixedLen;
			int iPosX = rcLayout.left;

			pChild = GetWindow(GSW_FIRSTCHILD);
			while (pChild)
			{
				if (!pChild->IsVisible(FALSE))
				{
					pChild=pChild->GetWindow(GSW_NEXTSIBLING);
					continue;
				}

				int nMinHeight = -1;// pChild->m_pDUIXmlInfo->m_nMinHeight;
				int nMaxHeight = -1;//pChild->m_pDUIXmlInfo->m_nMaxHeight;
				int nMinWidth = -1;//pChild->m_pDUIXmlInfo->m_nMinWidth;
				int nMaxWidth = -1;//pChild->m_pDUIXmlInfo->m_nMaxWidth;
				CRect rcPadd = {0,0,0,0};// pChild->m_pDUIXmlInfo->m_rcPadd;
				szRemain.cx -= rcPadd.left;
				

				// ����x
				CSize szMeasure(POS_INIT,POS_INIT);//�������������ֵ
				szMeasure =pChild->GetDesiredSize(NULL);
				if (POS_INIT == szMeasure.cx)// û�����ÿ�ȵĿؼ�
				{
					iAdjustIndex++;
					szMeasure.cx = ncxAdjustItemLen;
					if (iAdjustIndex == nAdjustNum)
					{// ���һ��û�����ÿ�ȵĿؼ�����Ϊǰ������������С����ʹ��cx!=ncxAdjustItemLen
						szMeasure.cx = szRemain.cx+rcPadd.left-nCxFixedLenRemain;
						szMeasure.cx = szMeasure.cx<=0?0:szMeasure.cx;
					}
					if (nMinWidth!=-1
						&&szMeasure.cx<nMinWidth)
					{
						szMeasure.cx = nMinWidth;
					}
					if (nMaxWidth!=-1
						&&szMeasure.cx>nMaxWidth)
					{
						szMeasure.cx = nMaxWidth;
					}
				}
				else
				{
					if (nMinWidth!=-1
						&&szMeasure.cx<nMinWidth)
					{
						szMeasure.cx = nMinWidth;
					}
					if (nMaxWidth!=-1
						&&szMeasure.cx>nMaxWidth)
					{
						szMeasure.cx = nMaxWidth;
					}
					nCxFixedLenRemain -=szMeasure.cx;
				} 

				// ����y
				szMeasure.cy = pChild->GetWindowRect().Height();
				if (-1 == szMeasure.cy)
				{
					szMeasure.cy =szAvailable.cy-rcPadd.bottom-rcPadd.top;
				}else if (szMeasure.cy<0)
				{
					szMeasure.cy = 0;
				}
				if (nMinHeight!=-1
					&&szMeasure.cy<nMinHeight)
				{
					szMeasure.cy = nMinHeight;
				}
				if (nMaxHeight!=-1
					&&szMeasure.cy>nMaxHeight)
				{
					szMeasure.cy = nMaxHeight;
				}

				CRect rcWindow = GetWindowRect();// ������С
				CRect rcArrange(iPosX+rcPadd.left,rcWindow.top+rcPadd.top,iPosX+rcPadd.left+szMeasure.cx, rcWindow.top+rcPadd.top+szMeasure.cy);
				pChild->Move(&rcArrange);
				pChild=pChild->GetWindow(GSW_NEXTSIBLING);
				iPosX += szMeasure.cx + (NULL == pChild?0:m_iChildPaddLen) + rcPadd.left+rcPadd.right;
				szRemain.cx -= szMeasure.cx + (NULL == pChild?0:m_iChildPaddLen) + rcPadd.right;
				nCxFixedLenRemain -= (NULL == pChild?0:m_iChildPaddLen) + rcPadd.left+rcPadd.right;
				 
			}
		} while (false); 
	}

	/// STableLayout---------------------------------------
	void STableLayout::UpdateChildrenPosition()
	{
		do 
		{
			// 1.����ӿؼ��Ĳ��ֿռ�
			CRect rcLayout = GetChildrenLayoutRect();
			if (rcLayout.IsRectEmpty())
			{
				break;
			}

			// 2.�ж������ӿؼ��ǲ���Ϊ��
			if (0==GetChildrenCount())
			{
				break;
			}

			if (0 == m_szItem.cx||0==m_szItem.cy)
			{
				break;
			}

			CPoint ptTab(rcLayout.left, rcLayout.top);
			bool bEnd = false;
			SWindow  *pObj = GetWindow(GSW_FIRSTCHILD);
			while (pObj)
			{
				if (!pObj->IsVisible(FALSE))
				{
					pObj=pObj->GetWindow(GSW_NEXTSIBLING);
					continue;
				}

				// ����
				if (rcLayout.right<ptTab.x+m_szItem.cx)
				{
					ptTab.x = rcLayout.left;
					ptTab.y += m_szItem.cy;// ����
				}
				if (ptTab.y+m_szItem.cy>rcLayout.bottom)
				{
					bEnd = true;// ����
				}
				CRect rcArrange;
				if (bEnd)
				{
					rcArrange.SetRectEmpty();// ������ÿհ�
				}
				else
				{
					rcArrange.left = ptTab.x;
					rcArrange.top  = ptTab.y;
					rcArrange.right = rcArrange.left+m_szItem.cx;
					rcArrange.bottom = rcArrange.top+m_szItem.cy;
					ptTab.x += m_szItem.cx + m_iChildPaddLen;
				}
				pObj->Move(&rcArrange);
				pObj=pObj->GetWindow(GSW_NEXTSIBLING);
			}
		} while (false); 
		 
	}

	/// SWrapLayout----------------------------------------
	void SWrapLayout::UpdateChildrenPosition()
	{ 
		do 
		{
			// 1.����ӿؼ��Ĳ��ֿռ�
			CRect rcLayout = GetChildrenLayoutRect();
			if (rcLayout.IsRectEmpty())
			{
				break;
			}

			// 2.�ж������ӿؼ��ǲ���Ϊ��
			if (0==GetChildrenCount())
			{
				break;
			}

			// 3.�����������õĴ�С
			//SIZE szAvailable = rcLayout.Size();
			if (m_szItem.cx>0)
			{// Ҳ����Ϊ0����ʱ��Ҫ�����趨��m_nColumns��ȷ��
				m_nColumns = rcLayout.Width()/m_szItem.cx;
			}
			if (0 == m_nColumns)
			{
				m_nColumns = 1;
			}

			int cyNeedLen = 0;
			int cxWidth = rcLayout.Width()/m_nColumns;  // Wrap��Ŀ��

			int cyHeight = m_szItem.cy;
			int iCount   = 0;
			CPoint ptWrap(rcLayout.left, rcLayout.top);
			int iPosX = rcLayout.left;
			SArray<SWindow *> m_ItemArray;
			SWindow *pObj = GetWindow(GSW_FIRSTCHILD);
			while (pObj)
			{
				if (!pObj->IsVisible(FALSE))
				{
					pObj=pObj->GetWindow(GSW_NEXTSIBLING);
					continue;
				}
				m_ItemArray.Add(pObj);
				pObj=pObj->GetWindow(GSW_NEXTSIBLING);
			}

			int nArrayCount = (int)m_ItemArray.GetCount();
			for (int i=0; i<nArrayCount; i++)
			{
				SWindow *&pChild = m_ItemArray[i];
				CRect rcWrapItem(ptWrap.x, ptWrap.y, ptWrap.x+cxWidth, ptWrap.y);
				if (0 == (iCount%m_nColumns))// һ�е��ʼ,�ڴ˼������е����height
				{
					int iIndex = iCount;
					for (int j=i;j<nArrayCount; j++)
					{
						SWindow *&pLineChild = m_ItemArray[j];
						CRect rcPadd = {0,0,0,0};// pLineChild->m_pDUIXmlInfo->m_rcPadd;
						CSize szAvailable(rcWrapItem.right-rcWrapItem.left-rcPadd.left-rcPadd.right,POS_INIT);
						if (iIndex == iCount // һ�е��ʼ
							||0==(iIndex+1)%m_nColumns// һ�е����
							)
						{
							szAvailable.cx -= m_iChildPaddLen/2;
						}
						else
						{
							szAvailable.cx -= m_iChildPaddLen;
						}

						int nMinHeight = -1;// pLineChild->m_pDUIXmlInfo->m_nMinHeight;
						int nMaxHeight = -1;//pLineChild->m_pDUIXmlInfo->m_nMaxHeight;
						int nMinWidth = -1;//pLineChild->m_pDUIXmlInfo->m_nMinWidth;
						int nMaxWidth = -1;//pLineChild->m_pDUIXmlInfo->m_nMaxWidth;
						if (nMinWidth!=-1&&
							szAvailable.cx<nMinWidth)
						{
							szAvailable.cx = nMinWidth;
						}
						if (nMaxWidth!=-1&&
							szAvailable.cx>nMaxWidth)
						{
							szAvailable.cx = nMaxWidth;
						}

						CSize szWrap = szAvailable; //���������ֵ
						szWrap  =pLineChild->GetDesiredSize(NULL);
						if (POS_INIT == szWrap.cy)
						{// ����û�����ø߶ȣ�,���ó�Ĭ������߶ȣ���������ع��ٸ�
							CAutoRefPtr<IFont>  pFont = SFontPool::getSingleton().GetFont(FF_DEFAULTFONT);
							
							szWrap.cy = abs(pFont->LogFont()->lfHeight) + 10;
						}
						if (nMinWidth!=-1&&
							szWrap.cx<nMinWidth)
						{
							szWrap.cx = nMinWidth;
						}
						if (nMaxWidth!=-1&&
							szWrap.cx>nMaxWidth)
						{
							szWrap.cx = nMaxWidth;
						}
						if (nMinHeight!=-1&&
							szWrap.cy<nMinHeight)
						{
							szWrap.cy = nMinHeight;
						}
						if (nMaxHeight!=-1&&
							szWrap.cy>nMaxHeight)
						{
							szWrap.cy = nMaxHeight;
						}
						int nHeightTemp = szWrap.cy+rcPadd.top+rcPadd.bottom;
						cyHeight = cyHeight>nHeightTemp?cyHeight:nHeightTemp; // �������һ������ߵ�����Ϊһ�е�height
						if (0==(++iIndex%m_nColumns))
						{
							break;
						}
					}
				}

				CRect rcPadd = {0,0,0,0};// pChild->m_pDUIXmlInfo->m_rcPadd;
				rcWrapItem.left += rcPadd.left + m_iChildPaddLen/2;
				rcWrapItem.right -= rcPadd.right + m_iChildPaddLen/2;
				if (0 == (iCount%m_nColumns))
				{// һ�е��ʼ
					rcWrapItem.left -= m_iChildPaddLen/2;
				}
				if (0 == (iCount+1)%m_nColumns)
				{// һ�е����
					rcWrapItem.right += m_iChildPaddLen/2;
				}

				int nMinHeight = -1;// pChild->m_pDUIXmlInfo->m_nMinHeight;
				int nMaxHeight = -1;//pChild->m_pDUIXmlInfo->m_nMaxHeight;
				int nMinWidth  = -1;//pChild->m_pDUIXmlInfo->m_nMinWidth;
				int nMaxWidth  = -1;//pChild->m_pDUIXmlInfo->m_nMaxWidth;
				rcWrapItem.top = ptWrap.y + rcPadd.top;
				rcWrapItem.bottom = ptWrap.y + cyHeight;
				CSize szAvailable(rcWrapItem.right-rcWrapItem.left, rcWrapItem.bottom-rcWrapItem.top);
				CSize szWrap = szAvailable;//�������������ֵ
				szWrap  =pChild->GetDesiredSize(NULL);
				if (-1!=nMinWidth&&
					szWrap.cx<nMinWidth)
				{
					szWrap.cx = nMinWidth;
				}
				if (-1!=nMaxWidth&&
					szWrap.cx>nMaxWidth)
				{
					szWrap.cx = nMaxWidth;
				}
				if (-1!=nMinHeight&&
					szWrap.cy<nMinHeight)
				{
					szWrap.cy = nMinHeight;
				}
				if (-1!=nMaxHeight&&
					szWrap.cy>nMaxHeight)
				{
					szWrap.cy = nMaxHeight;
				}

				CRect rcArrange;
				rcArrange.left = (rcWrapItem.left+rcWrapItem.right-szWrap.cx)/2;
				rcArrange.top  = (rcWrapItem.top+rcWrapItem.bottom-szWrap.cy)/2;
				rcArrange.right = rcArrange.left + szWrap.cx;
				rcArrange.bottom = rcArrange.top + szWrap.cy;
				pChild->Move(&rcArrange);

				if (0 == (++iCount%m_nColumns))
				{
					ptWrap.x = iPosX;
					ptWrap.y += cyHeight + m_iChildPaddLen;
					cyHeight = m_szItem.cy;
				}
				else
				{
					ptWrap.x += cxWidth;
				}
				cyNeedLen = rcWrapItem.bottom-rcWrapItem.top;
			}

		} while (false); 
	}

	//--------------------------
	SSplitLayout::SSplitLayout()
	{
		m_bVert				= false;
		m_bFirstChange		= true;
		m_iFirstChildWidth  = 20;
		m_iSliderWid		= 1;
		m_iFixWid			= -1;
		m_bDrag				= false;
		m_ClrSlider = RGB(0xff, 0xff, 0xff);
	}
	BOOL SSplitLayout::CreateChildren(pugi::xml_node xmlNode)
	{
		BOOL iErr = FALSE;
		do 
		{
			iErr = __super::CreateChildren(xmlNode);
			if (2!=GetChildrenCount())
			{
				SASSERT_FMTW(0,L"����splitter��֧�������Ӵ���");
				break;
			}
		} while (false);
		return iErr;
	}

	HRESULT SSplitLayout::OnAttrVert(const SStringW& strValue, BOOL bLoading)
	{
		m_bVert = strValue != L"0";
		if (bLoading)
		{
			if (m_bVert)
			{
				SWindow *pChild = GetWindow(GSW_LASTCHILD);
				if (pChild)
					m_iFixWid = pChild->GetWindowRect().Height(); 
			}
			else
			{
				SWindow *pChild = GetWindow(GSW_LASTCHILD);
				if (pChild)
					m_iFixWid = pChild->GetWindowRect().Width();
			}
		}
		else
		{
			 
		}
		return S_FALSE;
	}

	HRESULT SSplitLayout::OnAttrSliderSkin(const SStringW& strValue, BOOL bLoading)
	{ 
		m_pSliderSkin = GETSKIN(strValue);
		if (!bLoading && m_layout.IsFitContent(PD_ALL))
		{
			RequestRelayout();
		}
		return S_FALSE;
	}

	HRESULT SSplitLayout::OnAttrFirstchange(const SStringW& strValue, BOOL bLoading)
	{
		m_bFirstChange = strValue != L"0";
		if (bLoading)
		{
			if (m_bVert)
			{
				SWindow *pChild = GetWindow(GSW_LASTCHILD);
				if (pChild)
					m_iFixWid = pChild->GetWindowRect().Height();
			}
			else
			{
				SWindow *pChild = GetWindow(GSW_LASTCHILD);
				if (pChild)
					m_iFixWid = pChild->GetWindowRect().Width();
			}
		}
		else
		{

		}
		return S_FALSE;
	}

	//
	void SSplitLayout::OnPaint(IRenderTarget *pRT)
	{
		SPainter painter;

		BeforePaint(pRT, painter);

		do
		{
			__super::OnPaint(pRT);
			if (!m_pSliderSkin && m_ClrSlider != RGB(0xFF, 0xFF, 0xFF))
			{
				break;
			}
			CRect rcSlider = GetSliderRect();
			if (m_pSliderSkin)
			{
				m_pSliderSkin->Draw(pRT, rcSlider, 0);
			}
			else if (m_ClrSlider != RGB(0xFF, 0xFF, 0xFF))
			{
				pRT->FillSolidRect(rcSlider, m_ClrSlider);
			}
		} while (false);

		AfterPaint(pRT, painter); 
	}

	void SSplitLayout::OnLButtonDown(UINT nFlags,CPoint pt)
	{
		do 
		{
			__super::OnLButtonDown(nFlags,pt);
			m_bDrag = true;
			m_ptDrag = pt;
		} while (false);
	}

	void SSplitLayout::OnLButtonUp(UINT nFlags,CPoint pt)
	{
		do 
		{
			__super::OnLButtonUp(nFlags,pt);
			m_bDrag = false;
		} while (false);
	}

	void SSplitLayout::OnMouseMove(UINT nFlags,CPoint pt)
	{
		do 
		{
			if (false == m_bDrag)
			{
				__super::OnMouseMove(nFlags,pt);
				break;
			}

			CPoint ptDiff = m_ptDrag;
			m_ptDrag      = pt;
			ptDiff        = m_ptDrag - ptDiff;
			int iDiff     = m_bVert?ptDiff.y:ptDiff.x;
			UpdateDrag(iDiff);	
		} while (false);
	}
	 
	void SSplitLayout::UpdateChildrenPosition()
	{
		do 
		{
			if (2!=GetChildrenCount())
			{
				SASSERT_FMTW(0,L"����splitter��֧�������Ӵ���");
				break;
			} 
			if (m_bVert)// ��ֱ
			{
				//1.��һ�γ�ʼ��
				if (-1 == m_iFixWid)
				{
					CRect rcFirst = GetWindowRect();
					rcFirst.bottom = rcFirst.top+m_iFirstChildWidth;
					if (GetWindowRect().bottom - rcFirst.bottom<m_iSliderWid)//1.1.ʣ��߶Ȳ��㣬����
					{
						rcFirst.bottom = GetWindowRect().bottom - m_iSliderWid;
					}

					SWindow *pFirstChild = GetWindow(GSW_FIRSTCHILD);
					if (pFirstChild)
						pFirstChild->Move(&rcFirst);

					CRect rcSecond = GetWindowRect();
					rcSecond.top = rcFirst.bottom+m_iSliderWid;
					
					SWindow *pLastChild = GetWindow(GSW_LASTCHILD);
					if (pLastChild)
						pLastChild->Move(&rcSecond); 

					UpdateFixWidth();
				} 
				else//2.�ѳ�ʼ��
				{
					CRect rcFirst = GetWindowRect();
					if (m_bFirstChange)
					{
						rcFirst.bottom -= m_iFixWid+m_iSliderWid;
					}
					else
					{
						rcFirst.bottom = rcFirst.top+m_iFixWid;
					}

					SWindow *pFirstChild = GetWindow(GSW_FIRSTCHILD);
					if (pFirstChild)
						pFirstChild->Move(&rcFirst);

					CRect rcSecond = GetWindowRect();
					rcSecond.top = rcFirst.bottom+m_iSliderWid;

					SWindow *pLastChild = GetWindow(GSW_LASTCHILD);
					if (pLastChild)
						pLastChild->Move(&rcSecond); 
				}
			}
			else
			{
				//1.��һ�γ�ʼ��
				if (-1 == m_iFixWid)
				{
					CRect rcFirst = GetWindowRect();
					rcFirst.right = rcFirst.left+m_iFirstChildWidth;
					if (GetWindowRect().right - rcFirst.right<m_iSliderWid)//1.1.ʣ��߶Ȳ��㣬����
					{
						rcFirst.right = GetWindowRect().right - m_iSliderWid;
					}


					SWindow *pFirstChild = GetWindow(GSW_FIRSTCHILD);
					if (pFirstChild)
						pFirstChild->Move(&rcFirst);

					CRect rcSecond = GetWindowRect();
					rcSecond.left = rcFirst.right+m_iSliderWid;

					SWindow *pLastChild = GetWindow(GSW_LASTCHILD);
					if (pLastChild)
						pLastChild->Move(&rcSecond); 

					UpdateFixWidth();
				}
				else//2.�ѳ�ʼ��
				{
					CRect rcFirst = GetWindowRect();
					if (m_bFirstChange)
					{
						rcFirst.right -= m_iFixWid+m_iSliderWid;
					}
					else
					{
						rcFirst.right = rcFirst.left+m_iFixWid;
					}

					SWindow *pFirstChild = GetWindow(GSW_FIRSTCHILD);
					if (pFirstChild)
						pFirstChild->Move(&rcFirst);
					 
					CRect rcSecond = GetWindowRect();
					rcSecond.left = rcFirst.right+m_iSliderWid;


					SWindow *pLastChild = GetWindow(GSW_LASTCHILD);
					if (pLastChild)
						pLastChild->Move(&rcSecond); 
				}
			}
		} while (false); 
	}

	BOOL SSplitLayout::OnSetCursor(const CPoint &pt)
	{
		::SetCursor(::LoadCursor(NULL,m_bVert?IDC_SIZENS:IDC_SIZEWE));
		return 0;
	}

	///
	BOOL SSplitLayout::UpdateFixWidth()
	{
		if (m_bFirstChange)// �̶���СΪ�ڶ�������
		{
			if (m_bVert)
			{
				SWindow *pChild = GetWindow(GSW_LASTCHILD);
				if (pChild)
					m_iFixWid = pChild->GetWindowRect().Height();
			}
			else
			{
				SWindow *pChild = GetWindow(GSW_LASTCHILD);
				if (pChild)
					m_iFixWid = pChild->GetWindowRect().Width(); 
			}
		}
		else
		{
			if (m_bVert)
			{ 
				SWindow *pChild = GetWindow(GSW_FIRSTCHILD);
				if (pChild)
					m_iFixWid = pChild->GetWindowRect().Height(); 
			}
			else
			{
				SWindow *pChild = GetWindow(GSW_FIRSTCHILD);
				if (pChild)
					m_iFixWid = pChild->GetWindowRect().Width(); 
			}
		}
		if (0>m_iFixWid)
		{
			SASSERT_FMTW(0, L"�̶���m_iFixWid��С��ȻС��0��");
		}
		return TRUE;
	}

	void SSplitLayout::UpdateDrag(int iDiff)
	{
		do 
		{
			if (m_bFirstChange)
			{
				m_iFixWid -= iDiff; 
			}
			else
			{
				m_iFixWid += iDiff;
			}
			if (m_iFixWid<0)
			{
				m_iFixWid = 0;
			}
			int nlimit = m_bVert ? GetWindowRect().Height() - m_iSliderWid : GetWindowRect().Width() - m_iSliderWid;
			if (m_iFixWid>nlimit)
			{
				m_iFixWid = nlimit;
			}
			UpdateChildrenPosition();
			Invalidate();
		} while (false);
	}

	CRect SSplitLayout::GetSliderRect()
	{
		CRect rcSlider;
		do 
		{
			CRect rcFirstWnd;
			SWindow *pFirstChild = GetWindow(GSW_FIRSTCHILD);
			if (pFirstChild)
				rcFirstWnd = pFirstChild->GetWindowRect();

			if (m_bVert)// ��ֱ
			{
				rcSlider = GetWindowRect();
				rcSlider.top = rcFirstWnd.bottom;
				rcSlider.bottom = rcSlider.top+m_iSliderWid;
			}
			else// ˮƽ
			{
				rcSlider = GetWindowRect();
				rcSlider.left = rcFirstWnd.right;
				rcSlider.right = rcSlider.left+m_iSliderWid;
			}
		} while (false);
		return rcSlider;
	}

	 
}//namespace SOUI