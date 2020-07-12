#pragma once
#include <Windows.h>
class ClientControl {
protected:
    HWND ParentWindow=NULL;
    HINSTANCE hInst=NULL;
    int index=0;
    const int margin = 2;
public:
    HWND hwnd1_ClientName=NULL;
    HWND hwnd2_Status = NULL;
    HWND hwnd3_IP = NULL;
    HWND hwnd4_Port = NULL;
    //HWND hwnd5_RecvMessage = NULL;
    HWND hwnd5a_RecvData = NULL;
    HWND hwnd5b_RecvData = NULL;
    HWND hwnd5c_RecvData = NULL;
    HWND hwnd5d_RecvData = NULL;
    //HWND hwnd6_SendMessage = NULL;
    HWND hwnd6a_SendData = NULL;
    HWND hwnd6b_SendData = NULL;
    HWND hwnd6c_SendData = NULL;
    HWND hwnd6d_SendData = NULL;
    HWND hwnd7_SendButton = NULL;
    HWND hwnd8_SendDataButton = NULL;

    RECT rect0_Control;

    RECT rect1_ClientName;
    RECT rect2_Status;
    RECT rect3_IP;
    RECT rect4_Port;
    //RECT rect5_RecvMessage;
    RECT rect5a_RecvData;
    RECT rect5b_RecvData;
    RECT rect5c_RecvData;
    RECT rect5d_RecvData;
    //RECT rect6_SendMessage;
    RECT rect6a_SendData;
    RECT rect6b_SendData;
    RECT rect6c_SendData;
    RECT rect6d_SendData;
    RECT rect7_SendButton;
    RECT rect8_SendDataButton;

protected:void Ini(HWND parentHwnd, HINSTANCE hInst, int top, int index) {
        this->ParentWindow = parentHwnd;
        this->hInst = hInst;
        this->index = index;

        rect0_Control.top = top;
        rect0_Control.bottom = 20;//Height
        rect0_Control.left = margin;

        rect1_ClientName.top = rect0_Control.top;
        rect2_Status.top = rect0_Control.top;
        rect3_IP.top = rect0_Control.top;
        rect4_Port.top = rect0_Control.top;
        //rect5_RecvMessage.top = rect0_Control.top;
        rect5a_RecvData.top = rect0_Control.top;
        rect5b_RecvData.top = rect0_Control.top;
        rect5c_RecvData.top = rect0_Control.top;
        rect5d_RecvData.top = rect0_Control.top;
        //rect6_SendMessage.top = rect0_Control.top;
        rect6a_SendData.top = rect0_Control.top;
        rect6b_SendData.top = rect0_Control.top;
        rect6c_SendData.top = rect0_Control.top;
        rect6d_SendData.top = rect0_Control.top;
        rect7_SendButton.top = rect0_Control.top;

        rect1_ClientName.bottom = rect0_Control.bottom;
        rect2_Status.bottom = rect0_Control.bottom;
        rect3_IP.bottom = rect0_Control.bottom;
        rect4_Port.bottom = rect0_Control.bottom;
        //rect5_RecvMessage.bottom = rect0_Control.bottom;
        rect5a_RecvData.bottom = rect0_Control.bottom;
        rect5b_RecvData.bottom = rect0_Control.bottom;
        rect5c_RecvData.bottom = rect0_Control.bottom;
        rect5d_RecvData.bottom = rect0_Control.bottom;
        //rect6_SendMessage.bottom = rect0_Control.bottom;
        rect6a_SendData.bottom = rect0_Control.bottom;
        rect6b_SendData.bottom = rect0_Control.bottom;
        rect6c_SendData.bottom = rect0_Control.bottom;
        rect6d_SendData.bottom = rect0_Control.bottom;
        rect7_SendButton.bottom = rect0_Control.bottom;

        rect1_ClientName.left = rect0_Control.left;
        rect1_ClientName.right = 60;//width
        rect2_Status.left = rect1_ClientName.left + rect1_ClientName.right + margin;
        rect2_Status.right = 70;//width
        rect3_IP.left = rect2_Status.left + rect2_Status.right + margin;
        rect3_IP.right = 70;//width
        rect4_Port.left = rect3_IP.left + rect3_IP.right + margin;
        rect4_Port.right = 70;//width
        //rect5_RecvMessage.left = rect4_Port.left + rect4_Port.right + margin;
        //rect5_RecvMessage.right = 220;//width
        rect5a_RecvData.left = rect4_Port.left + rect4_Port.right + margin;
        rect5a_RecvData.right = 40;//width
        rect5b_RecvData.left = rect5a_RecvData.left + rect5a_RecvData.right + margin;
        rect5b_RecvData.right = 40;//width
        rect5c_RecvData.left = rect5b_RecvData.left + rect5b_RecvData.right + margin;
        rect5c_RecvData.right = 80;//width
        rect5d_RecvData.left = rect5c_RecvData.left + rect5c_RecvData.right + margin;
        rect5d_RecvData.right = 80;//width
        //rect6_SendMessage.left = rect5_RecvMessage.left + rect5_RecvMessage.right + margin;
        //rect6_SendMessage.right = 220;//width
        rect6a_SendData.left = rect5d_RecvData.left + rect5d_RecvData.right + margin;
        rect6a_SendData.right = 40;//width
        rect6b_SendData.left = rect6a_SendData.left + rect6a_SendData.right + margin;
        rect6b_SendData.right = 40;//width
        rect6c_SendData.left = rect6b_SendData.left + rect6b_SendData.right + margin;
        rect6c_SendData.right = 80;//width
        rect6d_SendData.left = rect6c_SendData.left + rect6c_SendData.right + margin;
        rect6d_SendData.right = 80;//width
        rect7_SendButton.left = rect6d_SendData.left + rect6d_SendData.right + margin;
        rect7_SendButton.right = 80;//width
        rect8_SendDataButton.left = rect7_SendButton.left + rect7_SendButton.right + margin;
        rect8_SendDataButton.right = 80;//width
}
public: 
    ClientControl(HWND parentHwnd, HINSTANCE hInst) {

        Ini( parentHwnd,  hInst, 0, 0);
    }
    ClientControl(HWND parentHwnd, HINSTANCE hInst, int top, int index)
    {
        Ini( parentHwnd,  hInst, top, index);
    }
    ~ClientControl() {
        DestroyWindow(hwnd1_ClientName);
        DestroyWindow(hwnd2_Status);
        DestroyWindow(hwnd3_IP);
        DestroyWindow(hwnd4_Port);
        //DestroyWindow(hwnd5_RecvMessage);
        DestroyWindow(hwnd5a_RecvData);
        DestroyWindow(hwnd5b_RecvData);
        DestroyWindow(hwnd5c_RecvData);
        DestroyWindow(hwnd5d_RecvData);
        //DestroyWindow(hwnd6_SendMessage);
        DestroyWindow(hwnd6a_SendData);
        DestroyWindow(hwnd6b_SendData);
        DestroyWindow(hwnd6c_SendData);
        DestroyWindow(hwnd6d_SendData);
        DestroyWindow(hwnd7_SendButton);
    }
void CreateControlWindows() {
    wchar_t buffer[20],bufferNumber[20];
    _itow_s(this->index, bufferNumber, 10);
    wcscpy_s(buffer, L"Client[");
    wcscat_s(buffer, bufferNumber);
    wcscat_s(buffer, L"]");
    hwnd1_ClientName = CreateWindowClient(L"STATIC", buffer, 0, &rect1_ClientName);
    hwnd2_Status = CreateWindowClient(L"STATIC", L"UNKNOW", 0, &rect2_Status);
    hwnd3_IP = CreateWindowClient(L"STATIC", L"UNKNOW", 0, &rect3_IP);
    hwnd4_Port = CreateWindowClient(L"STATIC", L"UNKNOW", 0, &rect4_Port);
    //hwnd5_RecvMessage = CreateWindowClient(L"STATIC", L"-NO MESSAGE-", 0, &rect5_RecvMessage);
    hwnd5a_RecvData = CreateWindowClient(L"STATIC", L"", 0, &rect5a_RecvData);
    hwnd5b_RecvData = CreateWindowClient(L"STATIC", L"", 0, &rect5b_RecvData);
    hwnd5c_RecvData = CreateWindowClient(L"STATIC", L"", 0, &rect5c_RecvData);
    hwnd5d_RecvData = CreateWindowClient(L"STATIC", L"", 0, &rect5d_RecvData);
    //hwnd6_SendMessage = CreateWindowClient(L"EDIT", NULL, 0, &rect6_SendMessage);
    hwnd6a_SendData = CreateWindowClient(L"EDIT", NULL, 0, &rect6a_SendData);
    hwnd6b_SendData = CreateWindowClient(L"EDIT", NULL, 0, &rect6b_SendData);
    hwnd6c_SendData = CreateWindowClient(L"EDIT", NULL, 0, &rect6c_SendData);
    hwnd6d_SendData = CreateWindowClient(L"EDIT", NULL, 0, &rect6d_SendData);
    hwnd7_SendButton = CreateWindowClient(L"BUTTON", L"Send data", ES_LEFT, &rect7_SendButton);
    hwnd8_SendDataButton = CreateWindowClient(L"BUTTON", L"DATA", ES_LEFT, &rect8_SendDataButton);

}
protected:HWND CreateWindowClient(const wchar_t* predClass,const wchar_t* tittle, long style,RECT* dim) {
    return CreateWindowEx(
        0, predClass,   // predefined class 
        tittle,         // no window title 
        WS_CHILD | WS_BORDER | style | WS_VISIBLE,
        dim->left,
        dim->top,
        dim->right,      // width
        dim->bottom,     // height
        ParentWindow,   // parent window 
        NULL,           // No menu. 
        hInst,
        NULL);          // pointer not needed 
}
};
