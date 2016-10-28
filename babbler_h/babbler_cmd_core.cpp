#include "babbler_cmd_core.h"

#include "babbler.h"

#include "stdio.h"
#include "string.h"

extern const babbler_cmd_t CMD_HELP = {
    "help",
    &cmd_help
};

extern const babbler_man_t MAN_HELP = {
    "help",
    "list available commands or show detailed help on selected command", 
    "SYNOPSIS\n"
    "    help\n"
    "    help [cmd_name]\n"
    "    help --list\n"
    "DESCRIPTION\n"
    "List available commands or show detailed help on selected command. "
    "Running help with no options would list commands with short description.\n"
    "OPTIONS\n"
    "    cmd_name - command name to show detailed help for\n"
    "    --list - list all available commands separated by space"
};


extern const babbler_cmd_t CMD_PING = {
    "ping",
    &cmd_ping
};

extern const babbler_man_t MAN_PING = {
    "ping",
    "check if device is available", 
    "SYNOPSIS\n"
    "    ping\n"
    "DESCRIPTION\n"
    "Check if device is available, returns \"ok\" if device is ok"
};


/** 
 * Вывести список команд.
 */
int cmd_help(char* reply_buffer, int reply_buf_size, int argc, char *argv[]) {
    if(argc <= 1) {
        // параметры не заданы или задан только 1й параметр (имя команды) - 
        // выводим список команд с кратким описанием
        sprintf(reply_buffer, "Commands: \n");
        for(int i=0; i < BABBLER_MANUALS_COUNT; i++) {
            sprintf(reply_buffer+strlen(reply_buffer), "%s\n", BABBLER_MANUALS[i].name);
            if(BABBLER_MANUALS[i].short_descr != NULL) {
                sprintf(reply_buffer+strlen(reply_buffer), "    %s\n", BABBLER_MANUALS[i].short_descr);
            }
        }
    } else if(strcmp("--list", argv[1]) == 0) {
        // вывести список всех команд через пробел
        for(int i=0; i < BABBLER_MANUALS_COUNT; i++) {
            sprintf(reply_buffer+strlen(reply_buffer), "%s", BABBLER_MANUALS[i].name);
            // добавлять пробел после каждой команды, кроме последней
            if(i < BABBLER_MANUALS_COUNT - 1) {
                sprintf(reply_buffer+strlen(reply_buffer), " ");
            }
        }
    } else {
        // вывести справку по указанной команде
        bool cmd_found = false;
        for(int i=0; i < BABBLER_MANUALS_COUNT && !cmd_found; i++) {
            if(strcmp(BABBLER_MANUALS[i].name, argv[1]) == 0) {
                sprintf(reply_buffer+strlen(reply_buffer), 
                    "%s - manual\n", BABBLER_MANUALS[i].name);
                if(BABBLER_MANUALS[i].short_descr != NULL && BABBLER_MANUALS[i].manual != NULL) {
                    sprintf(reply_buffer+strlen(reply_buffer), 
                        "NAME\n"
                        "    %s - %s\n"
                        "%s",
                        BABBLER_MANUALS[i].name, BABBLER_MANUALS[i].short_descr, 
                        BABBLER_MANUALS[i].manual);
                }
                
                cmd_found = true;
            }
        }
        
        if(!cmd_found) {
            // команда не найдена
            sprintf(reply_buffer+strlen(reply_buffer), "help: COMMAND NOT FOUND: %s", argv[1]);
        }
    }
        
    return strlen(reply_buffer);
}


/** 
 * Проверить доступность устройства - вернуть 'ok'.
 */
int cmd_ping(char* reply_buffer, int reply_buf_size, int argc, char *argv[]) {
    // команда выполнена
    strcpy(reply_buffer, REPLY_OK);
    return strlen(reply_buffer);
}

