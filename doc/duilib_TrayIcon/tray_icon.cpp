#include "tray_icon.h"

using DuiLib::CPoint;


const DWORD WM_SYSTEM_TRAY_MESSAGE = WM_USER + 1;
CTrayIcon::CTrayIcon():m_Wnd(NULL),m_CurrentIcon(NULL),m_Observer(NULL),m_Visible(false)
{
    WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));
    m_Wnd = new CMsgWnd(NULL);
    m_hwnd = m_Wnd->Create(0, NULL, WS_POPUP,WS_EX_TOOLWINDOW);
    m_Wnd->ShowWindow(false,false);
    memset(&m_data, 0, sizeof(NOTIFYICONDATA));
    m_data.cbSize = sizeof(NOTIFYICONDATA);
    m_data.hWnd = m_hwnd;
    m_data.uID = UINT(m_data.hWnd);
    m_data.hIcon = m_CurrentIcon;
    m_data.uFlags |=  NIF_ICON | NIF_MESSAGE;
    m_data.uCallbackMessage = WM_SYSTEM_TRAY_MESSAGE;
    m_Wnd->m_Owner = this;
    Refresh();
}

CTrayIcon::~CTrayIcon()
{
    delete m_Wnd;
    m_Wnd = NULL;
}

bool CTrayIcon::Refresh()
{
    m_data.hIcon = m_CurrentIcon; 
    if (m_Visible) return Refresh(NIM_MODIFY);
    return true;
}

LRESULT CTrayIcon::CMsgWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    if (m_Owner)
    {
        BOOL handled = FALSE;
        LRESULT lres = m_Owner->HandleMessage(uMsg, wParam, lParam, handled);
        if (handled) return lres;

    }
    return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}


LRESULT CTrayIcon::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    LRESULT lres = FALSE;
    if (wParam != m_data.uID) return lres;
    if ((uMsg == WM_TASKBARCREATED) && m_Visible)
    {
        Refresh(NIM_ADD);
        bHandled = TRUE;
        lres = TRUE;
    }
    else if (uMsg == WM_SYSTEM_TRAY_MESSAGE)
    {
        switch (lParam)
        {
		case WM_MOUSEMOVE:
			{
				if (m_Observer)
				{
// 					CPoint point;
// 					::GetCursorPos(&point);
// 					lres = m_Observer->DoTrayIconMouseMove(WM_MOUSEMOVE, wParam,  
// 						MAKELPARAM(point.x, point.y), bHandled);
				}
				break;
			}
			break;
        case WM_LBUTTONUP:
            {
                if (m_Observer)
                {
                    CPoint point;
                    ::GetCursorPos(&point);
                    lres = m_Observer->DoTrayIconLButtonUp(WM_LBUTTONUP, wParam,  
						MAKELPARAM(point.x, point.y), bHandled);
                }
                break;
            }
        case WM_RBUTTONUP:
            {
                if (m_Observer )
                {
                    CPoint point;
                    ::GetCursorPos(&point);
                    lres = m_Observer->DoTrayIconRButtonUp(WM_RBUTTONUP, wParam, 
						MAKELPARAM(point.x, point.y), bHandled);
                }
                break;
            }
        }
    }
    else if (uMsg == WM_ENDSESSION && wParam == TRUE)
    {
        Refresh(NIM_DELETE);
        bHandled = TRUE;
        lres = TRUE;
    }
    return lres;

}

bool CTrayIcon::SetIcon( HICON nRes )
{
    m_CurrentIcon = nRes;
    return Refresh();
}

bool CTrayIcon::SetToolTip( const std::wstring& ATips )
{
    wcscpy_s(m_data.szTip, ATips.c_str());
    if (wcslen(m_data.szTip) > 0)
        m_data.uFlags |= NIF_TIP;
    else
        m_data.uFlags &= ~NIF_TIP;
    return Refresh();
}

bool CTrayIcon::SetVisible( bool AValue )
{
    if (m_Visible == AValue) return true;
    m_Visible = AValue;
    if (m_Visible)
       return Refresh(NIM_ADD);
    else
       return Refresh(NIM_DELETE);
}


