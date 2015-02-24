/**
* Copyright (C) 2014-2050 
* All rights reserved.
* 
* @file       SwndContainer-i.h
* @brief      
* @version    v1.0      
* @author     SOUI group   
* @date       2014/08/02
* 
* Describe    DUI���������ӿ�
*/

#pragma once

#include "event/Events.h"
#include "SMsgLoop.h"
#include "interface/SScriptModule-i.h"

namespace SOUI
{

    struct IAcceleratorMgr;
    
    enum{
    ZORDER_MIN  = 0,
    ZORDER_MAX  = (UINT)-1,
    };
    
    /**
    * @struct     ITimelineHandler
    * @brief      ʱ���ᴦ��ӿ�
    * 
    * Describe    
    */
    struct ITimelineHandler
    {
        virtual void OnNextFrame()=0;
    };

    /**
    * @struct     ISwndContainer
    * @brief      SOUI���������ӿ�
    * 
    * Describe    
    */
    struct ISwndContainer : public ITimelineHandler
    {
        virtual BOOL RegisterDragDrop(SWND swnd,IDropTarget *pDropTarget)=0;

        virtual BOOL RevokeDragDrop(SWND swnd)=0;

        virtual BOOL OnFireEvent(EventArgs &evt)=0;

        virtual HWND GetHostHwnd()=0;

        virtual const SStringW & GetTranslatorContext()=0;

        virtual BOOL IsTranslucent()=0;

        virtual CRect GetContainerRect()=0;

        virtual IRenderTarget * OnGetRenderTarget(const CRect & rc,DWORD gdcFlags)=0;

        virtual void OnReleaseRenderTarget(IRenderTarget *pRT,const CRect &rc,DWORD gdcFlags)=0;

        virtual void OnRedraw(const CRect &rc)=0;

        virtual SWND OnGetSwndCapture()=0;

        virtual BOOL OnReleaseSwndCapture()=0;

        virtual SWND OnSetSwndCapture(SWND swnd)=0;

        virtual void OnSetSwndFocus(SWND swnd)=0;

        virtual SWND SwndGetHover()=0;

        virtual SWND SwndGetFocus()=0;

        virtual BOOL SwndCreateCaret(HBITMAP hBmp,int nWidth,int nHeight)=0;

        virtual BOOL SwndShowCaret(BOOL bShow)=0;

        virtual BOOL SwndSetCaretPos(int x,int y)=0;

        virtual BOOL SwndUpdateWindow()=0;

        virtual IAcceleratorMgr* GetAcceleratorMgr()=0;

        virtual BOOL RegisterTimelineHandler(ITimelineHandler *pHandler)=0;

        virtual BOOL UnregisterTimelineHandler(ITimelineHandler *pHandler)=0;

        virtual BOOL RegisterTrackMouseEvent(SWND swnd)=0;

        virtual BOOL UnregisterTrackMouseEvent(SWND swnd)=0;

        virtual SMessageLoop * GetMsgLoop() =0;

        //��Ǵ�������zorderʧЧ
        virtual void MarkWndTreeZorderDirty() = 0;

        //�ؽ���������zorder
        virtual void BuildWndTreeZorder() = 0;

        virtual IScriptModule * GetScriptModule() = 0;
    };


}//namespace SOUI

