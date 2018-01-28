#include <SDL2/SDL.h>

void render_init();
void render_frame(int mouse_dx, int mouse_dy);

int g_windowWidth = 800;
int g_windowHeight = 600;

int main(int argc, const char * argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        printf("failed to init SDL\n");
        return 1;
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
     
    SDL_Window* window = NULL;
    SDL_GLContext* context = NULL;
    
    window = SDL_CreateWindow("Spherical Harmonics",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              g_windowWidth,
                              g_windowHeight,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    
    /* Create our opengl context and attach it to our window */
    context = SDL_GL_CreateContext(window);
    
    if (!window || !context)
    {
        printf("failed to create window and context\n");
        return 1;
    }
    
    SDL_GL_SetSwapInterval(1);   
    glViewport(0, 0, g_windowWidth, g_windowHeight);

    render_init(); 
     
    int quit = 0;
    int dragging = 0;

    int previous_mouse_x;
    int previous_mouse_y;
    
    while (!quit)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    dragging = 1;
                    previous_mouse_x = x;
                    previous_mouse_y = y;
                    break;
                case SDL_MOUSEBUTTONUP:
                    dragging = 0;
                    break;
           }
        }

        int dx = 0;
        int dy = 0;

        if (dragging)
        {
            dx = x - previous_mouse_x;
            dy = y - previous_mouse_y;

            previous_mouse_x = x;
            previous_mouse_y = y;
        }

        render_frame(dx, dy);
        SDL_GL_SwapWindow(window); 
    }
    
    SDL_GL_DeleteContext(context);
    
    return 0;
}


