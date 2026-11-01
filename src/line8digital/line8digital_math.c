/*
 * Selected module from the MSPM0 H-problem project.
 * Copyright (c) 2026 mamorrozer. All rights reserved.
 * See the repository LICENSE for reuse terms.
 */

#include "line8digital_math.h"

bool line8digital_position(uint8_t high_bits, int16_t *position, uint8_t *active_count)
{
    static const int16_t weights[8] = {
        -3500, -2500, -1500, -500, 500, 1500, 2500, 3500
    };
    int32_t sum = 0;
    uint8_t count = 0U;

    if ((position == 0) || (active_count == 0)) {
        return false;
    }

    for (uint8_t i = 0U; i < 8U; i++) {
        if ((high_bits & (uint8_t) (1U << i)) != 0U) {
            sum += weights[i];
            count++;
        }
    }

    *active_count = count;
    if (count == 0U) {
        *position = 0;
        return false;
    }

    *position = (int16_t) (sum / count);
    return true;
}

bool line8digital_finish_marker(uint8_t high_bits)
{
    uint8_t adjacent = 0U;

    for (uint8_t i = 0U; i < 8U; i++) {
        if ((high_bits & (uint8_t) (1U << i)) != 0U) {
            adjacent++;
            if (adjacent >= 4U) {
                return true;
            }
        } else {
            adjacent = 0U;
        }
    }
    return false;
}

bool line8digital_finish_marker_update(uint8_t high_bits,
                                       bool allow_three_adjacent,
                                       uint8_t *three_frame_count)
{
    uint8_t adjacent = 0U;
    uint8_t max_adjacent = 0U;

    if (three_frame_count == 0) {
        return false;
    }
    if (line8digital_finish_marker(high_bits)) {
        *three_frame_count = 0U;
        return true;
    }

    for (uint8_t i = 0U; i < 8U; i++) {
        if ((high_bits & (uint8_t) (1U << i)) != 0U) {
            adjacent++;
            if (adjacent > max_adjacent) {
                max_adjacent = adjacent;
            }
        } else {
            adjacent = 0U;
        }
    }

    if (allow_three_adjacent && (max_adjacent >= 3U)) {
        if (*three_frame_count < 2U) {
            (*three_frame_count)++;
        }
    } else {
        *three_frame_count = 0U;
    }
    return *three_frame_count >= 2U;
}
