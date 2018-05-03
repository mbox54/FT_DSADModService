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
#define VALUE_BIAS_MIN			0x00
#define VALUE_BIAS_MAX			0xFF
#define VALUE_BIAS_DEF			0x40

#define VALUE_MOD_MIN			0x0000
#define VALUE_MOD_MAX			0xFFFF
#define VALUE_MOD_DEF			0x1000

#define VALUE_HORISON_MIN		-127
#define VALUE_HORISON_MAX		127
#define VALUE_HORISON_DEF		0

#define VALUE_VERTICAL_MIN		0
#define VALUE_VERTICAL_MAX		127
#define VALUE_VERTICAL_DEF		64

#define ADDR_BIAS				0x08
#define ADDR_MOD				0x0B
#define ADDR_HORISON			0x02
#define ADDR_VERTICAL			0x01


// ##########################################################################
// CDS4830A_SFP_LX_CONF_ENGI dialog
// ##########################################################################

class CDS4830A_SFP_LX_CONF_ENGI : public CDialogEx
{
	DECLARE_DYNAMIC(CDS4830A_SFP_LX_CONF_ENGI)

public:
	// standard constructor
	CDS4830A_SFP_LX_CONF_ENGI(CWnd* pParent = NULL);   
	CDS4830A_SFP_LX_CONF_ENGI(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd* pParent = NULL);


	virtual ~CDS4830A_SFP_LX_CONF_ENGI();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_DS4830A_SFP_LX_CONF_ENGI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


protected:
	HID_SMBUS_DEVICE* m_pHidSmbus;

	// service data values
	st_serviceData* p_service;

	// main window status input
	CEdit* p_EDIT_STATUS;

	// main window status progress
	CProgressCtrl * p_cPB_OP;

private:
	// dialog internal grid operations
	BYTE uValues[256];

	// OnHScroll get 2 copies of msg. Second is needless.
	BYTE m_bOnHScrollDouble = 0;

protected:

	// Output sample
	void Trace(LPCTSTR szFmt, ...);

	// slider config
	void SliderInit();



public:
	CGridSFF_CP2112 m_Grid;

	CEdit m_Edit_Bias_H;
	CEdit m_Edit_Mod_H;
	CEdit m_Edit_Mod_L;

	void UpdateRead();
	void NoUpdateRead();

	// Grid events
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();


	CSliderCtrl m_Slider_BIAS;
	CSliderCtrl m_Slider_MOD;
	afx_msg void OnBnClickedButtonBiasSet();
	afx_msg void OnBnClickedButtonModSet();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
