#include "paraswap_plugin.h"

// Set UI for the "Send" screen.
static void set_send_ui(ethQueryContractUI_t *msg, paraswap_parameters_t *context) {
    switch (context->selectorIndex) {
        case JOIN_POOL_VIA_0X:
        case JOIN_POOL_VIA_SWAAP:
            strlcpy(msg->title, "Deposit", msg->titleLength);
            break;

            /*case REMOVE_LIQUIDITY:
                strlcpy(msg->title, "Send Max", msg->titleLength);
                break;
                */

        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    // Added from paraswap plugin, was not on Quickswap plugin
    if (ADDRESS_IS_NETWORK_TOKEN(context->token_address_a)) {
        strlcpy(context->ticker_a, msg->network_ticker, sizeof(context->ticker_a));
    }

    amountToString(context->amount_a,
                   sizeof(context->amount_a),
                   context->decimals_a,
                   context->ticker_a,
                   msg->msg,
                   msg->msgLength);
}
// Set UI for "Receive" screen.
static void set_receive_ui(ethQueryContractUI_t *msg, paraswap_parameters_t *context) {
    switch (context->selectorIndex) {
        case JOIN_POOL_VIA_0X:
            // case JOIN_POOL_VIA_0X_ETH:
            strlcpy(msg->title, "Exit", msg->titleLength);
            break;

            /*case REMOVE_LIQUIDITY:
                strlcpy(msg->title, "Receive min", msg->titleLength);
                break;
                */

        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    // Added from paraswap plugin, was not on Quickswap plugin
    if (ADDRESS_IS_NETWORK_TOKEN(context->token_address_b)) {
        strlcpy(context->ticker_b, msg->network_ticker, sizeof(context->ticker_b));
    }

    amountToString(context->amount_b,
                   sizeof(context->amount_b),
                   context->decimals_b,
                   context->ticker_b,
                   msg->msg,
                   msg->msgLength);
}

// Helper function that returns the enum corresponding to the screen that should be displayed.
static screens_t get_screen(const ethQueryContractUI_t *msg, const paraswap_parameters_t *context) {
    uint8_t index = msg->screenIndex;

    bool token_a_found = context->tokens_found & TOKEN_SENT_FOUND;

    bool token_b_found = context->tokens_found & TOKEN_RECEIVED_FOUND;

    PRINTF("=== get_screen: Current screen index %d \n", index);
    // PRINTF("=== token_a_found %d \n", token_a_found);
    PRINTF("=== ### token_a_found %d \n", token_a_found);
    // PRINTF("=== both_tokens_found %d \n", both_tokens_found);
    // PRINTF("=== both_tokens_not_found %d \n", both_tokens_not_found);

    // DEPOSIT_SCREEN  0
    // EXIT_SCREEN 1
    // WARN_SCREEN  2
    // ERROR  3
    switch (index) {
        case 0:
            if (token_a_found) {
                return DEPOSIT_SCREEN;
            } else {
                return ERROR;
            }
            break;
        default:
            return ERROR;
            break;
    }
    return ERROR;
}

void handle_deposit_screen() {
}

void handle_query_contract_ui(void *parameters) {
    PRINTF("=== start handle_query_contract_ui");
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    paraswap_parameters_t *context = (paraswap_parameters_t *) msg->pluginContext;

    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);
    msg->result = ETH_PLUGIN_RESULT_OK;

    /*switch (context->selectorIndex) {
        case JOIN_POOL_VIA_0X:
        default:
            handle_deposit_screen(msg, context);
            return;
    }*/

    screens_t screen = get_screen(msg, context);
    PRINTF("=== Found screen: %d\n", screen);
    switch (screen) {
        case DEPOSIT_SCREEN:
            set_send_ui(msg, context);
            break;
        case EXIT_SCREEN:
            set_receive_ui(msg, context);
            break;
        default:
            PRINTF("=== Received an invalid screenIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}
