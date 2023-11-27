/* Testing the multiple interface DHCP Client */

/* This is the DHCP Client that will run on both interfaces independently.
   Required: NX_MAX_PHYSICAL_INTERFACES = 2 and NX_DHCP_CLIENT_MAX_INTERFACES = 1.
   There are two DHCP Client threads that are switching from manual to DHCP CLient address
   mode, and switch from activated to deactivated. There is one server thread checking
   for DHCP Client messages on primary and secondary interfaces. 
*/



#include    "tx_api.h"
#include    "nx_api.h"
extern   void   test_control_return(UINT);
#ifndef NX_DISABLE_IPV4
#include    "nx_arp.h"
#include    "nx_ram_network_driver_test_1500.h"
#ifdef __PRODUCT_NETXDUO__
#include    "nxd_dhcp_client.h"
#else
#include    "nx_dhcp.h"
#endif

#define     DEMO_STACK_SIZE         4096
#define     PACKET_PAYLOAD          1518

                  

/* Define the ThreadX, NetX object control blocks...  */

static NX_UDP_SOCKET           server0_socket;
static TX_THREAD               client0_thread;
static TX_THREAD               client1_thread;
static TX_THREAD               server0_thread;
static NX_PACKET_POOL          client_pool;
static NX_PACKET_POOL          server_pool;
static NX_IP                   client_ip;
static NX_IP                   server0_ip;

static UINT                    probe0 = NX_FALSE;
static UINT                    probe1 = NX_FALSE;
static UINT                    bound0 = NX_FALSE;
static UINT                    bound1 = NX_FALSE;

/* Define the NetX FTP object control block.  */
static NX_DHCP                dhcp_client;
static ULONG                  dhcp_xid;


typedef struct DHCP_RESPONSE_STRUCT
{
    char          *dhcp_response_pkt_data;
    int           dhcp_response_pkt_size;
} DHCP_RESPONSE;

#define NUM_RESPONSES      2
static  DHCP_RESPONSE      dhcp_response0[NUM_RESPONSES];
static  DHCP_RESPONSE      dhcp_response1[NUM_RESPONSES];

/* Define the counters used in the demo application...  */

static  UINT            error_counter = 0;
static  UINT            client0_running = NX_TRUE;
static  UINT            client1_running = NX_TRUE;


#define SERVER_PORT      67


/* Replace the 'ram' driver with your Ethernet driver. */
extern  VOID nx_driver_ram_driver(NX_IP_DRIVER*); 

static void    server0_thread_entry(ULONG thread_input);
static void    client0_thread_entry(ULONG thread_input);
static void    client1_thread_entry(ULONG thread_input);

static  UINT   nx_dhcp_response_packet_send(NX_UDP_SOCKET *server_socket, UINT port, INT packet_number, UINT iface_index);
static  void   dhcp_test_initialize();

extern   void _nx_ram_network_driver_1024(NX_IP_DRIVER *driver_req_ptr);
static  UINT   my_dhcp_process_bc_callback(NX_IP *ip_ptr, NX_PACKET *packet_ptr, UINT *operation_ptr, UINT *delay_ptr);
extern  UINT  (*advanced_packet_process_callback)(NX_IP *ip_ptr, NX_PACKET *packet_ptr, UINT *operation_ptr, UINT *delay_ptr);
static  ULONG  dhcp_get_dhcp_data(UCHAR *data, UINT size);
static  VOID dhcp_interface_state_change(NX_DHCP *dhcp_ptr, UINT iface_index, UCHAR new_state);


/* Note that the network is 192.2.2.0 and the MAC address is 11 22 33 44 56
   because there are four entities (server 2 interfaces, client 2 interfaces
   and the ram driver increases the MAC sequentially starting from
   11 22 33 44 56. */


static char offer_response0[300] = {

0x02, 0x01, 0x06, 0x00, 0x13, 0xae, 0x1c, 0xeb, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0xc0, 0x02, 0x02, 0xf7, 0xc0, 0x02, /* ........ */
0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, /* ........ */
0x22, 0x33, 0x44, 0x58, 0x00, 0x00, 0x00, 0x00, /* T....... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x82, /* ......c. */
0x53, 0x63, 0x35, 0x01, 0x02, 0x01, 0x04, 0xff, /* Sc5..... */
0xff, 0xff, 0x00, 0x3a, 0x04, 0x00, 0x06, 0xac, /* ...:.... */
0x98, 0x3b, 0x04, 0x00, 0x0b, 0xae, 0x0a, 0x33, /* .;.....3 */
0x04, 0x00, 0x0d, 0x59, 0x30, 0x36, 0x04, 0xc0, /* ...Y06.. */
0x02, 0x02, 0x01, 0x03, 0x04, 0xc0, 0x02, 0x02, /* ........ */
0x01, 0x06, 0x04, 0xc0, 0x02, 0x02, 0x01, 0xff, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00              /* ...... */
};

static int offer_response0_size = 300;

/* Frame (342 bytes) */
static char ack_response0[300] = {

0x02, 0x01, 0x06, 0x00, 0x13, 0xae, 0x1c, 0xeb, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0xc0, 0x02, 0x02, 0xf7, 0xc0, 0x02, /* ........ */
0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, /* ........ */
0x22, 0x33, 0x44, 0x58, 0x00, 0x00, 0x00, 0x00, /* T....... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x82, /* ......c. */
0x53, 0x63, 0x35, 0x01, 0x05, 0x3a, 0x04, 0x00, /* Sc5..:.. */
0x06, 0xac, 0x98, 0x3b, 0x04, 0x00, 0x0b, 0xae, /* ...;.... */
0x0a, 0x33, 0x04, 0x00, 0x0d, 0x59, 0x30, 0x36, /* .3...Y06 */
0x04, 0xc0, 0x02, 0x02, 0x01, 0x01, 0x04, 0xff, /* ........ */
0xff, 0xff, 0x00, 0x03, 0x04, 0xc0, 0x02, 0x02, /* ........ */
0x01, 0x06, 0x04, 0xc0, 0x02, 0x02, 0x01, 0xff, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00              /* ...... */
};

static int ack_response0_size = 300;

static char offer_response1[300] = {

#ifdef __PRODUCT_NETXDUO__
0x02, 0x01, 0x06, 0x00, 0x2a, 0x3e, 0xF0, 0x1D, 
#else
0x02, 0x01, 0x06, 0x00, 0x08, 0x0d, 0xB4, 0x55,
#endif
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0xc0, 0x01, 0x01, 0xF7, 0xc0, 0x01, /* ........ */
0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, /* ........ */
0x22, 0x33, 0x44, 0x59, 0x00, 0x00, 0x00, 0x00, /* T....... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x82, /* ......c. */
0x53, 0x63, 0x35, 0x01, 0x02, 0x01, 0x04, 0xff, /* Sc5..... */
0xff, 0xff, 0x00, 0x3a, 0x04, 0x00, 0x06, 0xac, /* ...:.... */
0x98, 0x3b, 0x04, 0x00, 0x0b, 0xae, 0x0a, 0x33, /* .;.....3 */
0x04, 0x00, 0x0d, 0x59, 0x30, 0x36, 0x04, 0xc0, /* ...Y06.. */
0x01, 0x01, 0x01, 0x03, 0x04, 0xc0, 0x01, 0x01, /* ........ */
0x01, 0x06, 0x04, 0xc0, 0x01, 0x01, 0x01, 0xff, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00              /* ...... */
};

static int offer_response1_size = 300;

/* Frame (342 bytes) */
static char ack_response1[300] = {

#ifdef __PRODUCT_NETXDUO__
0x02, 0x01, 0x06, 0x00, 0x2a, 0x3e, 0xF0, 0x1D, 
#else
0x02, 0x01, 0x06, 0x00, 0x08, 0x0d, 0xB4, 0x55,
#endif
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0xc0, 0x01, 0x01, 0xf7, 0xc0, 0x01, /* ........ */
0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, /* ........ */
0x22, 0x33, 0x44, 0x59, 0x00, 0x00, 0x00, 0x00, /* T....... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x82, /* ......c. */
0x53, 0x63, 0x35, 0x01, 0x05, 0x3a, 0x04, 0x00, /* Sc5..:.. */
0x06, 0xac, 0x98, 0x3b, 0x04, 0x00, 0x0b, 0xae, /* ...;.... */
0x0a, 0x33, 0x04, 0x00, 0x0d, 0x59, 0x30, 0x36, /* .3...Y06 */
0x04, 0xc0, 0x01, 0x01, 0x01, 0x01, 0x04, 0xff, /* ........ */
0xff, 0xff, 0x00, 0x03, 0x04, 0xc0, 0x01, 0x01, /* ........ */
0x01, 0x06, 0x04, 0xc0, 0x01, 0x01, 0x01, 0xff, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00              /* ...... */
};

static int ack_response1_size = 300;

/* Define what the initial system looks like.  */

#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void    netx_dhcp_client_arpprobe_multiple_interface_test_application_define(void *first_unused_memory)
#endif
{

UINT    status;
UCHAR   *pointer;

    
    /* Setup the working pointer.  */
    pointer =  (UCHAR *) first_unused_memory;

    /* Initialize NetX.  */
    nx_system_initialize();

    /* Set up the DHCP Server. */

    /* Create the main server thread.  */
    status = tx_thread_create(&server0_thread, "Server 0 thread ", server0_thread_entry, 0,  
                              pointer, DEMO_STACK_SIZE, 
                              6, 6, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer = pointer + DEMO_STACK_SIZE ;

    /* Check status.  */
    if (status != NX_SUCCESS)
    {
        error_counter++;
    }

    /* Create the server packet pool.  */
    status =  nx_packet_pool_create(&server_pool, "Server Packet Pool", 700, pointer , 700*10);

    pointer = pointer + 700*10;
    if (status)
        error_counter++;

    /* Create an IP instance.  */
    status = nx_ip_create(&server0_ip, 
                          "Server 0 IP", 
                          IP_ADDRESS(192,2,2,1), 
                          0xFFFFFF00UL, 
                          &server_pool, _nx_ram_network_driver_1024,
                          pointer, DEMO_STACK_SIZE, 1);

    pointer = pointer + DEMO_STACK_SIZE;
    
    if (status)
        error_counter++;

    status = nx_ip_interface_attach(&server0_ip, "Server IP", IP_ADDRESS(192,1,1,1), 0xFFFFFF00UL, _nx_ram_network_driver_1024);
    /* Check status.  */
    if (status != NX_SUCCESS)
    {
        error_counter++;
    }

    pointer = pointer + DEMO_STACK_SIZE;
    
    if (status)
        error_counter++;

    /* Enable ARP and supply ARP cache memory for the server IP instance.  */
    status =  nx_arp_enable(&server0_ip, (void *) pointer, 1024);
    pointer = pointer + 1024;
    if (status)
        error_counter++;

     /* Enable UDP traffic.  */
    status = nx_udp_enable(&server0_ip);

    if (status)
        error_counter++;

    /* Set up the Client. */

    /* Create the main client thread.  */
    status = tx_thread_create(&client0_thread, "Client 0", client0_thread_entry, 0,  
                              pointer, DEMO_STACK_SIZE, 
                              6, 6, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer = pointer + DEMO_STACK_SIZE ;

    /* Check status.  */
    if (status != NX_SUCCESS)
    {
        error_counter++;
    }

    /* Create the secondary client thread.  */
    status = tx_thread_create(&client1_thread, "Client 1", client1_thread_entry, 0,  
                              pointer, DEMO_STACK_SIZE, 
                              6, 6, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer = pointer + DEMO_STACK_SIZE ;

    /* Check status.  */
    if (status != NX_SUCCESS)
    {
        error_counter++;
    }

    /* Create a packet pool for the client.  */
    status =  nx_packet_pool_create(&client_pool, "Client Packet Pool", PACKET_PAYLOAD, pointer, 12*PACKET_PAYLOAD);
    
        /* Check status.  */
    if (status != NX_SUCCESS)
    {
        error_counter++;
    }
    
    pointer =  pointer + 12*PACKET_PAYLOAD;

    /* Create an IP instance for the client.  */
    status = nx_ip_create(&client_ip, "Client 0 IP", IP_ADDRESS(0,0,0,0), 0xFFFFFF00UL, 
                          &client_pool, _nx_ram_network_driver_1024, pointer, 2048, 1);
    
    /* Check status.  */
    if (status != NX_SUCCESS)
    {
        error_counter++;
    }
    
    pointer = pointer + 2048;

    status = nx_ip_interface_attach(&client_ip, "Client 1 IP", IP_ADDRESS(0,0,0,0), 0xFFFFFF00UL, _nx_ram_network_driver_1024);
    /* Check status.  */
    if (status != NX_SUCCESS)
    {
        error_counter++;
    }

    /* Enable ARP and supply ARP cache memory for the Client IP.  */
    nx_arp_enable(&client_ip, (void *) pointer, 1024);

    pointer = pointer + 1024;

    /* Enable UDP for client IP instance.  */
    nx_udp_enable(&client_ip);
    nx_icmp_enable(&client_ip);
    
    /* Load up the server 'responses'. */
    dhcp_test_initialize();

    /* Create the DHCP instance.  */
    status =  nx_dhcp_create(&dhcp_client, &client_ip, "dhcp0");

    if (status)
        error_counter++;
    
    /* Keep track of interface DHCP Client state changes. */
    status = nx_dhcp_interface_state_change_notify(&dhcp_client, dhcp_interface_state_change);

    if (status)
        error_counter++;

    return;
}


/* Define the DHCP client thread.  */

void    client0_thread_entry(ULONG thread_input)
{

UINT        status;
UINT        time_keeper;


    tx_thread_sleep(10);
    
    /* Start DHCP on all enabled interfaces (which at the moment is just the primary interface). */
    status =  nx_dhcp_interface_start(&dhcp_client, 0);
    if (status != NX_SUCCESS)
    {
        error_counter++;
    }

    if (dhcp_client.nx_dhcp_interface_record[0].nx_dhcp_state == NX_DHCP_STATE_NOT_STARTED)
    {
        error_counter++;
    }

    /* Wait for the Client to bind an ARP adddress, including the ARP probe test. */
    time_keeper = 0;

    while(dhcp_client.nx_dhcp_interface_record[0].nx_dhcp_state != NX_DHCP_STATE_BOUND)
    {
        tx_thread_sleep(10);
        time_keeper += 10;
        if (time_keeper > 1000)
        {
           break;
        }
    }
    
    if ((probe0 != NX_TRUE) || (bound0 != NX_TRUE))
    {
        error_counter++;
    }

    /* Now stop the DHCP Client 0. */
    nx_dhcp_interface_disable(&dhcp_client, 0);

    /* Verify the DHCP CLient is not enabled on the primary interface. */
    if (dhcp_client.nx_dhcp_interface_record[0].nx_dhcp_state != NX_DHCP_STATE_NOT_STARTED)
    {
        error_counter++;
    }    
        
    if (client_ip.nx_ip_interface[0].nx_interface_ip_address != 0x0)
    {
        error_counter++;
    }
    client0_running = NX_FALSE;

}

void    client1_thread_entry(ULONG thread_input)
{

UINT status;


    tx_thread_sleep(10);

    if (NX_DHCP_CLIENT_MAX_RECORDS != 2) 
    {
        error_counter++;
    }
    
    status = nx_dhcp_interface_enable(&dhcp_client, 1);
    if (status != NX_SUCCESS) 
    {

        error_counter++;
    }   

    status = nx_dhcp_interface_start(&dhcp_client, 1);

    if (status != NX_SUCCESS) 
    {
        error_counter++;
    }
    else
    {  
      
      UINT time_keeper = 0;
      while(dhcp_client.nx_dhcp_interface_record[1].nx_dhcp_state != NX_DHCP_STATE_BOUND)
      {
          tx_thread_sleep(20);
          time_keeper += 20;
          if (time_keeper > 1000)
          {
              break;
          }
      }
    
      if ((probe1 != NX_TRUE) || (bound1 != NX_TRUE))
      {
          error_counter++;
      }      

      /* Now disable DHCP Client 1 activity. This reinitializes the Client too. */
      status  = nx_dhcp_interface_disable(&dhcp_client, 1);
      if (status)
      {
        error_counter++;
      }    
    }
    
    if (dhcp_client.nx_dhcp_interface_record[1].nx_dhcp_state != NX_DHCP_STATE_NOT_STARTED)
    {
        error_counter++;
    }
    
    if (client_ip.nx_ip_interface[1].nx_interface_ip_address != 0x0)
    {
        error_counter++;
    }
    client1_running = NX_FALSE;
}
                           
/* Define the Primary DHCP server thread.  */
void    server0_thread_entry(ULONG thread_input)
{

UINT         status;
NX_PACKET   *my_packet;
UINT         i, j, k, pkt_number;

    /* Print out test information banner.  */
    printf("NetX Test:   DHCP Client Multiple Interface ARP Probe Test............."); 

    /* Check for earlier error. */
    if(error_counter)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Create a  socket as the  server.  */
    status = nx_udp_socket_create(&server0_ip, &server0_socket, "Socket 0 Server", NX_IP_NORMAL, NX_FRAGMENT_OKAY,  NX_IP_TIME_TO_LIVE, 5);

    /* Check status.  */
    if (status)
    {
        error_counter++;
    }
    
    status =  nx_udp_socket_bind(&server0_socket, NX_DHCP_SERVER_UDP_PORT, TX_WAIT_FOREVER);

    /* Check status.  */
    if (status)
    {
        error_counter++;
    }

    i = 0;
    j = 0;
    k = 0;
    pkt_number = 0;

    /* Set the advanced callback to ensure broadcast packets are routed correctly. */
    advanced_packet_process_callback = my_dhcp_process_bc_callback;

    /* Wait for Client requests */
    while ( i < (2*NUM_RESPONSES))
    {
       
        status =  nx_udp_socket_receive(&server0_socket, &my_packet, 10 * NX_IP_PERIODIC_RATE);

        /* Check status.  */
        if (status)
        {
            error_counter++;
        } 
        else
        {

            UINT source_port;
            ULONG source_ip_address;
            UINT protocol;
            UINT iface_index;
            
            /* Get the XID of DHCP message from DHCP Server.  */    
            dhcp_xid = dhcp_get_dhcp_data(my_packet -> nx_packet_prepend_ptr + NX_BOOTP_OFFSET_XID, 4);

            nx_udp_packet_info_extract(my_packet, &source_ip_address, &protocol, &source_port, &iface_index);

            /* Release the packet.  */
            nx_packet_release(my_packet);

           if (iface_index == 0)
           {
              pkt_number = j;

           }
           else
           {
              pkt_number = k;

           }

           status = nx_dhcp_response_packet_send(&server0_socket, 68, pkt_number, iface_index);
            
           if (iface_index == 0)
              j++;
           else
              k++;
           
           /* Check status.  */
          if (status)
          {
             error_counter++;
          }        
        }

        /* Advance the index for the next response. */
        i++;
    } 

    /* Wait for the client to terminate the connection. */
    while((client0_running == NX_TRUE) || (client1_running == NX_TRUE))
      tx_thread_sleep(20);

    /* Delete the UDP socket.  */
    nx_udp_socket_delete(&server0_socket);

    if(error_counter)
    {

        printf("ERROR!\n");
        test_control_return(1);
    }
    else
    {
        printf("SUCCESS!\n");
        test_control_return(0);
    };
}

static void  dhcp_test_initialize()
{


    /* Set up server responses on the primary interface.  */
    dhcp_response0[0].dhcp_response_pkt_data = &offer_response0[0];
    dhcp_response0[0].dhcp_response_pkt_size = offer_response0_size ;  
    
    dhcp_response0[1].dhcp_response_pkt_data = &ack_response0[0];
    dhcp_response0[1].dhcp_response_pkt_size = ack_response0_size ;

    /* Set up server responses on the secondary interface.  */
    dhcp_response1[0].dhcp_response_pkt_data = &offer_response1[0];
    dhcp_response1[0].dhcp_response_pkt_size = offer_response1_size ;  
    
    dhcp_response1[1].dhcp_response_pkt_data = &ack_response1[0];
    dhcp_response1[1].dhcp_response_pkt_size = ack_response1_size ;

}

static UINT   nx_dhcp_response_packet_send(NX_UDP_SOCKET *server_socket_ptr, UINT port, INT packet_number, UINT iface_index)
{

UINT        status;
NX_PACKET   *response_packet;
UCHAR       *work_ptr;
#ifdef __PRODUCT_NETXDUO__
NXD_ADDRESS ip_address;
#else
ULONG       ip_address;
NX_PACKET   **response_packet_ptr_ptr;
#endif

#ifdef __PRODUCT_NETXDUO__
    ip_address.nxd_ip_version = NX_IP_VERSION_V4;
    ip_address.nxd_ip_address.v4 = 0xFFFFFFFF;
#else
    ip_address = 0xFFFFFFFF;
#endif

    /* Allocate a response packet.  */
    status =  nx_packet_allocate(&server_pool, &response_packet, NX_TCP_PACKET, TX_WAIT_FOREVER);
    
    /* Check status.  */
    if (status)
    {
        error_counter++;
    }

    /* Write the  response messages into the packet payload!  */
    if (iface_index == 0) 
    {
        memcpy(response_packet -> nx_packet_prepend_ptr, dhcp_response0[packet_number].dhcp_response_pkt_data, 
               dhcp_response0[packet_number].dhcp_response_pkt_size);

       response_packet -> nx_packet_length =  dhcp_response0[packet_number].dhcp_response_pkt_size;
    }
    else if (iface_index == 1) 
    {

        memcpy(response_packet -> nx_packet_prepend_ptr, dhcp_response1[packet_number].dhcp_response_pkt_data, 
               dhcp_response1[packet_number].dhcp_response_pkt_size);

        response_packet -> nx_packet_length =  dhcp_response1[packet_number].dhcp_response_pkt_size;
    }

    /* Now replace the XID in the server message with what we know is the Client XID. */
    work_ptr = (UCHAR *)(response_packet -> nx_packet_prepend_ptr  + NX_BOOTP_OFFSET_XID);
    NX_CHANGE_ULONG_ENDIAN(dhcp_xid);
    memcpy(work_ptr, (void const *)(&dhcp_xid), 4);


    /* Adjust the write pointer.  */
    response_packet -> nx_packet_append_ptr =  response_packet -> nx_packet_prepend_ptr + response_packet -> nx_packet_length;
       
    /* Send the  packet with the correct port.  */
#ifdef __PRODUCT_NETXDUO__
    status = nxd_udp_socket_source_send(server_socket_ptr, response_packet, &ip_address, 68, iface_index);
#else
    response_packet_ptr_ptr = &response_packet;
    status = nx_udp_socket_interface_send(server_socket_ptr, *response_packet_ptr_ptr, ip_address, 68, iface_index);
#endif

    /* Check the status.  */
    if (status)      
    {
    
        error_counter++;
    
        nx_packet_release(response_packet);
    }

    return status;
}


static UINT my_dhcp_process_bc_callback(NX_IP *ip_ptr, NX_PACKET *packet_ptr, UINT *operation_ptr, UINT *delay_ptr)
{


UCHAR          *work_ptr;
ULONG          *message_ptr;
UINT           interface_index;
UINT           packet_client_mac_lsw;
ULONG          sender_physical_lsw;
NX_PACKET      *packet_copy;


    /* Check if this is an IP or an ARP (smaller) packet. We don't know where the packet 
       prepend pointer is so assume anything less than 200 bytes is an ARP packet. */
    if (packet_ptr -> nx_packet_length < 200)
    {

        message_ptr = (ULONG *)(packet_ptr -> nx_packet_prepend_ptr);
        
        NX_CHANGE_ULONG_ENDIAN(*(message_ptr + 2));
        NX_CHANGE_ULONG_ENDIAN(*(message_ptr + 3));

        sender_physical_lsw =  (*(message_ptr + 2) << 16) | (*(message_ptr + 3) >> 16);

        NX_CHANGE_ULONG_ENDIAN(*(message_ptr + 2));
        NX_CHANGE_ULONG_ENDIAN(*(message_ptr + 3));


        /* Determine what interface to use based on MAC address and which IP instance is sending the packet. */
        if (sender_physical_lsw == 0x22334458)
        {
           interface_index = 0;
        }
        else if (sender_physical_lsw == 0x22334459)
        {
            interface_index = 1;
        }
        else
        {
            /* Don't know what this packet is. Let DHCP Client handle it. */
            return NX_TRUE;
        }
    }
    else /* if (packet_type == MY_DRIVER_ETHERNET_IP) */
    {

        /* Set work_ptr.  */
#ifdef __PRODUCT_NETXDUO__
        work_ptr = packet_ptr -> nx_packet_prepend_ptr + sizeof(NX_IPV4_HEADER) + sizeof(NX_UDP_HEADER) +NX_BOOTP_OFFSET_CLIENT_HW;
#else
        work_ptr = packet_ptr -> nx_packet_prepend_ptr + sizeof(NX_IP_HEADER) + sizeof(NX_UDP_HEADER) +NX_BOOTP_OFFSET_CLIENT_HW;
#endif
        /* Pickup the target MAC address in the DHCP message.  */
        packet_client_mac_lsw = (((ULONG)work_ptr[2]) << 24) |
                                (((ULONG)work_ptr[3]) << 16) |
                                (((ULONG)work_ptr[4]) << 8) |
                                ((ULONG)work_ptr[5]);
        
        /* Determine what interface to use based on MAC address and which IP instance is sending the packet. */
        if (packet_client_mac_lsw == 0x22334458)
        {
           interface_index = 0;
        }
        else if (packet_client_mac_lsw == 0x22334459)
        {
            interface_index = 1;
        }
        else
        {
            /* Don't know what this packet is. Let DHCP Client handle it. */
            return NX_TRUE;
        }
    }
    
    /* Copy to a new packet and drop the original packet. */
    nx_packet_copy(packet_ptr, &packet_copy, &client_pool, NX_WAIT_FOREVER);
    
    /* Based on the packet mac address, set the packet interface based on the mac address */
    packet_copy -> nx_packet_ip_interface = &(ip_ptr -> nx_ip_interface[interface_index]); 

    if (packet_ptr -> nx_packet_length > 200)
    {

        if (ip_ptr == &server0_ip)
        {
            _nx_ip_packet_receive(&client_ip, packet_copy);
        }
        else
        {
            _nx_ip_packet_receive(&server0_ip, packet_copy);
        }
    }
    else
    {

        if (ip_ptr == &server0_ip)
        {
            _nx_arp_packet_receive(&client_ip, packet_copy);
        }
        else
        {
            _nx_arp_packet_receive(&server0_ip, packet_copy);
        }
    }

    *operation_ptr = NX_RAMDRIVER_OP_DROP;

    return NX_TRUE;

}

ULONG  dhcp_get_dhcp_data(UCHAR *data, UINT size)
{

ULONG   value = 0;

   
    /* Process the data retrieval request.  */
    while (size-- > 0)
    {

        /* Build return value.  */
        value = (value << 8) | *data++;
    }

    /* Return value.  */
    return(value);
}

VOID dhcp_interface_state_change(NX_DHCP *dhcp_ptr, UINT iface_index, UCHAR new_state)
{

    if (iface_index == 0) 
    {
    
        if (new_state == NX_DHCP_STATE_ADDRESS_PROBING) 
        {
            probe0 = NX_TRUE;
        }
        else if (new_state == NX_DHCP_STATE_BOUND) 
        {
            bound0 = NX_TRUE;
        }
    }
    else if (iface_index == 1) 
    {

        if (new_state == NX_DHCP_STATE_ADDRESS_PROBING) 
        {
            probe1 = NX_TRUE;
        }
        else if (new_state == NX_DHCP_STATE_BOUND) 
        {
            bound1 = NX_TRUE;
        }
    }
    else
        error_counter++;

    return;
}
#else
#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void    netx_dhcp_client_arpprobe_multiple_interface_test_application_define(void *first_unused_memory)
#endif
{
    printf("NetX Test:   DHCP Client Multiple Interface ARP Probe Test...............N/A\n"); 
    test_control_return(3);
}
#endif
