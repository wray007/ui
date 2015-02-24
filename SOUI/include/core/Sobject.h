/**
* Copyright (C) 2014-2050 
* All rights reserved.
* 
* @file       Sobject.h
* @brief      
* @version    v1.0      
* @author     SOUI group   
* @date       2014/08/01
* 
* Describe    the base class used in SOUI, which provides type identify of class in runtime
*             and implement attributes dispatcher described in XML. 
*/

#pragma once
#include <trace.h>

//////////////////////////////////////////////////////////////////////////

// SObject Class Name Declaration
#define SOUI_CLASS_NAME(theclass, classname)            \
public:                                                 \
    static LPCWSTR GetClassName()                       \
    {                                                   \
        return classname;                               \
    }                                                   \
    \
    static LPCWSTR BaseClassName()                      \
    {                                                   \
        return __super::GetClassName();                 \
    }                                                   \
    \
    virtual LPCWSTR GetObjectClass()                    \
    {                                                   \
        return classname;                               \
    }                                                   \
    \
    virtual BOOL IsClass(LPCWSTR lpszName)              \
    {                                                   \
        if(wcscmp(GetClassName(), lpszName)  == 0)      \
            return TRUE;                                \
        return __super::IsClass(lpszName);              \
    }                                                   \


namespace SOUI
{

    /**
    * @class      SObject
    * @brief      SOUIϵͳ�еĶ������
    * 
    * Describe    �ṩ��RTTI���ƣ�ʵ�ִ�XML�ڵ��и������������������
    */
    class SOUI_EXP SObject
    {
    public:
        SObject()
        {
        }

        virtual ~SObject()
        {
        }

        /**
         * GetClassName
         * @brief    �������
         * @return   LPCWSTR -- ����
         * Describe  ��̬����
         */    
        static LPCWSTR GetClassName()
        {
            return NULL;
        }

        /**
         * BaseClassName
         * @brief    ��û��������
         * @return   LPCWSTR -- ���������
         * Describe  
         */    
        static LPCWSTR BaseClassName()
        {
            return NULL;
        }

        /**
         * IsClass
         * @brief    �ж�this�ǲ�������ָ��������
         * @param    LPCWSTR lpszName --  ����������
         * @return   BOOL -- true�ǲ�������
         * Describe  
         */    
        virtual BOOL IsClass(LPCWSTR lpszName)
        {
            return FALSE;
        }

        /**
         * GetObjectClass
         * @brief    ���������
         * @return   LPCWSTR -- ������
         * Describe  ����һ���麯����ע����GetClassName������
         */    
        virtual LPCWSTR GetObjectClass()
        {
            return NULL;
        }

        /**
         * InitFromXml
         * @brief    ��XML�ڵ��г�ʼ����������
         * @param    pugi::xml_node xmlNode --  XML�ڵ�����
         * @return   BOOL -- true�ɹ�
         * Describe  
         */    
        virtual BOOL InitFromXml(pugi::xml_node xmlNode);

        /**
         * SetAttribute
         * @brief    ����һ����������
         * @param    const SStringA & strAttribName --  ������
         * @param    const SStringA & strValue --  ����ֵ
         * @param    BOOL bLoading --  ���󴴽�ʱ��ϵͳ���ñ�־
         * @return   HRESULT -- ��������
         * Describe  
         */    
        virtual HRESULT SetAttribute(const SStringA &  strAttribName, const SStringA &  strValue, BOOL bLoading)
        {
            return SetAttribute(S_CA2W(strAttribName),S_CA2W(strValue),bLoading);
        }

        /**
        * SetAttribute
        * @brief    ����һ����������
        * @param    const SStringA & strAttribName --  ������
        * @param    const SStringA & strValue --  ����ֵ
        * @param    BOOL bLoading --  ���󴴽�ʱ��ϵͳ���ñ�־
        * @return   HRESULT -- ��������
        * Describe  
        */    
        virtual HRESULT SetAttribute(const SStringW &  strAttribName, const SStringW &  strValue, BOOL bLoading)
        {
            return DefAttributeProc(strAttribName,strValue,bLoading);
        }

        /**
         * DefAttributeProc
         * @brief    Ĭ�ϵ����Դ���ӿ�
         * @param    const SStringA & strAttribName --  ������
         * @param    const SStringA & strValue --  ����ֵ
         * @param    BOOL bLoading --  ���󴴽�ʱ��ϵͳ���ñ�־
         * @return   HRESULT -- ��������
         * Describe  
         */    
        virtual HRESULT DefAttributeProc(const SStringW & strAttribName,const SStringW & strValue, BOOL bLoading)
        {
            STraceW(L"warning!!! unhandled attribute %s in class %s, value = %s",strAttribName,GetObjectClass(),strValue);
            return E_FAIL;
        }

        /**
         * OnAttribute
         * @brief    ���Դ������õķ���
         * @param    const SStringW & strAttribName --  ������
         * @param    const SStringW & strValue --  ������
         * @param    HRESULT hr --  ���Դ�����
         * @return   HRESULT -- ���Դ�����
         * Describe  ��������ֱ�ӷ���
         */    
        virtual HRESULT AfterAttribute(const SStringW & strAttribName,const SStringW & strValue,HRESULT hr)
        {
            return hr;
        }

        /**
         * tr
         * @brief    ���Է���ӿ�
         * @param    const SStringW & strSrc --  Դ�ַ���
         * @return   SStringW -- �������ַ���
         * Describe  
         */    
        virtual SStringW tr(const SStringW &strSrc);

        /**
         * GetID
         * @brief    ��ȡ����ID
         * @return   int -- ����ID
         * Describe  
         */    
        virtual int GetID() const {return 0;}

        /**
         * GetName
         * @brief    ��ȡ����Name
         * @return   LPCWSTR -- ����Name
         * Describe  
         */    
        virtual LPCWSTR GetName() const {return NULL;}
    protected:
        /**
         * OnInitFinished
         * @brief    ���Գ�ʼ����ɴ���ӿ�
         * @param    pugi::xml_node xmlNode --  ���Խڵ�
         * @return   void
         * Describe  
         */    
        virtual void OnInitFinished(pugi::xml_node xmlNode) {}

#ifdef    _DEBUG
    public:
        SStringW m_strXml;  //<** XML�ַ����������ڵ���ʱ�۲����*/
#endif//_DEBUG
    };

    /**
     * sobj_cast
     * @brief    SOUI Object �����Ͱ�ȫ������ת���ӿ�
     * @param    SObject * pObj --  Դ����
     * @return   T * -- ת����Ķ���
     * Describe  ���Դ�����Ǵ�ת���������ͣ�����NULL
     */    
    template<class T>
    T * sobj_cast(SObject *pObj)
    {
        if(!pObj)
            return NULL;

        if(pObj->IsClass(T::GetClassName()))
            return (T*)pObj;
        else
            return NULL;
    }

}//namespace SOUI
