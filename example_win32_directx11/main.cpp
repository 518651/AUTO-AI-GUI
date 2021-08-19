

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "API.h"
#include <d3d11.h>
#include <tchar.h>
#include <examples/example_win32_directx11/sttins.cpp>

using namespace std;

static char input_text_path[128] = "";




// Data
static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int main(int, char**)
{

    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("AUTO-AI������"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);


    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;


    
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);


    io.Fonts->AddFontFromFileTTF("c:/windows/fonts/simhei.ttf", 13.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

    // Our state
    bool auto_ai_window_main = true;
    bool show_demo_window = false;
    bool auto_ai_train = true;
    bool show_another_window = false;
    bool file_upload_windows = false;
    bool auto_ai_settings = false;
    bool auto_ai_system_sttings = false;
    bool auto_ai_process_about = false;

    /*��������*/
    bool environment_config = false;

    /*����ѡ��*/

  
    char environment_path[256]="";  //��װ����
    bool environment_pycharm = false;    //��װpycharm
    bool environment_Anaconda = false; //��װAnaconda
    bool environment_Anaconda_environment = false; //��װAI��Ӧ��Anaconda���⻷��
    bool environment_moniqi = false;   //��װ������ɵ�ģ����
  


   

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
      
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin(u8"AUTO-AI����ϵͳ������",&auto_ai_window_main,ImGuiWindowFlags_MenuBar);                      
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu(u8"�ļ�")) {
                    if (ImGui::MenuItem(u8"���ļ�", "Ctrl+O")) {}
                    if (ImGui::MenuItem(u8"����", "Ctrl+S")) {}
                    if (ImGui::MenuItem(u8"�ر�", "Ctrl+W")) { auto_ai_window_main = false; }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
           
             //ImGui::ShowAboutWindow(&auto_ai_process_about);
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu(u8"����")) {
                    if (ImGui::MenuItem(u8"���ڱ�����")) { auto_ai_process_about = true; }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            if (auto_ai_process_about)
            {
                ImGui::Begin(u8"���ڱ�����", &auto_ai_process_about);   
                ImGui::Text(u8"������ҫAI����ģ��汾:%s", ImGui::GetVersion());
                ImGui::Separator();
                ImGui::Text(u8"��ǰ�汾����:");
                ImGui::Text(u8"ͼ�λ�����ҳ�桢Զ���������������");
                ImGui::Text(u8"��ǰ�汾�Ż�:");
                ImGui::Text(u8"����������ϴ��ļ�������̶��ϼ��������ʱ��.");
                ImGui::Text(u8"ʹ��BT���ӵȼ�����ʽ����ͬһʱ���ڣ�����ѵ��ģ������Խ�࣬���������ٶ�Խ��");
                ImGui::Separator();
                ImGui::Text(u8"���������Ŀ��:�ڶ�ʱ����ʹ��Ⱥ�߷�����AI���ٳɳ���һ����ѵ��AI�ĳɼ��᲻̫���룬������Ⱥ����ԣ�Ⱥ�����ѵ����Ʒ�Ǹ���ѵ����Ʒ�ı���");
                if (ImGui::Button(u8"�ر�"))
                    auto_ai_process_about = false;
                ImGui::End();
            }






            char ����״̬[50]=u8"�ѳɹ�����AUTO-AI��������";
            char ���ݿ�״̬[]="NULL";
            char user_id[1024] = {NULL};
            int user_upload = 106000;
            int cuda_quantity = 2;
            string hore = u8"����-����";
            string cpu_information = u8"I7-10700F";
            char GPU_0_information[]=u8"IGAME-NVIDIA-GeFoce-RTX-3070TI";
            char GPU_1_information[]=u8"ROG-NVIDIA-GeForce-RTX-3060";
            char system_test []= u8"�豸���Զ�����ɶ�AI��ѵ��+ѧϰ,�������SERVER�豸";


            /*��ȡBIOSʶ���*/
            getBoisIDByCmd(user_id);


            /*�ж��Ƿ��������ݿ�*/

            int ���ݿ��ж�;
            ���ݿ��ж� = Connectmysql();

            
           

            ImGui::Text(u8"Holle,������!��ӭ��ʹ��AUTO-AI����ϵͳ");              
            ImGui::Separator();
            ImGui::Text(u8"�������ӷ�����״̬");
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.0f, 0.5f), u8"������������״̬:%s",& ����״̬);
            if (���ݿ��ж�)
            {
                char ���ݿ�״̬[50] = u8"�ѳɹ�����AI���ݿ�";
                ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.0f, 0.5f), u8"�������ݿ�״̬:%s", &���ݿ�״̬);
            }
            else
            {
                char ���ݿ�״̬[50] = u8"����AI���ݿ�ʧ��,���Ժ�����!";
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 0.5f), u8"�������ݿ�״̬:%s", &���ݿ�״̬);
            }       
            ImGui::Separator();
            ImGui::Text(u8"�û�״̬");
            ImGui::Text(u8"User-ID:%s",user_id);
            ImGui::Text(u8"ѵ��Ӣ��:%s",&hore);
            ImGui::Text(u8"�ϴ�����:%d", user_upload);
            ImGui::Separator();
            ImGui::Text(u8"�豸��Ϣ");
            ImGui::Text(u8"CPU��Ϣ:%s",&cpu_information);
            ImGui::Text(u8"��⵽CUDA�豸����:%d", cuda_quantity);
            ImGui::Text(u8"GPU-0:%s", &GPU_0_information);
            ImGui::Text(u8"GPU-1:%s", &GPU_1_information);
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.0f, 0.5f), u8"ϵͳ����:%s", &system_test);
            ImGui::Separator();
            static float arr[] = { 263.0f, 691.0f, 700.0f, 361.0f,381.0f, 264.0f, 631.0f };
            char text_look[50] = u8"ÿ��������������¼��ѵ������";
            ImGui::PlotHistogram(u8"����¼ѵ������ͳ��", arr, IM_ARRAYSIZE(arr), 0, text_look, 0.0f, 1500.0f, ImVec2(0, 80.0f));




            if (ImGui::CollapsingHeader(u8"��Ҫ���ܲ˵�"))
            {
                ImGui::Checkbox(u8"���ٿ�ʼѵ��", &auto_ai_train);      // Edit bools storing our window open/close state
                ImGui::Checkbox(u8"ѵ�������ϴ�", &file_upload_windows);
                ImGui::Checkbox(u8"AUTO-AIϵͳ����", &auto_ai_settings);

            }
            if (ImGui::CollapsingHeader(u8"�򿪷��Ŷӱ�������"))
            {
                ImGui::Text(u8"����BUG");
                ImGui::SameLine(); HelpMarker(
                    u8"���ڴ˷�����BUG��ֱ�ӷ��͸������Ŷӣ������Ŷ��յ�������\n"
                    u8"�Ὺʼ�����޸������ѶȵȲ�ȷ������Ӱ�죬�޷����������޸�ʱ��");
                const char* items[] = { u8"ѵ�������ϴ�����", u8"ѵ��ģ����������", u8"AI����һ����������", u8"AUTO-AI������������", u8"�����ĵ�����", u8"����������豸��������", u8"�������Ǳ�ĸ���" };
                static int item_current = 1;
                ImGui::ListBox(u8"ѡ����Ҫ���������", &item_current, items, IM_ARRAYSIZE(items), 4);

            }

            ImGui::End();
        }

       
        if (file_upload_windows)
        {
            ImGui::Begin(u8"ѵ�������ϴ�", &file_upload_windows);   
            ImGui::Text(u8"�ļ��ϴ�·��:%s",&input_text_path);
            //ImGui::Text(u8"F:\\python��Ŀ��\\WZCQ-main\\δ��");
            ImGui::InputTextWithHint(u8"�ļ�·��(ѵ������)", u8"�����Ҫ�޸�·����������", input_text_path, IM_ARRAYSIZE(input_text_path));
            if (ImGui::Button(u8"ɾ���ϴ�·��.."))
                ImGui::OpenPopup(u8"��Ҫɾ��·��?");

         
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal(u8"��Ҫɾ��·��?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text(u8"���ɾ����.\n����Ҫ���������ϴ��ļ���·��\n\n");
                ImGui::Separator();

                //static int unused_i = 0;
                //ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

                static bool dont_ask_me_next_time = false;
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                ImGui::Checkbox(u8"�´β�������", &dont_ask_me_next_time);
                ImGui::PopStyleVar();

                if (ImGui::Button(u8"ȷ��", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button(u8"�ر�", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }
            if (ImGui::Button(u8"�ر�"))
                show_another_window = false;
            ImGui::End();
        }

    

        if (auto_ai_settings)
        {
            ImGui::Begin(u8"AUTO-AI����",&auto_ai_settings);

            if (ImGui::CollapsingHeader(u8"�ļ�����"))
            {
                if (ImGui::TreeNode(u8"�ϴ��ļ�����"))
                {
                    ImGui::Text(u8"�ļ��ϴ�·��:%s\n", &input_text_path);
                    ImGui::InputTextWithHint(u8"�ļ�·��(ѵ������)\n", u8"�����Ҫ�޸�·����������\n", input_text_path, IM_ARRAYSIZE(input_text_path));
                    ImGui::SameLine(); HelpMarker(u8"������ʱ��Ҫ���ֶ�����ѵ�������ļ�����·��\n");
                    ImGui::Separator();
                    ImGui::Text(u8"�ȴ�����:");
                    ImGui::Separator();

                    ImGui::Text("END:");

                    ImGui::TreePop();
                }
                if (ImGui::TreeNode(u8"����ģ������"))
                {

                    ImGui::TreePop();
                }
                if (ImGui::TreeNode(u8"AI��������"))
                {
                    ImGui::Text(u8"�ļ��ϴ�·��:%s\n", &environment_path);
                    ImGui::InputTextWithHint(u8"AI������װ·��\n", u8"��������Ҫ��װ������·��\n", environment_path, IM_ARRAYSIZE(environment_path));
                    ImGui::TreePop();
                }
            }

            if (ImGui::CollapsingHeader(u8"��������"))
            {
                if (ImGui::BeginTable("sprots", 3))
                {
                    ImGui::TableNextColumn(); ImGui::Checkbox(u8"AI����һ������", &environment_config);
                    ImGui::SameLine(); HelpMarker(
                        u8"���С�׵���Ⱥ���õ�һ����������AI��ʹ�õĻ���\n"
                        u8"ȫ��������ANACONDA��PYCHARM��PYTHON��MINITOUCH��ADB�ȵ�ȫ�����ء���װ������\n"
                        u8"��������ɶԻ����������������ֱ�ӽ��в���\n");
                        


                    ImGui::EndTable();
                }
            }

            /*AI�������ô���*/
            if (environment_config)
            {
                ImGui::Begin(u8"AI��������", &environment_config);
                ImGui::Text(u8"�����ļ���װ·��:%s\n", &environment_path);
                /*ʹ���۵���ѡ��*/

                /*��������*/
                ImGui::End();
            }




     

            int clicked = 0;

            if (ImGui::CollapsingHeader(u8"���ڻ�������"))
            {
                if (ImGui::BeginTable("split", 3))
                {                  
                    ImGui::TableNextColumn(); ImGui::Checkbox(u8"���ر�����\n", &no_titlebar);
                    ImGui::TableNextColumn(); ImGui::Checkbox(u8"���ع�����\n", &no_scrollbar);
                    ImGui::TableNextColumn(); ImGui::Checkbox(u8"���ز˵�\n", &no_menu);
                    ImGui::TableNextColumn(); ImGui::Checkbox(u8"���عرհ���\n", &no_close);
                    ImGui::TableNextColumn(); ImGui::Checkbox(u8"���ر���\n", &no_background);
                    ImGui::TableNextColumn(); ImGui::Checkbox(u8"��ֹ������С\n", &no_resize);
                    ImGui::TableNextColumn(); ImGui::Checkbox(u8"��ֹ�쳣�˳�\n", &no_collapse);
                    ImGui::TableNextColumn(); ImGui::Checkbox(u8"δ������ĵ�", &unsaved_document);
                    ImGui::Separator();
                    ImGui::Text(u8"���ñ�����ɫ");               
                    ImGui::ColorEdit3(u8"������ɫ", (float*)&clear_color); // Edit 3 floats representing a color
                    ImGui::EndTable();
                }
            }
            if (ImGui::ShowStyleSelector(u8"������ɫ##Selector"))
                // ref_saved_style = style;
              
            if (ImGui::Button(u8"��������"))
                clicked++;
            if (clicked & 1)
            {
                ImGui::SameLine();
                ImGui::Text(u8"�������óɹ�!");
            }

            if (ImGui::Button(u8"�ر�����"))
                auto_ai_settings = false;
            ImGui::End();

        }


        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
