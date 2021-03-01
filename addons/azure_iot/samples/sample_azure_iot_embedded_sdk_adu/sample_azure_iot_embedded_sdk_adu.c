/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/

#include <stdio.h>

#include "nx_api.h"
#include "nx_azure_iot_adu_agent.h"

/* These are sample files, user can build their own certificate and ciphersuites.  */
#include "nx_azure_iot_cert.h"
#include "nx_azure_iot_ciphersuites.h"
#include "sample_config.h"

/* Define Azure RTOS TLS info.  */
static NX_SECURE_X509_CERT root_ca_cert;
static UCHAR nx_azure_iot_tls_metadata_buffer[NX_AZURE_IOT_TLS_METADATA_BUFFER_SIZE];
static ULONG nx_azure_iot_thread_stack[NX_AZURE_IOT_STACK_SIZE / sizeof(ULONG)];

/* Define the prototypes for AZ IoT.  */
static NX_AZURE_IOT             nx_azure_iot;
static NX_AZURE_IOT_PNP_CLIENT  pnp_client;
static NX_AZURE_IOT_ADU_AGENT   adu_agent;
static TX_EVENT_FLAGS_GROUP     sample_events;

/* ADU model id.  */
#define SAMPLE_ADU_MODEL_ID                                             "dtmi:AzureDeviceUpdate;1"

/* Device properties.  */
#define SAMPLE_DEVICE_MANUFACTURER                                      "Contoso"
#define SAMPLE_DEVICE_MODEL                                             "IoTDevice"

/* Current update id.  */
#define SAMPLE_UPDATE_ID_PROVIDER                                       "Contoso"
#define SAMPLE_UPDATE_ID_NAME                                           "IoTDevice"
#define SAMPLE_UPDATE_ID_VERSION                                        "6.1.0"

/* Sample events.  */
#define SAMPLE_ALL_EVENTS                                               ((ULONG)0xFFFFFFFF)
#define SAMPLE_DEVICE_PROPERTIES_GET_EVENT                              ((ULONG)0x00000001)
#define SAMPLE_DEVICE_DESIRED_PROPERTIES_EVENT                          ((ULONG)0x00000002)

extern void nx_azure_iot_adu_agent_simulator_driver(NX_AZURE_IOT_ADU_AGENT_DRIVER *driver_req_ptr);

VOID sample_entry(NX_IP *ip_ptr, NX_PACKET_POOL *pool_ptr, NX_DNS *dns_ptr, UINT (*unix_time_callback)(ULONG *unix_time));

static VOID connection_status_callback(NX_AZURE_IOT_PNP_CLIENT *pnp_client_ptr, UINT status)
{
    NX_PARAMETER_NOT_USED(pnp_client_ptr);
    if (status)
    {
        printf("Disconnected from IoTHub!: error code = 0x%08x\r\n", status);
    }
    else
    {
        printf("Connected to IoTHub.\r\n");
    }
}

static VOID message_receive_callback_properties(NX_AZURE_IOT_PNP_CLIENT *hub_client_ptr, VOID *context)
{

    NX_PARAMETER_NOT_USED(hub_client_ptr);
    NX_PARAMETER_NOT_USED(context);
    tx_event_flags_set(&sample_events, SAMPLE_DEVICE_PROPERTIES_GET_EVENT, TX_OR);
}

static VOID message_receive_callback_desire_property(NX_AZURE_IOT_PNP_CLIENT *hub_client_ptr, VOID *context)
{

    NX_PARAMETER_NOT_USED(hub_client_ptr);
    NX_PARAMETER_NOT_USED(context);
    tx_event_flags_set(&sample_events, SAMPLE_DEVICE_DESIRED_PROPERTIES_EVENT, TX_OR);
}

static UINT sample_initialize_iothub(NX_AZURE_IOT_PNP_CLIENT *pnp_client_ptr)
{
UINT status;
UCHAR *iothub_hostname = (UCHAR *)HOST_NAME;
UCHAR *iothub_device_id = (UCHAR *)DEVICE_ID;
UINT iothub_hostname_length = sizeof(HOST_NAME) - 1;
UINT iothub_device_id_length = sizeof(DEVICE_ID) - 1;


    printf("IoTHub Host Name: %.*s; Device ID: %.*s.\r\n",
           iothub_hostname_length, iothub_hostname, iothub_device_id_length, iothub_device_id);
    
    /* Initialize PnP client.  */
    if ((status = nx_azure_iot_pnp_client_initialize(pnp_client_ptr, &nx_azure_iot,
                                                     iothub_hostname, iothub_hostname_length,
                                                     iothub_device_id, iothub_device_id_length,
                                                     (UCHAR *)MODULE_ID, sizeof(MODULE_ID) - 1,
                                                     (const UCHAR *)SAMPLE_ADU_MODEL_ID, sizeof(SAMPLE_ADU_MODEL_ID) - 1,
                                                     _nx_azure_iot_tls_supported_crypto,
                                                     _nx_azure_iot_tls_supported_crypto_size,
                                                     _nx_azure_iot_tls_ciphersuite_map,
                                                     _nx_azure_iot_tls_ciphersuite_map_size,
                                                     nx_azure_iot_tls_metadata_buffer,
                                                     sizeof(nx_azure_iot_tls_metadata_buffer),
                                                     &root_ca_cert)))
    {
        printf("Failed on nx_azure_iot_pnp_client_initialize!: error code = 0x%08x\r\n", status);
        return(status);
    }
    
#if (USE_DEVICE_CERTIFICATE == 1)

    /* Initialize the device certificate.  */
    if ((status = nx_secure_x509_certificate_initialize(&device_certificate,
                                                        (UCHAR *)sample_device_cert_ptr, (USHORT)sample_device_cert_len,
                                                        NX_NULL, 0,
                                                        (UCHAR *)sample_device_private_key_ptr, (USHORT)sample_device_private_key_len,
                                                        DEVICE_KEY_TYPE)))
    {
        printf("Failed on nx_secure_x509_certificate_initialize!: error code = 0x%08x\r\n", status);
    }

    /* Set device certificate.  */
    else if ((status = nx_azure_iot_pnp_client_device_cert_set(pnp_client_ptr, &device_certificate)))
    {
        printf("Failed on nx_azure_iot_pnp_client_device_cert_set!: error code = 0x%08x\r\n", status);
    }
#else

    /* Set symmetric key.  */
    if ((status = nx_azure_iot_pnp_client_symmetric_key_set(pnp_client_ptr,
                                                            (UCHAR *)DEVICE_SYMMETRIC_KEY,
                                                            sizeof(DEVICE_SYMMETRIC_KEY) - 1)))
    {
        printf("Failed on nx_azure_iot_pnp_client_symmetric_key_set!\r\n");
    }
#endif /* USE_DEVICE_CERTIFICATE */

    /* Set connection status callback.  */
    else if ((status = nx_azure_iot_pnp_client_connection_status_callback_set(pnp_client_ptr,
                                                                              connection_status_callback)))
    {
        printf("Failed on connection_status_callback!\r\n");
    }
    else if ((status = nx_azure_iot_pnp_client_receive_callback_set(pnp_client_ptr,
                                                                    NX_AZURE_IOT_PNP_PROPERTIES,
                                                                    message_receive_callback_properties,
                                                                    (VOID *)pnp_client_ptr)))
    {
        printf("device properties callback set!: error code = 0x%08x\r\n", status);
    }
    else if ((status = nx_azure_iot_pnp_client_receive_callback_set(pnp_client_ptr,
                                                                    NX_AZURE_IOT_PNP_DESIRED_PROPERTIES,
                                                                    message_receive_callback_desire_property,
                                                                    (VOID *)pnp_client_ptr)))
    {
        printf("device desired property callback set!: error code = 0x%08x\r\n", status);
    }

    if (status)
    {
        nx_azure_iot_pnp_client_deinitialize(pnp_client_ptr);
    }

    return(status);
}

static void sample_device_properties_get_action(NX_AZURE_IOT_PNP_CLIENT *pnp_client_ptr)
{
UINT status = 0;
NX_AZURE_IOT_JSON_READER json_reader;
ULONG desired_properties_version;

    /* Receive full properties.  */
    if ((status = nx_azure_iot_pnp_client_properties_receive(pnp_client_ptr,
                                                             &json_reader,
                                                             &desired_properties_version,
                                                             NX_NO_WAIT)))
    {
        printf("Get all properties receive failed!: error code = 0x%08x\r\n", status);
        return;
    }

    /* Call nx_azure_iot_pnp_client_desired_component_property_value_next to process properties.  */

    nx_azure_iot_json_reader_deinit(&json_reader);
}

static void sample_device_desired_property_action(NX_AZURE_IOT_PNP_CLIENT *pnp_client_ptr)
{
UINT status = 0;
NX_AZURE_IOT_JSON_READER json_reader;
ULONG properties_version;

    /* Receive desired properties.  */
    if ((status = nx_azure_iot_pnp_client_desired_properties_receive(pnp_client_ptr,
                                                                     &json_reader,
                                                                     &properties_version,
                                                                     NX_NO_WAIT)))
    {
        printf("Receive desired property receive failed!: error code = 0x%08x\r\n", status);
        return;
    }

    /* Call nx_azure_iot_pnp_client_desired_component_property_value_next to process properties.  */

    nx_azure_iot_json_reader_deinit(&json_reader);
}

static void log_callback(az_log_classification classification, UCHAR *msg, UINT msg_len)
{
    if (classification == AZ_LOG_IOT_AZURERTOS)
    {
        printf("%.*s", msg_len, (CHAR *)msg);
    }
}

static void adu_agent_state_change(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr, UINT state)
{

    /* Check state.  */
    switch (state)
    {
    case NX_AZURE_IOT_ADU_AGENT_STATE_IDLE:
        printf("ADU AGENT STATE: IDLE\r\n");
        break;
    case NX_AZURE_IOT_ADU_AGENT_STATE_DOWNLOAD_STARTED:
        printf("ADU AGENT STATE: DOWNLOAD STARTED\r\n");
        break;
    case NX_AZURE_IOT_ADU_AGENT_STATE_DOWNLOAD_SUCCEEDED:
        printf("ADU AGENT STATE: DOWNLOAD SUCCEEDED\r\n");
        break;
    case NX_AZURE_IOT_ADU_AGENT_STATE_INSTALL_STARTED:
        printf("ADU AGENT STATE: INSTALL STARTED\r\n");
        break;
    case NX_AZURE_IOT_ADU_AGENT_STATE_INSTALL_SUCCEEDED:
        printf("ADU AGENT STATE: INSTALL SUCCEEDED\r\n");
        break;
    case NX_AZURE_IOT_ADU_AGENT_STATE_APPLY_STARTED:
        printf("ADU AGENT STATE: APPLY STARTED\r\n");

        /* Apply update immediately for testing.  */
        nx_azure_iot_adu_agent_update_apply(adu_agent_ptr);
        break;
    default:
        break;
    }
}

void sample_entry(NX_IP *ip_ptr, NX_PACKET_POOL *pool_ptr, NX_DNS *dns_ptr, UINT (*unix_time_callback)(ULONG *unix_time))
{
UINT status = 0;
ULONG app_events;
UINT loop = NX_TRUE;

    nx_azure_iot_log_init(log_callback);

    tx_event_flags_create(&sample_events, (CHAR*)"sample_app");

    /* Create Azure IoT handler.  */
    if ((status = nx_azure_iot_create(&nx_azure_iot, (UCHAR *)"Azure IoT", ip_ptr, pool_ptr, dns_ptr,
                                      nx_azure_iot_thread_stack, sizeof(nx_azure_iot_thread_stack),
                                      NX_AZURE_IOT_THREAD_PRIORITY, unix_time_callback)))
    {
        printf("Failed on nx_azure_iot_create!: error code = 0x%08x\r\n", status);
        return;
    }

    /* Initialize CA certificate.  */
    if ((status = nx_secure_x509_certificate_initialize(&root_ca_cert, (UCHAR *)_nx_azure_iot_root_cert,
                                                        (USHORT)_nx_azure_iot_root_cert_size,
                                                        NX_NULL, 0, NULL, 0, NX_SECURE_X509_KEY_TYPE_NONE)))
    {
        printf("Failed to initialize ROOT CA certificate!: error code = 0x%08x\r\n", status);
        nx_azure_iot_delete(&nx_azure_iot);
        return;
    }

    /* Initialize.  */
    if ((status = sample_initialize_iothub(&pnp_client)))
    {
        printf("Failed to initialize pnp client: error code = 0x%08x\r\n", status);
        nx_azure_iot_delete(&nx_azure_iot);
        return;
    }

    /* Start PnP connection.  */
    if (nx_azure_iot_pnp_client_connect(&pnp_client, NX_TRUE, NX_WAIT_FOREVER))
    {
        printf("Failed on nx_azure_iot_pnp_client_connect!\r\n");
        nx_azure_iot_pnp_client_deinitialize(&pnp_client);
        nx_azure_iot_delete(&nx_azure_iot);
        return;
    }

    /* Start ADU agent.  */
    if (nx_azure_iot_adu_agent_start(&adu_agent, &pnp_client,
                                     (const UCHAR *)SAMPLE_DEVICE_MANUFACTURER, sizeof(SAMPLE_DEVICE_MANUFACTURER) - 1,
                                     (const UCHAR *)SAMPLE_DEVICE_MODEL, sizeof(SAMPLE_DEVICE_MODEL) - 1,
                                     (const UCHAR *)SAMPLE_UPDATE_ID_PROVIDER, sizeof(SAMPLE_UPDATE_ID_PROVIDER) - 1,
                                     (const UCHAR *)SAMPLE_UPDATE_ID_NAME, sizeof(SAMPLE_UPDATE_ID_NAME) - 1,
                                     (const UCHAR *)SAMPLE_UPDATE_ID_VERSION, sizeof(SAMPLE_UPDATE_ID_VERSION) - 1,
                                     adu_agent_state_change,
                                     nx_azure_iot_adu_agent_simulator_driver))
    {
        printf("Failed on nx_azure_iot_adu_agent_start!\r\n");
        nx_azure_iot_pnp_client_deinitialize(&pnp_client);
        nx_azure_iot_delete(&nx_azure_iot);
        return;
    }

    printf("Device Properties: manufacturer: %s, model: %s\r\n", SAMPLE_DEVICE_MANUFACTURER, SAMPLE_DEVICE_MODEL);
    printf("Installed Update ID: provider: %s, name: %s, version: %s\r\n", SAMPLE_UPDATE_ID_PROVIDER, SAMPLE_UPDATE_ID_NAME, SAMPLE_UPDATE_ID_VERSION);
    printf("\r\n");

    /* Request full properties.  */
    nx_azure_iot_pnp_client_properties_request(&pnp_client, NX_WAIT_FOREVER);

    /* Loop to process events.  */
    while (loop)
    {

        /* Pickup event flags.  */
        tx_event_flags_get(&sample_events, SAMPLE_ALL_EVENTS, TX_OR_CLEAR, &app_events, NX_WAIT_FOREVER);

        if (app_events & SAMPLE_DEVICE_PROPERTIES_GET_EVENT)
        {
            sample_device_properties_get_action(&pnp_client);
        }

        if (app_events & SAMPLE_DEVICE_DESIRED_PROPERTIES_EVENT)
        {
            sample_device_desired_property_action(&pnp_client);
        }
    }

    nx_azure_iot_pnp_client_disconnect(&pnp_client);
    nx_azure_iot_pnp_client_deinitialize(&pnp_client);
    nx_azure_iot_delete(&nx_azure_iot);
}

