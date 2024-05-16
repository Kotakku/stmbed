#pragma once
#ifdef HAL_TIM_MODULE_ENABLED

#include "main.h"
#include <cmath>

namespace stmbed
{

class Encoder
{
public:
    Encoder(TIM_HandleTypeDef *handle, uint16_t ppr, bool is_reverse = false) :
        _handle(handle)
    {
        HAL_TIM_Encoder_Start(_handle, TIM_CHANNEL_ALL);
        set_ppr(ppr);
        set_dir(is_reverse);
    }

    void reset_pos() { set_pos(0.0); }

    void set_ppr(uint16_t ppr) { _ppr = ppr; }
    void set_dir(bool is_reverse) { _count_dir = (is_reverse == true) ? -1 : 1; }
    void set_pos(float pos)
    {
        uint16_t CPR = _ppr * 4; // (Count Per Revolusion)
        _pos_cnt = pos * CPR;

        _velocity = 0;
        _idx_cnt = 0;
        
        if(_pos_cnt < 0)
        {
            while(_pos_cnt < 0)
            {
                _pos_cnt += CPR;
                _idx_cnt -= 1;
            }
        }
        else
        {
            _idx_cnt += (_pos_cnt / CPR);
            _pos_cnt %= CPR;
        }
        _position = (float)_pos_cnt / CPR;
    }

    void update(float dt)
    {
        uint16_t CPR = _ppr * 4; // (Count Per Revolusion)
        int16_t deltaCount = _count_dir * get_reset_cnt();
        
        _now_dir = sign(deltaCount);
        
        if(deltaCount == 0)
            _now_dir = 0;
        
        // 速度計算
        _velocity = deltaCount / dt / CPR;
        
        // 位置計算
        _pos_cnt += deltaCount;
        if(_pos_cnt < 0)
        {
            _pos_cnt += CPR;
            _idx_cnt -= 1;
        }
        else
        {
            _idx_cnt += (_pos_cnt / CPR);
            _pos_cnt %= CPR;
        }
        _position = (float)_pos_cnt / CPR;
    }

    int direction() { return _now_dir; }
    int revolutions() { return _idx_cnt; }
    float position() { return _position; }
    float rotation() { return (revolutions() + position()); }
    float angular_vel() { return (_velocity * 2 * M_PI); }
    float rpm() { return (_velocity * 60); }
    float rps() { return _velocity; }

private:
    int16_t get_reset_cnt()
    {
        int16_t val = __HAL_TIM_GET_COUNTER(_handle);
        __HAL_TIM_SET_COUNTER(_handle,0);
        return val;
    }

    int sign(int16_t val)
    {
        if (val > 0)
            return 1;
        else if (val < 0)
            return -1;
        return 0;
    }

private:
    TIM_HandleTypeDef *_handle;
    uint16_t _ppr;

    int _count_dir;
    int16_t _pos_cnt;
    int16_t _idx_cnt;
    int _now_dir;

    float _velocity;
    float _position;
};
}

#endif // HAL_TIM_MODULE_ENABLED
