/*
* This file is part of FBIde, an open-source (cross-platform) IDE for 
* FreeBasic compiler.
* Copyright (C) 2005  Albert Varaksin
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact e-mail: Albert Varaksin <vongodric@hotmail.com>
* Program URL   : http://fbide.sourceforge.net
*/

#include "inc/main.h"
//#include "inc/tabctrl.h"
#include "inc/fbedit.h"


//------------------------------------------------------------------------------
//Load menu's
void MyFrame::LoadUI () {

    LoadToolBar();
    LoadMenu();
    LoadStatusBar();

    FB_App->SetTopWindow(this);
    
    FBCodePanel = new wxPanel(this, wxID_ANY,
        wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN);
    FBCodePanel->SetBackgroundColour(wxSystemSettings::GetColour( wxSYS_COLOUR_APPWORKSPACE ));

    int style = wxNO_FULL_REPAINT_ON_RESIZE|wxCLIP_CHILDREN|wxBC_DEFAULT;
    FBNotebook = new wxNotebook( FBCodePanel, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, style);

    s_Console = new wxBoxSizer(wxVERTICAL);
    FBConsole = new wxListCtrl(FBCodePanel, 
                               wxID_ANY, 
                               wxDefaultPosition, 
                               wxDefaultSize, 
                               wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_HRULES|wxLC_VRULES );

    wxFont LbFont (10, wxMODERN, wxNORMAL, wxNORMAL, false);
	FBConsole->SetFont(LbFont);

    wxListItem itemCol;
    itemCol.SetText(_T("Line"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
    FBConsole->InsertColumn(0, itemCol);

    itemCol.SetText(_T("File"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
    FBConsole->InsertColumn(1, itemCol);
    
    itemCol.SetText(_T("Error nr"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
    FBConsole->InsertColumn(2, itemCol);

    itemCol.SetText(_T("Message"));
    itemCol.SetAlign(wxLIST_FORMAT_LEFT);
    FBConsole->InsertColumn(3, itemCol);
    
    
    FBConsole->SetColumnWidth( 0, 60 );
    FBConsole->SetColumnWidth( 1, 150 );
    FBConsole->SetColumnWidth( 2, 60 );
    FBConsole->SetColumnWidth( 3, 600 );
    
    s_Console->Add(FBConsole, 1, wxGROW | wxALL, 0);
    s_Console->SetMinSize(400, 150);
    FBConsole->Hide();

    s_Code = new wxBoxSizer(wxVERTICAL);
    s_Code->Add(FBNotebook, 3, wxGROW | wxALL, 0);
    
    

    int w, h;
    GetClientSize(&w, &h);
    FBCodePanel->SetSizer(s_Code);
    
    s_Code->Fit(this);
    s_Code->SetSizeHints(this);
    
    SetClientSize(w, h);
    SetMinSize(wxSize(400, 400));
    
    FBNotebook->Hide();
    stc=0;

    Show(true);    
    return;

}



void MyFrame::LoadMenu () {

    MenuBar = new wxMenuBar();
    
    //File
    FB_File = new wxMenu;
    FB_File->Append (Menu_New, _T(Lang[11]), _T(Lang[12]));
    FB_File->Append (Menu_Open, _T(Lang[13]), _T(Lang[14]));
    
    FB_File->AppendSeparator();
    FB_File->Append (Menu_Save,	_T(Lang[15]), _T(Lang[16]));
    FB_File->Append (Menu_SaveAS, _T(Lang[17]), _T(Lang[18]));
    FB_File->Append (Menu_SaveAll, _T(Lang[19]), _T(Lang[20]));
    
    FB_File->AppendSeparator();
    FB_File->Append (Menu_Close, _T(Lang[21]), _T(Lang[22]));
    FB_File->Append (Menu_CloseAll, "Close All", "Close all opened files");

    FB_File->AppendSeparator();
    FB_File->Append (Menu_NewEditor, _T(Lang[23]), _T(Lang[24]));
    FB_File->Append (Menu_Quit, _T(Lang[25]), _T(Lang[26]));



    // Edit menu
    _FB_Edit = new wxMenu;
    _FB_Edit->Append (Menu_Undo, 	_(Lang[27]), _(Lang[28]));
    _FB_Edit->Append (Menu_Redo, 	_(Lang[29]), _(Lang[30]));
    _FB_Edit->AppendSeparator();
    
    _FB_Edit->Append (Menu_Cut, 		_(Lang[31]), _(Lang[32]));
    _FB_Edit->Append (Menu_Copy, 	_(Lang[33]), _(Lang[34]));
    _FB_Edit->Append (Menu_Paste, 	_(Lang[35]), _(Lang[36]));
    _FB_Edit->AppendSeparator();

    _FB_Edit->Append (Menu_SelectAll,	_(Lang[37]), _(Lang[38]));
    _FB_Edit->Append (Menu_SelectLine,	_(Lang[39]), _(Lang[40]));
    _FB_Edit->AppendSeparator();

    _FB_Edit->Append (Menu_IndentIncrease, _(Lang[41]), _(Lang[42]));
    _FB_Edit->Append (Menu_IndentDecrease, _(Lang[43]), _(Lang[44]));

    _FB_Edit->AppendSeparator();
    _FB_Edit->Append (Menu_Comment,      _(Lang[45]), _(Lang[46]));
    _FB_Edit->Append (Menu_UnComment,    _(Lang[47]), _(Lang[48]));


    // Search menu
    FB_Search = new wxMenu;
    FB_Search->Append (Menu_Find,	  _(Lang[49]), _(Lang[50]));
    FB_Search->Append (Menu_FindNext, _(Lang[51]), _(Lang[52]));
    FB_Search->Append (Menu_Replace,  _(Lang[53]), _(Lang[54]));
    FB_Search->Append (Menu_GotoLine, _(Lang[55]), _(Lang[56]));



    // View menu
    FB_View = new wxMenu;
    FB_View->Append          (Menu_Settings, _(Lang[57]), _(Lang[58]));
    FB_View->AppendCheckItem (Menu_Result, _(Lang[59]), _(Lang[60]));
    FB_View->Append (Menu_Subs, _(Lang[61]), _(Lang[62]));
    //FB_Tools->Append (Menu_Converter, _(Language.ToolsConverter), _(Language.ToolsConverterDesc));


    //Run menu
    FB_Run = new wxMenu;
    FB_Run->Append (Menu_Compile,       _(Lang[63]), _(Lang[64]));
    FB_Run->Append (Menu_CompileAndRun, _(Lang[65]), _(Lang[66]));
    FB_Run->Append (Menu_Run,           _(Lang[67]), _(Lang[68]));
    FB_Run->Append (Menu_QuickRun,      _(Lang[69]), _(Lang[70]));
    FB_Run->Append (Menu_CmdPromt,      _(Lang[71]), _(Lang[72]));
    FB_Run->Append (Menu_Parameters,    _(Lang[73]), _(Lang[74]));
//    FB_Run->Append (Menu_CompParam,     _(Lang[75]), _(Lang[76]));
    FB_Run->AppendCheckItem (Menu_ShowExitCode, _(Lang[77]), _(Lang[78]));
	FB_Run->Check  (Menu_ShowExitCode,  Prefs.ShowExitCode);


    //Help
    HelpMenu = new wxMenu;
    HelpMenu->Append(Menu_About,   _T(Lang[79]),    _T(Lang[80]));


    //Implement menus
    MenuBar->Append(FB_File,  _T(Lang[4]));
    MenuBar->Append(_FB_Edit,  _T(Lang[5]));
    MenuBar->Append(FB_Search,_T(Lang[6]));
    MenuBar->Append(FB_View,  _T(Lang[7]));
//    MenuBar->Append(FB_Tools, _T(Lang[8]));
    MenuBar->Append(FB_Run,   _T(Lang[9]));
    MenuBar->Append(HelpMenu, _T(Lang[10]));
    
    SetMenuBar(MenuBar);
    return;
}



//------------------------------------------------------------------------------
// Load toolbar
void MyFrame::LoadToolBar () {
	
//    FB_Toolbar = GetToolBar();
    FB_Toolbar = CreateToolBar();
    
    //Add controls:
    wxBitmap toolBarBitmaps[15];
    toolBarBitmaps[0] = wxBITMAP(new);
    toolBarBitmaps[1] = wxBITMAP(open);
    toolBarBitmaps[2] = wxBITMAP(save);
    toolBarBitmaps[3] = wxBITMAP(cut);
    toolBarBitmaps[4] = wxBITMAP(copy);
    toolBarBitmaps[5] = wxBITMAP(paste);
    toolBarBitmaps[6] = wxBITMAP(undo);
    toolBarBitmaps[7] = wxBITMAP(redo);
    toolBarBitmaps[8] = wxBITMAP(compile);
    toolBarBitmaps[9] = wxBITMAP(run);
    toolBarBitmaps[10]= wxBITMAP(compnrun);
    toolBarBitmaps[11]= wxBITMAP(qrun);
    toolBarBitmaps[12]= wxBITMAP(saveall);
    toolBarBitmaps[13]= wxBITMAP(close);
    toolBarBitmaps[14]= wxBITMAP(output);
    
    FB_Toolbar->AddTool(Menu_New, toolBarBitmaps[0], wxNullBitmap, FALSE, 100, 100, (wxObject *) NULL, _(Lang[83]));
    FB_Toolbar->AddTool(Menu_Open, toolBarBitmaps[1], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[84]));
    FB_Toolbar->AddTool(Menu_Save, toolBarBitmaps[2], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[85]));
    FB_Toolbar->AddTool(Menu_SaveAll, toolBarBitmaps[12], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[86]));
    FB_Toolbar->AddTool(Menu_Close, toolBarBitmaps[13], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[87]));
    FB_Toolbar->AddSeparator();
    FB_Toolbar->AddTool(Menu_Cut, toolBarBitmaps[3], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[88]));
    FB_Toolbar->AddTool(Menu_Copy, toolBarBitmaps[4], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[89]));
    FB_Toolbar->AddTool(Menu_Paste, toolBarBitmaps[5], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[90]));
    FB_Toolbar->AddSeparator();
    FB_Toolbar->AddTool(Menu_Undo, toolBarBitmaps[6], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[91]));
    FB_Toolbar->AddTool(Menu_Redo, toolBarBitmaps[7], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[92]));
    FB_Toolbar->AddSeparator();
    FB_Toolbar->AddTool(Menu_Compile, toolBarBitmaps[8], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[93]));
    FB_Toolbar->AddTool(Menu_Run, toolBarBitmaps[9], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[94]));
    FB_Toolbar->AddTool(Menu_CompileAndRun, toolBarBitmaps[10], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[95]));
    FB_Toolbar->AddTool(Menu_QuickRun, toolBarBitmaps[11], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[96]));
    FB_Toolbar->AddTool(Menu_Result, toolBarBitmaps[14], wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, _(Lang[97]));
    
    FB_Toolbar->Realize();


    return;
}



//------------------------------------------------------------------------------
// Load Statusbar
void MyFrame::LoadStatusBar () {
    CreateStatusBar(2);
    SetStatusText(_T(Lang[1]));
    return;
}


void MyFrame::NewSTCPage ( wxString InitFile, bool select ) {
    
    void* doc;
    if ( InitFile == "" ) InitFile = FBUNNAMED;
    Buffer* buff;
    
    if (stc==NULL) {
        OldTabSelected = -1;
        stc = new FB_Edit( this, FBNotebook, -1, "" );
        stc->Freeze();
        stc->LoadSTCTheme();
        stc->LoadSTCSettings();
        buff = bufferList.AddFileBuffer("", "");
        doc = stc->GetDocPointer();
        stc->AddRefDocument(doc);
        stc->SetDocPointer(doc);
        if (InitFile!=FBUNNAMED) stc->LoadFile(InitFile);
        FBNotebook->AddPage( stc, wxFileNameFromPath(InitFile), true );
        FBNotebook->Refresh();
        FBNotebook->Show();
    }
    else {
        stc->SetBuffer( (Buffer *) 0 );
        stc->Freeze();
        buff = bufferList.AddFileBuffer("", "");
        SaveDocumentStatus(FBNotebook->GetSelection());
        doc = stc->CreateDocument();
        stc->AddRefDocument(doc);
        stc->SetDocPointer(doc);
        OldTabSelected = -1;
        if (InitFile!=FBUNNAMED) stc->LoadFile(InitFile);
        FBNotebook->InsertPage(FBNotebook->GetPageCount(), stc, wxFileNameFromPath(InitFile), true);
    }
    
    buff->SetFileName(InitFile);
    buff->SetModified(false);
    buff->UpdateModTime();
    buff->SetDocument(doc);
    stc->SetFocus();
    stc->SetBuffer( (Buffer *) buff );
    stc->Thaw();

    return;
}

void MyFrame::ChangingNBPage   ( wxNotebookEvent& event) {
    return;
}

void MyFrame::ChangeNBPage   ( wxNotebookEvent& event) {
    if (OldTabSelected==-1) {
        OldTabSelected = 0 ;
        return;
    }
    
    if (stc==0) return;
    
    int index = event.GetSelection();
    if (FBNotebook->GetPageCount()>1) SaveDocumentStatus(event.GetOldSelection());
    SetSTCPage ( index );
    return;
}


void MyFrame::SaveDocumentStatus ( int docID ) {
    Buffer* buff = bufferList.GetBuffer(docID);
    buff->SetPositions(stc->GetSelectionStart(), stc->GetSelectionEnd());
    buff->SetLine(stc->GetFirstVisibleLine());
    buff->SetCaretPos(stc->GetCurrentPos());
}

void MyFrame::SetSTCPage ( int index ) {
    if (stc == 0) return;
    
    stc->Freeze();
        stc->SetBuffer( (Buffer *) 0 );
        
        Buffer* buff = bufferList.GetBuffer(index);
            
        void* doc = buff->GetDocument();
        stc->AddRefDocument(doc);
        stc->SetDocPointer(doc);
    
        stc->ScrollToLine(buff->GetLine());
        stc->SetCurrentPos(buff->GetCaretPos());
        stc->SetSelectionStart(buff->GetSelectionStart());
        stc->SetSelectionEnd(buff->GetSelectionEnd());
        stc->SetFocus();
        
        stc->SetBuffer( (Buffer *) buff);
    stc->Thaw();
}

void MyFrame::SetModified ( int index, bool status ) {

    if (index==-1) index = FBNotebook->GetSelection();
    
    Buffer* buff = bufferList.GetBuffer(index);
    if (status!=buff->GetModified()) {
        buff->SetWasModified(!status);
        wxString NewName;
        if(status) NewName << "[*] ";            
        
        bufferList.SetBufferModified(index, status);
        NewName << wxFileNameFromPath(buff->GetFileName());
        FBNotebook->SetPageText( index, NewName );
    }
}

