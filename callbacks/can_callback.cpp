#include "../can.hpp"

#ifdef HAL_CAN_MODULE_ENABLED
#ifndef STMBED_MANUAL_CAN_CB
#ifdef CAN_RX_FIFO0
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    using namespace stmbed;
    CAN_RxHeaderTypeDef rx_header;
    CANMessage msg;

    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, msg.data.data()) == HAL_OK) {
        msg.id = (rx_header.IDE == CAN_ID_STD) ? rx_header.StdId : rx_header.ExtId;
        msg.format = (rx_header.IDE == CAN_ID_STD) ? CANFormat::CANStandard : CANFormat::CANExtended;
        msg.size = rx_header.DLC;
        callback::callback<CAN::CallbackFnType>(reinterpret_cast<intptr_t>(hcan), msg);
    }
}
#endif // CAN_RX_FIFO0
#endif // STMBED_MANUAL_CAN_CB
#endif // HAL_CAN_MODULE_ENABLED

#include <stdio.h>

#ifdef HAL_FDCAN_MODULE_ENABLED
#ifndef STMBED_MANUAL_CAN_CB
#ifdef FDCAN_RX_FIFO0
// コールバックの実装
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
    using namespace stmbed;
    FDCAN_RxHeaderTypeDef rx_header;
    CANMessage msg;

    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rx_header, msg.data.data()) == HAL_OK) {
        msg.id = rx_header.Identifier;
        msg.size = rx_header.DataLength;
        callback::callback<CAN::CallbackFnType>(reinterpret_cast<intptr_t>(hfdcan), msg);
    }
}
#endif // FDCAN_RX_FIFO0
#endif // STMBED_MANUAL_CAN_CB
#endif // HAL_FDCAN_MODULE_ENABLED
