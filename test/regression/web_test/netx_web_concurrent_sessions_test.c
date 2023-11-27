/* This case tests concurrent HTTPS sessions by launching several
 * HTTPS clients in separate threads and using randomized delay to
 * hit the server with repeated requests. */
#include    "tx_api.h"
#include    "nx_api.h"
#include    "fx_api.h"
#include    "nx_web_http_client.h"
#include    "nx_web_http_server.h"


extern void test_control_return(UINT);

#if !defined(NX_DISABLE_IPV4)

#include "test_device_cert.c"
#include "test_ca_cert.c"
#define ca_cert_der test_ca_cert_der
#define ca_cert_der_len test_ca_cert_der_len

#define     DEMO_STACK_SIZE         4096

/* Set up FileX and file memory resources. */
static CHAR             ram_disk_memory[4096];
static FX_MEDIA         ram_disk;
static UCHAR            media_memory[4096];

static UCHAR            server_stack[16000];

/* Define device drivers.  */
extern void _fx_ram_driver(FX_MEDIA *media_ptr);
extern void _nx_ram_network_driver_1024(NX_IP_DRIVER *driver_req_ptr);

/* Set up the HTTP client global variables. */

#define         CLIENT_PACKET_SIZE  (NX_WEB_HTTP_CLIENT_MIN_PACKET_SIZE * 2)

#define HTTP_CLIENT_THREADS       4
#define HTTP_CONCURRENT_TESTS  10

static TX_THREAD           client_threads[HTTP_CLIENT_THREADS];
static UINT client_thread_index = 0;
static NX_PACKET_POOL      client_pool;
static NX_WEB_HTTP_CLIENT  my_clients[HTTP_CLIENT_THREADS];
static NX_IP               client_ip;
static UINT                error_counter;

/* Set up the HTTP server global variables */

#define         SERVER_WAIT_TICKS   (50)
#define         SERVER_PACKET_SIZE  (NX_WEB_HTTP_SERVER_MIN_PACKET_SIZE * 2)

static NX_WEB_HTTP_SERVER  my_server;
static NX_PACKET_POOL      server_pool;
static TX_THREAD           server_thread;
static NX_IP               server_ip;
static NXD_ADDRESS         server_ip_address;
static UINT                http_server_start = 0;
static UINT                http_client_stop = 0;

static void thread_client_entry(ULONG thread_input);
static void thread_server_entry(ULONG thread_input);

#define HTTP_SERVER_ADDRESS  IP_ADDRESS(1,2,3,4)
#define HTTP_CLIENT_ADDRESS  IP_ADDRESS(1,2,3,5)

#ifdef NX_WEB_HTTPS_ENABLE
static UINT https_server_start = 0;
static UINT https_client_stop = 0;
static UINT loop = 2;
extern const NX_SECURE_TLS_CRYPTO nx_crypto_tls_ciphers;
static CHAR crypto_metadata_server[20000 * NX_WEB_HTTP_SERVER_SESSION_MAX];
static CHAR crypto_metadata_client[HTTP_CLIENT_THREADS][20000];
static UCHAR tls_packet_buffer[NX_WEB_HTTP_SERVER_SESSION_MAX * 18500];
static UCHAR client_packet_buffer[HTTP_CLIENT_THREADS][18500];
static NX_SECURE_X509_CERT certificate;
static NX_SECURE_X509_CERT trusted_certificate;
static NX_SECURE_X509_CERT remote_certificates[HTTP_CLIENT_THREADS], remote_issuers[HTTP_CLIENT_THREADS];
static UCHAR remote_cert_buffers[HTTP_CLIENT_THREADS][2000];
static UCHAR remote_issuer_buffers[HTTP_CLIENT_THREADS][2000];

static UINT tls_setup_callback(NX_WEB_HTTP_CLIENT *client_ptr, NX_SECURE_TLS_SESSION *tls_session);
#else
static UINT loop = 1;
#endif /* NX_WEB_HTTPS_ENABLE  */


#define PRINT_ERROR(error_code)  _error_print(error_code, __LINE__)

static void _error_print(UINT error_code, UINT line)
{
    printf("Error on line %d, status: 0x%x\n", line, error_code);
    error_counter++;
}

static void _print_trace(CHAR *message, ...)
{
#if 0
va_list arg;

   va_start (arg, message);
   printf (message, arg);
   va_end (arg);
#endif

}

static UINT  authentication_check(NX_WEB_HTTP_SERVER *server_ptr, UINT request_type, 
                                  CHAR *resource, CHAR **name, CHAR **password, CHAR **realm);
static UINT server_request_callback(NX_WEB_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr);

#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void    netx_web_concurrent_sessions_test_application_define(void *first_unused_memory)
#endif
{
CHAR    *pointer;
CHAR thread_name[100];
UINT i;
UINT    status;


    error_counter = 0;

    /* Setup the working pointer.  */
    pointer =  (CHAR *) first_unused_memory;

    /* Create a helper thread for the server. */
    tx_thread_create(&server_thread, "HTTP Server thread", thread_server_entry, 0,  
                     pointer, DEMO_STACK_SIZE, 
                     NX_WEB_HTTP_SERVER_PRIORITY, NX_WEB_HTTP_SERVER_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);

    pointer =  pointer + DEMO_STACK_SIZE;

    /* Initialize the NetX system.  */
    nx_system_initialize();

    /* Create the server packet pool.  */
    status =  nx_packet_pool_create(&server_pool, "HTTP Server Packet Pool", SERVER_PACKET_SIZE, 
                                    pointer, SERVER_PACKET_SIZE*64);
    pointer = pointer + SERVER_PACKET_SIZE * 64;
    if (status)
        PRINT_ERROR(status);

    /* Create an IP instance.  */
    status = nx_ip_create(&server_ip, "HTTP Server IP", HTTP_SERVER_ADDRESS, 
                          0xFFFFFF00UL, &server_pool, _nx_ram_network_driver_1024,
                          pointer, 4096, 1);
    pointer =  pointer + 4096;
    if (status)
        PRINT_ERROR(status);

    /* Enable ARP and supply ARP cache memory for the server IP instance.  */
    status = nx_arp_enable(&server_ip, (void *) pointer, 1024);
    pointer = pointer + 1024;
    if (status)
        PRINT_ERROR(status);


     /* Enable TCP traffic.  */
    status = nx_tcp_enable(&server_ip);
    if (status)
        PRINT_ERROR(status);

    /* Create the HTTP Client thread. */
    for(i = 0; i < HTTP_CLIENT_THREADS; ++i)
    {
        snprintf(thread_name, 100, "HTTP Client %d", i);
        status = tx_thread_create(&client_threads[i], thread_name, thread_client_entry, i,
                                  pointer, DEMO_STACK_SIZE,
                                  NX_WEB_HTTP_SERVER_PRIORITY + 2, NX_WEB_HTTP_SERVER_PRIORITY + 2, TX_NO_TIME_SLICE, TX_AUTO_START);
        pointer =  pointer + DEMO_STACK_SIZE;
    }
    if (status)
        PRINT_ERROR(status);

    /* Create the Client packet pool.  */
    status =  nx_packet_pool_create(&client_pool, "HTTP Client Packet Pool", CLIENT_PACKET_SIZE, 
                                    pointer, CLIENT_PACKET_SIZE*16);
    pointer = pointer + CLIENT_PACKET_SIZE * 16;
    if (status)
        PRINT_ERROR(status);

    /* Create an IP instance.  */
    status = nx_ip_create(&client_ip, "HTTP Client IP", HTTP_CLIENT_ADDRESS, 
                          0xFFFFFF00UL, &client_pool, _nx_ram_network_driver_1024,
                          pointer, 2048, 1);
    pointer =  pointer + 2048;
    if (status)
        PRINT_ERROR(status);

    status  = nx_arp_enable(&client_ip, (void *) pointer, 1024);
    pointer =  pointer + 2048;
    if (status)
        PRINT_ERROR(status);

     /* Enable TCP traffic.  */
    status = nx_tcp_enable(&client_ip);
    if (status)
        PRINT_ERROR(status);
}

#ifdef NX_WEB_HTTPS_ENABLE
/* Define the TLS setup callback function.  */
static UINT tls_setup_callback(NX_WEB_HTTP_CLIENT *client_ptr, NX_SECURE_TLS_SESSION *tls_session)
{
UINT status;

    _print_trace("Setting up tls for thread: %s\n", client_ptr->nx_web_http_client_name);

    /* Use counter from client name (set up when client is created. */
    client_thread_index  = client_ptr->nx_web_http_client_name[0] - '0';

    /* Initialize and create TLS session.  */
    status = nx_secure_tls_session_create(tls_session, &nx_crypto_tls_ciphers, crypto_metadata_client[client_thread_index], sizeof(crypto_metadata_client[0]));
    
    /* Check status.  */
    if (status)
    {
        return(status);
    }

    /* Allocate space for packet reassembly.  */
    status = nx_secure_tls_session_packet_buffer_set(tls_session, client_packet_buffer[client_thread_index], sizeof(client_packet_buffer[0]));

    /* Check status.  */
    if (status)
    {
        return(status);
    }

    /* Add a CA Certificate to our trusted store for verifying incoming server certificates.  */
    nx_secure_x509_certificate_initialize(&trusted_certificate, ca_cert_der, ca_cert_der_len, NX_NULL, 0, NULL, 0, NX_SECURE_X509_KEY_TYPE_NONE);
    nx_secure_tls_trusted_certificate_add(tls_session, &trusted_certificate);

    /* Need to allocate space for the certificate coming in from the remote host.  */
    nx_secure_tls_remote_certificate_allocate(tls_session, &remote_certificates[client_thread_index], remote_cert_buffers[client_thread_index], sizeof(remote_cert_buffers[0]));
    nx_secure_tls_remote_certificate_allocate(tls_session, &remote_issuers[client_thread_index], remote_issuer_buffers[client_thread_index], sizeof(remote_issuer_buffers[0]));

    /* Increment global counter. */
    client_thread_index++;

    return(NX_SUCCESS);
}
#endif /* NX_WEB_HTTPS_ENABLE  */

static VOID http_response_callback(NX_WEB_HTTP_CLIENT *client_ptr, CHAR *field_name, UINT field_name_length,
                                   CHAR *field_value, UINT field_value_length)
{
    if (memcmp(field_name, "Content-Type", field_name_length) == 0)
    {
        if (memcmp(field_value, "text/plain", field_value_length) != 0)
            PRINT_ERROR(1);
    }
    else if(memcmp(field_name, "Content-Length", field_name_length) == 0)
    {
        if (memcmp(field_value, "12", field_value_length) != 0)
            PRINT_ERROR(1);
    }
    else if(memcmp(field_name, "Connection", field_name_length) == 0)
    {
        if (memcmp(field_value, "keep-alive", field_value_length) != 0)
            PRINT_ERROR(1);
    }
}

void thread_client_entry(ULONG thread_input)
{
UINT            i;
UINT            status;
UINT            thread_index;
UCHAR           client_name[100];
NX_PACKET       *recv_packet;
NX_WEB_HTTP_CLIENT *my_client;
UINT num_tests;

    /* Give IP task and driver a chance to initialize the system.  */
    tx_thread_sleep(NX_IP_PERIODIC_RATE);

    /* Get the index of this thread. */
    thread_index = (UINT)(thread_input);

    /* Get the client for this thread. */
    my_client = &my_clients[thread_index];

    /* Set server IP address.  */
    server_ip_address.nxd_ip_address.v4 = HTTP_SERVER_ADDRESS;
    server_ip_address.nxd_ip_version = NX_IP_VERSION_V4;

    /* First loop test HTTP, second loop test HTTPS.  */
    for (i = 1; i < loop ; i++)
    {
        _print_trace("Starting HTTP Client %d\n", thread_index);
        for(num_tests = 0; num_tests < HTTP_CONCURRENT_TESTS; ++num_tests)
        {
            if (i == 0)
            {

                /* Wait HTTP server started.  */
                while(!http_server_start)
                {
                    tx_thread_sleep(NX_IP_PERIODIC_RATE);
                }
            }
#ifdef NX_WEB_HTTPS_ENABLE
            else
            {

                /* Wait HTTPS server started.  */
                while(!https_server_start)
                {
                    tx_thread_sleep(NX_IP_PERIODIC_RATE);
                }
            }
#endif /* NX_WEB_HTTPS_ENABLE  */

            _print_trace("Starting HTTP Client %d, test number: %d\n", thread_index, num_tests);

            /* Random start between 10ms and 1s so we hit during different states. */
            tx_thread_sleep((rand() % 10) * 10);



            /* Create an HTTP client instance.  */
            snprintf(client_name, 100, "%d HTTP Client", thread_index);
            status = nx_web_http_client_create(my_client, client_name, &client_ip, &client_pool, 1536);

            /* Check status.  */
            if (status)
                PRINT_ERROR(status);

            /* Set the header callback routine. */
            nx_web_http_client_response_header_callback_set(my_client, http_response_callback);

            /* Send a GET request.  */
            if (i == 0)
            {
                status = nx_web_http_client_get_start(my_client, &server_ip_address,
                                                      NX_WEB_HTTP_SERVER_PORT, "http://1.2.3.4/test.txt",
                                                      "1.2.3.4", "name", "password", NX_WAIT_FOREVER);
            }
#ifdef NX_WEB_HTTPS_ENABLE
            else
            {
                _print_trace("Starting secure get\n");
                status = nx_web_http_client_get_secure_start(my_client, &server_ip_address,
                                                             NX_WEB_HTTPS_SERVER_PORT, "https://1.2.3.4/test.txt",
                                                             "1.2.3.4", "name", "password",
                                                             tls_setup_callback, NX_WAIT_FOREVER);
            }
#endif /* NX_WEB_HTTPS_ENABLE  */

            /* Check status.  */
            if (status)
            {
                _print_trace("Error in connecting to server. Status: 0x%x, Thread index: %d\n", status, thread_index);
                PRINT_ERROR(status);
            }

            /* Get response from server.  */
            status = nx_web_http_client_response_body_get(my_client, &recv_packet, 1 * NX_IP_PERIODIC_RATE);

            /* Check status.  */
            if (status != NX_WEB_HTTP_GET_DONE)
            {
                _print_trace("Error in getting response body (check connection). Status: 0x%x, Thread: %d\n", status, thread_index);
                PRINT_ERROR(status);
            }
            else
                nx_packet_release(recv_packet);

            status = nx_web_http_client_delete(my_client);
            if (status)
                PRINT_ERROR(status);

        }

        _print_trace("Ending HTTP Client %d\n", thread_index);

        /* Set the flag.  */
        if (i == 0)
        {
            http_client_stop++;
        }
#ifdef NX_WEB_HTTPS_ENABLE
        else
        {
            https_client_stop++;
        }
#endif

    }
}

/* Define the helper HTTP server thread.  */
void    thread_server_entry(ULONG thread_input)
{
UINT            i;
UINT            status;
FX_FILE         my_file;
UINT            server_port = NX_WEB_HTTP_SERVER_PORT;


    /* Print out test information banner.  */
    printf("NetX Test:   Web Concurrent Sessions Test..............................");

    /* Check for earlier error.  */
    if(error_counter)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    fx_media_format(&ram_disk,
                    _fx_ram_driver,               // Driver entry
                    ram_disk_memory,              // RAM disk memory pointer
                    media_memory,                 // Media buffer pointer
                    sizeof(media_memory),         // Media buffer size
                    "MY_RAM_DISK",                // Volume Name
                    1,                            // Number of FATs
                    32,                           // Directory Entries
                    0,                            // Hidden sectors
                    256,                          // Total sectors
                    512,                          // Sector size
                    8,                            // Sectors per cluster
                    1,                            // Heads
                    1);                           // Sectors per track
    
    /* Open the RAM disk.  */
    status = fx_media_open(&ram_disk, "RAM DISK", _fx_ram_driver, ram_disk_memory, media_memory, sizeof(media_memory)) ;
    status += fx_file_create(&ram_disk, "TEST.TXT");
    status += fx_file_open(&ram_disk, &my_file, "TEST.TXT", FX_OPEN_FOR_WRITE);
    status += fx_file_write(&my_file, "https server", 12);
    status += fx_file_close(&my_file);
    if(status)
        PRINT_ERROR(status);

    /* Give NetX a chance to initialize the system.  */
    tx_thread_sleep(NX_IP_PERIODIC_RATE);

    /* First loop test HTTP, second loop test HTTPS.  */
    for (i = 1; i < loop; i++)
    {

        if (i == 1)
        {
            server_port = NX_WEB_HTTPS_SERVER_PORT;
        }

        _print_trace("\nStarting server\n");

        /* Create the HTTP Server. */
        status = nx_web_http_server_create(&my_server, "My HTTP Server", &server_ip, server_port, &ram_disk,
                                           &server_stack, sizeof(server_stack), &server_pool,
                                           authentication_check, server_request_callback);
        if (status)
            PRINT_ERROR(status);

#ifdef NX_WEB_HTTPS_ENABLE
        /* Set TLS for HTTPS.  */
        if (i == 1)
        {
            /* Initialize device certificate (used for all sessions in HTTPS server).  */
            memset(&certificate, 0, sizeof(certificate));
            nx_secure_x509_certificate_initialize(&certificate, test_device_cert_der, test_device_cert_der_len, NX_NULL, 0, test_device_cert_key_der, test_device_cert_key_der_len, NX_SECURE_X509_KEY_TYPE_RSA_PKCS1_DER);

            /* Setup TLS session data for the TCP server.  */
            status = nx_web_http_server_secure_configure(&my_server, &nx_crypto_tls_ciphers,
                                                         crypto_metadata_server, sizeof(crypto_metadata_server), tls_packet_buffer, sizeof(tls_packet_buffer),
                                                         &certificate, NX_NULL, 0, NX_NULL, 0, NX_NULL, 0);
            if (status)
                PRINT_ERROR(status);
        }
#endif /* NX_WEB_HTTPS_ENABLE  */

        /* OK to start the HTTP Server.  */
        status = nx_web_http_server_start(&my_server);
        if (status)
            PRINT_ERROR(status);

        _print_trace("...Server started\n");

        /* Set the flag.  */
        if (i == 0)
        {
            http_server_start = 1;

            /* Wait HTTP test finished.  */
            while(http_client_stop < HTTP_CLIENT_THREADS)
            {
                tx_thread_sleep(NX_IP_PERIODIC_RATE);
            }
        }
#ifdef NX_WEB_HTTPS_ENABLE
        else
        {
            https_server_start = 1;

            /* Wait HTTPS test finished.  */
            while(https_client_stop < HTTP_CLIENT_THREADS)
            {
                tx_thread_sleep(NX_IP_PERIODIC_RATE);
            }
        }
#endif /* NX_WEB_HTTPS_ENABLE  */

        _print_trace("Stopping server...\n");

        status = nx_web_http_server_stop(&my_server);
        if (status)
            PRINT_ERROR(status);

        status = nx_web_http_server_delete(&my_server);
        if (status)
            PRINT_ERROR(status);
    }

    if(error_counter)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }
    else
    {
        printf("SUCCESS!\n");
        test_control_return(0);
    }
}

/* Define the application's authentication check.  This is called by
   the HTTP server whenever a new request is received.  */
static UINT  authentication_check(NX_WEB_HTTP_SERVER *server_ptr, UINT request_type, 
                                  CHAR *resource, CHAR **name, CHAR **password, CHAR **realm)
{

    /* Just use a simple name, password, and realm for all 
       requests and resources.  */
    *name =     "name";
    *password = "password";
    *realm =    "NetX Duo HTTP demo";

    /* Request basic authentication.  */
    return(NX_WEB_HTTP_BASIC_AUTHENTICATE);
}

/* Define the server request callback function.  */
static UINT server_request_callback(NX_WEB_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr)
{
ULONG        offset, length;
NX_PACKET   *response_pkt;
UCHAR        buffer[4000];
UINT         status;
CHAR         response[] = "Test response from server - uploaded file contents:\n";


    memset(buffer, 0, sizeof(buffer));
    length = 0;

    _print_trace("***************** Server recevied request..\n");

    /* Process multipart data.  */
    if(request_type == NX_WEB_HTTP_SERVER_POST_REQUEST)
    {

        /* Get the content header.  */
        while(nx_web_http_server_get_entity_header(server_ptr, &packet_ptr, buffer,
                                                   sizeof(buffer)) == NX_SUCCESS)
        {

            /* Header obtained successfully. Get the content data location.  */
            while(nx_web_http_server_get_entity_content(server_ptr, &packet_ptr, &offset,
                                                        &length) == NX_SUCCESS)
            {
                /* make sure we don't overwrite our buffer!  */
                if(length > sizeof(buffer))
                {
                    length = sizeof(buffer);
                }
              
                /* Write content data to buffer.  */
                nx_packet_data_extract_offset(packet_ptr, offset, buffer, length, 
                                              &length);
                buffer[length] = 0;
            }
        }

        /* Generate HTTP header.  */
        status = nx_web_http_server_callback_generate_response_header(server_ptr,
                                                                      &response_pkt, NX_WEB_HTTP_STATUS_OK, 800, "text/html",
                                                                      "Server: NetX HTTPS Experimental\r\n");

        if(status)
        {
            return(status); 
        }

        status = nx_packet_data_append(response_pkt, response, strlen(response), 
                                       &server_pool, SERVER_WAIT_TICKS);

        if(length > 0)
        {
            /* Only send what is in the buffer.  */
            if(length > 4000)
            {
              length = 4000;
            }
            status = nx_packet_data_append(response_pkt, buffer, length, 
                                           &server_pool, SERVER_WAIT_TICKS);
        }

        if(status == NX_SUCCESS)
        {
            if(nx_web_http_server_callback_packet_send(server_ptr, response_pkt) !=  NX_SUCCESS)
            {
                nx_packet_release(response_pkt);
            }
        }
    }
    else
    {
        /* Indicate we have not processed the response to client yet.  */
        return(NX_SUCCESS);
    }

    /* Indicate the response to client is transmitted.  */
    return(NX_WEB_HTTP_CALLBACK_COMPLETED);
}
#else

#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void    netx_web_basic_test_application_define(void *first_unused_memory)
#endif
{

    /* Print out test information banner.  */
    printf("NetX Test:   Web Basic Test............................................N/A\n"); 

    test_control_return(3);  
}      
#endif

