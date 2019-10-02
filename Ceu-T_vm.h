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
#include <stdio.h>

typedef struct {
     int32_t togo; //
    uint16_t lbl;  //
} tceu_wclock;

typedef struct {
     uint8_t stack; //
     uint8_t tree;  //
    uint16_t lbl;   //
} tceu_trk;

typedef struct {
             int tracks_n;  //
         uint8_t stack;     //
           void* ext_data;  //
             int ext_int;   //
             int wclk_late; //
    tceu_wclock* wclk_cur;  //
             int async_cur; //
       tceu_trk* p_tracks;  // 0 is reserved
        uint8_t* p_mem;     //
} tceu;

#define CEU (&CEU_)
#define CEU_STACK_MIN 0x01 // min prio for `stack´
tceu CEU_ = {
        0,                 // tracks_n
        CEU_STACK_MIN,     // stack
        (void*) 0,         // ext_data
        0,                 // ext_int
        0,                 // wclk_late
        (void*) 0,         // wclk_cur
        0,                 // async_cur
        (void*) 0,         // p_tracks
        (void*) 0          // p_mem
        };

// VM input event structure
typedef struct evtData {
    uint8_t evtId; //
    uint8_t auxId; //
      void* data;  //
} evtData_t;

#define EVTQ_SIZE 10 //SHORT_QUEUES = 6
enum {
    EVT_QUEUE_SIZE = EVTQ_SIZE, //

    // Ctl section pointers
    DEFAULT_VARS   = 0,  //
    STRUC_DEF      = 2,  //
    VAR_SPACE      = 4,  //
    EVT_VARS       = 6,  //
    TMR_DEFS       = 8,  //
    GR_DEFS        = 10, //
    AGG_DEFS       = 12, //
    EVT_SPACE      = 14, //
    FUNCTION_SPACE = 16, //
    INIT_FUNCTION  = 18, //
    MOTE_ID        = 20, //
    RESULT         = 22, //

    // Var Types
    U8  = 0, //
    U16 = 1, //
    U32 = 2, //
    F32 = 3, //
    S8  = 4, //
    S16 = 5, //
    S32 = 6, //

    // Generic var type
    x8  = 0, //
    x16 = 1, //
    x32 = 2, //

    // Cast modes
    U32_F = 0, //
    S32_F = 1, //
    F_U32 = 2, //
    F_S32 = 3, //

    // OpCodes uint8
    op_nop        = 0,   // not operation
    op_end        = 1,   // ? not operation
    op_bnot       = 2,   // binary not
    op_lnot       = 3,   // logical not
    op_neg        = 4,   // negate
    op_sub        = 5,   // subtract
    op_add        = 6,   // add
    op_mod        = 7,   // module
    op_mult       = 8,   // multiplication
    op_div        = 9,   // division
    op_bor        = 10,  // binary or
    op_band       = 11,  // binary and
    op_lshft      = 12,  // left shift
    op_rshft      = 13,  // right shift
    op_bxor       = 14,  // binary xor
    op_eq         = 15,  // equal
    op_neq        = 16,  // no equal
    op_gte        = 17,  // greater or equal
    op_lte        = 18,  // less or equal
    op_gt         = 19,  // greater
    op_lt         = 20,  // less
    op_lor        = 21,  // logical or
    op_land       = 22,  // logical and

    op_popx       = 23,  // pop from stack (uint32)

    // OpCodes float
    op_neg_f      = 25,  // negate
    op_sub_f      = 26,  // subtract
    op_add_f      = 27,  // add
    op_mult_f     = 28,  // multiplication
    op_div_f      = 29,  // division
    op_eq_f       = 30,  // equal
    op_neq_f      = 31,  // no equal
    op_gte_f      = 32,  // greater or equal
    op_lte_f      = 33,  // less or equal
    op_gt_f       = 34,  // greater
    op_lt_f       = 35,  // less


    op_func       = 36,  // call function
    op_outEvt_e   = 37,  // out event e
    op_outevt_z   = 38,  // out event z
    op_clken_e    = 39,  // ?
    op_clken_v    = 40,  // ?
    op_clken_c    = 41,  // ?
    op_set_v      = 42,  // set value in memory (float or uint32)
    op_setarr_vc  = 43,  // ? set array in memory (float or uint32)
    op_setarr_vv  = 44,  // ? set array in memory (float or uint32)

    op_poparr_v   = 48,  // ?
    op_pusharr_v  = 50,  // ?
    op_getextdt_e = 52,  // get external data
    op_trg        = 54,  // trigger
    op_exec       = 56,  // execute (jump)
    op_chkret     = 58,  // jmp MEM + 1 if MEM + v1u16 > 0
    op_tkins_z    = 60,  // ? track insert

    op_push_c     = 64,  // push to stack (uint32)
    op_cast       = 68,  // cast top of stack
    op_memclr     = 72,  // set mem to 0
    op_ifelse     = 76,  // if else on top stack
    op_asen       = 80,  // ? async enable
    op_tkclr      = 84,  // ? track clear
    op_outEvt_c   = 88,  // out event
    op_getextdt_v = 92,  // get from external memory
    op_inc        = 96,  // increment memory
    op_dec        = 100, // decrement memory
    op_set_e      = 104, // set bin on memory
    op_deref      = 112, // push to stack memory value
    op_memcpy     = 120, // memory copy

    op_tkins_max  = 136, // ?track insert
    op_push_v     = 144, // push to stack
    op_pop        = 160, // pop from stack
    op_outEvt_v   = 176, // out event
    op_set_c      = 192, // set value in memory

};

// Ceu Environment vars
typedef struct progEnv {
    uint16_t Version;     //
    uint16_t ProgStart;   //
    uint16_t ProgEnd;     //
    uint16_t nTracks;     //
    uint16_t wClocks;     //
    uint16_t asyncs;      //
    uint16_t wClock0;     //
    uint16_t gate0;       //
    uint16_t inEvts;      //
    uint16_t async0;      //
    uint16_t appSize;     //
     uint8_t persistFlag; //
} progEnv_t;

// Event ID only for System Error (from VMCustom*.h)
enum {
    I_ERROR_ID = 0,  //
    I_ERROR    = 1,  //

// VM Error codes
    E_DIVZERO  = 10, // Division by zero
    E_IDXOVF   = 11, // Array index overflow
    E_STKOVF   = 20, // Stack overflow
    E_NOSETUP  = 21, // Missing operation setup
};

////////////////

#define STACK_SIZE 4840 // BLOCK_SIZE=22 * CURRENT_MAX_BLOCKS=220

// Ceu Environment vars
     bool haltedFlag = true;            // vm halted flag
     bool procFlag = false;             // vm processing flag
  uint8_t *MEM;                         // internal memory
  uint8_t ExtDataSysError;              // last system error code
  uint8_t CEU_data[STACK_SIZE];         // Ceu data room for 'tracks', 'mem', 'Prog', and 'Stack'
 uint16_t PC;                           // program counter
 uint16_t currStack = (STACK_SIZE) - 5; // stack control
progEnv_t envData; //

/////////////////////////////////////////
void evtError(uint8_t v1);
void VMCustom_reset(void);
void VMCustom_callFunction(uint8_t v);
void VMCustom_procOutEvt(uint8_t v1, uint32_t v2);
void ceu_wclock_enable(uint16_t v1, int32_t v2, uint16_t v3);
void ceu_trigger(uint16_t v1, uint16_t v2);
void ceu_track_ins(int8_t v1, int8_t v2, int8_t v3, uint16_t v4);
void ceu_track_clr(uint16_t v1, uint16_t v2);
void ceu_async_enable(uint16_t v1, uint16_t v2);
void push(uint32_t value);
void pushf(float value);
void setMVal(uint32_t buffer, uint16_t Maddr, uint8_t fromTp, uint8_t toTp);

uint8_t getPar8(uint8_t p_len);
uint8_t getBits(uint8_t data, uint8_t stBit, uint8_t endBit);
uint8_t getBitsPow(uint8_t data, uint8_t stBit, uint8_t endBit);

uint16_t getPar16(uint8_t p_len);

uint32_t unit2val(uint32_t val, uint8_t unit);
uint32_t pop();
uint32_t getMVal(uint16_t Maddr, uint8_t type);
uint32_t getPar32(uint8_t p_len);

float popf();
float getMValf(uint16_t Maddr);
/////////////////////////////////////////

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


///////////////////////// aux ///////////////////////////////////////

void evtError(uint8_t v1) {
    printf("evtError %d\n", v1);
}

//
void VMCustom_reset(void) {
    printf("VMCustom_reset\n");
}

//
void VMCustom_callFunction(uint8_t v) {
    printf("VMCustom_callFunction %d\n", v);
}

//
void VMCustom_procOutEvt(uint8_t v1, uint32_t v2) {
    printf("VMCustom_procOutEvt %d, %d\n", v1, v2);
}

//
void ceu_wclock_enable(uint16_t v1, int32_t v2, uint16_t v3) {
    printf("ceu_wclock_enable %d, %d, %d\n", v1, v2, v3);
}

//
void ceu_trigger(uint16_t v1, uint16_t v2) {
    printf("ceu_trigger %d, %d\n", v1, v2);
}

//
void ceu_track_ins(int8_t v1, int8_t v2, int8_t v3, uint16_t v4) {
    printf("ceu_track_ins %d, %d, %d, %d\n", v1, v2, v3, v4);
}

//
void ceu_track_clr(uint16_t v1, uint16_t v2) {
    printf("ceu_track_clr %d, %d\n", v1, v2);
}

//
void ceu_async_enable(uint16_t v1, uint16_t v2) {
    printf("ceu_async_enable %d, %d\n", v1, v2);
}

/////////////////////////////////////////

// Get constant values from program memory. Convert big-endian to the local type endian.
uint8_t getPar8(uint8_t p_len) {
    uint8_t temp = (uint8_t) CEU_data[PC];
    PC++;
    return temp;
}

uint16_t getPar16(uint8_t p_len) {
    uint16_t temp = 0L;
    switch (p_len) {
        case 1:
            temp = ((uint16_t) ((uint8_t) CEU_data[PC]) << 0);
            PC++;
            break;
        case 2:
            temp = ((uint16_t) ((uint8_t) CEU_data[PC]) << 8);
            PC++;
            temp = temp + ((uint16_t) ((uint8_t) CEU_data[PC]) << 0);
            PC++;
            break;
    }
    return temp;
}

uint32_t getPar32(uint8_t p_len) {
    uint32_t temp = 0L;
    switch (p_len) {
        case 1:
            temp = ((uint32_t) ((uint8_t) CEU_data[PC]) << 0);
            PC++;
            break;
        case 2:
            temp = ((uint32_t) ((uint8_t) CEU_data[PC]) << 8);
            PC++;
            temp = temp + ((uint32_t) ((uint8_t) CEU_data[PC]) << 0);
            PC++;
            break;
        case 3:
            temp = ((uint32_t) ((uint8_t) CEU_data[PC]) << 16);
            PC++;
            temp = temp + ((uint32_t) ((uint8_t) CEU_data[PC]) << 8);
            PC++;
            temp = temp + ((uint32_t) ((uint8_t) CEU_data[PC]) << 0);
            PC++;
            break;
        case 4:
            temp = ((uint32_t) ((uint8_t) CEU_data[PC]) << 24);
            PC++;
            temp = temp + ((uint32_t) ((uint8_t) CEU_data[PC]) << 16);
            PC++;
            temp = temp + ((uint32_t) ((uint8_t) CEU_data[PC]) << 8);
            PC++;
            temp = temp + ((uint32_t) ((uint8_t) CEU_data[PC]) << 0);
            PC++;
            break;
    }
    return temp;
}

uint8_t getBits(uint8_t data, uint8_t stBit, uint8_t endBit) {
    uint8_t ret = 0;
    ret = (data << (7 - endBit));
    ret = ret >> (7 - endBit + stBit);
    return ret;
}

uint8_t getBitsPow(uint8_t data, uint8_t stBit, uint8_t endBit) {
    return (1 << getBits(data, stBit, endBit));
}

uint32_t unit2val(uint32_t val, uint8_t unit) {
    switch (unit) { // result in 'ms'
        case 0:
            return (uint32_t) (val); // ms
        case 1:
            return (uint32_t) (val * 1000L); // seg
        case 2:
            return (uint32_t) (val * 60L * 1000L); // min
        case 3:
            return (uint32_t) (val * 60L * 60L * 1000L); // h
    }
    return val;
}

void push(uint32_t value) {
    currStack = currStack - 4;
    if ((currStack) > envData.ProgEnd)
        *(uint32_t*) (CEU_data + currStack) = value;
    else {
        evtError(E_STKOVF);
        // stop VM execution to prEVENT unexpected state
        haltedFlag = true;
        VMCustom_reset();
    }
}

void pushf(float value) {
    currStack = currStack - 4;
    if ((currStack) > envData.ProgEnd) {
        *(float*) (CEU_data + currStack) = value;
    } else {
        evtError(E_STKOVF);
        // stop VM execution to prEVENT unexpected state
        haltedFlag = true;
        VMCustom_reset();
    }
}

uint32_t pop() {
    currStack = currStack + 4;
    return *(uint32_t*) (CEU_data + currStack - 4);
}

float popf() {
    currStack = currStack + 4;
    return *(float*) (CEU_data + currStack - 4);
}

uint32_t getMVal(uint16_t Maddr, uint8_t type) {
    switch (type) {
        case U8:
            return (uint32_t) *(uint8_t*) (MEM + Maddr);
        case U16:
            return (uint32_t) *(uint16_t*) (MEM + Maddr);
        case U32:
            return (uint32_t) *(uint32_t*) (MEM + Maddr);
        case S8:
            return (uint32_t) *(int8_t*) (MEM + Maddr);
        case S16:
            return (uint32_t) *(int16_t*) (MEM + Maddr);
        case S32:
            return (uint32_t) *(int32_t*) (MEM + Maddr);
    }
    printf("ERROR VM::getMVal(): Invalid type=%d\n", type);
    return 0;
}

float getMValf(uint16_t Maddr) {
    return (float) *(float*) (MEM + Maddr);
}

void setMVal(uint32_t buffer, uint16_t Maddr, uint8_t fromTp, uint8_t toTp) {
    if (fromTp == F32) {
        //float value = *(float*) &buffer; //type-punning error
        float value;
        memcpy(&value, &buffer, sizeof(uint32_t));
        switch (toTp) {
            case U8:
                *(uint8_t*) (MEM + Maddr) = (uint8_t) value;
                return;
            case U16:
                *(uint16_t*) (MEM + Maddr) = (uint16_t) value;
                return;
            case U32:
                *(uint32_t*) (MEM + Maddr) = (uint32_t) value;
                return;
            case F32:
                *(float*) (MEM + Maddr) = (float) value;
                return;
            case S8:
                *(int8_t*) (MEM + Maddr) = (int8_t) value;
                return;
            case S16:
                *(int16_t*) (MEM + Maddr) = (int16_t) value;
                return;
            case S32:
                *(int32_t*) (MEM + Maddr) = (int32_t) value;
                return;
        }
    } else {
        if (fromTp <= F32) { // from unsignal integer
            uint32_t value = *(uint32_t*) &buffer;
            switch (toTp) {
                case U8:
                    *(uint8_t*) (MEM + Maddr) = (uint8_t) value;
                    return;
                case U16:
                    *(uint16_t*) (MEM + Maddr) = (uint16_t) value;
                    return;
                case U32:
                    *(uint32_t*) (MEM + Maddr) = (uint32_t) value;
                    return;
                case F32:
                    *(float*) (MEM + Maddr) = (float) value;
                    return;
                case S8:
                    *(int8_t*) (MEM + Maddr) = (int8_t) value;
                    return;
                case S16:
                    *(int16_t*) (MEM + Maddr) = (int16_t) value;
                    return;
                case S32:
                    *(int32_t*) (MEM + Maddr) = (int32_t) value;
                    return;
            }
        } else {  // from signaled integer
            int32_t value = *(int32_t*) &buffer;
            switch (toTp) {
                case U8:
                    *(uint8_t*) (MEM + Maddr) = (uint8_t) value;
                    return;
                case U16:
                    *(uint16_t*) (MEM + Maddr) = (uint16_t) value;
                    return;
                case U32:
                    *(uint32_t*) (MEM + Maddr) = (uint32_t) value;
                    return;
                case F32:
                    *(float*) (MEM + Maddr) = (float) value;
                    return;
                case S8:
                    *(int8_t*) (MEM + Maddr) = (int8_t) value;
                    return;
                case S16:
                    *(int16_t*) (MEM + Maddr) = (int16_t) value;
                    return;
                case S32:
                    *(int32_t*) (MEM + Maddr) = (int32_t) value;
                    return;
            }
        }
    }
    printf("ERROR VM::setMVal(): Invalid fromTp=%d, toTp=%d\n", fromTp, toTp);
}
