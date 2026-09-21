#include <SDL3/SDL.h>

#include "screen.h"

typedef struct State
{
    int r;
    int direction;

} State;

static bool _init(void *state, SDL_Renderer *renderer)
{
    State *m_state = (State *)state;
    m_state->r = 100;
    m_state->direction = 1;
    return true;
}

static SDL_AppResult _event(void *state, SDL_Event *event)
{
    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        if (event->key.key == SDLK_ESCAPE)
        {
            // Wenn ENTER gedrückt wird: Wechsel zum Gameplay-Screen
            // (Hinweis: Dafür müsste der ScreenManager-Zeiger im State erreichbar sein)
            return SDL_APP_SUCCESS;
        }
    }
    return SDL_APP_CONTINUE;
}

static SDL_AppResult _update(void *state, double delta_time)
{
    State *m_state = (State *)state;

    m_state->r += m_state->direction * delta_time * 100;

    SDL_Log("direction: %d delta-time: %f rot: %d", m_state->direction, delta_time, m_state->r);

    if (m_state->r > 255)
    {
        m_state->r = 255;
        m_state->direction = -1;
    }

    if (m_state->r < 0)
    {
        m_state->r = 0;
        m_state->direction = 1;
    }

    return SDL_APP_CONTINUE;
}

static SDL_AppResult _render(void *state, SDL_Renderer *renderer)
{
    State *m_state = (State *)state;

    SDL_SetRenderDrawColor(renderer, m_state->r, 0, 0, 255);

    SDL_Log("color: %d", m_state->r);

    SDL_FRect my_rect = {
        .x = 0.0f,   // Startposition X
        .y = 0.0f,   // Startposition Y
        .w = 100.0f, // Breite
        .h = 100.0f  // Höhe
    };

    // 4. Rechteck auf den Bildschirm zeichnen
    // Nutzen Sie SDL_RenderFillRect für ein ausgefülltes Rechteck:
    SDL_RenderFillRect(renderer, &my_rect);

    return SDL_APP_CONTINUE;
}

static void _cleanup(void *state)
{
    SDL_free(state);
}

// Factory-Funktion zum Erstellen des Screens
Screen StartScreen_Create(void)
{

    SDL_Log("created start screen");

    Screen s = {0};
    s.state = SDL_calloc(1, sizeof(State));
    s.init = _init;
    s.event = _event;
    s.update = _update;
    s.render = _render;
    s.cleanup = _cleanup;
    return s;
}
