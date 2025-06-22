// safe guard
#ifndef KEY_TRACER
#define KEY_TRACER
#define MAX_SIZE 512


typedef struct
{
    int shift_active;
    int caps_lock_on;
    // add more modifiers if needed
}modifier_state_t;



void get_handler(char *pdevice,char *phandler);

void connect_handler(char *handler);


#endif