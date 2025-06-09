/**
 * @file curl.c
 * @brief Monitors a log file and periodically sends new data to a remote server using libcurl.
 *
 * This program reads appended data from a log file, buffers it, and sends it to a remote server
 * at regular intervals or when the buffer exceeds a maximum size. It handles log rotation
 * 
 */

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <curl/curl.h>
#include <errno.h>
#include <sys/syslog.h>
#include "../include/logger.h"

/** @brief Maximum in-memory buffer size (10MB) */
#define MAX_BUF_CAPACITY (1024 * 1024 * 10)
/** @brief Interval between sending data (in seconds) */
#define SEND_INTERVAL_SECONDS (5*60)
/** @brief Initial buffer size */
#define INITIAL_BUF_CAPACITY 4096

extern char *url;

static CURL *curl_handle = NULL;
static char *log_buffer = NULL;
static size_t log_buffer_len = 0;
static size_t log_buffer_capacity = INITIAL_BUF_CAPACITY;

/**
 * @struct MemoryStruct
 * @brief Structure for capturing server response.
 */
typedef struct
{
    char *memory;
    size_t size;
} MemoryStruct;

/**
 * @brief Callback for libcurl to write server response.
 * @param contents Pointer to received data.
 * @param size Size of a data chunk.
 * @param nmemb Number of data chunks.
 * @param dataptr Pointer to user data (MemoryStruct).
 * @return Number of bytes handled.
 */
size_t write_callback(void *contents, size_t size, size_t nmemb, void *dataptr)
{
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)dataptr;
    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr)
    {
        syslog(LOG_ERR, "Memory realloc failed in write_callback.");
        return 0;
    }
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

/**
 * @brief Initialize and configure libcurl.
 * @return 1 on success, 0 on failure.
 */
int init_curl(void)
{
    curl_handle = curl_easy_init();
    if (!curl_handle)
    {
        syslog(LOG_ERR, "curl_easy_init() failed.");
        return 0;
    }
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_POST, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 2L);
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 60L);
    curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 10L);
    return 1;
}

/**
 * @brief Send log data to the remote server.
 * @param data_to_send Pointer to data buffer.
 * @param data_len Length of data.
 * @return 1 on success, 0 on failure.
 */
int send_log_data_to_remote(const char *data_to_send, size_t data_len)
{
    if (data_len == 0 || !curl_handle)
        return 1;

    MemoryStruct response_chunk = {.memory = malloc(1), .size = 0};
    if (!response_chunk.memory)
    {
        syslog(LOG_ERR, "Failed to allocate memory for server response.");
        return 0;
    }
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&response_chunk);
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data_to_send);
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, (long)data_len);

    CURLcode curl_res = curl_easy_perform(curl_handle);
    long http_code = 0;
    curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &http_code);

    int success = 0;
    if (curl_res == CURLE_OK && http_code >= 200 && http_code < 300)
    {
        success = 1;
    }
    else
    {
        syslog(LOG_ERR, "curl_easy_perform() failed or server error. HTTP %ld. Response: %s\n",
                http_code, response_chunk.memory);
    }
    free(response_chunk.memory);
    return success;
}

/**
 * @brief Open or create the log file for reading.
 * @return FILE pointer on success, exits on failure.
 */
FILE *open_log_file(void)
{
    FILE *log_file = fopen(LOG_PATH, "r+");
    if (!log_file)
    {
        log_file = fopen(LOG_PATH, "w+");
        if (!log_file)
        {
            fprintf(stderr, "FATAL: Could not create log file '%s'. Exiting.\n", LOG_PATH);
            free(log_buffer);
            if (curl_handle)
                curl_easy_cleanup(curl_handle);
            curl_global_cleanup();
            exit(EXIT_FAILURE);
        }
        fclose(log_file);
        log_file = fopen(LOG_PATH, "r+");
        if (!log_file)
        {
            fprintf(stderr, "FATAL: Failed to reopen log file '%s'. Exiting.\n", LOG_PATH);
            free(log_buffer);
            if (curl_handle)
                curl_easy_cleanup(curl_handle);
            curl_global_cleanup();
            exit(EXIT_FAILURE);
        }
    }
    return log_file;
}

/**
 * @brief Main application entry point.
 * @return Exit status.
 */
void run_uploader()
{

    syslog(LOG_INFO, "Starting uploader process... from uploader : %s",url);
    FILE *logfile = NULL;
    off_t offset_pos = 0;
    time_t last_send_time = time(NULL);

    log_buffer = (char *)malloc(log_buffer_capacity);
    if (!log_buffer)
    {
        fprintf(stderr, "FATAL: Malloc failed for log buffer. Exiting.\n");
        exit(EXIT_FAILURE);
    }
    log_buffer[0] = '\0';

    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK)
    {
        free(log_buffer);
        syslog(LOG_ERR, "curl_global_init() failed.");
        exit(EXIT_FAILURE);
    }
    if (!init_curl())
        exit(EXIT_FAILURE);

    logfile = open_log_file();

    struct stat st;
    if (fstat(fileno(logfile), &st) == 0)
    {
        offset_pos = st.st_size;
        fseek(logfile, offset_pos, SEEK_SET);
    }

    while (1)
    {
        int c;
        while ((c = fgetc(logfile)) != EOF)
        {
            if (log_buffer_len + 1 >= log_buffer_capacity)
            {
                if (log_buffer_len + 1 > MAX_BUF_CAPACITY)
                    goto send_data_label;
                log_buffer_capacity *= 2;
                char *new_buffer = (char *)realloc(log_buffer, log_buffer_capacity);
                if (!new_buffer)
                {
                    syslog(LOG_ERR, "Failed to reallocate log buffer. Attempting to send current buffer.");
                    send_log_data_to_remote(log_buffer, log_buffer_len);
                    log_buffer_len = 0;
                    log_buffer[0] = '\0';
                    log_buffer_capacity = INITIAL_BUF_CAPACITY;
                    continue;
                }
                log_buffer = new_buffer;
            }
            log_buffer[log_buffer_len++] = (char)c;
            log_buffer[log_buffer_len] = '\0';
            offset_pos++;
        }

        if (fstat(fileno(logfile), &st) == 0 && st.st_size < offset_pos)
        {
            fclose(logfile);
            logfile = open_log_file();
            if (!logfile)
                break;
            offset_pos = 0;
        }

        clearerr(logfile); // clean EOF error
        time_t current_time = time(NULL);
        if (current_time - last_send_time >= SEND_INTERVAL_SECONDS)
        {
            last_send_time = current_time;
        send_data_label:
            if (log_buffer_len > 0)
            {

                if (send_log_data_to_remote(log_buffer, log_buffer_len))
                {
                    syslog(LOG_ALERT, "log successfully sent to remote server");
                    log_buffer_len = 0;
                    log_buffer[0] = '\0';
                    // Truncate the log file after successful send
                    int fd = fileno(logfile);
                    if (fd != -1 && ftruncate(fd, 0) == 0)
                    {
                        offset_pos = 0;
                        fseek(logfile, 0, SEEK_SET);
                    }
                    else
                    {
                        syslog(LOG_ERR, "Failed to truncate log file after sending.");
                    }
                }
                else
                {
                    syslog(LOG_ERR, "Failed to send log data to remote server.");
                }
                usleep(500 * 1000);
            }
        }
    }
    if (logfile)
        fclose(logfile);
    if (log_buffer)
        free(log_buffer);
    if (curl_handle)
        curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
}