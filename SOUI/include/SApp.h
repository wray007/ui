/**
 * Copyright (C) 2014-2050 SOUI�Ŷ�
 * All rights reserved.
 * 
 * @file       stabctrl.h
 * @brief      
 * @version    v1.0      
 * @author     soui      
 * @date       2014-07-06
 * 
 * Describe    SOUIӦ�ó������ 
 */

#pragma once
#include "core/ssingleton.h"
#include "unknown/obj-ref-impl.hpp"
#include "interface/render-i.h"
#include "interface/SScriptModule-i.h"
#include "interface/STranslator-i.h"
#include "interface/stooltip-i.h"
#include "control/RealWndHandler-i.h"

#include "res.mgr/SResProviderMgr.h"

#include "core/smsgloop.h"
#include "core/SWndFactoryMgr.h"
#include "core/SSkinFactoryMgr.h"

#define GETRESPROVIDER      SApplication::getSingletonPtr()
#define GETRENDERFACTORY    SApplication::getSingleton().GetRenderFactory()
#define GETREALWNDHANDLER   SApplication::getSingleton().GetRealWndHander()
#define GETTOOLTIPFACTORY   SApplication::getSingleton().GetToolTipFactory()

#define LOADXML(p1,p2,p3)   SApplication::getSingleton().LoadXmlDocment(p1,p2,p3)
#define LOADIMAGE(p1,p2)    SApplication::getSingleton().LoadImage(p1,p2)
#define LOADIMAGE2(p1)      SApplication::getSingleton().LoadImage2(p1)
#define LOADICON(p1,p2)     SApplication::getSingleton().LoadIcon(p1,p2,p2)
#define LOADICON2(p1)       SApplication::getSingleton().LoadIcon2(p1)
#define TR(p1,p2)           SApplication::getSingleton().GetTranslator()->tr(p1,p2)
#define STR2ID(p1)          SNamedID::getSingleton().String2ID(p1)

#define RT_UIDEF _T("UIDEF")
#define RT_LAYOUT _T("LAYOUT")

namespace SOUI
{

/** 
 * @class     SApplication
 * @brief     SOUI Application
 *
 * Describe   SOUI Application
 */
class SOUI_EXP SApplication :public SSingleton<SApplication>
                        ,public SWindowFactoryMgr
                        ,public SSkinFactoryMgr
                        ,public SResProviderMgr
                        ,public SMessageLoop
{
public:
    /**
     * SApplication
     * @brief    ���캯��
     * @param    IRenderFactory * pRendFactory --  ��Ⱦģ��
     * @param    HINSTANCE hInst --  Ӧ�ó�����
     * @param    LPCTSTR pszHostClassName --  ʹ��SOUI��������ʱĬ�ϵĴ�������
     *
     * Describe  
     */
    SApplication(IRenderFactory *pRendFactory,HINSTANCE hInst,LPCTSTR pszHostClassName=_T("SOUIHOST"));

    ~SApplication(void);


    /**
     * GetInstance
     * @brief    ���Ӧ�ó�����
     * @return   HINSTANCE 
     *
     * Describe  
     */
    HINSTANCE GetInstance();

    /**
     * Init
     * @brief    ��ʼ��SOUIϵͳ
     * @param    LPCTSTR pszName --  ��ʼ��SOUI��XML�ļ�����Դ�е�name
     * @param    LPCTSTR pszType --  ��ʼ��SOUI��XML�ļ�����Դ�е�type
     * @return   BOOL true-��ʼ���ɹ�, false-��ʼ��ʧ��
     *
     * Describe  ��ʼ����XML��������SOUI�ĸ�ʽ��
     */
    BOOL Init(LPCTSTR pszName ,LPCTSTR pszType=RT_UIDEF);

    /**
     * LoadSystemNamedResource
     * @brief    ����SOUIϵͳĬ�ϵ�������Դ
     * @param    IResProvider * pResProvider --  
     * @return   UINT 
     *
     * Describe  
     */
    UINT LoadSystemNamedResource(IResProvider *pResProvider);
    
    /**
     * LoadXmlDocment
     * @brief    ����Դ�м���һ��XML Document��
     * @param [out] pugi::xml_document & xmlDoc --  �����xml_document����
     * @param    LPCTSTR pszXmlName --  XML�ļ�����Դ�е�name
     * @param    LPCTSTR pszType --  XML�ļ�����Դ�е�type
     * @return   BOOL true-���سɹ�, false-����ʧ��
     *
     * Describe  
     */
    BOOL LoadXmlDocment(pugi::xml_document & xmlDoc,LPCTSTR pszXmlName ,LPCTSTR pszType);

    /**
     * GetRenderFactory
     * @brief    ��õ�ǰ����Ⱦģ��
     * @return   IRenderFactory * ��Ⱦģ��ָ��
     *
     * Describe  
     */
    IRenderFactory * GetRenderFactory();
    
    /**
     * GetScriptModule
     * @brief    �����ű�ģ�����
     * @param [out] IScriptModule **ppScriptModule -- �ű�ģ�����
     * @return   HRESULT -- S_OK �����ɹ�
     *
     * Describe  
     */
    HRESULT CreateScriptModule(IScriptModule **ppScriptModule);

    /**
     * SetScriptModule
     * @brief    ����SOUI��ʹ�õĽű�ģ���೧
     * @param    IScriptFactory *pScriptModule --  �ű�ģ���೧
     * @return   void 
     *
     * Describe  
     */
    void SetScriptFactory(IScriptFactory *pScriptModule);
    
    /**
     * GetTranslator
     * @brief    ��ȡ���Է���ģ��
     * @return   ITranslator * ���Է���ģ��ָ��
     *
     * Describe  
     */
    ITranslatorMgr * GetTranslator();
    
    /**
     * SetTranslator
     * @brief    �������Է���ģ��
     * @param    ITranslator * pTrans --  ���Է���ģ��ָ��
     * @return   void 
     *
     * Describe  
     */
    void SetTranslator(ITranslatorMgr * pTrans);
    
    /**
     * GetRealWndHander
     * @brief    ���RealWndHander
     * @return   IRealWndHandler * -- RealWndHander
     * Describe  
     */    
    IRealWndHandler * GetRealWndHander();

    /**
     * SetRealWndHandler
     * @brief    ����RealWnd����ӿ�
     * @param    IRealWndHandler * pRealHandler --  RealWnd����ӿ�
     * @return   void
     * Describe  
     */    
    void SetRealWndHandler(IRealWndHandler *pRealHandler);

    /**
     * GetToolTipFactory
     * @brief    ��ȡToolTip����ӿ�
     * @return   IToolTipFactory * -- ToolTip����ӿ�
     * Describe  
     */    
    IToolTipFactory * GetToolTipFactory();

    /**
     * SetToolTipFactory
     * @brief    ����ToolTip����ӿ�
     * @param    IToolTipFactory * pToolTipFac --  ToolTip����ӿ�
     * @return   void -- 
     * Describe  
     */    
    void SetToolTipFactory(IToolTipFactory* pToolTipFac);

    /**
     * Run
     * @brief    ����SOUI������Ϣѭ��
     * @param    HWND hMainWnd --  Ӧ�ó��������ھ��
     * @return   int ��Ϣѭ������ʱ�ķ���ֵ
     *
     * Describe  
     */
    int Run(HWND hMainWnd);

    void SetAppDir(const SStringT & strAppDir){m_strAppDir = strAppDir;}

    SStringT GetAppDir()const{return m_strAppDir;}
protected:
    void _CreateSingletons();
    void _DestroySingletons();
    BOOL _LoadXmlDocment(LPCTSTR pszXmlName ,LPCTSTR pszType ,pugi::xml_document & xmlDoc);
    
    CAutoRefPtr<IRealWndHandler>    m_pRealWndHandler;
    CAutoRefPtr<IScriptFactory>     m_pScriptFactory;
    CAutoRefPtr<IRenderFactory>     m_RenderFactory;
    CAutoRefPtr<ITranslatorMgr>     m_translator;
    CAutoRefPtr<IToolTipFactory>    m_tooltipFactory;

    SStringT    m_strAppDir;
    HINSTANCE   m_hInst;
};

}//namespace SOUI