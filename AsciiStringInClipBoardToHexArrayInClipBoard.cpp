#include "AsciiStringInClipBoardToHexArrayInClipBoard.h"
#include "Windows.h"

static void GetClipBoardStringIntoBuffer(char* inBuff, unsigned int sizeOfInBuff)
{
	HANDLE hGlobal = NULL;
	bool bAvailable = FALSE;
	LPVOID pGlobal;

	bAvailable = IsClipboardFormatAvailable(CF_TEXT);
	if (bAvailable)
	{
		OpenClipboard(NULL);
		hGlobal = GetClipboardData(CF_TEXT);
		if (hGlobal)
		{
			if (sizeOfInBuff >= GlobalSize(hGlobal))
			{
				pGlobal = GlobalLock(hGlobal);
				lstrcpyA(inBuff, (char*)pGlobal);
			}
			else
			{
				MessageBox(NULL, L"insurficient memory during getting data from clipboard", L"!", MB_OK);
			}
		}
		GlobalUnlock(hGlobal);
		CloseClipboard();
	}
	else
	{
		MessageBox(NULL, L"clipboard doesn't have any text data", L"!", MB_OK);
	}
}

static void AsciiStringToHexArray(char * asciiString)
{
	for (size_t i = 0; i < lstrlenA(asciiString); i++)
	{
		wsprintfA(m_HexBuffer + i*5, "0x%02x,", asciiString[i]);
	}
}

static void SetBackToClipBoard(char * String, SIZE_T lengthOfasciiString)
{
	HANDLE hGlobal = NULL;
	PCHAR pGlobal = NULL;
	size_t i = 0;
	WCHAR SuccessMsg[MAX_PATH];

	hGlobal = GlobalAlloc(GHND | GMEM_SHARE, lengthOfasciiString + 1);
	if (hGlobal)
	{
		pGlobal = (PCHAR)GlobalLock(hGlobal);
		for (i = 0; i < lengthOfasciiString; i++)
		{
			*pGlobal++ = String[i];
		}
		GlobalUnlock(hGlobal);
		OpenClipboard(NULL);
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hGlobal);
		CloseClipboard();

		wsprintf(SuccessMsg, L"success to set %d elements back to clipboard!", lengthOfasciiString/5);
		MessageBox(NULL, SuccessMsg, L"success!", MB_OK);
	}
	else
	{
		MessageBox(NULL, L"insurficient memory during setting back to clipboard", L"!", MB_OK);
	}
}

void AsciiStringInClipBoardToHexArrayInClipBoard(void)
{
	GetClipBoardStringIntoBuffer(m_CharBuffer, sizeof(m_CharBuffer));
	AsciiStringToHexArray(m_CharBuffer);
	SetBackToClipBoard(m_HexBuffer, lstrlenA(m_HexBuffer));
}