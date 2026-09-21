#include <SDL3/SDL.h>

#include "screen_manager.h"
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
        switch (event->key.key)
        {
        case SDLK_ESCAPE:
            return SDL_APP_SUCCESS;
            break;
        case SDLK_SPACE:
            sm_change_screen(SCREEN_GAME);
            break;

        default:
            break;
        }
    }
    return SDL_APP_CONTINUE;
}

static SDL_AppResult _update(void *state, double delta_time)
{
    SDL_Log("start screen update");
    State *m_state = (State *)state;

    m_state->r += m_state->direction * delta_time * 100;

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
    SDL_Log("start screen render");
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

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    if (!SDL_RenderDebugText(renderer, 10.0, 10.0, "Press space to continue ..."))
    {
        SDL_Log("Unable to write text");
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

static void _cleanup(void *state)
{
    SDL_Log("start screen cleanup");
    SDL_free(state);
}

// Factory-Funktion zum Erstellen des Screens
Screen ScreenStart_Create(void)
{
    SDL_Log("start screen created");

    Screen s = {0};
    s.state = SDL_calloc(1, sizeof(State));
    s.init = _init;
    s.event = _event;
    s.update = _update;
    s.render = _render;
    s.cleanup = _cleanup;
    return s;
}
