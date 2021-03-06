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
//#include "inc/configdiag.h"
#include "inc/configdiag.h"
#include "inc/fbedit.h"
#include "inc/browser.h"
#include "inc/format.h"

void MyFrame::OnSettings (wxCommandEvent  &WXUNUSED(event)) {
    ConfigDialog dlg( this, -1, _T(Lang[98]));
    if( dlg.ShowModal() == wxID_OK ) {
        if( stc ) {
            SaveSettings();

            int index = FBNotebook->GetSelection();
            Buffer * buff = bufferList[index];

            stc->Freeze();
            stc->StyleClearAll();
            stc->LoadSTCTheme(buff->GetFileType());
            stc->LoadSTCSettings();
            stc->Thaw();
        }
    }
    return;
}


void MyFrame::OnFormat (wxCommandEvent  &WXUNUSED(event)) {
    if(stc==0)
        return;
    if(formatDialog)
        return;
    int index = FBNotebook->GetSelection();
    Buffer * buff = bufferList[index];
    if (buff->GetFileType()!=0)
        return;

    formatDialog=new format(this, -1, Lang[223]); //Convert the format
    formatDialog->Show();

    return;
}


void MyFrame::OnResult  (wxCommandEvent  &WXUNUSED(event)) {

    Freeze();
    if ( HSplitter->IsSplit() ) {
        ConsoleSize = HSplitter->GetSashPosition();
        HSplitter->Unsplit( FBConsole );
        FB_View->Check(Menu_Result, false);
    }
    else {
        HSplitter->SplitHorizontally( FBCodePanel, FBConsole, ConsoleSize );
        FB_View->Check(Menu_Result, true);
    }
    Thaw();
    return;
}


void MyFrame::OnSubs (wxCommandEvent  &WXUNUSED(event)) {
    if (stc==0)
        return;
    if (SFDialog)
        return;

    SFDialog = new SFBrowser(this, -1, Lang[224]); //Sub/Function Browser
    SFDialog->Show();

    return;
}




