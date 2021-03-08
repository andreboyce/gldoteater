//---------------------------------------------------------------------------
//
// Name:        sdts.h
// Author:      
// Created:     5/23/2005 9:09:36 PM
//
//---------------------------------------------------------------------------
#ifndef __SPECIALDATATYPESDIALOG_HPP_
#define __SPECIALDATATYPESDIALOG_HPP_

/*
// For compilers that support precompilation, includes "wx.h".
//#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
// Include your minimal set of headers here, or wx.h
#include <wx/wx.h>
#endif


//Do not add custom headers.
//wx-dvcpp designer will remove them
////Header Include Start
#include <wx/listctrl.h>
////Header Include End

#include <wx/dialog.h>

//Compatibility for 2.4.x code
#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif
#ifndef wxFIXED_MINSIZE
#define wxFIXED_MINSIZE 0
#endif

////Dialog Style Start
	#define THIS_DIALOG_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX |  wxCLOSE_BOX
////Dialog Style End

class SpecialDataTypesDialog : public wxDialog
{
public:
    SpecialDataTypesDialog( wxWindow *parent, wxWindowID id = 1, const wxString &title = _T("Special Data Types"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = THIS_DIALOG_STYLE);
    virtual ~SpecialDataTypesDialog();

public:

  //Do not add custom Control Declarations here.
  //wx-devcpp will remove them. Try adding the custom code 
  //after the block.
  ////GUI Control Declaration Start
	wxListCtrl *WxListCtrl2;
	wxListCtrl *WxListCtrl1;
  ////GUI Control Declaration End

private:
    DECLARE_EVENT_TABLE()

public:
    //Note: if you receive any error with these enums, then you need to
    //change your old form code that are based on the #define control ids.
    //#defines may replace a numeric value for the enums names.
    //Try copy pasting the below block in your old Form header Files.
	enum {
////GUI Enum Control ID Start
ID_WXLISTCTRL2 = 1005 ,
ID_WXLISTCTRL1 = 1003 ,
////GUI Enum Control ID End
   ID_DUMMY_VALUE_ //Dont Delete this DummyValue
   }; //End of Enum

public:
    void SpecialDataTypesDialogClose(wxCloseEvent& event);
    void CreateGUIControls(void);

};
*/

#endif
 
 
 
 
