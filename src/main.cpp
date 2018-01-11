
#include <wchar.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <limits.h>

#include <SDL2/SDL.h>
#include "utils/observer.hpp"
#include "gfx/fbo.hpp"
#include "gfx/resource.hpp"
#include "gfx/pipeline.hpp"
#include "obj/camera.hpp"
#include "obj/object_base.hpp"

int engine_init();
int engine_handle_input();

extern SDL_Window *video_window;

int main() {
    //int argc, char *argv[]) {

    int n = engine_init();

    if (n != 0) {
        printf("ERROR %u\n", n);
        return 1;
    }

    fbo frame(320, 240, fbo::COLOR_AND_DEPTH);

    std::vector <object_base*> objects;
    objects.push_back(new object_base());

    while (engine_handle_input()) {

        pipeline pipe;
        glm::vec3 f(0, 0, -10), t(0, 0, 0), u(0, 1, 0);
        generic_camera cam(f, t, u, 320, 240);

        pipe << frame << cam;
        pipe.flush();

        SDL_GL_SwapWindow(video_window);
    }

    return 0;
}
