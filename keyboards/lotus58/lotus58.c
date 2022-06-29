/* Copyright 2021 TweetyDaBird
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "lotus58.h"

#ifdef RGB_MATRIX_ENABLE

#define LED_MATRIX_CENTER {110, 30}

led_config_t g_led_config = {
    {
        // Key Matrix to LED Index
        { 5,  4,  3,  2,  1,  0},
        { 6,  7,  8,  9, 10, 11},
        {17, 16, 15, 14, 13, 12},
        {18, 19, 20, 21, 22, 23},
    {NO_LED, 28, 27, 26, 25, 24},

                                        {34, 33, 32, 31, 30, 29},
                                        {35, 36, 37, 38, 39, 40},
                                        {46, 45, 44, 43, 42, 41},
                                        {47, 48, 49, 50, 51, 52},
                                {NO_LED, 57, 56, 55, 54, 53}
    }, {
        // LED Index to Physical Position
        // The default expected range of values for `{ x, y }` is the inclusive range `{ 0..224, 0..64 }`
        // 224 / (14 real cols + 6 cols as buffer) ~= 11 per col
        // 64 / (5 real rows + 1 row as buffer) ~= 10 per row
        // Remember to set #define LED_MATRIX_CENTER { xxx, yyy } properly above!!!
        { 55,  2}, { 44,  1}, { 33,  0}, { 22,  1}, { 11,  3}, {  0,  4},
        {  0, 14}, { 11, 13}, { 22, 11}, { 33, 10}, { 44, 11}, { 55, 12},
        { 55, 22}, { 44, 21}, { 33, 20}, { 22, 21}, { 11, 23}, {  0, 24},
        {  0, 34}, { 11, 33}, { 22, 31}, { 33, 30}, { 44, 31}, { 55, 32},
        { 70, 27}, { 66, 40}, { 50, 42}, { 39, 41} ,{ 28, 40},  // bottom row, right to left, far outer right key is first

        // TODO: right half!
        {122, 42}, {142, 42}, {162, 42}, {183, 42}, {203, 42}, {224, 42},
        {  0, 64}, { 20, 64}, { 40, 64}, { 61, 64}, { 81, 64}, {111, 64},
        {131, 64}, {142, 64}, {162, 64}, {183, 64}, {203, 64}, {224, 64},
        {244, 64}, {255, 64}, { 50, 86}, { 70, 86}, { 90, 86}, {110, 86},
                   {120, 86}, {140, 86}, {160, 86}, {180, 86}, {180, 86}
    }, {
        // LED Index to Flag
        4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4,

        4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4
    }
};

void suspend_power_down_kb(void) {
    rgb_matrix_set_suspend_state(true);
    suspend_power_down_user();
}

void suspend_wakeup_init_kb(void) {
    rgb_matrix_set_suspend_state(false);
    suspend_wakeup_init_user();
}
#endif
