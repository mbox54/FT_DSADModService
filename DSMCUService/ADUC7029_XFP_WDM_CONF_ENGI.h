#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "SLABCP2112.h"
#include "GridSFF_CP2112.h"
#include "afxwin.h"
#include "afxcmn.h"

/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////

#define OP_SUCCESS				0x00
#define OP_FAILED				0x01

// slider range
#define VALUE_BIAS_MIN			0x0000
#define VALUE_BIAS_MAX			0x03FF
#define VALUE_BIAS_DEF			0x01EE

#define VALUE_MOD_MIN			0x0000
#define VALUE_MOD_MAX			0x03FF
#define VALUE_MOD_DEF			0x0200

#define VALUE_HORISON_MIN		-127
#define VALUE_HORISON_MAX		127
#define VALUE_HORISON_DEF		0

#define VALUE_VERTICAL_MIN		0
#define VALUE_VERTICAL_MAX		127
#define VALUE_VERTICAL_DEF		64

#define ADDR_BIAS_HI			47
#define ADDR_BIAS_LO			46
#define ADDR_MOD_HI				38
#define ADDR_MOD_LO				37

#define ADDR_HORISON			0x02
#define ADDR_VERTICAL			0x01

/////////////////////////////////////////////////////////////////////////////
// Typedefs
/////////////////////////////////////////////////////////////////////////////
union unWORD_2BYTE
{
	WORD usValue;
	BYTE ucValue[2];
};



// ##########################################################################
// CADUC7029_XFP_WDM_CONF_ENGI dialog
// ##########################################################################

class CADUC7029_XFP_WDM_CONF_ENGI : public CDialog
{
	DECLARE_DYNAMIC(CADUC7029_XFP_WDM_CONF_ENGI)

public:
	CADUC7029_XFP_WDM_CONF_ENGI(CWnd* pParent = NULL);   // standard constructor
	CADUC7029_XFP_WDM_CONF_ENGI(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd* pParent = NULL);

	virtual ~CADUC7029_XFP_WDM_CONF_ENGI();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_ADUC7029_XFP_WDM_CONF_ENGI };
#endif

protected:
	HID_SMBUS_DEVICE* m_pHidSmbus;

	// service data values
	st_serviceData* p_service;

	// main window status input
	CEdit* p_EDIT_STATUS;

	// main window status progress
	CProgressCtrl * p_cPB_OP;

	CString m_sEdit_PassValue;
	CString m_Edit_Filename;

	// slider
	CSliderCtrl m_Slider_BIAS;
	CSliderCtrl m_Slider_MOD;

	// edit
	CEdit m_Edit_Bias_H;
	CEdit m_Edit_Bias_L;
	CEdit m_Edit_Mod_H;
	CEdit m_Edit_Mod_L;


private:
	// dialog internal grid operations
	BYTE uValues[256];

	// OnHScroll get 2 copies of msg. Second is needless.
	BYTE m_bOnHScrollDouble = 0;

	// !debug
	BYTE uValues2[256];


	// > Function prototypes
	// slider config
	void SliderInit();



protected:

	// Output sample
	void Trace(LPCTSTR szFmt, ...);

	void EditHexControl(CEdit *pEdit);

	// Table File OP
	int TableValues_LoadFromFile(char * v_filename, BYTE * v_ByteData);
	int TableValues_SaveToFile(BYTE * v_ByteData);

	// Table OPs
	void NoUpdateRead();
	void UpdateRead();



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


public:
	CGridSFF_CP2112 m_Grid;


	virtual BOOL OnInitDialog();

	// Grid events
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);

	// Control events 
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButtonBiasSet();
	afx_msg void OnBnClickedButtonModSet();
	afx_msg void OnBnClickedButtonHorSet();
	afx_msg void OnBnClickedButtonVerticalSet();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
