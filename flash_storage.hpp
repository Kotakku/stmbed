#pragma once

#include "main.h"
#include "stm32h5xx_hal_flash.h"
#include "stm32h5xx_hal_flash_ex.h"
#include "stm32h5xx_hal_icache.h"
#include <cstring>

namespace stmbed {

class FlashStorage {
public:
  static constexpr uint32_t SECTOR_SIZE = 0x4000; // 16KB per sector
  static constexpr uint32_t SECTORS_PER_BANK = 64;

  // sector: bank-relative sector number (0-63)
  // bank: FLASH_BANK_1 or FLASH_BANK_2
  FlashStorage(uint32_t sector, uint32_t bank)
      : sector_(sector), bank_(bank),
        base_addr_((bank == FLASH_BANK_1 ? 0x08000000UL : 0x08100000UL) +
                   sector * SECTOR_SIZE) {}

  bool erase() {
    HAL_ICACHE_Disable();

    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef erase_init{};
    erase_init.TypeErase = FLASH_TYPEERASE_SECTORS;
    erase_init.Banks = bank_;
    erase_init.Sector = sector_;
    erase_init.NbSectors = 1;

    uint32_t sector_error = 0;
    HAL_StatusTypeDef status = HAL_FLASHEx_Erase(&erase_init, &sector_error);
    HAL_FLASH_Lock();

    HAL_ICACHE_Invalidate();
    HAL_ICACHE_Enable();
    return status == HAL_OK;
  }

  bool write(uint32_t offset, const void *data, uint32_t size) {
    if (offset + size > SECTOR_SIZE) {
      return false;
    }

    HAL_ICACHE_Disable();
    HAL_FLASH_Unlock();

    // Quad-word buffer (16 bytes, 4-byte aligned for flash ECC)
    uint32_t quad_word[4];
    uint32_t addr = base_addr_ + offset;
    const uint8_t *src = static_cast<const uint8_t *>(data);

    uint32_t remaining = size;
    while (remaining > 0) {
      // Fill with 0xFF for padding
      quad_word[0] = quad_word[1] = quad_word[2] = quad_word[3] = 0xFFFFFFFFUL;
      uint32_t chunk = (remaining >= 16) ? 16 : remaining;
      std::memcpy(quad_word, src, chunk);

      HAL_StatusTypeDef status = HAL_FLASH_Program(
          FLASH_TYPEPROGRAM_QUADWORD, addr,
          reinterpret_cast<uint32_t>(quad_word));

      if (status != HAL_OK) {
        HAL_FLASH_Lock();
        HAL_ICACHE_Invalidate();
        HAL_ICACHE_Enable();
        return false;
      }

      addr += 16;
      src += chunk;
      remaining -= chunk;
    }

    HAL_FLASH_Lock();

    HAL_ICACHE_Invalidate();
    HAL_ICACHE_Enable();
    return true;
  }

  void read(uint32_t offset, void *data, uint32_t size) const {
    std::memcpy(data, reinterpret_cast<const void *>(base_addr_ + offset),
                size);
  }

  const void *address(uint32_t offset = 0) const {
    return reinterpret_cast<const void *>(base_addr_ + offset);
  }

  uint32_t base_addr() const { return base_addr_; }

private:
  uint32_t sector_;
  uint32_t bank_;
  uint32_t base_addr_;
};

} // namespace stmbed
