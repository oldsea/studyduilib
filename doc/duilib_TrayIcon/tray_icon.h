#ifndef __TRAY_ICON_H_
#define __TRAY_ICON_H_

#include <string>

#include <ObjIdl.h>
#include <shellapi.h>

#include "third_party/duilib/UIlib.h"



	class ITrayIconObserver
	{
	public:
		virtual LRESULT DoTrayIconLButtonUp(UINT uMsg, WPARAM wParam, 
			LPARAM lParam, BOOL& bHandled) = 0; 
		virtual LRESULT DoTrayIconRButtonUp(UINT uMsg, WPARAM wParam, 
			LPARAM lParam, BOOL& bHandled) = 0;
		virtual LRESULT DoTrayIconMouseMove(UINT uMsg, WPARAM wParam, 
			LPARAM lParam, BOOL& bHandled) = 0;
	};

	class CTrayIcon
	{
	public:
		CTrayIcon();
		virtual ~CTrayIcon();

		bool SetIcon(HICON nRes);
		bool SetToolTip(const std::wstring& ATips);
		bool SetVisible(bool AValue);
		void SetObserver(ITrayIconObserver* AObserver){m_Observer = AObserver;}
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	private:
		class CMsgWnd: public DuiLib::CWindowWnd
		{
		public:
			CTrayIcon* m_Owner;
			CMsgWnd(CTrayIcon* AOwner){m_Owner = AOwner;}

			virtual LPCTSTR GetWindowClassName() const {return _T("__TrayIconWnd");}
			virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		};

		bool Refresh(DWORD Message){ return FALSE != Shell_NotifyIcon(Message, &m_data);}
		bool Refresh();
	private:
		bool m_Visible;
		NOTIFYICONDATA m_data;
		ITrayIconObserver* m_Observer;
		HICON m_CurrentIcon;
		UINT WM_TASKBARCREATED;
		HWND m_hwnd;
		CMsgWnd* m_Wnd;
	};
