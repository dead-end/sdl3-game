#define SDL_MAIN_USE_CALLBACKS 1 // Aktiviert das neue SDL3-Callback-System
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// Struktur für deinen Spielzustand (verhindert globale Variablen)
typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
} AppState;

// 1. Initialisierung (Wird einmalig beim Start aufgerufen)
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    // Speicher für den Zustand reservieren
    AppState *state = SDL_calloc(1, sizeof(AppState));
    if (!state)
    {
        return SDL_APP_FAILURE;
    }
    *appstate = state;

    // SDL Video initialisieren
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL konnte nicht initialisiert werden: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Fenster und Renderer erstellen
    if (!SDL_CreateWindowAndRenderer("SDL3 WASM Game", 800, 600, 0, &state->window, &state->renderer))
    {
        SDL_Log("Fenster/Renderer Fehler: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Log("Started!");

    return SDL_APP_CONTINUE; // Signalisiert erfolgreichen Start
}

// 2. Event-Handling (Wird für jedes anstehende Event aufgerufen)
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS; // Beendet die App sauber
    }
    return SDL_APP_CONTINUE; // App läuft weiter
}

// 3. Game-Loop / Rendering (Wird einmal pro Frame aufgerufen)
SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *state = (AppState *)appstate;

    // Hintergrundfarbe setzen (z. B. Dunkelblau)
    SDL_SetRenderDrawColor(state->renderer, 120, 40, 80, 255);
    SDL_RenderClear(state->renderer);

    // Hier kommt dein Zeichencode hin (z. B. SDL_RenderTexture)

    // Auf den Bildschirm bringen
    SDL_RenderPresent(state->renderer);

    return SDL_APP_CONTINUE; // Weitermachen zum nächsten Frame
}

// 4. Cleanup (Wird beim Beenden aufgerufen)
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    if (appstate)
    {
        AppState *state = (AppState *)appstate;
        SDL_DestroyRenderer(state->renderer);
        SDL_DestroyWindow(state->window);
        SDL_free(state);
    }
    SDL_Quit();
    SDL_Log("App erfolgreich beendet.");
}
