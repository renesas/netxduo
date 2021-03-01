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

/* Version: 6.1 ADU Preview 1 */

#include "nx_azure_iot_adu_agent.h"

static VOID nx_azure_iot_adu_agent_event_process(VOID *adu_agen, ULONG common_events, ULONG module_own_events);
static VOID nx_azure_iot_adu_agent_timer_event_process(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr);
static VOID nx_azure_iot_adu_agent_workflow_update(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr);
static UINT nx_azure_iot_adu_agent_manifest_verify(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr);
static UINT nx_azure_iot_adu_agent_jws_split(UCHAR *jws, UINT jws_length,
                                             UCHAR **header, UINT *header_length,
                                             UCHAR **payload, UINT *payload_length,
                                             UCHAR **signature, UINT *signature_length);
static UINT nx_azure_iot_adu_agent_service_properties_get(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr,
                                                          NX_AZURE_IOT_JSON_READER *json_reader_ptr);
static UINT nx_azure_iot_adu_agent_service_update_manifest_property_process(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr);
static UINT nx_azure_iot_adu_agent_service_reported_properties_send(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr, 
                                                                    UINT status_code, ULONG version, const CHAR *description,
                                                                    ULONG wait_option);
static UINT nx_azure_iot_adu_agent_client_reported_properties_send(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr,
                                                                   UINT adu_agent_state,
                                                                   UINT installed_update_id_flag,
                                                                   UINT device_properties_flag,
                                                                   NX_AZURE_IOT_ADU_AGENT_RESULT *adu_agent_result,
                                                                   UINT wait_option);
static VOID nx_azure_iot_adu_agent_state_update(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr,
                                                UINT state,
                                                NX_AZURE_IOT_ADU_AGENT_RESULT *adu_agent_result);
static UINT nx_azure_iot_adu_agent_method_idle(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr);
static UINT nx_azure_iot_adu_agent_method_download(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr);
static UINT nx_azure_iot_adu_agent_method_install(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr);
static UINT nx_azure_iot_adu_agent_method_apply(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr);
static UINT nx_azure_iot_adu_agent_method_cancel(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr);
static UINT nx_azure_iot_adu_agent_duplicate_request_check(UINT action, UINT last_reported_state);
static UINT nx_azure_iot_adu_agent_update_is_installed(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr);
static const NX_AZURE_IOT_ADU_AGENT_RSA_ROOT_KEY *nx_azure_iot_adu_agent_rsa_root_key_find(const UCHAR* kid, UINT kid_size);
static UINT nx_azure_iot_adu_agent_sha256_calculate(NX_CRYPTO_METHOD *sha256_method,
                                                    UCHAR *metadata_ptr, UINT metadata_size,
                                                    UCHAR *input_ptr, ULONG input_size,
                                                    UCHAR *output_ptr, ULONG output_size);
static UINT nx_azure_iot_adu_agent_rs256_verify(NX_AZURE_IOT_ADU_AGENT_CRYPTO *adu_agent_crypto,
                                                UCHAR *input_ptr, ULONG input_size,
                                                UCHAR *signature_ptr, ULONG signature_size,
                                                UCHAR *n, ULONG n_size,
                                                UCHAR *e, ULONG e_size,
                                                UCHAR *buffer_ptr, UINT buffer_size);
static UINT nx_azure_iot_adu_agent_file_url_parse(UCHAR *file_url, ULONG file_url_length, 
                                                  UCHAR *buffer_ptr, UINT buffer_size,
                                                  NX_AZURE_IOT_ADU_AGENT_DOWNLOADER *downloader_ptr);
static void nx_azure_iot_adu_agent_dns_query(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr);
static void nx_azure_iot_adu_agent_dns_response_notify(NX_UDP_SOCKET *socket_ptr);
static void nx_azure_iot_adu_agent_dns_response_get(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr);
static void nx_azure_iot_adu_agent_http_connect(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr);
static void nx_azure_iot_adu_agent_http_request_send(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr);
static void nx_azure_iot_adu_agent_http_response_receive(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr);
static void nx_azure_iot_adu_agent_http_establish_notify(NX_TCP_SOCKET *socket_ptr);
static void nx_azure_iot_adu_agent_http_receive_notify(NX_TCP_SOCKET *socket_ptr);
static void nx_azure_iot_adu_agent_download_state_update(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr, UINT success);
static UINT nx_azure_iot_adu_agent_component_property_process(NX_AZURE_IOT_JSON_READER *json_reader_ptr,
                                                              ULONG version,
                                                              VOID *args);
extern UINT nx_azure_iot_pnp_client_component_add_internal(NX_AZURE_IOT_PNP_CLIENT *pnp_client_ptr,
                                                           const UCHAR *component_name_ptr,
                                                           UINT component_name_length,
                                                           UINT (*callback_ptr)(NX_AZURE_IOT_JSON_READER *json_reader_ptr,
                                                                                ULONG version,
                                                                                VOID *args),
                                                           VOID *callback_args);

UINT nx_azure_iot_adu_agent_start(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr,
                                  NX_AZURE_IOT_PNP_CLIENT *pnp_client_ptr,
                                  const UCHAR *manufacturer, UINT manufacturer_length,
                                  const UCHAR *model, UINT model_length,
                                  const UCHAR *provider, UINT provider_length,
                                  const UCHAR *name, UINT name_length,
                                  const UCHAR *version, UINT version_length,
                                  VOID (*adu_agent_state_change_notify)(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr, UINT new_state),
                                  VOID (*adu_agent_driver)(NX_AZURE_IOT_ADU_AGENT_DRIVER *))
{
UINT i;
UINT status;
INT update_id_length;
NX_AZURE_IOT *nx_azure_iot_ptr;
NX_AZURE_IOT_RESOURCE *resource_ptr;
NX_CRYPTO_METHOD *method_sha256 = NX_NULL;
NX_CRYPTO_METHOD *method_rsa = NX_NULL;
NX_SECURE_TLS_SESSION *tls_session;
NX_AZURE_IOT_ADU_AGENT_CRYPTO *adu_agent_crypto;
NX_AZURE_IOT_ADU_AGENT_RESULT result;
NX_AZURE_IOT_ADU_AGENT_DRIVER driver_request;

    if ((adu_agent_ptr == NX_NULL) || (pnp_client_ptr == NX_NULL) ||
        (manufacturer == NX_NULL) || (manufacturer_length == 0) ||
        (model == NX_NULL) || (model_length == 0) ||
        (provider == NX_NULL) || (provider_length == 0) ||
        (name == NX_NULL) || (name_length == 0) ||
        (version == NX_NULL) || (version_length == 0) ||
        (adu_agent_driver == NX_NULL))
    {
        LogError(LogLiteralArgs("ADU agent start fail: INVALID POINTER"));
        return(NX_AZURE_IOT_INVALID_PARAMETER);
    }

    memset(adu_agent_ptr, 0, sizeof(NX_AZURE_IOT_ADU_AGENT));

    /* Set pnp client pointer and azure iot pointer.  */
    adu_agent_ptr -> nx_azure_iot_pnp_client_ptr = pnp_client_ptr;
    nx_azure_iot_ptr = pnp_client_ptr -> nx_azure_iot_pnp_client_transport.nx_azure_iot_ptr;
    
    /* Add ADU component.  */
    if ((status = nx_azure_iot_pnp_client_component_add_internal(pnp_client_ptr, 
                                                                 (const UCHAR *)NX_AZURE_IOT_ADU_AGENT_COMPONENT_NAME,
                                                                 sizeof(NX_AZURE_IOT_ADU_AGENT_COMPONENT_NAME) - 1,
                                                                 nx_azure_iot_adu_agent_component_property_process,
                                                                 adu_agent_ptr)))
    {
        LogError(LogLiteralArgs("ADU agent start fail: PnP COMPONENT ADD FAIL: %d"), status);
        return(status);
    }

    /* Save the mutex.  */
    adu_agent_ptr -> nx_azure_iot_adu_agent_mutex_ptr = nx_azure_iot_ptr -> nx_azure_iot_mutex_ptr;

    /* Find RSA and SHA256.  */
    resource_ptr = &(pnp_client_ptr -> nx_azure_iot_pnp_client_transport.nx_azure_iot_hub_transport_resource);
    for(i = 0; i < resource_ptr -> resource_crypto_array_size; i++)
    {
        if(resource_ptr -> resource_crypto_array[i] -> nx_crypto_algorithm == NX_CRYPTO_HASH_SHA256)
        {
            method_sha256 = (NX_CRYPTO_METHOD *)resource_ptr -> resource_crypto_array[i];
        }
        else if(resource_ptr -> resource_crypto_array[i] -> nx_crypto_algorithm == NX_CRYPTO_KEY_EXCHANGE_RSA)
        {
            method_rsa = (NX_CRYPTO_METHOD *)resource_ptr -> resource_crypto_array[i];
        }

        if ((method_sha256) && (method_rsa))
        {
            break;
        }
    }

    /* Check if find the crypto method.  */
    if ((method_sha256 == NX_NULL) || (method_rsa == NX_NULL))
    {
        LogError(LogLiteralArgs("ADU agent start fail: NO AVAILABLE CIPHER SHA256"));
        return(NX_AZURE_IOT_NO_AVAILABLE_CIPHER);
    }

    /* Check if the metadata size is enough.  */
    if (method_sha256 -> nx_crypto_metadata_area_size > NX_AZURE_IOT_ADU_AGENT_SHA256_METADATA_SIZE)
    {
        LogError(LogLiteralArgs("ADU agent start fail: INSUFFICIENT BUFFER FOR SHA256"));
        return(NX_AZURE_IOT_INSUFFICIENT_BUFFER_SPACE);
    }

    /* Save the crypto methods (RS256) for verifying update manifest.  */
    adu_agent_crypto = &(adu_agent_ptr -> nx_azure_iot_adu_agent_crypto);

    /* Set RSA crypto, reuse the metadata from tls session.  */
    tls_session = &(pnp_client_ptr -> nx_azure_iot_pnp_client_transport.nx_azure_iot_hub_transport_resource.resource_mqtt.nxd_mqtt_tls_session);
    adu_agent_crypto -> method_rsa = method_rsa;
    adu_agent_crypto -> method_rsa_metadata = tls_session -> nx_secure_public_cipher_metadata_area;
    adu_agent_crypto -> method_rsa_metadata_size = tls_session -> nx_secure_public_cipher_metadata_size;

    /* Set SHA256 crypto.  */
    adu_agent_crypto -> method_sha256 = method_sha256;

    /* Setup the driver.  */
    adu_agent_ptr -> nx_azure_iot_adu_agent_driver_entry = adu_agent_driver;

    /* Call the driver to initialize the hardware.  */
    driver_request.nx_azure_iot_adu_agent_driver_command = NX_AZURE_IOT_ADU_AGENT_DRIVER_INITIALIZE;
    driver_request.nx_azure_iot_adu_agent_driver_status = NX_AZURE_IOT_SUCCESS;
    (adu_agent_ptr -> nx_azure_iot_adu_agent_driver_entry)(&driver_request);

    /* Check status.  */
    if (driver_request.nx_azure_iot_adu_agent_driver_status)
    {
        LogError(LogLiteralArgs("ADU agent start fail: DRIVER ERROR"));
        return(NX_AZURE_IOT_FAILURE);
    }

    /* Save the device properties (manufacturer and model).  */
    adu_agent_ptr -> nx_azure_iot_adu_agent_device_properties.manufacturer = manufacturer;
    adu_agent_ptr -> nx_azure_iot_adu_agent_device_properties.manufacturer_length = manufacturer_length;
    adu_agent_ptr -> nx_azure_iot_adu_agent_device_properties.model = model;
    adu_agent_ptr -> nx_azure_iot_adu_agent_device_properties.model_length = model_length;

    /* Save the current update id (provider, name and version.)*/
    adu_agent_ptr -> nx_azure_iot_adu_agent_current_update_id.provider = provider;
    adu_agent_ptr -> nx_azure_iot_adu_agent_current_update_id.provider_length = provider_length;
    adu_agent_ptr -> nx_azure_iot_adu_agent_current_update_id.name = name;
    adu_agent_ptr -> nx_azure_iot_adu_agent_current_update_id.name_length = name_length;
    adu_agent_ptr -> nx_azure_iot_adu_agent_current_update_id.version = version;
    adu_agent_ptr -> nx_azure_iot_adu_agent_current_update_id.version_length = version_length;

    /* Encode the update id as string.*/
    update_id_length = snprintf((CHAR *)adu_agent_ptr -> nx_azure_iot_adu_agent_current_update_id.update_id_buffer,
                                NX_AZURE_IOT_ADU_AGENT_UPDATE_ID_SIZE,
                                "{\"%s\":\"%s\",\"%s\":\"%s\",\"%s\":\"%s\"}", 
                                NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_PROVIDER, provider, 
                                NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_NAME, name,
                                NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_VERSION, version);
    if (update_id_length)
    {
        adu_agent_ptr -> nx_azure_iot_adu_agent_current_update_id.update_id_length = (UINT)update_id_length;
    }
    else
    {
        LogError(LogLiteralArgs("ADU agent start fail: INSUFFICIENT BUFFER FOR UPDATE ID"));
        return(NX_AZURE_IOT_INSUFFICIENT_BUFFER_SPACE);
    }

    /* We're assuming that the update (before restart) was succeeded and the update was applied correctly.
       In this case, we will update the 'InstalledContentId' to match 'ExpectedContentId', then go to Idel state.  */
    result.result_code = NX_AZURE_IOT_ADU_AGENT_RESULT_CODE_SUCCESS;
    result.extended_result_code = 0;
    status = nx_azure_iot_adu_agent_client_reported_properties_send(adu_agent_ptr, NX_AZURE_IOT_ADU_AGENT_STATE_IDLE, NX_TRUE, NX_TRUE, &result, NX_WAIT_FOREVER);
    if (status)
    {
        LogError(LogLiteralArgs("ADU agent start fail: CLIENT REPORTED PROPERTIES SEND FAIL"));
        return(status);
    }

    /* Set the last reported state as IDLE.  */
    adu_agent_ptr -> nx_azure_iot_adu_agent_last_reported_state = NX_AZURE_IOT_ADU_AGENT_STATE_IDLE;
    adu_agent_ptr -> nx_azure_iot_adu_agent_operation_in_progress = NX_FALSE;
    adu_agent_ptr -> nx_azure_iot_adu_agent_operation_cancelled = NX_FALSE;

    /* Set the state change notifiction.  */
    adu_agent_ptr -> nx_azure_iot_adu_agent_state_change_notify = adu_agent_state_change_notify;

    /* Set the dns pointer.  */
    adu_agent_ptr -> nx_azure_iot_adu_agent_downloader.dns_ptr = nx_azure_iot_ptr -> nx_azure_iot_dns_ptr;

    /* Set the UDP socket receive callback function for non-blocking DNS.  */
    nx_azure_iot_ptr -> nx_azure_iot_dns_ptr -> nx_dns_socket.nx_udp_socket_reserved_ptr = adu_agent_ptr;
    status = nx_udp_socket_receive_notify(&(nx_azure_iot_ptr -> nx_azure_iot_dns_ptr -> nx_dns_socket),
                                          nx_azure_iot_adu_agent_dns_response_notify);
    if (status)
    {
        LogError(LogLiteralArgs("DNS Receive notification register fail status: %d"), status);
        return(status);
    }

    /* Register ADU module on cloud helper.  */
    status = nx_cloud_module_register(&(nx_azure_iot_ptr -> nx_azure_iot_cloud),
                                      &(adu_agent_ptr -> nx_azure_iot_adu_agent_cloud_module),
                                      "Azure Device Update Module", 
                                      (NX_CLOUD_MODULE_AZURE_ADU_EVENT | NX_CLOUD_COMMON_PERIODIC_EVENT),
                                      nx_azure_iot_adu_agent_event_process, adu_agent_ptr);
    if (status)
    {
        LogError(LogLiteralArgs("ADU module register fail status: %d"), status);
        return(status);
    }

    LogInfo(LogLiteralArgs("ADU agent started successfully!"));

    return(NX_AZURE_IOT_SUCCESS);
}

static UINT nx_azure_iot_adu_agent_component_property_process(NX_AZURE_IOT_JSON_READER *json_reader_ptr,
                                                              ULONG version,
                                                              VOID *args)
{
UINT status;
NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr = (NX_AZURE_IOT_ADU_AGENT *)args;


    /* Check "service" property name.   */
    if (nx_azure_iot_json_reader_token_is_text_equal(json_reader_ptr,
                                                     (UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_SERVICE,
                                                     sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_SERVICE) - 1))
    {

        /* Obtain the mutex.  */
        tx_mutex_get(adu_agent_ptr -> nx_azure_iot_adu_agent_mutex_ptr, NX_WAIT_FOREVER);

        /* Step1. Get service property value.  */
        status = nx_azure_iot_adu_agent_service_properties_get(adu_agent_ptr, json_reader_ptr);
        if (status)
        {

            /* Release the mutex.  */
            tx_mutex_put(adu_agent_ptr -> nx_azure_iot_adu_agent_mutex_ptr);
            LogError(LogLiteralArgs("ADU agent component process fail: SERVICE PROPERTIES GET FAIL"));
            return(status);
        }

        /* Step2. Send service response.  */
        nx_azure_iot_adu_agent_service_reported_properties_send(adu_agent_ptr, 
                                                                NX_AZURE_IOT_PNP_STATUS_SUCCESS, version, "",
                                                                NX_NO_WAIT);

        /* Release the mutex.  */
        tx_mutex_put(adu_agent_ptr -> nx_azure_iot_adu_agent_mutex_ptr);

        /* Ste3. Set property receive event to let cloud thread to process.  */
        nx_cloud_module_event_set(&(adu_agent_ptr -> nx_azure_iot_adu_agent_cloud_module),
                                  NX_AZURE_IOT_ADU_AGENT_PROPERTY_RECEIVE_EVENT);
    }
    else
    {
        return(NX_AZURE_IOT_FAILURE);
    }

    return(NX_AZURE_IOT_SUCCESS);
}

UINT nx_azure_iot_adu_agent_update_apply(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr)
{
NX_AZURE_IOT_ADU_AGENT_DRIVER driver_request;

    if (adu_agent_ptr == NX_NULL)
    {
        LogError(LogLiteralArgs("ADU agent apply fail: INVALID POINTER"));
        return(NX_AZURE_IOT_INVALID_PARAMETER);
    }

    /* Applying...  */
    LogInfo(LogLiteralArgs("Applying...\r\n\r\n"));

    /* Send the firmware apply request to the driver.   */
    driver_request.nx_azure_iot_adu_agent_driver_command = NX_AZURE_IOT_ADU_AGENT_DRIVER_APPLY;
    (adu_agent_ptr -> nx_azure_iot_adu_agent_driver_entry)(&driver_request);    

    /* Apply should reboot the device and never return. If it does return, something is wrong.  */
    LogError(LogLiteralArgs("Apply failed"));

    return(NX_AZURE_IOT_FAILURE);
}

static VOID nx_azure_iot_adu_agent_event_process(VOID *adu_agent, ULONG common_events, ULONG module_own_events)
{

NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr = (NX_AZURE_IOT_ADU_AGENT *)adu_agent;

    /* Obtain the mutex.  */
    tx_mutex_get(adu_agent_ptr -> nx_azure_iot_adu_agent_mutex_ptr, NX_WAIT_FOREVER);

    /* Process common periodic event.   */
    if (common_events & NX_CLOUD_COMMON_PERIODIC_EVENT)
    {

        /* Process timer event.  */
        nx_azure_iot_adu_agent_timer_event_process(adu_agent_ptr);
    }

    /* Loop to process events.  */
    if (module_own_events & NX_AZURE_IOT_ADU_AGENT_PROPERTY_RECEIVE_EVENT)
    {

        /* Update workflow.  */
        nx_azure_iot_adu_agent_workflow_update(adu_agent_ptr);
    }
    if (module_own_events & NX_AZURE_IOT_ADU_AGENT_DNS_RESPONSE_RECEIVE_EVENT)
    {

        /* Process DNS response get event.  */
        nx_azure_iot_adu_agent_dns_response_get(adu_agent_ptr);
    }
    if (module_own_events & NX_AZURE_IOT_ADU_AGENT_HTTP_CONNECT_DONE_EVENT)
    {

        /* Process HTTP connect done event.  */
        nx_azure_iot_adu_agent_http_request_send(adu_agent_ptr);
    }
    if (module_own_events & NX_AZURE_IOT_ADU_AGENT_HTTP_RECEIVE_EVENT)
    {

        /* Process HTTP receive event.  */
        nx_azure_iot_adu_agent_http_response_receive(adu_agent_ptr);
    }

    /* Release the mutex.  */
    tx_mutex_put(adu_agent_ptr -> nx_azure_iot_adu_agent_mutex_ptr);
}

static VOID nx_azure_iot_adu_agent_timer_event_process(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr)
{

NX_AZURE_IOT_ADU_AGENT_DOWNLOADER *downloader_ptr = &(adu_agent_ptr -> nx_azure_iot_adu_agent_downloader);

    /* Check the timer for DNS/HTTP.  */
    if (downloader_ptr -> timeout)
    {

        /* Decrease the timeout.  */
        downloader_ptr -> timeout--;

        /* Check if it is timeout.  */
        if (downloader_ptr -> timeout != 0)
        {
            return;
        }

        /* Check the state.  */
        if (downloader_ptr -> state == NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_ADDRESS_QUERY)
        {

            /* DNS query timeout, try to receive dns response, if there is no DNS response. Retry DNS query.  */
            nx_azure_iot_adu_agent_dns_response_get(adu_agent_ptr);
        }
        else if ((downloader_ptr -> state == NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_HTTP_CONNECT) ||
                 (downloader_ptr -> state == NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_HTTP_CONTENT_GET))
        {

            /* Timeout for http connect or http content get.  */
            nx_azure_iot_adu_agent_download_state_update(adu_agent_ptr, NX_FALSE);
        }
    }
}

static VOID nx_azure_iot_adu_agent_workflow_update(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr)
{
NX_AZURE_IOT_ADU_AGENT_RESULT result;
UINT is_duplicate_request;
UINT status = 0;

    //
    // Special case: Cancel is handled here.
    //
    // If Cancel action is received while another action (e.g. download) is in progress the agent should cancel
    // the in progress action and the agent should set Idle state.
    //
    // If an operation completes with a failed state, the error should be reported to the service, and the agent
    // should set Failed state.  The CBO once it receives the Failed state will send the agent a Cancel action to
    // indicate that it should return to Idle.  It's assumed that there is no action in progress on the agent at this
    // point and as such, the agent should set Idle state and return a success result code to the service.
    //
    //  Cancel should only be sent from the CBO when:
    // * An operation is in progress, to cancel the operation.
    // * After an operation fails to return the agent back to Idle state.
    // * A rollout end time has passed & the device has been offline and did not receive the previous command.
    //
    if (adu_agent_ptr -> nx_azure_iot_adu_agent_action == NX_AZURE_IOT_ADU_AGENT_ACTION_CANCEL)
    {

        if (adu_agent_ptr -> nx_azure_iot_adu_agent_operation_in_progress)
        {

            // Set OperationCancelled so that when the operation in progress completes, it's clear
            // that it was due to a cancellation.
            // We will ignore the result of what the operation in progress returns when it completes in cancelled state.
            adu_agent_ptr-> nx_azure_iot_adu_agent_operation_cancelled = true;

            nx_azure_iot_adu_agent_method_cancel(adu_agent_ptr);
        }
        else
        {

            /* Cancel without an operation in progress means return to Idle state.  */
            LogInfo(LogLiteralArgs("Cancel received with no operation in progress - returning to Idle state"));

            result.result_code = NX_AZURE_IOT_ADU_AGENT_RESULT_CODE_SUCCESS;
            result.extended_result_code = 0;

            nx_azure_iot_adu_agent_state_update(adu_agent_ptr, NX_AZURE_IOT_ADU_AGENT_STATE_IDLE, &result);
        }

        return;
    }

    /* Not a cancel action.  */

    /* Check if it is a valid action.  */
    if (adu_agent_ptr -> nx_azure_iot_adu_agent_action > NX_AZURE_IOT_ADU_AGENT_ACTION_APPLY)
    {
        return;
    }

    /* Verify manifest.  */
    if (nx_azure_iot_adu_agent_manifest_verify(adu_agent_ptr) != NX_TRUE)
    {
        return;
    }

    //
    // Workaround:
    // Connections to the service may disconnect after a period of time (e.g. 40 minutes)
    // due to the need to refresh a token. When the reconnection occurs, all properties are re-sent
    // to the client, and as such the client might see a duplicate request, for example, another download request
    // after already processing a downloadrequest.
    // We ignore these requests because they have been handled, are currently being handled, or would be a no-op.
    //
    is_duplicate_request = nx_azure_iot_adu_agent_duplicate_request_check(adu_agent_ptr -> nx_azure_iot_adu_agent_action,
                                                                          adu_agent_ptr -> nx_azure_iot_adu_agent_last_reported_state);

    /* Check result.  */
    if (is_duplicate_request)
    {
        return;
    }

    // Fail if we have already have an operation in progress.
    // This check happens after the check for duplicates, so we don't log a warning in our logs for an operation
    // that is currently being processed.
    if (adu_agent_ptr -> nx_azure_iot_adu_agent_operation_in_progress)
    {
        return;
    }

    /* Update operation in progress flag.  */
    adu_agent_ptr -> nx_azure_iot_adu_agent_operation_in_progress = NX_TRUE;

    /* Check the action.  */
    if (adu_agent_ptr -> nx_azure_iot_adu_agent_action == NX_AZURE_IOT_ADU_AGENT_ACTION_DOWNLOAD)
    {

        /* Start download.  */
        status = nx_azure_iot_adu_agent_method_download(adu_agent_ptr);
    }
    else if (adu_agent_ptr -> nx_azure_iot_adu_agent_action == NX_AZURE_IOT_ADU_AGENT_ACTION_INSTALL)
    {

        /* Start install.  */
        status = nx_azure_iot_adu_agent_method_install(adu_agent_ptr);
    }
    else if (adu_agent_ptr -> nx_azure_iot_adu_agent_action == NX_AZURE_IOT_ADU_AGENT_ACTION_APPLY)
    {

        /* Start apply.  */
        status = nx_azure_iot_adu_agent_method_apply(adu_agent_ptr);
    }

    // Action is complete (i.e. we wont get a WorkCompletionCallback call from upper-layer) if:
    // * Upper-level did the work in a blocking manner.
    // * Method returned failure.
    // NOLINTNEXTLINE(misc-redundant-expression)
    if (status)
    {

        // Operation (e.g. Download) failed or was cancelled - both are considered AducResult failure codes.

        if (adu_agent_ptr -> nx_azure_iot_adu_agent_operation_cancelled)
        {
            // Operation cancelled.
            //
            // We are now at the completion of the operation that was cancelled and will just return to Idle state,
            // Ignore the result of the operation, which most likely is cancelled, e.g. ADUC_DownloadResult_Cancelled.
            result.result_code = NX_AZURE_IOT_ADU_AGENT_RESULT_CODE_SUCCESS;
            result.extended_result_code = 0;
            nx_azure_iot_adu_agent_state_update(adu_agent_ptr, NX_AZURE_IOT_ADU_AGENT_STATE_IDLE, &result);
        }
        else
        {
            // Operation failed.
            //
            // Report back the result and set state to "Failed".
            // It's expected that the service will call us again with a "Cancel" action,
            // to indicate that it's received the operation result and state, at which time
            // we'll return back to idle state.
            
            result.result_code = NX_AZURE_IOT_ADU_AGENT_RESULT_CODE_ERROR;
            result.extended_result_code = 0;
            nx_azure_iot_adu_agent_state_update(adu_agent_ptr, NX_AZURE_IOT_ADU_AGENT_STATE_FAILED, &result);
        }
    }

    // Operation is now complete.
    adu_agent_ptr -> nx_azure_iot_adu_agent_operation_in_progress = NX_FALSE;
    adu_agent_ptr -> nx_azure_iot_adu_agent_operation_cancelled = NX_FALSE;

}

static UINT nx_azure_iot_adu_agent_manifest_verify(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr)
{
UINT   status;
UCHAR *header_b64;
UINT   header_b64_length;
UCHAR *payload_b64;
UINT   payload_b64_length;
UCHAR *signature_b64;
UINT   signature_b64_length;
UCHAR *jwk_header_b64;
UINT   jwk_header_b64_length;
UCHAR *jwk_payload_b64;
UINT   jwk_payload_b64_length;
UCHAR *jwk_signature_b64;
UINT   jwk_signature_b64_length;
UCHAR *signature;
UINT   signature_length;
UINT   sjwk_size = 0;
UCHAR *alg_ptr = NX_NULL;
UINT   alg_size = 0;
UCHAR *kid_ptr = NX_NULL;
UINT   kid_size = 0;
UCHAR *kty_ptr = NX_NULL;
UINT   kty_size = 0;
UCHAR *n_b64_ptr = NX_NULL;
UINT   n_b64_size = 0;
UCHAR *e_b64_ptr = NX_NULL;
UINT   e_b64_size = 0;
UCHAR *n_ptr = NX_NULL;
UINT   n_size = 0;
UCHAR *e_ptr = NX_NULL;
UINT   e_size = 0;
UCHAR *buffer_ptr;
UINT   buffer_size;
UINT   bytes_copied;
NX_AZURE_IOT_ADU_AGENT_RSA_ROOT_KEY *rsa_root_key;
NX_AZURE_IOT_ADU_AGENT_CRYPTO *adu_agent_crypto = &(adu_agent_ptr -> nx_azure_iot_adu_agent_crypto);
NX_AZURE_IOT_JSON_READER json_reader;
UCHAR  *sha256_generated_hash_ptr;
UCHAR  *sha256_decoded_hash_64_ptr;
UCHAR  *sha256_decoded_hash_ptr;

    /* Signed update manifest: https://microsoft.visualstudio.com/Universal%20Store/_wiki/wikis/EDS%20Wiki/58449/Signed-update-manifest  */

    /* JWS value format: BASE64URL(UTF8(header)) + "." + BASE64URL(UTF8(payload) + "." + BASE64URL(signature)).  */

    /* Step1. Parse JWS data.  */

    /* Header:
       {
           "alg": "RS256",
           "sjwk": "signed JWK"
       }

       Payload:
       {
           "sha256":"xxx...xxx"
       }

       Signature:
    */

    /* Initialize.  */
    alg_size = 0;
    sjwk_size = 0;
    buffer_ptr = adu_agent_ptr -> nx_azure_iot_adu_agent_buffer;
    buffer_size = NX_AZURE_IOT_ADU_AGENT_BUFFER_SIZE;

    /* 1.1 Split header, payload and signature.  */
    if (nx_azure_iot_adu_agent_jws_split(adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_signature,
                                         adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_signature_size,
                                         &header_b64, &header_b64_length,
                                         &payload_b64, &payload_b64_length,
                                         &signature_b64, &signature_b64_length) == NX_FALSE)
    {
        return(NX_FALSE);
    }

    /* 1.2 Decode header.  */
    if (nx_azure_iot_base64_decode((CHAR *)header_b64, header_b64_length,
                                   buffer_ptr, buffer_size, &bytes_copied))
    {
        return(NX_FALSE);
    }
    
    /* Initialize the header string as json.  */
    if (nx_azure_iot_json_reader_with_buffer_init(&json_reader, buffer_ptr, bytes_copied))
    {
        return(NX_FALSE);
    }
    buffer_ptr += bytes_copied;
    buffer_size -= bytes_copied;

    /* Skip the first begin object. */
    if ((nx_azure_iot_json_reader_next_token(&json_reader) != NX_AZURE_IOT_SUCCESS) ||
        (nx_azure_iot_json_reader_token_type(&json_reader) != NX_AZURE_IOT_READER_TOKEN_BEGIN_OBJECT))
    {
        return(NX_FALSE);
    }

    /* Loop to process all data.  */
    while (nx_azure_iot_json_reader_next_token(&json_reader) == NX_AZURE_IOT_SUCCESS)
    {
        if (nx_azure_iot_json_reader_token_type(&json_reader) == NX_AZURE_IOT_READER_TOKEN_PROPERTY_NAME)
        {

            /* Get alg value.  */
            if (nx_azure_iot_json_reader_token_is_text_equal(&json_reader, (UCHAR *)"alg", sizeof("alg") - 1))
            {
                if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                    nx_azure_iot_json_reader_token_string_get(&json_reader,
                                                              buffer_ptr,
                                                              buffer_size,
                                                              &alg_size))
                {
                    return(NX_FALSE);
                }
                alg_ptr = buffer_ptr;
                buffer_ptr += alg_size;
                buffer_size -= alg_size;
            }

            /* Get sjwk value.  */
            else if (nx_azure_iot_json_reader_token_is_text_equal(&json_reader, (UCHAR *)"sjwk", sizeof("sjwk") - 1))
            {
                if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                    nx_azure_iot_json_reader_token_string_get(&json_reader,
                                                              adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_sjwk,
                                                              NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_SJWK_SIZE,
                                                              &sjwk_size))
                {
                    return(NX_FALSE);
                }
            }
            else
            {
                return(NX_FALSE);
            }
        }
        else
        {
            break;
        }
    }

    /* Check if there are "alg" and "sjwk" properties.  */
    if ((alg_size == 0) || (sjwk_size == 0))
    {
        return(NX_FALSE);
    }

    /* Check if alg is supported.  */
    if ((alg_size != sizeof("RS256") - 1) || memcmp(alg_ptr, "RS256", alg_size))
    {
        return(NX_FALSE);
    }

    /* Step2. Verify signing key is signed by master key.  */
    
    /* Header:
       {
           "alg": "RS256",
           "kid": "ADU.200702.R"
       }
       
       Payload:
       {
           "kty": "RSA",
           "n": "xxx...xxx",
           "e": "AQAB",
           "alg": "RS256"
           "kid": "ADU.Signing.2020-04-29"
       }

       Signature:
    */

    /* Initialize.  */
    alg_size = 0;
    kid_size = 0;
    buffer_ptr = adu_agent_ptr -> nx_azure_iot_adu_agent_buffer;
    buffer_size = NX_AZURE_IOT_ADU_AGENT_BUFFER_SIZE;

    /* 2.1 Split sjwk header, payload and signature.  */
    if (nx_azure_iot_adu_agent_jws_split(adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_sjwk, sjwk_size,
                                         &jwk_header_b64, &jwk_header_b64_length,
                                         &jwk_payload_b64, &jwk_payload_b64_length,
                                         &jwk_signature_b64, &jwk_signature_b64_length) == NX_FALSE)
    {
        return(NX_FALSE);
    }

    /* 2.2 Decode sjwk header.  */
    if (nx_azure_iot_base64_decode((CHAR *)jwk_header_b64, jwk_header_b64_length,
                                   buffer_ptr, NX_AZURE_IOT_ADU_AGENT_BUFFER_SIZE, &bytes_copied))
    {
        return(NX_FALSE);
    }
    
    /* Initialize the header string as json.  */
    if (nx_azure_iot_json_reader_with_buffer_init(&json_reader, buffer_ptr, bytes_copied))
    {
        return(NX_FALSE);
    }
    buffer_ptr += bytes_copied;
    buffer_size -= bytes_copied;

    /* Skip the first begin object. */
    if ((nx_azure_iot_json_reader_next_token(&json_reader) != NX_AZURE_IOT_SUCCESS) ||
        (nx_azure_iot_json_reader_token_type(&json_reader) != NX_AZURE_IOT_READER_TOKEN_BEGIN_OBJECT))
    {
        return(NX_FALSE);
    }

    /* Loop to process all header data.  */
    while (nx_azure_iot_json_reader_next_token(&json_reader) == NX_AZURE_IOT_SUCCESS)
    {
        if (nx_azure_iot_json_reader_token_type(&json_reader) == NX_AZURE_IOT_READER_TOKEN_PROPERTY_NAME)
        {

            /* Get alg value.  */
            if (nx_azure_iot_json_reader_token_is_text_equal(&json_reader, (UCHAR *)"alg", sizeof("alg") - 1))
            {
                if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                    nx_azure_iot_json_reader_token_string_get(&json_reader,
                                                              buffer_ptr,
                                                              buffer_size,
                                                              &alg_size))
                {
                    return(NX_FALSE);
                }
                alg_ptr = buffer_ptr;
                buffer_ptr += alg_size;
                buffer_size -= alg_size;
            }

            /* Get kid value.  */
            else if (nx_azure_iot_json_reader_token_is_text_equal(&json_reader, (UCHAR *)"kid", sizeof("kid") - 1))
            {
                if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                    nx_azure_iot_json_reader_token_string_get(&json_reader,
                                                              buffer_ptr,
                                                              buffer_size,
                                                              &kid_size))
                {
                    return(NX_FALSE);
                }
                kid_ptr = buffer_ptr;
                buffer_ptr += kid_size;
                buffer_size -= kid_size;
            }
            else
            {
                return(NX_FALSE);
            }
        }
        else
        {
            break;
        }
    }

    /* Check if there are "alg" and "kid" properties.  */
    if ((alg_size == 0) || (kid_size == 0))
    {
        return(NX_FALSE);
    }

    /* Check if alg is supported.  */
    if ((alg_size != sizeof("RS256") - 1) || memcmp(alg_ptr, "RS256", alg_size))
    {
        return(NX_FALSE);
    }

    /* Search master key.  */
    rsa_root_key = (NX_AZURE_IOT_ADU_AGENT_RSA_ROOT_KEY *)nx_azure_iot_adu_agent_rsa_root_key_find(kid_ptr, kid_size);
    if (rsa_root_key == NX_NULL)
    {
        return(NX_FALSE);
    }

    /* 2.3 Decode sjwk signature.  */
    signature = adu_agent_ptr -> nx_azure_iot_adu_agent_buffer;
    signature_length = NX_AZURE_IOT_ADU_AGENT_BUFFER_SIZE;
    if (nx_azure_iot_base64_decode((CHAR *)jwk_signature_b64, jwk_signature_b64_length,
                                   signature, signature_length, &signature_length))
    {
        return(NX_FALSE);
    }

    /* 2.4 Verify signature.  */
    if (nx_azure_iot_adu_agent_rs256_verify(&adu_agent_ptr -> nx_azure_iot_adu_agent_crypto,
                                            jwk_header_b64, (jwk_header_b64_length + 1 + jwk_payload_b64_length),
                                            signature, signature_length,
                                            (UCHAR *)rsa_root_key -> n, rsa_root_key -> n_size,
                                            (UCHAR *)rsa_root_key -> e, rsa_root_key -> e_size,
                                            adu_agent_ptr -> nx_azure_iot_adu_agent_buffer + signature_length,
                                            NX_AZURE_IOT_ADU_AGENT_BUFFER_SIZE - signature_length) == NX_FALSE)
    {
        return(NX_FALSE);
    }

    /* Step3. Verify distroman signature is signed by the signing key.  */

    /* Initialize.  */
    kty_size = 0;
    n_size = 0;
    e_size = 0;
    kid_size = 0;
    buffer_ptr = adu_agent_ptr -> nx_azure_iot_adu_agent_buffer;
    buffer_size = NX_AZURE_IOT_ADU_AGENT_BUFFER_SIZE;

    /* 3.1 Decode sjwk payload to get the signing key.  */
    if (nx_azure_iot_base64_decode((CHAR *)jwk_payload_b64, jwk_payload_b64_length,
                                   buffer_ptr, buffer_size, &bytes_copied))
    {
        return(NX_FALSE);
    }
    
    /* Initialize the payload string as json.  */
    if (nx_azure_iot_json_reader_with_buffer_init(&json_reader, buffer_ptr, bytes_copied))
    {
        return(NX_FALSE);
    }
    buffer_ptr += bytes_copied;
    buffer_size -= bytes_copied;

    /* Skip the first begin object. */
    if ((nx_azure_iot_json_reader_next_token(&json_reader) != NX_AZURE_IOT_SUCCESS) ||
        (nx_azure_iot_json_reader_token_type(&json_reader) != NX_AZURE_IOT_READER_TOKEN_BEGIN_OBJECT))
    {
        return(NX_FALSE);
    }

    /* Loop to process all header data.  */
    while (nx_azure_iot_json_reader_next_token(&json_reader) == NX_AZURE_IOT_SUCCESS)
    {
        if (nx_azure_iot_json_reader_token_type(&json_reader) == NX_AZURE_IOT_READER_TOKEN_PROPERTY_NAME)
        {

            /* Get kty value.  */
            if (nx_azure_iot_json_reader_token_is_text_equal(&json_reader, (UCHAR *)"kty", sizeof("kty") - 1))
            {
                if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                    nx_azure_iot_json_reader_token_string_get(&json_reader,
                                                              buffer_ptr,
                                                              buffer_size,
                                                              &kty_size))
                {
                    return(NX_FALSE);
                }
                kty_ptr = buffer_ptr;
                buffer_ptr += kty_size;
                buffer_size -= kty_size;
            }

            /* Get n value.  */
            else if (nx_azure_iot_json_reader_token_is_text_equal(&json_reader, (UCHAR *)"n", sizeof("n") - 1))
            {
                if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                    nx_azure_iot_json_reader_token_string_get(&json_reader,
                                                              buffer_ptr,
                                                              buffer_size,
                                                              &n_b64_size))
                {
                    return(NX_FALSE);
                }
                n_b64_ptr = buffer_ptr;
                buffer_ptr += n_b64_size;
                buffer_size -= n_b64_size;
            }
            
            /* Get e value.  */
            else if (nx_azure_iot_json_reader_token_is_text_equal(&json_reader, (UCHAR *)"e", sizeof("e") - 1))
            {
                if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                    nx_azure_iot_json_reader_token_string_get(&json_reader,
                                                              buffer_ptr,
                                                              buffer_size,
                                                              &e_b64_size))
                {
                    return(NX_FALSE);
                }
                e_b64_ptr = buffer_ptr;
                buffer_ptr += e_b64_size;
                buffer_size -= e_b64_size;
            }
            
            /* Get alg value.  */
            else if (nx_azure_iot_json_reader_token_is_text_equal(&json_reader, (UCHAR *)"alg", sizeof("alg") - 1))
            {
                if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                    nx_azure_iot_json_reader_token_string_get(&json_reader,
                                                              buffer_ptr,
                                                              buffer_size,
                                                              &alg_size))
                {
                    return(NX_FALSE);
                }
                alg_ptr = buffer_ptr;
                buffer_ptr += alg_size;
                buffer_size -= alg_size;
            }
            
            /* Get kid value.  */
            else if (nx_azure_iot_json_reader_token_is_text_equal(&json_reader, (UCHAR *)"kid", sizeof("kid") - 1))
            {
                if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                    nx_azure_iot_json_reader_token_string_get(&json_reader,
                                                              buffer_ptr,
                                                              buffer_size,
                                                              &kid_size))
                {
                    return(NX_FALSE);
                }
                kid_ptr = buffer_ptr;
                buffer_ptr += kid_size;
                buffer_size -= kid_size;
            }

            else
            {
                return(NX_FALSE);
            }
        }
        else
        {
            break;
        }
    }

    /* Check if there are "alg" and "kid" properties.  */
    if ((kty_size == 0) || (n_b64_size == 0) || (e_b64_size == 0) || (kid_size == 0))
    {
        return(NX_FALSE);
    }
    
    /* Check if alg is supported.  */
    if ((alg_size != sizeof("RS256") - 1) || memcmp(alg_ptr, "RS256", alg_size))
    {
        return(NX_FALSE);
    }

    /* Check if alg is supported.  */
    if ((kty_size != sizeof("RSA") - 1) || memcmp(kty_ptr, "RSA", kty_size))
    {
        return(NX_FALSE);
    }

    /* 3.2 Use sjwk to decode n, e, signature and verify signature.  */
    buffer_ptr = adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_sjwk;
    buffer_size = NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_SJWK_SIZE;
    n_ptr = buffer_ptr;
    n_size = buffer_size;
    if (nx_azure_iot_base64_decode((CHAR *)n_b64_ptr, n_b64_size,
                                   n_ptr, n_size, &n_size))
    {
        return(NX_FALSE);
    }
    buffer_ptr += n_size;
    buffer_size -= n_size;

    e_ptr = buffer_ptr;
    e_size = buffer_size;
    if (nx_azure_iot_base64_decode((CHAR *)e_b64_ptr, e_b64_size,
                                   e_ptr, e_size, &e_size))
    {
        return(NX_FALSE);
    }
    buffer_ptr += e_size;
    buffer_size -= e_size;

    signature = buffer_ptr;
    signature_length = buffer_size;
    if (nx_azure_iot_base64_decode((CHAR *)signature_b64, signature_b64_length,
                                   signature, signature_length, &signature_length))
    {
        return(NX_FALSE);
    }
    buffer_ptr += signature_length;
    buffer_size -= signature_length;

    /* 3.3 Verify signature.  */
    if (nx_azure_iot_adu_agent_rs256_verify(&adu_agent_ptr -> nx_azure_iot_adu_agent_crypto,
                                            header_b64, (header_b64_length + 1 + payload_b64_length),
                                            signature, signature_length,
                                            n_ptr, n_size,
                                            e_ptr, e_size,
                                            buffer_ptr, buffer_size) == NX_FALSE)
    {
        return(NX_FALSE);
    }

    /* Step4. Verify distroman body digest (update manifest) matches what's in JWS payload section.  */

    /* Initialize.  */
    buffer_ptr = adu_agent_ptr -> nx_azure_iot_adu_agent_buffer;
    buffer_size = NX_AZURE_IOT_ADU_AGENT_BUFFER_SIZE;

    /* 4.1 Calculate update manifest sha256 value.  */
    sha256_generated_hash_ptr = buffer_ptr;
    status = nx_azure_iot_adu_agent_sha256_calculate(adu_agent_crypto -> method_sha256,
                                                     adu_agent_crypto -> method_sha256_metadata,
                                                     NX_AZURE_IOT_ADU_AGENT_SHA256_METADATA_SIZE,
                                                     adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest,
                                                     adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_size,
                                                     sha256_generated_hash_ptr,
                                                     NX_AZURE_IOT_ADU_AGENT_SHA256_HASH_SIZE);

    /* Check status.  */
    if (status)
    {
        return(NX_FALSE);
    }
    buffer_ptr += NX_AZURE_IOT_ADU_AGENT_SHA256_HASH_SIZE;
    buffer_size -= NX_AZURE_IOT_ADU_AGENT_SHA256_HASH_SIZE;

    /* 4.2 Decode the payload to get the sha256 base64 value.  */
    status = nx_azure_iot_base64_decode((CHAR *)payload_b64, payload_b64_length,
                                        buffer_ptr, buffer_size, &bytes_copied);

    /* Initialize the payload string as json.  */
    if (nx_azure_iot_json_reader_with_buffer_init(&json_reader, buffer_ptr, bytes_copied))
    {
        return(NX_FALSE);
    }
    buffer_ptr += bytes_copied;
    buffer_size -= bytes_copied;

    /* Skip the first begin object. */
    if ((nx_azure_iot_json_reader_next_token(&json_reader) != NX_AZURE_IOT_SUCCESS) ||
        (nx_azure_iot_json_reader_token_type(&json_reader) != NX_AZURE_IOT_READER_TOKEN_BEGIN_OBJECT))
    {
        return(NX_FALSE);
    }

    /* Get the next token. */
    if ((nx_azure_iot_json_reader_next_token(&json_reader) != NX_AZURE_IOT_SUCCESS) ||
        (nx_azure_iot_json_reader_token_type(&json_reader) != NX_AZURE_IOT_READER_TOKEN_PROPERTY_NAME))
    {
        return(NX_FALSE);
    }
    sha256_decoded_hash_64_ptr = buffer_ptr;

    /* Get sha256 base64 value.  */
    if (nx_azure_iot_json_reader_token_is_text_equal(&json_reader, (UCHAR *)"sha256", sizeof("sha256") - 1))
    {
        if (nx_azure_iot_json_reader_next_token(&json_reader) ||
            nx_azure_iot_json_reader_token_string_get(&json_reader,
                                                      sha256_decoded_hash_64_ptr,
                                                      buffer_size,
                                                      &bytes_copied))
        {
            return(NX_FALSE);
        }
    }
    else
    {
        return(NX_FALSE);
    }
    buffer_ptr += bytes_copied;
    buffer_size -= bytes_copied;

    sha256_decoded_hash_ptr = buffer_ptr;

    /* Decode sha256 base64 hash.  */
    if (nx_azure_iot_base64_decode((CHAR *)sha256_decoded_hash_64_ptr, NX_AZURE_IOT_ADU_AGENT_SHA256_HASH_BASE64_SIZE,
                                   sha256_decoded_hash_ptr, buffer_size, &bytes_copied))
    {
        return(NX_FALSE);
    }

    /* Verify the hash value.  */
    if (memcmp(sha256_generated_hash_ptr, sha256_decoded_hash_ptr, NX_AZURE_IOT_ADU_AGENT_SHA256_HASH_SIZE))
    {
        return(NX_FALSE);
    }

    return(NX_TRUE);
}

static UINT nx_azure_iot_adu_agent_jws_split(UCHAR *jws, UINT jws_length,
                                             UCHAR **header, UINT *header_length,
                                             UCHAR **payload, UINT *payload_length,
                                             UCHAR **signature, UINT *signature_length)
{

UCHAR *dot1_pointer;
UCHAR *dot2_pointer;
UINT   dot_count = 0;
UINT   i = 0;

    /* Set the header pointer.  */
    *header = jws;

    /* Loop to find the dots.  */
    while(i < jws_length)
    {
        if (*jws == '.')
        {
            dot_count ++;

            if (dot_count == 1)
            {
                dot1_pointer = jws;
            }
            else if (dot_count == 2)
            {
                dot2_pointer = jws;
            }
            else if (dot_count > 2)
            {
                return(NX_FALSE);
            }
        }
        jws ++;
        i ++;
    }

    /* Check if the dot count is correct.  */
    if ((dot_count != 2) || (dot2_pointer >= (*header + jws_length - 1)))
    {
        return(NX_FALSE);
    }

    /* Set the header, payload and signature.  */
    *header_length = (UINT)(dot1_pointer - *header);
    *payload = dot1_pointer + 1;
    *payload_length = (UINT)(dot2_pointer - *payload);
    *signature = dot2_pointer + 1;
    *signature_length = (UINT)((*header + jws_length) - *signature);

    return(NX_TRUE);
}

static VOID nx_azure_iot_adu_agent_state_update(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr, UINT state, NX_AZURE_IOT_ADU_AGENT_RESULT *adu_agent_result)
{
UINT old_state = adu_agent_ptr -> nx_azure_iot_adu_agent_last_reported_state;

    /* Report result.  */
    nx_azure_iot_adu_agent_client_reported_properties_send(adu_agent_ptr, state, NX_FALSE, NX_TRUE, adu_agent_result, NX_NO_WAIT);

    /* Update state.  */
    adu_agent_ptr -> nx_azure_iot_adu_agent_last_reported_state = state;

    /* Special case: if going to Idle, need to reset state.  */
    if (state == NX_AZURE_IOT_ADU_AGENT_STATE_IDLE)
    {
        nx_azure_iot_adu_agent_method_idle(adu_agent_ptr);

        adu_agent_ptr -> nx_azure_iot_adu_agent_operation_in_progress = NX_FALSE;
        adu_agent_ptr-> nx_azure_iot_adu_agent_operation_cancelled = NX_FALSE;
    }

    /* Check state change callback.  */
    if ((adu_agent_ptr -> nx_azure_iot_adu_agent_state_change_notify) && (old_state != state))
    {
        adu_agent_ptr -> nx_azure_iot_adu_agent_state_change_notify(adu_agent_ptr, state);
    }
}

static UINT nx_azure_iot_adu_agent_method_idle(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr)
{
    NX_PARAMETER_NOT_USED(adu_agent_ptr);

    /* FIXME: no operation.  */
    return(NX_AZURE_IOT_SUCCESS);
}

static UINT nx_azure_iot_adu_agent_method_download(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr)
{
UINT                status;
UCHAR              *buffer_ptr;
UINT                buffer_size;
NX_CRYPTO_METHOD   *sha256_method;
UCHAR              *sha256_method_metadata;
ULONG               sha256_method_metadata_size;
VOID               *handler;
NX_DNS             *dns_ptr;
NX_AZURE_IOT_ADU_AGENT_DOWNLOADER *downloader_ptr;
NX_AZURE_IOT_ADU_AGENT_DRIVER driver_request;

    /* Check state.  */
    if (adu_agent_ptr -> nx_azure_iot_adu_agent_last_reported_state != NX_AZURE_IOT_ADU_AGENT_STATE_IDLE)
    {
        return(NX_AZURE_IOT_FAILURE);
    }

    /* Get the update manifest sub content.  */
    if (nx_azure_iot_adu_agent_service_update_manifest_property_process(adu_agent_ptr))
    {
        return(NX_AZURE_IOT_FAILURE);
    }

    /* Check if this update is installed.  */
    if (nx_azure_iot_adu_agent_update_is_installed(adu_agent_ptr))
    {
        return(NX_AZURE_IOT_FAILURE);
    }

    /* Check if include download file.  FIXME: also check file number.  */
    if ((adu_agent_ptr -> nx_azure_iot_adu_agent_file_urls.file_url == NX_NULL) ||
        (adu_agent_ptr -> nx_azure_iot_adu_agent_file_urls.file_url_length == 0) ||
        (adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_content.file_sha256 == NX_NULL) ||
        (adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_content.file_sha256_length == 0) ||
        (adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_content.file_size_in_bytes == 0))
    {
        return(NX_AZURE_IOT_FAILURE);
    }

    /* Update state.  */
    nx_azure_iot_adu_agent_state_update(adu_agent_ptr, NX_AZURE_IOT_ADU_AGENT_STATE_DOWNLOAD_STARTED, NX_NULL);
    
    /* Output info.  */
    LogInfo(LogLiteralArgs("Firmware downloading..."));

    /* Initialization.  */
    downloader_ptr = &(adu_agent_ptr -> nx_azure_iot_adu_agent_downloader);
    dns_ptr = downloader_ptr -> dns_ptr;
    memset(downloader_ptr, 0, sizeof(NX_AZURE_IOT_ADU_AGENT_DOWNLOADER));
    downloader_ptr -> dns_ptr = dns_ptr;
    buffer_ptr = adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest;
    buffer_size = NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_SIZE;
    sha256_method = adu_agent_ptr -> nx_azure_iot_adu_agent_crypto.method_sha256;
    sha256_method_metadata = adu_agent_ptr -> nx_azure_iot_adu_agent_crypto.method_sha256_metadata;
    sha256_method_metadata_size = NX_AZURE_IOT_ADU_AGENT_SHA256_METADATA_SIZE;
    handler = adu_agent_ptr -> nx_azure_iot_adu_agent_crypto.handler;
    
    /* Send the preprocess request to the driver.   */
    driver_request.nx_azure_iot_adu_agent_driver_command = NX_AZURE_IOT_ADU_AGENT_DRIVER_PREPROCESS;
    driver_request.nx_azure_iot_adu_agent_driver_firmware_size = adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_content.file_size_in_bytes;
    driver_request.nx_azure_iot_adu_agent_driver_status = NX_AZURE_IOT_SUCCESS;
    (adu_agent_ptr -> nx_azure_iot_adu_agent_driver_entry)(&driver_request);
    
    /* Check status.  */
    if (driver_request.nx_azure_iot_adu_agent_driver_status)
    {
        LogError(LogLiteralArgs("Firmware download fail: DRIVER PREPROCESS ERROR"));
        return(NX_AZURE_IOT_FAILURE);
    }
    
    /* Initialize the sha256 for firmware hash. */
    status = sha256_method -> nx_crypto_init((NX_CRYPTO_METHOD*)sha256_method,
                                             NX_NULL,
                                             0,
                                             &handler,
                                             sha256_method_metadata,
                                             sha256_method_metadata_size);

    /* Check status.  */
    if (status)
    {
        LogError(LogLiteralArgs("Firmware download fail: SHA256 INIT ERROR"));
        return(NX_AZURE_IOT_FAILURE);
    }
    status = sha256_method -> nx_crypto_operation(NX_CRYPTO_HASH_INITIALIZE,
                                                  handler,
                                                  (NX_CRYPTO_METHOD*)sha256_method,
                                                  NX_NULL,
                                                  0,
                                                  NX_NULL,
                                                  0,
                                                  NX_NULL,
                                                  NX_NULL,
                                                  0,
                                                  sha256_method_metadata,
                                                  sha256_method_metadata_size,
                                                  NX_NULL,
                                                  NX_NULL); 

    /* Check status.  */
    if (status)
    {
        LogError(LogLiteralArgs("Firmware download fail: SHA256 INIT ERROR"));
        return(NX_AZURE_IOT_FAILURE);
    }

    /* Parse the url.  */
    status = nx_azure_iot_adu_agent_file_url_parse(adu_agent_ptr -> nx_azure_iot_adu_agent_file_urls.file_url,
                                                   adu_agent_ptr -> nx_azure_iot_adu_agent_file_urls.file_url_length,
                                                   buffer_ptr, buffer_size, downloader_ptr);

    /* Check status.  */
    if (status)
    {
        LogError(LogLiteralArgs("Firmware download fail: URL PARSE ERROR"));
        return(status);
    }

    /* Check if start dns query to get the address.  */
    if (downloader_ptr -> state ==  NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_URL_PARSED)
    {

        /* Start dns query.  */
        nx_azure_iot_adu_agent_dns_query(adu_agent_ptr);
    }
    else if (downloader_ptr -> state == NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_ADDRESS_DONE)
    {

        /* Start HTTP connect.  */
        nx_azure_iot_adu_agent_http_connect(adu_agent_ptr);
    }

    /* Return.  */
    return(status);
}

static UINT nx_azure_iot_adu_agent_method_install(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr)
{
NX_AZURE_IOT_ADU_AGENT_DRIVER driver_request;

    /* Check state.  */
    if (adu_agent_ptr -> nx_azure_iot_adu_agent_last_reported_state != NX_AZURE_IOT_ADU_AGENT_STATE_DOWNLOAD_SUCCEEDED)
    {
        return(NX_AZURE_IOT_FAILURE);
    }

    /* Update state.  */
    nx_azure_iot_adu_agent_state_update(adu_agent_ptr, NX_AZURE_IOT_ADU_AGENT_STATE_INSTALL_STARTED, NX_NULL);

    /* Output info.  */
    LogInfo(LogLiteralArgs("Firmware installing..."));

    /* Send the firmware install request to the driver.   */
    driver_request.nx_azure_iot_adu_agent_driver_command = NX_AZURE_IOT_ADU_AGENT_DRIVER_INSTALL;    
    driver_request.nx_azure_iot_adu_agent_driver_status = NX_AZURE_IOT_SUCCESS;
    (adu_agent_ptr -> nx_azure_iot_adu_agent_driver_entry)(&driver_request);
    
    /* Install firmware.  */
    if (driver_request.nx_azure_iot_adu_agent_driver_status)
    {
        LogError(LogLiteralArgs("Firmware install fail: DRIVER ERROR"));
        return(NX_AZURE_IOT_FAILURE);
    }

    /* Output info.  */
    LogInfo(LogLiteralArgs("Firmware installed"));

    /* Install complete, update state.  */
    nx_azure_iot_adu_agent_state_update(adu_agent_ptr, NX_AZURE_IOT_ADU_AGENT_STATE_INSTALL_SUCCEEDED, NX_NULL);

    return(NX_AZURE_IOT_SUCCESS);
}

static UINT nx_azure_iot_adu_agent_method_apply(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr)
{

    /* Check state.  */
    if (adu_agent_ptr -> nx_azure_iot_adu_agent_last_reported_state != NX_AZURE_IOT_ADU_AGENT_STATE_INSTALL_SUCCEEDED)
    {
        return(NX_AZURE_IOT_FAILURE);
    }

    /* Update state.  */
    nx_azure_iot_adu_agent_state_update(adu_agent_ptr, NX_AZURE_IOT_ADU_AGENT_STATE_APPLY_STARTED, NX_NULL);

    /* Let users call nx_azure_iot_adu_agent_update_apply() to apply update.  */
    return(NX_AZURE_IOT_SUCCESS);
}

static UINT nx_azure_iot_adu_agent_method_cancel(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr)
{
    NX_PARAMETER_NOT_USED(adu_agent_ptr);

    /* FIXME: */
    return(NX_AZURE_IOT_SUCCESS);
}

static UINT nx_azure_iot_adu_agent_duplicate_request_check(UINT action, UINT last_reported_state)
{

UINT is_duplicate_request = NX_FALSE;

    switch (action)
    {
        case NX_AZURE_IOT_ADU_AGENT_ACTION_DOWNLOAD:
        {
            is_duplicate_request = ((last_reported_state == NX_AZURE_IOT_ADU_AGENT_STATE_DOWNLOAD_STARTED) ||
                                    (last_reported_state == NX_AZURE_IOT_ADU_AGENT_STATE_DOWNLOAD_SUCCEEDED));

            break;
        }

        case NX_AZURE_IOT_ADU_AGENT_ACTION_INSTALL:
        {
            is_duplicate_request = ((last_reported_state == NX_AZURE_IOT_ADU_AGENT_STATE_INSTALL_STARTED) || 
                                    (last_reported_state == NX_AZURE_IOT_ADU_AGENT_STATE_INSTALL_SUCCEEDED));

            break;
        }

        case NX_AZURE_IOT_ADU_AGENT_ACTION_APPLY:
        {
            is_duplicate_request = ((last_reported_state == NX_AZURE_IOT_ADU_AGENT_STATE_APPLY_STARTED) ||
                                    (last_reported_state == NX_AZURE_IOT_ADU_AGENT_STATE_IDLE));
            break;
        }

        case NX_AZURE_IOT_ADU_AGENT_ACTION_CANCEL:
        {
            // Cancel is considered a duplicate action when in the Idle state.
            // This is because one of the purposes of cancel is to get
            // the client back to the idle state. If the client is already
            // in the idle state, there is no operation to cancel.
            // Also, if a cancel is processed, the client will be in the idle state.
            // If the same cancel is sent/received again, the device would already be in the
            // idle state and the client should take no action on the duplicate cancel.
            is_duplicate_request = (last_reported_state == NX_AZURE_IOT_ADU_AGENT_STATE_IDLE);
            break;
        }
        default:
        {
            break;
        }
    }

    return (is_duplicate_request);
}

static UINT nx_azure_iot_adu_agent_update_is_installed(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr)
{

NX_AZURE_IOT_ADU_AGENT_UPDATE_ID *current_update_id = &(adu_agent_ptr -> nx_azure_iot_adu_agent_current_update_id);
NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_CONTENT *manifest = &(adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_content);

    /* Check if update id exists.  */
    if ((manifest -> provider == NX_NULL) ||
        (manifest -> provider_length == 0) ||
        (manifest -> name == NX_NULL) ||
        (manifest -> name_length == 0) ||
        (manifest -> version == NX_NULL) ||
        (manifest -> version_length == 0))
    {
        return(NX_FALSE);
    }

    /* Check if already installed this update.  */
    if ((manifest -> provider_length == current_update_id -> provider_length) &&
        (!memcmp(manifest -> provider, current_update_id -> provider, current_update_id -> provider_length)) &&
        (manifest -> name_length == current_update_id -> name_length) &&
        (!memcmp(manifest ->name, current_update_id -> name, current_update_id -> name_length)) &&
        (manifest -> version_length == current_update_id -> version_length) &&
        (!memcmp(manifest -> version, current_update_id -> version, current_update_id -> version_length)))
    {
        return(NX_TRUE);
    }

    return(NX_FALSE);
}

static UINT nx_azure_iot_adu_agent_service_properties_get(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr,
                                                          NX_AZURE_IOT_JSON_READER *json_reader_ptr)
{
UINT action_flag = 0;
NX_AZURE_IOT_ADU_AGENT_FILE_URLS *file_urls = &(adu_agent_ptr -> nx_azure_iot_adu_agent_file_urls);


    /* Initialization.  */
    adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_size = 0;
    adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_signature_size = 0;
    memset(file_urls, 0, sizeof (NX_AZURE_IOT_ADU_AGENT_FILE_URLS));

    /* Skip service property.  */
    nx_azure_iot_json_reader_next_token(json_reader_ptr);

    /* Next one should be begin object.  */
    if (nx_azure_iot_json_reader_token_type(json_reader_ptr) != NX_AZURE_IOT_READER_TOKEN_BEGIN_OBJECT)
    {
        return(NX_NOT_SUCCESSFUL);
    }

    /* Loop to process all data.  */
    while (nx_azure_iot_json_reader_next_token(json_reader_ptr) == NX_AZURE_IOT_SUCCESS)
    {
        if (nx_azure_iot_json_reader_token_type(json_reader_ptr) == NX_AZURE_IOT_READER_TOKEN_PROPERTY_NAME)
        {

            /* Action.  */
            if (nx_azure_iot_json_reader_token_is_text_equal(json_reader_ptr,
                                                             (UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_ACTION,
                                                             sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_ACTION) - 1))
            {

                /* Get action data.  */
                if (nx_azure_iot_json_reader_next_token(json_reader_ptr) ||
                    nx_azure_iot_json_reader_token_int32_get(json_reader_ptr, (int32_t *)&adu_agent_ptr -> nx_azure_iot_adu_agent_action))
                {
                    return(NX_NOT_SUCCESSFUL);
                }

                /* Update the flag.  */
                action_flag = NX_TRUE;
            }

            /* Update manifest.  */
            if (nx_azure_iot_json_reader_token_is_text_equal(json_reader_ptr,
                                                             (UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_UPDATE_MANIFEST,
                                                             sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_UPDATE_MANIFEST) - 1))
            {

                /* Get update manifest string.  */
                if (nx_azure_iot_json_reader_next_token(json_reader_ptr) ||
                    nx_azure_iot_json_reader_token_string_get(json_reader_ptr,
                                                              adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest,
                                                              NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_SIZE,
                                                              &(adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_size)))
                {
                    return(NX_NOT_SUCCESSFUL);
                }
            }

            /* Update manifest signature.  */
            else if (nx_azure_iot_json_reader_token_is_text_equal(json_reader_ptr,
                                                                  (UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_UPDATE_MANIFEST_SIGNATURE,
                                                                  sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_UPDATE_MANIFEST_SIGNATURE) - 1))
            {

                /* Get update manifest signature.  */
                if (nx_azure_iot_json_reader_next_token(json_reader_ptr) ||
                    nx_azure_iot_json_reader_token_string_get(json_reader_ptr,
                                                              adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_signature,
                                                              NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_SIGNATURE_SIZE,
                                                              &(adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_signature_size)))
                {
                    return(NX_NOT_SUCCESSFUL);
                }
            }

            /* File URLs. 
               Note: 1. file urls property can exist or not.
                     2. file urls property value can be object.  */
            else if (nx_azure_iot_json_reader_token_is_text_equal(json_reader_ptr,
                                                                  (UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_FILEURLS,
                                                                  sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_FILEURLS) - 1))
            {

                /*  Skip the file urls property name.  */
                if (nx_azure_iot_json_reader_next_token(json_reader_ptr))
                {
                    return(NX_NOT_SUCCESSFUL);
                }

                /* Check the token type.  */
                if (nx_azure_iot_json_reader_token_type(json_reader_ptr) == NX_AZURE_IOT_READER_TOKEN_BEGIN_OBJECT)
                {

                    /* Get file number and file url.  */
                    if (nx_azure_iot_json_reader_next_token(json_reader_ptr) ||
                        nx_azure_iot_json_reader_token_string_get(json_reader_ptr,
                                                                  file_urls -> file_buffer,
                                                                  NX_AZURE_IOT_ADU_AGENT_FILE_URLS_SIZE,
                                                                  &(file_urls -> file_number_length)))
                    {
                        return(NX_NOT_SUCCESSFUL);
                    }

                    /* Set file number pointer and update the buffer size.  */
                    file_urls -> file_number = file_urls -> file_buffer;

                    /* Get file url.  */
                    if (nx_azure_iot_json_reader_next_token(json_reader_ptr) ||
                        nx_azure_iot_json_reader_token_string_get(json_reader_ptr,
                                                                  file_urls -> file_buffer + file_urls -> file_number_length,
                                                                  NX_AZURE_IOT_ADU_AGENT_FILE_URLS_SIZE - file_urls -> file_number_length,
                                                                  &(file_urls -> file_url_length)))
                    {
                        return(NX_NOT_SUCCESSFUL);
                    }

                    /* Set file url pointer.  */
                    file_urls -> file_url = file_urls -> file_buffer + file_urls -> file_number_length;

                    /* Skip the end object.  */ 
                    if (nx_azure_iot_json_reader_next_token(json_reader_ptr) ||
                        nx_azure_iot_json_reader_token_type(json_reader_ptr) != NX_AZURE_IOT_READER_TOKEN_END_OBJECT)
                    {
                        return(NX_NOT_SUCCESSFUL);
                    }
                }
            }

            /* Skip the unknow properties.  */
            else
            {
                if (nx_azure_iot_json_reader_skip_children(json_reader_ptr))
                {
                    return(NX_NOT_SUCCESSFUL);
                }
            }
        }
        else if (nx_azure_iot_json_reader_token_type(json_reader_ptr) ==
                    NX_AZURE_IOT_READER_TOKEN_END_OBJECT)
        {
            break;
        }
    }

    /* Check if there is action flag.  */
    if (action_flag != NX_TRUE)
    {
        return(NX_NOT_SUCCESSFUL);
    }

    return(NX_AZURE_IOT_SUCCESS);
}

static UINT nx_azure_iot_adu_agent_service_update_manifest_property_process(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr)
{

UCHAR *buffer_ptr = adu_agent_ptr -> nx_azure_iot_adu_agent_buffer;
UINT buffer_size = NX_AZURE_IOT_ADU_AGENT_BUFFER_SIZE;
NX_AZURE_IOT_JSON_READER json_reader;
NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_CONTENT *update_manifest_content = &(adu_agent_ptr ->nx_azure_iot_adu_agent_update_manifest_content);


    /* Initialization.  */
    memset(update_manifest_content, 0, sizeof (NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_CONTENT));

    /* updateManifest property value can be object or null string.  */
    if (adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_size == 0)
    {
        return (NX_AZURE_IOT_SUCCESS);
    }

    /* Initialize the update manifest string as json.  */
    if (nx_azure_iot_json_reader_with_buffer_init(&json_reader,
                                                  adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest,
                                                  adu_agent_ptr ->nx_azure_iot_adu_agent_update_manifest_size))
    {
        return(NX_NOT_SUCCESSFUL);
    }

    /* Skip the first begin object. */
    if ((nx_azure_iot_json_reader_next_token(&json_reader) != NX_AZURE_IOT_SUCCESS) ||
        (nx_azure_iot_json_reader_token_type(&json_reader) != NX_AZURE_IOT_READER_TOKEN_BEGIN_OBJECT))
    {
        return(NX_NOT_SUCCESSFUL);
    }

    /* Loop to process all data.  */
    while (nx_azure_iot_json_reader_next_token(&json_reader) == NX_AZURE_IOT_SUCCESS)
    {
        if (nx_azure_iot_json_reader_token_type(&json_reader) == NX_AZURE_IOT_READER_TOKEN_PROPERTY_NAME)
        {

            /* Manifest version.  */
            if (nx_azure_iot_json_reader_token_is_text_equal(&json_reader,
                                                             (UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_MANIFEST_VERSION,
                                                             sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_MANIFEST_VERSION) - 1))
            {

                /* Get manifest version value.  */
                if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                    nx_azure_iot_json_reader_token_string_get(&json_reader,
                                                              buffer_ptr,
                                                              buffer_size,
                                                              &(update_manifest_content -> manifest_version_length)))
                {
                    return(NX_NOT_SUCCESSFUL);
                }

                /* Set file number pointer and update the buffer size.  */
                NX_AZURE_IOT_ADU_AGENT_PTR_UPDATE(update_manifest_content -> manifest_version,
                                                  update_manifest_content -> manifest_version_length,
                                                  buffer_ptr, buffer_size);
            }

            /* Update id.  */
            else if (nx_azure_iot_json_reader_token_is_text_equal(&json_reader,
                                                                  (UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_UPDATE_ID,
                                                                  sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_UPDATE_ID) - 1))
            {

                /* Skip the first begin object. */
                if ((nx_azure_iot_json_reader_next_token(&json_reader) != NX_AZURE_IOT_SUCCESS) ||
                    (nx_azure_iot_json_reader_token_type(&json_reader) != NX_AZURE_IOT_READER_TOKEN_BEGIN_OBJECT))
                {
                    return(NX_NOT_SUCCESSFUL);
                }

                /* Loop to process all update id field.  */
                while (nx_azure_iot_json_reader_next_token(&json_reader) == NX_AZURE_IOT_SUCCESS)
                {
                    if (nx_azure_iot_json_reader_token_type(&json_reader) == NX_AZURE_IOT_READER_TOKEN_PROPERTY_NAME)
                    {

                        /* Provider.  */
                        if (nx_azure_iot_json_reader_token_is_text_equal(&json_reader,
                                                                         (UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_PROVIDER,
                                                                         sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_PROVIDER) - 1))
                        {

                            /* Get the provider value.  */
                            if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                                nx_azure_iot_json_reader_token_string_get(&json_reader,
                                                                          buffer_ptr,
                                                                          buffer_size,
                                                                          &(update_manifest_content -> provider_length)))
                            {
                                return(NX_NOT_SUCCESSFUL);
                            }

                            /* Set file number pointer and update the buffer size.  */
                            NX_AZURE_IOT_ADU_AGENT_PTR_UPDATE(update_manifest_content -> provider,
                                                              update_manifest_content -> provider_length,
                                                              buffer_ptr, buffer_size);
                        }
                        
                        /* Name.  */
                        else if (nx_azure_iot_json_reader_token_is_text_equal(&json_reader,
                                                                              (UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_NAME,
                                                                              sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_NAME) - 1))
                        {

                            /* Get the name value.  */
                            if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                                nx_azure_iot_json_reader_token_string_get(&json_reader,
                                                                          buffer_ptr,
                                                                          buffer_size,
                                                                          &(update_manifest_content -> name_length)))
                            {
                                return(NX_NOT_SUCCESSFUL);
                            }

                            /* Set file number pointer and update the buffer size.  */
                            NX_AZURE_IOT_ADU_AGENT_PTR_UPDATE(update_manifest_content -> name,
                                                              update_manifest_content -> name_length,
                                                              buffer_ptr, buffer_size);
                        }

                        /* Version.  */
                        else if (nx_azure_iot_json_reader_token_is_text_equal(&json_reader,
                                                                              (UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_VERSION,
                                                                              sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_VERSION) - 1))
                        {

                            /* Get the version value.  */
                            if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                                nx_azure_iot_json_reader_token_string_get(&json_reader,
                                                                          buffer_ptr,
                                                                          buffer_size,
                                                                          &(update_manifest_content -> version_length)))
                            {
                                return(NX_NOT_SUCCESSFUL);
                            }

                            /* Set file number pointer and update the buffer size.  */
                            NX_AZURE_IOT_ADU_AGENT_PTR_UPDATE(update_manifest_content -> version,
                                                              update_manifest_content -> version_length,
                                                              buffer_ptr, buffer_size);
                        }

                        /* Skip the unknow properties.  */
                        else
                        {
                            if (nx_azure_iot_json_reader_skip_children(&json_reader))
                            {
                                return(NX_NOT_SUCCESSFUL);
                            }
                        }
                    }

                    else if (nx_azure_iot_json_reader_token_type(&json_reader) ==
                                NX_AZURE_IOT_READER_TOKEN_BEGIN_OBJECT)
                    {
                        if (nx_azure_iot_json_reader_skip_children(&json_reader))
                        {
                            return(NX_NOT_SUCCESSFUL);
                        }
                    }
                    else if (nx_azure_iot_json_reader_token_type(&json_reader) ==
                                NX_AZURE_IOT_READER_TOKEN_END_OBJECT)
                    {
                        break;
                    }
                }
            }

            /* Update type.  */
            else if (nx_azure_iot_json_reader_token_is_text_equal(&json_reader,
                                                                    (UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_UPDATE_TYPE,
                                                                    sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_UPDATE_TYPE) - 1))
            {

                /* Get the update type value.  */
                if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                    nx_azure_iot_json_reader_token_string_get(&json_reader,
                                                              buffer_ptr,
                                                              buffer_size,
                                                              &(update_manifest_content -> update_type_length)))
                {
                    return(NX_NOT_SUCCESSFUL);
                }

                /* Set file number pointer and update the buffer size.  */
                NX_AZURE_IOT_ADU_AGENT_PTR_UPDATE(update_manifest_content -> update_type,
                                                  update_manifest_content -> update_type_length,
                                                  buffer_ptr, buffer_size);
            }
            
            /* Installed criteria.  */
            else if (nx_azure_iot_json_reader_token_is_text_equal(&json_reader,
                                                                  (UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_INSTALLED_CRITERIA,
                                                                  sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_INSTALLED_CRITERIA) - 1))
            {

                /* Get the installed criteria value.  */
                if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                    nx_azure_iot_json_reader_token_string_get(&json_reader,
                                                              buffer_ptr,
                                                              buffer_size,
                                                              &(update_manifest_content -> installed_criteria_length)))
                {
                    return(NX_NOT_SUCCESSFUL);
                }

                /* Set file number pointer and update the buffer size.  */
                NX_AZURE_IOT_ADU_AGENT_PTR_UPDATE(update_manifest_content -> installed_criteria,
                                                  update_manifest_content -> installed_criteria_length,
                                                  buffer_ptr, buffer_size);
            }

            /* Files.  */
            else if (nx_azure_iot_json_reader_token_is_text_equal(&json_reader,
                                                                  (UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_FILES,
                                                                  sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_FILES) - 1))
            {

                /* Store one file.  */

                /* Skip the first begin object of files property. */
                if ((nx_azure_iot_json_reader_next_token(&json_reader) != NX_AZURE_IOT_SUCCESS) ||
                    (nx_azure_iot_json_reader_token_type(&json_reader) != NX_AZURE_IOT_READER_TOKEN_BEGIN_OBJECT))
                {
                    return(NX_NOT_SUCCESSFUL);
                }

                /* Skip the file number property.  */
                if ((nx_azure_iot_json_reader_next_token(&json_reader) != NX_AZURE_IOT_SUCCESS) ||
                    (nx_azure_iot_json_reader_token_type(&json_reader) != NX_AZURE_IOT_READER_TOKEN_PROPERTY_NAME))
                {
                    return(NX_NOT_SUCCESSFUL);
                }

                /* Skip the first begin object of file number property. */
                if ((nx_azure_iot_json_reader_next_token(&json_reader) != NX_AZURE_IOT_SUCCESS) ||
                    (nx_azure_iot_json_reader_token_type(&json_reader) != NX_AZURE_IOT_READER_TOKEN_BEGIN_OBJECT))
                {
                    return(NX_NOT_SUCCESSFUL);
                }

                /* Filename.  */
                if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                    !nx_azure_iot_json_reader_token_is_text_equal(&json_reader,
                                                                  (UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_FILE_NAME,
                                                                  sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_FILE_NAME) - 1))
                {
                    return(NX_NOT_SUCCESSFUL);
                }

                /* Get the file name value.  */
                if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                    nx_azure_iot_json_reader_token_string_get(&json_reader,
                                                              buffer_ptr,
                                                              buffer_size,
                                                              &(update_manifest_content -> file_name_length)))
                {
                    return(NX_NOT_SUCCESSFUL);
                }

                /* Set file name pointer and update the buffer size.  */
                NX_AZURE_IOT_ADU_AGENT_PTR_UPDATE(update_manifest_content -> file_name,
                                                  update_manifest_content -> file_name_length,
                                                  buffer_ptr, buffer_size);

                /* Size in bytes.  */
                if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                    !nx_azure_iot_json_reader_token_is_text_equal(&json_reader,
                                                                  (UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_SIZE_IN_BYTES,
                                                                  sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_SIZE_IN_BYTES) - 1))
                {
                    return(NX_NOT_SUCCESSFUL);
                }

                /* Get the size in bytes.  */
                if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                    nx_azure_iot_json_reader_token_int32_get(&json_reader,
                                                             (int32_t *)&(update_manifest_content -> file_size_in_bytes)))
                {
                    return(NX_NOT_SUCCESSFUL);
                }

                /* Get the next property.  */
                if ((nx_azure_iot_json_reader_next_token(&json_reader) != NX_AZURE_IOT_SUCCESS) ||
                    (nx_azure_iot_json_reader_token_type(&json_reader) != NX_AZURE_IOT_READER_TOKEN_PROPERTY_NAME))
                {
                    return(NX_NOT_SUCCESSFUL);
                }

                /* Hashes.  */
                if (!nx_azure_iot_json_reader_token_is_text_equal(&json_reader,
                                                                  (UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_HASHES,
                                                                  sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_HASHES) - 1))
                {
                    return(NX_NOT_SUCCESSFUL);
                }

                /* Skip the first begin object of files property. */
                if ((nx_azure_iot_json_reader_next_token(&json_reader) != NX_AZURE_IOT_SUCCESS) ||
                    (nx_azure_iot_json_reader_token_type(&json_reader) != NX_AZURE_IOT_READER_TOKEN_BEGIN_OBJECT))
                {
                    return(NX_NOT_SUCCESSFUL);
                }

                /* sha256.  */
                if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                    !nx_azure_iot_json_reader_token_is_text_equal(&json_reader,
                                                                  (UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_SHA256,
                                                                  sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_SHA256) - 1))
                {
                    return(NX_NOT_SUCCESSFUL);
                }

                /* Get the sha256 value value.  */
                if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                    nx_azure_iot_json_reader_token_string_get(&json_reader,
                                                              buffer_ptr,
                                                              buffer_size,
                                                              &(update_manifest_content -> file_sha256_length)))
                {
                    return(NX_NOT_SUCCESSFUL);
                }

                /* Set file number pointer and update the buffer size.  */
                NX_AZURE_IOT_ADU_AGENT_PTR_UPDATE(update_manifest_content -> file_sha256,
                                                  update_manifest_content -> file_sha256_length,
                                                  buffer_ptr, buffer_size);
                
                /* Skip the end object.  */ 
                for (UINT i = 0; i < 3; i ++)
                {
                    if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                        nx_azure_iot_json_reader_token_type(&json_reader) != NX_AZURE_IOT_READER_TOKEN_END_OBJECT)
                    {
                        return(NX_NOT_SUCCESSFUL);
                    }
                }
            }

            /* Created date time.  */
            else if (nx_azure_iot_json_reader_token_is_text_equal(&json_reader,
                                                                  (UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_CREATED_DATE_TIME,
                                                                  sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_CREATED_DATE_TIME) - 1))
            {

                /* Get the created date time value.  */
                if (nx_azure_iot_json_reader_next_token(&json_reader) ||
                    nx_azure_iot_json_reader_token_string_get(&json_reader,
                                                              buffer_ptr,
                                                              buffer_size,
                                                              &(update_manifest_content -> created_date_time_length)))
                {
                    return(NX_NOT_SUCCESSFUL);
                }

                /* Set file number pointer and update the buffer size.  */
                NX_AZURE_IOT_ADU_AGENT_PTR_UPDATE(update_manifest_content -> created_date_time,
                                                  update_manifest_content -> created_date_time_length,
                                                  buffer_ptr, buffer_size);
            }

            /* Skip the unknow properties.  */
            else
            {
                if (nx_azure_iot_json_reader_skip_children(&json_reader))
                {
                    return(NX_NOT_SUCCESSFUL);
                }
            }
        }

        else if (nx_azure_iot_json_reader_token_type(&json_reader) ==
                    NX_AZURE_IOT_READER_TOKEN_BEGIN_OBJECT)
        {
            if (nx_azure_iot_json_reader_skip_children(&json_reader))
            {
                return(NX_NOT_SUCCESSFUL);
            }
        }
        else if (nx_azure_iot_json_reader_token_type(&json_reader) ==
                    NX_AZURE_IOT_READER_TOKEN_END_OBJECT)
        {
            break;
        }
    }

    return(NX_AZURE_IOT_SUCCESS);
}

/* FIXME: adu_core_interface.c  */

/* client reported properties sample:

    {
        "azureDeviceUpdateAgent": {
            "__t": "c",
            "client": {
                "state": 0,
                "installedUpdateId": "{\"provider\":\"Microsoft\",\"Name\":\"MS-Board\",\"Version\":\"1.0\"}",
                "deviceProperties": {
                    "manufacturer": "Microsoft",
                    "model": "MS-Board"
                },
                "resultCode": 200,
                "extendedResultCode": 0
            }
        }
    }

*/

/**
 * @brief Send client reported properties, including Report state, and optionally installedUpdateID, 
 *        deviceProperties and result to service.
 *
 * @param updateState state to report.
 * @param result Result to report (optional, can be NULL).
 */
static UINT nx_azure_iot_adu_agent_client_reported_properties_send(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr,
                                                                   UINT adu_agent_state,
                                                                   UINT installed_update_id_flag,
                                                                   UINT device_properties_flag,
                                                                   NX_AZURE_IOT_ADU_AGENT_RESULT *adu_agent_result,
                                                                   UINT wait_option)
{

NX_AZURE_IOT_JSON_WRITER json_writer;
NX_AZURE_IOT_ADU_AGENT_DEVICE_PROPERTIES *device_properties = &(adu_agent_ptr -> nx_azure_iot_adu_agent_device_properties);
UINT status;
UINT response_status;
UINT request_id;
ULONG reported_property_version;

    /* Create json writer for client reported property.  FIXME: wait option.  */
    status = nx_azure_iot_pnp_client_reported_properties_create(adu_agent_ptr -> nx_azure_iot_pnp_client_ptr, &json_writer, wait_option);
    if (status)
    {
        return(status);
    }

    /* Fill the ADU agent component name.  */
    status = nx_azure_iot_pnp_client_reported_property_component_begin(adu_agent_ptr -> nx_azure_iot_pnp_client_ptr,
                                                                       &json_writer, 
                                                                       (const UCHAR *)NX_AZURE_IOT_ADU_AGENT_COMPONENT_NAME,
                                                                       sizeof(NX_AZURE_IOT_ADU_AGENT_COMPONENT_NAME) - 1);
    if (status)
    {
        nx_azure_iot_json_writer_deinit(&json_writer);
        return(status);
    }

    /* Fill the client property name.  */
    if (nx_azure_iot_json_writer_append_property_name(&json_writer,
                                                      (const UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_CLIENT,
                                                      sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_CLIENT) - 1))
    {
        nx_azure_iot_json_writer_deinit(&json_writer);
        return(NX_NOT_SUCCESSFUL);
    }

    /* Start to fill client property value.   */
    if (nx_azure_iot_json_writer_append_begin_object(&json_writer))
    {
        nx_azure_iot_json_writer_deinit(&json_writer);
        return(NX_NOT_SUCCESSFUL);
    }

    /* Fill the state.   */
    if (nx_azure_iot_json_writer_append_property_with_int32_value(&json_writer,
                                                                  (const UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_STATE,
                                                                  sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_STATE) - 1,
                                                                  (INT)adu_agent_state))
    {
        nx_azure_iot_json_writer_deinit(&json_writer);
        return(NX_NOT_SUCCESSFUL);
    }

    /* Fill installed update id flag.  */
    if (installed_update_id_flag)
    {
        if (nx_azure_iot_json_writer_append_property_with_string_value(&json_writer,
                                                                       (const UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_INSTALLED_CONTENT_ID,
                                                                       sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_INSTALLED_CONTENT_ID) - 1,
                                                                       adu_agent_ptr -> nx_azure_iot_adu_agent_current_update_id.update_id_buffer,
                                                                       adu_agent_ptr -> nx_azure_iot_adu_agent_current_update_id.update_id_length))
        {
            nx_azure_iot_json_writer_deinit(&json_writer);
            return (NX_NOT_SUCCESSFUL);
        }
    }

    /* Fill the deviceProperties.  */
    if (device_properties_flag)
    {
        if ((nx_azure_iot_json_writer_append_property_name(&json_writer,
                                                           (const UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_DEVICEPROPERTIES,
                                                           sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_DEVICEPROPERTIES) - 1)) ||
            (nx_azure_iot_json_writer_append_begin_object(&json_writer)) ||
            (nx_azure_iot_json_writer_append_property_with_string_value(&json_writer,
                                                                        (const UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_MANUFACTURER,
                                                                        sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_MANUFACTURER) - 1,
                                                                        device_properties -> manufacturer, device_properties -> manufacturer_length)) ||
            (nx_azure_iot_json_writer_append_property_with_string_value(&json_writer,
                                                                        (const UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_MODEL,
                                                                        sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_MODEL) - 1,
                                                                        device_properties -> model, device_properties -> model_length)) ||
           (nx_azure_iot_json_writer_append_end_object(&json_writer)))
        {
            nx_azure_iot_json_writer_deinit(&json_writer);
            return(NX_NOT_SUCCESSFUL);
        }
    }

    /* Fill the result.  */
    if (adu_agent_result)
    {
        if ((nx_azure_iot_json_writer_append_property_with_int32_value(&json_writer,
                                                                       (const UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_RESULT_CODE,
                                                                       sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_RESULT_CODE) - 1,
                                                                       (INT)adu_agent_result -> result_code)) ||
            (nx_azure_iot_json_writer_append_property_with_int32_value(&json_writer,
                                                                       (const UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_EXTENDED_RESULT_CODE,
                                                                       sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_EXTENDED_RESULT_CODE) - 1,
                                                                       (INT)adu_agent_result -> extended_result_code)))

        {
            nx_azure_iot_json_writer_deinit(&json_writer);
            return(NX_NOT_SUCCESSFUL);
        }
    }

    /* End the client property value.  */
    if (nx_azure_iot_json_writer_append_end_object(&json_writer))
    {
        nx_azure_iot_json_writer_deinit(&json_writer);
        return(NX_NOT_SUCCESSFUL);
    }

    /* End ADU agent component.  */
    if (nx_azure_iot_pnp_client_reported_property_component_end(adu_agent_ptr -> nx_azure_iot_pnp_client_ptr, &json_writer))
    {
        nx_azure_iot_json_writer_deinit(&json_writer);
        return(NX_NOT_SUCCESSFUL);
    }

    /* Send device info reported properties message to IoT Hub.  */
    status = nx_azure_iot_pnp_client_reported_properties_send(adu_agent_ptr -> nx_azure_iot_pnp_client_ptr, &json_writer,
                                                              &request_id, &response_status,
                                                              &reported_property_version, wait_option);
    if (status)
    {
        nx_azure_iot_json_writer_deinit(&json_writer);
        return(status);
    }

    /* Check the response statue for blocking.  */
    if (wait_option)
    {
        if ((response_status < 200) || (response_status >= 300))
        {
            return(NX_NOT_SUCCESSFUL);
        }
    }

    /* Deinit the json writer.  */
    nx_azure_iot_json_writer_deinit(&json_writer);

    return(NX_AZURE_IOT_SUCCESS);
}

/* service reported properties sample:

    {
        "azureDeviceUpdateAgent": {
            "__t": "c",
            "<service>": {
                "ac": <ack_code>,
                "av": <ack_version>,
                "ad": "<ack_description>",
                "value": <user_value>
            }
        }
    }
*/

/**
 * @brief Send service reported properties.
 * 
 * @param updateState state to report.
 * @param result Result to report (optional, can be NULL).
 */
static UINT nx_azure_iot_adu_agent_service_reported_properties_send(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr,
                                                                    UINT status_code, ULONG version, const CHAR *description,
                                                                    ULONG wait_option)
{
NX_AZURE_IOT_PNP_CLIENT *pnp_client_ptr = adu_agent_ptr -> nx_azure_iot_pnp_client_ptr;
NX_AZURE_IOT_JSON_WRITER json_writer;
UINT status;

    /* Create json writer for service reported property.  */
    status = nx_azure_iot_pnp_client_reported_properties_create(pnp_client_ptr, &json_writer, wait_option);
    if (status)
    {
        return(status);
    }

    /* Fill the response of desired service property.  */
    if (nx_azure_iot_pnp_client_reported_property_component_begin(pnp_client_ptr, &json_writer,
                                                                  (const UCHAR *)NX_AZURE_IOT_ADU_AGENT_COMPONENT_NAME,
                                                                  sizeof(NX_AZURE_IOT_ADU_AGENT_COMPONENT_NAME) - 1) ||
        nx_azure_iot_pnp_client_reported_property_status_begin(pnp_client_ptr, &json_writer,
                                                               (UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_SERVICE,
                                                               sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_SERVICE) - 1,
                                                               status_code, version,
                                                               (const UCHAR *)description, strlen(description)))
    {
        nx_azure_iot_json_writer_deinit(&json_writer);
        return (NX_NOT_SUCCESSFUL);
    }

    /* Append begin object to start to fill user value.  */
    if (nx_azure_iot_json_writer_append_begin_object(&json_writer))
    {
        nx_azure_iot_json_writer_deinit(&json_writer);
        return (NX_NOT_SUCCESSFUL);
    }

    /* Fill action.  */
    if (nx_azure_iot_json_writer_append_property_with_int32_value(&json_writer,
                                                                  (const UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_ACTION,
                                                                  sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_ACTION) - 1,
                                                                  (INT)adu_agent_ptr -> nx_azure_iot_adu_agent_action))
    {
        nx_azure_iot_json_writer_deinit(&json_writer);
        return (NX_NOT_SUCCESSFUL);
    }

    /* Fill updateManifest.  */
    if (nx_azure_iot_json_writer_append_property_with_string_value(&json_writer,
                                                                   (const UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_UPDATE_MANIFEST,
                                                                   sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_UPDATE_MANIFEST) - 1,
                                                                   adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest,
                                                                   adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_size))
    {
        nx_azure_iot_json_writer_deinit(&json_writer);
        return (NX_NOT_SUCCESSFUL);
    }

    /* Fill updateManifestSignature.  */
    if (nx_azure_iot_json_writer_append_property_with_string_value(&json_writer,
                                                                    (const UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_UPDATE_MANIFEST_SIGNATURE,
                                                                    sizeof(NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_UPDATE_MANIFEST_SIGNATURE) - 1,
                                                                    adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_signature,
                                                                    adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_signature_size))
    {
        nx_azure_iot_json_writer_deinit(&json_writer);
        return (NX_NOT_SUCCESSFUL);
    }

    /* Fill File URLs.  */
    if ((adu_agent_ptr -> nx_azure_iot_adu_agent_file_urls.file_url) &&
        (adu_agent_ptr -> nx_azure_iot_adu_agent_file_urls.file_number))
    {

        /* Fill the fileUrls property values as object.  */
        if (nx_azure_iot_json_writer_append_property_name(&json_writer,
                                                            (const UCHAR *)NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_FILEURLS,
                                                            sizeof (NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_FILEURLS) - 1) ||
            nx_azure_iot_json_writer_append_begin_object(&json_writer) ||
            nx_azure_iot_json_writer_append_property_with_string_value(&json_writer,
                                                                        adu_agent_ptr -> nx_azure_iot_adu_agent_file_urls.file_number,
                                                                        adu_agent_ptr -> nx_azure_iot_adu_agent_file_urls.file_number_length,
                                                                        adu_agent_ptr -> nx_azure_iot_adu_agent_file_urls.file_url,
                                                                        adu_agent_ptr -> nx_azure_iot_adu_agent_file_urls.file_url_length) ||
            nx_azure_iot_json_writer_append_end_object(&json_writer))
        {
            nx_azure_iot_json_writer_deinit(&json_writer);
            return (NX_NOT_SUCCESSFUL);
        }
    }
    
    /* Append end object.  */
    if (nx_azure_iot_json_writer_append_end_object(&json_writer))
    {
        nx_azure_iot_json_writer_deinit(&json_writer);
        return (NX_NOT_SUCCESSFUL);
    }

    /* End status and component.  */
    if (nx_azure_iot_pnp_client_reported_property_status_end(pnp_client_ptr, &json_writer) ||
        nx_azure_iot_pnp_client_reported_property_component_end(pnp_client_ptr, &json_writer))
    {
        nx_azure_iot_json_writer_deinit(&json_writer);
        return (NX_NOT_SUCCESSFUL);
    }

    /* Send service reported property.  */
    status = nx_azure_iot_pnp_client_reported_properties_send(pnp_client_ptr,
                                                              &json_writer, NX_NULL,
                                                              NX_NULL, NX_NULL,
                                                              wait_option);
    if(status)
    {
        nx_azure_iot_json_writer_deinit(&json_writer);
        return(status);
    }

    return(NX_AZURE_IOT_SUCCESS);
}

extern const NX_AZURE_IOT_ADU_AGENT_RSA_ROOT_KEY _nx_azure_iot_adu_agent_rsa_root_key_list[];
extern const UINT _nx_azure_iot_adu_agent_rsa_root_key_list_size;
static const NX_AZURE_IOT_ADU_AGENT_RSA_ROOT_KEY *nx_azure_iot_adu_agent_rsa_root_key_find(const UCHAR* kid, UINT kid_size)
{

    /* Loop to find the root key.  */
    for (UINT i = 0; i < _nx_azure_iot_adu_agent_rsa_root_key_list_size; i++)
    {

        /* Check the kid.  */
        if ((kid_size == _nx_azure_iot_adu_agent_rsa_root_key_list[i].kid_size) &&
            (memcmp(kid, _nx_azure_iot_adu_agent_rsa_root_key_list[i].kid, kid_size) == 0))
        {

            /* Find the root key.  */
            return(&_nx_azure_iot_adu_agent_rsa_root_key_list[i]);
        }
    }

    return(NX_NULL);
}

/* SHA256. */
static UINT nx_azure_iot_adu_agent_sha256_calculate(NX_CRYPTO_METHOD *sha256_method,
                                                    UCHAR *metadata_ptr, UINT metadata_size,
                                                    UCHAR *input_ptr, ULONG input_size,
                                                    UCHAR *output_ptr, ULONG output_size)
{
UINT status;

    /* Check for invalid pointer.  */
    if ((sha256_method == NX_NULL) || (sha256_method -> nx_crypto_operation == NX_NULL))
    {
        return(NX_AZURE_IOT_INVALID_PARAMETER);
    }

    /* Initialize.  */
    if (sha256_method -> nx_crypto_init)
    {
        status = sha256_method -> nx_crypto_init((NX_CRYPTO_METHOD*)sha256_method,
                                                 NX_NULL,
                                                 0,
                                                 NX_NULL,
                                                 metadata_ptr,
                                                 metadata_size);

        /* Check status.  */
        if (status)
        {
            return(status);
        }
    } 

    status = sha256_method -> nx_crypto_operation(NX_CRYPTO_HASH_INITIALIZE,
                                                  NX_NULL,
                                                  (NX_CRYPTO_METHOD*)sha256_method,
                                                  NX_NULL,
                                                  0,
                                                  NX_NULL,
                                                  0,
                                                  NX_NULL,
                                                  NX_NULL,
                                                  0,
                                                  metadata_ptr,
                                                  metadata_size,
                                                  NX_NULL,
                                                  NX_NULL);

    /* Check status.  */
    if (status)
    {
        return(status);
    }

    /* Update hash value for data.  */
    status = sha256_method -> nx_crypto_operation(NX_CRYPTO_HASH_UPDATE,
                                                  NX_NULL,
                                                  (NX_CRYPTO_METHOD*)sha256_method,
                                                  NX_NULL,
                                                  0,
                                                  input_ptr,
                                                  input_size,
                                                  NX_NULL,
                                                  NX_NULL,
                                                  0,
                                                  metadata_ptr,
                                                  metadata_size,
                                                  NX_NULL,
                                                  NX_NULL);
    
    /* Check status.  */
    if (status)
    {
        return(status);
    }

    /* Calculate the hash value.  */
    status = sha256_method -> nx_crypto_operation(NX_CRYPTO_HASH_CALCULATE,
                                                  NX_NULL,
                                                  (NX_CRYPTO_METHOD*)sha256_method,
                                                  NX_NULL,
                                                  0,
                                                  NX_NULL,
                                                  0,
                                                  NX_NULL,
                                                  output_ptr,
                                                  output_size,
                                                  metadata_ptr,
                                                  metadata_size,
                                                  NX_NULL,
                                                  NX_NULL);
    
    /* Check status.  */
    if (status)
    {
        return(status);
    }

    /* Clearnup.  */
    if (sha256_method -> nx_crypto_cleanup)
    {
        sha256_method -> nx_crypto_cleanup(metadata_ptr);
    }

    return(NX_AZURE_IOT_SUCCESS);
}


/* RS256.  */
static UINT nx_azure_iot_adu_agent_rs256_verify(NX_AZURE_IOT_ADU_AGENT_CRYPTO *adu_agent_crypto,
                                                UCHAR *input_ptr, ULONG input_size,
                                                UCHAR *signature_ptr, ULONG signature_size,
                                                UCHAR *n, ULONG n_size,
                                                UCHAR *e, ULONG e_size,
                                                UCHAR *buffer_ptr, UINT buffer_size)
{

UINT   status;
UCHAR *oid;
UINT   oid_length;
UCHAR *decrypted_hash;
UINT   decrypted_hash_length;
UCHAR *rsa_buffer = buffer_ptr;
UCHAR *sha_buffer = buffer_ptr + NX_AZURE_IOT_ADU_AGENT_RSA3072_SIZE;

    /* Check for invalid pointer.  */
    if ((adu_agent_crypto -> method_rsa == NX_NULL) ||
        (adu_agent_crypto -> method_rsa -> nx_crypto_init == NX_NULL) ||
        (adu_agent_crypto -> method_rsa -> nx_crypto_operation == NX_NULL) ||
        (adu_agent_crypto -> method_sha256 == NX_NULL) || 
        (adu_agent_crypto -> method_sha256 -> nx_crypto_operation == NX_NULL))
    {
        return(NX_FALSE);
    }

    /* Check buffer size.  */
    if (buffer_size < (NX_AZURE_IOT_ADU_AGENT_RSA3072_SIZE + NX_AZURE_IOT_ADU_AGENT_SHA256_HASH_SIZE))
    {
        return(NX_FALSE);
    }

    /* Decrypt the signature by RSA.  */

    /* Initialize.  */
    status = adu_agent_crypto -> method_rsa -> nx_crypto_init((NX_CRYPTO_METHOD*)adu_agent_crypto -> method_rsa,
                                                              n,
                                                              n_size << 3,
                                                              NX_NULL,
                                                              adu_agent_crypto -> method_rsa_metadata,
                                                              adu_agent_crypto -> method_rsa_metadata_size);

    /* Check status.  */
    if (status)
    {
        return(NX_FALSE);
    }

    /* Decrypt the signature.  */
    status = adu_agent_crypto -> method_rsa -> nx_crypto_operation(NX_CRYPTO_DECRYPT,
                                                                   NX_NULL,
                                                                   (NX_CRYPTO_METHOD*)adu_agent_crypto -> method_rsa,
                                                                   e,
                                                                   e_size << 3, 
                                                                   signature_ptr,
                                                                   signature_size,
                                                                   NX_NULL,
                                                                   rsa_buffer,
                                                                   NX_AZURE_IOT_ADU_AGENT_RSA3072_SIZE,
                                                                   adu_agent_crypto -> method_rsa_metadata,
                                                                   adu_agent_crypto -> method_rsa_metadata_size,
                                                                   NX_NULL,
                                                                   NX_NULL);
    
    /* Check status.  */
    if (status)
    {
        return(NX_FALSE);
    }

    /* Cleanup.  */
    if (adu_agent_crypto -> method_rsa -> nx_crypto_cleanup)
    {
        adu_agent_crypto -> method_rsa -> nx_crypto_cleanup(adu_agent_crypto -> method_rsa_metadata);
    }

    /* Decode the decrypted signature, which should be in PKCS#7 format. */
    status = _nx_secure_x509_pkcs7_decode(rsa_buffer, signature_size,
                                          (const UCHAR **)&oid, &oid_length,
                                          (const UCHAR **)&decrypted_hash, &decrypted_hash_length);

    /* Check status.  */
    if (status)
    {
        return(NX_FALSE);
    }

    /* Calculate input by SHA256.  */    
    status = nx_azure_iot_adu_agent_sha256_calculate(adu_agent_crypto -> method_sha256,
                                                     adu_agent_crypto -> method_sha256_metadata,
                                                     NX_AZURE_IOT_ADU_AGENT_SHA256_METADATA_SIZE,
                                                     input_ptr, input_size,
                                                     sha_buffer, NX_AZURE_IOT_ADU_AGENT_SHA256_HASH_SIZE);

    /* Check status.  */
    if (status)
    {
        return(NX_FALSE);
    }

    /* Verify.  */
    if ((decrypted_hash_length != NX_AZURE_IOT_ADU_AGENT_SHA256_HASH_SIZE) || 
        (memcmp(decrypted_hash, sha_buffer, NX_AZURE_IOT_ADU_AGENT_SHA256_HASH_SIZE)))
    {
        return(NX_FALSE);
    }

    return(NX_TRUE);
}

static UINT nx_azure_iot_adu_agent_file_url_parse(UCHAR *file_url, ULONG file_url_length, 
                                                  UCHAR *buffer_ptr, UINT buffer_size,
                                                  NX_AZURE_IOT_ADU_AGENT_DOWNLOADER *downloader_ptr)
{
UINT    i;
UINT    dot_count = 0;
UINT    temp = 0;
ULONG   ip_address = 0;
UCHAR   address_found = NX_FALSE;
UCHAR   port_found = NX_FALSE;


    /* Initialize.  */
    downloader_ptr -> host = NX_NULL;
    downloader_ptr -> resource = NX_NULL;

    /* The url must be "http://host/resource".  */
    if (memcmp(file_url, NX_AZURE_IOT_ADU_AGENT_HTTP_PROTOCOL, sizeof(NX_AZURE_IOT_ADU_AGENT_HTTP_PROTOCOL) - 1))
    {
        return(NX_AZURE_IOT_FAILURE);
    }

    /* Set the host ptr.  */
    file_url += (sizeof(NX_AZURE_IOT_ADU_AGENT_HTTP_PROTOCOL) - 1);
    file_url_length -= (sizeof(NX_AZURE_IOT_ADU_AGENT_HTTP_PROTOCOL) - 1);

    /* Try to detect whether the host is numerical IP address. */
    for (i = 0; i < file_url_length; i++)
    {
        if (file_url[i] >= '0' && file_url[i] <= '9')
        {
            temp = (UINT)(temp * 10 + (UINT)(file_url[i] - '0'));
            if ((temp > 0xFF && port_found == NX_FALSE) ||
                (temp > 0xFFFF && port_found == NX_TRUE))
            {
                break;
            }
        }
        else if (file_url[i] == '.')
        {
            if (dot_count++ == 3)
            {
                break;
            }
            ip_address = (ip_address << 8) + temp;
            temp = 0;
        }
        else if (file_url[i] == ':')
        {
            if ((dot_count != 3) || (port_found == NX_TRUE))
            {
                break;
            }
            ip_address = (ip_address << 8) + temp;

            /* Set the address.  */
            downloader_ptr -> address.nxd_ip_version = NX_IP_VERSION_V4;
            downloader_ptr -> address.nxd_ip_address.v4 = ip_address;
            address_found = NX_TRUE;
                
            /* Try to reslove the port.  */
            temp = 0;
            port_found = NX_TRUE;
        }
        else if (file_url[i] == '/')
        {   
            if (dot_count == 3)
            {
                if (port_found)
                {
                    downloader_ptr -> port = temp;
                }
                else
                {
                    ip_address = (ip_address << 8) + temp;
                            
                    /* Set the address.  */
                    downloader_ptr -> address.nxd_ip_version = NX_IP_VERSION_V4;
                    downloader_ptr -> address.nxd_ip_address.v4 = ip_address;
                    address_found = NX_TRUE;                
                }                
            }    
            break;
        }
        else
        {
            break;
        }
    }
 
    /* Check if there is enough buffer.  */
    if (file_url_length >= buffer_size)
    {
        return(NX_AZURE_IOT_FAILURE);
    }

    /* Split host and resource url . */
    for (; i < file_url_length; i++)
    {
        if (file_url[i] == '/')
        {

            /* Store the host ans resource.  */
            downloader_ptr -> host = buffer_ptr;
            memcpy(downloader_ptr -> host, file_url, i); /* Use case of memcpy is verified. */
            *(buffer_ptr + i) = NX_NULL;

            /* Set the resource url.  */
            downloader_ptr -> resource = (buffer_ptr + i + 1);
            memcpy(downloader_ptr -> resource, &file_url[i + 1], (file_url_length - i - 1)); /* Use case of memcpy is verified. */
            *(buffer_ptr + file_url_length) = NX_NULL;

            /* Update buffer size.  */
            buffer_size -= (file_url_length + 1);
            break;
        }
    }
    
    /* Check the host and resource.   */
    if ((downloader_ptr -> host == NX_NULL) || (downloader_ptr -> resource == NX_NULL))
        return(NX_AZURE_IOT_FAILURE);

    /* Update the state.  */
    if (address_found == NX_FALSE)
    {
        downloader_ptr -> state = NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_URL_PARSED;
    }
    else
    {
        downloader_ptr -> state = NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_ADDRESS_DONE;
    }

    /* Check if found the port.  */
    if (port_found == NX_FALSE)
    {

        /* Set tht http port as default.  */
        downloader_ptr -> port = NX_WEB_HTTP_SERVER_PORT;
    }
    
    return(NX_AZURE_IOT_SUCCESS);
}

static void nx_azure_iot_adu_agent_dns_query(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr)
{
UINT status;
NX_AZURE_IOT_ADU_AGENT_DOWNLOADER *downloader_ptr = &(adu_agent_ptr -> nx_azure_iot_adu_agent_downloader);


    /* Check the state.  */
    if ((downloader_ptr -> state != NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_URL_PARSED) &&
        (downloader_ptr -> state != NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_ADDRESS_QUERY))
    {
        return;
    }

    /* Check if reach the max retry count.  */
    if (downloader_ptr -> dns_query_count <= NX_AZURE_IOT_ADU_AGENT_DNS_RETRANSMIT_COUNT)
    {

        /* Set the timeout.  */
        downloader_ptr -> timeout = (ULONG)(NX_AZURE_IOT_ADU_AGENT_DNS_INITIAL_TIMEOUT << downloader_ptr -> dns_query_count);
    
        /* Update the query count.  */
        downloader_ptr -> dns_query_count++;

        /* Update state.  */
        downloader_ptr -> state = NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_ADDRESS_QUERY;

        /* Resolve the host name by DNS.  */
        status = nxd_dns_host_by_name_get(downloader_ptr -> dns_ptr, 
                                          downloader_ptr -> host,
                                          &(downloader_ptr -> address),
                                          NX_NO_WAIT, NX_IP_VERSION_V4);

        /* Check status.  */
        if (status == NX_SUCCESS)
        {

            /* Got the address, update the state.  */
            downloader_ptr -> state = NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_ADDRESS_DONE;

            /* Start HTTP connect.  */
            nx_azure_iot_adu_agent_http_connect(adu_agent_ptr);
            return;
        }
        else if (status == NX_IN_PROGRESS)
        {

            /* Query in progress.  */
            return;
        }
    }

    /* Send dns query failed or already reach the max retransmission count.  */
    nx_azure_iot_adu_agent_download_state_update(adu_agent_ptr, NX_FALSE);
}

static void nx_azure_iot_adu_agent_dns_response_notify(NX_UDP_SOCKET *socket_ptr)
{
NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr;


    /* Set adu agent pointer.  */
    adu_agent_ptr = (NX_AZURE_IOT_ADU_AGENT *)socket_ptr -> nx_udp_socket_reserved_ptr;

    /* Set the DNS response receive event.  */
    nx_cloud_module_event_set(&(adu_agent_ptr -> nx_azure_iot_adu_agent_cloud_module),
                              NX_AZURE_IOT_ADU_AGENT_DNS_RESPONSE_RECEIVE_EVENT);
}

static void nx_azure_iot_adu_agent_dns_response_get(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr)
{
UINT status;
UINT record_count;
NX_AZURE_IOT_ADU_AGENT_DOWNLOADER *downloader_ptr = &(adu_agent_ptr -> nx_azure_iot_adu_agent_downloader);


    /* Try to get the response.  */
    status = _nx_dns_response_get(downloader_ptr -> dns_ptr, downloader_ptr -> host,
                                  (UCHAR *)&downloader_ptr -> address.nxd_ip_address.v4, sizeof(ULONG),
                                  &record_count, NX_NO_WAIT);

    /* Check status.  */
    if (status)
    {

        /* Retry DNS query.  */
        nx_azure_iot_adu_agent_dns_query(adu_agent_ptr);
    }
    else
    {

        /* Set the address version.  */
        downloader_ptr -> address.nxd_ip_version = NX_IP_VERSION_V4;

        /* Update the state.  */
        adu_agent_ptr -> nx_azure_iot_adu_agent_downloader.state = NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_ADDRESS_DONE;

        /* Start HTTP connect.  */
        nx_azure_iot_adu_agent_http_connect(adu_agent_ptr);
    }
}

static void nx_azure_iot_adu_agent_http_connect(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr)
{

UINT status;
NX_IP *ip_ptr;
NX_AZURE_IOT_ADU_AGENT_DOWNLOADER *downloader_ptr = &(adu_agent_ptr -> nx_azure_iot_adu_agent_downloader);

    /* Initialize.  */
    ip_ptr = adu_agent_ptr -> nx_azure_iot_pnp_client_ptr -> nx_azure_iot_pnp_client_transport.nx_azure_iot_ptr -> nx_azure_iot_ip_ptr;
    downloader_ptr = &(adu_agent_ptr -> nx_azure_iot_adu_agent_downloader);

    /* Create an HTTP client instance.  */
    status = nx_web_http_client_create(&(downloader_ptr -> http_client),
                                       "HTTP Client",
                                       ip_ptr, ip_ptr -> nx_ip_default_packet_pool,
                                       NX_AZURE_IOT_ADU_AGENT_HTTP_WINDOW_SIZE);

    /* Check status.  */
    if (status)
    {
        nx_azure_iot_adu_agent_download_state_update(adu_agent_ptr, NX_FALSE);
        return;
    }

    /* Update the state and timeout.  */
    downloader_ptr -> state = NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_HTTP_CONNECT;
    downloader_ptr -> timeout = NX_AZURE_IOT_ADU_AGENT_HTTP_CONNECT_TIMEOUT;

    /* Set the notify.  */
    downloader_ptr -> http_client.nx_web_http_client_socket.nx_tcp_socket_reserved_ptr = adu_agent_ptr;
    nx_tcp_socket_establish_notify(&(downloader_ptr -> http_client.nx_web_http_client_socket),
                                   nx_azure_iot_adu_agent_http_establish_notify);
    nx_tcp_socket_receive_notify(&(downloader_ptr -> http_client.nx_web_http_client_socket), 
                                 nx_azure_iot_adu_agent_http_receive_notify);

    /* Connect to Server.  */
    status = nx_web_http_client_connect(&(downloader_ptr -> http_client),
                                        &(downloader_ptr -> address),
                                        downloader_ptr -> port,
                                        NX_NO_WAIT);
    
    /* Check status.  */
    if (status == NX_SUCCESS)
    {

        /* Connection established. Start to get file content.  */
        nx_cloud_module_event_set(&(adu_agent_ptr -> nx_azure_iot_adu_agent_cloud_module),
                                  NX_AZURE_IOT_ADU_AGENT_HTTP_CONNECT_DONE_EVENT);
        return;
    }
    else if (status == NX_IN_PROGRESS)
    {

        /* Query in progress.  */
        return;
    }

    /* Failed.  */
    nx_azure_iot_adu_agent_download_state_update(adu_agent_ptr, NX_FALSE);
    return;
}

static void nx_azure_iot_adu_agent_http_request_send(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr)
{
UINT status;
NX_AZURE_IOT_ADU_AGENT_DOWNLOADER *downloader_ptr = &(adu_agent_ptr -> nx_azure_iot_adu_agent_downloader);


    /* Update the state and timeout.  */
    downloader_ptr -> state = NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_HTTP_CONTENT_GET;
    downloader_ptr -> timeout = NX_AZURE_IOT_ADU_AGENT_HTTP_DOWNLOAD_TIMEOUT;

    /* Use the service to send a GET request to the server . */
    status = nx_web_http_client_request_initialize(&(downloader_ptr -> http_client),
                                                   NX_WEB_HTTP_METHOD_GET, 
                                                   (CHAR *)downloader_ptr -> resource,
                                                   (CHAR *)downloader_ptr -> host,
                                                   0, NX_FALSE, NX_NULL, NX_NULL,
                                                   NX_NO_WAIT);

    /* Check status.  */
    if (status)
    {
        nx_azure_iot_adu_agent_download_state_update(adu_agent_ptr, NX_FALSE);
        return;
    }

    /* Add HTTP request header.  */
    status = nx_web_http_client_request_header_add(&(downloader_ptr -> http_client), NX_AZURE_IOT_ADU_AGENT_STRING("Accept"), NX_AZURE_IOT_ADU_AGENT_STRING("*/*"), NX_NO_WAIT);
    status += nx_web_http_client_request_header_add(&(downloader_ptr -> http_client), NX_AZURE_IOT_ADU_AGENT_STRING("Accept-Encoding"), NX_AZURE_IOT_ADU_AGENT_STRING("peerdist*"), NX_NO_WAIT);
    status += nx_web_http_client_request_header_add(&(downloader_ptr -> http_client), NX_AZURE_IOT_ADU_AGENT_STRING("Range"), NX_AZURE_IOT_ADU_AGENT_STRING("bytes=0-"), NX_NO_WAIT);
    status += nx_web_http_client_request_header_add(&(downloader_ptr -> http_client), NX_AZURE_IOT_ADU_AGENT_STRING("User-Agent"), NX_AZURE_IOT_ADU_AGENT_STRING("Microsoft-Delivery-Optimization/10.0"), NX_NO_WAIT);
    status += nx_web_http_client_request_header_add(&(downloader_ptr -> http_client), NX_AZURE_IOT_ADU_AGENT_STRING("MS-CV"), NX_AZURE_IOT_ADU_AGENT_STRING("+ucpLSBeUESzB8XY.1.1.2.64.1.6.2.7.3.1.2"), NX_NO_WAIT);
    status += nx_web_http_client_request_header_add(&(downloader_ptr -> http_client), NX_AZURE_IOT_ADU_AGENT_STRING("X-P2P-PeerDist"), NX_AZURE_IOT_ADU_AGENT_STRING("Version=1.1"), NX_NO_WAIT);
    status += nx_web_http_client_request_header_add(&(downloader_ptr -> http_client), NX_AZURE_IOT_ADU_AGENT_STRING("X-P2P-PeerDistEx"), NX_AZURE_IOT_ADU_AGENT_STRING("MinContentInformation=1.0, MaxContentInformation=2.0"), NX_NO_WAIT);

    /* Check status.  */
    if (status)
    {
        nx_azure_iot_adu_agent_download_state_update(adu_agent_ptr, NX_FALSE);
        return;
    }

    /* Send the HTTP request we just built. */
    status = nx_web_http_client_request_send(&(downloader_ptr -> http_client), NX_NO_WAIT);

    /* Check status.  */
    if (status)
    {
        nx_azure_iot_adu_agent_download_state_update(adu_agent_ptr, NX_FALSE);
        return;
    }
}

static void nx_azure_iot_adu_agent_http_response_receive(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr)
{
    
UINT        status;
UINT        get_status;
UINT        while_loop_counter_for_diagnostics = 0;
NX_PACKET  *received_packet;
NX_PACKET  *data_packet;
NX_CRYPTO_METHOD *sha256_method = adu_agent_ptr -> nx_azure_iot_adu_agent_crypto.method_sha256;
UCHAR      *sha256_method_metadata = adu_agent_ptr -> nx_azure_iot_adu_agent_crypto.method_sha256_metadata;;
ULONG       sha256_method_metadata_size = NX_AZURE_IOT_ADU_AGENT_SHA256_METADATA_SIZE;
VOID       *handler = adu_agent_ptr -> nx_azure_iot_adu_agent_crypto.handler;
UCHAR      *generated_hash;
UCHAR      *decoded_hash;
UINT        bytes_copied;
NX_AZURE_IOT_ADU_AGENT_DRIVER driver_request;
NX_AZURE_IOT_ADU_AGENT_DOWNLOADER *downloader_ptr = &(adu_agent_ptr -> nx_azure_iot_adu_agent_downloader);

    /* Check the state.  */
    if (adu_agent_ptr -> nx_azure_iot_adu_agent_last_reported_state != NX_AZURE_IOT_ADU_AGENT_STATE_DOWNLOAD_STARTED)
    {
        return;
    }

    /* Receive response data from the server. Loop until all data is received. */
    get_status = NX_SUCCESS;
    while ((get_status != NX_WEB_HTTP_GET_DONE) && (downloader_ptr -> received_firmware_size < adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_content.file_size_in_bytes))
    {

        /* Every 10 times through master while() loop, print something to show activity.  */
        if (while_loop_counter_for_diagnostics % 10 == 0)
        {
            LogInfo(LogLiteralArgs("Getting download data..."));
        }

        while_loop_counter_for_diagnostics++;
        get_status = nx_web_http_client_response_body_get(&(downloader_ptr -> http_client), &received_packet, NX_NO_WAIT);

        /* Check for error.  */
        if ((get_status == NX_SUCCESS) || (get_status == NX_WEB_HTTP_GET_DONE) || (get_status == NX_WEB_HTTP_STATUS_CODE_PARTIAL_CONTENT))
        {

            /* Loop to write the data from packet into flash.  */
            data_packet = received_packet;
#ifndef NX_DISABLE_PACKET_CHAIN
            while(data_packet)
            {
#endif /* NX_DISABLE_PACKET_CHAIN  */

                /* Update the hash value for data.  */
                status = sha256_method -> nx_crypto_operation(NX_CRYPTO_HASH_UPDATE,
                                                              NX_NULL,
                                                              (NX_CRYPTO_METHOD*)sha256_method,
                                                              NX_NULL,
                                                              0,
                                                              data_packet -> nx_packet_prepend_ptr,
                                                              (ULONG)(data_packet -> nx_packet_append_ptr - data_packet -> nx_packet_prepend_ptr),
                                                              NX_NULL,
                                                              NX_NULL,
                                                              0,
                                                              sha256_method_metadata,
                                                              sha256_method_metadata_size,
                                                              NX_NULL,
                                                              NX_NULL);
    
                /* Check status.  */
                if (status)
                {

                    /* Release the packet.  */
                    nx_packet_release(received_packet);
                    nx_azure_iot_adu_agent_download_state_update(adu_agent_ptr, NX_FALSE);
                    return;
                }

                /* Send the firmware write request to the driver.   */
                driver_request.nx_azure_iot_adu_agent_driver_command = NX_AZURE_IOT_ADU_AGENT_DRIVER_WRITE;
                driver_request.nx_azure_iot_adu_agent_driver_firmware_data_offset = downloader_ptr -> received_firmware_size;
                driver_request.nx_azure_iot_adu_agent_driver_firmware_data_ptr = data_packet -> nx_packet_prepend_ptr;
                driver_request.nx_azure_iot_adu_agent_driver_firmware_data_size = (UINT)(data_packet -> nx_packet_append_ptr - data_packet -> nx_packet_prepend_ptr);
                driver_request.nx_azure_iot_adu_agent_driver_status = NX_AZURE_IOT_SUCCESS;
                (adu_agent_ptr -> nx_azure_iot_adu_agent_driver_entry)(&driver_request);
                
                /* Check status.  */
                if (driver_request.nx_azure_iot_adu_agent_driver_status)
                {

                    /* Release the packet.  */
                    nx_packet_release(received_packet);
                    nx_azure_iot_adu_agent_download_state_update(adu_agent_ptr, NX_FALSE);
                    return;
                }

                /* Update received firmware size.  */
                downloader_ptr -> received_firmware_size += (UINT)(data_packet -> nx_packet_append_ptr - data_packet -> nx_packet_prepend_ptr);
                
#ifndef NX_DISABLE_PACKET_CHAIN
                data_packet = data_packet -> nx_packet_next;
            }
#endif /* NX_DISABLE_PACKET_CHAIN  */

            /* Release the packet.  */
            nx_packet_release(received_packet);
        }
        else
        {
            return;
        }
    }

    /* Firmware downloaded. Verify the hash.  */

    /* Set hash buffer.  */
    if ((NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_SIZE - (downloader_ptr -> host_length + 1 + downloader_ptr -> resource_length + 1)) < 
        (NX_AZURE_IOT_ADU_AGENT_SHA256_HASH_SIZE << 1))
    {
        LogError(LogLiteralArgs("Firmware download fail: INSUFFICIENT BUFFER FOR SHA256"));
        nx_azure_iot_adu_agent_download_state_update(adu_agent_ptr, NX_FALSE);
        return;
    }
    generated_hash = adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest + 
                    downloader_ptr -> host_length + 1 + downloader_ptr -> resource_length + 1;
    decoded_hash = generated_hash + NX_AZURE_IOT_ADU_AGENT_SHA256_HASH_SIZE;

    /* Calculate the hash value.  */
    status = sha256_method -> nx_crypto_operation(NX_CRYPTO_HASH_CALCULATE,
                                                  handler,
                                                  (NX_CRYPTO_METHOD*)sha256_method,
                                                  NX_NULL,
                                                  0,
                                                  NX_NULL,
                                                  0,
                                                  NX_NULL,
                                                  generated_hash,
                                                  NX_AZURE_IOT_ADU_AGENT_SHA256_HASH_SIZE,
                                                  sha256_method_metadata,
                                                  sha256_method_metadata_size,
                                                  NX_NULL,
                                                  NX_NULL);

    /* Check status.  */
    if (status)
    {
        LogError(LogLiteralArgs("Firmware download fail: HASH ERROR"));
        nx_azure_iot_adu_agent_download_state_update(adu_agent_ptr, NX_FALSE);
        return;
    }

    /* Cleanup.  */
    if (sha256_method -> nx_crypto_cleanup)
    {
        sha256_method -> nx_crypto_cleanup(sha256_method_metadata);
    }

    /* Decode the file hash (base64).  */
    if (nx_azure_iot_base64_decode((CHAR *)adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_content.file_sha256,
                                   adu_agent_ptr -> nx_azure_iot_adu_agent_update_manifest_content.file_sha256_length,
                                   decoded_hash, NX_AZURE_IOT_ADU_AGENT_SHA256_HASH_SIZE, &bytes_copied))
    {
        LogError(LogLiteralArgs("Firmware download fail: HASH ERROR"));
        nx_azure_iot_adu_agent_download_state_update(adu_agent_ptr, NX_FALSE);
        return;
    }
    
    /* Verify the hash value.  */
    if (memcmp(generated_hash, decoded_hash, NX_AZURE_IOT_ADU_AGENT_SHA256_HASH_SIZE))
    {
        LogError(LogLiteralArgs("Firmware download fail: HASH ERROR"));
        nx_azure_iot_adu_agent_download_state_update(adu_agent_ptr, NX_FALSE);
        return;
    }

    /* Output info.  */
    LogInfo(LogLiteralArgs("Firmware downloaded"));

    /* Update download state.  */
    nx_azure_iot_adu_agent_download_state_update(adu_agent_ptr, NX_TRUE);
}

static void nx_azure_iot_adu_agent_http_establish_notify(NX_TCP_SOCKET *socket_ptr)
{
NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr;


    /* Set adu agent pointer.  */
    adu_agent_ptr = (NX_AZURE_IOT_ADU_AGENT *)socket_ptr -> nx_tcp_socket_reserved_ptr;

    /* Set the DNS response receive event.  */
    nx_cloud_module_event_set(&(adu_agent_ptr -> nx_azure_iot_adu_agent_cloud_module),
                              NX_AZURE_IOT_ADU_AGENT_HTTP_CONNECT_DONE_EVENT);
}

static void nx_azure_iot_adu_agent_http_receive_notify(NX_TCP_SOCKET *socket_ptr)
{
NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr;


    /* Set adu agent pointer.  */
    adu_agent_ptr = (NX_AZURE_IOT_ADU_AGENT *)socket_ptr -> nx_tcp_socket_reserved_ptr;

    /* Set the DNS response receive event.  */
    nx_cloud_module_event_set(&(adu_agent_ptr -> nx_azure_iot_adu_agent_cloud_module),
                              NX_AZURE_IOT_ADU_AGENT_HTTP_RECEIVE_EVENT);
}

static void nx_azure_iot_adu_agent_download_state_update(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr, UINT success)
{

NX_AZURE_IOT_ADU_AGENT_RESULT result;

    /* Check the status.  */
    if (success == NX_TRUE)
    {

        /* Download complete, update state.  */
        nx_azure_iot_adu_agent_state_update(adu_agent_ptr, NX_AZURE_IOT_ADU_AGENT_STATE_DOWNLOAD_SUCCEEDED, NX_NULL);
    }
    else
    {
        result.result_code = NX_AZURE_IOT_ADU_AGENT_RESULT_CODE_ERROR;
        result.extended_result_code = 0;
        nx_azure_iot_adu_agent_state_update(adu_agent_ptr, NX_AZURE_IOT_ADU_AGENT_STATE_FAILED, &result);
    }

    /* Cleanup.  */
    if (adu_agent_ptr -> nx_azure_iot_adu_agent_downloader.state >= NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_HTTP_CONNECT)
    {

        /* Delete http client.  */
        nx_web_http_client_delete(&(adu_agent_ptr -> nx_azure_iot_adu_agent_downloader.http_client));
    }

    /* Reset the state.  */
    adu_agent_ptr -> nx_azure_iot_adu_agent_downloader.state = NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_IDLE;
}
