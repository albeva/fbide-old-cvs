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
#include "inc/configdiag.h"

BEGIN_EVENT_TABLE( ConfigDialog, wxDialog)
    EVT_BUTTON(CDID_OK,             ConfigDialog::Button_OK)
    EVT_BUTTON(CDID_Cancel,         ConfigDialog::Button_Cancel)
    EVT_BUTTON(CDID_GetCompPath,    ConfigDialog::Button_GetCompPath)
    EVT_BUTTON(CDID_Background,     ConfigDialog::Button_Background)
    EVT_BUTTON(CDID_Foreground,     ConfigDialog::Button_Foreground)
    EVT_BUTTON(CDID_SaveTheme,      ConfigDialog::Button_SaveTheme)
    
    EVT_LISTBOX(-1,                 ConfigDialog::ListBox_Select)
    EVT_COMBOBOX(CDID_ThemeSelector,ConfigDialog::ThemeSelector)
    EVT_CHOICE(CDID_KeywordSelector,ConfigDialog::KeywordSelect)
END_EVENT_TABLE()

ConfigDialog::ConfigDialog( wxWindow* parent,
                            wxWindowID id,
                            const wxString& title,
                            long style,
                            const wxString& name )
{
    
    Parent = ( MyFrame * ) parent;
    ModKw=false;
    ModTheme=false;
    Style = Parent->Style;
    Oldselection = 0;

    Create(parent,id,title,wxDefaultPosition, wxSize(405, 330), style, name);
    
    CD_Notebook=new wxNotebook(this,-1,wxPoint(5,4),wxSize(390,265));
    //CD_Notebook->SetTitle(wxT(_(Lang[98])));
    
    CD_Settings=new wxPanel(CD_Notebook,-1,wxPoint(4,22),wxSize(382,234));
    CD_Notebook->AddPage((wxNotebookPage *)CD_Settings, _(Parent->Lang[99]) ,false);
    
    CD_Theme=new wxPanel(CD_Notebook,-1,wxPoint(4,22),wxSize(382,234));
    CD_Notebook->AddPage((wxNotebookPage *)CD_Theme,_(Parent->Lang[100]),false);
    
    CD_Keywords=new wxPanel(CD_Notebook,-1,wxPoint(4,22),wxSize(382,234));
    CD_Notebook->AddPage((wxNotebookPage *)CD_Keywords,_(Parent->Lang[101]),false);
    
    CD_Compiler=new wxPanel(CD_Notebook,-1,wxPoint(4,22),wxSize(382,234));
    CD_Notebook->AddPage((wxNotebookPage *)CD_Compiler,_(Parent->Lang[102]),false);

    
    //--------------------------------------------------------------------------
    
    CB_CurrentLine=new wxCheckBox(CD_Settings,-1,wxT(""),wxPoint(180,30),wxSize(148,13));
    CB_CurrentLine->SetTitle(wxT(Parent->Lang[103]));
    CB_CurrentLine->SetValue(Parent->Prefs.LineNumber);
    
    
    st20=new wxStaticText(CD_Settings,-1,wxT(""),wxPoint(10,5),wxSize(90,13),wxST_NO_AUTORESIZE);
    st20->SetTitle(wxT(Parent->Lang[104]));

    CB_HiLightBraces=new wxCheckBox(CD_Settings,-1,wxT(""),wxPoint(10,90),wxSize(148,13));
    CB_HiLightBraces->SetTitle(wxT(Parent->Lang[105]));
    CB_HiLightBraces->SetValue(Parent->Prefs.BraceHighlight);
    
    CB_SyntaxHiLight=new wxCheckBox(CD_Settings,-1,wxT(""),wxPoint(180,50),wxSize(148,13));
    CB_SyntaxHiLight->SetTitle(wxT(Parent->Lang[106]));
    CB_SyntaxHiLight->SetValue(Parent->Prefs.SyntaxHighlight);
    
    CB_LongLine=new wxCheckBox(CD_Settings,-1,wxT(""),wxPoint(10,110),wxSize(108,13));
    CB_LongLine->SetTitle(wxT(Parent->Lang[107]));
    CB_LongLine->SetValue(Parent->Prefs.LongLine);
    
    CB_LineEndings=new wxCheckBox(CD_Settings,-1,wxT(""),wxPoint(180,70),wxSize(148,13));
    CB_LineEndings->SetTitle(wxT(Parent->Lang[108]));
    CB_LineEndings->SetValue(Parent->Prefs.DisplayEOL);
    
    CB_WhiteSpaces=new wxCheckBox(CD_Settings,-1,wxT(""),wxPoint(10,69),wxSize(148,13));
    CB_WhiteSpaces->SetTitle(wxT(Parent->Lang[109]));
    CB_WhiteSpaces->SetValue(Parent->Prefs.whiteSpace);
    
    CB_IndentGuides=new wxCheckBox(CD_Settings,-1,wxT(""),wxPoint(10,50),wxSize(148,13));
    CB_IndentGuides->SetTitle(wxT(Parent->Lang[110]));
    CB_IndentGuides->SetValue(Parent->Prefs.IndentGuide);
    
   
    CB_FoldMargin=new wxCheckBox(CD_Settings,-1,wxT(""),wxPoint(180,90),wxSize(148,13));
    CB_FoldMargin->SetTitle(wxT(Parent->Lang[111]));
    CB_FoldMargin->SetValue(Parent->Prefs.FolderMargin);
    
    CB_AutoIndent=new wxCheckBox(CD_Settings,-1,wxT(""),wxPoint(10,30),wxSize(148,13));
    CB_AutoIndent->SetTitle(wxT(Parent->Lang[112]));
    CB_AutoIndent->SetValue(Parent->Prefs.AutoIndent);


    TB_Width=new wxSpinCtrl(CD_Settings,-1,wxT(""),wxPoint(275,110),wxSize(60,15));
    TB_Width->SetValue(Parent->Prefs.EdgeColumn);
    
    lno21=new wxStaticLine(CD_Settings,-1,wxPoint(10,20),wxSize(360,2));
    


    // compile page
    st33=new wxStaticText(CD_Compiler,-1,wxT(""),wxPoint(10,5),wxSize(105,13),wxST_NO_AUTORESIZE);
    st33->SetLabel(wxT(Parent->Lang[113]));
    lno31=new wxStaticLine(CD_Compiler,-1,wxPoint(10,20),wxSize(360,2));

    st37=new wxStaticText(CD_Compiler,-1,wxT(""),wxPoint(10,30),wxSize(70,13),wxST_NO_AUTORESIZE);
    st37->SetLabel(wxT(Parent->Lang[114]));
    
    TB_CompilerPath=new wxTextCtrl(CD_Compiler,-1,wxT(""),wxPoint(85,28),wxSize(250,21));
    TB_CompilerPath->SetValue(Parent->CompilerPath);

    But_GetCompilerPath=new wxButton(CD_Compiler, CDID_GetCompPath,wxT(""),wxPoint(340,30),wxSize(30,20));
    But_GetCompilerPath->SetLabel(wxT("..."));
    
    //Compiler command
    st41=new wxStaticText(CD_Compiler,-1,wxT(""),wxPoint(10,60),wxSize(70,13),wxST_NO_AUTORESIZE);
    st41->SetLabel(wxT(Parent->Lang[115]));
    
    CompilerCommand=new wxTextCtrl(CD_Compiler,-1,wxT(""),wxPoint(85,58),wxSize(250,21));
    CompilerCommand->SetValue(Parent->CMDPrototype);
    
//////////////////////////////////////////////////

    
    st34=new wxStaticText(CD_Settings,-1,wxT(""),wxPoint(180,110),wxSize(95,13),wxST_NO_AUTORESIZE);
    st34->SetLabel(wxT(Parent->Lang[116]));
    
    //Compiler path
    
    //Tab Size
    st43=new wxStaticText(CD_Settings,-1,wxT(""),wxPoint(180,130),wxSize(45,13),wxST_NO_AUTORESIZE);
    st43->SetLabel(wxT(Parent->Lang[117]));

    spc44=new wxSpinCtrl(CD_Settings,-1,wxT(""),wxPoint(275,130),wxSize(60,15));
    spc44->SetValue(Parent->Prefs.TabSize);


    //Theme colors types:
    wxArrayString Choices;
    Choices.Add(Parent->Lang[118]);        //Comments
    Choices.Add(Parent->Lang[119]);        //Numbers
    Choices.Add(Parent->Lang[120]);        //Keywords 1
    Choices.Add(Parent->Lang[121]);        //String Closed
    Choices.Add(Parent->Lang[122]);        //Preprocessor
    Choices.Add(Parent->Lang[123]);        //Operator
    Choices.Add(Parent->Lang[124]);        //Identifier
    Choices.Add(Parent->Lang[125]);        //Date (doesn't work)
    Choices.Add(Parent->Lang[126]);        //String Open
    Choices.Add(Parent->Lang[127]);        //Keywords 2
    Choices.Add(Parent->Lang[128]);        //Keywords 3
    Choices.Add(Parent->Lang[129]);        //Keywords 4

    Choices.Add(Parent->Lang[130]);        //Caret
    Choices.Add(Parent->Lang[131]);        //Line numbers
    Choices.Add(Parent->Lang[132]);        //Text select
    Choices.Add(Parent->Lang[133]);        //Brace match
    Choices.Add(Parent->Lang[134]);        //Brace mismatch
    Choices.Add(Parent->Lang[135]);        //Editor

    LB_Types=new wxListBox(CD_Theme,-1,wxPoint(10,45),wxSize(120,176), Choices);
    LB_Types->SetSelection(0);
    
    But_Foreground=new wxButton(CD_Theme, CDID_Foreground, wxT(""),wxPoint(135,95),wxSize(145,21));
    But_Foreground->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
    
    But_Background=new wxButton(CD_Theme, CDID_Background, wxT(""),wxPoint(135,140),wxSize(145,21));
    But_Background->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));

    
    CB_Bold=new wxCheckBox(CD_Theme,-1,wxT(""),wxPoint(300,95),wxSize(78,13));
    CB_Bold->SetTitle(wxT(Parent->Lang[136])); //"Bold"
    
    CB_Italic=new wxCheckBox(CD_Theme,-1,wxT(""),wxPoint(300,115),wxSize(38,13));
    CB_Italic->SetTitle(wxT(Parent->Lang[137])); //"Italic"
    
    CB_Underlined=new wxCheckBox(CD_Theme,-1,wxT(""),wxPoint(300,135),wxSize(68,13));
    CB_Underlined->SetTitle(wxT(Parent->Lang[138])); //"Underlined"
    
    But_SaveTheme=new wxButton(CD_Theme, CDID_SaveTheme,wxT(""),wxPoint(285,45),wxSize(85,20));
    But_SaveTheme->SetLabel(wxT(Parent->Lang[139])); //"Save theme"
    
    st58=new wxStaticText(CD_Theme,-1,wxT(""),wxPoint(135,30),wxSize(45,13),wxST_NO_AUTORESIZE);
    st58->SetLabel(wxT(Parent->Lang[140])); //"Theme"

    wxArrayString Themes;
    Themes.Add(Parent->Lang[141]); //"Create new theme"
//    Themes.Add(w.GetFullName());

    wxFileSystem ThemeFiles;
    wxString Fn = ThemeFiles.FindFirst(Parent->EditorPath+"ide\\*.fbt");
    wxFileName w;
    
    int counter=0, selector=0;
    while (Fn!="") {
        counter++;
        w.Assign(Fn, wxPATH_NATIVE);
        Fn = w.GetName();
        Fn = Fn.Lower();
        if (Fn==Parent->ThemeFile) selector = counter;
        Themes.Add(Fn);
        Fn = ThemeFiles.FindNext();
    }
        
    CB_Themes=new wxComboBox(CD_Theme, CDID_ThemeSelector,wxT(""),wxPoint(135,45),wxSize(145,21),Themes, wxCB_READONLY | wxCB_DROPDOWN );
    CB_Themes->Select(selector);
    
    st59=new wxStaticText(CD_Theme,-1,wxT(""),wxPoint(10,5),wxSize(45,13),wxST_NO_AUTORESIZE);
    st59->SetLabel(wxT(Parent->Lang[142])); //"Colors"
    
    lno60=new wxStaticLine(CD_Theme,-1,wxPoint(10,20),wxSize(360,2));
    
    st61=new wxStaticText(CD_Theme,-1,wxT(""),wxPoint(10,30),wxSize(45,13),wxST_NO_AUTORESIZE);
    st61->SetLabel(wxT(Parent->Lang[143])); //"Type"
    
    st62=new wxStaticText(CD_Theme,-1,wxT(""),wxPoint(135,80),wxSize(65,13),wxST_NO_AUTORESIZE);
    st62->SetLabel(wxT(Parent->Lang[144])); //"Foreground"
    
    st63=new wxStaticText(CD_Theme,-1,wxT(""),wxPoint(135,125),wxSize(60,13),wxST_NO_AUTORESIZE);
    st63->SetLabel(wxT(Parent->Lang[145])); //"Background"
    
    st64=new wxStaticText(CD_Theme,-1,wxT(""),wxPoint(300,80),wxSize(50,13),wxST_NO_AUTORESIZE);
    st64->SetLabel(wxT(Parent->Lang[146])); //"Fontstyle"
    
    lno65=new wxStaticLine(CD_Theme,-1,wxPoint(135,75),wxSize(235,2));
    
    lnv66=new wxStaticLine(CD_Theme,-1,wxPoint(290,80),wxSize(2,120),wxLI_VERTICAL);
    
    st67=new wxStaticText(CD_Theme,-1,wxT(""),wxPoint(135,165),wxSize(45,13),wxST_NO_AUTORESIZE);
    st67->SetLabel(wxT(Parent->Lang[147])); //"Font\r\n"
    
    
    wxArrayString Fonts;
    Fonts.Add("Courier");
    Fonts.Add("Courier New");
    Fonts.Add("Fixedsys");
    Fonts.Add("Lucida Console");
    Fonts.Add("Modern");
    Fonts.Add("Terminal");
    Fonts.Add("Arial");

    
    cmb69=new wxComboBox(CD_Theme,-1, wxT("Courier"),wxPoint(135,180),wxSize(145,21),Fonts, wxCB_READONLY | wxCB_DROPDOWN );
    cmb69->Select(1);
    
    st70=new wxStaticText(CD_Theme,-1,wxT(""),wxPoint(300,165),wxSize(45,13),wxST_NO_AUTORESIZE);
    st70->SetLabel(wxT(Parent->Lang[148])); //"Fontsize\r\n"
    
    //
    spc71=new wxSpinCtrl(CD_Theme,-1,wxT(""),wxPoint(300,180),wxSize(70,20));
    

    lno74=new wxStaticLine(CD_Keywords,-1,wxPoint(10,20),wxSize(360,2));
    
    st75=new wxStaticText(CD_Keywords,-1,wxT(""),wxPoint(10,5),wxSize(60,13),wxST_NO_AUTORESIZE);
    st75->SetLabel(wxT(Parent->Lang[149])); //"Keywords\r\n"
    
    st77=new wxStaticText(CD_Keywords,-1,wxT(""),wxPoint(10,30),wxSize(110,13),wxST_NO_AUTORESIZE);
    st77->SetLabel(wxT(Parent->Lang[150]));//"Select keyword group"
    
    CB_KeywordGroup=new wxChoice(CD_Keywords,CDID_KeywordSelector,wxPoint(10,45),wxSize(150,21),0,NULL);
    CB_KeywordGroup->Append(Parent->Lang[151]);//"Group 1"
    CB_KeywordGroup->Append(Parent->Lang[152]);//"Group 2"
    CB_KeywordGroup->Append(Parent->Lang[153]);//"Group 3"
    CB_KeywordGroup->Append(Parent->Lang[154]);//"Group 4"
    CB_KeywordGroup->Select(0);
    OldKWSelect = 1;
    
    TA_Keywords=new wxTextCtrl(CD_Keywords, -1,wxT(""),wxPoint(10,70),wxSize(360,158),wxTE_WORDWRAP|wxTE_MULTILINE);
    TA_Keywords->AppendText(Parent->Keyword[1]);
    
    But_OK=new wxButton(this, CDID_OK,wxT(""),wxPoint(250,270),wxSize(70,25));
    But_OK->SetLabel(wxT(Parent->Lang[2])); //OK
    
    But_Cancel=new wxButton(this, CDID_Cancel, wxT(""),wxPoint(325,270),wxSize(70,25));
    But_Cancel->SetLabel(wxT(Parent->Lang[3])); //Cancel
    
    Refresh();
    wxCommandEvent test;
    ListBox_Select (test);
}


void ConfigDialog::Button_OK      (wxCommandEvent&  event) {
    
    int selection = LB_Types->GetSelection();
    Oldselection=selection;
    Oldselection++;
    if (Oldselection>0) SaveSettings();
//    if (CB_Themes->GetSelection()!=0)
//        Parent->SaveThemeFile(Style, CB_Themes->GetValue());
    
    Parent->Keyword[OldKWSelect]    = TA_Keywords->GetValue();
    Parent->SavekwFile( Parent->SyntaxFile );
    
    Parent->Prefs.LineNumber        = CB_CurrentLine->GetValue();
    Parent->Prefs.BraceHighlight    = CB_HiLightBraces->GetValue();
    Parent->Prefs.SyntaxHighlight   = CB_SyntaxHiLight->GetValue();
    Parent->Prefs.LongLine          = CB_LongLine->GetValue();
    Parent->Prefs.DisplayEOL        = CB_LineEndings->GetValue();
    Parent->Prefs.whiteSpace        = CB_WhiteSpaces->GetValue();
    Parent->Prefs.IndentGuide       = CB_IndentGuides->GetValue();
    Parent->Prefs.FolderMargin      = CB_FoldMargin->GetValue();
    Parent->Prefs.AutoIndent        = CB_AutoIndent->GetValue();
    
    Parent->Prefs.EdgeColumn        = TB_Width->GetValue();
    Parent->Prefs.TabSize           = spc44->GetValue();
    Parent->CompilerPath            = TB_CompilerPath->GetValue();
    Parent->CMDPrototype            = CompilerCommand->GetValue();
    
    Parent->Style = Style;
    
    Close(true);
    return;
}


void ConfigDialog::Button_Cancel  (wxCommandEvent&  event){
    Close(true);
    return;
}



void ConfigDialog::Button_GetCompPath   (wxCommandEvent&  event) {

    wxFileDialog dlg (this,
        _T(Parent->Lang[155]), //Open file
        _T(""),
        _T(".exe"),
        _T(Parent->Lang[156]), //"All programs (*.exe)|*.exe"
    wxFILE_MUST_EXIST | wxCHANGE_DIR);
    if (dlg.ShowModal() != wxID_OK) return;
    
    TB_CompilerPath->SetValue(dlg.GetPath());

    return;
}

void ConfigDialog::ListBox_Select (wxCommandEvent &event) {

    bool ModFG = false, ModBG = false, ModFont = false, ModStyle = false, ModSize = false;
    bool SetBold = false, SetItalic = false, SetUnderlined = false;
    int SetSize = 0;
    wxString SetFont = ""; 
    int selection = LB_Types->GetSelection();
    selection++;
    
    if (Oldselection==selection) return;
    if (Oldselection>0) SaveSettings();
    Oldselection=selection;
    
    if ( selection < 13 ) {
        ModFG = true; ModBG = true;
        ModFont = true; ModStyle = true; ModSize = true;
        SetBold = (Style.Info[selection].fontstyle & mySTC_STYLE_BOLD)>0;
        SetItalic = (Style.Info[selection].fontstyle & mySTC_STYLE_ITALIC)>0;
        SetUnderlined = (Style.Info[selection].fontstyle & mySTC_STYLE_UNDERL)>0;    
        SetSize = Style.Info[selection].fontsize;
        SetFont = Style.Info[selection].fontname;
        fg = Style.Info[selection].foreground;
        bg = Style.Info[selection].background;
    }
    else if (selection==13) {
        ModFG = true;
        fg = Style.CaretColour;
    }
    else if (selection==14) {
        ModFG = true; ModBG = true;
        bg = Style.LineNumberBgColour;
        fg = Style.LineNumberFgColour;
    }
    else if (selection==15) {
        ModFG = true; ModBG = true;
        bg = Style.SelectBgColour;
        fg = Style.SelectFgColour;
    }
    else if (selection==16) {
        ModStyle = true;
        ModFG = true;
        ModBG = true;
        SetBold = (Style.BraceFontStyle & mySTC_STYLE_BOLD)>0;
        SetItalic = (Style.BraceFontStyle & mySTC_STYLE_ITALIC)>0;
        SetUnderlined = (Style.BraceFontStyle & mySTC_STYLE_UNDERL)>0;
        fg = Style.BraceFgColour;
        bg = Style.BraceBgColour;
    }
    else if (selection==17) {
        ModStyle = true;
        ModFG = true;
        ModBG = true;
        SetBold = (Style.BadBraceFontStyle & mySTC_STYLE_BOLD)>0;
        SetItalic = (Style.BadBraceFontStyle & mySTC_STYLE_ITALIC)>0;
        SetUnderlined = (Style.BadBraceFontStyle & mySTC_STYLE_UNDERL)>0;
        bg = Style.BadBraceBgColour;
        fg = Style.BadBraceFgColour;
    }
    else if (selection==18) {
        ModBG = true, ModFG = true, ModSize = true;
        bg = Style.DefaultBgColour;
        fg = Style.DefaultFgColour;
        SetSize = Style.DefaultFontSize;
    }


    if (ModStyle) {
        CB_Bold->Enable();
        CB_Italic->Enable();
        CB_Underlined->Enable();
        
        CB_Bold->SetValue(SetBold);
        CB_Italic->SetValue(SetItalic);
        CB_Underlined->SetValue(SetUnderlined);
    }
    else {
        CB_Bold->Disable();
        CB_Italic->Disable();
        CB_Underlined->Disable();
    }
    

    if (ModSize) {
        spc71->Enable();
        spc71->SetValue(SetSize);
    }
    else spc71->Disable();
    

    if (ModFont) {
        cmb69->Enable();
        if (SetFont.Lower() == "courier")
            cmb69->Select(0);
        else if (SetFont.Lower() == "courier new")
            cmb69->Select(1);
        else if (SetFont.Lower() == "fixedsys")
            cmb69->Select(2);
        else if (SetFont.Lower() == "lucida console")
            cmb69->Select(3);
        else if (SetFont.Lower() == "modern")
            cmb69->Select(4);
        else if (SetFont.Lower() == "terminal")
            cmb69->Select(5);
        else if (SetFont.Lower() == "arial")
            cmb69->Select(6);
        else cmb69->Select(1);
    }
    else cmb69->Disable();
    
    if (ModFG) {
        But_Foreground->Enable();
        But_Foreground->SetBackgroundColour(GetClr(fg));
    }
    else {
        But_Foreground->Disable();
        But_Foreground->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
    }
    
    if (ModBG) {
        But_Background->Enable();
        But_Background->SetBackgroundColour(GetClr(bg));
    }
    else {
        But_Background->Disable();
        But_Background->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
    }
        

//    Choices.Add("Caret");
//    Choices.Add("Linenumbers");
//    Choices.Add("Text select");
//    Choices.Add("Brace match");
//    Choices.Add("Brace mismatch");

    return;

}

void ConfigDialog::Button_Foreground ( wxCommandEvent&  event ) {
    wxColour clrt;
    Parent->colr.SetChooseFull(false);

    wxColourDialog dlg(this, &(Parent->colr));
    if (dlg.ShowModal()!=wxID_OK) return;

    Parent->colr = dlg.GetColourData();
    clrt = Parent->colr.GetColour();

    But_Foreground->SetBackgroundColour(clrt);
    fg = (clrt.Red() << 16) | (clrt.Green() << 8) | clrt.Blue();
    
    return;
}

void ConfigDialog::Button_Background ( wxCommandEvent&  event ) {
    wxColour clrt;
    Parent->colr.SetChooseFull(false);
    
    wxColourDialog dlg(this, &(Parent->colr));
    if (dlg.ShowModal()!=wxID_OK) return;
    
    Parent->colr = dlg.GetColourData();
    clrt = Parent->colr.GetColour();
    
    But_Background->SetBackgroundColour(clrt);
    bg = (clrt.Red() << 16) | (clrt.Green() << 8) | clrt.Blue();
    
    return;
}

void ConfigDialog::SaveSettings () {
    int selection = Oldselection;
    if (selection < 13) {
        Style.Info[selection].fontstyle = 0;
        if (CB_Bold->GetValue())
                Style.Info[selection].fontstyle |= mySTC_STYLE_BOLD;
        if (CB_Italic->GetValue())
                Style.Info[selection].fontstyle |= mySTC_STYLE_ITALIC;
        if (CB_Underlined->GetValue())
                Style.Info[selection].fontstyle |= mySTC_STYLE_UNDERL;

        Style.Info[selection].fontsize = spc71->GetValue();
        Style.Info[selection].fontname = cmb69->GetValue();
        Style.Info[selection].foreground = fg;
        Style.Info[selection].background = bg;
    }
    else if (selection==13) {
        Style.CaretColour = fg;
    }
    else if (selection==14) {
        Style.LineNumberBgColour = bg;
        Style.LineNumberFgColour = fg;
    }
    else if (selection==15) {
        Style.SelectBgColour = bg;
        Style.SelectFgColour = fg;
    }
    else if (selection==16) {
        
        Style.BraceFontStyle = 0;
        if (CB_Bold->GetValue())
                Style.BraceFontStyle |= mySTC_STYLE_BOLD;
        if (CB_Italic->GetValue())
                Style.BraceFontStyle |= mySTC_STYLE_ITALIC;
        if (CB_Underlined->GetValue())
                Style.BraceFontStyle |= mySTC_STYLE_UNDERL;
        Style.BraceFgColour = fg;
        Style.BraceBgColour = bg;
    }
    else if (selection==17) {
        Style.BadBraceFontStyle = 0;
        if (CB_Bold->GetValue())
                Style.BadBraceFontStyle |= mySTC_STYLE_BOLD;
        if (CB_Italic->GetValue())
                Style.BadBraceFontStyle |= mySTC_STYLE_ITALIC;
        if (CB_Underlined->GetValue())
                Style.BadBraceFontStyle |= mySTC_STYLE_UNDERL;
        Style.BadBraceBgColour = bg;
        Style.BadBraceFgColour = fg;
    }
    else if (selection==18) {
        Style.DefaultBgColour = bg;
        Style.DefaultFgColour = fg;
        Style.DefaultFontSize = spc71->GetValue();
    }
}

void ConfigDialog::ThemeSelector (wxCommandEvent & event) {
    
    if (CB_Themes->GetSelection()!=0) {
        Style = Parent->LoadThemeFile(CB_Themes->GetValue());
        Parent->ThemeFile = CB_Themes->GetValue();
    }
    Oldselection = 0;
    wxCommandEvent test;
    ListBox_Select (test);
    
    return;
}


void ConfigDialog::Button_SaveTheme ( wxCommandEvent&  event ) {
    
    SaveSettings();
    if (CB_Themes->GetSelection()!=0)
        Parent->SaveThemeFile(Style, CB_Themes->GetValue());
    else {
        wxTextEntryDialog dialog(this, _T(Parent->Lang[157]), //"Enter theme name:"
                                        _T(Parent->Lang[158]), //"Paremeters..."
                                        "", wxOK | wxCANCEL);
        if (dialog.ShowModal() != wxID_OK) return;
        
        wxString fn = dialog.GetValue();
        fn = fn.Lower();
        fn = fn.Trim(true);
        fn = fn.Trim(false);

        wxFileSystem File;
        if (File.FindFirst(Parent->EditorPath+"ide\\"+fn+".fbt")!="") {
            //"Theme \""+ fn +"\" already exists!\nOverwrite it?"
            if (wxMessageBox (Parent->Lang[159]+ fn + Parent->Lang[160], Parent->Lang[161], wxICON_QUESTION | wxYES_NO )!=wxOK) 
                return;
        }
        else {
            wxTextFile temp(Parent->EditorPath+"ide\\"+fn+".fbt");
            temp.Create();
            temp.Write();
            temp.Close();
        }
            
        Parent->SaveThemeFile(Style, fn);
        Parent->ThemeFile = fn;
        CB_Themes->Append(fn);
        CB_Themes->Select(CB_Themes->GetCount()-1);
    }
    return;
}


void ConfigDialog::KeywordSelect          ( wxCommandEvent& event ) {
    int selection = CB_KeywordGroup->GetSelection() + 1;
    Parent->Keyword[OldKWSelect] = TA_Keywords->GetValue();
    TA_Keywords->Clear();
    TA_Keywords->AppendText( Parent->Keyword[selection] );
    OldKWSelect = selection;

}


ConfigDialog::~ConfigDialog   (  ) {
 
    delete CD_Settings;
    delete CD_Theme;
    delete CD_Keywords;
    delete CD_Compiler;
    delete CD_Notebook;
}