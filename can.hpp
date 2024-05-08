#pragma once
#include "callbacks/callback.hpp"
#include <array>
#include <cstring>

namespace stmbed {
enum CANFormat { CANStandard = 0, CANExtended = 1 };

// enum CANType
// {
//     CANData = 0,
//     CANRemote = 2
// };

struct CANMessage {
    CANMessage() = default;

    CANMessage(uint32_t _id, unsigned char *_data, unsigned char _size = 8) {
        size = (_size > 8) ? 8 : _size;
        id = _id;
        memcpy(data.data(), _data, size);
    }

    CANMessage(uint32_t _id, std::array<uint8_t, 8> _data) {
        id = _id;
        data = _data;
        size = 8;
    }

    uint32_t id;                 // 11bit or 29bit identifier
    std::array<uint8_t, 8> data; // Data field
    uint8_t size;                // Length of data field in bytes
    CANFormat format;
    // CANType        type;
};

} // namespace stmbed

#ifdef HAL_CAN_MODULE_ENABLED

#include "main.h"
#include <functional>
#include <map>
#include <queue>

namespace stmbed {

class CAN {
public:
    using CallbackFnType = void(const CANMessage &msg);

    CAN(CAN_HandleTypeDef *handle) : handle_(handle) {
        CAN_FilterTypeDef filter;
        filter.FilterIdHigh = 0;
        filter.FilterIdLow = 0;
        filter.FilterMaskIdHigh = 0;
        filter.FilterMaskIdLow = 0;
        filter.FilterScale = CAN_FILTERSCALE_32BIT;
        filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
        filter.FilterBank = 0;

#if defined(CAN2)
        if (handle->Instance == CAN2)
            filter.FilterBank = 14;
#endif

        filter.FilterMode = CAN_FILTERMODE_IDMASK;
        filter.SlaveStartFilterBank = 14;
        filter.FilterActivation = ENABLE;
        if (HAL_CAN_ConfigFilter(handle_, &filter) != HAL_OK) {
            Error_Handler();
        }

        if (HAL_CAN_ActivateNotification(handle, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
            Error_Handler();
        }

        if (HAL_CAN_Start(handle_) != HAL_OK) {
            Error_Handler();
        }
    }

    HAL_StatusTypeDef write(const CANMessage &msg, bool wait_mailbox_free = true) {
        CAN_TxHeaderTypeDef TxHeader;
        if (msg.format == CANFormat::CANExtended) {
            TxHeader.ExtId = msg.id;
        } else {
            TxHeader.StdId = msg.id;
        }
        TxHeader.RTR = CAN_RTR_DATA;
        TxHeader.IDE = (msg.format == CANFormat::CANStandard) ? CAN_ID_STD : CAN_ID_EXT;
        TxHeader.DLC = msg.size;
        TxHeader.TransmitGlobalTime = DISABLE;
        uint32_t TxMailbox;

        while (wait_mailbox_free && HAL_CAN_GetTxMailboxesFreeLevel(handle_) == 0)
            ;
        return HAL_CAN_AddTxMessage(handle_, &TxHeader, (uint8_t *)msg.data.data(), &TxMailbox);
    }

    // int read(CANMessage &msg)
    // {
    // }

    // 受信割り込み
    void attach(std::function<CallbackFnType> &&fn, uint8_t priority = 100) {
        callback::attach(reinterpret_cast<intptr_t>(handle_), std::move(fn), priority);
    }

private:
    CAN_HandleTypeDef *handle_;
};

} // namespace stmbed

#endif // HAL_CAN_MODULE_ENABLED
