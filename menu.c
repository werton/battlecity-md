
#include <genesis.h>
#include "menu.h"
#include "map.h"
#include "resmap.h"
#include "sprite.h"
#include "map_editor.h"
#include "gameover.h"
#include "game.h"
#include "defs.h"
#include "option.h"
#include "resources.h"

void startMenu_JoyEvent(u16 joy, u16 changed, u16 state);
void modeMenu_JoyEvent(u16 joy, u16 changed, u16 state);

s8 selector;
s8 selected_item;
u16 scroll = 0;

void startMenu(s8 do_scroll) {

    selected_item = -1;

    selector = 0;

    if (do_scroll) {
        scroll = -224;
    }

    u32 now_top = game_player[0].scor > game_player[1].scor ? game_player[0].scor : game_player[1].scor;

    VDP_setPalette(0, palette_black);
    VDP_resetSprites();

    VDP_fillTileMapRect(PLAN_B, 0, 0, 0, planWidth, planHeight);

    setMap(PLAN_B, maps_data, 0);

    drawNum(top_scor, 136 / 8, 1);
    drawNum(now_top, 56 / 8, 1);
    VDP_setVerticalScroll(PLAN_B, scroll);
    sleepFrames(1);
    VDP_setPalette(0, pal_menu);
    VDP_setPalette(1, pal_yellow);
    _tank tank;
    tank.color = TANK_COLOR_RED;
    tank.posx = SPRITE_POS_SELECTOR_X;
    tank.posy = SPRITE_POS_SELECTOR_Y;
    tank.rotate = 3;
    tank.type = 0;
    tank.color = TANK_COLOR_YELLOW;
    tank.hitpoint = 1;
    tank.god = 0;
    tank.birth = 0;
    tank.speed = 1;
    tank.freeze = 0;
    tank.bonus = 0;

    JOY_setEventHandler(startMenu_JoyEvent);

    while (scroll++) {
        VDP_setVerticalScroll(PLAN_B, scroll);
        VDP_waitVSync();
    }
    scroll = 0;
    VDP_setVerticalScroll(PLAN_B, 0);

    for (;;) {
        tank.posy = SPRITE_POS_SELECTOR_Y + (selector << 4);
        drawTank(&tank);
        updateSprite();
        VDP_waitVSync();
        if (selected_item >= 0)
            break;
    }

    JOY_setEventHandler(NULL);
}

void modeMenu() {

    selector = 0;

    u32 now_top = game_player[0].scor > game_player[1].scor ? game_player[0].scor : game_player[1].scor;

    VDP_setPalette(0, palette_black);
    VDP_resetSprites();

    VDP_fillTileMapRect (PLAN_B, 0, 0, 0, planWidth, planHeight);

    setMap(PLAN_B, menu_data, 0);

    drawNum(top_scor, 136 / 8, 1);
    drawNum(now_top, 56 / 8, 1);
    VDP_setVerticalScroll(PLAN_B, scroll);
    sleepFrames(1);
    VDP_setPalette(0, pal_menu);
    VDP_setPalette(1, pal_yellow);
    _tank tank;
    tank.color = TANK_COLOR_RED;
    tank.posx = SPRITE_POS_SELECTOR_X;
    tank.posy = SPRITE_POS_SELECTOR_Y;
    tank.rotate = 3;
    tank.type = 0;
    tank.color = TANK_COLOR_YELLOW;
    tank.hitpoint = 1;
    tank.god = 0;
    tank.birth = 0;
    tank.speed = 1;
    tank.freeze = 0;
    tank.bonus = 0;

    JOY_setEventHandler(modeMenu_JoyEvent);

    config.game_mode = 0;
    for (;;) {
        tank.posy = SPRITE_POS_SELECTOR_Y + (selector << 4);
        drawTank(&tank);
        updateSprite();
        VDP_waitVSync();
        if (config.game_mode > 0)
            break;
    }

    JOY_setEventHandler(NULL);
}

s8 menuGetSelectedItem() {
    return selected_item;
}

void startMenu_JoyEvent(u16 joy, u16 changed, u16 state) {

    if (scroll) {
        if (BUTTON_START & (state & changed)) {
            scroll = 0;
        }
    }
    else {
        if ((BUTTON_START & (state & changed)) && (BUTTON_A & state)) {
            selected_item = selector;
            if (selector == 3) {
                config.cheats_on = ON;
                soundPlay(snd_score_bonus, sizeof(snd_score_bonus), SOUND_PCM_CH1, FALSE);
            }
        }

        switch (state & changed) {
            case BUTTON_UP:
                selector--;
                if (selector < 0)
                    selector = 3;
                break;
            case BUTTON_DOWN:
                selector++;
                if (selector > 3)
                    selector = 0;
                break;
            case BUTTON_START:
                selected_item = selector;
                break;
        }
    }
}


void modeMenu_JoyEvent(u16 joy, u16 changed, u16 state) {

    switch (state & changed) {
        case BUTTON_UP:
            selector--;
            if (selector < 0)
                selector = 3;
            break;
        case BUTTON_DOWN:
            selector++;
            if (selector > 3)
                selector = 0;
            break;
        case BUTTON_START:
            switch (selector) {
                case 0:
                    config.game_mode = MODE_MODS;
                    break;
                case 1:
                    config.game_mode = MODE_BC;
                    break;
                case 2:
                    config.game_mode = MODE_TANK;
                    break;
                case 3:
                    config.game_mode = MODE_CUSTOM;
                    break;
            }
            break;
    }
}
