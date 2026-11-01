#ifndef LINE8DIGITAL_MATH_H
#define LINE8DIGITAL_MATH_H

#include <stdbool.h>
#include <stdint.h>

/* Bit 0 is OUT1 (far left). A high bit means black line. */
bool line8digital_position(uint8_t high_bits, int16_t *position, uint8_t *active_count);
/* Finish line is a brief run of at least four adjacent black sensors. */
bool line8digital_finish_marker(uint8_t high_bits);
/* Four adjacent sensors confirm immediately. When soft detection is enabled,
 * three adjacent sensors must persist for two consecutive control frames. */
bool line8digital_finish_marker_update(uint8_t high_bits,
                                       bool allow_three_adjacent,
                                       uint8_t *three_frame_count);

#endif
