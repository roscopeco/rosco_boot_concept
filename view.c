#include <stdio.h>
#include <string.h>

#include "view.h"
#include "model.h"

#include "backend.h"

#include "text.h"
#include "main_font.h"
#include "num_font.h"
#include "debug.h"

#define MAX_BOX_HEIGHT  ((VIEW_VRES-(MIN_PADDING*2)))
#define CHAR_BUF_SIZE   16

static Model current;

static char secs_buf[2];
static char cpu_buffer[CHAR_BUF_SIZE];
static char mem_buffer[CHAR_BUF_SIZE];
static uint8_t cpu_buffer_len;

static void view_repaint_force(View *view, bool force);
void view_init(View *view, Model *model) {
    view->model = model;

    view_recompute_size(view, model);

    snprintf(mem_buffer, CHAR_BUF_SIZE, "%dMB RAM", model->mem_count / 1048576);
    snprintf(cpu_buffer, CHAR_BUF_SIZE, "MC%d @ %dMHz", model->cpu, model->mhz);

    cpu_buffer_len = strnlen(cpu_buffer, 16);
}

void view_recompute_size(View *view, Model *model) {
    int main_box_height = LINE_HEIGHT * (model->n_items + 1);

    if (main_box_height > MAX_BOX_HEIGHT) {
        model->n_items = MAX_BOX_HEIGHT / LINE_HEIGHT - 1;
        main_box_height = LINE_HEIGHT * (model->n_items + 1);
        debugf("WARN: Adjusted n_items to %d for height\n", model->n_items);
    }

    view->main_box.x = VIEW_HRES / 2 - BOX_WIDTH / 2;
    view->main_box.y = VIEW_VRES / 2 - main_box_height / 2;
    view->main_box.w = BOX_WIDTH;
    view->main_box.h = main_box_height;

    view->main_box_header.x = view->main_box.x;
    view->main_box_header.y = view->main_box.y;
    view->main_box_header.w = view->main_box.w;
    view->main_box_header.h = LINE_HEIGHT;

    view->right_shadow.x = view->main_box.x + view->main_box.w;
    view->right_shadow.y = view->main_box.y + SHADOW_OFFSET;
    view->right_shadow.w = SHADOW_OFFSET;
    view->right_shadow.h = view->main_box.h;

    view->bottom_shadow.x = view->main_box.x + SHADOW_OFFSET;
    view->bottom_shadow.y = view->main_box.y + view->main_box.h;
    view->bottom_shadow.w = view->main_box.w;
    view->bottom_shadow.h = SHADOW_OFFSET;
}

#ifdef __ROSCO_M68K
#define debug_model_update(...)
#else
void debug_model_update(Model *model) {
    debugf("Model updated:\n");
    debugf("    selection: 0x%02x\n", model->selection);
    debugf("    secs_left: 0x%02x\n", model->timer_secs_left);
    debugf("    items_ptr: %p\n", model->items);
    debugf("    n_items  : %d\n", model->n_items);
    debugf("\n");
}
#endif

void view_repaint(View *view, bool force) {
    if (force || model_is_dirty(&current, view->model)) {
        debug_model_update(view->model);

        backend_set_color(0x2f, 0x3c, 0x48, 0xff);
        backend_clear();

        // Main box
        backend_set_color(220, 220, 220, 0xff);
        backend_fill_rect(&view->main_box);

        backend_set_color(0, 0, 0, 0xff);
        backend_draw_rect(&view->main_box);
        backend_fill_rect(&view->main_box_header);

        // Shadow
        backend_set_color(0x1f, 0x2c, 0x38, 0xff);
        backend_fill_rect(&view->right_shadow);
        backend_fill_rect(&view->bottom_shadow);

        // Header text
        backend_set_color(220, 220, 220, 0xff);
        text_write("rosco_m68k\xae", view->main_box_header.x + 4, view->main_box_header.y + 2, FONT, FONT_HEIGHT);

        backend_set_color(220, 220, 0, 0xff);
        text_write(VERSION, view->main_box_header.x + view->main_box_header.w - 4 - (strlen(VERSION) * 8), view->main_box_header.y + 2, FONT, FONT_HEIGHT);

        // Selection bar
        Rect selection_rect;
        selection_rect.x = view->main_box.x + 1;
        selection_rect.y = view->main_box.y + view->main_box_header.h + (LINE_HEIGHT * current.selection);
        selection_rect.w = view->main_box.w - 2;
        selection_rect.h = LINE_HEIGHT - 1;  // -1 so as not to overdraw border on last item!
        backend_set_color(0x10, 0x80, 0xa0, 0xff);
        backend_fill_rect(&selection_rect);

        // Items text
        backend_set_color(0x10, 0x10, 0x10, 0xff);
        int y = view->main_box_header.y + view->main_box_header.h + 2;

        for (int i = 0; i < view->model->n_items; i++) {
#           ifdef CENTER_ITEMS
            int x = view->main_box.x + (view->main_box.w / 2) - (strlen(view->model->items[i]) * 8 / 2);
            #else
            int x = main_box_header.x + 4;
#           endif

#           ifdef HIGHLIGHT_SELECTION
            if (i == current.selection) {
                backend_set_color(0xe6, 0xe6, 0xe6, 0xff);
            }
#           endif

            text_write(view->model->items[i], x, y, FONT, FONT_HEIGHT);

#           ifdef HIGHLIGHT_SELECTION
            if (i == current.selection) {
                backend_set_color(0x10, 0x10, 0x10, 0xff);
            }
#           endif

            y += 20;
        }

        // ticks remaining
        if (view->model->timer_secs_left) {
            secs_buf[0] = view->model->timer_secs_left;
            backend_set_color(220, 220, 0, 0xff);
            text_write(secs_buf, selection_rect.x + selection_rect.w - 12, selection_rect.y + 2, NUM_FONT, NUM_FONT_HEIGHT);
        }

        // Sysinfo header
        backend_set_color(0x1f, 0x2c, 0x38, 0xff);
        text_write(mem_buffer, 6, 6, FONT, 16);
        text_write(cpu_buffer, VIEW_HRES - (cpu_buffer_len * 8) - 6, 6, FONT, FONT_HEIGHT);

        // Copyright footer
        text_write("\xaf 2024 The rosco_m68k\xae Open Source Project", 296, 460, FONT, FONT_HEIGHT);

        backend_present();
    }
}