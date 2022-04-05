#include <afxwin.h>   //MFC core and standard components
#include <afxdlgs.h>
#include "resource.h"    //main symbols
#include <stdio.h>
#include <windows.h>
#include <Wincrypt.h>

//#pragma comment (lib, "Advapi32.lib")
#pragma comment (lib, "crypt32.lib")


//Globals
CButton  * pButtonimp;
CButton  * pButtonquit;

class GAME_FORM : public CDialog
{
public:
	GAME_FORM(CWnd* pParent = NULL) : CDialog(GAME_FORM::IDD, pParent)
	{    }

	// Dialog Data, name of dialog form
	enum{ IDD = IDD_GAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) { CDialog::DoDataExchange(pDX); }

	//Called right after constructor. Initialize things here.
	virtual BOOL OnInitDialog()
	{
		CDialog::OnInitDialog();
		pButtonimp = (CButton *)GetDlgItem(IDOK);
		pButtonquit = (CButton *)GetDlgItem(IDCANCEL);
		
		return true;
	}
public:
	
	afx_msg void installcer()
	{  
		TCHAR szFilters[] = _T("CER Files (*.cer)|*.cer||");

		// Create an Open dialog; the default file name extension is ".my".
		CFileDialog dlg(TRUE, _T("cer"), _T("*.cer"),
			OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

        CString filename;
        if (dlg.DoModal() == IDOK)
        {

			filename = dlg.GetPathName(); // return full path and filename
			PCCERT_CONTEXT pCertCtx = NULL;

			if (CryptQueryObject(
				CERT_QUERY_OBJECT_FILE,
				filename,
				CERT_QUERY_CONTENT_FLAG_ALL,
				CERT_QUERY_FORMAT_FLAG_ALL,
				0,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				(const void **)&pCertCtx) != 0)
			{
				HCERTSTORE hCertStoremy = CertOpenStore(
					CERT_STORE_PROV_SYSTEM,
					0,
					0,
					CERT_STORE_OPEN_EXISTING_FLAG |
					CERT_SYSTEM_STORE_CURRENT_USER,
					L"My");
				if (hCertStoremy != NULL)
				{
					if (CertAddCertificateContextToStore(
						hCertStoremy,
						pCertCtx,
						CERT_STORE_ADD_ALWAYS,
						NULL))
					{
						MessageBox(_T("Added certificate to store." ));
					}

					if (CertCloseStore(hCertStoremy, 0))
					{
						MessageBox(_T("Cert. store handle closed." ));
					}


				}

				HCERTSTORE hCertStore = CertOpenStore(
					CERT_STORE_PROV_SYSTEM,
					0,
					0,
					CERT_STORE_OPEN_EXISTING_FLAG |
					CERT_SYSTEM_STORE_CURRENT_USER,
					L"ROOT");
				if (hCertStore != NULL)
				{
					if (CertAddCertificateContextToStore(
						hCertStore,
						pCertCtx,
						CERT_STORE_ADD_ALWAYS,
						NULL))
					{
						MessageBox(_T("Added certificate to store."));
					}

					if (CertCloseStore(hCertStore, 0))
					{
						MessageBox(_T("Cert. store handle closed."));
					}


				}

				if (pCertCtx)
				{
					CertFreeCertificateContext(pCertCtx);
				}
			}

		}
	}
	afx_msg void closdialog()
	{
		CDialog::OnCancel();
	}

	DECLARE_MESSAGE_MAP()
};

//-----------------------------------------------------------------------------------------

class TheGame : public CWinApp
{
public:
	TheGame() {  }

public:
	virtual BOOL InitInstance()
	{
		CWinApp::InitInstance();
	    GAME_FORM dlg;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		return FALSE;

	} //close function
	
};

//-----------------------------------------------------------------------------------------
//Need a Message Map Macro for both CDialog and CWinApp

BEGIN_MESSAGE_MAP(GAME_FORM, CDialog)
	ON_COMMAND(IDOK, installcer)
	ON_COMMAND(IDCANCEL, closdialog)

END_MESSAGE_MAP()

//-----------------------------------------------------------------------------------------
TheGame theApp;  //Starts the Application

