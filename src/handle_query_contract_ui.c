#include "paraswap_plugin.h"

// Set UI for the "Send" screen.
static void set_send_ui(ethQueryContractUI_t *msg, paraswap_parameters_t *context) {
    switch (context->selectorIndex) {

        case JOIN_POOL_VIA_0X:
        //case JOIN_POOL_VIA_0X_ETH:
            strlcpy(msg->title, "Send", msg->titleLength);
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
    if (ADDRESS_IS_NETWORK_TOKEN(context->contract_address_sent)) {
            strlcpy(context->ticker_sent, msg->network_ticker, sizeof(context->ticker_sent));
        }

        amountToString(context->amount_sent,
                       sizeof(context->amount_sent),
                       context->decimals_sent,
                       context->ticker_sent,
                       msg->msg,
                       msg->msgLength);
}
// Set UI for "Receive" screen.
static void set_receive_ui(ethQueryContractUI_t *msg, paraswap_parameters_t *context) {
    switch (context->selectorIndex) {
        case JOIN_POOL_VIA_0X:
        //case JOIN_POOL_VIA_0X_ETH:
            strlcpy(msg->title, "Receive min", msg->titleLength);
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
    if (ADDRESS_IS_NETWORK_TOKEN(context->contract_address_received)) {
            strlcpy(context->ticker_received, msg->network_ticker, sizeof(context->ticker_received));
        }

        amountToString(context->amount_received,
                       sizeof(context->amount_received),
                       context->decimals_received,
                       context->ticker_received,
                       msg->msg,
                       msg->msgLength);
}


// Set UI for "Warning" screen.
static void set_warning_ui(ethQueryContractUI_t *msg,
                           const paraswap_parameters_t *context __attribute__((unused))) {
    strlcpy(msg->title, "WARNING", msg->titleLength);
    strlcpy(msg->msg, "Unknown token", msg->msgLength);
}

// Helper function that returns the enum corresponding to the screen that should be displayed.
static screens_t get_screen(const ethQueryContractUI_t *msg, const paraswap_parameters_t *context) {
    uint8_t index = msg->screenIndex;

    bool token_sent_found = context->tokens_found & TOKEN_SENT_FOUND;

    bool token_received_found = context->tokens_found & TOKEN_RECEIVED_FOUND;



    PRINTF("=== get_screen: Current screen index %d \n", index);
    //PRINTF("=== token_sent_found %d \n", token_sent_found);
    PRINTF("=== token_received_found %d \n", token_received_found);
    //PRINTF("=== both_tokens_found %d \n", both_tokens_found);
    //PRINTF("=== both_tokens_not_found %d \n", both_tokens_not_found);

    // SEND_SCREEN  0
    // RECEIVE_SCREEN 1
    // WARN_SCREEN  2
    // ERROR  3
    switch (index) {
        case 0:
            if (token_sent_found) {
                return SEND_SCREEN;
            } else{
                return WARN_SCREEN;
            }
            break;
        case 1:
            if (token_sent_found) {
                if (token_received_found){
                    return RECEIVE_SCREEN;
                } else {
                    return WARN_SCREEN;
                }
            } else {
                return SEND_SCREEN;
            }
            break;

        case 2:
           if (token_sent_found) {
               if (token_received_found){
                   PRINTF("=== Error, we already made all screen");
                   return ERROR; // we made all screen
               } else {
                   return RECEIVE_SCREEN;
               }
           } else {
               if (token_received_found){
                  return RECEIVE_SCREEN;
              } else {
                  return WARN_SCREEN;
              }
           }
           break;
        case 3:
           if (token_sent_found) {
               PRINTF("=== index 3, found sent. Error, we already made all screen");
               return ERROR; // we made all screen
           } else {
               if (token_received_found){
                  PRINTF("=== index 3, found receive. Error, we already made all screen");
                  return ERROR;
              } else {
                  return RECEIVE_SCREEN;
              }
           }
           break;
        case 4:
            PRINTF("=== Screen index 4, should not happen");
            return ERROR;
        default:
            return ERROR;
            break;
    }
    return ERROR;
}

void handle_deposit_screen(){


}


void handle_query_contract_ui(void *parameters) {
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
        case SEND_SCREEN:
            set_send_ui(msg, context);
            break;
        case RECEIVE_SCREEN:
            set_receive_ui(msg, context);
            break;
        case WARN_SCREEN:
            set_warning_ui(msg, context);
            break;
        default:
            PRINTF("=== Received an invalid screenIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}
