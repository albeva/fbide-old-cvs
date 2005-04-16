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
* Program URL   : http://www.hot.ee/fbide
*/

#include "inc/main.h"

void MyFrame::LoadSettings() {
    
    wxFileName w(FB_App->argv[0]);
    EditorPath = w.GetPath(wxPATH_GET_SEPARATOR|wxPATH_GET_VOLUME);
    
    wxFileInputStream PrefsINIIS( EditorPath + "ide\\prefs.ini");
    wxFileConfig PrefsINI(PrefsINIIS);
    
    wxString Temp = PRODUCT_NAME;
    SetTitle(Temp + " - " + Document);
    
    bool b = false;
    PrefsINI.SetPath("/general");
    Prefs.AutoIndent        = PrefsINI.Read("autoindent",       b);
    Prefs.SyntaxHighlight   = PrefsINI.Read("syntaxhighlight",  b);
    Prefs.LongLine          = PrefsINI.Read("borderline",       b);
    Prefs.whiteSpace        = PrefsINI.Read("whitespaces",      b);
    Prefs.LineNumber        = PrefsINI.Read("linenumbers",      b);
    Prefs.IndentGuide       = PrefsINI.Read("indentguide",      b);
    Prefs.BraceHighlight    = PrefsINI.Read("bracehighlight",   b);
    Prefs.ShowExitCode      = PrefsINI.Read("showexitcode",     b);
    Prefs.FolderMargin      = PrefsINI.Read("foldermargin",     b);
    Prefs.DisplayEOL        = PrefsINI.Read("Displayeol",       b);
    Prefs.CurrentLine       = PrefsINI.Read("lightcursorline",  b);
    Prefs.TabSize           = PrefsINI.Read("tabsize",          3l);
    Prefs.EdgeColumn        = PrefsINI.Read("edgecolumn",       80L);
    Prefs.Language          = PrefsINI.Read("language",         "English");
    
    PrefsINI.SetPath("/paths");
    CompilerPath            = PrefsINI.Read("fbc",      "");
    if (CompilerPath == "") CompilerPath = EditorPath + "fbc.exe";

    SyntaxFile              = PrefsINI.Read("syntax",   "");
    if (SyntaxFile=="") SyntaxFile = "fbfull.lng";

    ThemeFile               = PrefsINI.Read("theme",    "");
    if (ThemeFile=="") ThemeFile = "classic";

    
    PrefsINI.SetPath("/compiler");
    CMDPrototype            = PrefsINI.Read("command",  "<fbc> <filename>");
    
    int winx, winy, winh, winw;
    PrefsINI.SetPath("/editor");
    Prefs.FloatBars         = PrefsINI.Read("floatbars",        b);
    winx                    = PrefsINI.Read("winx",             50),
    winy                    = PrefsINI.Read("winy",             50),
    winw                    = PrefsINI.Read("winw",             350),
    winh                    = PrefsINI.Read("winh",             200);
    
    if(winw==-1||winh==-1) Maximize();
    else {
        Move(winx, winy);
        SetClientSize(winw, winh);
    }
    
    OpenLangFile(Prefs.Language);
    return;
}



void MyFrame::SaveSettings() {
    wxFileInputStream PrefsINIIS( EditorPath + "ide\\prefs.ini");
    wxFileConfig PrefsINI(PrefsINIIS);
    
    wxFileOutputStream PrefsINIOS( EditorPath + "ide\\prefs.ini");
    
    PrefsINI.SetPath("/general");
    PrefsINI.Write("autoindent",        (bool)Prefs.AutoIndent);
    PrefsINI.Write("syntaxhighlight",   (bool)Prefs.SyntaxHighlight);
    PrefsINI.Write("linenumbers",       (bool)Prefs.LineNumber);
    PrefsINI.Write("indentguide",       (bool)Prefs.IndentGuide);
    PrefsINI.Write("bracehighlight",    (bool)Prefs.BraceHighlight);
    PrefsINI.Write("showexitcode",      (bool)Prefs.ShowExitCode);
    PrefsINI.Write("foldermargin",      (bool)Prefs.FolderMargin);
    PrefsINI.Write("Displayeol",        (bool)Prefs.DisplayEOL);
    PrefsINI.Write("borderline",        (bool)Prefs.LongLine);
    PrefsINI.Write("whitespaces",       (bool)Prefs.whiteSpace);
    PrefsINI.Write("lightcursorline",   (bool)Prefs.CurrentLine);
    PrefsINI.Write("tabsize",           (long)Prefs.TabSize);
    PrefsINI.Write("edgecolumn",        (long)Prefs.EdgeColumn);

    PrefsINI.SetPath("/paths");
    PrefsINI.Write("fbc",               CompilerPath);
    PrefsINI.Write("syntax",            SyntaxFile);
    PrefsINI.Write("theme",             ThemeFile);

    
    PrefsINI.SetPath("/compiler");
    PrefsINI.Write("command",           CMDPrototype);

    int winx, winy, winh, winw;
    
    if (IsMaximized()) {
        winw=-1;
        winh=-1;
        winx=0;
        winy=0;
    }
    else {
        GetClientSize(&winw, &winh);
        GetPosition(&winx, &winy);
    }
    
    PrefsINI.SetPath("/editor");
    PrefsINI.Write("winx",             (long)winx),
    PrefsINI.Write("winy",             (long)winy),
    PrefsINI.Write("winw",             (long)winw),
    PrefsINI.Write("winh",             (long)winh);
    
    PrefsINI.Save(PrefsINIOS);

    return;
}



StyleInfo  MyFrame::LoadThemeFile( wxString ThemeFile ) {
    
    StyleInfo Style;
    
    wxFileInputStream ThemeIS( EditorPath + "ide\\" + ThemeFile + ".fbt" );
    wxFileConfig Theme( ThemeIS );
    
	wxString StyleTypes[]={	"default", 	    "comment", 	   "number",
                            "keyword",      "string",      "preprocessor",
							"operator", 	"identifier",  "date",
                            "stringeol",    "keyword2",    "keyword3",
                            "keyword4",     "constant",    "asm" };

	//Default:
	
    Theme.SetPath("/default");
    Style.DefaultBgColour       = Theme.Read("background",      0xffffff);
    Style.DefaultFgColour       = Theme.Read("foreground",      0L);
    Style.CaretColour           = Theme.Read("caret",           0L);
    Style.DefaultFontSize       = Theme.Read("fontsize",        12L);
    Style.CaretLine             = Theme.Read("caretline",       0xdddddd);
    Style.DefaultFontStyle      = Theme.Read("fontstyle",       0L);
    Style.DefaultFont           = Theme.Read("font",            "");
   
	//Line number
    Theme.SetPath("/linenumber");
    Style.LineNumberBgColour    = Theme.Read("background",      0xc0c0c0);
    Style.LineNumberFgColour    = Theme.Read("foreground",      0xffffff);
	
    Theme.SetPath("/select");
    Style.SelectBgColour        = Theme.Read("background",      0xc0c0c0);
    Style.SelectFgColour        = Theme.Read("foreground",      0xffffff);
    
    Theme.SetPath("/brace");
    Style.BraceBgColour         = Theme.Read("background",      (long)Style.DefaultFgColour);
    Style.BraceFgColour         = Theme.Read("foreground",      (long)Style.DefaultBgColour);
    Style.BraceFontStyle        = Theme.Read("fontstyle",       0L);
    
    Theme.SetPath("/badbrace");
    Style.BadBraceBgColour      = Theme.Read("background",      (long)Style.DefaultFgColour);
    Style.BadBraceFgColour      = Theme.Read("foreground",      0L);
    Style.BadBraceFontStyle     = Theme.Read("fonstyle",        0L);
    
    
    for (int i=1; i<15;i++) {
        Theme.SetPath("/" + StyleTypes[i]);
        Style.Info[i].background    = Theme.Read("background",      (long)Style.DefaultBgColour);
        Style.Info[i].foreground    = Theme.Read("foreground",      (long)Style.DefaultFgColour);
        Style.Info[i].fontsize      = Theme.Read("fontsize",        (long)Style.DefaultFontSize);
        Style.Info[i].lettercase    = Theme.Read("capital",         0L);
        Style.Info[i].fontstyle     = Theme.Read("fontstyle",       (long)Style.DefaultFontStyle);
        Style.Info[i].fontname      = Theme.Read("font",            Style.DefaultFont);
    }
    
    return Style;
}



void MyFrame::SaveThemeFile      ( StyleInfo Style, wxString ThemeFile ) {

    wxFileInputStream ThemeIS( EditorPath + "ide\\" + ThemeFile + ".fbt" );
    wxFileOutputStream ThemeISOS( EditorPath + "ide\\" + ThemeFile + ".fbt" );
    wxFileConfig Theme( ThemeIS );
    
	wxString StyleTypes[]={	"default", 	    "comment", 	   "number",
                            "keyword",      "string",      "preprocessor",
							"operator", 	"identifier",  "date",
                            "stringeol",    "keyword2",    "keyword3",
                            "keyword4",     "constant",    "asm" };

	//Default:
	
    Theme.SetPath("/default");
    Theme.Write("background",                   (long)Style.DefaultBgColour);
    Theme.Write("foreground",                   (long)Style.DefaultFgColour);
    Theme.Write("caret",                        (long)Style.CaretColour);
    Theme.Write("fontsize",                     (long)Style.DefaultFontSize);
    Theme.Write("caretline",                    (long)Style.CaretLine);
    Theme.Write("fontstyle",                    (long)Style.DefaultFontStyle);
    Theme.Write("font",                         Style.DefaultFont);
   
	//Line number
    Theme.SetPath("/linenumber");
    Theme.Write("background",                   (long)Style.LineNumberBgColour);
    Theme.Write("foreground",                   (long)Style.LineNumberFgColour);
	
    Theme.SetPath("/select");
    Theme.Write("background",                   (long)Style.SelectBgColour);
    Theme.Write("foreground",                   (long)Style.SelectFgColour);
    
    Theme.SetPath("/brace");
    Theme.Write("background",                   (long)Style.BraceBgColour);
    Theme.Write("foreground",                   (long)Style.BraceFgColour);
    Theme.Write("fontstyle",                    (long)Style.BraceFontStyle);
    
    Theme.SetPath("/badbrace");
    Theme.Write("background",                   (long)Style.BadBraceBgColour);
    Theme.Write("foreground",                   (long)Style.BadBraceFgColour);
    Theme.Write("fonstyle",                     (long)Style.BadBraceFontStyle);
    
    
    for (int i=1; i<15;i++) {
        Theme.SetPath("/" + StyleTypes[i]);
        Theme.Write("background",               (long)Style.Info[i].background);
        Theme.Write("foreground",               (long)Style.Info[i].foreground);
        Theme.Write("font",                     Style.Info[i].fontname);
        Theme.Write("fontsize",                 (long)Style.Info[i].fontsize);
        Theme.Write("capital",                  (long)Style.Info[i].lettercase);
        Theme.Write("fontstyle",                (long)Style.Info[i].fontstyle);
    }
    
    Theme.Save(ThemeISOS);
    return;
}



void MyFrame::LoadkwFile( wxString SyntaxFile ) {
    
    wxFileInputStream SyntaxIS( EditorPath + "ide\\" + SyntaxFile );
    wxFileConfig Syntax(SyntaxIS);
    
    Syntax.SetPath("/keywords");
    Keyword[1]            = Syntax.Read("kw1", "");
    Keyword[2]            = Syntax.Read("kw2", "");
    Keyword[3]            = Syntax.Read("kw3", "");
    Keyword[4]            = Syntax.Read("kw4", "");

    return;    
}


void MyFrame::SavekwFile( wxString SyntaxFile ) {
    
    wxFileInputStream SyntaxIS( EditorPath + "ide\\" + SyntaxFile );
    wxFileOutputStream SyntaxOS( EditorPath + "ide\\" + SyntaxFile );
    wxFileConfig Syntax(SyntaxIS);
    
    Syntax.SetPath("/keywords");
    Syntax.Write("kw1",     Keyword[1]);
    Syntax.Write("kw2",     Keyword[2]);
    Syntax.Write("kw3",     Keyword[3]);
    Syntax.Write("kw4",     Keyword[4]);
    
    Syntax.Save(SyntaxOS);
    return;    
}