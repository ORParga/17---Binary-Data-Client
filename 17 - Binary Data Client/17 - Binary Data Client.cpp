// 17 - Binary Data Client.cpp : Define el punto de entrada de la aplicación.
//

#include "framework.h"
#ifndef WM_SOCKET
#define WM_SOCKET WM_USER + 1
#endif // !WM_SOCKET
#ifndef IDT_TIMER1
#define IDT_TIMER1 1
#endif // !IDT_TIMER1
#ifndef IDT_TIMER1_MILIS
#define IDT_TIMER1_MILIS 500
#endif
#define IDC_GRPBUTTONS 1001
#define IDC_CHK1 1002
#define IDC_CHK2 1003
#include "windowsx.h"
#include "17 - Binary Data Client.h"
#include "ClientControl_Client.h"
#include "OBDC_Client.h"
//#include "ObjectBinaryDataCommunicator.h"
#include "My_XTrace_Client.h"

#define MAX_LOADSTRING 100
#define IDC_MAINFRM_BTN_1               40501   //SEND button ID
// Variables globales:
HINSTANCE hInst;                                // Current Instance
WCHAR szTitle[MAX_LOADSTRING];                  // App Tittle
WCHAR szWindowClass[MAX_LOADSTRING];            // Window name

//*********************COMMON CONTROLS ***************************************************
HWND hwndButton, hwndStatic, hwndStatic2;// hwndMesageRec;
HWND hGrpButtons, hGrpRad1, hGrpRad2;
HWND hwndIP, hwndEditIP, hwndPort, hwndButtonReconnect;
HWND  hwndEditData1, hwndEditData2, hwndEditData3, hwndEditData4;
HWND  hwndRecvData1, hwndRecvData2, hwndRecvData3, hwndRecvData4;
RECT RadGroupIPRect = { 5,60,25,60 };
RECT RadIPRect1 = { 10,70,15,20 };
RECT RadIPRect2 = { 10,100,15,20 };
RECT ComboIPRect1 = { 30,70,150,200 };
RECT EditIPRect = { 30,100,150,20 };
RECT EditPortRect1 = { ComboIPRect1.left + ComboIPRect1.right + 10,70,80,20 };
RECT ButtonReconect = { EditPortRect1.left ,30,80,30 };

int topCoordMessages = 130;
RECT StaticRect1 = { 10,topCoordMessages,40,20 };
RECT StaticRect2 = { 80,topCoordMessages,100,20 };
RECT StaticRect3 = { 10,topCoordMessages + 30,40,20 };
RECT EditRect = { 80,topCoordMessages + 30,0,0 };
RECT ButtonRect = { 80,topCoordMessages + 60,100,20 };


RECT RecvRect1 = { 60,topCoordMessages,40,20 };
RECT RecvRect2 = { RecvRect1.left + RecvRect1.right + 5,topCoordMessages,40,20 };
RECT RecvRect3 = { RecvRect2.left + RecvRect2.right + 5,topCoordMessages,60,20 };
RECT RecvRect4 = { RecvRect3.left + RecvRect3.right + 5,topCoordMessages,60,20 };

RECT EditData1nRect = { 60,topCoordMessages + 30,40,20 };
RECT EditData2nRect = { EditData1nRect.left+ EditData1nRect.right+5,topCoordMessages + 30,40,20 };
RECT EditData3nRect = { EditData2nRect.left + EditData2nRect.right + 5,topCoordMessages + 30,60,20 };
RECT EditData4nRect = { EditData3nRect.left + EditData3nRect.right + 5,topCoordMessages + 30,60,20 };

int ID_SendButton = IDC_MAINFRM_BTN_1;
//********************WSA_NON_BLOKING initialization **************************************
OBDC_Client WSAnb_Client;

wchar_t IPString[20] = L"127.0.0.1";
int portNumber = 27015;
// ******************* FUNCTION DECLARATIONS **********************************************
ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND                InitInstance( int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int Ini_WSA_non_blocking_client(HWND);
void UpdateUI_IP_Port(HDC, RECT*);
void Ini_UI(HWND);
void ShowMessageControls(BOOL);

/// <summary>
/// App's Entry point
/// </summary>
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY17BINARYDATACLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Initialize app
    HWND hwnd = InitInstance(nCmdShow);
    if (!hwnd) {
        MessageBox(NULL,
            L"CreateWindowW() or UpdateWindow() has returned error.\rUnknow reasson.",
            L"ERROR. Main Window can not be created.",
            MB_OK | MB_ICONERROR);
        return FALSE;
    }

    if (!Ini_WSA_non_blocking_client(hwnd))
    {

        TCHAR szMessageFormatedBuffer[512];
        wchar_t ErrorMessage1[] = L"";
        MessageFormated(szMessageFormatedBuffer,
            L"Error al inicializar el socket cliente.\rCodigo de error:%u = %s",
            WSAnb_Client.lastWSAError,
            WSAnb_Client.WindowsErrorToString(WSAnb_Client.lastWSAError));
        MessageBox(NULL,
            szMessageFormatedBuffer,
            L"ERROR\n",
            MB_OK | MB_ICONERROR);
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY17BINARYDATACLIENT));

    MSG msg;

    // Main message loop
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

/// <summary>
/// Initialize Client Socket
/// Initialize TIMER
/// </summary>
/// <param name="hwnd">Main Window Handle</param>
/// <returns>TRUE if succeeds, FalSe if failure. If the failure occurred while
/// initializing the socket, the variable WSAnb_Client..lastWSAError saves the 
/// value returned by WSAGetLastError() </returns>
int Ini_WSA_non_blocking_client(HWND hwnd) {

    if (WSAnb_Client.CreateClientSocket() == FALSE)return FALSE;

    if (SetTimer(hwnd,              // handle to main window 
        IDT_TIMER1,             // timer identifier 
        IDT_TIMER1_MILIS,       // interval 
        (TIMERPROC)NULL)       // no timer callback 
        == 0)return FALSE;
    return TRUE;
}

/// <summary>
/// Initialize the User Interface
/// </summary>
/// <param name="hwnd">main Window Handle</param>
void Ini_UI(HWND hwnd) {
    //***********************************************Group Radio Button ***********************************************
    hGrpButtons = CreateWindowEx(WS_EX_WINDOWEDGE,
        L"BUTTON",
        L"",
        WS_VISIBLE | WS_CHILD | BS_GROUPBOX,// <----BS_GROUPBOX does nothing on the grouping 
        RadGroupIPRect.left,      // x position 
        RadGroupIPRect.top,       // y position 
        RadGroupIPRect.right,     // Combo width
        RadGroupIPRect.bottom,    // Combo height
        hwnd,
        (HMENU)IDC_GRPBUTTONS,
        hInst, NULL);
    hGrpRad1=CreateWindowEx(WS_EX_WINDOWEDGE,
        L"BUTTON",
        L"",
        WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP,  // <---- WS_GROUP group the following radio buttons 1st,2nd button 
        RadIPRect1.left,      // x position 
        RadIPRect1.top,       // y position 
        RadIPRect1.right,     // Combo width
        RadIPRect1.bottom,    // Combo height
        hwnd, //<----- Use main window handle
        (HMENU)IDC_CHK1,
        hInst, NULL);
    hGrpRad2=CreateWindowEx(WS_EX_WINDOWEDGE,
        L"BUTTON",
        L"",
        WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  // Styles 
        RadIPRect2.left,      // x position 
        RadIPRect2.top,       // y position 
        RadIPRect2.right,     // Combo width
        RadIPRect2.bottom,    // Combo height
        hwnd,
        (HMENU)IDC_CHK2,
        hInst, NULL);
    //*******************************IP COMBO BOX + IP Manual EDIT + PORT EDIT + REconnect Button***********
    hwndIP = CreateWindowEx(
        0, L"COMBOBOX",         // predefined class 
        L"Recieve:",            // no window title 
        CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        ComboIPRect1.left,      // x position 
        ComboIPRect1.top,       // y position 
        ComboIPRect1.right,     // Combo width
        ComboIPRect1.bottom,    // Combo height
        hwnd,                   // parent window 
        NULL,                   // No menu. 
        hInst,
        NULL);                  // pointer not needed 

    hwndEditIP  = CreateWindowEx(
        0, L"EDIT",             // predefined class 
        NULL,                   // no window title 
        WS_CHILD | WS_BORDER | WS_VISIBLE | ES_LEFT,
        EditIPRect.left,
        EditIPRect.top,
        EditIPRect.right,
        EditIPRect.bottom,   // set size in WM_SIZE message 
        hwnd,                   // parent window 
        NULL,                   // No menu. 
        hInst,
        NULL);                  // pointer not needed 

    hwndPort = CreateWindowEx(
        0, L"EDIT",             // predefined class 
        NULL,                   // no window title 
        WS_CHILD | WS_BORDER | WS_VISIBLE | ES_LEFT,
        EditPortRect1.left,
        EditPortRect1.top,
        EditPortRect1.right,
        EditPortRect1.bottom,   // set size in WM_SIZE message 
        hwnd,                   // parent window 
        NULL,                   // No menu. 
        hInst,
        NULL);                  // pointer not needed 

    hwndButtonReconnect = CreateWindow(
        L"BUTTON",              // Predefined class; Unicode assumed 
        L"Reconnect",                // Button text 
        WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,  // Styles 
        ButtonReconect.left,        // x position 
        ButtonReconect.top,         // y position 
        ButtonReconect.right,       // Button width
        ButtonReconect.bottom,      // Button height
        hwnd,                   // Parent window
        (HMENU)IDC_MAINFRM_BTN_1,// For buttons, hMenu is used to send the WM_BUTTON identifier
        hInst,
        NULL);                  // Pointer to identify the button in WndProc()

    /********************************************************************************/

    hwndStatic = CreateWindowEx(
        0, L"STATIC",           // predefined class 
        L"Recv:",            // no window title 
        WS_CHILD | WS_VISIBLE | ES_LEFT,
        StaticRect1.left,       // x position 
        StaticRect1.top,        // y position 
        StaticRect1.right,      // Button width
        StaticRect1.bottom,     // Button height
        hwnd,                   // parent window 
        NULL,                   // No menu. 
        hInst,
        NULL);                  // pointer not needed 


    hwndStatic2 = CreateWindowEx(
        0, L"STATIC",           // predefined class 
        L"Send:",               // no window title 
        WS_CHILD | ES_LEFT,
        StaticRect3.left,       // x position 
        StaticRect3.top,        // y position 
        StaticRect3.right,      // Button width
        StaticRect3.bottom,     // Button height
        hwnd,                   // parent window 
        NULL,                   // No menu. 
        hInst,
        NULL);                  // pointer not needed 

    hwndButton = CreateWindow(
        L"BUTTON",              // Predefined class; Unicode assumed 
        L"Send data.",                // Button text 
        WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        ButtonRect.left,        // x position 
        ButtonRect.top,         // y position 
        ButtonRect.right,       // Button width
        ButtonRect.bottom,      // Button height
        hwnd,                   // Parent window
        (HMENU)IDC_MAINFRM_BTN_1,// For buttons, hMenu is used to send the WM_BUTTON identifier
        hInst,
        NULL);                  // Pointer to identify the button in WndProc()

    hwndRecvData1 = CreateWindowEx(
        0, L"STATIC",           // predefined class 
        L"",          // no window title 
        WS_CHILD | WS_BORDER | ES_LEFT,
        RecvRect1.left,       // x position 
        RecvRect1.top,        // y position 
        RecvRect1.right,      // Button width
        RecvRect1.bottom,     // Button height
        hwnd,                   // parent window 
        NULL,                   // No menu. 
        hInst,
        NULL);                  // pointer not needed 

    hwndRecvData2 = CreateWindowEx(
        0, L"STATIC",           // predefined class 
        L"",          // no window title 
        WS_CHILD | WS_BORDER | ES_LEFT,
        RecvRect2.left,       // x position 
        RecvRect2.top,        // y position 
        RecvRect2.right,      // Button width
        RecvRect2.bottom,     // Button height
        hwnd,                   // parent window 
        NULL,                   // No menu. 
        hInst,
        NULL);                  // pointer not needed 

    hwndRecvData3 = CreateWindowEx(
        0, L"STATIC",           // predefined class 
        L"",          // no window title 
        WS_CHILD | WS_BORDER | ES_LEFT,
        RecvRect3.left,       // x position 
        RecvRect3.top,        // y position 
        RecvRect3.right,      // Button width
        RecvRect3.bottom,     // Button height
        hwnd,                   // parent window 
        NULL,                   // No menu. 
        hInst,
        NULL);                  // pointer not needed 

    hwndRecvData4 = CreateWindowEx(
        0, L"STATIC",           // predefined class 
        L"",          // no window title 
        WS_CHILD | WS_BORDER | ES_LEFT,
        RecvRect4.left,       // x position 
        RecvRect4.top,        // y position 
        RecvRect4.right,      // Button width
        RecvRect4.bottom,     // Button height
        hwnd,                   // parent window 
        NULL,                   // No menu. 
        hInst,
        NULL);                  // pointer not needed 

    hwndEditData1 = CreateWindowEx(
        0, L"EDIT",             // predefined class 
        NULL,                   // no window title 
        WS_CHILD | WS_BORDER | ES_LEFT,
        EditData1nRect.left,
        EditData1nRect.top,
        EditData1nRect.right,
        EditData1nRect.bottom,             // set size in WM_SIZE message 
        hwnd,                   // parent window 
        NULL,                   // No menu. 
        hInst,
        NULL);                  // pointer not needed 
    hwndEditData2 = CreateWindowEx(
        0, L"EDIT",             // predefined class 
        NULL,                   // no window title 
        WS_CHILD | WS_BORDER | ES_LEFT,
        EditData2nRect.left,
        EditData2nRect.top,
        EditData2nRect.right,
        EditData2nRect.bottom,             // set size in WM_SIZE message 
        hwnd,                   // parent window 
        NULL,                   // No menu. 
        hInst,
        NULL);                  // pointer not needed 
    hwndEditData3 = CreateWindowEx(
        0, L"EDIT",             // predefined class 
        NULL,                   // no window title 
        WS_CHILD | WS_BORDER | ES_LEFT,
        EditData3nRect.left,
        EditData3nRect.top,
        EditData3nRect.right,
        EditData3nRect.bottom,             // set size in WM_SIZE message 
        hwnd,                   // parent window 
        NULL,                   // No menu. 
        hInst,
        NULL);                  // pointer not needed 
    hwndEditData4 = CreateWindowEx(
        0, L"EDIT",             // predefined class 
        NULL,                   // no window title 
        WS_CHILD | WS_BORDER | ES_LEFT,
        EditData4nRect.left,
        EditData4nRect.top,
        EditData4nRect.right,
        EditData4nRect.bottom,             // set size in WM_SIZE message 
        hwnd,                   // parent window 
        NULL,                   // No menu. 
        hInst,
        NULL);                  // pointer not needed 

}


/// <summary>
/// Regiter the main window's class
/// </summary>
/// <returns>Returns a class atom that uniquely identifies the class. Zero if fails</returns>
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY17BINARYDATACLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY17BINARYDATACLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

/// <summary>
/// Create and shows the main window.
/// </summary>
/// <param name="nCmdShow">ShowWindow() parameter</param>
/// <returns>Main window handle</returns>
HWND InitInstance(int nCmdShow)
{

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 300, 300, nullptr, nullptr, hInst, nullptr);

    if (!hWnd)
    {
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return hWnd;
}
/// <summary>
/// Main window procedure
/// </summary>
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //Obtener el IP de la ordeñadora
    LPSOCKADDR sockaddr_ip;
    ADDRINFOW* ptr = NULL;
    INT iRetval;
    wchar_t ipstringbuffer[46];
    wchar_t itoa_buffer[46];
    DWORD ipbufferlength = 46;
    TCHAR  ListItem[256];

    switch (message)
    {
    case WM_TIMER:
    {
        switch (wParam)
        {
        case IDT_TIMER1:
            XTrace(L"WM_TIMER\n");
            switch (WSAnb_Client.state)
            {
            case OBDC_Client::STATE::NONE:
                WSAnb_Client.Attemp_connect(IPString, portNumber);
                break;
            case OBDC_Client::STATE::CONNECTED:
                _itow_s(WSAnb_Client.sharedData.data1, itoa_buffer, 10);
                SetWindowTextW(hwndRecvData1, (LPWSTR)itoa_buffer);
                _itow_s(WSAnb_Client.sharedData.data2, itoa_buffer, 10);
                SetWindowTextW(hwndRecvData2, (LPWSTR)itoa_buffer);
                _itow_s(WSAnb_Client.sharedData.data3, itoa_buffer, 10);
                SetWindowTextW(hwndRecvData3, (LPWSTR)itoa_buffer);
                _itow_s(WSAnb_Client.sharedData.data4, itoa_buffer, 10);
                SetWindowTextW(hwndRecvData4, (LPWSTR)itoa_buffer);
                break;
            default:
                break;
            }
            WSAnb_Client.testForEvents();

            InvalidateRect(hWnd, NULL, TRUE);
            return 0;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    case WM_CREATE:
    {
        Ini_UI(hWnd);
        // Add text to the Message editBox
        wchar_t text[] = L"255";
        SendMessage(hwndEditData1, WM_SETTEXT, 0, (LPARAM)text);
        SendMessage(hwndEditData2, WM_SETTEXT, 0, (LPARAM)L"127");
        //text[] = L"65536";
        SendMessage(hwndEditData3, WM_SETTEXT, 0, (LPARAM)L"65535");
        //text[] = L"65535";
        SendMessage(hwndEditData4, WM_SETTEXT, 0, (LPARAM)L"65536");
        // Add the Port Number to editBox
        wchar_t text2[10];
        _itow_s(portNumber, text2, 10);
        SendMessage(hwndPort, WM_SETTEXT, 0, (LPARAM)text2);
        //Add string to the Manual IP Edit box selector
        SendMessage(hwndEditIP, WM_SETTEXT, 0, (LPARAM)L"192.168.1.1");
        // Add string to Auto IP combobox.
        //First add the default localhost IP
        SendMessage(hwndIP, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)IPString);
        SendMessage(hGrpRad1, BM_SETCHECK, BST_CHECKED, 0);
        //Then. list the avaliable IP's on device
        ADDRINFOW* addrResult;
        WSAnb_Client.GetIPList(&addrResult);
        // Retrieve each address and print out the hex bytes
        for (ptr = addrResult; ptr != NULL; ptr = ptr->ai_next)
        {
            switch (ptr->ai_family) {
            case AF_UNSPEC:
                break;
            case AF_INET:
            {
                sockaddr_ip = (LPSOCKADDR)ptr->ai_addr;
                // The buffer length is changed by each call to WSAAddresstoString
                // So we need to set it for each iteration through the loop for safety
                ipbufferlength = 46;
                iRetval = WSAAddressToStringW(sockaddr_ip, (DWORD)ptr->ai_addrlen, NULL,
                    ipstringbuffer, &ipbufferlength);
                if (iRetval)
                    XTrace(L"WSAAddressToString failed with ", WSAGetLastError());
                else
                {
                    SendMessage(hwndIP, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)ipstringbuffer);
                    // Send the CB_SETCURSEL message to display an initial item 
                    //  in the selection field  
                    SendMessage(hwndIP, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
                }
                break;
            }
            }
        }
        return 0;
    }
    case WM_COMMAND:
    {
        if (lParam == (LPARAM)hwndButton)
        {
            const int textSize = 1000;
            char text[textSize];
            unsigned long numberConverted_dword = 0;
            unsigned char numberConverted_byte = 0;
            SharedClass local_sharedData;
            local_sharedData.SetDataFromWindow(hwndEditData1, &local_sharedData.data1);
            local_sharedData.SetDataFromWindow(hwndEditData2, &local_sharedData.data2);
            local_sharedData.SetDataFromWindow(hwndEditData3, &local_sharedData.data3);
            local_sharedData.SetDataFromWindow(hwndEditData4, &local_sharedData.data4);
            WSAnb_Client.SendData( local_sharedData);
            return 0;
        }
        if (lParam == (LPARAM)hwndButtonReconnect)
        {
            //Get IP 
            if (SendMessage(hGrpRad1, BM_GETCHECK, 0, 0))
            {
                //IP Auto
                int ItemIndex = SendMessage(hwndIP, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                (TCHAR)SendMessage(hwndIP, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)IPString);
            }
        else
        {
            //IP Manual
                GetWindowText(hwndEditIP,IPString,sizeof(IPString));
        }
            //Get Port
            const int textSize = 1000;
            wchar_t text[textSize];
            if (GetWindowText(hwndPort, text, textSize) != 0)
            {
                //If the number is correct, we use it.
                //If not, the older is used
                unsigned int number = _wtoi(text);
                if ((number < 65535) && (number > 0)) {
                    portNumber = number;
                }
            }
            WSAnb_Client.Attemp_connect(IPString, portNumber);
        }
        if (lParam == (LPARAM)hwndIP) {
            if (HIWORD(wParam) == CBN_SELCHANGE) {
            }
        }
        if (lParam == (LPARAM)hwndPort) {
            if (HIWORD(wParam) == EN_CHANGE) {
                const int textSize = 1000;
                wchar_t text[textSize];
                if (GetWindowText(hwndPort, text, textSize) != 0)
                {
                    //If the number is correct, we use it.
                    //If not, the older is used
                    unsigned int number = _wtoi(text);
                    if ((number < 65535) && (number > 0)) {
                        portNumber = number;
                    }
                }
            }
        }
        int wmId = LOWORD(wParam);
        // Analizar las selecciones de menú:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            return 0;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            return 0;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_SETFOCUS:
        SetFocus(hwndEditData1);
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Agregar cualquier código de dibujo que use hDC aquí...
        RECT TextRect = { 10,10,300,100 };
        switch (WSAnb_Client.state)
        {
        case OBDC_Client::STATE::NONE:
            DrawText(hdc, L"Estado:None", -1, &TextRect, DT_VCENTER | DT_LEFT);
            ShowMessageControls(FALSE);
            break;
        case OBDC_Client::STATE::CONNECTED:
            DrawText(hdc, L"Estado:Conectado", -1, &TextRect, DT_VCENTER | DT_LEFT);
            UpdateUI_IP_Port(hdc, &TextRect);
            ShowMessageControls(TRUE);
            break;
        case OBDC_Client::STATE::REQUESTING:
            DrawText(hdc, L"Estado:Solicitando conexion", -1, &TextRect, DT_VCENTER | DT_LEFT);
            UpdateUI_IP_Port(hdc, &TextRect);
            ShowMessageControls(FALSE);
            break;
        default:
            DrawText(hdc, L"Estado:Desconocido", -1, &TextRect, DT_VCENTER | DT_LEFT);
            ShowMessageControls(FALSE);
            break;
        }
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_SIZE:
    {
        // Make the edit control the size of the window's client area. 
        //MoveWindow(hwndMesageRec,
        //    StaticRect2.left, StaticRect2.top,                    // starting x- and y-coordinates 
        //    LOWORD(lParam) - StaticRect2.left - 20,                // width of client area 
        //    20,                // height of client area 
        //    TRUE);                 // repaint window 

        //MoveWindow(hwndEdit,
        //    EditRect.left, EditRect.top,                    // starting x- and y-coordinates 
        //    LOWORD(lParam) - EditRect.left - 20,                // width of client area 
        //    20,                // height of client area 
        //    TRUE);                 // repaint window 
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

/// <summary>
/// Show the Client's basic parametres ( Status, Ip and Port ) 
/// </summary>
/// <param name="hdc">Device context handler received by BeginPaint () on WM_PAINT</param>
/// <param name="message">Texto para ser mostrado en primer lugar</param>
void UpdateUI_IP_Port(HDC hdc, RECT* TextRect) {
    TextRect->top += 20;
    TextRect->bottom += 20;
    DrawText(hdc, WSAnb_Client.IPString, -1, TextRect, DT_VCENTER | DT_LEFT);
    TextRect->top += 20;
    TextRect->bottom += 20;
    DrawText(hdc, WSAnb_Client.PortString, -1, TextRect, DT_VCENTER | DT_LEFT);

}

/// <summary>
/// Shows or hides the send/recv User Interface
/// </summary>
/// <param name="show">TRUE= show, FALSE = hide</param>
void ShowMessageControls(BOOL show) {
    ShowWindow(hwndStatic, show ? SW_SHOW : SW_HIDE);
    ShowWindow(hwndStatic2, show ? SW_SHOW : SW_HIDE);
    ShowWindow(hwndRecvData1, show ? SW_SHOW : SW_HIDE);
    ShowWindow(hwndRecvData2, show ? SW_SHOW : SW_HIDE);
    ShowWindow(hwndRecvData3, show ? SW_SHOW : SW_HIDE);
    ShowWindow(hwndRecvData4, show ? SW_SHOW : SW_HIDE);
    ShowWindow(hwndEditData1, show ? SW_SHOW : SW_HIDE);
    ShowWindow(hwndEditData2, show ? SW_SHOW : SW_HIDE);
    ShowWindow(hwndEditData3, show ? SW_SHOW : SW_HIDE);
    ShowWindow(hwndEditData4, show ? SW_SHOW : SW_HIDE);
    ShowWindow(hwndButton, show ? SW_SHOW : SW_HIDE);
}
// Controlador de mensajes del cuadro Acerca de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
