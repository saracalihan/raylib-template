#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <dlfcn.h>

#include "plugin.h"
#include "hotreload.c"

#define LOAD_SYM(name) \
name = dlsym(plugin_handler, #name); \
    if (!name) { \
        fprintf(stderr, "%s\n", dlerror()); \
        exit(EXIT_FAILURE); \
    } \
    TraceLog(LOG_INFO, "[PLUGIN RELOAD] "#name" function loaded");\

#define PLUGIN_PATH "./build/libplugin.so"

void* plugin_handler = NULL;
void* (*plugin_init)(void) = NULL;
void* (*plugin_preload)(void) = NULL;
void (*plugin_postload)(void*) = NULL;
void (*plugin_task)() = NULL;
void* plugin_data = NULL;

void load_plugin(){
    plugin_init     = NULL;
    plugin_postload = NULL;
    plugin_task     = NULL;
    plugin_preload  = NULL;

    if(plugin_handler != NULL){
        dlclose(plugin_handler);
    }
    plugin_handler = dlopen(PLUGIN_PATH, RTLD_NOW);
     if (!plugin_handler) {
        fprintf(stderr, "[PLUGIN RELOAD] %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    TraceLog(LOG_INFO, "[PLUGIN RELOAD] ------------------");

    LOAD_SYM(plugin_init)
    LOAD_SYM(plugin_preload)
    LOAD_SYM(plugin_postload)
    LOAD_SYM(plugin_task)
    TraceLog(LOG_INFO, "[PLUGIN RELOAD] Plugin functions loaded");
    TraceLog(LOG_INFO, "[PLUGIN RELOAD] ------------------");
}



int main(){
    #ifdef HOTRELOAD_ONCHANGE
    hotreload_onchange();
    #endif

    InitWindow(800, 400, "Raylib template | Alihan SARAÇ");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    load_plugin();
    plugin_data = plugin_init();

    while (!WindowShouldClose())
    {
        if(IsKeyPressed(KEY_R)){
            TraceLog(LOG_INFO, "Reload");
            void* data = plugin_preload();
            plugin_data = data;
            load_plugin();
            plugin_postload(data);
        }
        #ifdef HOTRELOAD_WITH_KEYPRESS
        if(IsKeyPressed(KEY_H)){
            printf("[HOTRELOAD] ----------------------\n");
            printf("[HOTRELOAD] Reloading...\n");
            hotreload_command();
            printf("[HOTRELOAD] ----------------------\n");
        }
        #endif
        plugin_task();
    }

    CloseWindow();
    return 0;
}