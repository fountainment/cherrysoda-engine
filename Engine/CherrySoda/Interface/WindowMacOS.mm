#include <CherrySoda/Interface/Window.h>

#include <SDL3/SDL.h>

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>

namespace cherrysoda {

void Window::SetOpaqueNativeBackground(const Color& color)
{
	if (m_mainWindow == nullptr) {
		return;
	}
	NSWindow* nsWindow = (__bridge NSWindow*)SDL_GetPointerProperty(
		SDL_GetWindowProperties(m_mainWindow), SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
	CALayer* layer = nsWindow.contentView.layer;
	if (layer == nil || ![layer isKindOfClass:[CAMetalLayer class]]) {
		return;
	}
	// bgfx never configures the CAMetalLayer it creates; before its first present
	// the layer is transparent and the window's default white background shows.
	layer.backgroundColor = [NSColor colorWithDeviceRed:color.R() green:color.G() blue:color.B() alpha:1.0f].CGColor;
	layer.opaque = YES;
}

} // namespace cherrysoda
