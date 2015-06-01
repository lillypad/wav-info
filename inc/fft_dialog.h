#include <windows.h>
#include <string>
#include <stdio.h>
#include <conio.h>
//#include "fft.h"
#include "libzplay.h"


void XSleepAnotherWay(DWORD dwWaitInMSecs)

{

    DWORD dwStopTime = GetTickCount() + dwWaitInMSecs;

    DWORD dwTimeout = dwWaitInMSecs;

    MSG msg;

    while(1) {

        switch(MsgWaitForMultipleObjects(0, NULL, FALSE, dwTimeout, QS_ALLINPUT)) {

        case WAIT_TIMEOUT:

            break;

        default:

#ifdef __AFX_H__

            while(::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) 

                AfxGetApp()->PumpMessage();

#else

            while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

                ::TranslateMessage(&msg);

                ::DispatchMessage(&msg);

            }

#endif

        }

        dwTimeout = dwStopTime - GetTickCount();

        if(!dwTimeout || dwTimeout>dwWaitInMSecs)

            break;

    }

}

using namespace libZPlay;
int iStartStop = 1;
ZPlay *player = CreateZPlay();
int result = player->OpenFile("test.wav", sfAutodetect);

/*
void stuff(HWND hWnd){
if(result == 0)
    {
        // display error message
        printf("Error: %s\n", player->GetError());
        player->Release();
        //return 0;
    }
TStreamInfo info;
player->GetStreamInfo(&info);
         player->SetFFTGraphParam(gpGraphType, gtAreaLeftOnTop);
         player->SetFFTGraphParam(gpHorizontalScale, gsLinear);
         
         player->Play();

    // display position and wait for song end
    
    while(iStartStop == 1)
    {
        // check key press
        if(kbhit())
        {
            int a = getch();
            if(a == 'q' || a == 'Q')
                break; // end program if Q key is pressed
        }
        
        // get stream status to check if song is still playing
        TStreamStatus status;
        player->GetStatus(&status); 
        if(status.fPlay == 0)
            break; // exit checking loop

        // get current position
        TStreamTime pos;
        player->GetPosition(&pos);
        // display position
        printf("Pos: %02u:%02u:%02u:%03u\r", pos.hms.hour, pos.hms.minute, pos.hms.second, pos.hms.millisecond);
        // draw FFT graph on desktop window
        player->DrawFFTGraphOnHWND(hWnd, 0, 0, 300, 200);

        XSleepAnotherWay(30); // wait 300 ms
    }
}
*/
LRESULT CALLBACK WndProc(HWND ffthwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI fftWinMain(HINSTANCE hFft, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

 MSG msg          = {0};
 WNDCLASS wc      = {0}; 
 wc.lpfnWndProc   = WndProc;
 wc.hInstance     = hFft;
 wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
 wc.lpszClassName = "minwindowsapp";
 if( !RegisterClass(&wc) )
   return 1;

 ffthwnd = CreateWindowEx (
                      WS_EX_TOPMOST,
                      "static",
                      "\n        Please Wait...",
                      WS_POPUP | WS_DLGFRAME,
                      437, //438
                      344, //383
                      150, //149
                      60,  //39
                      ffthwnd,
                      NULL,
                      GetModuleHandle( NULL ),
                      NULL
             );
             ShowWindow(ffthwnd, SW_SHOW);
    return 2;

    while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
        DispatchMessage( &msg );

    return 0;
}

LRESULT CALLBACK WndProc(HWND ffthwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

 switch(message)
 {
   case WM_CLOSE:
     PostQuitMessage(0);
     break;
   default:
     return DefWindowProc(ffthwnd, message, wParam, lParam);
 }
 return 0;

}  
