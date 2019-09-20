#include "stdafx.h"

#include <d3d9.h>
#include <d3dx9.h>

#include "CText.h"


CText::CText(LPDIRECT3DDEVICE9 pD3DDevice, int width, int height)
{
	D3DXFONT_DESC desc;
	ZeroMemory(&desc, sizeof(D3DXFONT_DESC));

	desc.CharSet = HANGUL_CHARSET;
	desc.Height = height;
	desc.Width = width;
	desc.Weight = FW_NORMAL; //FW_BOLD;
	desc.Quality = DEFAULT_QUALITY;
	desc.MipLevels = 1;

	auto hr = D3DXCreateFontIndirect(pD3DDevice, &desc, &m_pFont);
	
	if (FAILED(hr)) {
		throw 2L;
	}
}


CText::~CText()
{
	if (m_pFont != NULL) {
		m_pFont->Release();
		m_pFont = NULL;
	}	
}

void CText::Draw(int x, int y, int width, int height, const TCHAR* lpszText,
	D3DCOLOR color /*= D3DCOLOR_XRGB(255,0,0)*/) {
	RECT rc;
	SetRect(&rc, x, y, x + width, y + height);
	
	m_pFont->DrawTextW(NULL, lpszText, -1, &rc, DT_CENTER | DT_VCENTER, color);
}

void CText::DrawLT(int x, int y, int width, int height, const TCHAR* lpszText,
	D3DCOLOR color /*= D3DCOLOR_XRGB(255,0,0)*/) {
	RECT rc;
	SetRect(&rc, x, y, x + width, y + height);
	m_pFont->DrawTextW(NULL, lpszText, -1, &rc, DT_LEFT | DT_TOP, color);
}
