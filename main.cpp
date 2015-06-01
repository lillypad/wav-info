#include <windows.h>
#include <fstream>
#include <string>
#include <iostream>
#include "audioresource.h"
#include "engine.h"
#include "resource.h"
#include ".\\inc\\about_dialog.h"
//#include ".\\inc\\fft_dialog.h"
//#include ".\\inc\\license.h"
#include ".\\inc\\libzplay.h"
#include <sstream>
//HINSTANCE hZPlayDll = LoadLibrary("libzplay.dll");

using namespace libZPlay;

HINSTANCE hDialog1;
HINSTANCE hDialog2;
HWND ffthwnd;
HWND hwnd;
HWND hSlider;
HWND hPlayButton;
HWND hStopButton;
HWND hPauseButton;
HWND hSelFFT;
HWND hFftVUL;
HWND hFftVUR;
RECT hSliderRect;
POINT hSliderPoint;
unsigned int iLeftVU;
unsigned int iRightVU;
/*  Declare Windows procedure  */
int WINAPI WinMain (HINSTANCE hThisInstance,HINSTANCE hPrevInstance,LPSTR lpszArgument,int nFunsterStil);
LRESULT CALLBACK WindowProcedure (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    bool test = 0;
RECT rect;
ZPlay *player = CreateZPlay();
int result;
TStreamInfo info;
/*  Make the class name into a global variable  */
char szClassName[ ] = "WAV_INFO";
char szClassName2[ ] = "FFTAPP";
int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
                   /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    WNDCLASSEX wClass2;
    memset(&wincl,0,sizeof(WNDCLASSEX));
	memset(&wClass2,0,sizeof(WNDCLASSEX));
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));


    wClass2.cbSize=sizeof(WNDCLASSEX);
	wClass2.style=CS_HREDRAW|CS_VREDRAW;
	wClass2.lpfnWndProc=(WNDPROC)WinProc2;
	//wClass2.cbClsExtra=NULL;
	//wClass2.cbWndExtra=NULL;
	wClass2.hInstance=hThisInstance;
	wClass2.hIcon=NULL;
	wClass2.hCursor=LoadCursor(NULL,IDC_ARROW);
	wClass2.hbrBackground=CreateSolidBrush(RGB(0, 0, 0));;
	wClass2.lpszMenuName=NULL;
	wClass2.lpszClassName="ffthwnd";
	wClass2.hIconSm=NULL; 

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl)){
        return 0;
        }
    if (!RegisterClassEx (&wClass2)){
        return 0;
        }

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "WAV_INFO",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
    ffthwnd = CreateWindowEx(0,"ffthwnd","FFT Spectrogram",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,800,300,NULL,NULL,hThisInstance,NULL);
    hSlider = CreateWindow ("msctls_trackbar32", NULL, WS_CHILD | TBS_HORZ | TBS_FIXEDLENGTH | WM_ERASEBKGND, 0, 0, 800, 25, ffthwnd, NULL, hThisInstance, NULL);
   
    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);
    ShowWindow (hSlider, nFunsterStil);
   
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

//String Vars
bool MULTI_WAV_PAINT = false;
string headerhexdump[500];
int filepathlist = 0;
int displistinc = 0;
int multihexloop = 0;
string strOpenMultiSingleBack;
int MULTI_HEXDUMP_TOGGLE = 0;
LPSTR lpstrLine1, lpstrLine2, lpstrLine3, lpstrLine4, lpstrLine5, lpstrFileName, lpstrLine6, lpstrLine7, lpstrLine8, lpstrLine9, lpstrLine10, lpstrAudioFormat, lpstrLine11;
string strOpen, strOpenMulti, strSave;
string strMultiSaveHex;
//Audio Properties Strings
string audioformat;
string asdf;
string line1;
string line2;
string line3;
string line4;
string line5;
string line6;
string line7;
string line8;
string line9;
string line10;
string line11;
stringstream ss(line11);
int iline11;
string filename;
string strGetRegStatus;
int playerms;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{  
        //TCHAR szMessage[] = "Paint Beginner";
        //TCHAR lpstrLine1[128];
        //UINT  nLen = sizeof(szMessage);
    //Search Directory
    UINT counter(0);
    bool working(true);
    string buffer;
    string fileName[1000];
    int testint;
    //string directory = "C:\\Users\\Lilly\\Desktop\\";
    string extension = "*.wav";
    //directory = directory.append(extension);
    WIN32_FIND_DATA myimage;
    HANDLE myHandle;

    //Multiout Text File
    ofstream multiout;
    ifstream  multihexdump;
    string strMultiHexDump;
    int textline = 0;

    std::ofstream file;
    LPSTR fail = "FAILED";
    PAINTSTRUCT ps;
    HDC hdc;
    //LPSTR lpstrLine1, lpstrLine2, lpstrLine3, lpstrLine4, lpstrLine5, lpstrFileName, lpstrLine6, lpstrLine7, lpstrLine8, lpstrLine9, lpstrLine10, lpstrAudioFormat;
    string strCancel = "CANCEL";
    switch (message)                  /* handle the messages */
    {
        case WM_COMMAND:
             switch (LOWORD (wParam)){
                    case IDM_SAVE1:
                    if (lpstrFileName == NULL){MessageBox(NULL, TEXT("File Not Open!"), TEXT("ERROR!"), MB_OK); break;}
                    strSave = save_file(hwnd);
                    if (strSave == "CANCEL"){ break;}
                    file.open(string_char(strSave),std::ios::out ) ; 
                    file << lpstrFileName << std::endl;
                    file << lpstrLine1    << std::endl;
                    file << lpstrLine2    << std::endl;
                    file << lpstrLine3    << std::endl;
                    file << lpstrLine4    << std::endl;
                    file << lpstrLine5    << std::endl;
                    file << lpstrLine6    << std::endl;
                    file << lpstrLine7    << std::endl;
                    file << lpstrLine8    << std::endl;
                    file << lpstrLine9    << std::endl;
                    file << lpstrLine10   << std::endl;
                    file.close();
                    break;
                    case IDM_OPEN1:
                         //Open the File
                         strOpen = load_file(hwnd);
                         
                         if (strOpen.compare(strCancel) != 0 && wav_info(strOpen,0) != fail)
                         {
                              //Show File Name
                              filename = strOpen.substr( strOpen.find_last_of("\\") + 1 );
                              filename.insert(0, "File Name: ");
                              lpstrFileName = string_char(filename);
                              
                              //Show File Type
                              if (strncmp(wavheader.RIFF, "RIFF", sizeof(wavheader.RIFF)) == 0){
                              line1 = "RIFF";
                              line1.insert(0, "File Type: ");
                              lpstrLine1 = string_char(line1);
                              }
                              
                              //Show Audio Format
                              audioformat = getAudioFormat(strOpen);
                              audioformat.insert(0, "Audio Format: ");
                              lpstrAudioFormat = string_char(audioformat);
                              
                              //Show Sample Rate
                              line2 = wav_info(strOpen, 1);
                              line2.insert(0, "Sample Rate: ");
                              line2.append("hz");
                              lpstrLine2 = string_char(line2);
                              
                              //Show Chunk Size in bytes
                              line3 = wav_info(strOpen, 2);
                              line3.insert(0, "Chunk Size: ");
                              line3.append(" bytes");
                              lpstrLine3 = string_char(line3);
                              
                              //Show Fmt Chunk Size
                              line4 = wav_info(strOpen, 3);
                              line4.insert(0, "FMT Chunk Size: ");
                              line4.append(" bytes");
                              lpstrLine4 = string_char(line4);
                              
                              //Show Number of Channels
                              line5 = wav_info(strOpen, 4);
                              line5.insert(0, "Channels: ");
                              lpstrLine5 = string_char(line5);
                              
                              //Show Bytes Per Second
                              line6 = wav_info(strOpen, 5);
                              line6.insert(0, "Bytes/Second: ");
                              lpstrLine6 = string_char(line6);
                              
                              //Show Bits/Sample
                              line7 = wav_info(strOpen, 6);
                              line7.insert(0, "Bits/Sample: ");
                              lpstrLine7 = string_char(line7);
                              
                              //Show Data Flag Offset
                              line8 = wav_info(strOpen, 7);
                              line8.insert(0, "Data Offset: ");
                              line8.append(" bytes");
                              lpstrLine8 = string_char(line8);
                              
                              //Show Subchunk2Size
                              line9 = wav_info(strOpen, 8);
                              line9.insert(0, "Subchunk2Size: ");
                              line9.append(" bytes");
                              lpstrLine9 = string_char(line9);
                              
                              //Show Block Align
                              line10 = wav_info(strOpen, 9);
                              line10.insert(0, "Block Align: ");
                              lpstrLine10 = string_char(line10);
                              
                              line11 = wav_info(strOpen, 11);
                              line11.insert(0, "Duration: ");
                              line11.append(" sec");
                              lpstrLine11 = string_char(line11);
                              ss >> iline11;
                              
                              InvalidateRect(hwnd, NULL, TRUE);
                             
                         }
                         
                         break;
                    case IDM_PLAY_PREVIEW1:
                         if (strncmp(wavheader.RIFF, "RIFF", sizeof(wavheader.RIFF)) == 0){
                         strGetRegStatus = GetRegistry("RegStatus");
                         if (strGetRegStatus.compare("registered") != 0){
                         PlaySound(MAKEINTRESOURCE(IDW_SOUND1), NULL, SND_RESOURCE | SND_NOSTOP);
                         }
                         PlaySound(string_char(strOpen), NULL, SND_FILENAME | SND_ASYNC);
                         }
                         if (strOpen.length() <= 0){MessageBox(NULL, TEXT("File Not Open!"), TEXT("ERROR!"), MB_OK);}
                         break;
                    case IDM_STOP_PREVIEW1:
                         PlaySound(NULL, 0, 0);
                         break;
                    case IDM_OPEN_MULTI1:
                         strGetRegStatus = GetRegistry("RegStatus");
                         if (strGetRegStatus.compare("registered") != 0){MessageBox(NULL, TEXT("Register For This Feature"), TEXT("ERROR!"), MB_OK); break;}
                         strOpenMulti = BrowseFolder();
                         if (strOpenMulti == "CANCEL"){ break;}
                         strSave = save_file(hwnd);
                         if (strSave == "CANCEL"){ break;}
                         strOpenMultiSingleBack = strOpenMulti;
                         strOpenMulti = strOpenMulti.append("\\");
                         strOpenMulti = pathSlashAdd(strOpenMulti);
                         strOpenMulti = strOpenMulti.append(extension);
                          //MessageBox(NULL, string_char(strOpenMulti), TEXT("ERROR!"), MB_OK);
                         myHandle  = FindFirstFile(string_char(strOpenMulti),&myimage);
                         if(myHandle!=INVALID_HANDLE_VALUE){
                              buffer=myimage.cFileName;
                              fileName[counter]=buffer;
                         while(working){
                              FindNextFile(myHandle,&myimage);
                              if(myimage.cFileName!=buffer){
                                   buffer=myimage.cFileName;
                                   ++counter;
                                   fileName[counter]=buffer;
                                   
                                   }
                              else{
                                   //end of files reached
                                   working=false;
                              }
                            }
                         }
                         
                            strOpenMultiSingleBack.append("\\");
                            multiout.open(string_char(strSave)); 
                            while (filepathlist < 1000){
                             if (fileName[filepathlist].empty()){MULTI_WAV_PAINT = false; break;}
                             //fileName[filepathlist].append(strOpenMultiSingleBack);
                             fileName[filepathlist].insert(0,strOpenMultiSingleBack);
                             //MessageBox(NULL, string_char(fileName[filepathlist]), TEXT("ERROR!"), MB_OK);
                             MULTI_WAV_PAINT = true;

                             
                             
                             //Show File Name
                             filename = fileName[filepathlist].substr( fileName[filepathlist].find_last_of("\\") + 1 );
                             filename.insert(0, "File Name: ");
                             //lpstrFileName = string_char(filename);
                             multiout << filename << endl;
                             
                             //Show File Type
                             line1 = "RIFF";
                             line1.insert(0, "File Type: ");
                             lpstrLine1 = string_char(line1);
                             multiout << line1 << endl;
                              
                             //Show Sample Rate
                             line2 = wav_info(fileName[filepathlist], 1);
                             line2.insert(0, "Sample Rate: ");
                             line2.append("hz");
                             lpstrLine2 = string_char(line2);
                             multiout << line2 << endl;
                             
                             //Show Chunk Size in bytes
                             line3 = wav_info(fileName[filepathlist], 2);
                             line3.insert(0, "Chunk Size: ");
                             line3.append(" bytes");
                             lpstrLine3 = string_char(line3);
                             multiout << line3 << endl;
                             
                             //Show Fmt Chunk Size
                             line4 = wav_info(fileName[filepathlist], 3);
                             line4.insert(0, "FMT Chunk Size: ");
                             line4.append(" bytes");
                             lpstrLine4 = string_char(line4);
                             multiout << line4 << endl;
                              
                             //Show Number of Channels
                             line5 = wav_info(fileName[filepathlist], 4);
                             line5.insert(0, "Channels: ");
                             lpstrLine5 = string_char(line5);
                             multiout << line5 << endl;
                       
                             //Show Bytes Per Second
                             line6 = wav_info(fileName[filepathlist], 5);
                             line6.insert(0, "Bytes/Second: ");
                             lpstrLine6 = string_char(line6);
                             multiout << line6 << endl;
                             
                             //Show Bits/Sample
                             line7 = wav_info(fileName[filepathlist], 6);
                             line7.insert(0, "Bits/Sample: ");
                             lpstrLine7 = string_char(line7);
                             multiout << line7 << endl;
                             
                             //Show Data Flag Offset
                             line8 = wav_info(fileName[filepathlist], 7);
                             line8.insert(0, "Data Offset: ");
                             line8.append(" bytes");
                             lpstrLine8 = string_char(line8);
                             multiout << line8 << endl;
                             
                             //Show Subchunk2Size
                             line9 = wav_info(fileName[filepathlist], 8);
                             line9.insert(0, "Subchunk2Size: ");
                             line9.append(" bytes");
                             lpstrLine9 = string_char(line9);
                             multiout << line9 << endl;
                             
                             //Show Block Align
                             line10 = wav_info(fileName[filepathlist], 9);
                             line10.insert(0, "Block Align: ");
                             lpstrLine10 = string_char(line10);
                             multiout << line10 << endl;
                             
                             multiout << endl;
                             
                             if (MULTI_HEXDUMP_TOGGLE == true){
                                hexdump_multi(fileName[filepathlist], headerhexdump);
                                multiout << "hexdump:" << endl;
                                while ( multihexloop < 500){
                                      if (headerhexdump[multihexloop].empty()){break;}
                                      multiout << headerhexdump[multihexloop] << endl;
                                      headerhexdump[multihexloop].clear();
                                      multihexloop++;
                                }
                                
                                multihexloop = 0;
                                multiout << endl;
                                //break;
                             }
                             
                             filepathlist++;
                             
                             }
                             MessageBox(NULL, TEXT("Analysis Complete"), TEXT("Complete!"), MB_OK);
                             filepathlist = 0;
                             multiout.close();
                         break;
                    case IDM_EXIT1:
                         DestroyWindow(hwnd);
                         break;
                    case IDM_HEX_DUMP_HEADER1:
                         
                         if (lpstrFileName == NULL){MessageBox(NULL, TEXT("File Not Open!"), TEXT("ERROR!"), MB_OK); break;}
                         strSave = save_file(hwnd);
                         if (strSave.compare(strCancel) != 0)
                         {
                         hexdump(strOpen, strSave);
                         }
                         
                         break;
                    case IDM_HELP1:
                         
                         break;
                    case IDM_OPEN_MULTI_WITH_HEADER_HEX_DUMP1:
                         strGetRegStatus = GetRegistry("RegStatus");
                         if (strGetRegStatus.compare("registered") != 0){MessageBox(NULL, TEXT("Register For This Feature"), TEXT("ERROR!"), MB_OK); break;}
                         switch (MULTI_HEXDUMP_TOGGLE){
                                case 0:                     
                                  ModifyMenu(GetMenu(hwnd), IDM_OPEN_MULTI_WITH_HEADER_HEX_DUMP1, MF_BYCOMMAND | MF_CHECKED, IDM_OPEN_MULTI_WITH_HEADER_HEX_DUMP1, " Hex Dump (enabled)");
                                  MULTI_HEXDUMP_TOGGLE = 1;
                                  break;
                                case 1:                       
                                  ModifyMenu(GetMenu(hwnd), IDM_OPEN_MULTI_WITH_HEADER_HEX_DUMP1, MF_BYCOMMAND | MF_UNCHECKED, IDM_OPEN_MULTI_WITH_HEADER_HEX_DUMP1, "Open Multi with Header Hex Dump");
                                  MULTI_HEXDUMP_TOGGLE = 0;
                                  break;
                                }     
                                                                      
                         break;
                    case IDM_FFT_TOOL1:
                         strGetRegStatus = GetRegistry("RegStatus");
                         if (strGetRegStatus.compare("registered") != 0){MessageBox(NULL, TEXT("Register For This Feature"), TEXT("ERROR!"), MB_OK); break;}
                         if (strOpen.length() <= 0){MessageBox(NULL, TEXT("File Not Open!"), TEXT("ERROR!"), MB_OK); break;}
                         if (strOpen == "CANCEL" ){MessageBox(NULL, TEXT("File Not Open!"), TEXT("ERROR!"), MB_OK); break;}
                         result = player->OpenFile(string_char(strOpen), sfAutodetect);
                          if(result == 0)
                          {
                          // display error message
                          //printf("Error: %s\n", player->GetError());
                          MessageBox(NULL, player->GetError(), TEXT("ERROR!"), MB_OK);
                          player->Release();
                               //return 0;
                          }
                          
    
    player->GetStreamInfo(&info);

    printf("Length: %02u:%02u:%02u:%03u\n\n", info.Length.hms.hour, 
                                                info.Length.hms.minute,
                                                info.Length.hms.second,
                                                info.Length.hms.millisecond);

    // set graph type to AREA, left channel on top
    player->SetFFTGraphParam(gpGraphType, gtSpectrum);
    //player->SetFFTGraphParam(gpHorizontalScale, gpFrequencyScaleVisible  );
    // set linear scale
    player->SetFFTGraphParam(gpHorizontalScale, gsLinear);
                         ShowWindow (ffthwnd, SW_SHOW);
                         
                         test = 1;
                         //MessageBox(NULL, string_char(line11), TEXT("ERROR!"), MB_OK);
                         SendMessage(hSlider,TBM_SETRANGE, (WPARAM)1,(LPARAM)MAKELONG(0,(info.Length.sec)));
                         
                         player->Play(); 
                         
                                                 while(test == 1){
                         
       GetWindowRect(ffthwnd, &rect);
        // get stream status to check if song is still playing
        TStreamStatus status;
        player->GetStatus(&status); 
        if(status.fPlay == 0)
            break; // exit checking loop
        player->GetVUData(&iLeftVU, &iRightVU);
        SendMessage(hFftVUL, PBM_SETPOS, iLeftVU, 0);
        SendMessage(hFftVUR, PBM_SETPOS, iRightVU, 0);
        // get current position
        TStreamTime pos;
        
        player->GetPosition(&pos);
        // display position
        printf("Pos: %02u:%02u:%02u:%03u\r", pos.hms.hour, pos.hms.minute, pos.hms.second, pos.hms.millisecond);
        SendMessage(hSlider,TBM_SETPOS, (WPARAM)1,pos.sec);

        // draw FFT graph on desktop window
        //rect.right - rect.left - 13, rect.bottom - rect.top - 15
        player->DrawFFTGraphOnHWND(ffthwnd, 0, 0, rect.right - rect.left - 55, rect.bottom - rect.top - 100);
        
        //SetWindowPos(hSlider, HWND_TOPMOST, 0, hSliderPoint.y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
        XSleepAnotherWay(30);
        }
                         break;
                    case IDM_ABOUT1:
                         DialogBox(hDialog1, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, reinterpret_cast<DLGPROC>(DlgProc));
                         //DialogBox(hDialog2, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, reinterpret_cast<DLGPROC>(fftDlg));
                         //SetParent(ffthwnd, hwnd);
                         
                         //MessageBox(0, "By Lilly Chalupowski", "About", MB_OK);
                         break;
                    default:
                            break;
                    }
             break;
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps); 
            SetBkMode( hdc,TRANSPARENT );
            SetTextColor(hdc,RGB(0,255,0));
            TextOut(hdc, 0, 0, filename.c_str(), filename.length()); 
            TextOut(hdc, 0, 14, line1.c_str(), line1.length()); 
            TextOut(hdc, 0, 28, line2.c_str(), line2.length()); 
            TextOut(hdc, 0, 42, line3.c_str(), line3.length()); 
            TextOut(hdc, 0, 56, line4.c_str(), line4.length()); 
            TextOut(hdc, 0, 70, line5.c_str(), line5.length()); 
            TextOut(hdc, 0, 84, line6.c_str(), line6.length()); 
            TextOut(hdc, 0, 98, line7.c_str(), line7.length()); 
            TextOut(hdc, 0, 112, line8.c_str(), line8.length()); 
            TextOut(hdc, 0, 126, line9.c_str(), line9.length()); 
            TextOut(hdc, 0, 140, line10.c_str(), line10.length()); 
            TextOut(hdc, 0, 154, audioformat.c_str(), audioformat.length()); 
            TextOut(hdc, 0, 168, line11.c_str(), line11.length());
            EndPaint(hwnd, &ps);
            break;
        case WM_CLOSE:
             test = 0; 
             player->Close();
             player->Stop();
             //test = 0;
            //player->Release();
            //player->Stop();
            //DestroyWindow(ffthwnd);
            ShowWindow(ffthwnd, SW_HIDE);
            PostQuitMessage (0);
             break;
        case WM_DESTROY:
             test = 0; 
             player->Stop();
             player->Release();
             ShowWindow(ffthwnd, SW_HIDE);
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
bool bPlaying = true;

LRESULT CALLBACK WinProc2 (HWND ffthwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    LPCSTR hSelFFTList[7];
    hSelFFTList[0] = "Spectrum";
    hSelFFTList[1] = "Lines RCT";
    hSelFFTList[2] = "Lines LCT";
    hSelFFTList[3] = "Area RCT";
    hSelFFTList[4] = "Area LCT";
    hSelFFTList[5] = "Bars RCT";
    hSelFFTList[6] = "Bars LCT";
    int iSelFFTList;
    int ihSliderPos;
    char buffer[20];
    TStreamTime hSliderTime;
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
             
             hPlayButton = CreateWindow("BUTTON","Play", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,0, 0,125,25,ffthwnd,(HMENU) 1,NULL, NULL);
             hStopButton = CreateWindow("BUTTON","Stop", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,130, 400,125,25,ffthwnd,(HMENU) 2,NULL, NULL);
             hPauseButton = CreateWindow("BUTTON","Pause", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,260, 400,125,25,ffthwnd,(HMENU) 3,NULL, NULL);
             hSelFFT = CreateWindow(WC_COMBOBOX, TEXT(""), CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 310, 400, 125, 210, ffthwnd, NULL, NULL,NULL);
             hFftVUL = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | PBS_SMOOTH | PBS_VERTICAL ,390, 400, 17, 260, ffthwnd, NULL, NULL, NULL);
             hFftVUR = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | PBS_SMOOTH | PBS_VERTICAL ,390, 400, 17, 260, ffthwnd, NULL, NULL, NULL);
             SendMessage(hFftVUL, PBM_SETRANGE, 0, MAKELPARAM(0, 100)); 
             SendMessage(hSelFFT,(UINT)CB_ADDSTRING,(WPARAM) 0,(LPARAM)hSelFFTList[0]); 
             SendMessage(hSelFFT,(UINT)CB_ADDSTRING,(WPARAM) 0,(LPARAM)hSelFFTList[1]); 
             SendMessage(hSelFFT,(UINT)CB_ADDSTRING,(WPARAM) 0,(LPARAM)hSelFFTList[2]); 
             SendMessage(hSelFFT,(UINT)CB_ADDSTRING,(WPARAM) 0,(LPARAM)hSelFFTList[3]); 
             SendMessage(hSelFFT,(UINT)CB_ADDSTRING,(WPARAM) 0,(LPARAM)hSelFFTList[4]); 
             SendMessage(hSelFFT,(UINT)CB_ADDSTRING,(WPARAM) 0,(LPARAM)hSelFFTList[5]); 
             SendMessage(hSelFFT,(UINT)CB_ADDSTRING,(WPARAM) 0,(LPARAM)hSelFFTList[6]); 
             SendMessage(hSelFFT, CB_SETCURSEL , 0, 0);
             break;
        case WM_SIZE:
        GetWindowRect(ffthwnd, &rect);
        //hSliderPoint.x = hSliderRect.left;
        //hSliderPoint.y = hSliderRect.top;
        //ScreenToClient(hSlider, &hSliderPoint);
        MoveWindow(hSlider, 0, rect.bottom - rect.top - 65, rect.right - rect.left - 13, 25, true);
        MoveWindow(hPlayButton, 0, rect.bottom - rect.top - 90, 125, 25, true);
        MoveWindow(hStopButton, 130, rect.bottom - rect.top - 90, 125, 25, true);
        MoveWindow(hPauseButton, 260, rect.bottom - rect.top - 90, 125, 25, true);
        MoveWindow(hSelFFT, 390, rect.bottom - rect.top - 90, 125, 210, true);
        MoveWindow(hFftVUL, rect.right - rect.left - 55, 0, 20, rect.bottom - rect.top - 100, true);
        MoveWindow(hFftVUR, rect.right - rect.left - 35, 0, 20, rect.bottom - rect.top - 100, true);
        //SetWindowPos(hSlider, HWND_TOPMOST, 0, rect.bottom - rect.top - 25, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
             break;
        case WM_COMMAND:
             switch (HIWORD(wParam)){
                    case CBN_SELCHANGE:
                         iSelFFTList = (int)SendMessage(hSelFFT, CB_GETCURSEL, 0, 0);
                         //sprintf(buffer, "Selection: %d", iSelFFTList + 1);
                         //MessageBox(ffthwnd, buffer, "Select Change", MB_OK );
                         if (iSelFFTList + 1 == 2){player->SetFFTGraphParam(gpGraphType, gtLinesRightOnTop );}
                         if (iSelFFTList + 1 == 1){player->SetFFTGraphParam(gpGraphType, gtSpectrum );}
                         if (iSelFFTList + 1 == 3){player->SetFFTGraphParam(gpGraphType, gtLinesLeftOnTop );}
                          if (iSelFFTList + 1 == 4){player->SetFFTGraphParam(gpGraphType, gtAreaRightOnTop );}
                           if (iSelFFTList + 1 == 5){player->SetFFTGraphParam(gpGraphType, gtAreaLeftOnTop );}
                           if (iSelFFTList + 1 == 6){player->SetFFTGraphParam(gpGraphType, gtBarsLeftOnTop );}
                           if (iSelFFTList + 1 == 7){player->SetFFTGraphParam(gpGraphType, gtBarsRightOnTop );}
                         break;
             }
             if (LOWORD(wParam) == 1){   
                ihSliderPos = SendMessage(hSlider, TBM_GETPOS, 0, 0);
                hSliderTime.sec = ihSliderPos;
                player->Seek(tfSecond, &hSliderTime, smFromBeginning);
                test = 1;    
                player->Play();
                while(test == 1){
                         
       GetWindowRect(ffthwnd, &rect);
        // get stream status to check if song is still playing
        TStreamStatus status;
        player->GetStatus(&status); 
        if(status.fPlay == 0)
            break; // exit checking loop
player->GetVUData(&iLeftVU, &iRightVU);
        SendMessage(hFftVUL, PBM_SETPOS, iLeftVU, 0);
        SendMessage(hFftVUR, PBM_SETPOS, iRightVU, 0);
        // get current position
        TStreamTime pos;
        
        player->GetPosition(&pos);
        // display position
        printf("Pos: %02u:%02u:%02u:%03u\r", pos.hms.hour, pos.hms.minute, pos.hms.second, pos.hms.millisecond);
        SendMessage(hSlider,TBM_SETPOS, (WPARAM)1,pos.sec);
        // draw FFT graph on desktop window
        //rect.right - rect.left - 13, rect.bottom - rect.top - 15
        player->DrawFFTGraphOnHWND(ffthwnd, 0, 0, rect.right - rect.left - 55, rect.bottom - rect.top - 100);
        
        //SetWindowPos(hSlider, HWND_TOPMOST, 0, hSliderPoint.y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
        XSleepAnotherWay(30);
        }
             }   
             if (LOWORD(wParam) == 2){  
                ihSliderPos = 0;
                hSliderTime.sec = ihSliderPos;
                SendMessage(hSlider, TBM_SETPOS,(WPARAM) TRUE,(LPARAM) ihSliderPos);
                player->Seek(tfSecond, &hSliderTime, smFromBeginning);     
                player->Stop();
             }   
             if (LOWORD(wParam) == 3){  
                //ihSliderPos = 0;
                //hSliderTime.sec = ihSliderPos;
                //SendMessage(hSlider, TBM_SETPOS,(WPARAM) TRUE,(LPARAM) ihSliderPos);
                //player->Seek(tfSecond, &hSliderTime, smFromBeginning);     
                player->Pause();
             }   
             break;
        case WM_KEYDOWN:
             switch (LOWORD(wParam)){
                    case VK_SPACE:
                         switch (bPlaying){
                                case true: 
                                     player->Pause();
                                     test = 0;
                                     bPlaying = false;
                                     break;
                                case false:
                                     bPlaying = true;
                                     ihSliderPos = SendMessage(hSlider, TBM_GETPOS, 0, 0);
                hSliderTime.sec = ihSliderPos;
                player->Seek(tfSecond, &hSliderTime, smFromBeginning);
                test = 1;    
                player->Play();
                while(test == 1){
                         
       GetWindowRect(ffthwnd, &rect);
        // get stream status to check if song is still playing
        TStreamStatus status;
        player->GetStatus(&status); 
        if(status.fPlay == 0)
            break; // exit checking loop
player->GetVUData(&iLeftVU, &iRightVU);
        SendMessage(hFftVUL, PBM_SETPOS, iLeftVU, 0);
        SendMessage(hFftVUR, PBM_SETPOS, iRightVU, 0);
        // get current position
        TStreamTime pos;
        
        player->GetPosition(&pos);
        // display position
        printf("Pos: %02u:%02u:%02u:%03u\r", pos.hms.hour, pos.hms.minute, pos.hms.second, pos.hms.millisecond);
        SendMessage(hSlider,TBM_SETPOS, (WPARAM)1,pos.sec);
        // draw FFT graph on desktop window
        //rect.right - rect.left - 13, rect.bottom - rect.top - 15
        player->DrawFFTGraphOnHWND(ffthwnd, 0, 0, rect.right - rect.left - 55, rect.bottom - rect.top - 100);
        
        //SetWindowPos(hSlider, HWND_TOPMOST, 0, hSliderPoint.y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
        XSleepAnotherWay(30);
        }
                                     
                                     break;
                                }
                          //player->Pause();
                         // if (!player->Pause()){player->Pause(); bPlaying = true;}
                          //if (bPlaying = true){
                          //if (!player->Play()){player->Pause();}
                         break;
             }
             break;
        case WM_CLOSE:
             test = 0; 
             player->Close();
             player->Stop();
             //test = 0;
            //player->Release();
            //player->Stop();
            //DestroyWindow(ffthwnd);
            ShowWindow(ffthwnd, SW_HIDE);
            break;
        case WM_DESTROY:
             
            //DestroyWindow(ffthwnd);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (ffthwnd, message, wParam, lParam);
    }

    return 0;
}
