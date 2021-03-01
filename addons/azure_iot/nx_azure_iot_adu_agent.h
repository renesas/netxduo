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

/**
 * @file nx_azure_iot_adu_agent.h
 *
 * @brief Definition for the Azure IoT ADU agent interface.
 *
 */

#ifndef NX_AZURE_IOT_ADU_AGENT_H
#define NX_AZURE_IOT_ADU_AGENT_H

#ifdef __cplusplus
extern   "C" {
#endif

#include "nx_azure_iot_pnp_client.h"
#include "nx_web_http_client.h"

#ifdef NX_AZURE_IOT_PNP_CLIENT_SYSTEM_SERVICE_DISABLE
#error "NX_AZURE_IOT_PNP_CLIENT_SYSTEM_SERVICE_DISABLE must not be defined"
#endif /* NX_AZURE_IOT_PNP_CLIENT_SYSTEM_SERVICE_DISABLE */

#ifndef NX_ENABLE_EXTENDED_NOTIFY_SUPPORT
#error "NX_ENABLE_EXTENDED_NOTIFY_SUPPORT must be defined"
#endif /* NX_ENABLE_EXTENDED_NOTIFY_SUPPORT */

/* Define the ADU agent component name.  */
#define NX_AZURE_IOT_ADU_AGENT_COMPONENT_NAME                           "azureDeviceUpdateAgent"

/* Define the ADU agent property name "client" and sub property names.  */
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_CLIENT                     "client"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_RESULT_CODE                "resultCode"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_EXTENDED_RESULT_CODE       "extendedResultCode"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_STATE                      "state"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_INSTALLED_CONTENT_ID       "installedUpdateId"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_DEVICEPROPERTIES           "deviceProperties"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_MANUFACTURER               "manufacturer"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_MODEL                      "model"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_PROVIDER                   "provider"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_NAME                       "name"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_VERSION                    "version"

/* Define the ADU agent property name "service" and sub property names.  */
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_SERVICE                    "service"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_ACTION                     "action"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_UPDATE_MANIFEST            "updateManifest"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_UPDATE_MANIFEST_SIGNATURE  "updateManifestSignature"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_FILEURLS                   "fileUrls"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_MANIFEST_VERSION           "manifestVersion"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_UPDATE_ID                  "updateId"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_UPDATE_TYPE                "updateType"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_INSTALLED_CRITERIA         "installedCriteria"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_FILES                      "files"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_FILE_NAME                  "fileName"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_SIZE_IN_BYTES              "sizeInBytes"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_HASHES                     "hashes"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_SHA256                     "sha256"
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_NAME_CREATED_DATE_TIME          "createdDateTime"

/* Define ADU events. These events are processed by the cloud thread.  */
#define NX_AZURE_IOT_ADU_AGENT_PROPERTY_RECEIVE_EVENT                   ((ULONG)0x00000001)
#define NX_AZURE_IOT_ADU_AGENT_DNS_RESPONSE_RECEIVE_EVENT               ((ULONG)0x00000002)
#define NX_AZURE_IOT_ADU_AGENT_HTTP_CONNECT_DONE_EVENT                  ((ULONG)0x00000004)
#define NX_AZURE_IOT_ADU_AGENT_HTTP_RECEIVE_EVENT                       ((ULONG)0x00000008)

/* Define the update state values.  */
#define NX_AZURE_IOT_ADU_AGENT_STATE_IDLE                               0
#define NX_AZURE_IOT_ADU_AGENT_STATE_DOWNLOAD_STARTED                   1
#define NX_AZURE_IOT_ADU_AGENT_STATE_DOWNLOAD_SUCCEEDED                 2
#define NX_AZURE_IOT_ADU_AGENT_STATE_INSTALL_STARTED                    3
#define NX_AZURE_IOT_ADU_AGENT_STATE_INSTALL_SUCCEEDED                  4
#define NX_AZURE_IOT_ADU_AGENT_STATE_APPLY_STARTED                      5
#define NX_AZURE_IOT_ADU_AGENT_STATE_FAILED                             255

/* Define the update action values.  */
#define NX_AZURE_IOT_ADU_AGENT_ACTION_DOWNLOAD                          0
#define NX_AZURE_IOT_ADU_AGENT_ACTION_INSTALL                           1
#define NX_AZURE_IOT_ADU_AGENT_ACTION_APPLY                             2
#define NX_AZURE_IOT_ADU_AGENT_ACTION_CANCEL                            255

/* Define the result code value.  */
#define NX_AZURE_IOT_ADU_AGENT_RESULT_CODE_SUCCESS                      200
#define NX_AZURE_IOT_ADU_AGENT_RESULT_CODE_ERROR                        500

/* FIXME: status codes should be defined in PnP.  */
/* Status codes for PnP, closely mapping to HTTP status. */
#define NX_AZURE_IOT_PNP_STATUS_SUCCESS                                 200
#define NX_AZURE_IOT_PNP_STATUS_BAD_FORMAT                              400
#define NX_AZURE_IOT_PNP_STATUS_NOT_FOUND                               404
#define NX_AZURE_IOT_PNP_STATUS_INTERNAL_ERROR                          500

/* Define the crypto size.  */
#define NX_AZURE_IOT_ADU_AGENT_SHA256_HASH_SIZE                         32
#define NX_AZURE_IOT_ADU_AGENT_SHA256_HASH_BASE64_SIZE                  44
#define NX_AZURE_IOT_ADU_AGENT_RSA3072_SIZE                             384

/* Define the sha256 metadata buffer size used for verifying file hash. 
   The default value is software sha256 crypto metadata (sizeof(NX_CRYPTO_SHA256)).  */
#ifndef NX_AZURE_IOT_ADU_AGENT_SHA256_METADATA_SIZE
#define NX_AZURE_IOT_ADU_AGENT_SHA256_METADATA_SIZE                     360
#endif /* NX_AZURE_IOT_ADU_AGENT_SHA256_METADATA_SIZE */

/* Define the max update manifest size, the buffer is used to store the original string data.  */
#ifndef NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_SIZE
#define NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_SIZE                     1024
#endif /* NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_SIZE */

/* Define the max update manifest signature size (base64).  */
#ifndef NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_SIGNATURE_SIZE
#define NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_SIGNATURE_SIZE           3072
#endif /* NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_SIGNATURE_SIZE */

/* Define the max update manifest sjwk size (base64).  */
#ifndef NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_SJWK_SIZE
#define NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_SJWK_SIZE                2048
#endif /* NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_SJWK_SIZE */

/* Define the buffer for parsing file url, update manifest content in service property. FIXME: consider to use packet.  */
#ifndef NX_AZURE_IOT_ADU_AGENT_BUFFER_SIZE
#define NX_AZURE_IOT_ADU_AGENT_BUFFER_SIZE                              2048
#endif /* NX_AZURE_IOT_ADU_AGENT_BUFFER_SIZE */

/* Define the buffer for storing installedUpdateId as string.  */
#ifndef NX_AZURE_IOT_ADU_AGENT_UPDATE_ID_SIZE
#define NX_AZURE_IOT_ADU_AGENT_UPDATE_ID_SIZE                           256
#endif /* NX_AZURE_IOT_ADU_AGENT_UPDATE_ID_SIZE */

/* Define the buffer for storing file URLs.  */
#ifndef NX_AZURE_IOT_ADU_AGENT_FILE_URLS_SIZE
#define NX_AZURE_IOT_ADU_AGENT_FILE_URLS_SIZE                           512
#endif /* NX_AZURE_IOT_ADU_AGENT_FILE_URLS_SIZE */

/* Set the default timeout for DNS query.  */
#ifndef NX_AZURE_IOT_ADU_AGENT_PROPERTIES_SEND_TIMEOUT
#define NX_AZURE_IOT_ADU_AGENT_PROPERTIES_SEND_TIMEOUT                  (5 * NX_IP_PERIODIC_RATE)
#endif /* NX_AZURE_IOT_ADU_AGENT_PROPERTIES_SEND_TIMEOUT */

#define NX_AZURE_IOT_ADU_AGENT_PTR_UPDATE(a, b, c, d)                   { \
                                                                            (a) = (c); \
                                                                            (c) += (b); \
                                                                            (d) -= (b); \
                                                                        }

/* Define the initial timeout for DNS query, the default wait time is 1s.
   For the next retransmission, the timeout will be doubled.  */
#ifndef NX_AZURE_IOT_ADU_AGENT_DNS_INITIAL_TIMEOUT 
#define NX_AZURE_IOT_ADU_AGENT_DNS_INITIAL_TIMEOUT                      (1)
#endif /* NX_AZURE_IOT_ADU_AGENT_DNS_INITIAL_TIMEOUT */

/* Define the maximum number of retries to a DNS server. The default count is 3.  */
#ifndef NX_AZURE_IOT_ADU_AGENT_DNS_RETRANSMIT_COUNT 
#define NX_AZURE_IOT_ADU_AGENT_DNS_RETRANSMIT_COUNT                     (3)
#endif /* NX_AZURE_IOT_ADU_AGENT_DNS_RETRANSMIT_COUNT */

/* Define the window size of HTTP for downloading firmware.  */
#ifndef NX_AZURE_IOT_ADU_AGENT_HTTP_WINDOW_SIZE
#define NX_AZURE_IOT_ADU_AGENT_HTTP_WINDOW_SIZE                         (16 * 1024)
#endif /* NX_AZURE_IOT_ADU_AGENT_HTTP_WINDOW_SIZE  */

/* Define the timeout of HTTP for connecting. The default time is 30s.  */
#ifndef NX_AZURE_IOT_ADU_AGENT_HTTP_CONNECT_TIMEOUT
#define NX_AZURE_IOT_ADU_AGENT_HTTP_CONNECT_TIMEOUT                     (30)
#endif /* NX_AZURE_IOT_ADU_AGENT_HTTP_CONNECT_TIMEOUT */

/* Define the total timeout of HTTP for downloading the whole firmware. The default time is 300s.  */
#ifndef NX_AZURE_IOT_ADU_AGENT_HTTP_DOWNLOAD_TIMEOUT
#define NX_AZURE_IOT_ADU_AGENT_HTTP_DOWNLOAD_TIMEOUT                    (300)
#endif /* NX_AZURE_IOT_ADU_AGENT_HTTP_DOWNLOAD_TIMEOUT */

/* Define the http protocol string.  */
#define NX_AZURE_IOT_ADU_AGENT_HTTP_PROTOCOL                            "http://"

/* Define the downloader state.  */
#define NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_IDLE                          0
#define NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_URL_PARSED                    1
#define NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_ADDRESS_QUERY                 2
#define NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_ADDRESS_DONE                  3
#define NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_HTTP_CONNECT                  4
#define NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_HTTP_CONTENT_GET              5

/* Define ADU driver constants.  */
#define NX_AZURE_IOT_ADU_AGENT_DRIVER_INITIALIZE                        0
#define NX_AZURE_IOT_ADU_AGENT_DRIVER_PREPROCESS                        1
#define NX_AZURE_IOT_ADU_AGENT_DRIVER_WRITE                             2
#define NX_AZURE_IOT_ADU_AGENT_DRIVER_INSTALL                           3
#define NX_AZURE_IOT_ADU_AGENT_DRIVER_APPLY                             4

/* Define string macro.  */
#define NX_AZURE_IOT_ADU_AGENT_STRING(p)                                p, sizeof(p) - 1

/**
 * @brief ADU driver struct
 *
 */
typedef struct NX_AZURE_IOT_ADU_AGENT_DRIVER_STRUCT
{

    /* Define the driver command.  */
    UINT                                    nx_azure_iot_adu_agent_driver_command;

    /* Define the driver return status.  */
    UINT                                    nx_azure_iot_adu_agent_driver_status;

    /* Define the firmware size for the driver to preprocess.  */
    UINT                                    nx_azure_iot_adu_agent_driver_firmware_size;
    
    /* Define the firmware data for the driver to write.   */
    UINT                                    nx_azure_iot_adu_agent_driver_firmware_data_offset;
    UCHAR                                  *nx_azure_iot_adu_agent_driver_firmware_data_ptr; 
    UINT                                    nx_azure_iot_adu_agent_driver_firmware_data_size;

    /* Define the return pointer for raw driver command requests.  */
    ULONG                                  *nx_azure_iot_adu_agent_driver_return_ptr;

} NX_AZURE_IOT_ADU_AGENT_DRIVER;

/**
 * @brief ADU crypto struct.
 */
typedef struct NX_AZURE_IOT_ADU_AGENT_CRYPTO_STRUCT
{

    /* RS256.  */

    /* RSA. Reuse the metadata from TLS cipher metadata.  */
    NX_CRYPTO_METHOD                        *method_rsa;
    UCHAR                                   *method_rsa_metadata;
    ULONG                                    method_rsa_metadata_size;
    VOID                                    *handler;

    /* SHA256.  */
    NX_CRYPTO_METHOD                        *method_sha256;
    UCHAR                                    method_sha256_metadata[NX_AZURE_IOT_ADU_AGENT_SHA256_METADATA_SIZE];

} NX_AZURE_IOT_ADU_AGENT_CRYPTO;

/**
 * @brief ADU result struct.
 */
typedef struct NX_AZURE_IOT_ADU_AGENT_RESULT_STRUCT
{

    /* Result. 200 indicates success.  */
    UINT                                    result_code;

    /* Extended result code.  */
    UINT                                    extended_result_code;

} NX_AZURE_IOT_ADU_AGENT_RESULT;

/**
 * @brief ADU device properties struct.
 */
typedef struct NX_AZURE_IOT_ADU_AGENT_DEVICE_PROPERTIES_STRUCT
{

    /* Manufacturer.  */
    const UCHAR                            *manufacturer;
    UINT                                    manufacturer_length;

    /* Name/model.  */
    const UCHAR                            *model;
    UINT                                    model_length;

} NX_AZURE_IOT_ADU_AGENT_DEVICE_PROPERTIES;

/**
 * @brief ADU update id struct.
 */
typedef struct NX_AZURE_IOT_ADU_AGENT_UPDATE_ID_STRUCT
{

    /* Manufacturer.  */
    const UCHAR                            *provider;
    UINT                                    provider_length;

    /* Name/model.  */
    const UCHAR                            *name;
    UINT                                    name_length;

    /* Version. */
    const UCHAR                            *version;
    UINT                                    version_length;

    /* Buffer for storing update id as string.   */
    UCHAR                                   update_id_buffer[NX_AZURE_IOT_ADU_AGENT_UPDATE_ID_SIZE];
    UINT                                    update_id_length;

} NX_AZURE_IOT_ADU_AGENT_UPDATE_ID;

/**
 * @brief ADU update manifest content struct.
 */
typedef struct NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_CONTENT_STRUCT
{

    /* Manifest version.  */
    UCHAR                                  *manifest_version;
    UINT                                    manifest_version_length;

    /* Provider.  */
    UCHAR                                  *provider;
    UINT                                    provider_length;
    
    /* Name.  */
    UCHAR                                  *name;
    UINT                                    name_length;

    /* Version.  */
    UCHAR                                  *version;
    UINT                                    version_length;

    /* Update type.  */
    UCHAR                                  *update_type;
    UINT                                    update_type_length;

    /* Installed criteria.  */
    UCHAR                                  *installed_criteria;
    UINT                                    installed_criteria_length;

    /* File name.  */
    UCHAR                                  *file_name;
    UINT                                    file_name_length;

    /* File size in bytes.  */
    UINT                                    file_size_in_bytes;

    /* File sha256.  */
    UCHAR                                  *file_sha256;
    UINT                                    file_sha256_length;

    /* Created date time.  */
    UCHAR                                  *created_date_time;
    UINT                                    created_date_time_length;

} NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_CONTENT;

/**
 * @brief ADU file url struct.
 */
typedef struct NX_AZURE_IOT_ADU_AGENT_FILE_URLS_STRUCT
{
    
    /* File number string.  */
    UCHAR                                  *file_number;
    UINT                                    file_number_length;

    /* File URL.  */
    UCHAR                                  *file_url;
    UINT                                    file_url_length;

    /* Buffer.   */
    UCHAR                                   file_buffer[NX_AZURE_IOT_ADU_AGENT_FILE_URLS_SIZE];

} NX_AZURE_IOT_ADU_AGENT_FILE_URLS;

/**
 * @brief ADU downloader struct.
 */
typedef struct NX_AZURE_IOT_ADU_AGENT_DOWNLOADER_STRUCT
{

    /* HTTP Client for downloading firmware.  */
    NX_WEB_HTTP_CLIENT                      http_client;

    /* Host string.  */
    UCHAR                                  *host;
    UINT                                    host_length;

    /* Resoruce string.  */
    UCHAR                                  *resource;
    UINT                                    resource_length;

    /* HTTP server address.  */
    NXD_ADDRESS                             address;

    /* HTTP server port.  */
    UINT                                    port;

    /* Received firmware size.  */
    UINT                                    received_firmware_size;

    /* Downloading state.  */
    UINT                                    state;

    /* DNS.  */
    NX_DNS                                 *dns_ptr;

    /* DNS query count.  */
    UINT                                    dns_query_count;

    /* Timeout.  */
    ULONG                                   timeout;

} NX_AZURE_IOT_ADU_AGENT_DOWNLOADER;

/**
 * @brief ADU RSA root key struct.
 */
typedef struct NX_AZURE_IOT_ADU_AGENT_RSA_ROOT_KEY_STRUCT
{
    const UCHAR                            *kid;
    const UINT                              kid_size;

    const UCHAR                             *n;
    const UINT                               n_size;

    const UCHAR                            *e;
    const UINT                              e_size;
} NX_AZURE_IOT_ADU_AGENT_RSA_ROOT_KEY;


/**
 * @brief ADU agent struct
 *
 */
typedef struct NX_AZURE_IOT_ADU_AGENT_STRUCT
{

    /* PnP client pointer.  */
    NX_AZURE_IOT_PNP_CLIENT                *nx_azure_iot_pnp_client_ptr;

    /* Mutex.  */
    TX_MUTEX                               *nx_azure_iot_adu_agent_mutex_ptr;

    /* Cloud module.  */
    NX_CLOUD_MODULE                         nx_azure_iot_adu_agent_cloud_module;

    /* Downloader.  */
    NX_AZURE_IOT_ADU_AGENT_DOWNLOADER       nx_azure_iot_adu_agent_downloader;

    /* ADU crypto.  */
    NX_AZURE_IOT_ADU_AGENT_CRYPTO           nx_azure_iot_adu_agent_crypto;

    /* ADU device properties.  */
    NX_AZURE_IOT_ADU_AGENT_DEVICE_PROPERTIES nx_azure_iot_adu_agent_device_properties;

    /* ADU update id.  */
    NX_AZURE_IOT_ADU_AGENT_UPDATE_ID        nx_azure_iot_adu_agent_current_update_id;

    /* Previously reported state (for state validation). */
    UINT                                    nx_azure_iot_adu_agent_last_reported_state;

    /* Is an upper-lever metod currently in progress.  */
    UINT                                    nx_azure_iot_adu_agent_operation_in_progress;

    /* Was the operation in progress requested to cancel.  */
    UINT                                    nx_azure_iot_adu_agent_operation_cancelled;

    /* Action.  */
    UINT                                    nx_azure_iot_adu_agent_action;

    /* Update manifest string.  FIXME: buffer size, consider using a packet.  */
    UCHAR                                   nx_azure_iot_adu_agent_update_manifest[NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_SIZE];
    UINT                                    nx_azure_iot_adu_agent_update_manifest_size;

    /* Update manifest signature. FIXME: buffer size */
    UCHAR                                   nx_azure_iot_adu_agent_update_manifest_signature[NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_SIGNATURE_SIZE];
    UINT                                    nx_azure_iot_adu_agent_update_manifest_signature_size;

    /* SJWK. FIXME: buffer size */
    UCHAR                                   nx_azure_iot_adu_agent_update_manifest_sjwk[NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_SJWK_SIZE];
    UINT                                    nx_azure_iot_adu_agent_update_manifest_sjwk_size;

    /* File URL.  */
    NX_AZURE_IOT_ADU_AGENT_FILE_URLS        nx_azure_iot_adu_agent_file_urls;

    /* Update manifest sub contents.  */
    NX_AZURE_IOT_ADU_AGENT_UPDATE_MANIFEST_CONTENT nx_azure_iot_adu_agent_update_manifest_content;

    /* Buffer for storing file number string, file urls and update manifest content.  FIXME: buffer size, consider using packet.  */
    UCHAR                                   nx_azure_iot_adu_agent_buffer[NX_AZURE_IOT_ADU_AGENT_BUFFER_SIZE];
    UINT                                    nx_azure_iot_adu_agent_buffer_size;

    /* Define the callback function for ADU agent state change notification. If specified
       by the application, this function is called whenever a state change occurs.  */
    VOID                                  (*nx_azure_iot_adu_agent_state_change_notify)(struct NX_AZURE_IOT_ADU_AGENT_STRUCT *adu_agent_ptr, UINT new_state);

    /* Define the Driver entry point.  */
    VOID                                  (*nx_azure_iot_adu_agent_driver_entry)(NX_AZURE_IOT_ADU_AGENT_DRIVER *);

} NX_AZURE_IOT_ADU_AGENT;

/**
 * @brief Start Azure IoT ADU agent
 *
 * @param[in] adu_agent_ptr A pointer to a #NX_AZURE_IOT_ADU_AGENT.
 * @param[in] pnp_client_ptr A pointer to a #NX_AZURE_IOT_PNP_CLIENT.
 * @param[in] manufacturer A `UCHAR` pointer to the manufacturer
 * @param[in] manufacturer_length Length of the `manufacturer`. Does not include the `NULL` terminator.
 * @param[in] model A `UCHAR` pointer to the model
 * @param[in] model_length Length of the `model`. Does not include the `NULL` terminator.
 * @param[in] provider A `UCHAR` pointer to the provider.
 * @param[in] provider_length Length of the `provider`. Does not include the `NULL` terminator.
 * @param[in] name A `UCHAR` pointer to the name
 * @param[in] name_length Length of the `name`. Does not include the `NULL` terminator.
 * @param[in] version A `UCHAR` pointer to the version
 * @param[in] version_length Length of the `version`. Does not include the `NULL` terminator.
 * @param[in] adu_agent_state_change_notify User supplied state change callback.
 * @param[in] adu_agent_driver User supplied driver for flash operation.
 * @return A `UINT` with the result of the API.
 *   @retval #NX_AZURE_IOT_SUCCESS Successfully start ADU agent.
 *   @retval #NX_AZURE_IOT_INVALID_PARAMETER Fail to start the Azure IoT ADU agent due to invalid parameter.
 *   @retval #NX_AZURE_IOT_NO_AVAILABLE_CIPHER Fail to start the Azure IoT ADU agent due to no available cipher.
 *   @retval #NX_AZURE_IOT_INSUFFICIENT_BUFFER_SPACE Fail to start the Azure IoT ADU agent due to insufficient buffer space.
 */
UINT nx_azure_iot_adu_agent_start(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr,
                                  NX_AZURE_IOT_PNP_CLIENT *pnp_client_ptr,
                                  const UCHAR *manufacturer, UINT manufacturer_length,
                                  const UCHAR *model, UINT model_length,
                                  const UCHAR *provider, UINT provider_length,
                                  const UCHAR *name, UINT name_length,
                                  const UCHAR *version, UINT version_length,
                                  VOID (*adu_agent_state_change_notify)(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr, UINT new_state),
                                  VOID (*adu_agent_driver)(NX_AZURE_IOT_ADU_AGENT_DRIVER *));

/**
 * @brief Apply the new update immediately. Note: The device will reboot and the routine should not return once applying the update successfully.
 *
 * @param[in] adu_agent_ptr A pointer to a #NX_AZURE_IOT_ADU_AGENT.
 * @return A `UINT` with the result of the API.
 *   @retval #NX_AZURE_IOT_INVALID_PARAMETER Fail to apply new firmware due to invalid parameter.
 *   @retval #NX_AZURE_IOT_FAILURE Fail to apply new firmware due to driver issue.
 */
UINT nx_azure_iot_adu_agent_update_apply(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr);

#ifdef __cplusplus
}
#endif
#endif /* NX_AZURE_IOT_ADU_AGENT_H */
