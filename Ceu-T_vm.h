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
#define CEU_STACK_MIN 0x01 // min prio for 'stack'
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

// vm input event structure
typedef struct evt_data {
    uint8_t evt_id; //
    uint8_t aux_id; //
      void* data;  //
} evt_data_t;

#define EVTQ_SIZE 10 //SHORT_QUEUES = 6
enum {
    EVT_QUEUE_SIZE = EVTQ_SIZE, //

    // ctrl section pointers
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

    // var types
    U8  = 0, //
    U16 = 1, //
    U32 = 2, //
    F32 = 3, //
    S8  = 4, //
    S16 = 5, //
    S32 = 6, //

    // generic var type
    x8  = 0, //
    x16 = 1, //
    x32 = 2, //

    // cast modes
    U32_F = 0, //
    S32_F = 1, //
    F_U32 = 2, //
    F_S32 = 3, //
};

enum {
    // opcodes uint8
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

    // opcodes float
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
    op_outevt_e   = 37,  // out event e
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
    op_memclr     = 72,  // set memory to 0
    op_ifelse     = 76,  // if else on top stack
    op_asen       = 80,  // ? asynchronous enable
    op_tkclr      = 84,  // ? track clear
    op_outevt_c   = 88,  // out event
    op_getextdt_v = 92,  // get from external memory
    op_inc        = 96,  // increment memory
    op_dec        = 100, // decrement memory
    op_set_e      = 104, // set bin on memory
    op_deref      = 112, // push to stack memory value
    op_memcpy     = 120, // memory copy

    op_tkins_max  = 136, // ? track insert
    op_push_v     = 144, // push to stack
    op_pop        = 160, // pop from stack
    op_outEvt_v   = 176, // out event
    op_set_c      = 192, // set value in memory
};

// Ceu environment vars
typedef struct prog_env {
    uint16_t version;     //
    uint16_t prog_start;   //
    uint16_t prog_end;     //
    uint16_t n_tracks;     //
    uint16_t w_clocks;     //
    uint16_t asyncs;      //
    uint16_t w_clock0;     //
    uint16_t gate0;       //
    uint16_t in_evts;      //
    uint16_t async0;      //
    uint16_t app_size;     //
     uint8_t persist_flag; //
} prog_env_t;

// event id only for system error (from VMCustom*.h)
enum {
    I_ERROR_ID = 0,  //
    I_ERROR    = 1,  //

// vm error codes
    E_DIVZERO  = 10, // Division by zero
    E_IDXOVF   = 11, // Array index overflow
    E_STKOVF   = 20, // Stack overflow
    E_NOSETUP  = 21, // Missing operation setup
};

////////////////

#define MEMORY_SIZE 4840 // BLOCK_SIZE=22 * CURRENT_MAX_BLOCKS=220
// Ceu environment vars
      bool halted_flag = true;             // vm halted flag
      bool proc_flag = false;              // vm processing flag
   uint8_t *mem;                          // internal memory
   uint8_t ext_data_sys_error;               // last system error code
   uint8_t ceu_data[MEMORY_SIZE];         // Ceu data room for 'tracks', 'mem', 'prog', and 'stack'
  uint16_t pc;                            // program counter
  uint16_t curr_stack = (MEMORY_SIZE) - 5; // stack control
prog_env_t env_data; //

static const uint8_t is_mask[] = {
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x01, 0x01,
        0x03, 0x03, 0x03, 0x03,
        0x03, 0x07, 0x07, 0x07,
        0x07, 0x07, 0x0f, 0x0f,
        0x0f, 0x0f, 0x0f, 0x0f,
        0x3f, 0x3f, 0x3f, 0x3f,
        0x3f, 0x3f, 0x3f, 0x3f,
};
/////////////////////////////////////////
    void evt_error(uint8_t v1);
    void vm_custom_reset(void);
    void vm_custom_call_function(uint8_t v);
    void vm_custom_proc_outevt(uint8_t v1, uint32_t v2);
    void ceu_wclock_enable(uint16_t v1, int32_t v2, uint16_t v3);
    void ceu_trigger(uint16_t v1, uint16_t v2);
    void ceu_track_ins(int8_t v1, int8_t v2, int8_t v3, uint16_t v4);
    void ceu_track_clr(uint16_t v1, uint16_t v2);
    void ceu_async_enable(uint16_t v1, uint16_t v2);
    void push(uint32_t value);
    void push_f(float value);
    void set_mval(uint32_t buffer, uint16_t Maddr, uint8_t fromTp, uint8_t toTp);
   float popf();
   float get_mval_f(uint16_t Maddr);
 uint8_t get_par8(uint8_t p_len);
 uint8_t get_bits(uint8_t data, uint8_t stBit, uint8_t endBit);
 uint8_t get_bits_pow(uint8_t data, uint8_t stBit, uint8_t endBit);
uint16_t get_par16(uint8_t p_len);
uint32_t unit2val(uint32_t val, uint8_t unit);
uint32_t pop();
uint32_t get_mval(uint16_t Maddr, uint8_t type);
uint32_t get_par32(uint8_t p_len);
/////////////////////////////////////////

// for test
static char op_str[255][16];
static void op_to_str(){
    strcpy  (op_str[0], "op_nop");
    strcpy  (op_str[1], "op_end");
    strcpy  (op_str[2], "op_bnot");
    strcpy  (op_str[3], "op_lnot");
    strcpy  (op_str[4], "op_neg");
    strcpy  (op_str[5], "op_sub");
    strcpy  (op_str[6], "op_add");
    strcpy  (op_str[7], "op_mod");
    strcpy  (op_str[8], "op_mult");
    strcpy  (op_str[9], "op_div");
    strcpy (op_str[10], "op_bor");
    strcpy (op_str[11], "op_band");
    strcpy (op_str[12], "op_lshft");
    strcpy (op_str[13], "op_rshft");
    strcpy (op_str[14], "op_bxor");
    strcpy (op_str[15], "op_eq");
    strcpy (op_str[16], "op_neq");
    strcpy (op_str[17], "op_gte");
    strcpy (op_str[18], "op_lte");
    strcpy (op_str[19], "op_gt");
    strcpy (op_str[20], "op_lt");
    strcpy (op_str[21], "op_lor");
    strcpy (op_str[22], "op_land");
    strcpy (op_str[23], "op_popx");
    strcpy (op_str[25], "op_neg_f");
    strcpy (op_str[26], "op_sub_f");
    strcpy (op_str[27], "op_add_f");
    strcpy (op_str[28], "op_mult_f");
    strcpy (op_str[29], "op_div_f");
    strcpy (op_str[30], "op_eq_f");
    strcpy (op_str[31], "op_neq_f");
    strcpy (op_str[32], "op_gte_f");
    strcpy (op_str[33], "op_lte_f");
    strcpy (op_str[34], "op_gt_f");
    strcpy (op_str[35], "op_lt_f");
    strcpy (op_str[36], "op_func");
    strcpy (op_str[37], "op_outevt_e");
    strcpy (op_str[38], "op_outevt_z");
    strcpy (op_str[39], "op_clken_e");
    strcpy (op_str[40], "op_clken_v");
    strcpy (op_str[41], "op_clken_c");
    strcpy (op_str[42], "op_set_v");
    strcpy (op_str[43], "op_setarr_vc");
    strcpy (op_str[44], "op_setarr_vv");
    strcpy (op_str[48], "op_poparr_v");
    strcpy (op_str[50], "op_pusharr_v");
    strcpy (op_str[52], "op_getextdt_e");
    strcpy (op_str[54], "op_trg");
    strcpy (op_str[56], "op_exec");
    strcpy (op_str[58], "op_chkret");
    strcpy (op_str[60], "op_tkins_z");
    strcpy (op_str[64], "op_push_c");
    strcpy (op_str[68], "op_cast");
    strcpy (op_str[72], "op_memclr");
    strcpy (op_str[76], "op_ifelse");
    strcpy (op_str[80], "op_asen");
    strcpy (op_str[84], "op_tkclr");
    strcpy (op_str[88], "op_outevt_c");
    strcpy (op_str[92], "op_getextdt_v");
    strcpy (op_str[96], "op_inc");
    strcpy(op_str[100], "op_dec");
    strcpy(op_str[104], "op_set_e");
    strcpy(op_str[112], "op_deref");
    strcpy(op_str[120], "op_memcpy");
    strcpy(op_str[136], "op_tkins_max");
    strcpy(op_str[144], "op_push_v");
    strcpy(op_str[160], "op_pop");
    strcpy(op_str[176], "op_outEvt_v");
    strcpy(op_str[192], "op_set_c");
}
//

uint8_t vm_init() {
    op_to_str();
    CEU->p_tracks = (tceu_trk*) ceu_data + 0;
    CEU->p_mem = ceu_data + ((env_data.n_tracks + 1) * sizeof(tceu_trk));
    mem = CEU->p_mem;

    return 0;
}

uint8_t get_opcode(uint8_t* opcode, uint8_t* modifier) {
    uint8_t temp_opc;
    temp_opc = (uint8_t) (ceu_data[pc]);
    printf("VM::getOpCode(): CEU_data[%d]=%d (0x%02x)  %s \n", pc, temp_opc, temp_opc, (temp_opc == op_end) ? "--> f_end" : "");
    pc++;
    *modifier = (uint8_t) (temp_opc & is_mask[(temp_opc >> 3)]);
    *opcode = (uint8_t) (temp_opc & ~is_mask[(temp_opc >> 3)]);
    return (*opcode);
}

void decode_opcode(uint8_t opcode, uint8_t modifier) {
       float v1f, v2f;
     uint8_t v1u8, v2u8, v3u8, v4u8, v5u8, v6u8, v7u8, v8u8, v9u9;
    uint16_t v1u16, v2u16, v3u16, v4u16;
    uint32_t v1u32, v2u32;

    op_to_str();
    printf("%d: %s %d\n", opcode, op_str[opcode], modifier);

    switch (opcode) {
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
                evt_error(E_DIVZERO);
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
                evt_error(E_DIVZERO);
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
            push_f(v1f);
            break;
        case op_sub_f:
            v1f = popf();
            v2f = popf();
            push_f(v1f - v2f);
            break;
        case op_add_f:
            v1f = popf();
            v2f = popf();
            push_f(v1f + v2f);
            break;
        case op_mult_f:
            v1f = popf();
            v2f = popf();
            push_f(v1f * v2f);
            break;
        case op_div_f:
            v1f = popf();
            v2f = popf();
            push_f(v1f / v2f);
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
            v1u8 = get_par8(1);
            vm_custom_call_function(v1u8);
            break;
        case op_outevt_e:
            v1u32 = pop();
            v1u8  = get_par8(1);
            vm_custom_proc_outevt(v1u8, v1u32);
            break;
        case op_outevt_z:
            v3u8 = get_par8(1);
            vm_custom_proc_outevt(v3u8, 0);
            break;
        case op_clken_e:
            v1u32 = 0;
            modifier = get_par8(1);
            v1u8  = get_bits_pow(modifier, 1, 1);
            v2u8  = get_bits_pow(modifier, 0, 0);
            v3u8  = get_bits(modifier, 4, 6);
            v1u16 = get_par16(v1u8);
            v3u16 = get_par16(v2u8);
            v1u32 = pop();
            ceu_wclock_enable(v1u16, (int32_t) unit2val(v1u32, v3u8), v3u16);
            break;
        case op_clken_v:
            v1u32 = 0;
            modifier = get_par8(1);
            v4u8  = get_bits(modifier, 5, 7);
            v5u8  = get_bits(modifier, 3, 4); // Expect type with 2 bits -- only unsigned integer
            v1u8  = get_bits_pow(modifier, 2, 2);
            v2u8  = get_bits_pow(modifier, 1, 1);
            v3u8  = get_bits_pow(modifier, 0, 0);
            v1u16 = get_par16(v1u8);
            v2u16 = get_par16(v2u8);
            v3u16 = get_par16(v3u8);
            v1u32 = get_mval(v2u16, v5u8);
            ceu_wclock_enable(v1u16, (int32_t) unit2val(v1u32, v4u8), v3u16);
            break;
        case op_clken_c:
            modifier = get_par8(1);
            v1u8  = get_bits_pow(modifier, 3, 3);
            v2u8  = (uint8_t) (get_bits(modifier, 1, 2) + 1);
            v3u8  = get_bits_pow(modifier, 0, 0);
            v1u16 = get_par16(v1u8);
            v1u32 = get_par32(v2u8);
            v3u16 = get_par16(v3u8);
            ceu_wclock_enable(v1u16, (int32_t) v1u32, v3u16);
            break;
        case op_set_v:
            modifier = get_par8(1);
            v1u8  = get_bits_pow(modifier, 7, 7);
            v4u8  = get_bits(modifier, 4, 6);
            v2u8  = get_bits_pow(modifier, 3, 3);
            v5u8  = get_bits(modifier, 0, 2);
            v1u16 = get_par16(v1u8);
            v2u16 = get_par16(v2u8);
            if (v5u8 == F32) {         // Source is a float
                v1f = get_mval_f(v2u16);
                //setMVal(*(uint32_t*) &v1f, v1u16, v5u8, v4u8); //type-punning error
                memcpy(&v1u32, &v1f, sizeof(float));
                set_mval(v1u32, v1u16, v5u8, v4u8);
            } else {                 // Source is an integer
                v1u32 = get_mval(v2u16, v5u8);
                set_mval(v1u32, v1u16, v5u8, v4u8);
            }
            break;
        case op_setarr_vc:
            modifier = get_par8(1);
            v8u8  = get_par8(1);
            v1u8  = get_bits_pow(modifier, 7, 7);
            v4u8  = get_bits(modifier, 4, 6);
            v2u8  = get_bits_pow(modifier, 3, 3);
            v5u8  = get_bits(modifier, 0, 2);
            v3u8  = get_bits_pow(v8u8, 2, 2);
            v7u8  = (uint8_t) (get_bits(v8u8, 0, 1) + 1);
            v6u8  = (v4u8 == F32) ? 4 : 1 << (v4u8 & 0x3);
            v1u16 = get_par16(v1u8);
            v3u16 = get_par16(v2u8);
            v4u16 = get_par16(v3u8);
            v1u32 = get_par32(v7u8);
            if (get_mval(v3u16, v5u8) >= v4u16)
                evt_error(E_IDXOVF);
            else {
                if (v4u8 == F32) {
                    v1f = (float) v1u32;
                    //setMVal(*(uint32_t*) &v1f, v1u16 + ((getMVal(v3u16, v5u8) % v4u16) * v6u8), v5u8, v4u8); //type-punning error
                    memcpy(&v1u32, &v1f, sizeof(float));
                    set_mval(v1u32, v1u16 + ((get_mval(v3u16, v5u8) % v4u16) * v6u8), v5u8, v4u8);
                } else {
                    set_mval(v1u32, v1u16 + ((get_mval(v3u16, v5u8) % v4u16) * v6u8), v5u8, v4u8);
                }
            }
            break;
        case op_setarr_vv:
            modifier = get_par8(1);
            v8u8  = get_par8(1);
            v1u8  = get_bits_pow(modifier, 7, 7);
            v4u8  = get_bits(modifier, 4, 6);
            v2u8  = get_bits_pow(modifier, 3, 3);
            v5u8  = get_bits(modifier, 0, 2);
            v3u8  = get_bits_pow(v8u8, 4, 4);
            v7u8  = get_bits_pow(v8u8, 3, 3);
            v9u9  = get_bits(v8u8, 0, 2);
            v6u8  = (v4u8 == F32) ? 4 : 1 << (v4u8 & 0x3);
            v1u16 = get_par16(v1u8);
            v3u16 = get_par16(v2u8);
            v4u16 = get_par16(v3u8);
            v2u16 = get_par16(v7u8);
            if (get_mval(v3u16, v5u8) >= v4u16)
                evt_error(E_IDXOVF);
            else {
                if (v9u9 == F32) {         // Source is a float
                    v1f = get_mval_f(v2u16);
                    //setMVal(*(uint32_t*) &v1f, v1u16 + ((getMVal(v3u16, v5u8) % v4u16) * v6u8), v9u9, v4u8); //type-punning error
                    memcpy(&v1u32, &v1f, sizeof(float));
                    set_mval(v1u32, v1u16 + ((get_mval(v3u16, v5u8) % v4u16) * v6u8), v9u9, v4u8);
                } else {                 // Source is an integer
                    v1u32 = get_mval(v2u16, v9u9);
                    set_mval(v1u32, v1u16 + ((get_mval(v3u16, v5u8) % v4u16) * v6u8), v9u9, v4u8);
                }
            }
            break;
        case op_poparr_v:
            v3u8  = get_bits_pow(modifier, 0, 0);
            v8u8  = get_par8(1);
            v1u8  = get_bits_pow(v8u8, 7, 7);
            v4u8  = get_bits(v8u8, 4, 6);
            v2u8  = get_bits_pow(v8u8, 3, 3);
            v5u8  = get_bits(v8u8, 0, 2);
            v6u8  = (v4u8 == F32) ? 4 : 1 << (v4u8 & 0x3);
            v1u16 = get_par16(v1u8);
            v3u16 = get_par16(v2u8);
            v4u16 = get_par16(v3u8);
            if (get_mval(v3u16, v5u8) >= v4u16)
                evt_error(E_IDXOVF);
            else {
                if (v4u8 == F32) { // Source/Target are float
                    v1f = popf();
                    //setMVal(*(uint32_t*) &v1f, v1u16 + ((getMVal(v3u16, v5u8) % v4u16) * v6u8), F32, v4u8); //type-punning error
                    memcpy(&v1u32, &v1f, sizeof(float));
                    set_mval(v1u32, v1u16 + ((get_mval(v3u16, v5u8) % v4u16) * v6u8), F32, v4u8);
                } else { // Source/Target are integer
                    v1u32 = pop();
                    set_mval(v1u32, v1u16 + ((get_mval(v3u16, v5u8) % v4u16) * v6u8), S32, v4u8);
                }
            }
            break;
        case op_pusharr_v:
            v3u8  = get_bits_pow(modifier, 0, 0);
            v8u8  = get_par8(1);
            v1u8  = get_bits_pow(v8u8, 7, 7);
            v4u8  = get_bits(v8u8, 4, 6);
            v2u8  = get_bits_pow(v8u8, 3, 3);
            v5u8  = get_bits(v8u8, 0, 2);
            v1u8  = (v4u8 == F32) ? 4 : 1 << (v4u8 & 0x3);
            v2u8  = (v5u8 == F32) ? 4 : 1 << (v5u8 & 0x3);
            v1u16 = get_par16(v1u8);
            v3u16 = get_par16(v2u8);
            v4u16 = get_par16(v3u8);
            if (get_mval(v3u16, v5u8) >= v4u16)
                evt_error(E_IDXOVF);
            else {
                push(v1u16 + ((get_mval(v3u16, v5u8) % v4u16) * v1u8));
            }
            break;
        case op_getextdt_e:
            v1u8  = get_bits_pow(modifier, 0, 0);
            v1u16 = (uint16_t) pop();
            v2u16 = get_par16(v1u8);
            memcpy((mem + v1u16), CEU->ext_data, v2u16);
            break;
        case op_trg:
            v1u8  = get_bits_pow(modifier, 0, 0);
            v1u16 = get_par16(v1u8);
            ceu_trigger(v1u16, 0);
            break;
        case op_exec:
            v1u8  = get_bits_pow(modifier, 0, 0);
            v1u16 = get_par16(v1u8);
            pc = v1u16;
            break;
        case op_chkret:
            v1u8  = get_bits_pow(modifier, 0, 0);
            v1u16 = get_par16(v1u8);
            if (*(uint8_t*) (mem + v1u16) > 0)
                pc = pc + 1;
            break;
        case op_tkins_z:
            v2u8  = get_bits_pow(modifier, 0, 0);
            v4u8  = get_par8(1);
            v3u8  = get_bits(v4u8, 7, 7);
            v1u8  = get_bits(v4u8, 0, 6);
            v3u16 = get_par16(v2u8);
            ceu_track_ins(0, v1u8, v3u8, v3u16);
            break;
        case op_push_c:
            v1u8  = (uint8_t) (get_bits(modifier, 0, 1) + 1);
            v1u32 = get_par32(v1u8);
            push(v1u32);
            break;
        case op_cast:
            v1u8 = get_bits(modifier, 0, 1);
            switch (v1u8) {
                case U32_F:
                    v1u32 = pop();
                    push_f((float) *(uint32_t*) &v1u32);
                    break;
                case S32_F:
                    v1u32 = pop();
                    push_f((float) *(int32_t*) &v1u32);
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
            v1u8  = get_bits_pow(modifier, 1, 1);
            v2u8  = get_bits_pow(modifier, 0, 0);
            v1u16 = get_par16(v1u8);
            v2u16 = get_par16(v2u8);
            memset((mem+v1u16),0,v2u16); // does not work in TOSSIM
            //{
            //    int x;
            //    for (x = 0; x < v2u16; x++)
            //        *(uint8_t*) (MEM + v1u16 + x) = 0;
            //}
            break;
        case op_ifelse:
            v1u8  = get_bits_pow(modifier, 1, 1);
            v2u8  = get_bits_pow(modifier, 0, 0);
            v1u16 = get_par16(v1u8);
            v2u16 = get_par16(v2u8);
            if (pop())
                pc = v1u16;
            else
                pc = v2u16;
            break;
        case op_asen:
            v1u8  = get_bits_pow(modifier, 1, 1);
            v2u8  = get_bits_pow(modifier, 0, 0);
            v1u16 = get_par16(v1u8);
            v3u16 = get_par16(v2u8);
            ceu_async_enable(v1u16, v3u16);
            break;
        case op_tkclr:
            v1u8  = get_bits_pow(modifier, 1, 1);
            v2u8  = get_bits_pow(modifier, 0, 0);
            v1u16 = get_par16(v1u8);
            v2u16 = get_par16(v2u8);
            ceu_track_clr(v1u16, v2u16);
            break;
        case op_outevt_c:
            v1u8  = (uint8_t) (get_bits(modifier, 0, 1) + 1);
            v3u8  = get_par8(1);
            v1u32 = get_par32(v1u8);
            vm_custom_proc_outevt(v3u8, v1u32);
            break;
        case op_getextdt_v:
            v1u8  = get_bits_pow(modifier, 1, 1);
            v2u8  = get_bits_pow(modifier, 0, 0);
            v1u16 = get_par16(v1u8);
            v2u16 = get_par16(v2u8);
            memcpy((mem + v1u16), CEU->ext_data, v2u16);
            break;
        case op_inc:
            v4u8  = get_bits(modifier, 0, 1);
            v6u8  = 1 << v4u8;
            v1u16 = (uint16_t) pop();
            set_mval((get_mval(v1u16, v4u8) + 1), v1u16, v4u8, v4u8);
            break;
        case op_dec:
            v4u8  = get_bits(modifier, 0, 1);
            v6u8  = 1 << v4u8;
            v1u16 = (uint16_t) pop();
            set_mval((get_mval(v1u16, v4u8) - 1), v1u16, v4u8, v4u8);
            break;
        case op_set_e:
            v4u8  = get_bits(modifier, 0, 2);
            v6u8  = (v4u8 == F32) ? 4 : 1 << (v4u8 & 0x3);
            v1u16 = (uint16_t) pop();
            if (v4u8 == F32) {
                v1f = popf();
                //setMVal(*(uint32_t*) &v1f, v1u16, F32, v4u8); //type-punning error
                memcpy(&v1u32, &v1f, sizeof(float));
                set_mval(v1u32, v1u16, F32, v4u8);
            } else {
                v1u32 = pop();
                set_mval(v1u32, v1u16, S32, v4u8);
            }
            break;
        case op_deref:
            v1u8  = get_bits(modifier, 0, 2);
            v1u16 = (uint16_t) pop();
            switch (v1u8) {
                case U8:
                    push((uint8_t) get_mval(v1u16, v1u8));
                    break;
                case U16:
                    push((uint16_t) get_mval(v1u16, v1u8));
                    break;
                case U32:
                    push((uint32_t) get_mval(v1u16, v1u8));
                    break;
                case F32:
                    push_f(get_mval_f(v1u16));
                    break;
                case S8:
                    push((int8_t) get_mval(v1u16, v1u8));
                    break;
                case S16:
                    push((int16_t) get_mval(v1u16, v1u8));
                    break;
                case S32:
                    push((int32_t) get_mval(v1u16, v1u8));
                    break;
            }
            break;
        case op_memcpy:
            v1u8  = get_bits_pow(modifier, 2, 2);
            v2u8  = get_bits_pow(modifier, 1, 1);
            v3u8  = get_bits_pow(modifier, 0, 0);
            v2u16 = get_par16(v1u8);
            v1u16 = get_par16(v2u8);
            v3u16 = get_par16(v3u8);
            memcpy((void*) (mem + v3u16), (void*) (mem + v1u16), v2u16);
            break;
        case op_tkins_max:
            v2u8  = (uint8_t) (CEU->stack + get_bits(modifier, 1, 2));
            v1u8  = get_bits_pow(modifier, 0, 0);
            v3u16 = get_par16(v1u8);
            ceu_track_ins(v2u8, 255, 0, v3u16);
            break;
        case op_push_v:
            v1u8  = get_bits_pow(modifier, 3, 3);
            v4u8  = get_bits(modifier, 0, 2);
            v1u16 = get_par16(v1u8);
            if (v4u8 == F32) {
                push_f(get_mval_f(v1u16));
            } else {
                push(get_mval(v1u16, v4u8));
            }
            break;
        case op_pop:
            v1u8  = get_bits_pow(modifier, 3, 3);
            v4u8  = get_bits(modifier, 0, 2);
            v1u16 = get_par16(v1u8);
            if (v4u8 == F32) {
                v1f = popf();
                //setMVal(*(uint32_t*) &v1f, v1u16, F32, v4u8); //type-punning error
                memcpy(&v1u32, &v1f, sizeof(float));
                set_mval(v1u32, v1u16, F32, v4u8);
            } else {
                v1u32 = pop();
                set_mval(v1u32, v1u16, S32, v4u8);
            }
            break;
        case op_outEvt_v:
            v2u8  = get_bits_pow(modifier, 3, 3);
            v3u8  = get_par8(1);
            v1u16 = get_par16(v2u8);
            vm_custom_proc_outevt(v3u8, v1u16);
            break;
        case op_set_c:
            v4u8  = get_bits(modifier, 0, 2);
            v6u8  = (v4u8 == F32) ? 4 : 1 << (v4u8 & 0x3);
            v1u8  = get_bits_pow(modifier, 3, 3);
            v2u8  = (uint8_t) (get_bits(modifier, 4, 5) + 1);
            v1u16 = get_par16(v1u8);
            v1u32 = get_par32(v2u8);
            if (v4u8 == F32) {
                //v1f = *(float*) &v1u32;
                //setMVal(*(uint32_t*) &v1f, v1u16, F32, v4u8); //type-punning error
                memcpy(&v1f, &v1u32, sizeof(uint32_t)); //TODO: revisar, doble conversion
                memcpy(&v2u32, &v1f, sizeof(float));
                set_mval(v2u32, v1u16, F32, v4u8);

            } else {
                set_mval(v1u32, v1u16, S32, v4u8);
            }
            break;
    }
}

//////////////// internal /////////////////

void evt_error(uint8_t v1) {
    printf("evtError %d\n", v1);
}

//
void vm_custom_reset(void) {
    printf("VMCustom_reset\n");
}

//
void vm_custom_call_function(uint8_t v) {
    printf("VMCustom_callFunction %d\n", v);
}

//
void vm_custom_proc_outevt(uint8_t v1, uint32_t v2) {
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

/////////////////// aux ///////////////////

// Get constant values from program memory. Convert big-endian to the local type endian.
uint8_t get_par8(uint8_t p_len) {
    uint8_t temp = (uint8_t) ceu_data[pc];
    pc++;
    return temp;
}

uint16_t get_par16(uint8_t p_len) {
    uint16_t temp = 0L;
    switch (p_len) {
        case 1:
            temp = ((uint16_t) ((uint8_t) ceu_data[pc]) << 0);
            pc++;
            break;
        case 2:
            temp = ((uint16_t) ((uint8_t) ceu_data[pc]) << 8);
            pc++;
            temp = temp + ((uint16_t) ((uint8_t) ceu_data[pc]) << 0);
            pc++;
            break;
    }
    return temp;
}

uint32_t get_par32(uint8_t p_len) {
    uint32_t temp = 0L;
    switch (p_len) {
        case 1:
            temp = ((uint32_t) ((uint8_t) ceu_data[pc]) << 0);
            pc++;
            break;
        case 2:
            temp = ((uint32_t) ((uint8_t) ceu_data[pc]) << 8);
            pc++;
            temp = temp + ((uint32_t) ((uint8_t) ceu_data[pc]) << 0);
            pc++;
            break;
        case 3:
            temp = ((uint32_t) ((uint8_t) ceu_data[pc]) << 16);
            pc++;
            temp = temp + ((uint32_t) ((uint8_t) ceu_data[pc]) << 8);
            pc++;
            temp = temp + ((uint32_t) ((uint8_t) ceu_data[pc]) << 0);
            pc++;
            break;
        case 4:
            temp = ((uint32_t) ((uint8_t) ceu_data[pc]) << 24);
            pc++;
            temp = temp + ((uint32_t) ((uint8_t) ceu_data[pc]) << 16);
            pc++;
            temp = temp + ((uint32_t) ((uint8_t) ceu_data[pc]) << 8);
            pc++;
            temp = temp + ((uint32_t) ((uint8_t) ceu_data[pc]) << 0);
            pc++;
            break;
    }
    return temp;
}

uint8_t get_bits(uint8_t data, uint8_t stBit, uint8_t endBit) {
    uint8_t ret = 0;
    ret = (data << (7 - endBit));
    ret = ret >> (7 - endBit + stBit);
    return ret;
}

uint8_t get_bits_pow(uint8_t data, uint8_t stBit, uint8_t endBit) {
    return (1 << get_bits(data, stBit, endBit));
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
    curr_stack = curr_stack - 4;
    if ((curr_stack) > env_data.prog_end)
        *(uint32_t*) (ceu_data + curr_stack) = value;
    else {
        evt_error(E_STKOVF);
        // stop VM execution to prEVENT unexpected state
        halted_flag = true;
        vm_custom_reset();
    }
}

void push_f(float value) {
    curr_stack = curr_stack - 4;
    if ((curr_stack) > env_data.prog_end) {
        *(float*) (ceu_data + curr_stack) = value;
    } else {
        evt_error(E_STKOVF);
        // stop VM execution to prEVENT unexpected state
        halted_flag = true;
        vm_custom_reset();
    }
}

uint32_t pop() {
    curr_stack = curr_stack + 4;
    return *(uint32_t*) (ceu_data + curr_stack - 4);
}

float popf() {
    curr_stack = curr_stack + 4;
    return *(float*) (ceu_data + curr_stack - 4);
}

uint32_t get_mval(uint16_t Maddr, uint8_t type) {
    switch (type) {
        case U8:
            return (uint32_t) *(uint8_t*) (mem + Maddr);
        case U16:
            return (uint32_t) *(uint16_t*) (mem + Maddr);
        case U32:
            return (uint32_t) *(uint32_t*) (mem + Maddr);
        case S8:
            return (uint32_t) *(int8_t*) (mem + Maddr);
        case S16:
            return (uint32_t) *(int16_t*) (mem + Maddr);
        case S32:
            return (uint32_t) *(int32_t*) (mem + Maddr);
    }
    printf("ERROR VM::getMVal(): Invalid type=%d\n", type);
    return 0;
}

float get_mval_f(uint16_t Maddr) {
    return (float) *(float*) (mem + Maddr);
}

void set_mval(uint32_t buffer, uint16_t Maddr, uint8_t fromTp, uint8_t toTp) {
    if (fromTp == F32) {
        //float value = *(float*) &buffer; //type-punning error
        float value;
        memcpy(&value, &buffer, sizeof(uint32_t));
        switch (toTp) {
            case U8:
                *(uint8_t*) (mem + Maddr) = (uint8_t) value;
                return;
            case U16:
                *(uint16_t*) (mem + Maddr) = (uint16_t) value;
                return;
            case U32:
                *(uint32_t*) (mem + Maddr) = (uint32_t) value;
                return;
            case F32:
                *(float*) (mem + Maddr) = (float) value;
                return;
            case S8:
                *(int8_t*) (mem + Maddr) = (int8_t) value;
                return;
            case S16:
                *(int16_t*) (mem + Maddr) = (int16_t) value;
                return;
            case S32:
                *(int32_t*) (mem + Maddr) = (int32_t) value;
                return;
        }
    } else {
        if (fromTp <= F32) { // from unsignal integer
            uint32_t value = *(uint32_t*) &buffer;
            switch (toTp) {
                case U8:
                    *(uint8_t*) (mem + Maddr) = (uint8_t) value;
                    return;
                case U16:
                    *(uint16_t*) (mem + Maddr) = (uint16_t) value;
                    return;
                case U32:
                    *(uint32_t*) (mem + Maddr) = (uint32_t) value;
                    return;
                case F32:
                    *(float*) (mem + Maddr) = (float) value;
                    return;
                case S8:
                    *(int8_t*) (mem + Maddr) = (int8_t) value;
                    return;
                case S16:
                    *(int16_t*) (mem + Maddr) = (int16_t) value;
                    return;
                case S32:
                    *(int32_t*) (mem + Maddr) = (int32_t) value;
                    return;
            }
        } else {  // from signaled integer
            int32_t value = *(int32_t*) &buffer;
            switch (toTp) {
                case U8:
                    *(uint8_t*) (mem + Maddr) = (uint8_t) value;
                    return;
                case U16:
                    *(uint16_t*) (mem + Maddr) = (uint16_t) value;
                    return;
                case U32:
                    *(uint32_t*) (mem + Maddr) = (uint32_t) value;
                    return;
                case F32:
                    *(float*) (mem + Maddr) = (float) value;
                    return;
                case S8:
                    *(int8_t*) (mem + Maddr) = (int8_t) value;
                    return;
                case S16:
                    *(int16_t*) (mem + Maddr) = (int16_t) value;
                    return;
                case S32:
                    *(int32_t*) (mem + Maddr) = (int32_t) value;
                    return;
            }
        }
    }
    printf("ERROR VM::setMVal(): Invalid fromTp=%d, toTp=%d\n", fromTp, toTp);
}
