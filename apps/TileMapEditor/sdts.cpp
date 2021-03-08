//---------------------------------------------------------------------------
//
// Name:        sdts.cpp
// Author:      
// Created:     5/23/2005 9:09:36 PM
//
//---------------------------------------------------------------------------
/*
#include "sdts.h"

//Do not add custom headers.
//wx-dvcpp designer will remove them
////Header Include Start
////Header Include End



//----------------------------------------------------------------------------
// SpecialDataTypesDialog
//----------------------------------------------------------------------------
     //Add Custom Events only in the appropriate Block.
    // Code added in  other places will be removed by wx-dvcpp 
    ////Event Table Start
BEGIN_EVENT_TABLE(SpecialDataTypesDialog,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(SpecialDataTypesDialog::SpecialDataTypesDialogClose)
END_EVENT_TABLE()
    ////Event Table End



SpecialDataTypesDialog::SpecialDataTypesDialog( wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style )
    : wxDialog( parent, id, title, position, size, style)
{
    CreateGUIControls();
}

SpecialDataTypesDialog::~SpecialDataTypesDialog() {} 

void SpecialDataTypesDialog::CreateGUIControls(void)
{
    //Do not add custom Code here
    //wx-devcpp designer will remove them.
    //Add the custom code before or after the Blocks
    ////GUI Items Creation Start

	this->SetSize(8,8,374,658);
	this->SetTitle(_("Special Data Types"));
	this->Center();
	this->SetIcon(wxNullIcon);
	

	WxListCtrl2 =  new wxListCtrl(this, ID_WXLISTCTRL2,wxPoint(45,11),wxSize(168,466)  , wxLC_REPORT);

	WxListCtrl1 =  new wxListCtrl(this, ID_WXLISTCTRL1,wxPoint(4,11),wxSize(41,466)  , wxLC_REPORT);
    ////GUI Items Creation End
}

void SpecialDataTypesDialog::SpecialDataTypesDialogClose(wxCloseEvent& event)
{
    // --> Don't use Close with a wxDialog,
    // use Destroy instead.
    Destroy();
}
*/ 
