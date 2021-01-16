#include <CherrySoda/Util/GUI.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Input/MInput.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Graphics/Texture.h>

#include <imgui.h>
#include <SDL.h>

#include <cstring>

using cherrysoda::GUI;

using cherrysoda::Effect;
using cherrysoda::Engine;
using cherrysoda::Graphics;
using cherrysoda::Keys;
using cherrysoda::Math;
using cherrysoda::MInput;
using cherrysoda::Texture2D;

namespace type = cherrysoda::type;

Effect GUI::ms_guiEffect;
Texture2D GUI::ms_fontTexture;
type::UInt16 GUI::ms_guiRenderPass = 0;
bool GUI::ms_disable = false;
bool GUI::ms_frameStarted = false;

const char* GetClipboardText_SDLImplForImGui(void*)
{
	return SDL_GetClipboardText();
}

void SetClipboardText_SDLImplForImGui(void*, const char* text)
{
	SDL_SetClipboardText(text);
}

void GUI::Initialize()
{
	ImGui::CreateContext();

	ms_guiRenderPass = Graphics::MaxRenderPassCount() - 1;

	// Setup back-end capabilities flags
	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
	io.BackendPlatformName = "imgui_impl_cherrysoda";

	// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
	io.KeyMap[ImGuiKey_Tab] = (int)Keys::Tab;
	io.KeyMap[ImGuiKey_LeftArrow] = (int)Keys::Left;
	io.KeyMap[ImGuiKey_RightArrow] = (int)Keys::Right;
	io.KeyMap[ImGuiKey_UpArrow] = (int)Keys::Up;
	io.KeyMap[ImGuiKey_DownArrow] = (int)Keys::Down;
	io.KeyMap[ImGuiKey_PageUp] = (int)Keys::PageUp;
	io.KeyMap[ImGuiKey_PageDown] = (int)Keys::PageDown;
	io.KeyMap[ImGuiKey_Home] = (int)Keys::Home;
	io.KeyMap[ImGuiKey_End] = (int)Keys::End;
	io.KeyMap[ImGuiKey_Insert] = (int)Keys::Insert;
	io.KeyMap[ImGuiKey_Delete] = (int)Keys::Delete;
	io.KeyMap[ImGuiKey_Backspace] = (int)Keys::Back;
	io.KeyMap[ImGuiKey_Space] = (int)Keys::Space;
	io.KeyMap[ImGuiKey_Enter] = (int)Keys::Enter;
	io.KeyMap[ImGuiKey_Escape] = (int)Keys::Escape;
	io.KeyMap[ImGuiKey_KeyPadEnter] = (int)Keys::Enter;
	io.KeyMap[ImGuiKey_A] = (int)Keys::A;
	io.KeyMap[ImGuiKey_C] = (int)Keys::C;
	io.KeyMap[ImGuiKey_V] = (int)Keys::V;
	io.KeyMap[ImGuiKey_X] = (int)Keys::X;
	io.KeyMap[ImGuiKey_Y] = (int)Keys::Y;
	io.KeyMap[ImGuiKey_Z] = (int)Keys::Z;

	// Font texture 
	BuildFontTexture();

	// Style
	// auto& style = ImGui::GetStyle();
	// style.WindowBorderSize = 2.0f;
	// style.FrameBorderSize = 2.0f;
	// style.ScaleAllSizes(2.0f);

	// Shader
	ms_guiEffect = Graphics::GetEmbeddedEffect("sprite");
}

void GUI::Terminate()
{
	Graphics::DestroyTexture(ms_fontTexture.GetHandle());
}

void GUI::Update()
{
	if (ms_disable) return;

	ImGuiIO& io = ImGui::GetIO();

	// Window
	auto winSize = Engine::Instance()->GetWindowSize();	
	io.DisplaySize.x = winSize.x;
	io.DisplaySize.y = winSize.y;

	// Keyboard
	auto keyboardKeys = MInput::GetCurrentKeyboardKeys();

	std::memset(io.KeysDown, 0, sizeof(io.KeysDown));
	for (auto key : keyboardKeys) {
		io.KeysDown[(int)key] = true;
	}

	io.KeyShift = MInput::Keyboard()->Check(Keys::LeftShift, Keys::RightShift);
	io.KeyCtrl = MInput::Keyboard()->Check(Keys::LeftControl, Keys::RightControl);
	io.KeyAlt = MInput::Keyboard()->Check(Keys::LeftAlt, Keys::RightAlt);
	io.KeySuper = false;

	// Mouse
	if (io.WantSetMousePos) {
		Engine::Instance()->SetMousePosition(Math::IVec2((int)io.MousePos.x, (int)io.MousePos.y));
	}
	else {
		io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
	}

	auto pos = MInput::Mouse()->RawPosition();
	io.MousePos = ImVec2((float)pos.x, (float)pos.y);	
	io.MouseDown[0] = MInput::Mouse()->CheckLeftButton();
	io.MouseDown[1] = MInput::Mouse()->CheckRightButton();
	io.MouseDown[2] = MInput::Mouse()->CheckMiddleButton();
	io.MouseWheel += MInput::Mouse()->WheelDelta() / 120;

	// TODO: Add mouse cursor control support

	// GamePad
	// TODO: Add GamePad support

	// Clipboard
	io.GetClipboardTextFn = GetClipboardText_SDLImplForImGui;
	io.SetClipboardTextFn = SetClipboardText_SDLImplForImGui;

	// Delta time
	io.DeltaTime = Engine::Instance()->RawDeltaTime();

	ImGui::NewFrame();

	ms_frameStarted = true;
}

void GUI::Render()
{
	if (ms_frameStarted) {
		ms_frameStarted = false;
	}
	else if (ms_disable) {
		return;
	}

	ImGui::Render();

	ImGuiIO& io = ImGui::GetIO();
	auto drawData = ImGui::GetDrawData();

	int fbWidth = (int)(drawData->DisplaySize.x * drawData->FramebufferScale.x);
	int fbHeight = (int)(drawData->DisplaySize.y * drawData->FramebufferScale.y);
	if (fbWidth <= 0 || fbHeight <= 0)
		return;

	ImVec2 clipOff = drawData->DisplayPos;
	ImVec2 clipScale = drawData->FramebufferScale;

	CHERRYSODA_ASSERT(sizeof(ImDrawIdx) == 2, "ImGui graphics index type is not type::UInt16\n");

	Graphics::BeginRenderPass(ms_guiRenderPass);
	Graphics::UseCurrentRenderPass()->SetViewport(0, 0, fbWidth, fbHeight);
	// TexelHalf is a hack for d3d9's different pixel coordinate
	// TODO: Consider moving this hack to camera
	Graphics::UseCurrentRenderPass()->SetViewProjectionMatrix(Math_Identity<Math::Mat4>(), Math_Ortho(Graphics::TexelHalf(), io.DisplaySize.x + Graphics::TexelHalf(), io.DisplaySize.y + Graphics::TexelHalf(), Graphics::TexelHalf(), -1000.f, 1000.f));
	for (int n = 0; n < drawData->CmdListsCount; ++n) {
		const ImDrawList* drawList = drawData->CmdLists[n];
		auto vb = Graphics::CreateTransientVertexBuffer(reinterpret_cast<Graphics::ImGuiVertex*>(drawList->VtxBuffer.Data), drawList->VtxBuffer.Size);
		auto ib = Graphics::CreateTransientIndexBuffer(drawList->IdxBuffer.Data, drawList->IdxBuffer.Size);
		for (const ImDrawCmd* cmd = drawList->CmdBuffer.begin(), *cmdEnd = drawList->CmdBuffer.end(); cmd != cmdEnd; ++cmd) {
			if (cmd->UserCallback)
			{
				cmd->UserCallback(drawList, cmd);
			}
			else if (0 != cmd->ElemCount)
			{
				ImVec4 clipRect;
				clipRect.x = (cmd->ClipRect.x - clipOff.x) * clipScale.x;
				clipRect.y = (cmd->ClipRect.y - clipOff.y) * clipScale.y;
				clipRect.z = (cmd->ClipRect.z - clipOff.x) * clipScale.x;
				clipRect.w = (cmd->ClipRect.w - clipOff.y) * clipScale.y;
				Graphics::SetScissor((int)clipRect.x, (int)(clipRect.y), (int)(clipRect.z - clipRect.x), (int)(clipRect.w - clipRect.y));
				Graphics::SetTexture((Texture*)cmd->TextureId);
				Graphics::SetTransientVertexBuffer(vb);
				Graphics::SetTransientIndexBuffer(ib, cmd->IdxOffset, cmd->ElemCount);
				Graphics::SetStateNoDepth(Graphics::BlendFunction::Alpha);
				Graphics::SubmitOnCurrentRenderPass(&ms_guiEffect);
			}
		}
	}
	Graphics::EndRenderPass(ms_guiRenderPass);
}

void GUI::BuildFontTexture()
{
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* data;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);
	ms_fontTexture.Dispose();
	ms_fontTexture = Texture2D::FromRGBA(data, width, height);
	io.Fonts->TexID = (ImTextureID)(&ms_fontTexture);
}

void GUI::TextInput(const char* text)
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddInputCharactersUTF8(text);
}

