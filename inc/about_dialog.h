#include <windows.h>
#include <string>
#include <Shlobj.h>
#include <Windowsx.h>
#include "license.h"

LRESULT CALLBACK DlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
//char asdf [] = "Lilly Chalupowski";

LRESULT CALLBACK DlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{   HKEY hk;
    DWORD dwDisp;
    string strEmpty = "";
    string strComputerID;
    string strRegStatus = "registered";
    string strGetRegStatus;
    string strName;
    string strCompany;
    string strKey;
    HKEY hkey;
    DWORD reg_value, dw;
    int ireg_value;
    char Name[128];
    char Company[128];
    char asdf[256];
    char concatdata[256];
    HBITMAP hImage;
	switch(Msg)
	{
               
	case WM_INITDIALOG:
         
         strComputerID = computerID();
         SetDlgItemText(hWndDlg, IDC_EDIT4, string_char(strComputerID));
         strGetRegStatus = GetRegistry("RegStatus");
         if (strGetRegStatus.compare("registered") == 0){
            Edit_Enable(GetDlgItem(hWndDlg, IDC_EDIT3), false);
            Edit_Enable(GetDlgItem(hWndDlg, IDC_EDIT2), false);
            Edit_Enable(GetDlgItem(hWndDlg, IDC_EDIT1), false);
            strName = GetRegistry("Name");
            SetDlgItemText(hWndDlg, IDC_EDIT1, string_char(strName));
            strCompany = GetRegistry("Company");
            SetDlgItemText(hWndDlg, IDC_EDIT2, string_char(strCompany));
            strKey = GetRegistry("Key");
            SetDlgItemText(hWndDlg, IDC_EDIT3, string_char(strKey));
         }
         hImage = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP1));
         SendMessage(GetDlgItem(hWndDlg, IDC_STATIC1), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);
	     break;

	case WM_COMMAND: 
		switch(wParam)
		{
             case IDOK:
                  EndDialog(hWndDlg, 0);
                  break;
             case IDC_BUTTON1:
                  ShellExecute(hWndDlg, "open",
                 "http://www.lillychalaudio.com",
                 NULL, NULL, SW_SHOWNORMAL);
                  break;
             case IDC_BUTTON2:
                  strGetRegStatus = GetRegistry("RegStatus");
                  if (strGetRegStatus.compare("registered") == 0){MessageBox(NULL, TEXT("Already Registered!"), TEXT("Reg Satus"), MB_OK); break;}
                  //Yvyylvehyrm
                  //strcpy (str,"these ");
                  memset(Name, 0, sizeof(Name));
                  memset(Company, 0, sizeof(Company));
                  memset(asdf, 0, sizeof(asdf));
                  memset(concatdata, 0, sizeof(concatdata));
                  GetDlgItemText ( hWndDlg , IDC_EDIT1 , Name , 128); 
                  GetDlgItemText ( hWndDlg , IDC_EDIT2 , Company ,128); 
                  GetDlgItemText ( hWndDlg , IDC_EDIT3 , asdf , 128); 
                  strcpy(concatdata, Name);
                  strcat(concatdata, Company);
                  //aesencryptstr(Name);
                  //MessageBox(NULL, Name, TEXT("Thank You!"), MB_OK);
                  //aesdecryptstr(Name);
                  //MessageBox(NULL, Name, TEXT("Thank You!"), MB_OK);
                  //encryptstr(concatdata);
                  //aesencryptstr(concatdata);
                  //MessageBox(NULL, concatdata, TEXT("debug"), MB_OK);
                  //aesdecryptstr(concatdata);
                  //MessageBox(NULL, concatdata, TEXT("debug"), MB_OK);
                  aesencryptstr(concatdata);
                  if (strlen(concatdata) == 0 || strlen(Company) == 0 || strlen(asdf) == 0 || strcmp(concatdata, asdf) != 0){MessageBox(NULL, TEXT("Enter All Correctly"), TEXT("ERROR!"), MB_OK);break;}
                  if (strcmp(concatdata, asdf) == 0){
                     RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\WAV_INFO\\",0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&hk,&dwDisp);
                     RegSetValueExA(hk, "Name", 0, REG_SZ, (const BYTE*)Name, sizeof(Name) + 1);	
                     RegSetValueExA(hk, "Company", 0, REG_SZ, (const BYTE*)Company, sizeof(Company) + 1);
                     RegSetValueExA(hk, "RegStatus", 0, REG_SZ, (const BYTE*)strRegStatus.c_str(), strRegStatus.size() + 1);
                     RegSetValueExA(hk, "Key", 0, REG_SZ, (const BYTE*)concatdata, sizeof(concatdata) + 1);
                     Edit_Enable(GetDlgItem(hWndDlg, IDC_EDIT3), false);
                     Edit_Enable(GetDlgItem(hWndDlg, IDC_EDIT2), false);
                     Edit_Enable(GetDlgItem(hWndDlg, IDC_EDIT1), false);
                     MessageBox(NULL, TEXT("Registered!"), TEXT("Thank You!"), MB_OK);
                  }
                  break;
             case IDC_BUTTON3:
                  if (MessageBox(NULL, TEXT("Are you sure?"), TEXT("Unregister"), MB_OKCANCEL) == IDOK){
                  RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\WAV_INFO\\",0,KEY_WRITE | KEY_ALL_ACCESS,&hkey );
                  RegDeleteValue(hkey, "RegStatus");
                  RegDeleteValue(hkey, "Name");
                  RegDeleteValue(hkey, "Company");
                  RegDeleteValue(hkey, "Key");
                  SetDlgItemText(hWndDlg, IDC_EDIT1, string_char(strEmpty));
                  SetDlgItemText(hWndDlg, IDC_EDIT2, string_char(strEmpty));
                  SetDlgItemText(hWndDlg, IDC_EDIT3, string_char(strEmpty));
                  Edit_Enable(GetDlgItem(hWndDlg, IDC_EDIT3), true);
                  Edit_Enable(GetDlgItem(hWndDlg, IDC_EDIT2), true);
                  Edit_Enable(GetDlgItem(hWndDlg, IDC_EDIT1), true);
                  RegCloseKey(hkey);
                  RegDeleteKey(HKEY_CURRENT_USER, "Software\\WAV_INFO");
                  }
                  break;
		}
		break;
	}

	return FALSE;
}
