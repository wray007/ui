#pragma once

class CFolderHander
{
    struct LANGEXTS
    {
        SStringW strLang;
        SStringW strExts;
    };

public:
    CFolderHander(void);
    ~CFolderHander(void);

    void OnInit(SWindow *pFolderRoot);
    void InitDir(const SStringT & strPath,BOOL bInput=FALSE);
protected:

    void OnBtnFileTypes(EventArgs *pEvt);
    void OnDirEnterFinish(EventArgs *pEvt);
    
    void InitLang(pugi::xml_node xmlNode);
    void InitDirTree(HSTREEITEM hTreeItem,const SStringT & strPath);

    EVENT_MAP_BEGIN()
        EVENT_NAME_HANDLER(L"btn_dropdown_filetypes",EventCmd::EventID, OnBtnFileTypes)
        EVENT_NAME_HANDLER(L"edit_dir",EventKillFocus::EventID,OnDirEnterFinish)
        EVENT_NAME_HANDLER(L"edit_dir",EventKeyEnter::EventID,OnDirEnterFinish)
    EVENT_MAP_BREAK()

    SWindow *   m_pFolderRoot;
    SStringT    m_strDir;
    STreeCtrl * m_pDirTree;

protected:

    typedef SArray<LANGEXTS> LANGEXTLIST;
    LANGEXTLIST m_lstLangExts;
};
