// MyApp.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "MyApp.h"

class CMainFrame:public CWindowWnd, public INotifyUI
{
public:
	CMainFrame();
	~CMainFrame();
	LPCTSTR GetWindowClassName()const;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Notify(TNotifyUI& msg);
	void OnPrepare();

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	CPaintManagerUI m_pm;  
};
LPCTSTR CMainFrame::GetWindowClassName()const
{
	return _T("UIMainFrame");
}
void CMainFrame::OnPrepare()
{

}

LRESULT CMainFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);  
	styleValue &= ~WS_CAPTION;  
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);  

	m_pm.Init(m_hWnd);//���������봰�ھ������  
	CDialogBuilder builder;  

	CControlUI* pRoot = builder.Create(_T("UISkin.xml"), (UINT)0, NULL, &m_pm);//����XML����̬�����������أ��벼�ֽ���Ԫ�أ����ĺ�����������  
	//ע�⣺CDialogBuilder ������һ���Ի�����  
	ASSERT(pRoot && "Failed to parse XML");  
	if (NULL==pRoot)//����Ҳ���Ƥ���ļ����˳�  
	{  
		MessageBox(NULL,TEXT("Cant not find the skin!"),NULL,MB_ICONHAND);  
		return 0;  
	}  
	m_pm.AttachDialog(pRoot);//���ӿؼ����ݵ�HASH���С���ΪpRoot��Ϊ�Ի����㣬Ϊ�䴴���ؼ���   
	m_pm.AddNotifier(this);//����֪ͨ����  

	return 0; 

}
LRESULT CMainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;  
	BOOL bHandled = TRUE;  
	switch( uMsg ) {  
	case WM_CREATE:   
		lRes = OnCreate(uMsg, wParam, lParam, bHandled);   
		break;  
	default:  
	bHandled = FALSE;  
	}  
	if( bHandled ) return lRes;  
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;  
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);  
}

void CMainFrame::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("windowinit"))
	{
		OnPrepare();
	}
}


CMainFrame::CMainFrame()
{

}
CMainFrame::~CMainFrame()
{

}


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);//���ó���ʵ��  
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath()   
		+ _T("skin\\myappres"));//ʵ���������Ⱦ�����,���Ƥ���ļ�Ŀ¼������Ƥ���ļ���OnCreate֮�У�  

	HRESULT Hr = ::CoInitialize(NULL);//��ʼ��COM��, Ϊ����COM���ṩ֧��  
	if( FAILED(Hr) )   
		return 0;  

	CMainFrame* pMainFrame = new CMainFrame();//����Ӧ�ó��򴰿������  
	if( pMainFrame == NULL )   
		return 0;  
	pMainFrame->Create(NULL, _T("AdderCalc"), UI_WNDSTYLE_DIALOG, 0);  
	pMainFrame->CenterWindow();//�����ڷŵ���������  
	pMainFrame->ShowWindow(true);//��ʾ����  
	CPaintManagerUI::MessageLoop();//������Ϣѭ��  

	::CoUninitialize();//�˳������ͷ�COM��  
	return 0;  
}

