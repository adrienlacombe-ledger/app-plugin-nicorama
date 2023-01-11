#include "paraswap_plugin.h"

// Copy amount sent parameter to amount_a
static void handle_amount_a(const ethPluginProvideParameter_t *msg,
                            paraswap_parameters_t *context) {
    copy_parameter(context->amount_a, msg->parameter, sizeof(context->amount_a));
}

// Copy amount sent parameter to amount_b
static void handle_amount_b(const ethPluginProvideParameter_t *msg,
                            paraswap_parameters_t *context) {
    copy_parameter(context->amount_b, msg->parameter, sizeof(context->amount_b));
}

static void handle_array_len(const ethPluginProvideParameter_t *msg,
                             paraswap_parameters_t *context) {
    context->array_len = msg->parameter[PARAMETER_LENGTH - 1];
    PRINTF("LIST LEN: %d\n", context->array_len);
}

static void handle_token_a(const ethPluginProvideParameter_t *msg, paraswap_parameters_t *context) {
    memset(context->token_address_a, 0, sizeof(context->token_address_a));
    memcpy(context->token_address_a,
           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
           sizeof(context->token_address_a));
    PRINTF("=== TOKEN SENT: %.*H\n", ADDRESS_LENGTH, context->token_address_a);
}

static void handle_token_b(const ethPluginProvideParameter_t *msg, paraswap_parameters_t *context) {
    memset(context->token_address_b, 0, sizeof(context->token_address_b));
    memcpy(context->token_address_b,
           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
           sizeof(context->token_address_b));
    PRINTF("=== TOKEN RECEIVED: %.*H\n", ADDRESS_LENGTH, context->token_address_b);
}

static void handle_0x_deposit(ethPluginProvideParameter_t *msg, paraswap_parameters_t *context) {
    switch (context->next_param) {
        case INITIAL_OFFSET:
            // To skip the 3 first lines, we need to put context->skip=2 because we are already at
            // the first line
            context->skip = 2;
            PRINTF("=== Skipping: %d \n", context->skip);
            context->next_param = TOKEN_A;

            // context->next_param = NONE;
            // context->go_to_offset = true;
            break;

        case TOKEN_A:  // tokenA
            handle_token_a(msg, context);
            context->next_param = AMOUNT_DEPOSIT;
            break;
        case AMOUNT_DEPOSIT:  // TokenA Min Amount
            handle_amount_a(msg, context);
            PRINTF("=== Amount sent:%d \n", context->amount_a);
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

static void handle_Swaap_deposit(ethPluginProvideParameter_t *msg, paraswap_parameters_t *context) {
    switch (context->next_param) {
        case INITIAL_OFFSET:
            // To skip the 1 first lines, we need to put context->skip=0 because we are already at
            // the first line
            context->skip = 0;
            PRINTF("=== Skipping: %d \n", context->skip);
            context->next_param = TOKEN_A;
            break;

        case TOKEN_A:  // tokenA
            handle_token_a(msg, context);
            context->next_param = AMOUNT_DEPOSIT;
            break;
        case AMOUNT_DEPOSIT:  // TokenA Min Amount
            handle_amount_a(msg, context);
            PRINTF("=== Amount sent:%d \n", context->amount_a);
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

static void handle_exit(ethPluginProvideParameter_t *msg, paraswap_parameters_t *context) {
    switch (context->next_param) {
            // We don't need to show how much is exited, so we are skipping SLP
        case TOKEN_B:  // Pool token
            handle_token_b(msg, context);
            context->next_param = AMOUNT_EXITED;
            break;
        case AMOUNT_EXITED:  // Amount of SPT
            handle_amount_b(msg, context);
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
        //        PRINTF("===# Skipping %d\n", context->skip);
        context->skip--;
        PRINTF("=== Skipping\n");

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
                handle_0x_deposit(msg, context);
                break;
            case JOIN_POOL_VIA_SWAAP:
                handle_Swaap_deposit(msg, context);
                break;
            case EXIT:
                handle_exit(msg, context);
                break;

            default:
                PRINTF("Selector Index %d not supported\n", context->selectorIndex);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
        }
    }
}
