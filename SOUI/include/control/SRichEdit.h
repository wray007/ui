/**
* Copyright (C) 2014-2050 SOUI�Ŷ�
* All rights reserved.
* 
* @file       SRichEdit.h
* @brief      RichEdit�ؼ�
* @version    v1.0      
* @author     soui      
* @date       2014-07-13
* 
* Describe    RichEdit�ؼ�
*/
#pragma once

#include <Imm.h>
#include <Richedit.h>
#include <TextServ.h>
#include "core/SPanel.h"
#include "core/SSingleton.h"

namespace SOUI
{

    class SRichEdit;
    /**
    * @class      STextServiceHelper
    * @brief      
    * 
    * Describe    
    */
    class SOUI_EXP STextServiceHelper: public SSingleton<STextServiceHelper>
    {
    public:
        /**
        * STextServiceHelper::CreateTextServices
        * @brief    
        * @param  IUnknown *punkOuter
        * @param  ITextHost *pITextHost
        * @param  IUnknown **ppUnk
        * @return ����HRESULT  
        *
        * Describe  
        */
        HRESULT CreateTextServices( IUnknown *punkOuter, ITextHost *pITextHost, IUnknown **ppUnk );

        /**
        * STextServiceHelper::Init
        * @brief    ��ʼ�� 
        * @return   ����BOOL
        *
        * Describe  ��ʼ��
        */
        static BOOL Init();

        /**
        * STextServiceHelper::Destroy
        * @brief    ���� 
        *
        * Describe  ���� 
        */
        static void Destroy()
        {
            if(ms_Singleton) delete ms_Singleton;
        }

    protected:
        /**
        * STextServiceHelper::STextServiceHelper
        * @brief    ���캯�� 
        *
        * Describe  ���캯�� 
        */
        STextServiceHelper();
        /**
        * STextServiceHelper::~STextServiceHelper
        * @brief    �������� 
        *
        * Describe  �������� 
        */
        ~STextServiceHelper();

        HINSTANCE    m_rich20; /**< richedit module */
        PCreateTextServices    m_funCreateTextServices;  /**< �ص����� */
    };

    /**
    * @class      SRicheditMenuDef
    * @brief      
    * 
    * Describe    
    */
    class SOUI_EXP SRicheditMenuDef : public SSingleton<SRicheditMenuDef>
    {
    public:
        /**
        * SRicheditMenuDef::Init
        * @brief    ��ʼ�� 
        * @return   ����BOOL
        *
        * Describe  ��ʼ��
        */
        static BOOL Init(){
            if(ms_Singleton) return FALSE;
            new SRicheditMenuDef();
            return TRUE;
        }

        /**
        * SRicheditMenuDef::Destroy
        * @brief    ���� 
        *
        * Describe  ���� 
        */
        static void Destroy()
        {
            if(ms_Singleton) delete ms_Singleton;
        }

        /**
        * SRicheditMenuDef::SetMenuXml
        * @brief    ����xml�ļ�
        *
        * Describe  ����xml�ļ� 
        */
        void SetMenuXml(pugi::xml_node xmlMenu)
        {
            m_xmlMenu.reset();
            m_xmlMenu.append_copy(xmlMenu);
        }

        /**
        * SRicheditMenuDef::GetMenuXml
        * @brief    ��ȡxml�ļ� 
        *
        * Describe  ��ȡxml�ļ� 
        */
        pugi::xml_node GetMenuXml()
        {
            return m_xmlMenu.first_child();
        }
    protected:
        pugi::xml_document  m_xmlMenu;  /**< xml�ļ����� */
    };

    /**
    * @class      STextHost
    * @brief      
    * 
    * Describe    
    */
    class SOUI_EXP STextHost : public ITextHost
    {
        friend class SRichEdit;
    public:
        /**
        * STextHost::STextHost
        * @brief    ���캯�� 
        *
        * Describe  ���캯�� 
        */
        STextHost(void);
        /**
        * STextHost::~STextHost
        * @brief    �������� 
        *
        * Describe  �������� 
        */
        ~STextHost(void);
        /**
        * STextHost::Init
        * @brief    ��ʼ������
        * @param    SRichEdit* pRichEdit -- SRichEdit����
        *
        * Describe  ��ʼ������ 
        */
        BOOL Init(SRichEdit* pRichEdit);
        /**
        * STextHost::GetTextService
        * @brief     
        *
        * Describe   
        */
        ITextServices * GetTextService()
        {
            return pserv;
        }

        /**
        * STextHost::GetCaretPos
        * @brief     ��ȡ����
        *
        * Describe   ��ȡ����   
        */
        POINT GetCaretPos(){return m_ptCaret;}
    protected:

        /**
        * STextHost::QueryInterface
        * @brief     
        * @param     REFIID riid
        * @param     void **ppvObject
        *
        * Describe      
        */
        virtual HRESULT _stdcall QueryInterface(REFIID riid, void **ppvObject);
        /**
        * STextHost::AddRef
        * @brief     
        * @brief     
        * @return    ����UNLONG
        *
        * Describe      
        */
        virtual ULONG _stdcall AddRef(void);
        /**
        * STextHost::Release
        * @brief     
        * @return    ����UNLONG
        *
        * Describe      
        */
        virtual ULONG _stdcall Release(void);

        /**
        * STextHost::TxGetDC
        * @brief     Get the DC for the host 
        * @return    ����HDC
        *
        * Describe   Get the DC for the host    
        */
        virtual HDC   TxGetDC();

        /**
        * STextHost::TxReleaseDC
        * @brief     Release the DC gotten from the host
        * @return    ����INT
        *
        * Describe   Release the DC gotten from the host   
        */
        virtual INT   TxReleaseDC(HDC hdc);

        /**
        * STextHost::TxShowScrollBar
        * @brief     Show the scroll bar
        * @param     INT fnBar -- 
        * @param     BOOL fShow -- 
        * @return    ����BOOL
        *
        * Describe   Show the scroll bar   
        */
        virtual BOOL  TxShowScrollBar(INT fnBar, BOOL fShow);

        /**
        * STextHost::TxEnableScrollBar
        * @brief     Enable the scroll bar
        * @param     INT fuSBFlags -- 
        * @param     INT fuArrowflags -- 
        * @return    ����BOOL
        *
        * Describe   Enable the scroll bar   
        */
        virtual BOOL  TxEnableScrollBar (INT fuSBFlags, INT fuArrowflags);

        /**
        * STextHost::TxEnableScrollBar
        * @brief     Set the scroll range
        * @param     INT fnBar -- 
        * @param     LONG nMinPos -- 
        * @param     INT nMaxPos -- 
        * @param     BOOL fRedraw -- 
        * @return    ����BOOL
        *
        * Describe   Set the scroll range   
        */
        virtual BOOL  TxSetScrollRange(
            INT fnBar,
            LONG nMinPos,
            INT nMaxPos,
            BOOL fRedraw);

        /**
        * STextHost::TxSetScrollPos
        * @brief     Set the scroll position
        * @param     INT fnBar -- 
        * @param     INT nPos -- 
        * @param     BOOL fRedraw -- 
        * @return    ����BOOL
        *
        * Describe   Set the scroll position  
        */
        virtual BOOL  TxSetScrollPos (INT fnBar, INT nPos, BOOL fRedraw);

        /**
        * STextHost::TxInvalidateRect
        * @brief     InvalidateRect
        * @param     LPCRECT prc -- 
        * @param     BOOL fMode -- 
        *
        * Describe   Set the scroll position  
        */
        virtual void  TxInvalidateRect(LPCRECT prc, BOOL fMode);

        /**
        * STextHost::TxViewChange
        * @brief     Send a WM_PAINT to the window
        * @param     BOOL fUpdate -- 
        *
        * Describe   Send a WM_PAINT to the window 
        */
        virtual void  TxViewChange(BOOL fUpdate);

        /**
        * STextHost::TxCreateCaret
        * @brief     Create the caret
        * @param     HBITMAP hbmp -- caret bitmap
        * @param     INT xWidth -- caret width
        * @param     INT yHeight -- caret height
        * @return    ����BOOL
        *
        * Describe   Create the caret
        */
        virtual BOOL  TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight);

        /**
        * STextHost::TxShowCaret
        * @brief     Show the caret
        * @param     BOOL fShow -- true to show the caret
        * @return    ����BOOL
        *
        * Describe   Show the caret
        */
        virtual BOOL  TxShowCaret(BOOL fShow);

        /**
        * STextHost::TxSetCaretPos
        * @brief     Set the caret position
        * @param     INT x -- caret position:x
        * @param     INT y -- caret position:y
        * @return    ����BOOL
        *
        * Describe   Set the caret position
        */
        virtual BOOL  TxSetCaretPos(INT x, INT y);

        /**
        * STextHost::TxSetTimer
        * @brief     Create a timer with the specified timeout
        * @param     UINT idTimer -- timer ID
        * @param     UINT uTimeout -- time interval
        * @return    ����BOOL
        *
        * Describe   Create a timer with the specified timeout
        */
        virtual BOOL  TxSetTimer(UINT idTimer, UINT uTimeout);

        /**
        * STextHost::TxSetTimer
        * @brief     Destroy a timer
        * @param     UINT idTimer -- timer id
        * @return    ����BOOL
        *
        * Describe   Destroy a timer
        */
        virtual void  TxKillTimer(UINT idTimer);

        /**
        * STextHost::TxScrollWindowEx
        * @brief     Scroll the content of the specified window's client area
        * @param     INT dx --
        * @param     INT dy --
        * @param     LPCRECT lprcScroll --
        * @param     LPCRECT lprcClip --
        * @param     HRGN hrgnUpdate --
        * @param     LPRECT lprcUpdate --
        * @param     UINT fuScroll -- 
        *
        * Describe   Scroll the content of the specified window's client area
        */
        virtual void  TxScrollWindowEx (
            INT dx,
            INT dy,
            LPCRECT lprcScroll,
            LPCRECT lprcClip,
            HRGN hrgnUpdate,
            LPRECT lprcUpdate,
            UINT fuScroll);

        /**
        * STextHost::TxSetCapture
        * @brief     Get mouse capture
        * @param     BOOL fCapture --
        *
        * Describe   Get mouse capture
        */
        virtual void  TxSetCapture(BOOL fCapture);

        /**
        * STextHost::TxSetFocus
        * @brief     Set the focus to the text window
        *
        * Describe   Set the focus to the text window
        */
        virtual void  TxSetFocus();

        /**
        * STextHost::TxSetCursor
        * @brief     Establish a new cursor shape
        * @param     HCURSOR hcur --
        * @param     BOOL fText --
        *
        * Describe   Establish a new cursor shape
        */
        virtual void  TxSetCursor(HCURSOR hcur, BOOL fText);

        /**
        * STextHost::TxScreenToClient
        * @brief     Converts screen coordinates of a specified point to the client coordinates
        * @param     LPPOINT lppt --
        * @return    ����BOOL
        *
        * Describe   Converts screen coordinates of a specified point to the client coordinates
        */
        virtual BOOL  TxScreenToClient (LPPOINT lppt);

        /**
        * STextHost::TxClientToScreen
        * @brief     Converts the client coordinates of a specified point to screen coordinates
        * @param     LPPOINT lppt --
        * @return    ����BOOL
        *
        * Describe   Converts the client coordinates of a specified point to screen coordinates
        */
        virtual BOOL  TxClientToScreen (LPPOINT lppt);

        /**
        * STextHost::TxActivate
        * @brief     Request host to activate text services
        * @param     LONG * plOldState --
        * @return    ����HRESULT
        *
        * Describe   Request host to activate text services
        */
        virtual HRESULT  TxActivate( LONG * plOldState );

        /**
        * STextHost::TxDeactivate
        * @brief     Request host to deactivate text services
        * @param     LONG lNewState --
        * @return    ����HRESULT
        *
        * Describe   Request host to deactivate text services
        */
        virtual HRESULT  TxDeactivate( LONG lNewState );

        /**
        * STextHost::TxGetClientRect
        * @brief     Retrieves the coordinates of a window's client area
        * @param     LPRECT prc --
        * @return    ����HRESULT
        *
        * Describe   Retrieves the coordinates of a window's client area
        */
        virtual HRESULT   TxGetClientRect(LPRECT prc);

        /**
        * STextHost::TxGetViewInset
        * @brief     Get the view rectangle relative to the inset
        * @param     LPRECT prc --
        * @return    ����HRESULT
        *
        * Describe   Get the view rectangle relative to the inset
        */
        virtual HRESULT  TxGetViewInset(LPRECT prc);

        /**
        * STextHost::TxGetCharFormat
        * @brief     Get the default character format for the text
        * @param     const CHARFORMATW **ppCF --
        * @return    ����HRESULT
        *
        * Describe   Get the default character format for the text
        */
        virtual HRESULT  TxGetCharFormat(const CHARFORMATW **ppCF );

        /**
        * STextHost::TxGetParaFormat
        * @brief     Get the default paragraph format for the text
        * @param     const PARAFORMAT **ppPF --
        * @return    ����HRESULT
        *
        * Describe   Get the default character format for the text
        */
        virtual HRESULT  TxGetParaFormat(const PARAFORMAT **ppPF);

        /**
        * STextHost::TxGetSysColor
        * @brief     Get the background color for the window
        * @param     int nIndex --
        * @return    ����COLORREF  
        *
        * Describe   Get the background color for the window
        */
        virtual COLORREF  TxGetSysColor(int nIndex);

        /**
        * STextHost::TxGetBackStyle
        * @brief     Get the background (either opaque or transparent)
        * @param     TXTBACKSTYLE *pstyle --
        * @return    ����HRESULT
        *
        * Describe   Get the background (either opaque or transparent)
        */
        virtual HRESULT  TxGetBackStyle(TXTBACKSTYLE *pstyle);

        /**
        * STextHost::TxGetMaxLength
        * @brief     Get the maximum length for the text
        * @param     DWORD *plength --
        * @return    ����HRESULT
        *
        * Describe   Get the maximum length for the text
        */
        virtual HRESULT  TxGetMaxLength(DWORD *plength);

        /**
        * STextHost::TxGetScrollBars
        * @brief     Get the bits representing requested scroll bars for the window
        * @param     DWORD *pdwScrollBar --
        * @return    ����HRESULT
        *
        * Describe   Get the bits representing requested scroll bars for the window
        */
        virtual HRESULT  TxGetScrollBars(DWORD *pdwScrollBar);

        /**
        * STextHost::TxGetPasswordChar
        * @brief     Get the character to display for password input
        * @param     TCHAR *pch --
        * @return    ����HRESULT
        *
        * Describe   Get the character to display for password input
        */
        virtual HRESULT  TxGetPasswordChar(TCHAR *pch);

        /**
        * STextHost::TxGetAcceleratorPos
        * @brief     Get the accelerator character
        * @param     LONG *pcp --
        * @return    ����HRESULT
        *
        * Describe   Get the accelerator character
        */
        virtual HRESULT  TxGetAcceleratorPos(LONG *pcp);

        /**
        * STextHost::TxGetExtent
        * @brief     Get the native size
        * @param     LPSIZEL lpExtent --
        * @return    ����HRESULT
        *
        * Describe   Get the native size
        */
        virtual HRESULT  TxGetExtent(LPSIZEL lpExtent);

        /**
        * STextHost::OnTxCharFormatChange
        * @brief     Notify host that default character format has changed
        * @param     const CHARFORMATW * pcf --
        * @return    ����HRESULT
        *
        * Describe   Notify host that default character format has changed
        */
        virtual HRESULT  OnTxCharFormatChange(const CHARFORMATW * pcf);

        /**
        * STextHost::OnTxParaFormatChange 
        * @brief     Notify host that default paragraph format has changed
        * @param     const PARAFORMAT * ppf --
        * @return    ����HRESULT
        *
        * Describe   Notify host that default paragraph format has changed
        */
        virtual HRESULT  OnTxParaFormatChange (const PARAFORMAT * ppf);

        /**
        * STextHost::TxGetPropertyBits
        * @brief     Bulk access to bit properties
        * @param     DWORD dwMask --
        * @param     DWORD *pdwBits --
        * @return    ����HRESULT
        *
        * Describe   Bulk access to bit properties
        */
        virtual HRESULT  TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits);

        /**
        * STextHost::TxNotify
        * @brief     Notify host of events
        * @param     DWORD iNotify  --
        * @param     void *pv --
        * @return    ����HRESULT
        *
        * Describe   Bulk access to bit properties
        */
        virtual HRESULT  TxNotify(DWORD iNotify, void *pv);

        // Far East Methods for getting the Input Context
        //#ifdef WIN95_IME
        /**
        * STextHost::TxImmGetContext
        * @brief     
        * @return     ����HIMC
        *
        * Describe  
        */
        virtual HIMC  TxImmGetContext();
        /**
        * STextHost::TxImmReleaseContext
        * @brief     
        * @param     HIMC himc  --
        *
        * Describe  
        */
        virtual void  TxImmReleaseContext( HIMC himc );
        //#endif

        /**
        * STextHost::TxGetSelectionBarWidth 
        * @brief     Returns HIMETRIC size of the control bar
        * @param     LONG *plSelBarWidth  --
        *
        * Describe   Returns HIMETRIC size of the control bar
        */
        virtual HRESULT  TxGetSelectionBarWidth (LONG *plSelBarWidth);
    protected:
        BOOL m_fUiActive; /**< Whether control is inplace active */

        ULONG            cRefs;          /**< Reference Count */
        ITextServices    *pserv;         /**< pointer to Text Services object */
        SRichEdit        *m_pRichEdit;   /**< swindow for text host */ 
        POINT            m_ptCaret;
    };

    /**
    * @class      SRichEdit
    * @brief      ʹ��Windowless Richeditʵ�ֵ�edit�ؼ�
    * 
    * Describe    SRichEdit
    */
    class SOUI_EXP SRichEdit :public SPanel
    {
        friend class STextHost;
    public:
        SOUI_CLASS_NAME(SRichEdit, L"richedit")

        /**
        * SRichEdit::SRichEdit
        * @brief     ���캯��
        *
        * Describe   ���캯��
        */
        SRichEdit();

        /**
        * SRichEdit::~SRichEdit
        * @brief     ��������
        *
        * Describe   ��������
        */
        virtual ~SRichEdit() {}

        DWORD SaveRtf(LPCTSTR pszFileName);

        DWORD LoadRtf(LPCTSTR pszFileName);
    public://richedit interface
        /**
        * SRichEdit::GetWindowText
        * @brief     ��ȡ���ڱ���
        * @return    ����SStringT
        *
        * Describe   ��ȡ���ڱ���
        */
        SStringT GetWindowText();
        /**
        * SRichEdit::GetWindowTextLength
        * @brief     ��ȡ���ڱ��ⳤ��
        * @return    ����int
        *
        * Describe   ��ȡ���ڱ��ⳤ��
        */
        int GetWindowTextLength();
        /**
        * SRichEdit::SetWindowText
        * @brief     ���ô��ڱ���
        * @param     LPCWSTR lpszText -- ���ڱ���
        *
        * Describe   ���ô��ڱ���
        */
        void SetWindowText(LPCTSTR lpszText);
        /**
        * SRichEdit::SetSel
        * @brief     ����ѡ��
        * @param     DWORD dwSelection -- 
        * @param     BOOL bNoScroll -- 
        *
        * Describe   ����ѡ��
        */
        void SetSel(DWORD dwSelection, BOOL bNoScroll = FALSE);
        /**
        * SRichEdit::ReplaceSel
        * @brief     �滻ѡ����
        * @param     LPCWSTR pszText -- 
        * @param     BOOL bCanUndo -- 
        *
        * Describe   �滻ѡ����
        */
        void ReplaceSel(LPCWSTR pszText,BOOL bCanUndo=TRUE);
        /**
        * SRichEdit::GetWordWrap
        * @brief    
        * @return   ����BOOL  
        *
        * Describe   
        */
        BOOL GetWordWrap(void);
        /**
        * SRichEdit::SetWordWrap
        * @brief     
        * @param     BOOL fWordWrap -- 
        *
        * Describe   
        */
        void SetWordWrap(BOOL fWordWrap);

        /**
        * SRichEdit::GetReadOnly
        * @brief    �ж��Ƿ�ֻ��
        * @return   ����BOOL 
        *
        * Describe  �ж��Ƿ�ֻ�� 
        */
        BOOL GetReadOnly();

        /**
        * SRichEdit::SetReadOnly
        * @brief     ����ֻ��
        * @param     BOOL bReadOnly -- �Ƿ�ֻ��
        * @return    ����BOOL 
        *
        * Describe   ����ֻ��   
        */
        BOOL SetReadOnly(BOOL bReadOnly);

        /**
        * SRichEdit::GetLimitText
        * @brief     ��ȡ��С�ı�����
        * @return    ����BOOL 
        *
        * Describe   ��ȡ��С�ı�����  
        */
        LONG GetLimitText();

        /**
        * SRichEdit::SetLimitText
        * @brief     ������С�ı�����  
        * @param     int nLength -- ����
        * @return    ����BOOL
        *
        * Describe   ������С�ı�����    
        */
        BOOL SetLimitText(int nLength);

        /**
        * SRichEdit::GetDefaultAlign
        * @brief     ��ȡ���뷽ʽ
        * @return    ����WORD
        *
        * Describe   ������С�ı�����    
        */
        WORD GetDefaultAlign();

        /**
        * SRichEdit::SetDefaultAlign
        * @brief     ����Ĭ�϶��뷽ʽ 
        * @param     WORD wNewAlign -- ���뷽ʽ
        *
        * Describe   ����Ĭ�϶��뷽ʽ   
        */
        void SetDefaultAlign(WORD wNewAlign);

        /**
        * SRichEdit::GetRichTextFlag
        * @brief     ��ȡ��־
        * @return    ����BOOL
        *
        * Describe   ��ȡ��־
        */
        BOOL GetRichTextFlag();

        /**
        * SRichEdit::SetRichTextFlag
        * @brief     ���ñ�־
        * @param     BOOL fRich -- ��־
        *
        * Describe   ���ñ�־
        */
        void SetRichTextFlag(BOOL fRich);

        /**
        * SRichEdit::SetRichTextFlag
        * @brief     ���ñ�־
        * @param     BOOL fRich -- ��־
        * @return    ����LONG
        *
        * Describe   ���ñ�־
        */
        LONG GetDefaultLeftIndent();

        /**
        * SRichEdit::SetDefaultLeftIndent
        * @brief     ��������
        * @param     LONG lNewIndent -- �����ַ���
        *
        * Describe   ��������
        */
        void SetDefaultLeftIndent(LONG lNewIndent);

        /**
        * SRichEdit::SetSaveSelection
        * @brief     
        * @param     BOOL fSaveSelection
        * @return    ����BOOL
        *
        * Describe   
        */
        BOOL SetSaveSelection(BOOL fSaveSelection);

        /**
        * SRichEdit::SetDefaultTextColor
        * @brief     Ĭ���ı���ɫ
        * @param     COLORREF cr -- ��ɫ
        * @return    ����COLORREF 
        *
        * Describe   ����Ĭ���ı���ɫ   
        */
        COLORREF SetDefaultTextColor(COLORREF cr);
    protected:
        /**
        * SRichEdit::OnCreate
        * @brief    ����
        * @return   LRESULT
        * 
        * Describe  �˺�������Ϣ��Ӧ����
        */
        LRESULT OnCreate(LPVOID);
        /**
        * SRichEdit::OnDestroy
        * @brief    ���ٴ���
        * 
        * Describe  ��Ϣ��Ӧ����
        */ 
        void OnDestroy();
        /**
        * SRichEdit::OnPaint
        * @brief    ������Ϣ
        * @param    IRenderTarget * pRT -- �滭�豸������
        * 
        * Describe  �˺�������Ϣ��Ӧ����
        */
        void OnPaint(IRenderTarget * pRT);
        /**
        * SRichEdit::OnSetFocus
        * @brief    ��ý���
        * 
        * Describe  �˺�������Ϣ��Ӧ����
        */
        void OnSetFocus();
        /**
        * SRichEdit::OnKillFocus
        * @brief    ʧȥ����
        * 
        * Describe  �˺�������Ϣ��Ӧ����
        */
        void OnKillFocus();
        /**
        * SRichEdit::OnTimer
        * @brief    ��ʱ��
        * @param    char idEvent idEvent 
        * 
        * Describe  ��ʱ��
        */
        void OnTimer(char idEvent);
        /**
        * SRichEdit::OnTimer2
        * @brief    ��ʱ��
        * @param    UINT_PTR idEvent 
        *
        * Describe  ��ʱ��
        */
        void OnTimer2(UINT_PTR idEvent);
        /**
        * SRichEdit::OnGetDlgCode
        * @brief    ��ȡ������Ϣ��
        * @return   ����UINT 
        *
        * Describe  ��ȡ������Ϣ��
        */
        virtual UINT OnGetDlgCode()
        {
            UINT uRet=SC_WANTCHARS|SC_WANTARROWS;
            if(m_fWantTab) uRet |= DLGC_WANTTAB;
            if(m_dwStyle&ES_WANTRETURN) uRet |= SC_WANTRETURN;
            return uRet;
        }
        
        /**
        * GetDesiredSize
        * @brief    ��û��ָ�����ڴ�Сʱ��ͨ����Ƥ�����㴰�ڵ�������С
        * @param    LPRECT pRcContainer --  ����λ��
        * @return   CSize 
        *
        * Describe  ע����Ҫ����inset�Ĵ�С
        */
        virtual CSize GetDesiredSize(LPCRECT pRcContainer);

        /**
        * SRichEdit::OnScroll
        * @brief    �������¼�
        * @param    BOOL bVertical -- �Ƿ�ֱ����
        * @param    UINT uCode -- ��Ϣ��
        * @param    int nPos -- λ��
        * @return   ����BOOL
        *
        * Describe  �������¼�
        */
        virtual BOOL OnScroll(BOOL bVertical,UINT uCode,int nPos);
        /**
        * SRichEdit::OnSetCursor
        * @brief    �������λ��
        * @param    const CPoint &pt -- ����
        * @return   ����BOOL
        *
        * Describe  �������λ��
        */
        virtual BOOL OnSetCursor(const CPoint &pt);
        /**
        * SRichEdit::SwndProc
        * @brief    ���ڴ�����
        * @param    UINT uMsg --  ��Ϣ��
        * @param    WPARAM wParam  -- 
        * @param    LPARAM lParam  --
        * @param    LRESULT &lResult -- 
        * @return   ����BOOL
        *
        * Describe  ���ڴ�����
        */
        virtual BOOL SwndProc(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT & lResult);
        /**
        * SRichEdit::InitDefaultCharFormat
        * @brief    
        * @param    CHARFORMAT2W* pcf --
        * @param    IFont *pFont -- 
        * @return   ����HRESULT 
        *
        * Describe  
        */
        HRESULT InitDefaultCharFormat(CHARFORMAT2W* pcf,IFont *pFont=NULL);
        /**
        * SRichEdit::InitDefaultParaFormat
        * @brief    
        * @param    PARAFORMAT2* ppf
        * @return   ����HRESULT
        *
        * Describe  
        */
        HRESULT InitDefaultParaFormat(PARAFORMAT2* ppf);
        /**
        * SRichEdit::OnTxNotify
        * @brief    
        * @param    DWORD iNotify --
        * @param    LPVOID pv  --  
        * @return   ����HRESULT
        *
        * Describe  
        */
        
        virtual HRESULT OnTxNotify(DWORD iNotify,LPVOID pv);
        /**
        * SRichEdit::DefAttributeProc
        * @brief    Ĭ�����Դ�����
        * @param    const SStringW & strAttribName -- ������
        * @param    const SStringW & strValue -- ֵ
        * @param    BOOL bLoading -- �Ƿ����
        *
        * Describe  Ĭ�����Դ�����
        */
        virtual HRESULT DefAttributeProc(const SStringW & strAttribName,const SStringW & strValue, BOOL bLoading);
        
        /**
        * SRichEdit::OnLButtonDown
        * @brief    ��������¼�
        * @param    UINT nFlags -- ��־
        * @param    CPoint point -- �������
        *
        * Describe  �˺�������Ϣ��Ӧ����
        */
        void OnLButtonDown(UINT nFlags, CPoint point);
        
        /**
        * SRichEdit::OnRButtonDown
        * @brief    �Ҽ������¼�
        * @param    UINT nFlags -- ��־
        * @param    CPoint point -- �������
        *
        * Describe  �˺�������Ϣ��Ӧ����
        */
        void OnRButtonDown(UINT nFlags, CPoint point);
        
        /**
        * SRichEdit::OnMouseMove
        * @brief    ����ƶ�
        * @param    UINT nFlags -- ��־
        * @param    CPoint point -- �������
        *
        * Describe  �˺�������Ϣ��Ӧ����
        */
        void OnMouseMove(UINT nFlags, CPoint point);
        
        /**
        * SRichEdit::OnKeyDown
        * @brief    ���̰����¼�
        * @param    UINT nChar -- ������Ӧ����ֵ 
        * @param    UINT nRepCnt -- �ظ�����
        * @param    UINT nFlags -- ��־
        * 
        * Describe  �˺�������Ϣ��Ӧ����
        */ 
        void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
        
        /**
        * SRichEdit::OnButtonClick
        * @brief    ͨ������¼�
        * @param    UINT uMsg -- msg
        * @param    WPARAM wParam -- wParam
        * @param    LPARAM lParam -- lParam
        * @return   ����LRESULT  
        *
        * Describe  �˺�������Ϣ��Ӧ����
        */
        LRESULT OnButtonClick(UINT uMsg,WPARAM wParam,LPARAM lParam);

        /**
        * SRichEdit::OnChar
        * @brief    �ַ��¼�
        * @param    UINT nChar -- ������Ӧ����ֵ 
        * @param    UINT nRepCnt -- �ظ�����
        * @param    UINT nFlags -- ��־
        * 
        * Describe  �˺�������Ϣ��Ӧ����
        */
        void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
        /**
        * SRichEdit::OnSetFont
        * @brief    ��������
        * @param    IFont * font -- ����
        * @param    BOOL bRedraw -- �Ƿ��ػ�
        *
        * Describe  ��������
        */
        void OnSetFont(IFont * font, BOOL bRedraw);
        /**
        * SRichEdit::OnSetText
        * @brief    �����ı�
        * @param    UINT uMsg -- ��Ϣ��
        * @param    WPARAM wparam -- 
        * @param    LPARAM lparam -- 
        * @return   ����LRESULT  
        *
        * Describe  �����ı�
        */
        LRESULT OnSetText(UINT uMsg,WPARAM wparam,LPARAM lparam);
        /**
        * SRichEdit::OnSetCharFormat
        * @brief    
        * @param    UINT uMsg -- ��Ϣ��
        * @param    WPARAM wparam -- 
        * @param    LPARAM lparam -- 
        * @return   ����LRESULT  
        *
        * Describe  
        */
        LRESULT OnSetCharFormat(UINT uMsg, WPARAM wParam, LPARAM lParam);
        /**
        * SRichEdit::OnSetParaFormat
        * @brief    
        * @param    UINT uMsg -- ��Ϣ��
        * @param    WPARAM wparam -- 
        * @param    LPARAM lparam -- 
        * @return   ����LRESULT  
        *
        * Describe  
        */
        LRESULT OnSetParaFormat(UINT uMsg, WPARAM wParam, LPARAM lParam);
        /**
        * SRichEdit::OnSetReadOnly
        * @brief    ����ֻ��
        * @param    UINT uMsg -- ��Ϣ��
        * @param    WPARAM wparam -- 
        * @param    LPARAM lparam -- 
        * @return   ����LRESULT  
        *
        * Describe  ����ֻ��
        */
        LRESULT OnSetReadOnly(UINT uMsg, WPARAM wParam, LPARAM lParam);
        /**
        * SRichEdit::OnSetLimitText
        * @brief    �����ı�����
        * @param    UINT uMsg -- ��Ϣ��
        * @param    WPARAM wparam -- 
        * @param    LPARAM lparam -- 
        * @return   ����LRESULT  
        *
        * Describe  �����ı�����
        */
        LRESULT OnSetLimitText(UINT uMsg, WPARAM wParam, LPARAM lParam);
        /**
        * SRichEdit::OnNcCalcSize
        * @brief    ����ǿͻ���С
        * @param    BOOL bCalcValidRects -- 
        * @param    LPARAM lparam -- 
        * @return   ����LRESULT  
        *
        * Describe  ��Ϣ��Ӧ����
        */
        LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam);
        /**
        * SRichEdit::OnEnableDragDrop
        * @brief    �Ƿ�������ק
        * @param    BOOL bEnable
        *
        * Describe  �Ƿ�������ק
        */
        void OnEnableDragDrop(BOOL bEnable);

    protected:
        SOUI_MSG_MAP_BEGIN()
            MSG_WM_CREATE(OnCreate)
            MSG_WM_DESTROY(OnDestroy)
            MSG_WM_PAINT_EX(OnPaint)
            MSG_WM_NCCALCSIZE(OnNcCalcSize)
            MSG_WM_SETFOCUS_EX(OnSetFocus)
            MSG_WM_KILLFOCUS_EX(OnKillFocus)
            MSG_WM_TIMER_EX(OnTimer)
            MSG_WM_TIMER2(OnTimer2)
            MSG_WM_LBUTTONDOWN(OnLButtonDown)
            MSG_WM_RBUTTONDOWN(OnRButtonDown)
            MSG_WM_MOUSEMOVE(OnMouseMove)
            MSG_WM_KEYDOWN(OnKeyDown)
            MSG_WM_CHAR(OnChar)
            MSG_WM_SETFONT_EX(OnSetFont)
            MESSAGE_HANDLER_EX(WM_LBUTTONDBLCLK,OnButtonClick)
            MESSAGE_HANDLER_EX(WM_LBUTTONUP,OnButtonClick)

            MESSAGE_HANDLER_EX(WM_SETTEXT,OnSetText)
            MESSAGE_HANDLER_EX(EM_SETPARAFORMAT,OnSetParaFormat)
            MESSAGE_HANDLER_EX(EM_SETCHARFORMAT,OnSetCharFormat)
            MESSAGE_HANDLER_EX(EM_SETREADONLY,OnSetReadOnly)
            MESSAGE_HANDLER_EX(EM_EXLIMITTEXT,OnSetLimitText)
        SOUI_MSG_MAP_END()

        SOUI_ATTRS_BEGIN()
            ATTR_INT(L"style",m_dwStyle,FALSE)
            ATTR_INT(L"maxBuf",m_cchTextMost,FALSE)
            ATTR_INT(L"transparent",m_fTransparent,FALSE)
            ATTR_INT(L"rich",m_fRich,FALSE)
            ATTR_INT(L"vertical",m_fVertical,FALSE)
            ATTR_INT(L"wordWrap",m_fWordWrap,FALSE)
            ATTR_INT(L"allowBeep",m_fAllowBeep,FALSE)
            ATTR_INT(L"autoWordSel",m_fEnableAutoWordSel,FALSE)
            ATTR_INT(L"vcenter",m_fSingleLineVCenter,FALSE)
            ATTR_RECT(L"inset",m_rcInsetPixel,FALSE)
            ATTR_CUSTOM(L"colorText",OnAttrTextColor)
            ATTR_CUSTOM(L"rtf",OnAttrRTF)
            ATTR_CUSTOM(L"align",OnAttrAlign)
        SOUI_ATTRS_END()

    protected:
        /**
        * SRichEdit::OnAttrTextColor
        * @brief    �����ı���ɫ
        * @param    const SStringW &  strValue -- �ַ���
        * @param    BOOL bLoading -- �Ƿ����
        * @return   ����HRESULT 
        *
        * Describe  �����ı���ɫ
        */
        HRESULT OnAttrTextColor(const SStringW &  strValue,BOOL bLoading);

        HRESULT OnAttrRTF(const SStringW &  strValue,BOOL bLoading);

        HRESULT OnAttrAlign(const SStringW &  strValue,BOOL bLoading);

        CHARFORMAT2W   m_cfDef;              /**< Default character format  */
        PARAFORMAT2    m_pfDef;              /**< Default paragraph format  */
        DWORD m_cchTextMost;                 /**< Maximize Characters       */
        TCHAR m_chPasswordChar;              /**< Password character        */
        LONG         m_lAccelPos;            /**< Accelerator position      */
        SIZEL        m_sizelExtent;          /**< Extent array              */
        CRect        m_rcInset;              /**< inset margin              */
        CRect        m_rcInsetPixel;         /**< inset margin in pixel     */
        int          m_nFontHeight;          /**< ������������߶�          */
        DWORD        m_dwStyle;

        UINT    m_fEnableAutoWordSel    :1;    /**< enable Word style auto word selection?  */
        UINT    m_fWordWrap            :1;     /**< Whether control should word wrap */
        UINT    m_fRich                :1;     /**< Whether control is rich text */
        UINT    m_fSaveSelection        :1;    /**< Whether to save the selection when inactive */
        UINT    m_fTransparent        :1;      /**< Whether control is transparent */
        UINT    m_fVertical            :1;     /**< Whether control is layout following vertical */
        UINT    m_fAllowBeep        :1;        /**< Whether message beep is allowed in the control */
        UINT    m_fWantTab            :1;      /**< Whether control will deal with tab input */
        UINT    m_fSingleLineVCenter:1;        /**< Whether control that is single line will be vertical centered */
        UINT    m_fScrollPending    :1;        /**< Whether scroll is activated by richedit or by panelex */
        UINT    m_fEnableDragDrop    :1;       /**< �����ڸÿؼ���ʹ���Ϸ� */
        UINT    m_fAutoSel              :1;    /**< �н���ʱ�Զ�ȫѡ */
        
        BYTE    m_byDbcsLeadByte; /**< DBCS����ʱ������ͷ�ֽ�*/
        SStringW m_strRtfSrc;     /**< ��XML��ָ����RTF����Դ*/
        STextHost    *m_pTxtHost; /**< Host of Richedit*/
    };

    /**
    * @class      SEdit 
    * @brief      ��edit�ؼ�
    * 
    * Describe    
    */
    class SOUI_EXP SEdit : public SRichEdit
    {
        SOUI_CLASS_NAME(SEdit, L"edit")
    public:
        /**
        * SEdit::SEdit
        * @brief    ���캯�� 
        *
        * Describe  ���캯��
        */
        SEdit();
        
        SStringT GetCueText() const;
        
        SOUI_ATTRS_BEGIN()
            ATTR_COLOR(L"cueColor",m_crCue,TRUE)
            ATTR_I18NSTRT(L"cueText",m_strCue,TRUE)
        SOUI_ATTRS_END()

    protected:
    
        /**
        * SEdit::OnPaint
        * @brief    ������Ϣ
        * @param    IRenderTarget * pRT -- �滭�豸������
        * 
        * Describe  �˺�������Ϣ��Ӧ����
        */
        void OnPaint(IRenderTarget * pRT);
        /**
        * SEdit::OnSetFocus
        * @brief    ��ý���
        * 
        * Describe  �˺�������Ϣ��Ӧ����
        */
        void OnSetFocus();
        /**
        * SEdit::OnKillFocus
        * @brief    ʧȥ����
        * 
        * Describe  �˺�������Ϣ��Ӧ����
        */
        void OnKillFocus();

        SOUI_MSG_MAP_BEGIN()
            MSG_WM_PAINT_EX(OnPaint)
            MSG_WM_SETFOCUS_EX(OnSetFocus)
            MSG_WM_KILLFOCUS_EX(OnKillFocus)
        SOUI_MSG_MAP_END()

        COLORREF    m_crCue;
        SStringT    m_strCue;
    };
}//namespace SOUI
