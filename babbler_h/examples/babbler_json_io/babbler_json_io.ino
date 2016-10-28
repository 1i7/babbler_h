#include "babbler.h"
#include "babbler_cmd_core.h"
#include "babbler_simple.h"
#include "babbler_json.h"
#include "babbler_serial.h"

// Размеры буферов для чтения команд и записи ответов
// Read and write buffer size for communication modules
#define SERIAL_READ_BUFFER_SIZE 128
#define SERIAL_WRITE_BUFFER_SIZE 512

// Буферы для обмена данными с компьютером через последовательный порт.
// +1 байт в конце для завершающего нуля
// Data exchange buffers to communicate with computer via serial port.
// +1 extra byte at the end for terminating zero
char serial_read_buffer[SERIAL_READ_BUFFER_SIZE+1];
char serial_write_buffer[SERIAL_WRITE_BUFFER_SIZE];

#define LED_PIN 13

/** Реализация команды ledon (включить лампочку) */
/** ledon (turn led ON) command implementation */
int cmd_ledon(char* reply_buffer, int reply_buf_size, int argc=0, char *argv[]=NULL) {
    digitalWrite(LED_PIN, HIGH);
    
    // команда выполнена
    strcpy(reply_buffer, REPLY_OK);
    return strlen(reply_buffer);
}

/** Реализация команды ledoff (включить лампочку) */
/** ledoff (turn led OFF) command implementation */
int cmd_ledoff(char* reply_buffer, int reply_buf_size, int argc=0, char *argv[]=NULL) {
    digitalWrite(LED_PIN, LOW);
    
    // команда выполнена
    strcpy(reply_buffer, REPLY_OK);
    return strlen(reply_buffer);
}

babbler_cmd_t CMD_LEDON = {
    /* имя команды */ 
    /* command name */
    "ledon",
    /* указатель на функцию с реализацией команды */ 
    /* pointer to function with command implementation*/ 
    &cmd_ledon
};

babbler_man_t MAN_LEDON = {
    /* имя команды */ 
    /* command name */
    "ledon",
    /* краткое описание */ 
    /* short description */
    "turn led ON",
    /* руководство */ 
    /* manual */
    "SYNOPSIS\n"
    "    ledon\n"
    "DESCRIPTION\n"
    "Turn led ON."
};

babbler_cmd_t CMD_LEDOFF = {
    /* имя команды */ 
    /* command name */
    "ledoff",
    /* указатель на функцию с реализацией команды */ 
    /* pointer to function with command implementation*/ 
    &cmd_ledoff
};

babbler_man_t MAN_LEDOFF = {
    /* имя команды */ 
    /* command name */
    "ledoff",
    /* краткое описание */ 
    /* short description */
    "turn led OFF",
    /* руководство */ 
    /* manual */
    "SYNOPSIS\n"
    "    ledoff\n"
    "DESCRIPTION\n"
    "Turn led OFF."
};

/** Зарегистрированные команды */
/** Registered commands */
extern const babbler_cmd_t BABBLER_COMMANDS[] = {
    // команды из babbler_cmd_core.h
    // commands from babbler_cmd.core.h
    CMD_HELP,
    CMD_PING,
    
    // пользовательские команды
    // custom commands
    CMD_LEDON,
    CMD_LEDOFF
};

/** Количество зарегистрированных команд */
/** Number of registered commands*/
extern const int BABBLER_COMMANDS_COUNT = sizeof(BABBLER_COMMANDS)/sizeof(babbler_cmd_t);


/** Руководства для зарегистрированных команд */
/** Manuals for registered commands */
extern const babbler_man_t BABBLER_MANUALS[] = {
    // команды из babbler_cmd_core.h
    // commands from babbler_cmd.core.h
    MAN_HELP,
    MAN_PING,
    
    // пользовательские команды
    // custom commands
    MAN_LEDON,
    MAN_LEDOFF
};

/** Количество руководств для зарегистрированных команд */
/** Number of manuals for registered commands */
extern const int BABBLER_MANUALS_COUNT = sizeof(BABBLER_MANUALS)/sizeof(babbler_man_t);


void setup() {
    Serial.begin(9600);
    Serial.println("Starting babbler-powered device with JSON i/o,"
        " type {\"cmd\": \"help\", \"id\": \"34\", \"params\":[]} for list of commands");
    // попробуйте отправить через монитор последовательного порта
    // try to send via Serial Monitor
    // {"cmd": "help", "id": "34", "params":[]}
    
    babbler_serial_set_packet_filter(packet_filter_newline);
    babbler_serial_set_input_handler(handle_input_json);
    //babbler_serial_setup(
    //    serial_read_buffer, SERIAL_READ_BUFFER_SIZE,
    //    serial_write_buffer, SERIAL_WRITE_BUFFER_SIZE,
    //    9600);
    babbler_serial_setup(
        serial_read_buffer, SERIAL_READ_BUFFER_SIZE,
        serial_write_buffer, SERIAL_WRITE_BUFFER_SIZE,
        BABBLER_SERIAL_SKIP_PORT_INIT);
    
    
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // постоянно следим за последовательным портом, ждем входные данные
    // monitor serial port for input data
    babbler_serial_tasks();
}

