#include "paraswap_plugin.h"

void handle_provide_token(void *parameters) {
    ethPluginProvideInfo_t *msg = (ethPluginProvideInfo_t *) parameters;
    paraswap_parameters_t *context = (paraswap_parameters_t *) msg->pluginContext;
    PRINTF("PARASWAP plugin provide token: 0x%p, 0x%p\n", msg->item1, msg->item2);

    if (ADDRESS_IS_NETWORK_TOKEN(context->token_address_a)) {
        context->decimals_a = WEI_TO_ETHER;
        context->tokens_found |= TOKEN_SENT_FOUND;
    } else if (msg->item1 != NULL) {
        context->decimals_a = msg->item1->token.decimals;
        strlcpy(context->ticker_a,
                (char *) msg->item1->token.ticker,
                sizeof(context->ticker_a));
        context->tokens_found |= TOKEN_SENT_FOUND;
    } else {
        // CAL did not find the token and token is not ETH.
        context->decimals_a = DEFAULT_DECIMAL;
        // We will need an additional screen to display a warning message.
        msg->additionalScreens++;
    }

    if (ADDRESS_IS_NETWORK_TOKEN(context->token_address_b)) {
        context->decimals_b = WEI_TO_ETHER;
        context->tokens_found |= TOKEN_RECEIVED_FOUND;
    } else if (msg->item2 != NULL) {
        context->decimals_b = msg->item2->token.decimals;
        strlcpy(context->ticker_b,
                (char *) msg->item2->token.ticker,
                sizeof(context->ticker_b));
        context->tokens_found |= TOKEN_RECEIVED_FOUND;
    } else {
        // CAL did not find the token and token is not ETH.
        context->decimals_b = DEFAULT_DECIMAL;
        // We will need an additional screen to display a warning message.
        msg->additionalScreens++;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}
