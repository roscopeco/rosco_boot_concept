#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include "model.h"
#include "view.h"
#include "controller.h"

#include "backend.h"

static int n_menu_items = 5;
static char* menu_items[] = {
    "SD card loader & shell",
    "Kermit loader",
    "ROM BASIC",
    "System configuration",
    "Memory check & diagnostic",
};

static Model model;
static View view;

int main() {
    model.n_items = n_menu_items;
    model.items = menu_items;
    model.mem_count = 14680064;
    model.cpu = 68010;
    model.mhz = 10;
    model.timer_secs_left = 5;

    if (!backend_init()) {
        printf("Backend init failed\n");
        return 2;
    }

    view_init(&view, &model);

    while (control(&model)) {
        view_repaint(&view, false);
    }

    backend_done();
}

