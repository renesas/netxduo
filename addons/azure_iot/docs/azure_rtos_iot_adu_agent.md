# Azure IoT ADU Agent

**nx_azure_iot_adu_agent_start**
***
<div style="text-align: right"> Start Azure IoT ADU agent. </div>

**Prototype**
```c
UINT nx_azure_iot_adu_agent_start(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr,
                                  NX_AZURE_IOT_PNP_CLIENT *pnp_client_ptr,
                                  const UCHAR *manufacturer, UINT manufacturer_length,
                                  const UCHAR *model, UINT model_length,
                                  const UCHAR *provider, UINT provider_length,
                                  const UCHAR *name, UINT name_length,
                                  const UCHAR *version, UINT version_length,
                                  VOID (*adu_agent_state_change_notify)(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr, UINT new_state),
                                  VOID (*adu_agent_driver)(NX_AZURE_IOT_ADU_AGENT_DRIVER *));
```
**Description**

<p>This routine starts the ADU agent.</p>

**Parameters**

| Name | Description |
| - |:-|
| adu_agent_ptr [in] | A pointer to a `NX_AZURE_IOT_ADU_AGENT`. |
| pnp_client_ptr [in] | A pointer to a `NX_AZURE_IOT_PNP_CLIENT`.|
| manufacturer [in] | A pointer to the manufacturer. Must be NULL terminated string. |
| manufacturer_length [in] | Length of the manufacturer.  |
| model [in]  | A pointer to the model. Must be NULL terminated string. |
| model_length [in] | Length of the model. |
| provider [in]  | A pointer to the update provider. Must be NULL terminated string. |
| provider_length [in] | Length of the update provider. |
| name [in]  | A pointer to the update name. Must be NULL terminated string. |
| name_length [in] | Length of the update name. |
| version [in]  | A pointer to the update version. Must be NULL terminated string. |
| version_length [in] | Length of the update version. |
| adu_agent_state_change_notify [in] | Pointer to a callback function invoked once adu status is changed. |
| adu_agent_driver [in] | User supplied driver for flash operation. |

**Return Values**
* NX_AZURE_IOT_SUCCESS Successfully started the Azure IoT ADU agent.
* NX_AZURE_IOT_INVALID_PARAMETER Fail to start the Azure IoT ADU agent due to invalid parameter.
* NX_AZURE_IOT_NO_AVAILABLE_CIPHER Fail to start the Azure IoT ADU agent due to no available cipher.
* NX_AZURE_IOT_INSUFFICIENT_BUFFER_SPACE Fail to start the Azure IoT ADU agent due to insufficient buffer space.

**Allowed From**

Threads

**Example**

**See Also**

<div style="page-break-after: always;"></div>

**nx_azure_iot_adu_agent_update_apply**
***
<div style="text-align: right"> Apply the new update immediately. </div>

**Prototype**
```c
UINT nx_azure_iot_adu_agent_update_apply(NX_AZURE_IOT_ADU_AGENT *adu_agent_ptr);
```
**Description**

<p>The routine applies the new update immediately. Note: The device will reboot and the routine should not return once applying the update successfully.</p>

**Parameters**
|               |               |
| - |:-|
| adu_agent_ptr [in]    | A pointer to a `NX_AZURE_IOT_ADU_AGENT` |


**Return Values**
* NX_AZURE_IOT_INVALID_PARAMETER Fail to apply the new update due to invalid parameter.
* NX_AZURE_IOT_FAILURE Fail to apply the new update due to driver error.

**Allowed From**

Threads

**Example**

**See Also**

<div style="page-break-after: always;"></div>
