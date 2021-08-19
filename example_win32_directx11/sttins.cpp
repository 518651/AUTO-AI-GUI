#include "imgui.h"


static bool no_titlebar = false;
static bool no_scrollbar = false;
static bool no_menu = false;
static bool no_move = false;
static bool no_resize = false;
static bool no_collapse = false;
static bool no_close = false;
static bool no_nav = false;
static bool no_background = false;
static bool no_bring_to_front = false;
static bool unsaved_document = false;



static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}



static char text[1024 * 16] =
"/*\n"
" ��������������������Ҫ����������,\n"
" ������ԵĻ��������������������Է������Ǻ�����������\n"
" more formally, the invalid operand with locked CMPXCHG8B\n"
" instruction bug, is a design flaw in the majority of\n"
" Intel Pentium, Pentium MMX, and Pentium OverDrive\n"
" processors (all in the P5 microarchitecture).\n"
"*/\n\n"
"label:\n"
"\tlock cmpxchg8b eax\n";












void email(char email_target, char message);
