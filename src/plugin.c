#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "plugin.h"

static Plugin* p = NULL;

void flush_plugin();

void plugin_task(){
    if(IsKeyPressed(KEY_F)){
        flush_plugin();
    }
    BeginDrawing();
        ClearBackground(CLITERAL(Color){17, 18, 18, 255});

        // TODO: MAIN LOGICS
        DrawText("Hello World!", 10, 10, 24, LIGHTGRAY);
    EndDrawing();
}

void load_assets(){
    if(p == NULL){
        return TraceLog(LOG_ERROR, "[LOAD ASSET] Plugin is null!");
    }

    // TODO: LOAD ASSETS HERE

    TraceLog(LOG_INFO, "[LOAD ASSET] Assets loaded.");
}

void unload_assets(){
    if(p == NULL){
        return TraceLog(LOG_ERROR, "[LOAD ASSET] Plugin is null!");
    }

    // TODO: UNLOAD ASSETS HERE

    TraceLog(LOG_INFO, "[LOAD ASSET] Assets unloaded.");
}

void flush_plugin(){
    if(p == NULL){
        p = malloc(sizeof(Plugin));
        TraceLog(LOG_INFO, "Plugin data allocated");
    }
    assert(p != NULL && "Plugin malloc error!");

    // TODO: FREE ALL FIELDS BEFORE FLUSH

    memset(p, 0, sizeof(Plugin));

    TraceLog(LOG_INFO, "Plugin data flushed!");
}

Plugin* plugin_init(){
    TraceLog(LOG_INFO, "[PLUGIN] Plugin initialized!");
    flush_plugin();
    load_assets();
    return p;
}

Plugin* plugin_preload(){
    unload_assets();
    TraceLog(LOG_INFO, "[PLUGIN] Plugin preload run!");
    return p;
}

void plugin_postload(Plugin* plugin){
    TraceLog(LOG_INFO, "[PLUGIN] Plugin postload run!");
    p = plugin;
    load_assets();
}