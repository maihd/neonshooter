#pragma once

namespace Mojo
{
    bool ImGuiImpl_Setup(void);
    void ImGuiImpl_Shutdown(void);

    void ImGuiImpl_NewFrame(void);
    void ImGuiImpl_EndFrame(void);
    bool ImGuiImpl_ProcessEvent(void);
}
