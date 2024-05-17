#pragma once
#include "main.h"

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
    CANFormat format = CANStandard;
    // CANType        type;
};

} // namespace stmbed

#if defined(HAL_CAN_MODULE_ENABLED) || defined(HAL_FDCAN_MODULE_ENABLED)

#include "main.h"
#include <functional>
#include <map>
#include <queue>

namespace stmbed {

class CAN {
public:
#if defined(HAL_CAN_MODULE_ENABLED)
    using CanHandleType = CAN_HandleTypeDef;
#elif defined(HAL_FDCAN_MODULE_ENABLED)
    using CanHandleType = FDCAN_HandleTypeDef;
#endif

    using CallbackFnType = void(const CANMessage &msg);

    CAN(CanHandleType *handle) : CAN(handle, 0, 0) {}
    HAL_StatusTypeDef write(CANMessage &msg, bool wait_mailbox_free = true) {
        return write(msg.id, msg.data.data(), msg.size, msg.format == CANFormat::CANExtended, wait_mailbox_free);
    }

    // int read(CANMessage &msg);

    // 受信割り込み
    void attach(std::function<CallbackFnType> &&fn, uint8_t priority = 100) {
        callback::attach(reinterpret_cast<intptr_t>(handle_), std::move(fn), priority);
    }

#if defined(HAL_CAN_MODULE_ENABLED)
    CAN(CAN_HandleTypeDef *handle, uint32_t filter_id, uint32_t filter_mask) : handle_(handle) {
        CAN_FilterTypeDef filter;
        filter.FilterIdHigh = filter_id << 5;
        filter.FilterIdLow = filter_id << 21;
        filter.FilterMaskIdHigh = filter_mask << 5;
        filter.FilterMaskIdLow = filter_mask << 21;
        filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
        filter.FilterBank = 0;

#if defined(CAN2)
        if (handle->Instance == CAN2)
            filter.FilterBank = 14;
#endif

        filter.FilterScale = CAN_FILTERSCALE_32BIT;
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

    HAL_StatusTypeDef write(uint32_t id, uint8_t *data, uint32_t size, bool is_extended_frame, bool wait_mailbox_free) {
        CAN_TxHeaderTypeDef TxHeader;
        if (is_extended_frame) {
            TxHeader.ExtId = id;
        } else {
            TxHeader.StdId = id;
        }
        TxHeader.RTR = CAN_RTR_DATA;
        TxHeader.IDE = is_extended_frame ? CAN_ID_EXT : CAN_ID_STD;
        TxHeader.DLC = size;
        TxHeader.TransmitGlobalTime = DISABLE;
        uint32_t TxMailbox;

        while (wait_mailbox_free && HAL_CAN_GetTxMailboxesFreeLevel(handle_) == 0)
            ;
        return HAL_CAN_AddTxMessage(handle_, &TxHeader, data, &TxMailbox);
    }
#endif

#if defined(HAL_FDCAN_MODULE_ENABLED)
    CAN(FDCAN_HandleTypeDef *handle, uint32_t filter_id, uint32_t filter_mask) : handle_(handle) {
        if (handle_->State == HAL_FDCAN_STATE_READY) {
            FDCAN_FilterTypeDef filter;
            filter.IdType = FDCAN_STANDARD_ID;
            filter.FilterIndex = 0;
            filter.FilterType = FDCAN_FILTER_MASK;
            filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
            filter.FilterID1 = filter_id;
            filter.FilterID2 = filter_mask;

            if (HAL_FDCAN_ConfigFilter(handle_, &filter) != HAL_OK) {
                Error_Handler();
            }

            if (HAL_FDCAN_ConfigGlobalFilter(handle_, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE,
                                             FDCAN_REJECT_REMOTE) != HAL_OK) {
                Error_Handler();
            }

            if (HAL_FDCAN_ActivateNotification(handle_, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
                Error_Handler();
            }

            if (HAL_FDCAN_Start(handle_) != HAL_OK) {
                Error_Handler();
            }
        } else {
            Error_Handler();
        }
    }

    HAL_StatusTypeDef write(uint32_t id, uint8_t *data, uint32_t size, bool is_extended_frame, bool wait_mailbox_free) {
        FDCAN_TxHeaderTypeDef tx_header;
        tx_header.Identifier = id;
        tx_header.IdType = is_extended_frame ? FDCAN_EXTENDED_ID : FDCAN_STANDARD_ID;
        tx_header.TxFrameType = FDCAN_DATA_FRAME;
        tx_header.DataLength = (size > 8 ? 8 : size);
        tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
        tx_header.BitRateSwitch = FDCAN_BRS_OFF;
        tx_header.FDFormat = FDCAN_CLASSIC_CAN;
        tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
        tx_header.MessageMarker = 0;

        while (wait_mailbox_free && HAL_FDCAN_GetTxFifoFreeLevel(handle_) == 0)
            ;

        return HAL_FDCAN_AddMessageToTxFifoQ(handle_, &tx_header, data);
    }
#endif

private:
    CanHandleType *handle_;
};

} // namespace stmbed

#endif // HAL_CAN_MODULE_ENABLED
