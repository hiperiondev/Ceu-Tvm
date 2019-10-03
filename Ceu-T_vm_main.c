/*
 * Copyright (c) 2019 Emiliano Augusto Gonzalez (comercial@hiperion.com.ar)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * This code is based on project: http://afbranco.github.io/Terra/terra-home.html
 */

#include "Ceu-T_vm.h"

extern void vm_evt_error(uint8_t v1) {
    printf("evt_error %d\n", v1);
}

//
extern void vm_custom_reset(void) {
    printf("vm_custom_reset\n");
}

//
extern void vm_custom_call_function(uint8_t v) {
    printf("vm_custom_call_function %d\n", v);
}

//
extern void vm_custom_proc_outevt(uint8_t v1, uint32_t v2) {
    printf("vm_custom_proc_outevt %d, %d\n", v1, v2);
}

//
extern void vm_ceu_wclock_enable(uint16_t v1, int32_t v2, uint16_t v3) {
    printf("ceu_wclock_enable %d, %d, %d\n", v1, v2, v3);
}

//
extern void vm_ceu_trigger(uint16_t v1, uint16_t v2) {
    printf("ceu_trigger %d, %d\n", v1, v2);
}

//
extern void vm_ceu_track_ins(int8_t v1, int8_t v2, int8_t v3, uint16_t v4) {
    printf("ceu_track_ins %d, %d, %d, %d\n", v1, v2, v3, v4);
}

//
extern void vm_ceu_track_clr(uint16_t v1, uint16_t v2) {
    printf("ceu_track_clr %d, %d\n", v1, v2);
}

//
extern void vm_ceu_async_enable(uint16_t v1, uint16_t v2) {
    printf("ceu_async_enable %d, %d\n", v1, v2);
}

int main(){
    return 0;
}

