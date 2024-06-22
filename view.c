/*
 *------------------------------------------------------------
 *                                  ___ ___ _
 *  ___ ___ ___ ___ ___       _____|  _| . | |_
 * |  _| . |_ -|  _| . |     |     | . | . | '_|
 * |_| |___|___|___|___|_____|_|_|_|___|___|_,_|
 *                     |_____|          firmware
 * ------------------------------------------------------------
 * Copyright (c)2024 The rosco_m68k Open Source Project
 * See top-level LICENSE.md for licence information.
 *
 * View for concept boot screen
 * ------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>

#include "view.h"
#include "model.h"

#include "backend.h"

#include "debug.h"

#define MAX_BOX_HEIGHT  ((VIEW_VRES-(MIN_PADDING*2)))
#define CHAR_BUF_SIZE   16

static Model current;

static char secs_buf[2];
static char cpu_buffer[CHAR_BUF_SIZE];
static char mem_buffer[CHAR_BUF_SIZE];
static uint8_t cpu_buffer_len;
static uint8_t copyright_len;

void view_init(View *view, Model *model) {
    view->model = model;

    view_recompute_size(view, model);

    snprintf(mem_buffer, CHAR_BUF_SIZE, "%dMB RAM", model->mem_count / 1048576);
    snprintf(cpu_buffer, CHAR_BUF_SIZE, "MC%d @ %dMHz", model->cpu, model->mhz);

    cpu_buffer_len = strnlen(cpu_buffer, CHAR_BUF_SIZE);
    copyright_len = strnlen(COPYRIGHT, COPYRIGHT_MAX);
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

#ifdef __ROSCO_M68K_ROM__
#define debug_model_update(...)
#else
#ifdef MODEL_DEBUG
void debug_model_update(Model *model) {
    debugf("Model updated:\n");
    debugf("    selection: 0x%02x\n", model->selection);
    debugf("    secs_left: 0x%02x\n", model->timer_secs_left);
    debugf("    items_ptr: %p\n", model->items);
    debugf("    n_items  : %d\n", model->n_items);

#   ifdef ENABLE_ANIM
    debugf("  anim_dirty : %d\n", model->anim_list_dirty);
    debugf("  anim_front : %p\n", model->animations_front.next);
    debugf("   anim_back : %p\n", model->animations_back.next);
#   endif

    debugf("\n");
}
#else
#define debug_model_update(...)
#endif
#endif

#ifdef ENABLE_ANIM
static void paint_anim_layer(Animation *current_anim) {
    while (current_anim) {
        // Don't check dirty, always paint anims when painting.
        // **do** set dirty false though, in case the anim was what triggered this repaint...
        current_anim->paint(current_anim);
        current_anim->dirty = false;
        current_anim = (Animation*)current_anim->node.next;
    }
}
#else
#define paint_anim_layer(...)
#endif

void view_repaint(View *view, bool force) {
    if (force || model_is_dirty(&current, view->model)) {        
        debug_model_update(view->model);

        backend_set_color(COLOR_BACKGROUND);
        backend_clear();

        // Sysinfo header
        backend_set_color(COLOR_BACKGROUND_SHADOW);
        backend_text_write(mem_buffer, 6, 6, FONT, FONT_WIDTH, FONT_HEIGHT);
        backend_text_write(cpu_buffer, VIEW_HRES - (cpu_buffer_len * FONT_WIDTH) - 6, 6, FONT, FONT_WIDTH, FONT_HEIGHT);

        // Copyright footer
        backend_text_write(COPYRIGHT, VIEW_HRES - (copyright_len * FONT_WIDTH) - 6, VIEW_VRES - LINE_HEIGHT, FONT, FONT_WIDTH, FONT_HEIGHT);

        // Animations (back)
        paint_anim_layer((Animation*)view->model->animations_back.next);

        // Main box
        backend_set_color(COLOR_WINDOW_BACKGROUND);
        backend_fill_rect(&view->main_box);

        backend_set_color(COLOR_BLACK);
        backend_draw_rect(&view->main_box);
        backend_fill_rect(&view->main_box_header);

        // Shadow
        backend_set_color(COLOR_BACKGROUND_SHADOW);
        backend_fill_rect(&view->right_shadow);
        backend_fill_rect(&view->bottom_shadow);

        // Header text
        backend_set_color(COLOR_WHITE);
        backend_text_write(BOX_TITLE, view->main_box_header.x + 4, view->main_box_header.y + 2, FONT, FONT_WIDTH, FONT_HEIGHT);

        backend_set_color(COLOR_YELLOW);
        backend_text_write(VERSION, view->main_box_header.x + view->main_box_header.w - 4 - (strlen(VERSION) * FONT_WIDTH), view->main_box_header.y + 2, FONT, FONT_WIDTH, FONT_HEIGHT);

        // Selection bar
        Rect selection_rect;
        selection_rect.x = view->main_box.x + 1;
        selection_rect.y = view->main_box.y + view->main_box_header.h + (LINE_HEIGHT * current.selection);
        selection_rect.w = view->main_box.w - 2;
        selection_rect.h = LINE_HEIGHT - 1;  // -1 so as not to overdraw border on last item!
        backend_set_color(COLOR_SELECTION_BAR);
#ifdef BLIT_DEBUG
        printf("SHADOW (B)   ::::::: ");
#endif   
        backend_fill_rect(&selection_rect);

        // Items text
        backend_set_color(COLOR_ITEM_TEXT);
        int y = view->main_box_header.y + view->main_box_header.h + LINE_PAD;

        for (int i = 0; i < view->model->n_items; i++) {
#           ifdef CENTER_ITEMS
            int x = view->main_box.x + (view->main_box.w / 2) - (strlen(view->model->items[i]) * FONT_WIDTH / 2);
            #else
            int x = view->main_box_header.x + 4;
#           endif

#           ifdef HIGHLIGHT_SELECTION
            if (i == current.selection) {
                backend_set_color(COLOR_ITEM_HIGHLIGHT_TEXT);
            }
#           endif

            backend_text_write(view->model->items[i], x, y, FONT, FONT_WIDTH, FONT_HEIGHT);

#           ifdef HIGHLIGHT_SELECTION
            if (i == current.selection) {
                backend_set_color(COLOR_ITEM_TEXT);
            }
#           endif

            y += LINE_HEIGHT;
        }

        // ticks remaining
        if (view->model->timer_secs_left) {
            secs_buf[0] = view->model->timer_secs_left;
            backend_set_color(COLOR_YELLOW);
            backend_text_write(secs_buf, selection_rect.x + selection_rect.w - 12, selection_rect.y + 2, NUM_FONT, NUM_FONT_WIDTH, NUM_FONT_HEIGHT);
        }

        // Animations (front)
        paint_anim_layer((Animation*)view->model->animations_front.next);

        backend_present();
    }
}