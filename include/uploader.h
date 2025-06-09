#ifndef UPLOADER_H
#define UPLOADER_H

size_t write_callback(void *contents, size_t size, size_t nmemb, void *dataptr);
int init_curl(void);
void run_uploader(void);
FILE *open_log_file(void);
void send_log_data_to_remote(const char *data, size_t len);
void free_memory(void);

#endif

