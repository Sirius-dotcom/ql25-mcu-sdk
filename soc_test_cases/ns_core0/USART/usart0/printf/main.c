#include <stdio.h>
#include <stdlib.h>
#include "ns_sdk_hal.h"

void print_misa(void)
{
    CSR_MISA_Type misa_bits = (CSR_MISA_Type) __RV_CSR_READ(CSR_MISA);
    static char misa_chars[30] = {0};
    uint8_t index = 0;
    if (misa_bits.b.mxl == 1) {
        misa_chars[index++] = '3';
        misa_chars[index++] = '2';
    } else if (misa_bits.b.mxl == 2) {
        misa_chars[index++] = '6';
        misa_chars[index++] = '4';
    } else if (misa_bits.b.mxl == 3) {
        misa_chars[index++] = '1';
        misa_chars[index++] = '2';
        misa_chars[index++] = '8';
    }
    if (misa_bits.b.i) {
        misa_chars[index++] = 'I';
    }
    if (misa_bits.b.m) {
        misa_chars[index++] = 'M';
    }
    if (misa_bits.b.a) {
        misa_chars[index++] = 'A';
    }
    if (misa_bits.b.b) {
        misa_chars[index++] = 'B';
    }
    if (misa_bits.b.c) {
        misa_chars[index++] = 'C';
    }
    if (misa_bits.b.e) {
        misa_chars[index++] = 'E';
    }
    if (misa_bits.b.f) {
        misa_chars[index++] = 'F';
    }
    if (misa_bits.b.d) {
        misa_chars[index++] = 'D';
    }
    if (misa_bits.b.q) {
        misa_chars[index++] = 'Q';
    }
    if (misa_bits.b.h) {
        misa_chars[index++] = 'H';
    }
    if (misa_bits.b.j) {
        misa_chars[index++] = 'J';
    }
    if (misa_bits.b.l) {
        misa_chars[index++] = 'L';
    }
    if (misa_bits.b.n) {
        misa_chars[index++] = 'N';
    }
    if (misa_bits.b.s) {
        misa_chars[index++] = 'S';
    }
    if (misa_bits.b.p) {
        misa_chars[index++] = 'P';
    }
    if (misa_bits.b.t) {
        misa_chars[index++] = 'T';
    }
    if (misa_bits.b.u) {
        misa_chars[index++] = 'U';
    }
    if (misa_bits.b.x) {
        misa_chars[index++] = 'X';
    }
    misa_chars[index++] = '\0';

    printf("MISA: RV%s\r\n", misa_chars);
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    int i  = 0;

    #ifndef CFG_SIMULATION
    print_misa();
    #endif
    simulation_pass();

    while (1) {}
}
