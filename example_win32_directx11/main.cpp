

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
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("AUTO-AI主程序"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);


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

    /*环境窗口*/
    bool environment_config = false;

    /*环境选项*/

  
    char environment_path[256]="";  //安装环境
    bool environment_pycharm = false;    //安装pycharm
    bool environment_Anaconda = false; //安装Anaconda
    bool environment_Anaconda_environment = false; //安装AI对应的Anaconda虚拟环境
    bool environment_moniqi = false;   //安装配置完成的模拟器
  


   

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

            ImGui::Begin(u8"AUTO-AI辅助系统主窗口",&auto_ai_window_main,ImGuiWindowFlags_MenuBar);                      
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu(u8"文件")) {
                    if (ImGui::MenuItem(u8"打开文件", "Ctrl+O")) {}
                    if (ImGui::MenuItem(u8"保存", "Ctrl+S")) {}
                    if (ImGui::MenuItem(u8"关闭", "Ctrl+W")) { auto_ai_window_main = false; }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
           
             //ImGui::ShowAboutWindow(&auto_ai_process_about);
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu(u8"工具")) {
                    if (ImGui::MenuItem(u8"关于本程序")) { auto_ai_process_about = true; }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            if (auto_ai_process_about)
            {
                ImGui::Begin(u8"关于本程序", &auto_ai_process_about);   
                ImGui::Text(u8"王者荣耀AI辅助模块版本:%s", ImGui::GetVersion());
                ImGui::Separator();
                ImGui::Text(u8"当前版本新增:");
                ImGui::Text(u8"图形化操作页面、远程请求服务器帮助");
                ImGui::Text(u8"当前版本优化:");
                ImGui::Text(u8"批量打包待上传文件，极大程度上减少所需的时间.");
                ImGui::Text(u8"使用BT种子等技术方式。在同一时间内，下载训练模型人数越多，本地下载速度越快");
                ImGui::Separator();
                ImGui::Text(u8"本软件开发目的:在短时间内使用群策方案让AI快速成长。一个人训练AI的成绩会不太理想，但换成群体策略，群体策略训练成品是个人训练成品的倍数");
                if (ImGui::Button(u8"关闭"))
                    auto_ai_process_about = false;
                ImGui::End();
            }






            char 联网状态[50]=u8"已成功连接AUTO-AI数据中心";
            char 数据库状态[]="NULL";
            char user_id[1024] = {NULL};
            int user_upload = 106000;
            int cuda_quantity = 2;
            string hore = u8"射手-后裔";
            string cpu_information = u8"I7-10700F";
            char GPU_0_information[]=u8"IGAME-NVIDIA-GeFoce-RTX-3070TI";
            char GPU_1_information[]=u8"ROG-NVIDIA-GeForce-RTX-3060";
            char system_test []= u8"设备可以独立完成对AI的训练+学习,无需调用SERVER设备";


            /*获取BIOS识别号*/
            getBoisIDByCmd(user_id);


            /*判断是否连接数据库*/

            int 数据库判断;
            数据库判断 = Connectmysql();

            
           

            ImGui::Text(u8"Holle,开发者!欢迎您使用AUTO-AI辅助系统");              
            ImGui::Separator();
            ImGui::Text(u8"本机连接服务器状态");
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.0f, 0.5f), u8"连接数据中心状态:%s",& 联网状态);
            if (数据库判断)
            {
                char 数据库状态[50] = u8"已成功连接AI数据库";
                ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.0f, 0.5f), u8"连接数据库状态:%s", &数据库状态);
            }
            else
            {
                char 数据库状态[50] = u8"连接AI数据库失败,请稍后重试!";
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 0.5f), u8"连接数据库状态:%s", &数据库状态);
            }       
            ImGui::Separator();
            ImGui::Text(u8"用户状态");
            ImGui::Text(u8"User-ID:%s",user_id);
            ImGui::Text(u8"训练英雄:%s",&hore);
            ImGui::Text(u8"上传场次:%d", user_upload);
            ImGui::Separator();
            ImGui::Text(u8"设备信息");
            ImGui::Text(u8"CPU信息:%s",&cpu_information);
            ImGui::Text(u8"检测到CUDA设备数量:%d", cuda_quantity);
            ImGui::Text(u8"GPU-0:%s", &GPU_0_information);
            ImGui::Text(u8"GPU-1:%s", &GPU_1_information);
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.0f, 0.5f), u8"系统评估:%s", &system_test);
            ImGui::Separator();
            static float arr[] = { 263.0f, 691.0f, 700.0f, 361.0f,381.0f, 264.0f, 631.0f };
            char text_look[50] = u8"每个月数据中心收录的训练数量";
            ImGui::PlotHistogram(u8"月收录训练数据统计", arr, IM_ARRAYSIZE(arr), 0, text_look, 0.0f, 1500.0f, ImVec2(0, 80.0f));




            if (ImGui::CollapsingHeader(u8"主要功能菜单"))
            {
                ImGui::Checkbox(u8"快速开始训练", &auto_ai_train);      // Edit bools storing our window open/close state
                ImGui::Checkbox(u8"训练数据上传", &file_upload_windows);
                ImGui::Checkbox(u8"AUTO-AI系统设置", &auto_ai_settings);

            }
            if (ImGui::CollapsingHeader(u8"向开发团队报告问题"))
            {
                ImGui::Text(u8"报告BUG");
                ImGui::SameLine(); HelpMarker(
                    u8"您在此反馈的BUG将直接发送给开发团队，开发团队收到反馈后\n"
                    u8"会开始进行修复。受难度等不确定因素影响，无法给定具体修复时间");
                const char* items[] = { u8"训练数据上传问题", u8"训练模型下载问题", u8"AI环境一键配置问题", u8"AUTO-AI辅助程序问题", u8"开发文档问题", u8"与服务器等设备连接问题", u8"帮助我们变的更好" };
                static int item_current = 1;
                ImGui::ListBox(u8"选择您要报告的问题", &item_current, items, IM_ARRAYSIZE(items), 4);

            }

            ImGui::End();
        }

       
        if (file_upload_windows)
        {
            ImGui::Begin(u8"训练数据上传", &file_upload_windows);   
            ImGui::Text(u8"文件上传路径:%s",&input_text_path);
            //ImGui::Text(u8"F:\\python项目库\\WZCQ-main\\未用");
            ImGui::InputTextWithHint(u8"文件路径(训练数据)", u8"如果需要修改路径就请输入", input_text_path, IM_ARRAYSIZE(input_text_path));
            if (ImGui::Button(u8"删除上传路径.."))
                ImGui::OpenPopup(u8"您要删除路径?");

         
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal(u8"您要删除路径?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text(u8"完成删除后.\n您需要重新配置上传文件夹路径\n\n");
                ImGui::Separator();

                //static int unused_i = 0;
                //ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

                static bool dont_ask_me_next_time = false;
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                ImGui::Checkbox(u8"下次不再提醒", &dont_ask_me_next_time);
                ImGui::PopStyleVar();

                if (ImGui::Button(u8"确认", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button(u8"关闭", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }
            if (ImGui::Button(u8"关闭"))
                show_another_window = false;
            ImGui::End();
        }

    

        if (auto_ai_settings)
        {
            ImGui::Begin(u8"AUTO-AI设置",&auto_ai_settings);

            if (ImGui::CollapsingHeader(u8"文件设置"))
            {
                if (ImGui::TreeNode(u8"上传文件设置"))
                {
                    ImGui::Text(u8"文件上传路径:%s\n", &input_text_path);
                    ImGui::InputTextWithHint(u8"文件路径(训练数据)\n", u8"如果需要修改路径就请输入\n", input_text_path, IM_ARRAYSIZE(input_text_path));
                    ImGui::SameLine(); HelpMarker(u8"这里暂时需要您手动填入训练数据文件保存路径\n");
                    ImGui::Separator();
                    ImGui::Text(u8"等待开发:");
                    ImGui::Separator();

                    ImGui::Text("END:");

                    ImGui::TreePop();
                }
                if (ImGui::TreeNode(u8"下载模型设置"))
                {

                    ImGui::TreePop();
                }
                if (ImGui::TreeNode(u8"AI环境配置"))
                {
                    ImGui::Text(u8"文件上传路径:%s\n", &environment_path);
                    ImGui::InputTextWithHint(u8"AI环境安装路径\n", u8"请输入您要安装环境的路径\n", environment_path, IM_ARRAYSIZE(environment_path));
                    ImGui::TreePop();
                }
            }

            if (ImGui::CollapsingHeader(u8"功能设置"))
            {
                if (ImGui::BeginTable("sprots", 3))
                {
                    ImGui::TableNextColumn(); ImGui::Checkbox(u8"AI环境一键配置", &environment_config);
                    ImGui::SameLine(); HelpMarker(
                        u8"针对小白等人群设置的一键部署王者AI所使用的环境\n"
                        u8"全环境包括ANACONDA、PYCHARM、PYTHON、MINITOUCH、ADB等等全套下载、安装、配置\n"
                        u8"当程序完成对环境部署后，您将可以直接进行操作\n");
                        


                    ImGui::EndTable();
                }
            }

            /*AI环境配置窗口*/
            if (environment_config)
            {
                ImGui::Begin(u8"AI环境配置", &environment_config);
                ImGui::Text(u8"配置文件安装路径:%s\n", &environment_path);
                /*使用折叠复选框*/

                /*作出请求*/
                ImGui::End();
            }




     

            int clicked = 0;

            if (ImGui::CollapsingHeader(u8"窗口基本设置"))
            {
                if (ImGui::BeginTable("split", 3))
                {                  
                    ImGui::TableNextColumn(); ImGui::Checkbox(u8"隐藏标题栏\n", &no_titlebar);
                    ImGui::TableNextColumn(); ImGui::Checkbox(u8"隐藏滚动条\n", &no_scrollbar);
                    ImGui::TableNextColumn(); ImGui::Checkbox(u8"隐藏菜单\n", &no_menu);
                    ImGui::TableNextColumn(); ImGui::Checkbox(u8"隐藏关闭按键\n", &no_close);
                    ImGui::TableNextColumn(); ImGui::Checkbox(u8"隐藏背景\n", &no_background);
                    ImGui::TableNextColumn(); ImGui::Checkbox(u8"禁止调整大小\n", &no_resize);
                    ImGui::TableNextColumn(); ImGui::Checkbox(u8"防止异常退出\n", &no_collapse);
                    ImGui::TableNextColumn(); ImGui::Checkbox(u8"未保存的文档", &unsaved_document);
                    ImGui::Separator();
                    ImGui::Text(u8"设置背景颜色");               
                    ImGui::ColorEdit3(u8"背景颜色", (float*)&clear_color); // Edit 3 floats representing a color
                    ImGui::EndTable();
                }
            }
            if (ImGui::ShowStyleSelector(u8"窗口颜色##Selector"))
                // ref_saved_style = style;
              
            if (ImGui::Button(u8"保存设置"))
                clicked++;
            if (clicked & 1)
            {
                ImGui::SameLine();
                ImGui::Text(u8"保存设置成功!");
            }

            if (ImGui::Button(u8"关闭设置"))
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
