/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "SDL_config.h"

#if SDL_VIDEO_DRIVER_UIKIT

#include "SDL_syswm.h"
#include "SDL_video.h"
#include "SDL_mouse.h"
#include "SDL_assert.h"
#include "SDL_hints.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include "SDL_uikitvideo.h"
#include "SDL_uikitevents.h"
#include "SDL_uikitmodes.h"
#include "SDL_uikitwindow.h"
#import "SDL_uikitappdelegate.h"

#import "SDL_uikitopenglview.h"

#include <Foundation/Foundation.h>




static int SetupWindowData(_THIS, SDL_Window *window, UIWindow *uiwindow, SDL_bool created)
{
    SDL_VideoDisplay *display = SDL_GetDisplayForWindow(window);
    SDL_DisplayModeData *displaymodedata = (SDL_DisplayModeData *) display->current_mode.driverdata;
    SDL_DisplayData *displaydata = (SDL_DisplayData *) display->driverdata;

    /* Fill in the SDL window with the window data */
    {
        window->x = 0;
        window->y = 0;

        CGRect bounds;
        if (window->flags & (SDL_WINDOW_FULLSCREEN|SDL_WINDOW_BORDERLESS)) {
            bounds = [displaydata->uiscreen bounds];
        } else {
            bounds = [displaydata->uiscreen applicationFrame];
        }

        /* Get frame dimensions in pixels */
        int width = (int)(bounds.size.width * displaymodedata->scale);
        int height = (int)(bounds.size.height * displaymodedata->scale);

        // Make sure the width/height are oriented correctly
        if (UIKit_IsDisplayLandscape(displaydata->uiscreen) != (width > height)) {
            int temp = width;
            width = height;
            height = temp;
        }

        window->w = width;
        window->h = height;
    }

    /* only one window on iOS, always shown */
    window->flags &= ~SDL_WINDOW_HIDDEN;

    // SDL_WINDOW_BORDERLESS controls whether status bar is hidden.
    // This is only set if the window is on the main screen. Other screens
    //  just force the window to have the borderless flag.
    if (displaydata->uiscreen == [UIScreen mainScreen]) {
        window->flags |= SDL_WINDOW_INPUT_FOCUS;  // always has input focus
        
        if ([UIApplication sharedApplication].statusBarHidden) {
            window->flags |= SDL_WINDOW_BORDERLESS;
        } else {
            window->flags &= ~SDL_WINDOW_BORDERLESS;
        }
    } else {
        window->flags &= ~SDL_WINDOW_RESIZABLE;  // window is NEVER resizeable
        window->flags &= ~SDL_WINDOW_INPUT_FOCUS;  // never has input focus
        window->flags |= SDL_WINDOW_BORDERLESS;  // never has a status bar.
    }

    ((SDLUIKitDelegate*)[SDLUIKitDelegate sharedAppDelegate]).viewController.window = window;
    return 0;
}

int
UIKit_CreateWindow(_THIS, SDL_Window *window)
{
    SDL_VideoDisplay *display = SDL_GetDisplayForWindow(window);
    SDL_DisplayData *data = (SDL_DisplayData *) display->driverdata;
    const BOOL external = ([UIScreen mainScreen] != data->uiscreen);

    // SDL currently puts this window at the start of display's linked list. We rely on this.
    SDL_assert(_this->windows == window);

    /* We currently only handle a single window per display on iOS */
    if (window->next != NULL) {
        SDL_SetError("Only one window allowed per display.");
        return -1;
    }

    // If monitor has a resolution of 0x0 (hasn't been explicitly set by the
    //  user, so it's in standby), try to force the display to a resolution
    //  that most closely matches the desired window size.
    if (SDL_UIKit_supports_multiple_displays) {
        const CGSize origsize = [[data->uiscreen currentMode] size];
        if ((origsize.width == 0.0f) && (origsize.height == 0.0f)) {
            if (display->num_display_modes == 0) {
                _this->GetDisplayModes(_this, display);
            }

            int i;
            const SDL_DisplayMode *bestmode = NULL;
            for (i = display->num_display_modes; i >= 0; i--) {
                const SDL_DisplayMode *mode = &display->display_modes[i];
                if ((mode->w >= window->w) && (mode->h >= window->h))
                    bestmode = mode;
            }

            if (bestmode) {
                SDL_DisplayModeData *modedata = (SDL_DisplayModeData *)bestmode->driverdata;
                [data->uiscreen setCurrentMode:modedata->uiscreenmode];

                // desktop_mode doesn't change here (the higher level will
                //  use it to set all the screens back to their defaults
                //  upon window destruction, SDL_Quit(), etc.
                display->current_mode = *bestmode;
            }
        }
    }
    
    [UIApplication sharedApplication].statusBarHidden = YES;
    
    if (!(window->flags & SDL_WINDOW_RESIZABLE)) {
        if (window->w > window->h) {
            if (!UIKit_IsDisplayLandscape(data->uiscreen)) {
                [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeRight animated:NO];
            }
        } else if (window->w < window->h) {
            if (UIKit_IsDisplayLandscape(data->uiscreen)) {
                [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait animated:NO];
            }
        }
    }

    UIWindow *uiwindow = [SDLUIKitDelegate getGameWindow];

    if (SetupWindowData(_this, window, uiwindow, SDL_TRUE) < 0) {
        [uiwindow release];
        return -1;
    }

    return 1;

}

void
UIKit_ShowWindow(_THIS, SDL_Window * window)
{
}

void
UIKit_HideWindow(_THIS, SDL_Window * window)
{
}

void
UIKit_RaiseWindow(_THIS, SDL_Window * window)
{
    // We don't currently offer a concept of "raising" the SDL window, since
    //  we only allow one per display, in the iOS fashion.
    // However, we use this entry point to rebind the context to the view
    //  during OnWindowRestored processing.
    _this->GL_MakeCurrent(_this, _this->current_glwin, _this->current_glctx);
}

void
UIKit_SetWindowFullscreen(_THIS, SDL_Window * window, SDL_VideoDisplay * display, SDL_bool fullscreen)
{
}

void
UIKit_DestroyWindow(_THIS, SDL_Window * window)
{
}

SDL_bool
UIKit_GetWindowWMInfo(_THIS, SDL_Window * window, SDL_SysWMinfo * info)
{
    UIWindow *uiwindow = [SDLUIKitDelegate getGameWindow];

    if (info->version.major <= SDL_MAJOR_VERSION) {
        info->subsystem = SDL_SYSWM_UIKIT;
        info->info.uikit.window = uiwindow;
        return SDL_TRUE;
    } else {
        SDL_SetError("Application not compiled with SDL %d.%d\n",
                     SDL_MAJOR_VERSION, SDL_MINOR_VERSION);
        return SDL_FALSE;
    }
}

int
SDL_iPhoneSetAnimationCallback(SDL_Window * window, int interval, void (*callback)(void*), void *callbackParam)
{
    [[SDLUIKitDelegate getGameView] setAnimationCallback:interval callback:callback callbackParam:callbackParam];
    return 0;
}

#endif /* SDL_VIDEO_DRIVER_UIKIT */

/* vi: set ts=4 sw=4 expandtab: */
