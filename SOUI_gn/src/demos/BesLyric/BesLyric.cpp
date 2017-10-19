﻿/*
*	Copyright (C) 2017  BensonLaur
*	note: Looking up header file for license detail
*/

// dui-demo.cpp : main source file
//

#include "MainDlg.h"
#include "com-cfg.h"
using namespace SOUI;
//从PE文件加载，注意从文件加载路径位置
//#define RES_TYPE 1
#define RES_TYPE 0   //从文件中加载资源
// #define RES_TYPE 1  //从PE资源中加载UI资源
 
#define SYS_NAMED_RESOURCE _T("soui-sys-resource.dll") 
	
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int /*nCmdShow*/)
{
    HRESULT hRes = OleInitialize(NULL);
	(void)hRes;
    SASSERT(SUCCEEDED(hRes));

    int nRet = 0;
    
    SComMgr *pComMgr = new SComMgr;

    //将程序的运行路径修改到项目所在目录所在的目录
    TCHAR szCurrentDir[MAX_PATH] = { 0 };

	GetModuleFileName(NULL, szCurrentDir, sizeof(szCurrentDir));
	LPTSTR lpInsertPos = _tcsrchr(szCurrentDir, _T('\\'));
	_tcscpy(lpInsertPos + 1, _T("..\\..\\src\\demos\\BesLyric"));
	SetCurrentDirectory(szCurrentDir);
    {
        BOOL bLoaded=FALSE;
        CAutoRefPtr<SOUI::IImgDecoderFactory> pImgDecoderFactory;
        CAutoRefPtr<SOUI::IRenderFactory> pRenderFactory;
        bLoaded = pComMgr->CreateRender_GDI((IObjRef**)&pRenderFactory);
        SASSERT_FMT(bLoaded,_T("load interface [render] failed!"));
        bLoaded=pComMgr->CreateImgDecoder((IObjRef**)&pImgDecoderFactory);
        SASSERT_FMT(bLoaded,_T("load interface [%s] failed!"),_T("imgdecoder"));

        pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);
        SApplication *theApp = new SApplication(pRenderFactory, hInstance);
        //从DLL加载系统资源
        HMODULE hModSysResource = LoadLibrary(SYS_NAMED_RESOURCE);
        if (hModSysResource)
        {
            CAutoRefPtr<IResProvider> sysResProvider;
            CreateResProvider(RES_PE, (IObjRef**)&sysResProvider);
            sysResProvider->Init((WPARAM)hModSysResource, 0);
            theApp->LoadSystemNamedResource(sysResProvider);
            FreeLibrary(hModSysResource);
        }else
        {
            SASSERT(0);
        }

        CAutoRefPtr<IResProvider>   pResProvider;
#if (RES_TYPE == 0)
        CreateResProvider(RES_FILE, (IObjRef**)&pResProvider);
        if (!pResProvider->Init((LPARAM)_T("uires"), 0))
        {
            SASSERT(0);
            return 1;
        }
#else 
        CreateResProvider(RES_PE, (IObjRef**)&pResProvider);
        pResProvider->Init((WPARAM)hInstance, 0);
#endif

		theApp->InitXmlNamedID(namedXmlID,ARRAYSIZE(namedXmlID),TRUE);
        theApp->AddResProvider(pResProvider);

		//读取自定义消息框布局
		int ret = 0;
		pugi::xml_document xmlDoc;
		if(!theApp->LoadXmlDocment(xmlDoc,_T("xml_messagebox"),_T("LAYOUT")) || !SetMsgTemplate(xmlDoc.child(L"SOUI")))
			ret=-1;
		if(ret==-1)
			SMessageBox(NULL,_T("【消息框皮肤】读取失败"),_T("提示"),0);
		
        
        // BLOCK: Run application
        {
            CMainDlg dlgMain;
            dlgMain.Create(GetActiveWindow());
            dlgMain.SendMessage(WM_INITDIALOG);
            dlgMain.CenterWindow(dlgMain.m_hWnd);
            dlgMain.ShowWindow(SW_SHOWNORMAL);
            nRet = theApp->Run(dlgMain.m_hWnd);
        }

        delete theApp;
    }
    
    delete pComMgr;
    
    OleUninitialize();
    return nRet;
}