// DS4830A_SFPP_LR_CONF_ENGI.cpp : implementation file
//

#include "stdafx.h"
#include "DSMCUService.h"
#include "DS4830A_SFPP_LR_CONF_ENGI.h"
#include "afxdialogex.h"


// DS4830A_SFPP_LR_CONF_ENGI dialog

IMPLEMENT_DYNAMIC(CDS4830A_SFPP_LR_CONF_ENGI, CDialogEx)

CDS4830A_SFPP_LR_CONF_ENGI::CDS4830A_SFPP_LR_CONF_ENGI(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROPPAGE_DS4830A_SFPP_LR_CONF_ENGI, pParent)
	, m_sEdit_PassValue(_T(""))
{

}

CDS4830A_SFPP_LR_CONF_ENGI::CDS4830A_SFPP_LR_CONF_ENGI(HID_SMBUS_DEVICE * pHidSmbus, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd * pParent)
	: CDialogEx(IDD_PROPPAGE_DS4830A_SFPP_LR_CONF_ENGI, pParent)
, m_pHidSmbus(pHidSmbus)
, p_EDIT_STATUS(p_EDIT_STATUS)
, p_cPB_OP(p_cPB_OP)
, p_service(p_service)
, m_Grid(m_pHidSmbus, p_cPB_OP, p_EDIT_STATUS, p_service)
{

}

CDS4830A_SFPP_LR_CONF_ENGI::~CDS4830A_SFPP_LR_CONF_ENGI()
{
}

BEGIN_MESSAGE_MAP(CDS4830A_SFPP_LR_CONF_ENGI, CDialogEx)

	// Grid event functions
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)

	ON_WM_HSCROLL()
	ON_WM_VSCROLL()

	ON_BN_CLICKED(IDC_BUTTON1, &CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_BIAS_SET, &CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedButtonBiasSet)
	ON_BN_CLICKED(IDC_BUTTON_MOD_SET, &CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedButtonModSet)
	ON_BN_CLICKED(IDC_BUTTON_HOR_SET, &CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedButtonHorSet)
	ON_BN_CLICKED(IDC_BUTTON_VERTICAL_SET, &CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedButtonVerticalSet)
	ON_BN_CLICKED(IDC_BUTTON4, &CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_CONF_READ, &CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedButtonConfRead)
	ON_BN_CLICKED(IDC_BUTTON_CONF_WRITE, &CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedButtonConfWrite)


	ON_BN_CLICKED(IDC_CHECK_VERTICAL, &CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedCheckVertical)
END_MESSAGE_MAP()


void CDS4830A_SFPP_LR_CONF_ENGI::Trace(LPCTSTR szFmt, ...)
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

void CDS4830A_SFPP_LR_CONF_ENGI::SliderInit()
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
	// // Get ByteH and ByteL from WORD
	unsigned short _word;
	unsigned char byte_H;
	unsigned char byte_L;

	// // // ByteH
	_word = VALUE_BIAS_DEF;
	_word &= 0xFF00;
	_word = _word >> 8;
	byte_H = (BYTE)_word;

	// // // ByteL
	_word = VALUE_BIAS_DEF;
	_word &= 0x00FF;
	byte_L = (BYTE)_word;

	// // place Values
	CString str_hex;
	str_hex.Format(_T("%02X"), byte_H);
	this->m_Edit_Bias_H.SetWindowTextW(str_hex);

	str_hex.Truncate(0);

	str_hex.Format(_T("%02X"), byte_L);
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
	// // Get ByteH and ByteL from WORD
	//unsigned int _word;
	//unsigned char byte_H;
	//unsigned char byte_L;

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


	// HORISONTAL
	// slider
	m_Slider_HORISON.SetRange(VALUE_HORISON_MIN, VALUE_HORISON_MAX);
	m_Slider_HORISON.SetPos(VALUE_HORISON_DEF);

	// get static components
	CWnd *pStaticHOR = this->GetDlgItem(IDC_STATIC_HOR);
	CWnd *pStaticHOR_MIN = this->GetDlgItem(IDC_STATIC_HOR_MIN2);
	CWnd *pStaticHOR_MAX = this->GetDlgItem(IDC_STATIC_HOR_MAX);

	// set Values
	str.Format(L"%d", VALUE_HORISON_MIN);		// Set & Convert
	pStaticHOR_MIN->SetWindowTextW(str);		// Out to Interface control 

	str.Format(L"%d", VALUE_HORISON_MAX);		// Set & Convert
	pStaticHOR_MAX->SetWindowTextW(str);		// Out to Interface control 

	str.Format(L"%d", VALUE_HORISON_DEF);		// Set & Convert
	pStaticHOR->SetWindowTextW(str);			// Out to Interface control 

												// edit 
												// config
	this->m_Edit_Horison.LimitText(2);

	// Out to Interface control Edits
	// Format Value
	// FORMAT:
	// 1Byte = bits:: data:7654321, direction:0

	// define direction
	// define Value
	char direction;
	unsigned char iPosFormat;

	char iPos = VALUE_HORISON_DEF;

	if (iPos < 0)
	{
		direction = 1;
		iPosFormat = iPos * (-1);
	}
	else
	{
		direction = 0;
		iPosFormat = iPos;
	}

	iPosFormat = iPosFormat << 1;
	iPosFormat &= 0xFE;
	iPosFormat += direction;

	// Out to Interface control Edits
	// // Convert to String Hex
	// // Out in Edits
	//CString str_hex;
	str_hex.Format(_T("%02X"), iPosFormat);
	this->m_Edit_Horison.SetWindowTextW(str_hex);

	// ! bug correct: init with bad position
	m_Slider_HORISON.SetPos(VALUE_HORISON_DEF - 20);

	// VERTICAL
	// slider
	m_Slider_VERTICAL.SetRange(VALUE_VERTICAL_MIN, VALUE_VERTICAL_MAX);
	m_Slider_VERTICAL.SetPos(VALUE_VERTICAL_DEF);

	// get static components
	CWnd *pStaticVERT = this->GetDlgItem(IDC_STATIC_VERT);
	CWnd *pStaticVERT_MIN = this->GetDlgItem(IDC_STATIC_VERT_MIN);
	CWnd *pStaticVERT_MAX = this->GetDlgItem(IDC_STATIC_VERT_MAX);

	// set Values
	str.Format(L"%d", VALUE_VERTICAL_MIN);		// Set & Convert
	pStaticVERT_MIN->SetWindowTextW(str);		// Out to Interface control 

	str.Format(L"%d", VALUE_VERTICAL_MAX);		// Set & Convert
	pStaticVERT_MAX->SetWindowTextW(str);		// Out to Interface control 

	str.Format(L"%d", VALUE_VERTICAL_DEF);		// Set & Convert
	pStaticVERT->SetWindowTextW(str);			// Out to Interface control 


												// checkbox
	this->m_bCheck_Vertical = TRUE;
	UpdateData(FALSE);

	// edit 
	// config
	this->m_Edit_Vertical.LimitText(2);

	// Out to Interface control Edits
	// Format Value
	// FORMAT:
	// 1Byte = bits:: data:7654321, active:0

	// define Status
	unsigned char iStatus;

	UpdateData(TRUE);
	if (m_bCheck_Vertical)
	{
		iStatus = 1;
	}
	else
	{
		iStatus = 0;
	}

	// define Value
	//unsigned char iPosFormat;

	iPosFormat = iPos;
	iPosFormat = iPosFormat << 1;
	iPosFormat &= 0xFE;
	iPosFormat += iStatus;

	// Out to Interface control Edits
	// // Convert to String Hex
	// // Out in Edits
	//CString str_hex;
	str_hex.Format(_T("%02X"), iPosFormat);
	this->m_Edit_Vertical.SetWindowTextW(str_hex);

	// ! bug correct: init with bad position
	m_Slider_HORISON.SetPos(VALUE_VERTICAL_DEF + 1);
	m_Slider_HORISON.SetPos(VALUE_HORISON_DEF + 1);

	// > Update interface for Write controls
	UpdateData(FALSE);
}

void CDS4830A_SFPP_LR_CONF_ENGI::EditHexControl(CEdit * pEdit)
{
	CString str;

	pEdit->GetWindowTextW(str);

	// valid symbol checking
	CString hexTextValid = str.SpanIncluding(_T("0123456789abcdefABCDEF"));

	// check for invalid hex symbol
	if (hexTextValid.Compare(str) != 0)
	{
		// clear control Text
		pEdit->SetWindowTextW(_T(""));
	}

}

int CDS4830A_SFPP_LR_CONF_ENGI::TableValues_LoadFromFile(char * v_filename, BYTE * v_ByteData)
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

int CDS4830A_SFPP_LR_CONF_ENGI::TableValues_SaveToFile(BYTE * v_ByteData)
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

void CDS4830A_SFPP_LR_CONF_ENGI::UpdateRead()
{
	// > Read Table
	NoUpdateRead();

	// > Update Output Controls
	// > > Get Table Values
	BYTE uValues[256];
	m_Grid.GridSFF_Read(uValues, 0, 256);	
	
	// > > Set Sliders Position
	// [SLIDER BIAS]
	// Get Value
	un_WordAndBytes unComplexValue;

	unComplexValue.strucValue.ucByte02 = uValues[0x80 + ADDR_BIAS];
	unComplexValue.strucValue.ucByte01 = uValues[0x80 + ADDR_BIAS + 1];

	// Set Slider Control
	this->m_Slider_BIAS.SetPos(unComplexValue.usWordValue);

	// Set Static Control
	CString strValue;
	strValue.Format(L"%d", unComplexValue.usWordValue);

	CWnd *pStaticBIAS = this->GetDlgItem(IDC_STATIC_BIAS);
	pStaticBIAS->SetWindowTextW(strValue);

	// Set Edit Control
	strValue.Truncate(0);
	strValue.Format(_T("%02X"), unComplexValue.strucValue.ucByte02);
	this->m_Edit_Bias_H.SetWindowTextW(strValue);

	strValue.Truncate(0);
	strValue.Format(_T("%02X"), unComplexValue.strucValue.ucByte01);
	this->m_Edit_Bias_L.SetWindowTextW(strValue);


	// [SLIDER MOD]
	// Get Value
	unComplexValue.strucValue.ucByte02 = uValues[0x80 + ADDR_MOD];
	unComplexValue.strucValue.ucByte01 = uValues[0x80 + ADDR_MOD + 1];

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


	// [SLIDER HORISON]
	// Get Value
	BYTE ucByte = uValues[0x80 + ADDR_HORISON];

	// define direction
	// define Value
	BYTE bPosDirection;
	char cPosValue;

	bPosDirection = ucByte >> 1;
	cPosValue = bPosDirection;

	bPosDirection = ucByte;
	bPosDirection &= 0x01;
	if (bPosDirection == 0)
	{
		// [POS]
		//		
	}
	else
	{
		// [NEG]

		cPosValue *= (-1);
	}

	// Set Slider Control
	this->m_Slider_HORISON.SetPos(cPosValue);

	// Set Static Control
	strValue.Truncate(0);
	strValue.Format(L"%d", cPosValue);

	CWnd *pStaticHOR = this->GetDlgItem(IDC_STATIC_HOR);
	pStaticHOR->SetWindowTextW(strValue);

	// Set Edit Control
	strValue.Truncate(0);
	strValue.Format(_T("%02X"), ucByte);
	this->m_Edit_Horison.SetWindowTextW(strValue);

	// [SLIDER VERTICAL]
	// Get Value
	ucByte = uValues[0x80 + ADDR_VERTICAL];

	// define state: active/deactive
	// define Value
	BYTE bPosState = ucByte >> 1;
	cPosValue = bPosState;

	bPosState = ucByte;
	bPosState &= 0x01;

	CWnd *wndSlide = this->GetDlgItem(IDC_SLIDER_VERTICAL);
	if (bPosState == 0)
	{
		// [DEACTICE]

		// set CheckBox Control
		m_bCheck_Vertical = 0;

		// disable Control
		wndSlide->EnableWindow(FALSE);
	}
	else
	{
		// [ACTIVE]

		// set CheckBox Control
		m_bCheck_Vertical = 1;

		// enadle Control
		wndSlide->EnableWindow(TRUE);
	}

	// Set Slider Control
	this->m_Slider_VERTICAL.SetPos(cPosValue);

	// Set Static Control
	strValue.Truncate(0);
	strValue.Format(L"%d", cPosValue);

	CWnd *pStaticVERT = this->GetDlgItem(IDC_STATIC_VERT);
	pStaticVERT->SetWindowTextW(strValue);

	// Set Edit Control
	strValue.Truncate(0);
	strValue.Format(_T("%02X"), ucByte);
	this->m_Edit_Vertical.SetWindowTextW(strValue);

	// > Update interface for Write controls
	UpdateData(FALSE);	

}


void CDS4830A_SFPP_LR_CONF_ENGI::NoUpdateRead()
{
	Trace(_T("�������: ������\n"));
	Trace(_T("���������: A2-T10-PASS, 256 ����\n"));


	// Valid ConfigTable Values
	unsigned char v_TablName[1] = { 0x10 };
	//	unsigned char v_TablPass[4] = { 'O', 'P', 'W', 'Y' };
	unsigned char v_TablPass[4]; // = { 0x00, 0x11, 0x22, 0x33 };


								 // get Password 4Bytes
	UpdateData(TRUE);

	CString strHex;

	for (unsigned char k = 0; k < 4; k++)
	{
		char cPassLetter[2];
		cPassLetter[0] = m_sEdit_PassValue[k * 2];
		cPassLetter[1] = m_sEdit_PassValue[k * 2 + 1];

		strHex.AppendChar(cPassLetter[0]);
		strHex.AppendChar(cPassLetter[1]);

		// convert to Byte
		unsigned char byte_passLetter;
		byte_passLetter = (BYTE)_tcstoul(strHex, NULL, 16);

		v_TablPass[k] = byte_passLetter;

		strHex.Truncate(0);
	}

	unsigned char v_WrByte[1];

	// progress component
	p_cPB_OP->SetPos(0);

	// send Table
	// write tabl in Device
	BYTE retVal = m_Grid.DeviceSlave_Write(v_TablName, SLAVEADDR_A2, 0x7F, 1);
	if (retVal != HID_SMBUS_SUCCESS)
	{
		Trace(_T("������. [���: %02d] \n"), retVal);
		Trace(_T("�� �����: ����� ������� \n"), retVal);
	}

	if (retVal == HID_SMBUS_SUCCESS)
	{
		Sleep(30);

		p_cPB_OP->SetPos(30);

		// send password
		// write pass in Device
		for (unsigned char k = 0; k < 4; k++)
		{
			v_WrByte[0] = v_TablPass[k];
			retVal = m_Grid.DeviceSlave_Write(v_WrByte, SLAVEADDR_A2, 0x7B + k, 1);

			if (retVal != HID_SMBUS_SUCCESS)
			{
				Trace(_T("������. [���: %02d] \n"), retVal);
				Trace(_T("�� �����: ���� ������ \n"), retVal);

				break;
			}

			Sleep(10);

			p_cPB_OP->SetPos(30 + 5 * k);
		}
	}

	if (retVal == HID_SMBUS_SUCCESS)
	{
		// Read op
		Sleep(30);

		p_cPB_OP->SetPos(80);

		//m_Grid.DeviceSlave_ReadTimer(uValues2, 0, SLAVEADDR_A2, 0, 256, 0, 0);
		retVal = m_Grid.DeviceSlave_Read(uValues2, SLAVEADDR_A2, 0, 256);

		if (retVal != HID_SMBUS_SUCCESS)
		{
			// error: Device Read
			Trace(_T("������. [���: %02d] \n"), retVal);
			Trace(_T("�� �����: �������� ������� \n"), retVal);
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

	Trace(_T("�������.\n"));
	Trace(_T("-----------------------\n"));

	m_Grid.GridSFF_Write(uValues2, 0, 256);

	p_cPB_OP->SetPos(100);

}

void CDS4830A_SFPP_LR_CONF_ENGI::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_Grid);

	DDX_Text(pDX, IDC_EDIT_FILENAME, m_Edit_Filename);

	DDX_Check(pDX, IDC_CHECK_VERTICAL, m_bCheck_Vertical);

	DDX_Control(pDX, IDC_EDIT_BIAS_H, m_Edit_Bias_H);
	DDX_Control(pDX, IDC_EDIT_BIAS_L, m_Edit_Bias_L);
	DDX_Control(pDX, IDC_EDIT_MOD_H, m_Edit_Mod_H);
	DDX_Control(pDX, IDC_EDIT_MOD_L, m_Edit_Mod_L);
	DDX_Control(pDX, IDC_EDIT_HOR, m_Edit_Horison);
	DDX_Control(pDX, IDC_EDIT_VERTICAL, m_Edit_Vertical);

	DDX_Control(pDX, IDC_SLIDER_BIAS, m_Slider_BIAS);
	DDX_Control(pDX, IDC_SLIDER_MOD, m_Slider_MOD);
	DDX_Control(pDX, IDC_SLIDER_HORISON, m_Slider_HORISON);
	DDX_Control(pDX, IDC_SLIDER_VERTICAL, m_Slider_VERTICAL);
	DDX_Text(pDX, IDC_EDIT_PASSVALUE, m_sEdit_PassValue);
}

void CDS4830A_SFPP_LR_CONF_ENGI::OnGridClick(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	Trace(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
	Trace(_T("Text: %s\n"), m_Grid.GetItemText(pItem->iRow, pItem->iColumn));
	//Item.strText
}

void CDS4830A_SFPP_LR_CONF_ENGI::OnGridEndEdit(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	Trace(_T("End Edit on row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	// > check Valid Input
	bool bInvalid = m_Grid.CheckValidHex(pItem->iRow, pItem->iColumn);

	*pResult = (bInvalid || m_Grid.m_bRejectEditChanges) ? -1 : 0;

	Trace(_T("Res: %d\n"), bInvalid && m_Grid.m_bRejectEditChanges);
}


// DS4830A_SFPP_LR_CONF_ENGI message handlers


void CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}


void CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedButtonBiasSet()
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
			(LPCWSTR)L"�������� �� ���������!\n���� �� ����� ����� �����.",
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
	this->m_Edit_Bias_L.GetWindowTextW(str_edit);

	// > > Check null Value
	if (str_edit.GetLength() == 0)
	{
		// raise warning
		int msgboxID = MessageBox(
			(LPCWSTR)L"�������� �� ���������!\n���� �� ����� ����� �����.",
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
	if (_word < VALUE_BIAS_MIN)
	{
		// raise warning
		int msgboxID = MessageBox(
			(LPCWSTR)L"�������� �� ���������!\n������������� �������� ������� �� ����� ����������� ���������.",
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
			(LPCWSTR)L"�������� �� ���������!\n������������� �������� ������� �� ����� ����������� ���������.",
			(LPCWSTR)L"Set Value",
			MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
		);

		// abort
		return;
	}
*/

	// > Write Value in Grid
	// get Grid Values
	unsigned char uValue[256];

	this->m_Grid.GridSFF_Read(uValue, 0, 256);

	// put Value
	uValue[0x80 + ADDR_BIAS] = byte_H;
	uValue[0x80 + ADDR_BIAS + 1] = byte_L;

	// set Grid Values
	this->m_Grid.GridSFF_Write(uValue, 0, 256);

}


void CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedButtonModSet()
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
			(LPCWSTR)L"�������� �� ���������!\n���� �� ����� ����� �����.",
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
			(LPCWSTR)L"�������� �� ���������!\n���� �� ����� ����� �����.",
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
			(LPCWSTR)L"�������� �� ���������!\n������������� �������� ������� �� ����� ����������� ���������.",
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
			(LPCWSTR)L"�������� �� ���������!\n������������� �������� ������� �� ����� ����������� ���������.",
			(LPCWSTR)L"Set Value",
			MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
		);

		// abort
		return;
	}
*/


	// > Write Value in Grid
	// get Grid Values
	unsigned char uValue[256];

	this->m_Grid.GridSFF_Read(uValue, 0, 256);

	// put Value
	uValue[0x80 + ADDR_MOD] = byte_H;
	uValue[0x80 + ADDR_MOD + 1] = byte_L;

	// set Grid Values
	this->m_Grid.GridSFF_Write(uValue, 0, 256);

}


void CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedButtonHorSet()
{
	// > Get Value
	CString str;

	// > proceed FIRST byte
	this->m_Edit_Horison.GetWindowTextW(str);

	// > > Check null Value
	// no check: edit disabled

	// convert to Byte
	// no convert: The Value has been already converted

	// > > align Hex String to Valid format
	// no align: The Value has been already aligned

	// > Make INT from String HEX
	unsigned char _byte;

	_byte = (BYTE)_tcstoul(str, NULL, 16);

	// TODO: simplify. No need to read/write all 256 Values
	// > Write Value in Grid
	// get Grid Values
	unsigned char uValue[256];

	this->m_Grid.GridSFF_Read(uValue, 0, 256);

	// put Value
	uValue[0x80 + ADDR_HORISON] = _byte;

	// set Grid Values
	this->m_Grid.GridSFF_Write(uValue, 0, 256);
}


void CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedButtonVerticalSet()
{
	// > Get Value
	CString str;

	// > proceed FIRST byte
	this->m_Edit_Vertical.GetWindowTextW(str);

	// > > Check null Value
	// no check: edit disabled

	// convert to Byte
	// no convert: The Value has been already converted

	// > > align Hex String to Valid format
	// no align: The Value has been already aligned

	// > Make INT from String HEX
	unsigned char _byte;

	_byte = (BYTE)_tcstoul(str, NULL, 16);

	// TODO: simplify. No need to read/write all 256 Values
	// > Write Value in Grid
	// get Grid Values
	unsigned char uValue[256];

	this->m_Grid.GridSFF_Read(uValue, 0, 256);

	// put Value
	uValue[0x80 + ADDR_VERTICAL] = _byte;

	// set Grid Values
	this->m_Grid.GridSFF_Write(uValue, 0, 256);
}


void CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedButton4()
{
	UpdateRead();

}


void CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedButton5()
{
	Trace(_T("�������: ���������� \n"));
	Trace(_T("���������: A2-T10-PASS, 256 ����\n"));

	// Valid ConfigTable Values
	unsigned char v_TablName[1] = { 0x10 };
	//	unsigned char v_TablPass[4] = { 'O', 'P', 'W', 'Y' };
	unsigned char v_TablPass[4]; // = { 0x00, 0x11, 0x22, 0x33 };


								 // get Password 4Bytes
	UpdateData(TRUE);

	CString strHex;

	for (unsigned char k = 0; k < 4; k++)
	{
		char cPassLetter[2];
		cPassLetter[0] = m_sEdit_PassValue[k * 2];
		cPassLetter[1] = m_sEdit_PassValue[k * 2 + 1];

		strHex.AppendChar(cPassLetter[0]);
		strHex.AppendChar(cPassLetter[1]);

		// convert to Byte
		unsigned char byte_passLetter;
		byte_passLetter = (BYTE)_tcstoul(strHex, NULL, 16);

		v_TablPass[k] = byte_passLetter;

		strHex.Truncate(0);
	}

	// progress component
	p_cPB_OP->SetPos(0);

	// send Table
	// write tabl in Device
	BYTE retVal = m_Grid.DeviceSlave_Write(v_TablName, SLAVEADDR_A2, 0x7F, 1);
	if (retVal != HID_SMBUS_SUCCESS)
	{
		Trace(_T("������. [���: %02d] \n"), retVal);
		Trace(_T("�� �����: ����� ������� \n"), retVal);
	}

	if (retVal == HID_SMBUS_SUCCESS)
	{
		Sleep(30);

		p_cPB_OP->SetPos(20);

		// send password
		// write pass in Device
		retVal = m_Grid.DeviceSlave_Write(v_TablPass, SLAVEADDR_A2, 0x7B, 4);

		if (retVal != HID_SMBUS_SUCCESS)
		{
			Trace(_T("������. [���: %02d] \n"), retVal);
			Trace(_T("�� �����: ���� ������ \n"), retVal);
		}

		Sleep(30);

		p_cPB_OP->SetPos(40);
	}

	if (retVal == HID_SMBUS_SUCCESS)
	{
		// NOTE:
		// it is must to:
		// write ALL 128 bytes of TABLE
		unsigned char v_GridVal_T10[128];
		unsigned char v_WrByte8[8];

		// get Values
		m_Grid.GridSFF_Read(v_GridVal_T10, 0x80, 128);

		//// Write to Device
		//for (int k = 0; k < 16; k++)
		//{
		//	// prepare write buffer
		//	for (int k2 = 0; k2 < 8; k2++)
		//	{
		//		v_WrByte8[k2] = v_GridVal_T10[k * 8 + k2];
		//	}

		//	// i2c write
		//	retVal = m_Grid.DeviceSlave_Write(v_WrByte8, SLAVEADDR_A2, 0x80 + k * 8, 8);

		//	if (retVal != HID_SMBUS_SUCCESS)
		//	{
		//		// error: Device Read
		//		Trace(_T("������. [���: %02d] \n"), retVal);
		//		Trace(_T("�� �����: ���������� ������� \n"), retVal);
		//	}

		//	p_cPB_OP->SetPos(40 + k * 7);

		//	Sleep(10);
		//}

		// set Data to Device
		BYTE retVal = m_Grid.DeviceSlave_Write(v_GridVal_T10, SLAVEADDR_A2, 0x80, 128);

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

	Trace(_T("�������.\n"));
	Trace(_T("-----------------------\n"));

	p_cPB_OP->SetPos(100);
	//	m_Grid.DeviceSlave_WriteTimer(uValues2, 0, SLAVEADDR_A2, 128 + 8, 8, 0, 0);
}


void CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedButtonConfRead()
{
	// loading Values
	char ch_fName[50];
	CString str_fName;

	UpdateData(TRUE);
	str_fName = m_Edit_Filename;

	for (BYTE k = 0; k < str_fName.GetLength(); k++)
	{
		ch_fName[k] = str_fName[k];
	}

	ch_fName[str_fName.GetLength()] = '\0';

	this->TableValues_LoadFromFile(ch_fName, this->uValues2);

	m_Grid.GridSFF_Write(uValues2, 0, 256);
}


void CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedButtonConfWrite()
{
	// TODO: Add your control notification handler code here
}


void CDS4830A_SFPP_LR_CONF_ENGI::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// get Slider components
	CWnd *pSliderBIAS = this->GetDlgItem(IDC_SLIDER_BIAS);
	CWnd *pSliderMOD = this->GetDlgItem(IDC_SLIDER_MOD);
	CWnd *pSliderHOR = this->GetDlgItem(IDC_SLIDER_HORISON);
	CWnd *pSliderVERT = this->GetDlgItem(IDC_SLIDER_VERTICAL);



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

		// // // ByteH
		_word = iPos1;
		_word &= 0xFF00;
		_word = _word >> 8;
		byte_H = (BYTE)_word;

		// // // ByteL
		_word = iPos1;
		_word &= 0x00FF;
		byte_L = (BYTE)_word;

		// // Convert to String Hex
		// // Out in Edits
		CString str_hex;
		str_hex.Format(_T("%02X"), byte_H);
		this->m_Edit_Bias_H.SetWindowTextW(str_hex);

		str_hex.Truncate(0);

		str_hex.Format(_T("%02X"), byte_L);
		this->m_Edit_Bias_L.SetWindowTextW(str_hex);

		// > Control SECONDARY slider
		// check limit event
		int iMin = this->m_Slider_MOD.GetRangeMin();
		int iMax = this->m_Slider_MOD.GetRangeMax();
		int iPos2 = 0;

		if (iPos1 < iMin)
		{
			iPos2 = iMin;
		}
		else
		{
			if (iPos1 * 1.5 > iMax)
			{
				iPos2 = iMax;
			}
			else
			{
				iPos2 = iPos1 * 1.5;
			}
		}


		// set SECONDARY Slider position
		m_Slider_MOD.SetPos(iPos2);

		// convert to str
		CString str2;
		str2.Format(L"%d", iPos2);

		// Out to Interface control 
		pStaticMOD->SetWindowTextW(str2);

		// Out to Interface control Edits
		// // Get ByteH and ByteL from WORD
		//unsigned int _word;
		//unsigned char byte_H;
		//unsigned char byte_L;

		// // // ByteH
		_word = iPos2;
		_word &= 0xFF00;
		_word = _word >> 8;
		byte_H = (BYTE)_word;

		// // // ByteL
		_word = iPos2;
		_word &= 0x00FF;
		byte_L = (BYTE)_word;

		// // Convert to String Hex
		// // Out in Edits
		//CString str_hex;
		str_hex.Truncate(0);

		str_hex.Format(_T("%02X"), byte_H);
		this->m_Edit_Mod_H.SetWindowTextW(str_hex);

		str_hex.Truncate(0);

		str_hex.Format(_T("%02X"), byte_L);
		this->m_Edit_Mod_L.SetWindowTextW(str_hex);

		// > Update interface for Write controls
		UpdateData(FALSE);
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


	}

	if (pScrollBar == pSliderHOR)			//  HOR Slider
	{
		CWnd *pStaticHOR = this->GetDlgItem(IDC_STATIC_HOR);

		// Update contents for Read controls
		UpdateData(TRUE);

		// Get current Slider position
		char iPos = this->m_Slider_HORISON.GetPos();

		// convert to str
		CString str;
		str.Format(L"%d", iPos);

		// Out to Interface control Label
		pStaticHOR->SetWindowTextW(str);

		// Out to Interface control Edits
		// Format Value
		// FORMAT:
		// 1Byte = bits:: data:7654321, direction:0

		// define direction
		// define Value
		char direction;
		unsigned char iPosFormat;

		if (iPos < 0)
		{
			direction = 1;
			iPosFormat = iPos * (-1);
		}
		else
		{
			direction = 0;
			iPosFormat = iPos;
		}

		iPosFormat = iPosFormat << 1;
		iPosFormat &= 0xFE;
		iPosFormat += direction;

		// Out to Interface control Edits
		// // Convert to String Hex
		// // Out in Edits
		CString str_hex;
		str_hex.Format(_T("%02X"), iPosFormat);
		this->m_Edit_Horison.SetWindowTextW(str_hex);

		// Update interface for Write controls
		UpdateData(FALSE);

	}

	if (pScrollBar == pSliderVERT)			//  VERT Slider
	{
		CWnd *pStaticVERT = this->GetDlgItem(IDC_STATIC_VERT);

		// Update contents for Read controls
		UpdateData(TRUE);

		// Get current Slider position
		unsigned char iPos = this->m_Slider_VERTICAL.GetPos();

		// convert to str
		CString str;
		str.Format(L"%d", iPos);

		// Out to Interface control Label
		pStaticVERT->SetWindowTextW(str);

		// Out to Interface control Edits
		// Format Value
		// FORMAT:
		// 1Byte = bits:: data:7654321, active:0

		// define Status
		unsigned char iStatus;

		UpdateData(TRUE);
		if (m_bCheck_Vertical)
		{
			iStatus = 1;
		}
		else
		{
			iStatus = 0;
		}

		// define Value
		unsigned char iPosFormat;

		iPosFormat = iPos;
		iPosFormat = iPosFormat << 1;
		iPosFormat &= 0xFE;
		iPosFormat += iStatus;

		// Out to Interface control Edits
		// // Convert to String Hex
		// // Out in Edits
		CString str_hex;
		str_hex.Format(_T("%02X"), iPosFormat);
		this->m_Edit_Vertical.SetWindowTextW(str_hex);

		// Update interface for Write controls
		UpdateData(FALSE);

	}


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
			OnBnClickedButtonHorSet();
			OnBnClickedButtonVerticalSet();

			// Write
			OnBnClickedButton5();

		}

	}

	if (nSBCode == TB_THUMBTRACK)
	{
		
	}


	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDS4830A_SFPP_LR_CONF_ENGI::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CDS4830A_SFPP_LR_CONF_ENGI::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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


void CDS4830A_SFPP_LR_CONF_ENGI::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialogEx::OnOK();
}


void CDS4830A_SFPP_LR_CONF_ENGI::OnBnClickedCheckVertical()
{
	CWnd *pStaticVERT = this->GetDlgItem(IDC_STATIC_VERT);

	// Update contents for Read controls
	UpdateData(TRUE);

	// Get current Slider position
	unsigned char iPos = this->m_Slider_VERTICAL.GetPos();

	// convert to str
	CString str;
	str.Format(L"%d", iPos);

	// Out to Interface control Label
	pStaticVERT->SetWindowTextW(str);

	// Out to Interface control Edits
	// Format Value
	// FORMAT:
	// 1Byte = bits:: data:7654321, active:0

	// define Status
	// set Slidebar Enable State
	unsigned char iStatus;

	CWnd *wndSlide = this->GetDlgItem(IDC_SLIDER_VERTICAL);

	UpdateData(TRUE);
	if (m_bCheck_Vertical)
	{
		iStatus = 1;

		// enadle Control
		wndSlide->EnableWindow(TRUE);

	}
	else
	{
		iStatus = 0;

		// disable Control
		wndSlide->EnableWindow(FALSE);
	}

	// define Value
	unsigned char iPosFormat;

	iPosFormat = iPos;
	iPosFormat = iPosFormat << 1;
	iPosFormat &= 0xFE;
	iPosFormat += iStatus;

	// Out to Interface control Edits
	// // Convert to String Hex
	// // Out in Edits
	CString str_hex;
	str_hex.Format(_T("%02X"), iPosFormat);
	this->m_Edit_Vertical.SetWindowTextW(str_hex);

	// Update interface for Write controls
	UpdateData(FALSE);
}
