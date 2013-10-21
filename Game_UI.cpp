#include "Game.h"
#include "Entity.h"
#include "Mob.h"
#include "Item.h"
#include "Map.h"
#include "Helper.h"
#include <vector>
#include <iostream>

void Game::DrawHud(bool update) {
    /* Update the hud information: */
    if(update) {

        TCODConsole::root->setBackgroundFlag(TCOD_BKGND_SET);
        TCODConsole::root->setDefaultBackground(TCODColor(0, 0, 0));
        TCODConsole::root->setAlignment(TCOD_LEFT);

       /* Update the output text */
        int screen_pos = 0; // actual position to print to
        int position = 0; // the "position" to start looping through in the message buffer

        if(player.msgbuff.size() >= MAX_MESSAGE_HEIGHT-33) // offset the position if the msgbuff is larger than the "window"
            position = player.msgbuff.size() - (MAX_MESSAGE_HEIGHT-33);

        // Display the last 15 messages
        int maxdisplay = player.msgbuff.size();
        for(int i = position; i <= maxdisplay; i++) {
            if( i + 1 <= player.msgbuff.size() ) {

                Msg* selectedmsg = player.msgbuff[i];

                TCODConsole::root->setDefaultBackground(selectedmsg->bgcolor);
                TCODConsole::root->setDefaultForeground(selectedmsg->fcolor);
                TCODConsole::root->print(0, UI_BOTTOM_DIV+1 + screen_pos, selectedmsg->message.c_str());
                screen_pos++;
            }
        }

       /* Update the character stats */
        TCODConsole::root->setDefaultForeground(TCODColor(100, 100, 100));
        TCODConsole::setColorControl(TCOD_COLCTRL_1,TCODColor::white,TCODColor::black);
        TCODConsole::root->print(52, UI_BOTTOM_DIV+2, "Health: %c %i   /   %i %c", TCOD_COLCTRL_1, player.HP, player.Max_HP, TCOD_COLCTRL_STOP);
        TCODConsole::setColorControl(TCOD_COLCTRL_1,TCODColor(50, 150, 50),TCODColor::black);
        TCODConsole::root->print(58, UI_BOTTOM_DIV+4, "Str: %c%i%c", TCOD_COLCTRL_1, player.strength, TCOD_COLCTRL_STOP);
        TCODConsole::root->print(58, UI_BOTTOM_DIV+5, "Int: %c%i%c", TCOD_COLCTRL_1, player.intelligence, TCOD_COLCTRL_STOP);
        TCODConsole::root->print(58, UI_BOTTOM_DIV+6, "Dex: %c%i%c", TCOD_COLCTRL_1, player.dexterity, TCOD_COLCTRL_STOP);
        TCODConsole::root->print(73, UI_BOTTOM_DIV+4, "Per: %c%i%c", TCOD_COLCTRL_1, player.perception, TCOD_COLCTRL_STOP);
        TCODConsole::root->print(73, UI_BOTTOM_DIV+5, "Con: %c%i%c", TCOD_COLCTRL_1, player.constitution, TCOD_COLCTRL_STOP);
        TCODConsole::root->print(73, UI_BOTTOM_DIV+6, "Spr: %c%i%c", TCOD_COLCTRL_1, player.spirit, TCOD_COLCTRL_STOP);


       /* Print items held in hands */
        // Left hand:
        if(!player.rhandselected) { // selected
            TCODConsole::root->setDefaultForeground(TCODColor::white);
        }
        TCODConsole::root->print(52, UI_BOTTOM_DIV+9, "L hand:");
        if(player.lefthand != NULL)
            TCODConsole::root->print(60, UI_BOTTOM_DIV+9, player.lefthand->name.c_str());

        // Right hand:
        if(!player.rhandselected) { // not selected
            TCODConsole::root->setDefaultForeground(TCODColor(100, 100, 100));
        }
        else { // selected
            TCODConsole::root->setDefaultForeground(TCODColor::white);
        }
        TCODConsole::root->print(52, UI_BOTTOM_DIV+10, "R hand:");
        if(player.righthand != NULL)
            TCODConsole::root->print(60, UI_BOTTOM_DIV+10, player.righthand->name.c_str());

        TCODConsole::root->setDefaultForeground(TCODColor(100, 100, 100));
        TCODConsole::setColorControl(TCOD_COLCTRL_1,TCODColor(51, 255, 153),TCODColor::black);
        TCODConsole::root->print(52, UI_BOTTOM_DIV+13, "Ether: %c %i / %i %c", TCOD_COLCTRL_1, player.Ether, player.Max_Ether, TCOD_COLCTRL_STOP);


    /* Draw all the other hud accessories like frames */
    } else {

       /* The messages frame */
        // Draw the main screen frame:
        TCODConsole::root->setDefaultForeground(TCODColor(100, 100, 100));
        TCODConsole::root->printFrame(0, 0, VIEW_WIDTH, UI_BOTTOM_DIV+1, false, TCOD_BKGND_NONE, NULL);

        // Draw the messages frame
        TCODConsole::root->setDefaultForeground(TCODColor(100, 100, 100));
        TCODConsole::root->putChar(24, UI_BOTTOM_DIV, 205);
        TCODConsole::root->putChar(15, UI_BOTTOM_DIV, 205);
        TCODConsole::root->setDefaultBackground(TCODColor(70, 70, 70));
        TCODConsole::root->setDefaultForeground(TCODColor(200, 200, 200));
        TCODConsole::root->setAlignment(TCOD_LEFT);
        TCODConsole::root->print(16, UI_BOTTOM_DIV, "Messages");

        // Draw the separator between output and character stats
        TCODConsole::root->setDefaultBackground(TCODColor(0, 0, 0));
        TCODConsole::root->setDefaultForeground(TCODColor(100, 100, 100));
        TCODConsole::root->vline(50, UI_BOTTOM_DIV+4, 13);

       /* The character frame */
        TCODConsole::root->setDefaultForeground(TCODColor(100, 100, 100));
        TCODConsole::root->putChar(73, UI_BOTTOM_DIV, 205);
        TCODConsole::root->putChar(63, UI_BOTTOM_DIV, 205);
        TCODConsole::root->setDefaultBackground(TCODColor(70, 70, 70));
        TCODConsole::root->setDefaultForeground(TCODColor(200, 200, 200));
        TCODConsole::root->print(64, UI_BOTTOM_DIV, "Character");
    }
    // Reset the colors back to normal
    TCODConsole::root->setDefaultBackground(TCODColor(0, 0, 0));
}

void Game::DrawInv() {

    if(player.viewinginventory) {
        // Refresh the inventory console if it exists
        if(_inventoryConsole == NULL) {
            _inventoryConsole = new TCODConsole(INV_WIDTH, INV_HEIGHT);
        }

        _inventoryConsole->setBackgroundFlag(TCOD_BKGND_SET);
        _inventoryConsole->setDefaultBackground(TCODColor(104, 104, 104));

        // Flood the console with the above color
        for(int i = 0; i < INV_WIDTH; i++) {
            for(int j = 0; j < INV_HEIGHT; j++) {
                _inventoryConsole->putChar(i, j, ' ');
            }
        }

        // Draw the frame and other accessories
        TCODConsole::root->setDefaultForeground(TCODColor(128, 128, 128));
        _inventoryConsole->printFrame(0, 0, INV_WIDTH, INV_HEIGHT, false, TCOD_BKGND_NONE, NULL);
        _inventoryConsole->setDefaultBackground(TCODColor(128, 128, 128));
        _inventoryConsole->setDefaultForeground(TCODColor::white);
        _inventoryConsole->setAlignment(TCOD_CENTER);
        _inventoryConsole->print(INV_WIDTH / 2, 0, "Inventory");

        // Begin drawing the inventory items
        _inventoryConsole->setDefaultBackground(TCODColor(104, 104, 104));
        _inventoryConsole->setDefaultForeground(TCODColor(200, 200, 200));

        int offset = 1;

        for(unsigned int i = 0; i < player.inventory.size(); i++) {
            Item* item = player.inventory[i];

            // Draw the item name in a special color if it is selected
            if(player.selecteditem == item) {
                _inventoryConsole->setDefaultBackground(TCODColor(155, 155, 155));
                _inventoryConsole->setDefaultForeground(TCODColor::white);
            }

            // Print the name and character index
            _inventoryConsole->setAlignment(TCOD_LEFT);
            _inventoryConsole->print(1, i + offset, "[");
            _inventoryConsole->print(3, i + offset, "] ");
            _inventoryConsole->print(2, i + offset, std::string(1, item->index).c_str());
            _inventoryConsole->print(5, i + offset, item->name.c_str());
            std::string slotstr=""; // for stuff like "(WORN)", "(R HAND)", etc
            if(Helper::Find(player.Worn, item)) {
                slotstr += "{WORN}";
            } else if(item == player.lefthand) {
                slotstr += "{L HAND}";
            } else if(item == player.righthand) {
                slotstr += "{R HAND}";
            }

            if(slotstr != "") {
                TCODConsole::root->setAlignment(TCOD_RIGHT);
                _inventoryConsole->print(INV_WIDTH-slotstr.size()-1, i + offset, slotstr.c_str());
                TCODConsole::root->setAlignment(TCOD_LEFT);
            }

            // Reset the colors
            if(player.selecteditem == item) {
                _inventoryConsole->setDefaultBackground(TCODColor(104, 104, 104));
                _inventoryConsole->setDefaultForeground(TCODColor(200, 200, 200));
            }
        }
        _inventoryConsole->setDefaultBackground(TCODColor(104, 104, 104));
        _inventoryConsole->setDefaultForeground(TCODColor(200, 200, 200));


        // Blit the inventory console onto the main console, roughly the center of the screen
        TCODConsole::blit(_inventoryConsole, 0, 0, INV_WIDTH, INV_HEIGHT, TCODConsole::root, 16, 2, 1, 0.9);
        // Draw an additional tip at the bottom of the inventory console
        TCODConsole::root->setAlignment(TCOD_CENTER);
        TCODConsole::root->print((INV_WIDTH/2 + 16), 2+INV_HEIGHT, "Press SPACE to close.");
        // Draw some text on top if necessary
        if(player.selectingwear) {
            TCODConsole::root->print((INV_WIDTH/2 + 16), 1, "Select an item to wear.");
        } else if(player.selectingwield) {
            TCODConsole::root->print((INV_WIDTH/2 + 16), 1,  "Select an item to put in your hand.");
        }

        // Update inventory information (examining/selecting specific item)
        InvInfo();
    }
}

void Game::InvInfo() {
    if(player.selecteditem) {

        // Refresh the info console if it exists
        if(_inventoryInfo == NULL) {
            _inventoryInfo = new TCODConsole(INV_I_WIDTH, INV_I_HEIGHT);
        }

        _inventoryInfo->setBackgroundFlag(TCOD_BKGND_SET);
        _inventoryInfo->setDefaultBackground(TCODColor(114, 114, 114));

        // Flood the console with the above color
        for(int i = 0; i < INV_I_WIDTH; i++) {
            for(int j = 0; j < INV_I_HEIGHT; j++) {
                _inventoryInfo->putChar(i, j, ' ');
            }
        }

        // Draw the frame and other accessories
        TCODConsole::root->setDefaultForeground(TCODColor(148, 148, 148));
        _inventoryInfo->printFrame(0, 0, INV_I_WIDTH, INV_I_HEIGHT, false, TCOD_BKGND_NONE, NULL);
        _inventoryInfo->setDefaultBackground(TCODColor(148, 148, 148));
        _inventoryInfo->setDefaultForeground(TCODColor::white);
        _inventoryInfo->setAlignment(TCOD_CENTER);
        _inventoryInfo->print(INV_I_WIDTH / 2, 0, player.selecteditem->name.c_str());

        // Begin drawing the the information
        _inventoryInfo->setDefaultBackground(TCODColor(114, 114, 114));
        _inventoryInfo->setDefaultForeground(TCODColor(210, 210, 210));

        int line = 1;
        // Weight and Volume:
        _inventoryInfo->setAlignment(TCOD_LEFT);
        _inventoryInfo->print(1, line, "Weight: %i",  player.selecteditem->weight);
        line++;
        _inventoryInfo->print(1, line, "Volume: %i",  player.selecteditem->volume);
        line++;
        // Apply item-specific info to console, add appropriate number of lines
        line = player.selecteditem->GetDescAdd(_inventoryInfo, line);
        line++;
        // Description:
        _inventoryInfo->hline(2, line, 41);
        line+=2;
        _inventoryInfo->printRect(1, line, INV_I_WIDTH-2, INV_I_HEIGHT-2, player.selecteditem->desc.c_str());

        // Blit the info console onto the main console, on top of the inventory console
        TCODConsole::blit(_inventoryInfo, 0, 0, INV_I_WIDTH, INV_I_HEIGHT, TCODConsole::root, 43, 5, 1, 0.9);

    }
}
