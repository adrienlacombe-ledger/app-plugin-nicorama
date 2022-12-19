#include "paraswap_plugin.h"

// Copy amount sent parameter to amount_sent
static void handle_amount_sent(const ethPluginProvideParameter_t *msg,
                               paraswap_parameters_t *context) {
    copy_parameter(context->amount_sent, msg->parameter, sizeof(context->amount_sent));
}

// Copy amount sent parameter to amount_received
static void handle_amount_received(const ethPluginProvideParameter_t *msg,
                                   paraswap_parameters_t *context) {
    copy_parameter(context->amount_received, msg->parameter, sizeof(context->amount_received));
}

static void handle_beneficiary(const ethPluginProvideParameter_t *msg,
                               paraswap_parameters_t *context) {
    memset(context->beneficiary, 0, sizeof(context->beneficiary));
    memcpy(context->beneficiary,
           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
           sizeof(context->beneficiary));
    PRINTF("BENEFICIARY: %.*H\n", ADDRESS_LENGTH, context->beneficiary);
}

static void handle_array_len(const ethPluginProvideParameter_t *msg,
                             paraswap_parameters_t *context) {
    context->array_len = msg->parameter[PARAMETER_LENGTH - 1];
    PRINTF("LIST LEN: %d\n", context->array_len);
}

static void contract_address_sent(const ethPluginProvideParameter_t *msg,
                              paraswap_parameters_t *context) {
    memset(context->contract_address_sent, 0, sizeof(context->contract_address_sent));
    memcpy(context->contract_address_sent,
           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
           sizeof(context->contract_address_sent));
    PRINTF("TOKEN SENT: %.*H\n", ADDRESS_LENGTH, context->contract_address_sent);
}

static void handle_token_sent(const ethPluginProvideParameter_t *msg,
                              paraswap_parameters_t *context) {
    memset(context->contract_address_sent, 0, sizeof(context->contract_address_sent));
    memcpy(context->contract_address_sent,
           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
           sizeof(context->contract_address_sent));
    PRINTF("TOKEN SENT: %.*H\n", ADDRESS_LENGTH, context->contract_address_sent);
}

static void handle_token_received(const ethPluginProvideParameter_t *msg,
                                  paraswap_parameters_t *context) {
    memset(context->contract_address_received, 0, sizeof(context->contract_address_received));
    memcpy(context->contract_address_received,
           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
           sizeof(context->contract_address_received));
    PRINTF("TOKEN RECEIVED: %.*H\n", ADDRESS_LENGTH, context->contract_address_received);
}


static void handle_add_remove_liquidity(ethPluginProvideParameter_t *msg,
                                        paraswap_parameters_t *context) {
    switch (context->next_param) {
        case INITIAL_OFFSET:
            // do nothing except skipping 3 lines
            context->offset = U2BE(msg->parameter, 3);
            context->next_param = TOKEN_SENT;
            //context->go_to_offset = true;
            break;

        case TOKEN_SENT:  // tokenA
            handle_token_sent(msg, context);
            context->next_param = AMOUNT_SENT;
            break;
        case AMOUNT_SENT:  // TokenA Min Amount
            handle_amount_sent(msg, context);
            context->next_param = TOKEN_RECEIVED;
            break;
        case TOKEN_RECEIVED:  // TokenB
            handle_token_received(msg, context);
            context->next_param = AMOUNT_RECEIVED;

            // don't know yet the purpose of this
            if (context->selectorIndex == JOIN_POOL_VIA_0X) {
                //context->skip = 2;
            } else {
                //context->skip = 1;
            }

            break;
        case AMOUNT_RECEIVED:  // TokenB Min Amount
            handle_amount_received(msg, context);
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:  // to
            handle_beneficiary(msg, context);
            context->next_param = NONE;
            break;
        case NONE:
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_add_remove_liquidity_eth(ethPluginProvideParameter_t *msg,
                                            paraswap_parameters_t *context) {
    switch (context->next_param) {
        case TOKEN_SENT:  // tokenA
            handle_token_sent(msg, context);
            context->next_param = AMOUNT_RECEIVED;
            //context->skip = 1;
            break;
        case AMOUNT_RECEIVED:  // TokenA Min Amount
            handle_amount_received(msg, context);
            context->next_param = AMOUNT_SENT;
            break;
        case AMOUNT_SENT:  // ETH Min Amount
            handle_amount_sent(msg, context);
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:  // to
            handle_beneficiary(msg, context);
            context->next_param = NONE;
            break;
        case NONE:
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}


void handle_provide_parameter(void *parameters) {
    ethPluginProvideParameter_t *msg = (ethPluginProvideParameter_t *) parameters;
    paraswap_parameters_t *context = (paraswap_parameters_t *) msg->pluginContext;

    msg->result = ETH_PLUGIN_RESULT_OK;

    if (context->skip) {
        // Skip this step, and don't forget to decrease skipping counter.
        context->skip--;
    } else {
        if ((context->offset) && msg->parameterOffset != context->checkpoint + context->offset) {
            PRINTF("offset: %d, checkpoint: %d, parameterOffset: %d\n",
                   context->offset,
                   context->checkpoint,
                   msg->parameterOffset);
            return;
        }
        context->offset = 0;  // Reset offset
        switch (context->selectorIndex) {

                    case JOIN_POOL_VIA_0X:
                    //case REMOVE_LIQUIDITY:
                        handle_add_remove_liquidity(msg, context);
                        break;

                    /*case JOIN_POOL_VIA_0X:_ETH:
                        handle_add_remove_liquidity_eth(msg, context);
                        break;
                    */
                    default:
                        PRINTF("Selector Index %d not supported\n", context->selectorIndex);
                        msg->result = ETH_PLUGIN_RESULT_ERROR;
                        break;
                }
    }
}
