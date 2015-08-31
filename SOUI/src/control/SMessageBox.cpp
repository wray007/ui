#include "souistd.h"
#include "SApp.h"
#include "control/SMessageBox.h"
#include "control/SCmnCtrl.h"

namespace SOUI
{

    pugi::xml_document SMessageBoxImpl::s_xmlMsgTemplate;
    
    BOOL SMessageBoxImpl::SetMsgTemplate( pugi::xml_node uiRoot )
    {
        if(wcscmp(uiRoot.name(),L"SOUI")!=0 ) return FALSE;
        if(!uiRoot.attribute(L"frameSize").value()[0]) return FALSE;
        if(!uiRoot.attribute(L"minSize").value()[0]) return FALSE;

        s_xmlMsgTemplate.reset();
        s_xmlMsgTemplate.append_copy(uiRoot);
        return TRUE;
    }

    SMessageBoxImpl::SMessageBoxImpl() :SHostDialog(NULL)
    {

    }
    
    static struct MsgBoxInfo
    {
        LPCTSTR pszText;
        LPCTSTR pszCaption;
        UINT    uType;
    }s_MsgBoxInfo;
    
    INT_PTR SMessageBoxImpl::MessageBox( HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType )
    {
        if(!s_xmlMsgTemplate) return ::MessageBox(hWnd,lpText,lpCaption,uType);
        s_MsgBoxInfo.pszText=lpText;
        s_MsgBoxInfo.pszCaption=lpCaption;
        s_MsgBoxInfo.uType=uType;
        
        return DoModal(hWnd);
    }

    BOOL SMessageBoxImpl::OnSetIcon( UINT uType )
    {
        SIconWnd *pIcon=(SIconWnd *)FindChildByName(NAME_MSGBOX_ICON);
        if(!pIcon) return FALSE;
        switch(uType&0xF0)
        {
        case MB_ICONEXCLAMATION:
            pIcon->SetIcon(LoadIcon(NULL,IDI_EXCLAMATION));
            break;
        case MB_ICONINFORMATION:
            pIcon->SetIcon(LoadIcon(NULL,IDI_INFORMATION));
            break;
        case MB_ICONQUESTION:
            pIcon->SetIcon(LoadIcon(NULL,IDI_QUESTION));
            break;
        case MB_ICONHAND:
            pIcon->SetIcon(LoadIcon(NULL,IDI_HAND));
            break;
        default:
            pIcon->SetVisible(FALSE,TRUE);
            break;
        }
        return TRUE;
    }

    struct MSGBTN_TEXT
    {
        int   nBtns;    //按钮数,<=3
        struct
        {
            UINT uBtnID;//按钮ID
            WCHAR szText[20]; //按钮字符    
        }btnInfo[3];
    }g_msgBtnText[]=
    {
        //MB_OK
        {
            1,
            {
                {IDOK,    L"ok"},
                {0,    L""},
                {0,   L""}
            }
        },
        //MB_OKCANCEL
        {
            2,
            {
                {IDOK,    L"ok"},
                {IDCANCEL,    L"cancel"},
                {0,    L""}
            }
        },
        //MB_ABORTRETRYIGNORE
        {
            3,
            {
                {IDABORT,L"abort"},
                {IDRETRY,L"retry"},
                {IDIGNORE,L"ignore"}
            }
        },
        //MB_YESNOCANCEL
        {
            3,
            {
                {IDYES,L"yes"},
                {IDNO,L"no"},
                {IDCANCEL,L"cancel"}
            }
        },
        //MB_YESNO
        {
            2,
            {
                {IDYES,L"yes"},
                {IDNO,L"no"},
                {0,L""}
            }
        },
        //MB_RETRYCANCEL
        {
            2,
            {
                {IDRETRY,L"retry"},
                {IDCANCEL,L"cancel"},
                {0,L""}
            }
        }
    };
    
    const WCHAR * g_wcsNameOfBtns[] =
    {
        NAME_MSGBOX_BTN1,
        NAME_MSGBOX_BTN2,
        NAME_MSGBOX_BTN3
    };
    
    BOOL SMessageBoxImpl::OnInitDialog( HWND wnd, LPARAM lInitParam )
    {
        pugi::xml_node uiRoot=s_xmlMsgTemplate.child(L"SOUI");
        
        InitFromXml(uiRoot);
        UINT uType = s_MsgBoxInfo.uType&0x0F;

        STabCtrl *pBtnSwitch= FindChildByName2<STabCtrl>(NAME_MSGBOX_BTNSWITCH);
        SASSERT(pBtnSwitch);
        pBtnSwitch->SetCurSel(g_msgBtnText[uType].nBtns-1);
        SWindow *pBtnPanel=pBtnSwitch->GetItem(g_msgBtnText[uType].nBtns-1);
        SASSERT(pBtnPanel);
        
        pugi::xml_node nodeBtnTxt = s_xmlMsgTemplate.child(L"SOUI").child(L"buttonText");
        for(int i=0; i<g_msgBtnText[uType].nBtns; i++)
        {
            SWindow *pBtn=pBtnPanel->FindChildByName(g_wcsNameOfBtns[i]);
            const wchar_t *pBtnText = g_msgBtnText[uType].btnInfo[i].szText;
            //先从模板中的buttonText节点里查按钮的文字
            pugi::xml_node nodeTxt = nodeBtnTxt.child(pBtnText);
            if(nodeTxt) pBtnText=nodeTxt.text().get();
            //设置按钮文字，从翻译引擎中翻译
            pBtn->SetWindowText(S_CW2T(TR(pBtnText,GetTranslatorContext())));    
            pBtn->SetID(g_msgBtnText[uType].btnInfo[i].uBtnID);
        }
        
        const wchar_t *pszFrameAttr=uiRoot.attribute(L"frameSize").value();
        CRect rcFrame;
        swscanf(pszFrameAttr,L"%d,%d,%d,%d",&rcFrame.left,&rcFrame.top,&rcFrame.right,&rcFrame.bottom);
        CSize szMin;
        const wchar_t *pszMinAttr=uiRoot.attribute(L"minSize").value();
        swscanf(pszMinAttr,L"%d,%d",&szMin.cx,&szMin.cy);

        SWindow * pTitle= FindChildByName(NAME_MSGBOX_TITLE);
        SASSERT(pTitle);
        pTitle->SetWindowText(S_CW2T(TR(s_MsgBoxInfo.pszCaption?S_CT2W(s_MsgBoxInfo.pszCaption):L"prompt",GetTranslatorContext())));

        SWindow * pMsg= FindChildByName(NAME_MSGBOX_TEXT);
        SASSERT(pMsg);
        pMsg->SetWindowText(S_CW2T(TR(S_CT2W(s_MsgBoxInfo.pszText),GetTranslatorContext())));

        OnSetIcon(s_MsgBoxInfo.uType);
        
        CSize szText = pMsg->GetDesiredSize(NULL);

        CRect rcText = pMsg->GetWindowRect();
        
        CSize szWnd;
        szWnd.cx=max(szMin.cx,rcText.left + szText.cx + rcFrame.right);
        szWnd.cy=max(szMin.cy,rcFrame.top + szText.cy + rcFrame.bottom);

        SetWindowPos(0,0,0,szWnd.cx,szWnd.cy,SWP_NOMOVE|SWP_NOACTIVATE);     
        CenterWindow();
        return 0;
    }

    int SMessageBox( HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType )
    {
        SMessageBoxImpl duiMsgBox;
        return duiMsgBox.MessageBox(hWnd,lpText,lpCaption,uType);
    }

}//end of namespace 
