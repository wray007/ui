/**
* Copyright (C) 2014-2050 
* All rights reserved.
* 
* @file       SResProvider-i.h
* @brief      
* @version    v1.0      
* @author     SOUI group   
* @date       2014/08/02
* 
* Describe    
*/

#ifndef _SRESPROVIDERBASE_
#define _SRESPROVIDERBASE_
#pragma once

#define OR_API SOUI_EXP

#include <unknown/obj-ref-i.h>
#include "render-i.h"


#define UIRES_INDEX    _T("uires.idx")        //�ļ�����Դ���ļ�ӳ����������ļ���

namespace SOUI
{
    enum BUILTIN_RESTYPE
    {
        RES_PE=0,
        RES_FILE,
    };

    /**
    * @struct     IResProvider
    * @brief      ResProvider����
    * 
    * Describe  ʵ�ָ�����Դ�ļ���
    */
    struct IResProvider : public IObjRef
    {
        /**
         * Init
         * @brief    ��Դ��ʼ������
         * @param    WPARAM wParam --  param 1 
         * @param    LPARAM lParam --  param 2
         * @return   BOOL -- true:succeed
         *
         * Describe  every Resprovider must implement this interface.
         */
        virtual BOOL Init(WPARAM wParam,LPARAM lParam) =0;
        
        /**
         * HasResource
         * @brief    ��ѯһ����Դ�Ƿ����
         * @param    LPCTSTR strType --  ��Դ����
         * @param    LPCTSTR pszResName --  ��Դ����
         * @return   BOOL -- true���ڣ�false������
         * Describe  
         */    
        virtual BOOL HasResource(LPCTSTR pszType,LPCTSTR pszResName)=0;

        /**
         * LoadIcon
         * @brief    ����Դ�м���ICON
         * @param    LPCTSTR pszResName --  ICON����
         * @param    int cx --  ICON���
         * @param    int cy --  ICON�߶�
         * @return   HICON -- �ɹ�����ICON�ľ����ʧ�ܷ���0
         * Describe  
         */    
        virtual HICON LoadIcon(LPCTSTR pszResName,int cx=0,int cy=0)=0;

        /**
         * LoadBitmap
         * @brief    ����Դ�м���HBITMAP
         * @param    LPCTSTR pszResName --  BITMAP����
         * @return   HBITMAP -- �ɹ�����BITMAP�ľ����ʧ�ܷ���0
         * Describe  
         */    
        virtual HBITMAP LoadBitmap(LPCTSTR pszResName)=0;

        /**
         * LoadCursor
         * @brief    ����Դ�м��ع��
         * @param    LPCTSTR pszResName --  �����
         * @return   HCURSOR -- �ɹ����ع��ľ����ʧ�ܷ���0
         * Describe  ֧�ֶ������
         */    
        virtual HCURSOR LoadCursor(LPCTSTR pszResName)=0;

        /**
         * LoadImage
         * @brief    ����Դ����һ��IBitmap����
         * @param    LPCTSTR strType --  ͼƬ����
         * @param    LPCTSTR pszResName --  ͼƬ��
         * @return   IBitmap * -- �ɹ�����һ��IBitmap����ʧ�ܷ���0
         * Describe  ���û�ж���strType�������nameʹ��FindImageType�Զ�����ƥ�������
         */    
        virtual IBitmap * LoadImage(LPCTSTR pszType,LPCTSTR pszResName)=0;

        /**
         * LoadImgX
         * @brief    ����Դ�д���һ��IImgX����
         * @param    LPCTSTR strType --  ͼƬ����
         * @param    LPCTSTR pszResName --  ͼƬ��
         * @return   IImgX   * -- �ɹ�����һ��IImgX����ʧ�ܷ���0
         * Describe  
         */    
        virtual IImgX   * LoadImgX(LPCTSTR pszType,LPCTSTR pszResName)=0;

        /**
         * GetRawBufferSize
         * @brief    �����Դ���ݴ�С
         * @param    LPCTSTR strType --  ��Դ����
         * @param    LPCTSTR pszResName --  ��Դ��
         * @return   size_t -- ��Դ��С��byte)��ʧ�ܷ���0
         * Describe  
         */    
        virtual size_t GetRawBufferSize(LPCTSTR pszType,LPCTSTR pszResName)=0;

        /**
         * GetRawBuffer
         * @brief    �����Դ�ڴ��
         * @param    LPCTSTR strType --  ��Դ����
         * @param    LPCTSTR pszResName --  ��Դ��
         * @param    LPVOID pBuf --  ����ڴ��
         * @param    size_t size --  �ڴ��С
         * @return   BOOL -- true�ɹ�
         * Describe  Ӧ������GetRawBufferSize��ѯ��Դ��С�ٷ����㹻�ռ�
         */    
        virtual BOOL GetRawBuffer(LPCTSTR pszType,LPCTSTR pszResName,LPVOID pBuf,size_t size)=0;
    };

}//namespace SOUI
#endif//_SRESPROVIDERBASE_
