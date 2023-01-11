#include "paraswap_plugin.h"

void handle_finalize(void *parameters) {
    ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
    paraswap_parameters_t *context = (paraswap_parameters_t *) msg->pluginContext;
    if (context->valid) {
        /**
         * NUMBER OF SCREENS
         */
        switch (context->selectorIndex) {
            case JOIN_POOL_VIA_0X:
            case JOIN_POOL_VIA_SWAAP:
                msg->numScreens = 1;
                break;
            case EXIT:
                msg->numScreens = 0;
                break;
            default:
                PRINTF("Missing selectorIndex\n");
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
        }
        PRINTF("=== finalize screen %d\n", msg->numScreens);
        /**
         * INFORMATIONS OF TOKENS : swaap team didn't touch that part, directly from Paraswap plugin
         */
        if (!ADDRESS_IS_NETWORK_TOKEN(context->token_address_a)) {
            // Address is not network token (0xeee...) so we will need to look up the token in the
            // CAL.
            msg->tokenLookup1 = context->token_address_a;  // from token_address_a
            PRINTF("Setting address sent to: %.*H\n", ADDRESS_LENGTH, context->token_address_a);

            // The user is not swapping ETH, so make sure there's no ETH being sent in this tx.
            // TODO: Swaap does not swap yet, so probably not needed. Better to keep for later
            // though
            if (!allzeroes(msg->pluginSharedRO->txContent->value.value,
                           msg->pluginSharedRO->txContent->value.length)) {
                PRINTF("ETH attached to tx when token being swapped is %.*H\n",
                       sizeof(context->token_address_a),
                       context->token_address_a);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
            }
        } else {
            msg->tokenLookup1 = NULL;
            // We have MATIC and 18
        }
        if (!ADDRESS_IS_NETWORK_TOKEN(context->token_address_b)) {
            // Address is not network token (0xeee...) so we will need to look up the token in the
            // CAL.
            PRINTF("Setting address receiving to: %.*H\n",
                   ADDRESS_LENGTH,
                   context->token_address_b);
            msg->tokenLookup2 = context->token_address_b;
        } else {
            msg->tokenLookup2 = NULL;
            // We have MATIC and 18
        }

        msg->uiType = ETH_UI_TYPE_GENERIC;
        msg->result = ETH_PLUGIN_RESULT_OK;
    } else {
        PRINTF("Context not valid\n");
        msg->result = ETH_PLUGIN_RESULT_FALLBACK;
    }
}
