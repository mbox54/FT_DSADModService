// ADUC7029_XFP_WDM_CONF_ENGI.cpp : implementation file
//

#include "stdafx.h"
#include "DSMCUService.h"
#include "ADUC7029_XFP_WDM_CONF_ENGI.h"
#include "afxdialogex.h"


// CADUC7029_XFP_WDM_CONF_ENGI dialog

IMPLEMENT_DYNAMIC(CADUC7029_XFP_WDM_CONF_ENGI, CDialog)

CADUC7029_XFP_WDM_CONF_ENGI::CADUC7029_XFP_WDM_CONF_ENGI(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PROPPAGE_ADUC7029_XFP_WDM_CONF_ENGI, pParent)
	, m_sEdit_PassValue(_T(""))
	, m_Edit_Filename(_T(""))
{

}

CADUC7029_XFP_WDM_CONF_ENGI::CADUC7029_XFP_WDM_CONF_ENGI(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd * pParent)
	: CDialog(IDD_PROPPAGE_ADUC7029_XFP_WDM_CONF_ENGI, pParent)
, m_pHidSmbus(pHidSmbus)
, p_EDIT_STATUS(p_EDIT_STATUS)
, p_cPB_OP(p_cPB_OP)
, p_service(p_service)
, m_Grid(m_pHidSmbus, p_cPB_OP, p_EDIT_STATUS, p_service)
{

}

CADUC7029_XFP_WDM_CONF_ENGI::~CADUC7029_XFP_WDM_CONF_ENGI()
{

}


void CADUC7029_XFP_WDM_CONF_ENGI::Trace(LPCTSTR szFmt, ...)
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



void CADUC7029_XFP_WDM_CONF_ENGI::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CWnd *pSliderBIAS = this->GetDlgItem(IDC_SLIDER_BIAS);
	CWnd *pSliderMOD = this->GetDlgItem(IDC_SLIDER_MOD);

	if (pScrollBar == pSliderBIAS)			//  BIAS Slider
	{
		// get static components
		CWnd *pStaticBIAS = this->GetDlgItem(IDC_STATIC_BIAS);

		// > Update contents for Read controls
		UpdateData(TRUE);

		// > Control MAIN slider
		// Get current Slider position
		WORD ucPos = this->m_Slider_BIAS.GetPos();

		// convert to str
		CString str1;
		str1.Format(L"%d", ucPos);

		// Out to Interface control Label
		pStaticBIAS->SetWindowTextW(str1);

		// Out to Interface control Edits
		unWORD_2BYTE buf_value;

		buf_value.usValue = ucPos;

		// Convert to String Hex
		// Out in Edits
		CString str_hex;
		str_hex.Format(_T("%02X"), buf_value.ucValue[1]);
		this->m_Edit_Bias_H.SetWindowTextW(str_hex);

		str_hex.Truncate(0);

		str_hex.Format(_T("%02X"), buf_value.ucValue[0]);
		this->m_Edit_Bias_L.SetWindowTextW(str_hex);

		// > Update interface for Write controls
		UpdateData(FALSE);
	}

	if (pScrollBar == pSliderMOD)			//  MOD Slider
	{
		// get static components
		CWnd *pStaticMOD = this->GetDlgItem(IDC_STATIC_MOD);

		// > Update contents for Read controls
		UpdateData(TRUE);

		// > Control MAIN slider
		// Get current Slider position
		WORD ucPos = this->m_Slider_MOD.GetPos();

		unWORD_2BYTE buf_value;

		buf_value.usValue = ucPos;

		// convert to str
		CString str1;
		str1.Format(L"%d", ucPos);

		// Out to Interface control Label
		pStaticMOD->SetWindowTextW(str1);

		// Out to Interface control Edits

		// Convert to String Hex
		// Out in Edits
		CString str_hex;
		str_hex.Format(_T("%02X"), buf_value.ucValue[1]);
		this->m_Edit_Mod_H.SetWindowTextW(str_hex);
		
		str_hex.Truncate(0);
		str_hex.Format(_T("%02X"), buf_value.ucValue[0]);
		this->m_Edit_Mod_L.SetWindowTextW(str_hex);

		// > Update interface for Write controls
		UpdateData(FALSE);
	}


	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CADUC7029_XFP_WDM_CONF_ENGI::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}



void CADUC7029_XFP_WDM_CONF_ENGI::SliderInit()
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
	this->m_Edit_Bias_L.LimitText(2);

	// fill with Value
	unWORD_2BYTE buf_value;

	buf_value.usValue = VALUE_BIAS_DEF;

	// place Values
	CString str_hex;
	str_hex.Format(_T("%02X"), buf_value.ucValue[1]);
	this->m_Edit_Bias_H.SetWindowTextW(str_hex);

	str_hex.Truncate(0);

	str_hex.Format(_T("%02X"), buf_value.ucValue[0]);
	this->m_Edit_Bias_L.SetWindowTextW(str_hex);


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
	buf_value.usValue = VALUE_MOD_DEF;

	str_hex.Format(_T("%02X"), buf_value.ucValue[1]);
	this->m_Edit_Mod_H.SetWindowTextW(str_hex);

	str_hex.Truncate(0);

	str_hex.Format(_T("%02X"), buf_value.ucValue[0]);
	this->m_Edit_Mod_L.SetWindowTextW(str_hex);
}


void CADUC7029_XFP_WDM_CONF_ENGI::EditHexControl(CEdit * pEdit)
{

}

int CADUC7029_XFP_WDM_CONF_ENGI::TableValues_LoadFromFile(char * v_filename, BYTE * v_ByteData)
{
	// CONST
	const unsigned char uLineCount = 16;
	const unsigned char uByteCount = 16;
	const unsigned char uLetterCount = 2;

	// err status
	unsigned char errLine;

	// position
	unsigned char l;				// active Line
	unsigned char k;				// active Byte
	unsigned char i;				// active Letter (Hex)

									// str Hex buffer
	unsigned char chex_buf[2];

	FILE *fp;


	// [#] FILE OP CHECK
	// > Define Filename

	// set Directory Filename
	char *dir_name = "conf\\";

	// form full filename
	char ch_fname[100];

	BYTE offset_k = 0;
	k = 0;

	BYTE act = 1;
	while (act != 0)
	{
		// [#] mode 1: set directory name
		if (act == 1)
		{
			// check end symbol
			if (dir_name[k] == '\0')
			{
				// [END]

				// switch mode
				act = 2;

				// set dir offset
				offset_k = k;

				// reset index
				k = 0;
			}
			else
			{
				// [NOT END]
				ch_fname[k] = dir_name[k];

				k++;
			}
		} //mode 1 proc
		else
		{
			// [#] mode 2: set file name
			if (act == 2)
			{
				if (v_filename[k] == '\0')
				{
					// [END]

					// set end of full filename string
					ch_fname[k + offset_k] = '\0';

					// switch mode
					act = 0;		// END OP
				}
				else
				{
					// [NOT END]
					ch_fname[k + offset_k] = v_filename[k];

					k++;
				}
			} //mode 2
		} //mode 1 else
	}//while (act != 0)

	 // > Open File try
	errno_t err;
	err = fopen_s(&fp, ch_fname, "r");

	if (err != 0)
	{	// "can't open file" 
		errLine = -1;
		return OP_FAILED;
	}

	// [STATUS OUTPUT]
	CString str_filename;

	k = 0;
	act = 1;
	while (act)
	{
		if (ch_fname[k] == '\0')
		{
			// [END]

			act = 0;
		}
		else
		{
			// [NOT END]

			str_filename.AppendChar(ch_fname[k]);

			k++;
		}
	}

	Trace(_T("File %s  \n"), str_filename);

	// > Define parameters for probress bar
	// count number of Intel Hex file lines
	unsigned int fileLineNum = 0;
	char *e_str;
	char IHline[256];
	while (TRUE)
	{
		e_str = fgets(IHline, sizeof(IHline), fp);
		fileLineNum++;

		// exit condition
		if (e_str == NULL)
		{
			// check line read condition
			if (feof(fp) != 0)
			{	// end of file

				break;
			}
			else
			{	// file corrupted
				errLine = -2;
				//		return MAXQBL_OP_FAILED;
				break;
			}
		}
	}

	// reopen FILE
	fclose(fp);

	err = fopen_s(&fp, ch_fname, "r");

	if (err != 0)
	{	// "can't open file" 
		errLine = -1;

		return OP_FAILED;
	}

	// [STATUS OUTPUT]
	Trace(_T("- lines: %03d \n"), fileLineNum);


	// > Read Data

	// init cycle Read var
	l = 0;
	k = 0;
	i = 0;
	chex_buf[0] = '0';
	chex_buf[1] = '0';


	// Read OP

	// NOTE:
	// FORMAT of file: 
	// H0 H1 H2 H3 H4 H5 H6 H7 H8 H9 HA HB HC HD HE HF // (Line 0x00)
	// ...
	// H0 H1 H2 H3 H4 H5 H6 H7 H8 H9 HA HB HC HD HE HF // (Line 0x0F)
	// where HN - Hex String symbols

	bool ACT = 1;						// reading file op is active
	while (ACT)
	{	// proceed symbol routine

		// > Get symbol
		int c = fgetc(fp);

		// > Select type
		switch (c)
		{
		case EOF:						// [#] cancel on End of File
			ACT = FALSE;
			break;

		case '\n':						// [#] prepare to New Line

			l++;						// line number

			if (l > uLineCount - 1)		// check End of Reading OP
			{
				ACT = FALSE;
			}
			else
			{
				// reset byte number in Line
				k = 0;
			}
			break;

		case ' ':						// [#] prepare to New Byte

			k++;						// Byte number

			if (k > uByteCount - 1)		// check End of Line
			{
				// NOP
				// !probably set error here
			}

			break;


		default:						// [#] all Common text letter symbols
										// [#] Parce Intel Hex line
										// read letter
			chex_buf[i] = c;

			if (i == 0)
			{
				// FIRST letter
				i++;
			}
			else
			{
				// SECOND letter

				// > check valid Symbols
				// make hex string
				CString fileText;
				fileText.AppendChar(chex_buf[0]);
				fileText.AppendChar(chex_buf[1]);

				// valid symbol checking
				CString hexTextValid = fileText.SpanIncluding(_T("0123456789abcdefABCDEF"));

				if (hexTextValid.Compare(fileText) == 0)
				{
					// convert to Byte
					// place to uValues[] Table
					v_ByteData[l * uByteCount + k] = (BYTE)_tcstoul(hexTextValid, NULL, 16);

					// reset letter number
					i = 0;
				}
				else
				{
					// set err number equal to Line number
					errLine = l;

					return OP_FAILED;
				} // if compare
			}

			break;

		} // switch c
	} // while ACT


	  // > Close Intel Hex File
	fclose(fp);


	return OP_SUCCESS;
}

int CADUC7029_XFP_WDM_CONF_ENGI::TableValues_SaveToFile(BYTE * v_ByteData)
{
	// CONST
	// # define COUNT of Byte in Line
	const unsigned char LINE_BYTE_COUNT = 16;

	const unsigned char uLineCount = 16;
	const unsigned char uByteCount = 16;
	const unsigned char uLetterCount = 2;

	// err status
	unsigned char errLine;

	// position
	unsigned char l;				// active Line
	unsigned char k;				// active Byte
	unsigned char i;				// active Letter (Hex)

									// str Hex buffer
	unsigned char chex_buf[2];

	FILE *fp;


	// [#] FILE OP CHECK
	// > Define Filename
	CString str_filename("test.conf");
	char *file_name = "test.conf";

	// > Open File try
	errno_t err;

	char ch_fname[200];
	for (int k = 0; k < str_filename.GetLength(); k++)
	{
		ch_fname[k] = str_filename[k];
	}

	ch_fname[str_filename.GetLength()] = '\0';

	err = fopen_s(&fp, ch_fname, "w");

	if (err != 0)
	{	// "can't open file" 
		errLine = -1;
		return OP_FAILED;
	}

	// [STATUS OUTPUT]
	Trace(_T("File %s  \n"), str_filename);


	// > Write Data

	// NOTE:
	// FORMAT of file: 
	// H0 H1 H2 H3 H4 H5 H6 H7 H8 H9 HA HB HC HD HE HF // (Line 0x00)
	// ...
	// H0 H1 H2 H3 H4 H5 H6 H7 H8 H9 HA HB HC HD HE HF // (Line 0x0F)
	// where HN - Hex String symbols

	// init cycle Write var
	l = 0;
	k = 0;

	//
	unsigned char cbyte_line[LINE_BYTE_COUNT];

	bool ACT = 1;							// writing file op is active
	while (ACT)
	{
		// fill the Line
		cbyte_line[k] = v_ByteData[l * uByteCount + k];

		if (k > uByteCount - 2)				// check End of Line
		{
			// > forming Line

			// make string of hex
			CString fileHexSymbol;
			CString fileHexLine;

			for (unsigned char ik = 0; ik < uByteCount; ik++)
			{
				fileHexSymbol.Format(_T("%02X"), cbyte_line[ik]);

				fileHexLine.Append(fileHexSymbol);
				fileHexLine.AppendChar(' ');
			}

			// make null_string Line_of_char to write in file
			// NOTE:
			// FORMAT: 'H' 'H' ' ', ... , 'H' 'H' \n \0
			char chex_line[uByteCount * 3 + 1];

			for (unsigned char ik = 0; ik < uByteCount * 3 - 1; ik++)
			{
				chex_line[ik] = fileHexLine[ik];
			}

			// add line ender
			chex_line[uByteCount * 3 - 1] = '\n';
			chex_line[uByteCount * 3] = '\0';

			// write string to file
			fputs(chex_line, fp);


			if (l > uLineCount - 2)			// check End of Reading OP
			{
				ACT = FALSE;
			}
			else
			{
				// prepare new Line
				l++;

				// reset Byte number in Line
				k = 0;
			}
		}
		else
		{
			k++;
		}
	} // 


	  // > Close Intel Hex File
	fclose(fp);


	return OP_SUCCESS;
}

void CADUC7029_XFP_WDM_CONF_ENGI::NoUpdateRead()
{
	Trace(_T("ÔÓÔ˚ÚÍ‡: ◊“≈Õ»≈\n"));
	Trace(_T("Ô‡‡ÏÂÚ˚: A2-T10-PASS, 256 ·‡ÈÚ\n"));


	// Valid ConfigTable Values
	unsigned char v_TablName[1] = { 0x10 };
	//	unsigned char v_TablPass[4] = { 'O', 'P', 'W', 'Y' };
	unsigned char v_TablPass[4]; // = { 0x00, 0x11, 0x22, 0x33 };


								 // get Password 4Bytes
	UpdateData(TRUE);

	CString strHex;

	// !debug _pass letters
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
	BYTE retVal = m_Grid.DeviceSlave_Write(v_TablName, SLAVEADDR_A0, 0x7F, 1);
	if (retVal != HID_SMBUS_SUCCESS)
	{
		Trace(_T("Œÿ»¡ ¿. [ÍÓ‰: %02d] \n"), retVal);
		Trace(_T("Õ¿ ›“¿œ≈: ¬€¡Œ– “¿¡À»÷€ \n"), retVal);
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
	//			Trace(_T("Œÿ»¡ ¿. [ÍÓ‰: %02d] \n"), retVal);
	//			Trace(_T("Õ¿ ›“¿œ≈: ¬¬Œƒ œ¿–ŒÀﬂ \n"), retVal);

	//			break;
	//		}

	//		Sleep(10);

	//		p_cPB_OP->SetPos(30 + 5 * k);
	//	}
	//}

	if (retVal == HID_SMBUS_SUCCESS)
	{
		// Read op
		Sleep(30);

		p_cPB_OP->SetPos(80);

		//m_Grid.DeviceSlave_ReadTimer(uValues2, 0, SLAVEADDR_A2, 0, 256, 0, 0);
		retVal = m_Grid.DeviceSlave_Read(uValues2, SLAVEADDR_A0, 0x80, 128);

		if (retVal != HID_SMBUS_SUCCESS)
		{
			// error: Device Read
			Trace(_T("Œÿ»¡ ¿. [ÍÓ‰: %02d] \n"), retVal);
			Trace(_T("Õ¿ ›“¿œ≈: «¿√–”« ¿ “¿¡À»÷€ \n"), retVal);
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

	Trace(_T("”—œ≈ÿÕŒ.\n"));
	Trace(_T("-----------------------\n"));

	m_Grid.GridSFF_Write(uValues2, 0x80, 128);

	p_cPB_OP->SetPos(100);

}


void CADUC7029_XFP_WDM_CONF_ENGI::UpdateRead()
{
	NoUpdateRead();

	// > Update Output Controls
	// > > Get Table Values
	BYTE uValues[256];
	m_Grid.GridSFF_Read(uValues, 0x80, 128);

	// > > Set Sliders Position
	// [SLIDER BIAS]
	// Get Value
	unWORD_2BYTE buf_value;

	buf_value.ucValue[1] = (uValues[ADDR_BIAS_HI] & 0x03);
	buf_value.ucValue[0] = uValues[ADDR_BIAS_LO];

	// Set Slider Control
	this->m_Slider_BIAS.SetPos(buf_value.usValue);

	// Set Static Control
	CString strValue;
	strValue.Format(L"%d", buf_value.usValue);

	CWnd *pStaticBIAS = this->GetDlgItem(IDC_STATIC_BIAS);
	pStaticBIAS->SetWindowTextW(strValue);

	// Set Edit Control
	strValue.Truncate(0);
	strValue.Format(_T("%02X"), buf_value.ucValue[1]);
	this->m_Edit_Bias_H.SetWindowTextW(strValue);

	strValue.Truncate(0);
	strValue.Format(_T("%02X"), buf_value.ucValue[0]);
	this->m_Edit_Bias_L.SetWindowTextW(strValue);


	//// [SLIDER MOD]
	//// Get Value
	//unComplexValue.strucValue.ucByte02 = uValues[0x80 + ADDR_MOD];
	//unComplexValue.strucValue.ucByte01 = uValues[0x80 + ADDR_MOD + 1];

	//// Set Slider Control
	//this->m_Slider_MOD.SetPos(unComplexValue.usWordValue);

	//// Set Static Control
	//strValue.Truncate(0);
	//strValue.Format(L"%d", unComplexValue.usWordValue);

	//CWnd *pStaticMOD = this->GetDlgItem(IDC_STATIC_MOD);
	//pStaticMOD->SetWindowTextW(strValue);

	//// Set Edit Control
	//strValue.Truncate(0);
	//strValue.Format(_T("%02X"), unComplexValue.strucValue.ucByte02);
	//this->m_Edit_Mod_H.SetWindowTextW(strValue);

	//strValue.Truncate(0);
	//strValue.Format(_T("%02X"), unComplexValue.strucValue.ucByte01);
	//this->m_Edit_Mod_L.SetWindowTextW(strValue);

	// > Update interface for Write controls
	UpdateData(FALSE);

}


void CADUC7029_XFP_WDM_CONF_ENGI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_Grid);
	DDX_Text(pDX, IDC_EDIT_PASSVALUE, m_sEdit_PassValue);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_Edit_Filename);
	DDX_Control(pDX, IDC_SLIDER_BIAS, m_Slider_BIAS);
	DDX_Control(pDX, IDC_SLIDER_MOD, m_Slider_MOD);
	DDX_Control(pDX, IDC_EDIT_BIAS_H, m_Edit_Bias_H);
	DDX_Control(pDX, IDC_EDIT_BIAS_L, m_Edit_Bias_L);
	DDX_Control(pDX, IDC_EDIT_MOD_H, m_Edit_Mod_H);
	DDX_Control(pDX, IDC_EDIT_MOD_L, m_Edit_Mod_L);
}


BEGIN_MESSAGE_MAP(CADUC7029_XFP_WDM_CONF_ENGI, CDialog)

	// Grid event functions
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)


	ON_BN_CLICKED(IDC_BUTTON4, &CADUC7029_XFP_WDM_CONF_ENGI::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CADUC7029_XFP_WDM_CONF_ENGI::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_BIAS_SET, &CADUC7029_XFP_WDM_CONF_ENGI::OnBnClickedButtonBiasSet)
	ON_BN_CLICKED(IDC_BUTTON_MOD_SET, &CADUC7029_XFP_WDM_CONF_ENGI::OnBnClickedButtonModSet)
	ON_BN_CLICKED(IDC_BUTTON_HOR_SET, &CADUC7029_XFP_WDM_CONF_ENGI::OnBnClickedButtonHorSet)
	ON_BN_CLICKED(IDC_BUTTON_VERTICAL_SET, &CADUC7029_XFP_WDM_CONF_ENGI::OnBnClickedButtonVerticalSet)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CADUC7029_XFP_WDM_CONF_ENGI message handlers


BOOL CADUC7029_XFP_WDM_CONF_ENGI::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_Grid.Init();

	// > Sliders
	SliderInit();

	// > Password
	m_sEdit_PassValue = (CString)"00112233";

	// > Confic File Default
	m_Edit_Filename = (CString)"test.conf";

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CADUC7029_XFP_WDM_CONF_ENGI::OnGridClick(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	Trace(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
	Trace(_T("Text: %s\n"), m_Grid.GetItemText(pItem->iRow, pItem->iColumn));
	//Item.strText
}

void CADUC7029_XFP_WDM_CONF_ENGI::OnGridEndEdit(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	Trace(_T("End Edit on row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	// > check Valid Input
	bool bInvalid = m_Grid.CheckValidHex(pItem->iRow, pItem->iColumn);

	*pResult = (bInvalid || m_Grid.m_bRejectEditChanges) ? -1 : 0;

	Trace(_T("Res: %d\n"), bInvalid && m_Grid.m_bRejectEditChanges);
}


// Read OP
void CADUC7029_XFP_WDM_CONF_ENGI::OnBnClickedButton4()
{
	// Do Read Table PROC
	// with update controls
	UpdateRead();

}

// Write OP
void CADUC7029_XFP_WDM_CONF_ENGI::OnBnClickedButton5()
{
	Trace(_T("ÔÓÔ˚ÚÍ‡: Œ¡ÕŒ¬À≈Õ»≈ \n"));
	Trace(_T("Ô‡‡ÏÂÚ˚: A0-T10-PASS, 128 ·‡ÈÚ\n"));

	// Valid ConfigTable Values
	unsigned char v_TablName[1] = { 0x10 };
	//	unsigned char v_TablPass[4] = { 'O', 'P', 'W', 'Y' };
	unsigned char v_TablPass[4]; // = { 0x00, 0x11, 0x22, 0x33 };


	// get Password 4Bytes
	UpdateData(TRUE);

	CString strHex;

	// !debug _convert pass symbols
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
	BYTE retVal = m_Grid.DeviceSlave_Write(v_TablName, SLAVEADDR_A0, 0x7F, 1);
	if (retVal != HID_SMBUS_SUCCESS)
	{
		Trace(_T("Œÿ»¡ ¿. [ÍÓ‰: %02d] \n"), retVal);
		Trace(_T("Õ¿ ›“¿œ≈: ¬€¡Œ– “¿¡À»÷€ \n"), retVal);
	}

	// !debug pass set
	//if (retVal == HID_SMBUS_SUCCESS)
	//{
	//	Sleep(30);

	//	p_cPB_OP->SetPos(20);

	//	// send password
	//	// write pass in Device
	//	retVal = m_Grid.DeviceSlave_Write(v_TablPass, SLAVEADDR_A2, 0x7B, 4);

	//	if (retVal != HID_SMBUS_SUCCESS)
	//	{
	//		Trace(_T("Œÿ»¡ ¿. [ÍÓ‰: %02d] \n"), retVal);
	//		Trace(_T("Õ¿ ›“¿œ≈: ¬¬Œƒ œ¿–ŒÀﬂ \n"), retVal);
	//	}

	//	Sleep(30);

	//	p_cPB_OP->SetPos(40);
	//}

	if (retVal == HID_SMBUS_SUCCESS)
	{
		// NOTE:
		// it is must to:
		// write ALL 128 bytes of TABLE
		unsigned char v_GridVal_T10[128];
		unsigned char v_WrByte32[32];

		// get Values
		m_Grid.GridSFF_Read(v_GridVal_T10, 0x80, 128);

		// Write to Device
		BYTE retVal = m_Grid.DeviceSlave_Write(v_GridVal_T10, SLAVEADDR_A0, 0x80, 128);

		// !debug 8BYTE regime
		for (int k = 0; k < 4; k++)
		{
			// prepare write buffer
			for (int k2 = 0; k2 < 32; k2++)
			{
				v_WrByte32[k2] = v_GridVal_T10[k * 32 + k2];
			}

			// i2c write
			retVal = m_Grid.DeviceSlave_Write(v_WrByte32, SLAVEADDR_A2, 0x80 + k * 32, 32);

			if (retVal != HID_SMBUS_SUCCESS)
			{
				// error: Device Read
				Trace(_T("Œÿ»¡ ¿. [ÍÓ‰: %02d] \n"), retVal);
				Trace(_T("Õ¿ ›“¿œ≈: Œ¡ÕŒ¬À≈Õ»≈ “¿¡À»÷€ \n"), retVal);
			}

			p_cPB_OP->SetPos(40 + k * 15);

			Sleep(300);
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

	Trace(_T("”—œ≈ÿÕŒ.\n"));
	Trace(_T("-----------------------\n"));

	p_cPB_OP->SetPos(100);
	//	m_Grid.DeviceSlave_WriteTimer(uValues2, 0, SLAVEADDR_A2, 128 + 8, 8, 0, 0);
}




void CADUC7029_XFP_WDM_CONF_ENGI::OnBnClickedButtonBiasSet()
{
	// TODO: Add your control notification handler code here
}


void CADUC7029_XFP_WDM_CONF_ENGI::OnBnClickedButtonModSet()
{
	// TODO: Add your control notification handler code here
}


void CADUC7029_XFP_WDM_CONF_ENGI::OnBnClickedButtonHorSet()
{
	// TODO: Add your control notification handler code here
}


void CADUC7029_XFP_WDM_CONF_ENGI::OnBnClickedButtonVerticalSet()
{
	// TODO: Add your control notification handler code here
}

