/*
 * CeutTvm.h
 *
 *  Created on: 28 sep. 2019
 *      Author: egonzalez
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "CeuTvm_aux.h"

void Decoder(uint8_t Opcode, uint8_t Modifier) {
     uint8_t v1u8, v2u8, v3u8, v4u8, v5u8, v6u8, v7u8, v8u8, v9u9;
    uint16_t v1u16, v2u16, v3u16, v4u16;
    uint32_t v1u32, v2u32;
       float v1f, v2f;

    switch (Opcode) {
        case op_nop:
            break;
        case op_end:
            break;
        case op_bnot:
            v1u8 = pop();
            push(~v1u8);
            break;
        case op_lnot:
            v1u8 = pop();
            push(!v1u8);
            break;
        case op_neg:
            v1u8 = pop();
            push(-1 * v1u8);
            break;
        case op_sub:
            v1u8 = pop();
            v2u8 = pop();
            push(v1u8 - v2u8);
            break;
        case op_add:
            v1u8 = pop();
            v2u8 = pop();
            push(v1u8 + v2u8);
            break;
        case op_mod:
            v1u8 = pop();
            v2u8 = pop();
            push((v2u8 == 0) ? 0 : v1u8 % v2u8);
            if (v2u8 == 0)
                evtError(E_DIVZERO);
            break;
        case op_mult:
            v1u8 = pop();
            v2u8 = pop();
            push(v1u8 * v2u8);
            break;
        case op_div:
            v1u8 = pop();
            v2u8 = pop();
            push((v2u8 == 0) ? 0 : v1u8 / v2u8);
            if (v2u8 == 0)
                evtError(E_DIVZERO);
            break;
        case op_bor:
            v1u8 = pop();
            v2u8 = pop();
            push(v1u8 | v2u8);
            break;
        case op_band:
            v1u8 = pop();
            v2u8 = pop();
            push(v1u8 & v2u8);
            break;
        case op_lshft:
            v1u8 = pop();
            v2u8 = pop();
            push(v1u8 << v2u8);
            break;
        case op_rshft:
            v1u8 = pop();
            v2u8 = pop();
            push(v1u8 >> v2u8);
            break;
        case op_bxor:
            v1u8 = pop();
            v2u8 = pop();
            push(v1u8 ^ v2u8);
            break;
        case op_eq:
            v1u8 = pop();
            v2u8 = pop();
            push(v1u8 == v2u8);
            break;
        case op_neq:
            v1u8 = pop();
            v2u8 = pop();
            push(v1u8 != v2u8);
            break;
        case op_gte:
            v1u8 = pop();
            v2u8 = pop();
            push(v1u8 >= v2u8);
            break;
        case op_lte:
            v1u8 = pop();
            v2u8 = pop();
            push(v1u8 <= v2u8);
            break;
        case op_gt:
            v1u8 = pop();
            v2u8 = pop();
            push(v1u8 > v2u8);
            break;
        case op_lt:
            v1u8 = pop();
            v2u8 = pop();
            push(v1u8 < v2u8);
            break;
        case op_lor:
            v1u8 = pop();
            v2u8 = pop();
            push(v1u8 || v2u8);
            break;
        case op_land:
            v1u8 = pop();
            v2u8 = pop();
            push(v1u8 && v2u8);
            break;
        case op_popx:
            pop();
            break;
        case op_neg_f:
            v1f = popf();
            v1f = -1 * v1f;
            pushf(v1f);
            break;
        case op_sub_f:
            v1f = popf();
            v2f = popf();
            pushf(v1f - v2f);
            break;
        case op_add_f:
            v1f = popf();
            v2f = popf();
            pushf(v1f + v2f);
            break;
        case op_mult_f:
            v1f = popf();
            v2f = popf();
            pushf(v1f * v2f);
            break;
        case op_div_f:
            v1f = popf();
            v2f = popf();
            pushf(v1f / v2f);
            break;
        case op_eq_f:
            v1f = popf();
            v2f = popf();
            push(v1f == v2f);
            break;
        case op_neq_f:
            v1f = popf();
            v2f = popf();
            push(v1f != v2f);
            break;
        case op_gte_f:
            v1f = popf();
            v2f = popf();
            push(v1f >= v2f);
            break;
        case op_lte_f:
            v1f = popf();
            v2f = popf();
            push(v1f <= v2f);
            break;
        case op_gt_f:
            v1f = popf();
            v2f = popf();
            push(v1f > v2f);
            break;
        case op_lt_f:
            v1f = popf();
            v2f = popf();
            push(v1f < v2f);
            break;
        case op_func:
            v1u8 = getPar8(1);
            VMCustom_callFunction(v1u8);
            break;
        case op_outEvt_e:
            v1u32 = pop();
            v1u8  = getPar8(1);
            VMCustom_procOutEvt(v1u8, v1u32);
            break;
        case op_outevt_z:
            v3u8 = getPar8(1);
            VMCustom_procOutEvt(v3u8, 0);
            break;
        case op_clken_e:
            v1u32 = 0;
            Modifier = getPar8(1);
            v1u8  = getBitsPow(Modifier, 1, 1);
            v2u8  = getBitsPow(Modifier, 0, 0);
            v3u8  = getBits(Modifier, 4, 6);
            v1u16 = getPar16(v1u8);
            v3u16 = getPar16(v2u8);
            v1u32 = pop();
            ceu_wclock_enable(v1u16, (int32_t) unit2val(v1u32, v3u8), v3u16);
            break;
        case op_clken_v:
            v1u32 = 0;
            Modifier = getPar8(1);
            v4u8  = getBits(Modifier, 5, 7);
            v5u8  = getBits(Modifier, 3, 4); // Expect type with 2 bits -- only unsigned integer
            v1u8  = getBitsPow(Modifier, 2, 2);
            v2u8  = getBitsPow(Modifier, 1, 1);
            v3u8  = getBitsPow(Modifier, 0, 0);
            v1u16 = getPar16(v1u8);
            v2u16 = getPar16(v2u8);
            v3u16 = getPar16(v3u8);
            v1u32 = getMVal(v2u16, v5u8);
            ceu_wclock_enable(v1u16, (int32_t) unit2val(v1u32, v4u8), v3u16);
            break;
        case op_clken_c:
            Modifier = getPar8(1);
            v1u8  = getBitsPow(Modifier, 3, 3);
            v2u8  = (uint8_t) (getBits(Modifier, 1, 2) + 1);
            v3u8  = getBitsPow(Modifier, 0, 0);
            v1u16 = getPar16(v1u8);
            v1u32 = getPar32(v2u8);
            v3u16 = getPar16(v3u8);
            ceu_wclock_enable(v1u16, (int32_t) v1u32, v3u16);
            break;
        case op_set_v:
            Modifier = getPar8(1);
            v1u8  = getBitsPow(Modifier, 7, 7);
            v4u8  = getBits(Modifier, 4, 6);
            v2u8  = getBitsPow(Modifier, 3, 3);
            v5u8  = getBits(Modifier, 0, 2);
            v1u16 = getPar16(v1u8);
            v2u16 = getPar16(v2u8);

            if (v5u8 == F32) {         // Source is a float
                v1f = getMValf(v2u16);
                //setMVal(*(uint32_t*) &v1f, v1u16, v5u8, v4u8);
                memcpy(&v1u32, &v1f, sizeof(float));
                setMVal(v1u32, v1u16, v5u8, v4u8);
            } else {                 // Source is an integer
                v1u32 = getMVal(v2u16, v5u8);
                setMVal(v1u32, v1u16, v5u8, v4u8);
            }
            break;
        case op_setarr_vc:
            Modifier = getPar8(1);
            v8u8  = getPar8(1);

            v1u8  = getBitsPow(Modifier, 7, 7);
            v4u8  = getBits(Modifier, 4, 6);
            v2u8  = getBitsPow(Modifier, 3, 3);
            v5u8  = getBits(Modifier, 0, 2);
            v3u8  = getBitsPow(v8u8, 2, 2);
            v7u8  = (uint8_t) (getBits(v8u8, 0, 1) + 1);
            v6u8  = (v4u8 == F32) ? 4 : 1 << (v4u8 & 0x3);

            v1u16 = getPar16(v1u8);
            v3u16 = getPar16(v2u8);
            v4u16 = getPar16(v3u8);
            v1u32 = getPar32(v7u8);

            if (getMVal(v3u16, v5u8) >= v4u16)
                evtError(E_IDXOVF);
            else {
                if (v4u8 == F32) {
                    v1f = (float) v1u32;
                    //setMVal(*(uint32_t*) &v1f, v1u16 + ((getMVal(v3u16, v5u8) % v4u16) * v6u8), v5u8, v4u8);
                    memcpy(&v1u32, &v1f, sizeof(float));
                    setMVal(v1u32, v1u16 + ((getMVal(v3u16, v5u8) % v4u16) * v6u8), v5u8, v4u8);
                } else {
                    setMVal(v1u32, v1u16 + ((getMVal(v3u16, v5u8) % v4u16) * v6u8), v5u8, v4u8);
                }
            }
            break;
        case op_setarr_vv:
            Modifier = getPar8(1);
            v8u8  = getPar8(1);

            v1u8  = getBitsPow(Modifier, 7, 7);
            v4u8  = getBits(Modifier, 4, 6);
            v2u8  = getBitsPow(Modifier, 3, 3);
            v5u8  = getBits(Modifier, 0, 2);

            v3u8  = getBitsPow(v8u8, 4, 4);
            v7u8  = getBitsPow(v8u8, 3, 3);
            v9u9  = getBits(v8u8, 0, 2);

            v6u8  = (v4u8 == F32) ? 4 : 1 << (v4u8 & 0x3);

            v1u16 = getPar16(v1u8);
            v3u16 = getPar16(v2u8);
            v4u16 = getPar16(v3u8);
            v2u16 = getPar16(v7u8);

            if (getMVal(v3u16, v5u8) >= v4u16)
                evtError(E_IDXOVF);
            else {
                if (v9u9 == F32) {         // Source is a float
                    v1f = getMValf(v2u16);
                    //setMVal(*(uint32_t*) &v1f, v1u16 + ((getMVal(v3u16, v5u8) % v4u16) * v6u8), v9u9, v4u8);
                    memcpy(&v1u32, &v1f, sizeof(float));
                    setMVal(v1u32, v1u16 + ((getMVal(v3u16, v5u8) % v4u16) * v6u8), v9u9, v4u8);
                } else {                 // Source is an integer
                    v1u32 = getMVal(v2u16, v9u9);
                    setMVal(v1u32, v1u16 + ((getMVal(v3u16, v5u8) % v4u16) * v6u8), v9u9, v4u8);
                }
            }
            break;
        case op_poparr_v:
            v3u8  = getBitsPow(Modifier, 0, 0);
            v8u8  = getPar8(1);
            v1u8  = getBitsPow(v8u8, 7, 7);
            v4u8  = getBits(v8u8, 4, 6);
            v2u8  = getBitsPow(v8u8, 3, 3);
            v5u8  = getBits(v8u8, 0, 2);

            v6u8  = (v4u8 == F32) ? 4 : 1 << (v4u8 & 0x3);

            v1u16 = getPar16(v1u8);
            v3u16 = getPar16(v2u8);
            v4u16 = getPar16(v3u8);
            if (getMVal(v3u16, v5u8) >= v4u16)
                evtError(E_IDXOVF);
            else {
                if (v4u8 == F32) { // Source/Target are float
                    v1f = popf();
                    //setMVal(*(uint32_t*) &v1f, v1u16 + ((getMVal(v3u16, v5u8) % v4u16) * v6u8), F32, v4u8);
                    memcpy(&v1u32, &v1f, sizeof(float));
                    setMVal(v1u32, v1u16 + ((getMVal(v3u16, v5u8) % v4u16) * v6u8), F32, v4u8);
                } else { // Source/Target are integer
                    v1u32 = pop();
                    setMVal(v1u32, v1u16 + ((getMVal(v3u16, v5u8) % v4u16) * v6u8), S32, v4u8);
                }
            }
            break;
        case op_pusharr_v:
            v3u8  = getBitsPow(Modifier, 0, 0);
            v8u8  = getPar8(1);
            v1u8  = getBitsPow(v8u8, 7, 7);
            v4u8  = getBits(v8u8, 4, 6);
            v2u8  = getBitsPow(v8u8, 3, 3);
            v5u8  = getBits(v8u8, 0, 2);

            v1u8  = (v4u8 == F32) ? 4 : 1 << (v4u8 & 0x3);
            v2u8  = (v5u8 == F32) ? 4 : 1 << (v5u8 & 0x3);

            v1u16 = getPar16(v1u8);
            v3u16 = getPar16(v2u8);
            v4u16 = getPar16(v3u8);

            if (getMVal(v3u16, v5u8) >= v4u16)
                evtError(E_IDXOVF);
            else {
                push(v1u16 + ((getMVal(v3u16, v5u8) % v4u16) * v1u8));
            }
            break;
        case op_getextdt_e:
            v1u8  = getBitsPow(Modifier, 0, 0);
            v1u16 = (uint16_t) pop();
            v2u16 = getPar16(v1u8);
            memcpy((MEM + v1u16), CEU->ext_data, v2u16);
            break;
        case op_trg:
            v1u8  = getBitsPow(Modifier, 0, 0);
            v1u16 = getPar16(v1u8);
            ceu_trigger(v1u16, 0);
            break;
        case op_exec:
            v1u8  = getBitsPow(Modifier, 0, 0);
            v1u16 = getPar16(v1u8);
            PC = v1u16;
            break;
        case op_chkret:
            v1u8  = getBitsPow(Modifier, 0, 0);
            v1u16 = getPar16(v1u8);
            if (*(uint8_t*) (MEM + v1u16) > 0)
                PC = PC + 1;
            break;
        case op_tkins_z:
            v2u8  = getBitsPow(Modifier, 0, 0);
            v4u8  = getPar8(1);
            v3u8  = getBits(v4u8, 7, 7);
            v1u8  = getBits(v4u8, 0, 6);
            v3u16 = getPar16(v2u8);
            ceu_track_ins(0, v1u8, v3u8, v3u16);
            break;
        case op_push_c:
            v1u8  = (uint8_t) (getBits(Modifier, 0, 1) + 1);
            v1u32 = getPar32(v1u8);
            push(v1u32);
            break;
        case op_cast:
            v1u8 = getBits(Modifier, 0, 1);
            switch (v1u8) {
                case U32_F:
                    v1u32 = pop();
                    pushf((float) *(uint32_t*) &v1u32);
                    break;
                case S32_F:
                    v1u32 = pop();
                    pushf((float) *(int32_t*) &v1u32);
                    break;
                case F_U32:
                    v1f = popf();
                    push((uint32_t) *(float*) &v1f);
                    break;
                case F_S32:
                    v1f = popf();
                    push((uint32_t) *(float*) &v1f);
                    break;
            }
            break;
        case op_memclr:
            v1u8  = getBitsPow(Modifier, 1, 1);
            v2u8  = getBitsPow(Modifier, 0, 0);
            v1u16 = getPar16(v1u8);
            v2u16 = getPar16(v2u8);
            memset((MEM+v1u16),0,v2u16); // does not work in TOSSIM
            //{
            //    int x;
            //    for (x = 0; x < v2u16; x++)
            //        *(uint8_t*) (MEM + v1u16 + x) = 0;
            //}
            break;
        case op_ifelse:
            v1u8  = getBitsPow(Modifier, 1, 1);
            v2u8  = getBitsPow(Modifier, 0, 0);
            v1u16 = getPar16(v1u8);
            v2u16 = getPar16(v2u8);
            if (pop())
                PC = v1u16;
            else
                PC = v2u16;
            break;
        case op_asen:
            v1u8  = getBitsPow(Modifier, 1, 1);
            v2u8  = getBitsPow(Modifier, 0, 0);
            v1u16 = getPar16(v1u8);
            v3u16 = getPar16(v2u8);
            ceu_async_enable(v1u16, v3u16);
            break;
        case op_tkclr:
            v1u8  = getBitsPow(Modifier, 1, 1);
            v2u8  = getBitsPow(Modifier, 0, 0);
            v1u16 = getPar16(v1u8);
            v2u16 = getPar16(v2u8);
            ceu_track_clr(v1u16, v2u16);
            break;
        case op_outEvt_c:
            v1u8  = (uint8_t) (getBits(Modifier, 0, 1) + 1);
            v3u8  = getPar8(1);
            v1u32 = getPar32(v1u8);
            VMCustom_procOutEvt(v3u8, v1u32);
            break;
        case op_getextdt_v:
            v1u8  = getBitsPow(Modifier, 1, 1);
            v2u8  = getBitsPow(Modifier, 0, 0);
            v1u16 = getPar16(v1u8);
            v2u16 = getPar16(v2u8);
            memcpy((MEM + v1u16), CEU->ext_data, v2u16);
            break;
        case op_inc:
            v4u8  = getBits(Modifier, 0, 1);
            v6u8  = 1 << v4u8;
            v1u16 = (uint16_t) pop();
            setMVal((getMVal(v1u16, v4u8) + 1), v1u16, v4u8, v4u8);
            break;
        case op_dec:
            v4u8  = getBits(Modifier, 0, 1);
            v6u8  = 1 << v4u8;
            v1u16 = (uint16_t) pop();
            setMVal((getMVal(v1u16, v4u8) - 1), v1u16, v4u8, v4u8);
            break;
        case op_set_e:
            v4u8  = getBits(Modifier, 0, 2);
            v6u8  = (v4u8 == F32) ? 4 : 1 << (v4u8 & 0x3);
            v1u16 = (uint16_t) pop();
            if (v4u8 == F32) {
                v1f = popf();
                //setMVal(*(uint32_t*) &v1f, v1u16, F32, v4u8);
                memcpy(&v1u32, &v1f, sizeof(float));
                setMVal(v1u32, v1u16, F32, v4u8);
            } else {
                v1u32 = pop();
                setMVal(v1u32, v1u16, S32, v4u8);
            }
            break;
        case op_deref:
            v1u8  = getBits(Modifier, 0, 2);
            v1u16 = (uint16_t) pop();
            switch (v1u8) {
                case U8:
                    push((uint8_t) getMVal(v1u16, v1u8));
                    break;
                case U16:
                    push((uint16_t) getMVal(v1u16, v1u8));
                    break;
                case U32:
                    push((uint32_t) getMVal(v1u16, v1u8));
                    break;
                case F32:
                    pushf(getMValf(v1u16));
                    break;
                case S8:
                    push((int8_t) getMVal(v1u16, v1u8));
                    break;
                case S16:
                    push((int16_t) getMVal(v1u16, v1u8));
                    break;
                case S32:
                    push((int32_t) getMVal(v1u16, v1u8));
                    break;
            }
            break;
        case op_memcpy:
            v1u8  = getBitsPow(Modifier, 2, 2);
            v2u8  = getBitsPow(Modifier, 1, 1);
            v3u8  = getBitsPow(Modifier, 0, 0);
            v2u16 = getPar16(v1u8);
            v1u16 = getPar16(v2u8);
            v3u16 = getPar16(v3u8);
            memcpy((void*) (MEM + v3u16), (void*) (MEM + v1u16), v2u16);
            break;
        case op_tkins_max:
            v2u8  = (uint8_t) (CEU->stack + getBits(Modifier, 1, 2));
            v1u8  = getBitsPow(Modifier, 0, 0);
            v3u16 = getPar16(v1u8);
            ceu_track_ins(v2u8, 255, 0, v3u16);
            break;
        case op_push_v:
            v1u8  = getBitsPow(Modifier, 3, 3);
            v4u8  = getBits(Modifier, 0, 2);
            v1u16 = getPar16(v1u8);
            if (v4u8 == F32) {
                pushf(getMValf(v1u16));
            } else {
                push(getMVal(v1u16, v4u8));
            }
            break;
        case op_pop:
            v1u8  = getBitsPow(Modifier, 3, 3);
            v4u8  = getBits(Modifier, 0, 2);
            v1u16 = getPar16(v1u8);
            if (v4u8 == F32) {
                v1f = popf();
                //setMVal(*(uint32_t*) &v1f, v1u16, F32, v4u8);
                memcpy(&v1u32, &v1f, sizeof(float));
                setMVal(v1u32, v1u16, F32, v4u8);
            } else {
                v1u32 = pop();
                setMVal(v1u32, v1u16, S32, v4u8);
            }
            break;
        case op_outEvt_v:
            v2u8  = getBitsPow(Modifier, 3, 3);
            v3u8  = getPar8(1);
            v1u16 = getPar16(v2u8);
            VMCustom_procOutEvt(v3u8, v1u16);
            break;
        case op_set_c:
            v4u8  = getBits(Modifier, 0, 2);
            v6u8  = (v4u8 == F32) ? 4 : 1 << (v4u8 & 0x3);
            v1u8  = getBitsPow(Modifier, 3, 3);
            v2u8  = (uint8_t) (getBits(Modifier, 4, 5) + 1);
            v1u16 = getPar16(v1u8);
            v1u32 = getPar32(v2u8);
            if (v4u8 == F32) {
                //v1f = *(float*) &v1u32;
                //setMVal(*(uint32_t*) &v1f, v1u16, F32, v4u8);
                memcpy(&v1f, &v1u32, sizeof(uint32_t)); //TODO: revisar, doble conversion
                memcpy(&v2u32, &v1f, sizeof(float));
                setMVal(v2u32, v1u16, F32, v4u8);

            } else {
                setMVal(v1u32, v1u16, S32, v4u8);
            }
            break;
    }
}
