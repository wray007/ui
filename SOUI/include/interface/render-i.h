#pragma once

#include <unknown/obj-ref-i.h>
#include "imgdecoder-i.h"

namespace SOUI
{
    struct IBrush;
    struct IPen;
    struct IFont;
    struct IBitmap;
    struct IRegion;
    struct IRenderTarget;
    struct IRenderFactory;

    /**
    * @struct     IRenderFactory
    * @brief      RenderFactory����
    * 
    * Describe
    */
    struct IRenderFactory : public IObjRef
    {
        virtual IImgDecoderFactory * GetImgDecoderFactory()=0;
        virtual void SetImgDecoderFactory(IImgDecoderFactory *pImgDecoderFac)=0;
        virtual BOOL CreateRenderTarget(IRenderTarget ** ppRenderTarget,int nWid,int nHei)=0;
        virtual BOOL CreateFont(IFont ** ppFont, const LOGFONT &lf)=0;
        virtual BOOL CreateBitmap(IBitmap ** ppBitmap)=0;
        virtual BOOL CreateRegion(IRegion **ppRgn)=0;
    };

    enum OBJTYPE
    {
        OT_NULL=0,
        OT_PEN,
        OT_BRUSH,
        OT_BITMAP,
        OT_FONT,
        OT_RGN,
    };

    /**
    * @struct     IRenderObj
    * @brief      ��Ⱦ�������
    * 
    * Describe    ������Ⱦ����ȫ��ʹ�����ü���������������
    */
    struct IRenderObj : public IObjRef
    {
        /**
         * ObjectType
         * @brief    ��ѯ��������
         * @return   const UINT 
         * Describe  
         */    
        virtual const OBJTYPE ObjectType() const = 0;

        /**
         * GetRenderFactory
         * @brief    ��ô�������Ⱦ������೧
         * @return   IRenderFactory * -- �೧ 
         * Describe  
         */    
        virtual IRenderFactory * GetRenderFactory() const = 0;
    };


    /**
    * @struct     IBrush
    * @brief      ��ˢ����
    * 
    * Describe    
    */
    struct IBrush : public IRenderObj
    {
        virtual const OBJTYPE ObjectType() const
        {
            return OT_BRUSH;
        }
    };

    /**
    * @struct     IPen
    * @brief      ���ʶ���
    * 
    * Describe    
    */
    struct IPen: public IRenderObj
    {
        virtual const OBJTYPE ObjectType() const
        {
            return OT_PEN;
        }
    };

    /**
    * @struct     IBitmap
    * @brief      λͼ����
    * 
    * Describe    
    */
    struct IBitmap: public IRenderObj
    {
        virtual const OBJTYPE ObjectType() const
        {
            return OT_BITMAP;
        }
        /**
         * Init
         * @brief    ��32λ��λͼ���ݳ�ʼ��IBitmap
         * @param    int nWid --  ͼƬ���
         * @param    int nHei --  ͼƬ�߶�
         * @param    const LPVOID pBits --  λͼ����,��ARGB��ʽ�洢
         * @return   HRESULT -- �ɹ�����S_OK,ʧ�ܷ��ش������
         * Describe  
         */    
        virtual HRESULT Init(int nWid,int nHei,const LPVOID pBits=NULL)=0;

        /**
         * Init
         * @brief    ��IImgFrame��ʼ��λͼ
         * @param    IImgFrame * pImgFrame --  IImgFrameָ��
         * @return   HRESULT -- �ɹ�����S_OK,ʧ�ܷ��ش������
         * Describe  
         */    
        virtual HRESULT Init(IImgFrame *pImgFrame) =0;

        /**
         * LoadFromFile
         * @brief    ���ļ��м���λͼ
         * @param    LPCTSTR pszFileName --  �ļ���
         * @return   HRESULT -- �ɹ�����S_OK,ʧ�ܷ��ش������
         * Describe  
         */    
        virtual HRESULT LoadFromFile(LPCTSTR pszFileName)=0;

        /**
         * LoadFromMemory
         * @brief    ���ڴ��м���λͼ
         * @param    LPBYTE pBuf --  �ڴ��ַ
         * @param    size_t szLen --  �ڴ��С
         * @return   HRESULT -- �ɹ�����S_OK,ʧ�ܷ��ش������
         * Describe  
         */    
        virtual HRESULT LoadFromMemory(LPBYTE pBuf,size_t szLen)=0;

        /**
         * Width
         * @brief    ���ͼƬ���
         * @return   UINT -- ͼƬ���
         * Describe  
         */    
        virtual UINT    Width() =0;

        /**
         * Height
         * @brief    ���ͼƬ�߶�
         * @return   UINT -- ͼƬ�߶�
         * Describe  
         */    
        virtual UINT    Height() =0;

        /**
         * Size
         * @brief    ���ͼƬ�߶ȼ����
         * @return   SIZE -- ͼƬ�߶ȼ����
         * Describe  
         */    
        virtual SIZE    Size() =0;

        /**
         * LockPixelBits
         * @brief    ����λͼ������
         * @return   LPVOID -- λͼ���ݵ�ַ
         * Describe  
         */    
        virtual LPVOID  LockPixelBits() =0;

        /**
         * UnlockPixelBits
         * @brief    �����������
         * @param    LPVOID --  ��LockPixelBits���ص�λͼ���ݵ�ַ
         * @return   void
         * Describe  ��LockPixelBits���ʹ��
         */    
        virtual void    UnlockPixelBits(LPVOID) =0;
        
         /**
         * UnlockPixelBits
         * @brief    Save
         * @param    LPCWSTR pszFileName --  File name
         * @param    const LPVOID *pFormat --  image format
         * @return   HRESULT -- S_OK: succeed
         * Describe  
         */    
        virtual HRESULT Save(LPCWSTR pszFileName,const LPVOID pFormat)
        {
            return GetRenderFactory()->GetImgDecoderFactory()->SaveImage(this,pszFileName,pFormat);
        }
    };

    /**
    * @struct     IFont
    * @brief      �������
    * 
    * Describe    
    */
    struct IFont : public IRenderObj
    {
        virtual const OBJTYPE ObjectType() const
        {
            return OT_FONT;
        }

        /**
         * LogFont
         * @brief    ��������LOGFONT
         * @return   const LOGFONT * -- ����������Ϣ��LOGFONT*
         * Describe  
         */    
        virtual const LOGFONT * LogFont() const =0;

        /**
         * FamilyName
         * @brief    ��ȡ������
         * @return   LPCTSTR -- ������
         * Describe  
         */    
        virtual LPCTSTR FamilyName()=0;

        /**
         * TextSize
         * @brief    ��ȡ�����С
         * @return   int -- �����С
         * Describe  
         */    
        virtual int TextSize()=0;

        /**
         * IsBold
         * @brief    ��ѯ�Ƿ�Ϊ����
         * @return   BOOL -- trueΪ���壬false����
         * Describe  
         */    
        virtual BOOL IsBold()=0;

        /**
         * IsUnderline
         * @brief    ��ѯ�����»���״̬
         * @return   BOOL -- true���»��ߣ�false����
         * Describe  
         */    
        virtual BOOL IsUnderline()=0;

        /**
         * IsItalic
         * @brief    ��ѯ�����б��״̬
         * @return   BOOL -- trueΪб�壬false����
         * Describe  
         */    
        virtual BOOL IsItalic()=0;

        /**
         * StrikeOut
         * @brief    ��ѯ�����ɾ����״̬
         * @return   BOOL -- true��ɾ����
         * Describe  
         */    
        virtual BOOL IsStrikeOut() =0;
    };

    /**
    * @struct     IRegion
    * @brief      Region����
    * 
    * Describe    
    */
    struct IRegion : public IRenderObj
    {
        virtual const OBJTYPE ObjectType() const
        {
            return OT_RGN;
        }

        /**
         * CombineRect
         * @brief    ��һ��������this���
         * @param    LPCRECT lprect --  Ҫ��ϵľ���
         * @param    int nCombineMode --  ���ģʽ
         * @return   void
         * Describe  ���ģʽͬWin32 API CombineRect
         */    
        virtual void CombineRect(LPCRECT lprect,int nCombineMode )=0;

        /**
         * PtInRegion
         * @brief    ���һ�����Ƿ���region��Χ��
         * @param    POINT pt --  �����ĵ�
         * @return   BOOL -- true��region��
         * Describe  
         */    
        virtual BOOL PtInRegion(POINT pt)=0;

        /**
         * RectInRegion
         * @brief    ���һ�������Ƿ���this�ཻ
         * @param    LPCRECT lprect --  �����ĵľ��Ρ�
         * @return   BOOL -- true��region��
         * Describe  
         */    
        virtual BOOL RectInRegion(LPCRECT lprect)=0;

        /**
         * GetRgnBox
         * @brief    ���this���������
         * @param [out] LPRECT lprect --  �������
         * @return   void  
         * Describe  
         */    
        virtual void GetRgnBox(LPRECT lprect)=0;

        /**
         * IsEmpty
         * @brief    ��ѯregion�Ƿ�Ϊ��
         * @return   BOOL -- trueΪ��
         * Describe  
         */    
        virtual BOOL IsEmpty()=0;

        /**
         * Offset
         * @brief    ��this����ƽ��
         * @param    POINT pt --  ƽ����x,y����ĵ�λ��
         * @return   void 
         * Describe  
         */    
        virtual void Offset(POINT pt)=0;

        /**
         * Clear
         * @brief    ���region
         * @return   void
         * Describe  
         */    
        virtual void Clear()=0;
    };

    enum EXPEND_MODE
    {
        EM_NULL=0,      //������
        EM_STRETCH,     //����
        EM_TILE,        //ƽ��
    };

    /**
    * @struct     IRenderTarget
    * @brief      RenderTarget����
    * 
    * Describe    ʵ�ָ�λ��Ⱦ�ӿڲ������豸�����Դ
    */
    struct IRenderTarget: public IObjRef
    {
        virtual HRESULT CreateCompatibleRenderTarget(SIZE szTarget,IRenderTarget **ppRenderTarget)=0;
        virtual HRESULT CreatePen(int iStyle,COLORREF cr,int cWidth,IPen ** ppPen)=0;
        virtual HRESULT CreateSolidColorBrush(COLORREF cr,IBrush ** ppBrush)=0;
        virtual HRESULT CreateBitmapBrush( IBitmap *pBmp,IBrush ** ppBrush )=0;

        virtual HRESULT Resize(SIZE sz)=0;

        virtual HRESULT OffsetViewportOrg(int xOff, int yOff, LPPOINT lpPoint=NULL)=0;
        virtual HRESULT GetViewportOrg(LPPOINT lpPoint) =0;
        virtual HRESULT SetViewportOrg(POINT pt) =0;

        virtual HRESULT PushClipRect(LPCRECT pRect,UINT mode=RGN_AND)=0;
        virtual HRESULT PushClipRegion(IRegion *pRegion,UINT mode=RGN_AND)=0;
        virtual HRESULT PopClip()=0;

        virtual HRESULT ExcludeClipRect(LPCRECT pRc)=0;
        virtual HRESULT IntersectClipRect(LPCRECT pRc)=0;

        virtual HRESULT SaveClip(int *pnState)=0;
        virtual HRESULT RestoreClip(int nState=-1)=0;

        virtual HRESULT GetClipRegion(IRegion **ppRegion)=0;
        virtual HRESULT GetClipBox(LPRECT prc)=0;

        virtual HRESULT DrawText(LPCTSTR pszText,int cchLen,LPRECT pRc,UINT uFormat)=0;
        virtual HRESULT MeasureText(LPCTSTR pszText,int cchLen, SIZE *psz) =0;
        virtual HRESULT TextOut(int x,int y, LPCTSTR lpszString,int nCount) =0;

        virtual HRESULT DrawRectangle(LPCRECT pRect)=0;
        virtual HRESULT FillRectangle(LPCRECT pRect)=0;
        virtual HRESULT FillSolidRect(LPCRECT pRect,COLORREF cr)=0;
        virtual HRESULT DrawRoundRect(LPCRECT pRect,POINT pt)=0;
        virtual HRESULT FillRoundRect(LPCRECT pRect,POINT pt)=0;
        virtual HRESULT ClearRect(LPCRECT pRect,COLORREF cr)=0;
        virtual HRESULT DrawEllipse(LPCRECT pRect)=0;
        virtual HRESULT FillEllipse(LPCRECT pRect)=0;

        virtual HRESULT DrawArc(LPCRECT pRect,float startAngle,float sweepAngle,bool useCenter) =0;
        virtual HRESULT FillArc(LPCRECT pRect,float startAngle,float sweepAngle) =0;

        virtual HRESULT DrawLines(LPPOINT pPt,size_t nCount) =0;
        virtual HRESULT GradientFill(LPCRECT pRect,BOOL bVert,COLORREF crBegin,COLORREF crEnd,BYTE byAlpha=0xFF)=0;
        virtual HRESULT GradientFillEx( LPCRECT pRect,const POINT* pts,COLORREF *colors,float *pos,int nCount,BYTE byAlpha=0xFF )=0;

        virtual HRESULT DrawIconEx(int xLeft, int yTop, HICON hIcon, int cxWidth,int cyWidth,UINT diFlags)=0;
        virtual HRESULT DrawBitmap(LPCRECT pRcDest,IBitmap *pBitmap,int xSrc,int ySrc,BYTE byAlpha=0xFF)=0;
        virtual HRESULT DrawBitmapEx(LPCRECT pRcDest,IBitmap *pBitmap,LPCRECT pRcSrc,EXPEND_MODE expendMode, BYTE byAlpha=0xFF)=0;
        virtual HRESULT DrawBitmap9Patch(LPCRECT pRcDest,IBitmap *pBitmap,LPCRECT pRcSrc,LPCRECT pRcSourMargin,EXPEND_MODE expendMode,BYTE byAlpha=0xFF) =0;
        virtual HRESULT BitBlt(LPCRECT pRcDest,IRenderTarget *pRTSour,int xSrc,int ySrc,DWORD dwRop=SRCCOPY)=0;
        virtual HRESULT AlphaBlend(LPCRECT pRcDest,IRenderTarget *pRTSrc,LPCRECT pRcSrc,BYTE byAlpha) =0;
        virtual IRenderObj * GetCurrentObject(OBJTYPE uType) =0;
        //��ָ����RenderObj�ָ�ΪĬ��״̬
        virtual HRESULT SelectDefaultObject(OBJTYPE objType, IRenderObj ** pOldObj = NULL) =0;
        virtual HRESULT SelectObject(IRenderObj *pObj,IRenderObj ** pOldObj = NULL) =0;
        virtual COLORREF GetTextColor() =0;
        virtual COLORREF SetTextColor(COLORREF color)=0;

        //��������GDI�����Ľӿ�
        virtual HDC GetDC(UINT uFlag=0)=0;
        virtual void ReleaseDC(HDC hdc) =0;
        
        /**
         * QueryInterface
         * @brief    �ṩ�ӿ���չ���õķ���
         * @param    REFGUID iid --  ����չ�ӿ�ID
         * @param    IObjRef * * ppObj --  �ӿ����
         * @return   HRESULT -- �ɹ�����S_OK
         *
         * Describe  �����ܻ�ȡʲô�ӿ������ڲ�ͬ����Ⱦ����
         */
        virtual HRESULT QueryInterface(REFGUID iid,IObjRef ** ppObj) =0;
    };


}//end of namespace SOUI
