#include "SDL_config.h"

#include "SDL.h"
#include "SDL_compat.h"

#import <UIKit/UIKit.h>

extern char g_application_dir_sdl[256];
extern char g_resource_dir_sdl[256];
void SDL_initDir()
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString  *plistPath = [paths objectAtIndex:0];
    strlcpy(g_resource_dir_sdl, [plistPath UTF8String], sizeof(g_resource_dir_sdl));
    
    g_resource_dir_sdl[strlen(g_resource_dir_sdl)] = '/';
    
    strlcpy(g_application_dir_sdl, [[[NSBundle mainBundle]bundlePath]UTF8String], sizeof(g_application_dir_sdl));
    g_application_dir_sdl[strlen(g_application_dir_sdl)] = '/';
}