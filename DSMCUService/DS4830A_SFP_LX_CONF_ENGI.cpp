// DS4830A_SFP_LX_CONF_ENGI.cpp : implementation file
//

#include "stdafx.h"
#include "DSMCUService.h"
#include "DS4830A_SFP_LX_CONF_ENGI.h"
#include "afxdialogex.h"


// CDS4830A_SFP_LX_CONF_ENGI dialog

IMPLEMENT_DYNAMIC(CDS4830A_SFP_LX_CONF_ENGI, CDialogEx)

CDS4830A_SFP_LX_CONF_ENGI::CDS4830A_SFP_LX_CONF_ENGI(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROPPAGE_DS4830A_SFP_LX_CONF_ENGI, pParent)
{

}


CDS4830A_SFP_LX_CONF_ENGI::CDS4830A_SFP_LX_CONF_ENGI(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd * pParent)
	: CDialogEx(IDD_PROPPAGE_DS4830A_SFP_LX_CONF_ENGI, pParent)
, m_pHidSmbus(pHidSmbus)
, p_EDIT_STATUS(p_EDIT_STATUS)
, p_cPB_OP(p_cPB_OP)
, p_service(p_service)
, m_Grid(m_pHidSmbus, p_cPB_OP, p_EDIT_STATUS, p_service)
{

}


CDS4830A_SFP_LX_CONF_ENGI::~CDS4830A_SFP_LX_CONF_ENGI()
{

}


void CDS4830A_SFP_LX_CONF_ENGI::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_EDIT_BIAS_H, m_Edit_Bias_H);
	DDX_Control(pDX, IDC_EDIT_MOD_H, m_Edit_Mod_H);
	DDX_Control(pDX, IDC_EDIT_MOD_L, m_Edit_Mod_L);
	DDX_Control(pDX, IDC_SLIDER_BIAS, m_Slider_BIAS);
	DDX_Control(pDX, IDC_SLIDER_MOD, m_Slider_MOD);
}


BEGIN_MESSAGE_MAP(CDS4830A_SFP_LX_CONF_ENGI, CDialogEx)

	// Grid event functions
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)

	ON_BN_CLICKED(IDC_BUTTON4, &CDS4830A_SFP_LX_CONF_ENGI::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDS4830A_SFP_LX_CONF_ENGI::OnBnClickedButton5)

	ON_BN_CLICKED(IDC_BUTTON_BIAS_SET, &CDS4830A_SFP_LX_CONF_ENGI::OnBnClickedButtonBiasSet)
	ON_BN_CLICKED(IDC_BUTTON_MOD_SET, &CDS4830A_SFP_LX_CONF_ENGI::OnBnClickedButtonModSet)

	ON_WM_HSCROLL()
	ON_WM_VSCROLL()

END_MESSAGE_MAP()


// CDS4830A_SFP_LX_CONF_ENGI message handlers


void CDS4830A_SFP_LX_CONF_ENGI::Trace(LPCTSTR szFmt, ...)
{
	CString str;

	// Format the message text
	va_list argptr;
	va_start(argptr, szFmt);
	str.FormatV(szFmt, argptr);
	va_end(argptr);

	str.Replace(_T("\n"), _T("\r\n"));

	CString strWndText;
	p_EDIT_STATUS->GetWindowText(strWndText);
	strWndText += str;
	p_EDIT_STATUS->SetWindowText(strWndText);

	//	m_TraceWnd.SetSel(str.GetLength()-1, str.GetLength()-2, FALSE);
	p_EDIT_STATUS->LineScroll(-p_EDIT_STATUS->GetLineCount());
	p_EDIT_STATUS->LineScroll(p_EDIT_STATUS->GetLineCount() - 4);
}

void CDS4830A_SFP_LX_CONF_ENGI::SliderInit()
{
	// var for label OP
	CString str;

	// BIAS
	// slider
	m_Slider_BIAS.SetRange(VALUE_BIAS_MIN, VALUE_BIAS_MAX);
	m_Slider_BIAS.SetPos(VALUE_BIAS_DEF);

	// label
	// get static components
	CWnd *pStaticBIAS = this->GetDlgItem(IDC_STATIC_BIAS);
	CWnd *pStaticBIAS_MIN = this->GetDlgItem(IDC_STATIC_BIAS_MIN);
	CWnd *pStaticBIAS_MAX = this->GetDlgItem(IDC_STATIC_BIAS_MAX);

	// set Values
	str.Format(L"%d", VALUE_BIAS_MIN);			// Set & Convert
	pStaticBIAS_MIN->SetWindowTextW(str);		// Out to Interface control 

	str.Format(L"%d", VALUE_BIAS_MAX);			// Set & Convert
	pStaticBIAS_MAX->SetWindowTextW(str);		// Out to Interface control 

	str.Format(L"%d", VALUE_BIAS_DEF);			// Set & Convert
	pStaticBIAS->SetWindowTextW(str);			// Out to Interface control 

												// edit 
												// config
	this->m_Edit_Bias_H.LimitText(2);
	//this->m_Edit_Bias_L.LimitText(2);

	//// fill with Value
	//// // Get ByteH and ByteL from WORD
	//unsigned short _word;
	//unsigned char byte_H;
	//unsigned char byte_L;

	//// // // ByteH
	//_word = VALUE_BIAS_DEF;
	//_word &= 0xFF00;
	//_word = _word >> 8;
	//byte_H = (BYTE)_word;

	//// // // ByteL
	//_word = VALUE_BIAS_DEF;
	//_word &= 0x00FF;
	//byte_L = (BYTE)_word;

	// // place Values
	CString str_hex;
	str_hex.Format(_T("%02X"), VALUE_BIAS_DEF);
	this->m_Edit_Bias_H.SetWindowTextW(str_hex);

	//str_hex.Truncate(0);

	//str_hex.Format(_T("%02X"), byte_L);
	//this->m_Edit_Bias_L.SetWindowTextW(str_hex);


	// MOD
	// slider
	m_Slider_MOD.SetRange(VALUE_MOD_MIN, VALUE_MOD_MAX);
	m_Slider_MOD.SetPos(VALUE_MOD_DEF);

	// get static components
	CWnd *pStaticMOD = this->GetDlgItem(IDC_STATIC_MOD);
	CWnd *pStaticMOD_MIN = this->GetDlgItem(IDC_STATIC_MOD_MIN);
	CWnd *pStaticMOD_MAX = this->GetDlgItem(IDC_STATIC_MOD_MAX);

	// set Values
	str.Format(L"%d", VALUE_MOD_MIN);			// Set & Convert
	pStaticMOD_MIN->SetWindowTextW(str);		// Out to Interface control 

	str.Format(L"%d", VALUE_MOD_MAX);			// Set & Convert
	pStaticMOD_MAX->SetWindowTextW(str);		// Out to Interface control 

	str.Format(L"%d", VALUE_MOD_DEF);			// Set & Convert
	pStaticMOD->SetWindowTextW(str);			// Out to Interface control 

												// edit 
												// config
	this->m_Edit_Mod_H.LimitText(2);
	this->m_Edit_Mod_L.LimitText(2);

	// fill with Value	
	// Get ByteH and ByteL from WORD
	unsigned int _word;
	unsigned char byte_H;
	unsigned char byte_L;

	// // // ByteH
	_word = VALUE_MOD_DEF;
	_word &= 0xFF00;
	_word = _word >> 8;
	byte_H = (BYTE)_word;

	// // // ByteL
	_word = VALUE_MOD_DEF;
	_word &= 0x00FF;
	byte_L = (BYTE)_word;

	// // place Values
	//CString str_hex;
	str_hex.Format(_T("%02X"), byte_H);
	this->m_Edit_Mod_H.SetWindowTextW(str_hex);

	str_hex.Truncate(0);

	str_hex.Format(_T("%02X"), byte_L);
	this->m_Edit_Mod_L.SetWindowTextW(str_hex);


	//// HORISONTAL
	//// slider
	//m_Slider_HORISON.SetRange(VALUE_HORISON_MIN, VALUE_HORISON_MAX);
	//m_Slider_HORISON.SetPos(VALUE_HORISON_DEF);

	//// get static components
	//CWnd *pStaticHOR = this->GetDlgItem(IDC_STATIC_HOR);
	//CWnd *pStaticHOR_MIN = this->GetDlgItem(IDC_STATIC_HOR_MIN2);
	//CWnd *pStaticHOR_MAX = this->GetDlgItem(IDC_STATIC_HOR_MAX);

	//// set Values
	//str.Format(L"%d", VALUE_HORISON_MIN);		// Set & Convert
	//pStaticHOR_MIN->SetWindowTextW(str);		// Out to Interface control 

	//str.Format(L"%d", VALUE_HORISON_MAX);		// Set & Convert
	//pStaticHOR_MAX->SetWindowTextW(str);		// Out to Interface control 

	//str.Format(L"%d", VALUE_HORISON_DEF);		// Set & Convert
	//pStaticHOR->SetWindowTextW(str);			// Out to Interface control 

	//											// edit 
	//											// config
	//this->m_Edit_Horison.LimitText(2);

	//// Out to Interface control Edits
	//// Format Value
	//// FORMAT:
	//// 1Byte = bits:: data:7654321, direction:0

	//// define direction
	//// define Value
	//char direction;
	//unsigned char iPosFormat;

	//char iPos = VALUE_HORISON_DEF;

	//if (iPos < 0)
	//{
	//	direction = 1;
	//	iPosFormat = iPos * (-1);
	//}
	//else
	//{
	//	direction = 0;
	//	iPosFormat = iPos;
	//}

	//iPosFormat = iPosFormat << 1;
	//iPosFormat &= 0xFE;
	//iPosFormat += direction;

	//// Out to Interface control Edits
	//// // Convert to String Hex
	//// // Out in Edits
	////CString str_hex;
	//str_hex.Format(_T("%02X"), iPosFormat);
	//this->m_Edit_Horison.SetWindowTextW(str_hex);

	//// ! bug correct: init with bad position
	//m_Slider_HORISON.SetPos(VALUE_HORISON_DEF - 20);

	//// VERTICAL
	//// slider
	//m_Slider_VERTICAL.SetRange(VALUE_VERTICAL_MIN, VALUE_VERTICAL_MAX);
	//m_Slider_VERTICAL.SetPos(VALUE_VERTICAL_DEF);

	//// get static components
	//CWnd *pStaticVERT = this->GetDlgItem(IDC_STATIC_VERT);
	//CWnd *pStaticVERT_MIN = this->GetDlgItem(IDC_STATIC_VERT_MIN);
	//CWnd *pStaticVERT_MAX = this->GetDlgItem(IDC_STATIC_VERT_MAX);

	//// set Values
	//str.Format(L"%d", VALUE_VERTICAL_MIN);		// Set & Convert
	//pStaticVERT_MIN->SetWindowTextW(str);		// Out to Interface control 

	//str.Format(L"%d", VALUE_VERTICAL_MAX);		// Set & Convert
	//pStaticVERT_MAX->SetWindowTextW(str);		// Out to Interface control 

	//str.Format(L"%d", VALUE_VERTICAL_DEF);		// Set & Convert
	//pStaticVERT->SetWindowTextW(str);			// Out to Interface control 


	//											// checkbox
	//this->m_bCheck_Vertical = TRUE;
	//UpdateData(FALSE);

	//// edit 
	//// config
	//this->m_Edit_Vertical.LimitText(2);

	//// Out to Interface control Edits
	//// Format Value
	//// FORMAT:
	//// 1Byte = bits:: data:7654321, active:0

	//// define Status
	//unsigned char iStatus;

	//UpdateData(TRUE);
	//if (m_bCheck_Vertical)
	//{
	//	iStatus = 1;
	//}
	//else
	//{
	//	iStatus = 0;
	//}

	//// define Value
	////unsigned char iPosFormat;

	//iPosFormat = iPos;
	//iPosFormat = iPosFormat << 1;
	//iPosFormat &= 0xFE;
	//iPosFormat += iStatus;

	//// Out to Interface control Edits
	//// // Convert to String Hex
	//// // Out in Edits
	////CString str_hex;
	//str_hex.Format(_T("%02X"), iPosFormat);
	//this->m_Edit_Vertical.SetWindowTextW(str_hex);

	//// ! bug correct: init with bad position
	//m_Slider_HORISON.SetPos(VALUE_VERTICAL_DEF + 1);
	//m_Slider_HORISON.SetPos(VALUE_HORISON_DEF + 1);

	// > Update interface for Write controls
	UpdateData(FALSE);
}

void CDS4830A_SFP_LX_CONF_ENGI::OnBnClickedButton4()
{
	UpdateRead();
}


void CDS4830A_SFP_LX_CONF_ENGI::OnBnClickedButton5()
{
	Trace(_T("попытка: ОБНОВЛЕНИЕ \n"));
	Trace(_T("параметры: A2-T02-NOPASS, 80 байт\n"));

	// Valid ConfigTable Values
	unsigned char v_TablName[1] = { 0x02 };
	////	unsigned char v_TablPass[4] = { 'O', 'P', 'W', 'Y' };
	//unsigned char v_TablPass[4]; // = { 0x00, 0x11, 0x22, 0x33 };


	//							 // get Password 4Bytes
	//UpdateData(TRUE);

	//CString strHex;

	//for (unsigned char k = 0; k < 4; k++)
	//{
	//	char cPassLetter[2];
	//	cPassLetter[0] = m_sEdit_PassValue[k * 2];
	//	cPassLetter[1] = m_sEdit_PassValue[k * 2 + 1];

	//	strHex.AppendChar(cPassLetter[0]);
	//	strHex.AppendChar(cPassLetter[1]);

	//	// convert to Byte
	//	unsigned char byte_passLetter;
	//	byte_passLetter = (BYTE)_tcstoul(strHex, NULL, 16);

	//	v_TablPass[k] = byte_passLetter;

	//	strHex.Truncate(0);
	//}

	// progress component
	p_cPB_OP->SetPos(0);

	// send Table
	// write tabl in Device

	Sleep(30);

	BYTE retVal = m_Grid.DeviceSlave_Write(v_TablName, SLAVEADDR_A2, 0x7F, 1);
	if (retVal != HID_SMBUS_SUCCESS)
	{
		Trace(_T("ОШИБКА. [код: %02d] \n"), retVal);
		Trace(_T("НА ЭТАПЕ: ВЫБОР ТАБЛИЦЫ \n"), retVal);
	}

	//if (retVal == HID_SMBUS_SUCCESS)
	//{
	//	Sleep(30);

	//	p_cPB_OP->SetPos(20);

	//	// send password
	//	// write pass in Device
	//	retVal = m_Grid.DeviceSlave_Write(v_TablPass, SLAVEADDR_A2, 0x7B, 4);

	//	if (retVal != HID_SMBUS_SUCCESS)
	//	{
	//		Trace(_T("ОШИБКА. [код: %02d] \n"), retVal);
	//		Trace(_T("НА ЭТАПЕ: ВВОД ПАРОЛЯ \n"), retVal);
	//	}

		Sleep(30);

		p_cPB_OP->SetPos(40);
	//}

	if (retVal == HID_SMBUS_SUCCESS)
	{
		unsigned char v_GridVal_T02[80];	
		unsigned char v_WrByte8[8];

		// get Values
		m_Grid.GridSFF_Read(v_GridVal_T02, 0xA0, 80);

		// Write to Device
		// NOTE:
		// CAN'T BE WRITTEN IN 1 OP. 
		// That's why written in 8-byte regime
		for (int k = 0; k < 10; k++)
		{
			// prepare write buffer
			for (int k2 = 0; k2 < 8; k2++)
			{
				v_WrByte8[k2] = v_GridVal_T02[k * 8 + k2];
			}

			// i2c write
			retVal = m_Grid.DeviceSlave_Write(v_WrByte8, SLAVEADDR_A2, 0xA0 + k * 8, 8);

			if (retVal != HID_SMBUS_SUCCESS)
			{
				// error: Device Read
				Trace(_T("ОШИБКА. [код: %02d] \n"), retVal);
				Trace(_T("НА ЭТАПЕ: ОБНОВЛЕНИЕ ТАБЛИЦЫ \n"), retVal);

				break;
			}

			p_cPB_OP->SetPos(40 + k * 7);

			Sleep(10);

		}

	}

	if (retVal != HID_SMBUS_SUCCESS)
	{
		CString str_ErrText;
		switch (retVal)
		{
		case 1:
			str_ErrText.AppendFormat(L"(MAXQBL_NO_DEVICE)");

			break;

		case 2:
			str_ErrText.AppendFormat(L"(MAXQBL_DEV_INACCESSIBLE)");

			break;

		case 3:
			str_ErrText.AppendFormat(L"(MAXQBL_OP_FAILED)");

			break;

		case 4:
			str_ErrText.AppendFormat(L"(MAXQBL_OP_WRONG)");

			break;

		default:
			break;
		}

		// err comment output
		Trace(str_ErrText, '\n');
		Trace(_T("-----------------------\n"));

		return;
	}

	Trace(_T("УСПЕШНО.\n"));
	Trace(_T("-----------------------\n"));

	p_cPB_OP->SetPos(100);
	//	m_Grid.DeviceSlave_WriteTimer(uValues2, 0, SLAVEADDR_A2, 128 + 8, 8, 0, 0);
}


void CDS4830A_SFP_LX_CONF_ENGI::UpdateRead()
{
	// > Read Table
	NoUpdateRead();

	// > Update Output Controls
	// > > Get Table Values
	BYTE uValues[80];
	m_Grid.GridSFF_Read(uValues, 0xA0, 80);

	// > > Set Sliders Position
	// [SLIDER BIAS]
	// Get Value
	BYTE ucByte = uValues[ADDR_BIAS];

	// Set Slider Control
	this->m_Slider_BIAS.SetPos(ucByte);

	// Set Static Control
	CString strValue;
	strValue.Format(L"%d", ucByte);

	CWnd *pStaticBIAS = this->GetDlgItem(IDC_STATIC_BIAS);
	pStaticBIAS->SetWindowTextW(strValue);

	// Set Edit Control
	strValue.Truncate(0);
	strValue.Format(_T("%02X"), ucByte);
	this->m_Edit_Bias_H.SetWindowTextW(strValue);


	// [SLIDER MOD]
	// Get Value
	un_WordAndBytes unComplexValue;

	unComplexValue.strucValue.ucByte02 = uValues[ADDR_MOD];
	unComplexValue.strucValue.ucByte01 = uValues[ADDR_MOD + 1];

	// Set Slider Control
	this->m_Slider_MOD.SetPos(unComplexValue.usWordValue);

	// Set Static Control
	strValue.Truncate(0);
	strValue.Format(L"%d", unComplexValue.usWordValue);

	CWnd *pStaticMOD = this->GetDlgItem(IDC_STATIC_MOD);
	pStaticMOD->SetWindowTextW(strValue);

	// Set Edit Control
	strValue.Truncate(0);
	strValue.Format(_T("%02X"), unComplexValue.strucValue.ucByte02);
	this->m_Edit_Mod_H.SetWindowTextW(strValue);

	strValue.Truncate(0);
	strValue.Format(_T("%02X"), unComplexValue.strucValue.ucByte01);
	this->m_Edit_Mod_L.SetWindowTextW(strValue);


	//// [SLIDER HORISON]
	//// Get Value
	//BYTE ucByte = uValues[0x80 + ADDR_HORISON];

	//// define direction
	//// define Value
	//BYTE bPosDirection;
	//char cPosValue;

	//bPosDirection = ucByte >> 1;
	//cPosValue = bPosDirection;

	//bPosDirection = ucByte;
	//bPosDirection &= 0x01;
	//if (bPosDirection == 0)
	//{
	//	// [POS]
	//	//		
	//}
	//else
	//{
	//	// [NEG]

	//	cPosValue *= (-1);
	//}

	//// Set Slider Control
	//this->m_Slider_HORISON.SetPos(cPosValue);

	//// Set Static Control
	//strValue.Truncate(0);
	//strValue.Format(L"%d", cPosValue);

	//CWnd *pStaticHOR = this->GetDlgItem(IDC_STATIC_HOR);
	//pStaticHOR->SetWindowTextW(strValue);

	//// Set Edit Control
	//strValue.Truncate(0);
	//strValue.Format(_T("%02X"), ucByte);
	//this->m_Edit_Horison.SetWindowTextW(strValue);

	//// [SLIDER VERTICAL]
	//// Get Value
	//ucByte = uValues[0x80 + ADDR_VERTICAL];

	//// define state: active/deactive
	//// define Value
	//BYTE bPosState = ucByte >> 1;
	//cPosValue = bPosState;

	//bPosState = ucByte;
	//bPosState &= 0x01;

	//CWnd *wndSlide = this->GetDlgItem(IDC_SLIDER_VERTICAL);
	//if (bPosState == 0)
	//{
	//	// [DEACTICE]

	//	// set CheckBox Control
	//	m_bCheck_Vertical = 0;

	//	// disable Control
	//	wndSlide->EnableWindow(FALSE);
	//}
	//else
	//{
	//	// [ACTIVE]

	//	// set CheckBox Control
	//	m_bCheck_Vertical = 1;

	//	// enadle Control
	//	wndSlide->EnableWindow(TRUE);
	//}

	//// Set Slider Control
	//this->m_Slider_VERTICAL.SetPos(cPosValue);

	//// Set Static Control
	//strValue.Truncate(0);
	//strValue.Format(L"%d", cPosValue);

	//CWnd *pStaticVERT = this->GetDlgItem(IDC_STATIC_VERT);
	//pStaticVERT->SetWindowTextW(strValue);

	//// Set Edit Control
	//strValue.Truncate(0);
	//strValue.Format(_T("%02X"), ucByte);
	//this->m_Edit_Vertical.SetWindowTextW(strValue);

	// > Update interface for Write controls
	UpdateData(FALSE);
}

void CDS4830A_SFP_LX_CONF_ENGI::NoUpdateRead()
{
	Trace(_T("попытка: ЧТЕНИЕ\n"));
	Trace(_T("параметры: A2-T02-NOPASS, 80 байт\n"));


	// Valid ConfigTable Values
	unsigned char v_TablName[1] = { 0x02 };

	////	unsigned char v_TablPass[4] = { 'O', 'P', 'W', 'Y' };
	//unsigned char v_TablPass[4]; // = { 0x00, 0x11, 0x22, 0x33 };


	//							 // get Password 4Bytes
	//UpdateData(TRUE);

	CString strHex;

	//for (unsigned char k = 0; k < 4; k++)
	//{
	//	char cPassLetter[2];
	//	cPassLetter[0] = m_sEdit_PassValue[k * 2];
	//	cPassLetter[1] = m_sEdit_PassValue[k * 2 + 1];

	//	strHex.AppendChar(cPassLetter[0]);
	//	strHex.AppendChar(cPassLetter[1]);

	//	// convert to Byte
	//	unsigned char byte_passLetter;
	//	byte_passLetter = (BYTE)_tcstoul(strHex, NULL, 16);

	//	v_TablPass[k] = byte_passLetter;

	//	strHex.Truncate(0);
	//}

	unsigned char v_WrByte[1];

	// progress component
	p_cPB_OP->SetPos(0);

	// send Table
	// write tabl in Device 
	BYTE retVal = m_Grid.DeviceSlave_Write(v_TablName, SLAVEADDR_A2, 0x7F, 1);
	if (retVal != HID_SMBUS_SUCCESS)
	{
		Trace(_T("ОШИБКА. [код: %02d] \n"), retVal);
		Trace(_T("НА ЭТАПЕ: ВЫБОР ТАБЛИЦЫ \n"), retVal);
	}

	//if (retVal == HID_SMBUS_SUCCESS)
	//{
	//	Sleep(30);

	//	p_cPB_OP->SetPos(30);

	//	// send password
	//	// write pass in Device
	//	for (unsigned char k = 0; k < 4; k++)
	//	{
	//		v_WrByte[0] = v_TablPass[k];
	//		retVal = m_Grid.DeviceSlave_Write(v_WrByte, SLAVEADDR_A2, 0x7B + k, 1);

	//		if (retVal != HID_SMBUS_SUCCESS)
	//		{
	//			Trace(_T("ОШИБКА. [код: %02d] \n"), retVal);
	//			Trace(_T("НА ЭТАПЕ: ВВОД ПАРОЛЯ \n"), retVal);

	//			break;
	//		}

	//		Sleep(10);

	//		p_cPB_OP->SetPos(30 + 5 * k);
	//	}
	//}

	p_cPB_OP->SetPos(20);

	BYTE v_Values[80];

	if (retVal == HID_SMBUS_SUCCESS)
	{
		// Read op
		Sleep(30);

		p_cPB_OP->SetPos(80);

		//m_Grid.DeviceSlave_ReadTimer(uValues2, 0, SLAVEADDR_A2, 0, 256, 0, 0);
		retVal = m_Grid.DeviceSlave_Read(v_Values, SLAVEADDR_A2, 0xA0, 80);

		if (retVal != HID_SMBUS_SUCCESS)
		{
			// error: Device Read
			Trace(_T("ОШИБКА. [код: %02d] \n"), retVal);
			Trace(_T("НА ЭТАПЕ: ЗАГРУЗКА ТАБЛИЦЫ \n"), retVal);
		}
	}

	if (retVal != HID_SMBUS_SUCCESS)
	{
		CString str_ErrText;
		switch (retVal)
		{
		case 1:
			str_ErrText.AppendFormat(L"(MAXQBL_NO_DEVICE)");

			break;

		case 2:
			str_ErrText.AppendFormat(L"(MAXQBL_DEV_INACCESSIBLE)");

			break;

		case 3:
			str_ErrText.AppendFormat(L"(MAXQBL_OP_FAILED)");

			break;

		case 4:
			str_ErrText.AppendFormat(L"(MAXQBL_OP_WRONG)");

			break;

		default:
			break;
		}

		// err comment output
		Trace(str_ErrText, '\n');
		Trace(_T("-----------------------\n"));

		return;
	}

	Sleep(30);

	Trace(_T("УСПЕШНО.\n"));
	Trace(_T("-----------------------\n"));

	m_Grid.ClearTable();
	m_Grid.GridSFF_Write(v_Values, 0xA0, 80);

	p_cPB_OP->SetPos(100);

}

void CDS4830A_SFP_LX_CONF_ENGI::OnGridClick(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	//Trace(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
	//Trace(_T("Text: %s\n"), m_Grid.GetItemText(pItem->iRow, pItem->iColumn));
	//Item.strText
}


void CDS4830A_SFP_LX_CONF_ENGI::OnGridEndEdit(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	//Trace(_T("End Edit on row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	// > check Valid Input
	bool bInvalid = m_Grid.CheckValidHex(pItem->iRow, pItem->iColumn);

	*pResult = (bInvalid || m_Grid.m_bRejectEditChanges) ? -1 : 0;

	//Trace(_T("Res: %d\n"), bInvalid && m_Grid.m_bRejectEditChanges);
}


BOOL CDS4830A_SFP_LX_CONF_ENGI::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_Grid.Init();

	// > Sliders
	SliderInit();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDS4830A_SFP_LX_CONF_ENGI::OnBnClickedButtonBiasSet()
{
	// > Get Value
	CString str;
	CString str_edit;

	// > proceed FIRST byte
	this->m_Edit_Bias_H.GetWindowTextW(str_edit);

	// > > Check null Value
	if (str_edit.GetLength() == 0)
	{
		// raise warning
		int msgboxID = MessageBox(
			(LPCWSTR)L"Операция не выполнена!\nОдно из полей ввода пусто.",
			(LPCWSTR)L"Set Value",
			MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
		);

		// abort
		return;
	}

	// convert to Byte
	unsigned char byte_H;
	byte_H = (BYTE)_tcstoul(str_edit, NULL, 16);

	// > > align Hex String to Valid format
	if (str_edit.GetLength() == 1)
	{
		CString strNew;
		strNew.AppendChar('0');

		strNew.Append(str_edit);
	}

	// > > Add to string
	str.Append(str_edit);

	//// > proceed SECOND byte
	////this->m_Edit_Bias_L.GetWindowTextW(str_edit);

	//// > > Check null Value
	//if (str_edit.GetLength() == 0)
	//{
	//	// raise warning
	//	int msgboxID = MessageBox(
	//		(LPCWSTR)L"Операция не выполнена!\nОдно из полей ввода пусто.",
	//		(LPCWSTR)L"Set Value",
	//		MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
	//	);

	//	// abort
	//	return;
	//}

	//// convert to Byte
	//unsigned char byte_L;
	//byte_L = (BYTE)_tcstoul(str_edit, NULL, 16);

	//// > > align Hex String to Valid format
	//if (str_edit.GetLength() == 1)
	//{
	//	CString strNew;
	//	strNew.AppendChar('0');

	//	strNew.Append(str_edit);
	//}

	//// > > Add to string
	//str.Append(str_edit);

	//// > Make INT from String HEX
	//unsigned int _word;

	//_word = (WORD)_tcstoul(str, NULL, 16);

	/*
	// Check Valid range
	if (_word < VALUE_BIAS_MIN)
	{
	// raise warning
	int msgboxID = MessageBox(
	(LPCWSTR)L"Операция не выполнена!\nУстановленное значение выходит за рамки допустимого диапазона.",
	(LPCWSTR)L"Set Value",
	MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
	);

	// abort
	return;
	}

	if (_word > VALUE_BIAS_MAX)
	{
	// raise warning
	int msgboxID = MessageBox(
	(LPCWSTR)L"Операция не выполнена!\nУстановленное значение выходит за рамки допустимого диапазона.",
	(LPCWSTR)L"Set Value",
	MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
	);

	// abort
	return;
	}
	*/

	// > Write Value in Grid
	// get Grid Values
	unsigned char v_Value[80];

	this->m_Grid.GridSFF_Read(v_Value, 0xA0, 80);

	// put Value
	v_Value[ADDR_BIAS] = byte_H;
	//uValue[0x80 + ADDR_BIAS + 1] = byte_L;

	// set Grid Values
	this->m_Grid.GridSFF_Write(v_Value, 0xA0, 80);

}


void CDS4830A_SFP_LX_CONF_ENGI::OnBnClickedButtonModSet()
{
	// > Get Value
	CString str;
	CString str_edit;

	// > proceed FIRST byte
	this->m_Edit_Mod_H.GetWindowTextW(str_edit);

	// > > Check null Value
	if (str_edit.GetLength() == 0)
	{
		// raise warning
		int msgboxID = MessageBox(
			(LPCWSTR)L"Операция не выполнена!\nОдно из полей ввода пусто.",
			(LPCWSTR)L"Set Value",
			MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
		);

		// abort
		return;
	}

	// convert to Byte
	unsigned char byte_H;
	byte_H = (BYTE)_tcstoul(str_edit, NULL, 16);

	// > > align Hex String to Valid format
	if (str_edit.GetLength() == 1)
	{
		CString strNew;
		strNew.AppendChar('0');

		strNew.Append(str_edit);
	}

	// > > Add to string
	str.Append(str_edit);

	// > proceed SECOND byte
	this->m_Edit_Mod_L.GetWindowTextW(str_edit);

	// > > Check null Value
	if (str_edit.GetLength() == 0)
	{
		// raise warning
		int msgboxID = MessageBox(
			(LPCWSTR)L"Операция не выполнена!\nОдно из полей ввода пусто.",
			(LPCWSTR)L"Set Value",
			MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
		);

		// abort
		return;
	}

	// convert to Byte
	unsigned char byte_L;
	byte_L = (BYTE)_tcstoul(str_edit, NULL, 16);

	// > > align Hex String to Valid format
	if (str_edit.GetLength() == 1)
	{
		CString strNew;
		strNew.AppendChar('0');

		strNew.Append(str_edit);
	}

	// > > Add to string
	str.Append(str_edit);

	// > Make INT from String HEX
	unsigned int _word;

	_word = (WORD)_tcstoul(str, NULL, 16);

	/*
	// Check Valid range
	if (_word < VALUE_MOD_MIN)
	{
	// raise warning
	int msgboxID = MessageBox(
	(LPCWSTR)L"Операция не выполнена!\nУстановленное значение выходит за рамки допустимого диапазона.",
	(LPCWSTR)L"Set Value",
	MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
	);

	// abort
	return;
	}

	if (_word > VALUE_MOD_MAX)
	{
	// raise warning
	int msgboxID = MessageBox(
	(LPCWSTR)L"Операция не выполнена!\nУстановленное значение выходит за рамки допустимого диапазона.",
	(LPCWSTR)L"Set Value",
	MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
	);

	// abort
	return;
	}
	*/


	// > Write Value in Grid
	// get Grid Values
	unsigned char v_Value[80];

	this->m_Grid.GridSFF_Read(v_Value, 0xA0, 80);

	// put Value
	v_Value[ADDR_MOD] = byte_H;
	v_Value[ADDR_MOD + 1] = byte_L;

	// set Grid Values
	this->m_Grid.GridSFF_Write(v_Value, 0xA0, 80);
}


void CDS4830A_SFP_LX_CONF_ENGI::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// get Slider components
	CWnd *pSliderBIAS = this->GetDlgItem(IDC_SLIDER_BIAS);
	CWnd *pSliderMOD = this->GetDlgItem(IDC_SLIDER_MOD);

	if (pScrollBar == pSliderBIAS)			//  BIAS Slider
	{
		// get Static components
		CWnd *pStaticBIAS = this->GetDlgItem(IDC_STATIC_BIAS);
		CWnd *pStaticMOD = this->GetDlgItem(IDC_STATIC_MOD);

		// > Update contents for Read controls
		UpdateData(TRUE);

		// > Control MAIN slider
		// Get current Slider position
		int iPos1 = this->m_Slider_BIAS.GetPos();

		// convert to str
		CString str1;
		str1.Format(L"%d", iPos1);

		// Out to Interface control Label
		pStaticBIAS->SetWindowTextW(str1);

		// Out to Interface control Edits
		// // Get ByteH and ByteL from WORD
		unsigned short _word;
		unsigned char byte_H;
		unsigned char byte_L;

		byte_H = iPos1;
		// // // ByteH
		//_word = iPos1;
		//_word &= 0xFF00;
		//_word = _word >> 8;
		//byte_H = (BYTE)_word;

		//// // // ByteL
		//_word = iPos1;
		//_word &= 0x00FF;
		//byte_L = (BYTE)_word;

		// // Convert to String Hex
		// // Out in Edits
		CString str_hex;
		str_hex.Format(_T("%02X"), byte_H);
		this->m_Edit_Bias_H.SetWindowTextW(str_hex);

		str_hex.Truncate(0);

		//str_hex.Format(_T("%02X"), byte_L);
		//this->m_Edit_Bias_L.SetWindowTextW(str_hex);

		//// > Control SECONDARY slider
		//// check limit event
		//int iMin = this->m_Slider_MOD.GetRangeMin();
		//int iMax = this->m_Slider_MOD.GetRangeMax();
		//int iPos2 = 0;

		//if (iPos1 < iMin)
		//{
		//	iPos2 = iMin;
		//}
		//else
		//{
		//	if (iPos1 * 1.5 > iMax)
		//	{
		//		iPos2 = iMax;
		//	}
		//	else
		//	{
		//		iPos2 = iPos1 * 1.5;
		//	}
		//}


		//// set SECONDARY Slider position
		//m_Slider_MOD.SetPos(iPos2);

		//// convert to str
		//CString str2;
		//str2.Format(L"%d", iPos2);

		//// Out to Interface control 
		//pStaticMOD->SetWindowTextW(str2);

		//// Out to Interface control Edits
		//// // Get ByteH and ByteL from WORD
		////unsigned int _word;
		////unsigned char byte_H;
		////unsigned char byte_L;

		//// // // ByteH
		//_word = iPos2;
		//_word &= 0xFF00;
		//_word = _word >> 8;
		//byte_H = (BYTE)_word;

		//// // // ByteL
		//_word = iPos2;
		//_word &= 0x00FF;
		//byte_L = (BYTE)_word;

		//// // Convert to String Hex
		//// // Out in Edits
		////CString str_hex;
		//str_hex.Truncate(0);

		//str_hex.Format(_T("%02X"), byte_H);
		//this->m_Edit_Mod_H.SetWindowTextW(str_hex);

		//str_hex.Truncate(0);

		//str_hex.Format(_T("%02X"), byte_L);
		//this->m_Edit_Mod_L.SetWindowTextW(str_hex);

		// > Update interface for Write controls
		UpdateData(FALSE);

		// > Update Controls in real time
		OnBnClickedButtonBiasSet();
	}

	if (pScrollBar == pSliderMOD)			//  MOD Slider
	{
		CWnd *pStaticMOD = this->GetDlgItem(IDC_STATIC_MOD);

		// Update contents for Read controls
		UpdateData(TRUE);

		// Get current Slider position
		int iPos = this->m_Slider_MOD.GetPos();

		// convert to str
		CString str;
		str.Format(L"%d", iPos);

		// Out to Interface control 
		pStaticMOD->SetWindowTextW(str);

		// Out to Interface control Edits
		// // Get ByteH and ByteL from WORD
		unsigned int _word;
		unsigned char byte_H;
		unsigned char byte_L;

		// // // ByteH
		_word = iPos;
		_word &= 0xFF00;
		_word = _word >> 8;
		byte_H = (BYTE)_word;

		// // // ByteL
		_word = iPos;
		_word &= 0x00FF;
		byte_L = (BYTE)_word;

		// // Convert to String Hex
		// // Out in Edits
		CString str_hex;
		str_hex.Format(_T("%02X"), byte_H);
		this->m_Edit_Mod_H.SetWindowTextW(str_hex);

		str_hex.Truncate(0);

		str_hex.Format(_T("%02X"), byte_L);
		this->m_Edit_Mod_L.SetWindowTextW(str_hex);

		// Update interface for Write controls
		UpdateData(FALSE);

		// > Update Controls in real time
		OnBnClickedButtonModSet();

	}

	//if (pScrollBar == pSliderHOR)			//  HOR Slider
	//{
	//	CWnd *pStaticHOR = this->GetDlgItem(IDC_STATIC_HOR);

	//	// Update contents for Read controls
	//	UpdateData(TRUE);

	//	// Get current Slider position
	//	char iPos = this->m_Slider_HORISON.GetPos();

	//	// convert to str
	//	CString str;
	//	str.Format(L"%d", iPos);

	//	// Out to Interface control Label
	//	pStaticHOR->SetWindowTextW(str);

	//	// Out to Interface control Edits
	//	// Format Value
	//	// FORMAT:
	//	// 1Byte = bits:: data:7654321, direction:0

	//	// define direction
	//	// define Value
	//	char direction;
	//	unsigned char iPosFormat;

	//	if (iPos < 0)
	//	{
	//		direction = 1;
	//		iPosFormat = iPos * (-1);
	//	}
	//	else
	//	{
	//		direction = 0;
	//		iPosFormat = iPos;
	//	}

	//	iPosFormat = iPosFormat << 1;
	//	iPosFormat &= 0xFE;
	//	iPosFormat += direction;

	//	// Out to Interface control Edits
	//	// // Convert to String Hex
	//	// // Out in Edits
	//	CString str_hex;
	//	str_hex.Format(_T("%02X"), iPosFormat);
	//	this->m_Edit_Horison.SetWindowTextW(str_hex);

	//	// Update interface for Write controls
	//	UpdateData(FALSE);

	//}

	//if (pScrollBar == pSliderVERT)			//  VERT Slider
	//{
	//	CWnd *pStaticVERT = this->GetDlgItem(IDC_STATIC_VERT);

	//	// Update contents for Read controls
	//	UpdateData(TRUE);

	//	// Get current Slider position
	//	unsigned char iPos = this->m_Slider_VERTICAL.GetPos();

	//	// convert to str
	//	CString str;
	//	str.Format(L"%d", iPos);

	//	// Out to Interface control Label
	//	pStaticVERT->SetWindowTextW(str);

	//	// Out to Interface control Edits
	//	// Format Value
	//	// FORMAT:
	//	// 1Byte = bits:: data:7654321, active:0

	//	// define Status
	//	unsigned char iStatus;

	//	UpdateData(TRUE);
	//	if (m_bCheck_Vertical)
	//	{
	//		iStatus = 1;
	//	}
	//	else
	//	{
	//		iStatus = 0;
	//	}

	//	// define Value
	//	unsigned char iPosFormat;

	//	iPosFormat = iPos;
	//	iPosFormat = iPosFormat << 1;
	//	iPosFormat &= 0xFE;
	//	iPosFormat += iStatus;

	//	// Out to Interface control Edits
	//	// // Convert to String Hex
	//	// // Out in Edits
	//	CString str_hex;
	//	str_hex.Format(_T("%02X"), iPosFormat);
	//	this->m_Edit_Vertical.SetWindowTextW(str_hex);

	//	// Update interface for Write controls
	//	UpdateData(FALSE);

	//}


	if (nSBCode == SB_ENDSCROLL)
	{
		// TODO: Find source of 2nd msg.
		if (m_bOnHScrollDouble)
		{
			// [SECOND MSG]

			// reset Flag
			m_bOnHScrollDouble = 0;
		}
		else
		{
			// [FIRST MSG]

			// set Flag
			//m_bOnHScrollDouble = 1;

			// > Proc 
			// Read previous
			NoUpdateRead();

			// Update from Sliders
			OnBnClickedButtonBiasSet();
			OnBnClickedButtonModSet();
			//OnBnClickedButtonHorSet();
			//OnBnClickedButtonVerticalSet();

			// Write
			OnBnClickedButton5();

		}

	}

	if (nSBCode == TB_THUMBTRACK)
	{

	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDS4830A_SFP_LX_CONF_ENGI::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}
