#include "paraswap_plugin.h"

void handle_query_contract_id(void *parameters) {
    ethQueryContractID_t *msg = (ethQueryContractID_t *) parameters;
    const paraswap_parameters_t *context = (paraswap_parameters_t *) msg->pluginContext;

    strlcpy(msg->name, PLUGIN_NAME, msg->nameLength);

    switch (context->selectorIndex) {
        case JOIN_POOL_VIA_0X:
        //case JOIN_POOL_VIA_0X_ETH:
            strlcpy(msg->version, "9Deposit via 0x", msg->versionLength);
            break;
        /*case REMOVE_LIQUIDITY:
            strlcpy(msg->version, "Withdraw", msg->versionLength);
            break;*/
        default:
            PRINTF("Selector Index :%d not supported\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}
