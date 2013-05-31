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

        TCODConsole::root->setBackgroundColor(TCODColor(0, 0, 0));

       /* Update the output text */
        int screen_pos = 0; // actual position to print to
        int position = 0; // the "position" to start looping through in the message buffer

        if(player.msgbuff.size() >= 15) // offset the position if the msgbuff is larger than the "window"
            position = player.msgbuff.size() - 15;

        // Display the last 15 messages
        int maxdisplay = player.msgbuff.size();
        for(int i = position; i <= maxdisplay; i++) {
            if( i + 1 <= player.msgbuff.size() ) {

                Msg* selectedmsg = player.msgbuff[i];

                TCODConsole::root->setBackgroundColor(selectedmsg->bgcolor);
                TCODConsole::root->setForegroundColor(selectedmsg->fcolor);
                TCODConsole::root->printLeft(0, 55 + screen_pos, TCOD_BKGND_SET, selectedmsg->message.c_str());
                screen_pos++;
            }
        }

       /* Update the character stats */
        TCODConsole::root->setForegroundColor(TCODColor(100, 100, 100));
        TCODConsole::setColorControl(TCOD_COLCTRL_1,TCODColor::white,TCODColor::black);
        TCODConsole::root->printLeft(52, 56, TCOD_BKGND_SET, "Health: %c %i   /   %i %c", TCOD_COLCTRL_1, player.HP, player.Max_HP, TCOD_COLCTRL_STOP);
        TCODConsole::setColorControl(TCOD_COLCTRL_1,TCODColor(50, 150, 50),TCODColor::black);
        TCODConsole::root->printLeft(58, 58, TCOD_BKGND_SET, "Str: %c%i%c", TCOD_COLCTRL_1, player.strength, TCOD_COLCTRL_STOP);
        TCODConsole::root->printLeft(58, 59, TCOD_BKGND_SET, "Int: %c%i%c", TCOD_COLCTRL_1, player.intelligence, TCOD_COLCTRL_STOP);
        TCODConsole::root->printLeft(58, 60, TCOD_BKGND_SET, "Dex: %c%i%c", TCOD_COLCTRL_1, player.dexterity, TCOD_COLCTRL_STOP);
        TCODConsole::root->printLeft(73, 58, TCOD_BKGND_SET, "Per: %c%i%c", TCOD_COLCTRL_1, player.perception, TCOD_COLCTRL_STOP);
        TCODConsole::root->printLeft(73, 59, TCOD_BKGND_SET, "Con: %c%i%c", TCOD_COLCTRL_1, player.constitution, TCOD_COLCTRL_STOP);
        TCODConsole::root->printLeft(73, 60, TCOD_BKGND_SET, "Spr: %c%i%c", TCOD_COLCTRL_1, player.spirit, TCOD_COLCTRL_STOP);

       /* Print items held in hands */
        // Left hand:
        if(!player.rhandselected) { // selected
            TCODConsole::root->setForegroundColor(TCODColor::white);
        }
        TCODConsole::root->printLeft(52, 63, TCOD_BKGND_SET, "L hand:");
        if(player.lefthand != NULL)
            TCODConsole::root->printLeft(60, 63, TCOD_BKGND_SET, player.lefthand->name.c_str());

        // Right hand:
        if(!player.rhandselected) { // not selected
            TCODConsole::root->setForegroundColor(TCODColor(100, 100, 100));
        }
        else { // selected
            TCODConsole::root->setForegroundColor(TCODColor::white);
        }
        TCODConsole::root->printLeft(52, 64, TCOD_BKGND_SET, "R hand:");
        if(player.righthand != NULL)
            TCODConsole::root->printLeft(60, 64, TCOD_BKGND_SET, player.righthand->name.c_str());

        TCODConsole::root->setForegroundColor(TCODColor(100, 100, 100));
        TCODConsole::setColorControl(TCOD_COLCTRL_1,TCODColor(51, 255, 153),TCODColor::black);
        TCODConsole::root->printLeft(52, 67, TCOD_BKGND_SET, "Ether: %c %i / %i %c", TCOD_COLCTRL_1, player.Ether, player.Max_Ether, TCOD_COLCTRL_STOP);



    /* Draw all the other hud accessories like frames */
    } else {

       /* The messages frame */
        // Draw the main screen frame:
        TCODConsole::root->setForegroundColor(TCODColor(100, 100, 100));
        TCODConsole::root->printFrame(0, 0, VIEW_WIDTH, 55, false, TCOD_BKGND_NONE, NULL);

        // Draw the messages frame
        TCODConsole::root->setForegroundColor(TCODColor(100, 100, 100));
        TCODConsole::root->putChar(24, 54, 205);
        TCODConsole::root->putChar(15, 54, 205);
        TCODConsole::root->setBackgroundColor(TCODColor(70, 70, 70));
        TCODConsole::root->setForegroundColor(TCODColor(200, 200, 200));
        TCODConsole::root->printLeft(16, 54, TCOD_BKGND_SET, "Messages");

        // Draw the separator between output and character stats
        TCODConsole::root->setBackgroundColor(TCODColor(0, 0, 0));
        TCODConsole::root->setForegroundColor(TCODColor(100, 100, 100));
        TCODConsole::root->vline(50, 56, 13);

       /* The character frame */
        TCODConsole::root->setForegroundColor(TCODColor(100, 100, 100));
        TCODConsole::root->putChar(73, 54, 205);
        TCODConsole::root->putChar(63, 54, 205);
        TCODConsole::root->setBackgroundColor(TCODColor(70, 70, 70));
        TCODConsole::root->setForegroundColor(TCODColor(200, 200, 200));
        TCODConsole::root->printLeft(64, 54, TCOD_BKGND_SET, "Character");
    }
    // Reset the colors back to normal
    TCODConsole::root->setBackgroundColor(TCODColor(0, 0, 0));
}

void Game::DrawInv() {

    if(player.viewinginventory) {
        // Refresh the inventory console if it exists
        if(_inventoryConsole == NULL) {
            _inventoryConsole = new TCODConsole(INV_WIDTH, INV_HEIGHT);
        }

        _inventoryConsole->setBackgroundColor(TCODColor(104, 104, 104));

        // Flood the console with the above color
        for(int i = 0; i < INV_WIDTH; i++) {
            for(int j = 0; j < INV_HEIGHT; j++) {
                _inventoryConsole->putChar(i, j, ' ');
            }
        }

        // Draw the frame and other accessories
        TCODConsole::root->setForegroundColor(TCODColor(128, 128, 128));
        _inventoryConsole->printFrame(0, 0, INV_WIDTH, INV_HEIGHT, false, TCOD_BKGND_NONE, NULL);
        _inventoryConsole->setBackgroundColor(TCODColor(128, 128, 128));
        _inventoryConsole->setForegroundColor(TCODColor::white);
        _inventoryConsole->printCenter(INV_WIDTH / 2, 0, TCOD_BKGND_SET, "Inventory");

        // Begin drawing the inventory items
        _inventoryConsole->setBackgroundColor(TCODColor(104, 104, 104));
        _inventoryConsole->setForegroundColor(TCODColor(200, 200, 200));

        int offset = 0;
        if(player.righthand) {
            offset++;
        }
        if(player.lefthand) {
            offset++;
        }
        for(unsigned int i = 0; i < player.inventory.size(); i++) {
            Item* item = player.inventory[i];

            if(item == player.lefthand || item == player.righthand) { // we will draw these later
                continue;
            }
            // Draw the item name in a special color if it is selected
            if(player.selecteditem == item) {
                _inventoryConsole->setBackgroundColor(TCODColor(155, 155, 155));
                _inventoryConsole->setForegroundColor(TCODColor::white);
            }

            // Print the name and character index
            _inventoryConsole->printLeft(1, i + offset, TCOD_BKGND_SET, "[");
            _inventoryConsole->printLeft(3, i + offset, TCOD_BKGND_SET, "] ");
            _inventoryConsole->printLeft(2, i + offset, TCOD_BKGND_SET, std::string(1, item->index).c_str());
            _inventoryConsole->printLeft(5, i + offset, TCOD_BKGND_SET, item->name.c_str());
            std::string slotstr=""; // for stuff like "(WORN)", "(R HAND)", etc
            if(Helper::Find(player.Worn, item))
                slotstr += "{WORN}";

            if(slotstr != "") {
                _inventoryConsole->printRight(INV_WIDTH-2, i + offset, TCOD_BKGND_SET, slotstr.c_str());
            }

            // Reset the colors
            if(player.selecteditem == item) {
                _inventoryConsole->setBackgroundColor(TCODColor(104, 104, 104));
                _inventoryConsole->setForegroundColor(TCODColor(200, 200, 200));
            }
        }
        _inventoryConsole->setBackgroundColor(TCODColor(104, 104, 104));
        _inventoryConsole->setForegroundColor(TCODColor(200, 200, 200));

        // Now we draw the left and right hand items
        int posy = 0;
        if(player.righthand) {
            posy++;
            if(player.selecteditem == player.righthand) {
                _inventoryConsole->setBackgroundColor(TCODColor(155, 155, 155));
                _inventoryConsole->setForegroundColor(TCODColor::white);
            }
            _inventoryConsole->printLeft(1, posy, TCOD_BKGND_SET, "[");
            _inventoryConsole->printLeft(3, posy, TCOD_BKGND_SET, "] ");
            _inventoryConsole->printLeft(2, posy, TCOD_BKGND_SET, std::string(1, player.righthand->index).c_str());
            _inventoryConsole->printLeft(5, posy, TCOD_BKGND_SET, player.righthand->name.c_str());
            std::string slotstr="{R HAND}"; // for stuff like "(WORN)", "(R HAND)", etc
            _inventoryConsole->printRight(INV_WIDTH-2, posy, TCOD_BKGND_SET, slotstr.c_str());

            if(player.selecteditem == player.righthand) {
                _inventoryConsole->setBackgroundColor(TCODColor(104, 104, 104));
                _inventoryConsole->setForegroundColor(TCODColor(200, 200, 200));
            }
        }
        if(player.lefthand) {
            posy++;
            if(player.selecteditem == player.lefthand) {
                _inventoryConsole->setBackgroundColor(TCODColor(155, 155, 155));
                _inventoryConsole->setForegroundColor(TCODColor::white);
            }
            _inventoryConsole->printLeft(1, posy, TCOD_BKGND_SET, "[");
            _inventoryConsole->printLeft(3, posy, TCOD_BKGND_SET, "] ");
            _inventoryConsole->printLeft(2, posy, TCOD_BKGND_SET, std::string(1, player.lefthand->index).c_str());
            _inventoryConsole->printLeft(5, posy, TCOD_BKGND_SET, player.lefthand->name.c_str());
            std::string slotstr="{L HAND}"; // for stuff like "(WORN)", "(R HAND)", etc
            _inventoryConsole->printRight(INV_WIDTH-2, posy, TCOD_BKGND_SET, slotstr.c_str());

            if(player.selecteditem == player.lefthand) {
                _inventoryConsole->setBackgroundColor(TCODColor(104, 104, 104));
                _inventoryConsole->setForegroundColor(TCODColor(200, 200, 200));
            }
        }


        // Blit the inventory console onto the main console, roughly the center of the screen
        TCODConsole::blit(_inventoryConsole, 0, 0, INV_WIDTH, INV_HEIGHT, TCODConsole::root, 16, 2, 1, 0.9);
        // Draw an additional tip at the bottom of the inventory console
        TCODConsole::root->printCenter((INV_WIDTH/2 + 16), 2+INV_HEIGHT, TCOD_BKGND_SET, "Press SPACE to close.");
        // Draw some text on top if necessary
        if(player.selectingwear) {
            TCODConsole::root->printCenter((INV_WIDTH/2 + 16), 1, TCOD_BKGND_SET, "Select an item to wear.");
        } else if(player.selectingwield) {
            TCODConsole::root->printCenter((INV_WIDTH/2 + 16), 1, TCOD_BKGND_SET, "Select an item to put in your hand.");
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

        _inventoryInfo->setBackgroundColor(TCODColor(114, 114, 114));

        // Flood the console with the above color
        for(int i = 0; i < INV_I_WIDTH; i++) {
            for(int j = 0; j < INV_I_HEIGHT; j++) {
                _inventoryInfo->putChar(i, j, ' ');
            }
        }

        // Draw the frame and other accessories
        TCODConsole::root->setForegroundColor(TCODColor(148, 148, 148));
        _inventoryInfo->printFrame(0, 0, INV_I_WIDTH, INV_I_HEIGHT, false, TCOD_BKGND_NONE, NULL);
        _inventoryInfo->setBackgroundColor(TCODColor(148, 148, 148));
        _inventoryInfo->setForegroundColor(TCODColor::white);
        _inventoryInfo->printCenter(INV_I_WIDTH / 2, 0, TCOD_BKGND_SET, player.selecteditem->name.c_str());

        // Begin drawing the the information
        _inventoryInfo->setBackgroundColor(TCODColor(114, 114, 114));
        _inventoryInfo->setForegroundColor(TCODColor(210, 210, 210));

        int line = 1;
        // Weight and Volume:
        _inventoryInfo->printLeft(1, line, TCOD_BKGND_SET, "Weight: %i",  player.selecteditem->weight);
        line++;
        _inventoryInfo->printLeft(1, line, TCOD_BKGND_SET, "Volume: %i",  player.selecteditem->volume);
        line++;
        // Apply item-specific info to console, add appropriate number of lines
        line = player.selecteditem->GetDescAdd(_inventoryInfo, line);
        line++;
        // Description:
        _inventoryInfo->hline(2, line, 41);
        line+=2;
        _inventoryInfo->printLeftRect(1, line, INV_I_WIDTH-2, INV_I_HEIGHT-2, TCOD_BKGND_SET, player.selecteditem->desc.c_str());

        // Blit the info console onto the main console, on top of the inventory console
        TCODConsole::blit(_inventoryInfo, 0, 0, INV_I_WIDTH, INV_I_HEIGHT, TCODConsole::root, 43, 5, 1, 0.9);

    }
}
