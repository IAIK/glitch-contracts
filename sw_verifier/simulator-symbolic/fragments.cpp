#include <fmt/core.h>
#include <iostream>
#include <string>
#include <string_view>
#include "contract_runtime.h"
#include "SymbolicManager.h"
#include "bitvector.h"
void* bv_fragment_resize(void* fragment, long source_size, long target_size, long offset)
{
    // printf("bv_fragment_resize (%ld -> %ld)\n", source_size, target_size);
    switch (source_size) {
        case 1: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<1>*)(fragment)));
            case 2: return new bv_frag<2>((*(bv_frag<1>*)(fragment)).zero_extend_impl<2>());
            case 3: return new bv_frag<3>((*(bv_frag<1>*)(fragment)).zero_extend_impl<3>());
            case 4: return new bv_frag<4>((*(bv_frag<1>*)(fragment)).zero_extend_impl<4>());
            case 5: return new bv_frag<5>((*(bv_frag<1>*)(fragment)).zero_extend_impl<5>());
            case 6: return new bv_frag<6>((*(bv_frag<1>*)(fragment)).zero_extend_impl<6>());
            case 7: return new bv_frag<7>((*(bv_frag<1>*)(fragment)).zero_extend_impl<7>());
            case 8: return new bv_frag<8>((*(bv_frag<1>*)(fragment)).zero_extend_impl<8>());
            case 9: return new bv_frag<9>((*(bv_frag<1>*)(fragment)).zero_extend_impl<9>());
            case 10: return new bv_frag<10>((*(bv_frag<1>*)(fragment)).zero_extend_impl<10>());
            case 11: return new bv_frag<11>((*(bv_frag<1>*)(fragment)).zero_extend_impl<11>());
            case 12: return new bv_frag<12>((*(bv_frag<1>*)(fragment)).zero_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<1>*)(fragment)).zero_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<1>*)(fragment)).zero_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<1>*)(fragment)).zero_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<1>*)(fragment)).zero_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<1>*)(fragment)).zero_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<1>*)(fragment)).zero_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<1>*)(fragment)).zero_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<1>*)(fragment)).zero_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<1>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<1>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<1>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<1>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<1>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<1>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<1>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<1>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<1>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<1>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<1>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<1>*)(fragment)).zero_extend_impl<32>());
        }
        case 2: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<2>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<2>*)(fragment)));
            case 3: return new bv_frag<3>((*(bv_frag<2>*)(fragment)).zero_extend_impl<3>());
            case 4: return new bv_frag<4>((*(bv_frag<2>*)(fragment)).zero_extend_impl<4>());
            case 5: return new bv_frag<5>((*(bv_frag<2>*)(fragment)).zero_extend_impl<5>());
            case 6: return new bv_frag<6>((*(bv_frag<2>*)(fragment)).zero_extend_impl<6>());
            case 7: return new bv_frag<7>((*(bv_frag<2>*)(fragment)).zero_extend_impl<7>());
            case 8: return new bv_frag<8>((*(bv_frag<2>*)(fragment)).zero_extend_impl<8>());
            case 9: return new bv_frag<9>((*(bv_frag<2>*)(fragment)).zero_extend_impl<9>());
            case 10: return new bv_frag<10>((*(bv_frag<2>*)(fragment)).zero_extend_impl<10>());
            case 11: return new bv_frag<11>((*(bv_frag<2>*)(fragment)).zero_extend_impl<11>());
            case 12: return new bv_frag<12>((*(bv_frag<2>*)(fragment)).zero_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<2>*)(fragment)).zero_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<2>*)(fragment)).zero_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<2>*)(fragment)).zero_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<2>*)(fragment)).zero_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<2>*)(fragment)).zero_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<2>*)(fragment)).zero_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<2>*)(fragment)).zero_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<2>*)(fragment)).zero_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<2>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<2>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<2>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<2>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<2>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<2>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<2>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<2>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<2>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<2>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<2>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<2>*)(fragment)).zero_extend_impl<32>());
        }
        case 3: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<3>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<3>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<3>*)(fragment)));
            case 4: return new bv_frag<4>((*(bv_frag<3>*)(fragment)).zero_extend_impl<4>());
            case 5: return new bv_frag<5>((*(bv_frag<3>*)(fragment)).zero_extend_impl<5>());
            case 6: return new bv_frag<6>((*(bv_frag<3>*)(fragment)).zero_extend_impl<6>());
            case 7: return new bv_frag<7>((*(bv_frag<3>*)(fragment)).zero_extend_impl<7>());
            case 8: return new bv_frag<8>((*(bv_frag<3>*)(fragment)).zero_extend_impl<8>());
            case 9: return new bv_frag<9>((*(bv_frag<3>*)(fragment)).zero_extend_impl<9>());
            case 10: return new bv_frag<10>((*(bv_frag<3>*)(fragment)).zero_extend_impl<10>());
            case 11: return new bv_frag<11>((*(bv_frag<3>*)(fragment)).zero_extend_impl<11>());
            case 12: return new bv_frag<12>((*(bv_frag<3>*)(fragment)).zero_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<3>*)(fragment)).zero_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<3>*)(fragment)).zero_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<3>*)(fragment)).zero_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<3>*)(fragment)).zero_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<3>*)(fragment)).zero_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<3>*)(fragment)).zero_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<3>*)(fragment)).zero_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<3>*)(fragment)).zero_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<3>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<3>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<3>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<3>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<3>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<3>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<3>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<3>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<3>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<3>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<3>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<3>*)(fragment)).zero_extend_impl<32>());
        }
        case 4: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<4>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<4>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<4>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<4>*)(fragment)));
            case 5: return new bv_frag<5>((*(bv_frag<4>*)(fragment)).zero_extend_impl<5>());
            case 6: return new bv_frag<6>((*(bv_frag<4>*)(fragment)).zero_extend_impl<6>());
            case 7: return new bv_frag<7>((*(bv_frag<4>*)(fragment)).zero_extend_impl<7>());
            case 8: return new bv_frag<8>((*(bv_frag<4>*)(fragment)).zero_extend_impl<8>());
            case 9: return new bv_frag<9>((*(bv_frag<4>*)(fragment)).zero_extend_impl<9>());
            case 10: return new bv_frag<10>((*(bv_frag<4>*)(fragment)).zero_extend_impl<10>());
            case 11: return new bv_frag<11>((*(bv_frag<4>*)(fragment)).zero_extend_impl<11>());
            case 12: return new bv_frag<12>((*(bv_frag<4>*)(fragment)).zero_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<4>*)(fragment)).zero_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<4>*)(fragment)).zero_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<4>*)(fragment)).zero_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<4>*)(fragment)).zero_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<4>*)(fragment)).zero_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<4>*)(fragment)).zero_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<4>*)(fragment)).zero_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<4>*)(fragment)).zero_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<4>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<4>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<4>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<4>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<4>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<4>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<4>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<4>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<4>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<4>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<4>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<4>*)(fragment)).zero_extend_impl<32>());
        }
        case 5: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<5>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<5>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<5>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<5>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<5>*)(fragment)));
            case 6: return new bv_frag<6>((*(bv_frag<5>*)(fragment)).zero_extend_impl<6>());
            case 7: return new bv_frag<7>((*(bv_frag<5>*)(fragment)).zero_extend_impl<7>());
            case 8: return new bv_frag<8>((*(bv_frag<5>*)(fragment)).zero_extend_impl<8>());
            case 9: return new bv_frag<9>((*(bv_frag<5>*)(fragment)).zero_extend_impl<9>());
            case 10: return new bv_frag<10>((*(bv_frag<5>*)(fragment)).zero_extend_impl<10>());
            case 11: return new bv_frag<11>((*(bv_frag<5>*)(fragment)).zero_extend_impl<11>());
            case 12: return new bv_frag<12>((*(bv_frag<5>*)(fragment)).zero_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<5>*)(fragment)).zero_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<5>*)(fragment)).zero_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<5>*)(fragment)).zero_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<5>*)(fragment)).zero_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<5>*)(fragment)).zero_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<5>*)(fragment)).zero_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<5>*)(fragment)).zero_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<5>*)(fragment)).zero_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<5>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<5>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<5>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<5>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<5>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<5>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<5>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<5>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<5>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<5>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<5>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<5>*)(fragment)).zero_extend_impl<32>());
        }
        case 6: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<6>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<6>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<6>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<6>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<6>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<6>*)(fragment)));
            case 7: return new bv_frag<7>((*(bv_frag<6>*)(fragment)).zero_extend_impl<7>());
            case 8: return new bv_frag<8>((*(bv_frag<6>*)(fragment)).zero_extend_impl<8>());
            case 9: return new bv_frag<9>((*(bv_frag<6>*)(fragment)).zero_extend_impl<9>());
            case 10: return new bv_frag<10>((*(bv_frag<6>*)(fragment)).zero_extend_impl<10>());
            case 11: return new bv_frag<11>((*(bv_frag<6>*)(fragment)).zero_extend_impl<11>());
            case 12: return new bv_frag<12>((*(bv_frag<6>*)(fragment)).zero_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<6>*)(fragment)).zero_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<6>*)(fragment)).zero_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<6>*)(fragment)).zero_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<6>*)(fragment)).zero_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<6>*)(fragment)).zero_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<6>*)(fragment)).zero_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<6>*)(fragment)).zero_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<6>*)(fragment)).zero_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<6>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<6>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<6>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<6>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<6>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<6>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<6>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<6>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<6>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<6>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<6>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<6>*)(fragment)).zero_extend_impl<32>());
        }
        case 7: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<7>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<7>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<7>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<7>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<7>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<7>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<7>*)(fragment)));
            case 8: return new bv_frag<8>((*(bv_frag<7>*)(fragment)).zero_extend_impl<8>());
            case 9: return new bv_frag<9>((*(bv_frag<7>*)(fragment)).zero_extend_impl<9>());
            case 10: return new bv_frag<10>((*(bv_frag<7>*)(fragment)).zero_extend_impl<10>());
            case 11: return new bv_frag<11>((*(bv_frag<7>*)(fragment)).zero_extend_impl<11>());
            case 12: return new bv_frag<12>((*(bv_frag<7>*)(fragment)).zero_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<7>*)(fragment)).zero_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<7>*)(fragment)).zero_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<7>*)(fragment)).zero_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<7>*)(fragment)).zero_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<7>*)(fragment)).zero_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<7>*)(fragment)).zero_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<7>*)(fragment)).zero_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<7>*)(fragment)).zero_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<7>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<7>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<7>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<7>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<7>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<7>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<7>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<7>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<7>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<7>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<7>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<7>*)(fragment)).zero_extend_impl<32>());
        }
        case 8: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<8>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<8>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<8>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<8>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<8>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<8>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<8>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<8>*)(fragment)));
            case 9: return new bv_frag<9>((*(bv_frag<8>*)(fragment)).zero_extend_impl<9>());
            case 10: return new bv_frag<10>((*(bv_frag<8>*)(fragment)).zero_extend_impl<10>());
            case 11: return new bv_frag<11>((*(bv_frag<8>*)(fragment)).zero_extend_impl<11>());
            case 12: return new bv_frag<12>((*(bv_frag<8>*)(fragment)).zero_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<8>*)(fragment)).zero_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<8>*)(fragment)).zero_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<8>*)(fragment)).zero_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<8>*)(fragment)).zero_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<8>*)(fragment)).zero_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<8>*)(fragment)).zero_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<8>*)(fragment)).zero_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<8>*)(fragment)).zero_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<8>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<8>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<8>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<8>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<8>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<8>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<8>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<8>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<8>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<8>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<8>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<8>*)(fragment)).zero_extend_impl<32>());
        }
        case 9: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<9>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<9>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<9>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<9>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<9>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<9>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<9>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<9>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<9>*)(fragment)));
            case 10: return new bv_frag<10>((*(bv_frag<9>*)(fragment)).zero_extend_impl<10>());
            case 11: return new bv_frag<11>((*(bv_frag<9>*)(fragment)).zero_extend_impl<11>());
            case 12: return new bv_frag<12>((*(bv_frag<9>*)(fragment)).zero_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<9>*)(fragment)).zero_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<9>*)(fragment)).zero_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<9>*)(fragment)).zero_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<9>*)(fragment)).zero_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<9>*)(fragment)).zero_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<9>*)(fragment)).zero_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<9>*)(fragment)).zero_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<9>*)(fragment)).zero_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<9>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<9>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<9>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<9>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<9>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<9>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<9>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<9>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<9>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<9>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<9>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<9>*)(fragment)).zero_extend_impl<32>());
        }
        case 10: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<10>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<10>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<10>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<10>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<10>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<10>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<10>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<10>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<10>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<10>*)(fragment)));
            case 11: return new bv_frag<11>((*(bv_frag<10>*)(fragment)).zero_extend_impl<11>());
            case 12: return new bv_frag<12>((*(bv_frag<10>*)(fragment)).zero_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<10>*)(fragment)).zero_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<10>*)(fragment)).zero_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<10>*)(fragment)).zero_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<10>*)(fragment)).zero_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<10>*)(fragment)).zero_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<10>*)(fragment)).zero_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<10>*)(fragment)).zero_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<10>*)(fragment)).zero_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<10>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<10>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<10>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<10>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<10>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<10>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<10>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<10>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<10>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<10>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<10>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<10>*)(fragment)).zero_extend_impl<32>());
        }
        case 11: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<11>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<11>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<11>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<11>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<11>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<11>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<11>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<11>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<11>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<11>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<11>*)(fragment)));
            case 12: return new bv_frag<12>((*(bv_frag<11>*)(fragment)).zero_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<11>*)(fragment)).zero_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<11>*)(fragment)).zero_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<11>*)(fragment)).zero_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<11>*)(fragment)).zero_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<11>*)(fragment)).zero_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<11>*)(fragment)).zero_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<11>*)(fragment)).zero_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<11>*)(fragment)).zero_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<11>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<11>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<11>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<11>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<11>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<11>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<11>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<11>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<11>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<11>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<11>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<11>*)(fragment)).zero_extend_impl<32>());
        }
        case 12: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<12>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<12>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<12>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<12>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<12>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<12>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<12>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<12>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<12>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<12>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<12>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<12>*)(fragment)));
            case 13: return new bv_frag<13>((*(bv_frag<12>*)(fragment)).zero_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<12>*)(fragment)).zero_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<12>*)(fragment)).zero_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<12>*)(fragment)).zero_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<12>*)(fragment)).zero_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<12>*)(fragment)).zero_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<12>*)(fragment)).zero_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<12>*)(fragment)).zero_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<12>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<12>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<12>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<12>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<12>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<12>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<12>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<12>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<12>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<12>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<12>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<12>*)(fragment)).zero_extend_impl<32>());
        }
        case 13: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<13>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<13>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<13>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<13>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<13>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<13>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<13>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<13>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<13>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<13>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<13>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<13>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<13>*)(fragment)));
            case 14: return new bv_frag<14>((*(bv_frag<13>*)(fragment)).zero_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<13>*)(fragment)).zero_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<13>*)(fragment)).zero_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<13>*)(fragment)).zero_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<13>*)(fragment)).zero_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<13>*)(fragment)).zero_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<13>*)(fragment)).zero_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<13>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<13>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<13>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<13>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<13>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<13>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<13>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<13>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<13>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<13>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<13>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<13>*)(fragment)).zero_extend_impl<32>());
        }
        case 14: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<14>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<14>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<14>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<14>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<14>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<14>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<14>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<14>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<14>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<14>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<14>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<14>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<14>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<14>*)(fragment)));
            case 15: return new bv_frag<15>((*(bv_frag<14>*)(fragment)).zero_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<14>*)(fragment)).zero_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<14>*)(fragment)).zero_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<14>*)(fragment)).zero_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<14>*)(fragment)).zero_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<14>*)(fragment)).zero_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<14>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<14>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<14>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<14>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<14>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<14>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<14>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<14>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<14>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<14>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<14>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<14>*)(fragment)).zero_extend_impl<32>());
        }
        case 15: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<15>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<15>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<15>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<15>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<15>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<15>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<15>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<15>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<15>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<15>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<15>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<15>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<15>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<15>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<15>*)(fragment)));
            case 16: return new bv_frag<16>((*(bv_frag<15>*)(fragment)).zero_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<15>*)(fragment)).zero_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<15>*)(fragment)).zero_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<15>*)(fragment)).zero_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<15>*)(fragment)).zero_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<15>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<15>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<15>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<15>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<15>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<15>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<15>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<15>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<15>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<15>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<15>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<15>*)(fragment)).zero_extend_impl<32>());
        }
        case 16: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<16>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<16>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<16>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<16>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<16>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<16>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<16>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<16>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<16>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<16>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<16>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<16>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<16>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<16>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<16>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<16>*)(fragment)));
            case 17: return new bv_frag<17>((*(bv_frag<16>*)(fragment)).zero_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<16>*)(fragment)).zero_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<16>*)(fragment)).zero_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<16>*)(fragment)).zero_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<16>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<16>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<16>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<16>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<16>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<16>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<16>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<16>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<16>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<16>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<16>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<16>*)(fragment)).zero_extend_impl<32>());
        }
        case 17: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<17>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<17>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<17>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<17>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<17>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<17>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<17>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<17>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<17>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<17>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<17>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<17>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<17>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<17>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<17>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<17>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<17>*)(fragment)));
            case 18: return new bv_frag<18>((*(bv_frag<17>*)(fragment)).zero_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<17>*)(fragment)).zero_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<17>*)(fragment)).zero_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<17>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<17>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<17>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<17>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<17>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<17>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<17>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<17>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<17>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<17>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<17>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<17>*)(fragment)).zero_extend_impl<32>());
        }
        case 18: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<18>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<18>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<18>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<18>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<18>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<18>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<18>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<18>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<18>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<18>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<18>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<18>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<18>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<18>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<18>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<18>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<18>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<18>*)(fragment)));
            case 19: return new bv_frag<19>((*(bv_frag<18>*)(fragment)).zero_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<18>*)(fragment)).zero_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<18>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<18>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<18>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<18>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<18>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<18>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<18>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<18>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<18>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<18>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<18>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<18>*)(fragment)).zero_extend_impl<32>());
        }
        case 19: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<19>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<19>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<19>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<19>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<19>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<19>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<19>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<19>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<19>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<19>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<19>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<19>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<19>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<19>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<19>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<19>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<19>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<19>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<19>*)(fragment)));
            case 20: return new bv_frag<20>((*(bv_frag<19>*)(fragment)).zero_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<19>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<19>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<19>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<19>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<19>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<19>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<19>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<19>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<19>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<19>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<19>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<19>*)(fragment)).zero_extend_impl<32>());
        }
        case 20: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<20>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<20>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<20>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<20>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<20>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<20>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<20>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<20>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<20>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<20>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<20>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<20>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<20>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<20>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<20>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<20>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<20>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<20>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<20>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<20>*)(fragment)));
            case 21: return new bv_frag<21>((*(bv_frag<20>*)(fragment)).zero_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<20>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<20>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<20>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<20>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<20>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<20>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<20>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<20>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<20>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<20>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<20>*)(fragment)).zero_extend_impl<32>());
        }
        case 21: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<21>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<21>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<21>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<21>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<21>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<21>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<21>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<21>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<21>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<21>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<21>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<21>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<21>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<21>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<21>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<21>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<21>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<21>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<21>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<21>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<21>*)(fragment)));
            case 22: return new bv_frag<22>((*(bv_frag<21>*)(fragment)).zero_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<21>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<21>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<21>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<21>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<21>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<21>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<21>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<21>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<21>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<21>*)(fragment)).zero_extend_impl<32>());
        }
        case 22: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<22>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<22>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<22>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<22>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<22>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<22>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<22>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<22>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<22>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<22>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<22>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<22>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<22>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<22>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<22>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<22>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<22>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<22>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<22>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<22>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<22>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<22>*)(fragment)));
            case 23: return new bv_frag<23>((*(bv_frag<22>*)(fragment)).zero_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<22>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<22>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<22>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<22>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<22>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<22>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<22>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<22>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<22>*)(fragment)).zero_extend_impl<32>());
        }
        case 23: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<23>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<23>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<23>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<23>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<23>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<23>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<23>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<23>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<23>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<23>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<23>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<23>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<23>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<23>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<23>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<23>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<23>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<23>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<23>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<23>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<23>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<23>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<23>*)(fragment)));
            case 24: return new bv_frag<24>((*(bv_frag<23>*)(fragment)).zero_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<23>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<23>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<23>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<23>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<23>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<23>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<23>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<23>*)(fragment)).zero_extend_impl<32>());
        }
        case 24: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<24>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<24>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<24>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<24>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<24>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<24>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<24>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<24>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<24>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<24>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<24>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<24>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<24>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<24>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<24>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<24>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<24>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<24>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<24>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<24>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<24>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<24>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<24>*)(fragment)).slice_impl<23>(offset));
            case 24: return new bv_frag<24>((*(bv_frag<24>*)(fragment)));
            case 25: return new bv_frag<25>((*(bv_frag<24>*)(fragment)).zero_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<24>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<24>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<24>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<24>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<24>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<24>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<24>*)(fragment)).zero_extend_impl<32>());
        }
        case 25: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<25>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<25>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<25>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<25>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<25>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<25>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<25>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<25>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<25>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<25>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<25>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<25>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<25>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<25>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<25>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<25>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<25>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<25>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<25>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<25>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<25>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<25>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<25>*)(fragment)).slice_impl<23>(offset));
            case 24: return new bv_frag<24>((*(bv_frag<25>*)(fragment)).slice_impl<24>(offset));
            case 25: return new bv_frag<25>((*(bv_frag<25>*)(fragment)));
            case 26: return new bv_frag<26>((*(bv_frag<25>*)(fragment)).zero_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<25>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<25>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<25>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<25>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<25>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<25>*)(fragment)).zero_extend_impl<32>());
        }
        case 26: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<26>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<26>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<26>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<26>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<26>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<26>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<26>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<26>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<26>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<26>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<26>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<26>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<26>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<26>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<26>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<26>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<26>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<26>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<26>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<26>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<26>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<26>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<26>*)(fragment)).slice_impl<23>(offset));
            case 24: return new bv_frag<24>((*(bv_frag<26>*)(fragment)).slice_impl<24>(offset));
            case 25: return new bv_frag<25>((*(bv_frag<26>*)(fragment)).slice_impl<25>(offset));
            case 26: return new bv_frag<26>((*(bv_frag<26>*)(fragment)));
            case 27: return new bv_frag<27>((*(bv_frag<26>*)(fragment)).zero_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<26>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<26>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<26>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<26>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<26>*)(fragment)).zero_extend_impl<32>());
        }
        case 27: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<27>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<27>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<27>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<27>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<27>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<27>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<27>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<27>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<27>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<27>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<27>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<27>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<27>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<27>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<27>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<27>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<27>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<27>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<27>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<27>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<27>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<27>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<27>*)(fragment)).slice_impl<23>(offset));
            case 24: return new bv_frag<24>((*(bv_frag<27>*)(fragment)).slice_impl<24>(offset));
            case 25: return new bv_frag<25>((*(bv_frag<27>*)(fragment)).slice_impl<25>(offset));
            case 26: return new bv_frag<26>((*(bv_frag<27>*)(fragment)).slice_impl<26>(offset));
            case 27: return new bv_frag<27>((*(bv_frag<27>*)(fragment)));
            case 28: return new bv_frag<28>((*(bv_frag<27>*)(fragment)).zero_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<27>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<27>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<27>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<27>*)(fragment)).zero_extend_impl<32>());
        }
        case 28: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<28>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<28>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<28>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<28>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<28>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<28>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<28>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<28>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<28>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<28>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<28>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<28>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<28>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<28>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<28>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<28>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<28>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<28>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<28>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<28>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<28>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<28>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<28>*)(fragment)).slice_impl<23>(offset));
            case 24: return new bv_frag<24>((*(bv_frag<28>*)(fragment)).slice_impl<24>(offset));
            case 25: return new bv_frag<25>((*(bv_frag<28>*)(fragment)).slice_impl<25>(offset));
            case 26: return new bv_frag<26>((*(bv_frag<28>*)(fragment)).slice_impl<26>(offset));
            case 27: return new bv_frag<27>((*(bv_frag<28>*)(fragment)).slice_impl<27>(offset));
            case 28: return new bv_frag<28>((*(bv_frag<28>*)(fragment)));
            case 29: return new bv_frag<29>((*(bv_frag<28>*)(fragment)).zero_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<28>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<28>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<28>*)(fragment)).zero_extend_impl<32>());
        }
        case 29: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<29>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<29>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<29>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<29>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<29>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<29>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<29>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<29>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<29>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<29>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<29>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<29>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<29>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<29>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<29>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<29>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<29>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<29>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<29>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<29>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<29>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<29>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<29>*)(fragment)).slice_impl<23>(offset));
            case 24: return new bv_frag<24>((*(bv_frag<29>*)(fragment)).slice_impl<24>(offset));
            case 25: return new bv_frag<25>((*(bv_frag<29>*)(fragment)).slice_impl<25>(offset));
            case 26: return new bv_frag<26>((*(bv_frag<29>*)(fragment)).slice_impl<26>(offset));
            case 27: return new bv_frag<27>((*(bv_frag<29>*)(fragment)).slice_impl<27>(offset));
            case 28: return new bv_frag<28>((*(bv_frag<29>*)(fragment)).slice_impl<28>(offset));
            case 29: return new bv_frag<29>((*(bv_frag<29>*)(fragment)));
            case 30: return new bv_frag<30>((*(bv_frag<29>*)(fragment)).zero_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<29>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<29>*)(fragment)).zero_extend_impl<32>());
        }
        case 30: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<30>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<30>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<30>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<30>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<30>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<30>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<30>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<30>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<30>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<30>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<30>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<30>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<30>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<30>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<30>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<30>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<30>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<30>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<30>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<30>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<30>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<30>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<30>*)(fragment)).slice_impl<23>(offset));
            case 24: return new bv_frag<24>((*(bv_frag<30>*)(fragment)).slice_impl<24>(offset));
            case 25: return new bv_frag<25>((*(bv_frag<30>*)(fragment)).slice_impl<25>(offset));
            case 26: return new bv_frag<26>((*(bv_frag<30>*)(fragment)).slice_impl<26>(offset));
            case 27: return new bv_frag<27>((*(bv_frag<30>*)(fragment)).slice_impl<27>(offset));
            case 28: return new bv_frag<28>((*(bv_frag<30>*)(fragment)).slice_impl<28>(offset));
            case 29: return new bv_frag<29>((*(bv_frag<30>*)(fragment)).slice_impl<29>(offset));
            case 30: return new bv_frag<30>((*(bv_frag<30>*)(fragment)));
            case 31: return new bv_frag<31>((*(bv_frag<30>*)(fragment)).zero_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<30>*)(fragment)).zero_extend_impl<32>());
        }
        case 31: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<31>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<31>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<31>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<31>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<31>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<31>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<31>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<31>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<31>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<31>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<31>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<31>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<31>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<31>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<31>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<31>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<31>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<31>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<31>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<31>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<31>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<31>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<31>*)(fragment)).slice_impl<23>(offset));
            case 24: return new bv_frag<24>((*(bv_frag<31>*)(fragment)).slice_impl<24>(offset));
            case 25: return new bv_frag<25>((*(bv_frag<31>*)(fragment)).slice_impl<25>(offset));
            case 26: return new bv_frag<26>((*(bv_frag<31>*)(fragment)).slice_impl<26>(offset));
            case 27: return new bv_frag<27>((*(bv_frag<31>*)(fragment)).slice_impl<27>(offset));
            case 28: return new bv_frag<28>((*(bv_frag<31>*)(fragment)).slice_impl<28>(offset));
            case 29: return new bv_frag<29>((*(bv_frag<31>*)(fragment)).slice_impl<29>(offset));
            case 30: return new bv_frag<30>((*(bv_frag<31>*)(fragment)).slice_impl<30>(offset));
            case 31: return new bv_frag<31>((*(bv_frag<31>*)(fragment)));
            case 32: return new bv_frag<32>((*(bv_frag<31>*)(fragment)).zero_extend_impl<32>());
        }
        case 32: switch (target_size) {
            case 1: return new bv_frag<1>((*(bv_frag<32>*)(fragment)).slice_impl<1>(offset));
            case 2: return new bv_frag<2>((*(bv_frag<32>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<32>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<32>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<32>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<32>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<32>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<32>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<32>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<32>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<32>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<32>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<32>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<32>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<32>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<32>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<32>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<32>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<32>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<32>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<32>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<32>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<32>*)(fragment)).slice_impl<23>(offset));
            case 24: return new bv_frag<24>((*(bv_frag<32>*)(fragment)).slice_impl<24>(offset));
            case 25: return new bv_frag<25>((*(bv_frag<32>*)(fragment)).slice_impl<25>(offset));
            case 26: return new bv_frag<26>((*(bv_frag<32>*)(fragment)).slice_impl<26>(offset));
            case 27: return new bv_frag<27>((*(bv_frag<32>*)(fragment)).slice_impl<27>(offset));
            case 28: return new bv_frag<28>((*(bv_frag<32>*)(fragment)).slice_impl<28>(offset));
            case 29: return new bv_frag<29>((*(bv_frag<32>*)(fragment)).slice_impl<29>(offset));
            case 30: return new bv_frag<30>((*(bv_frag<32>*)(fragment)).slice_impl<30>(offset));
            case 31: return new bv_frag<31>((*(bv_frag<32>*)(fragment)).slice_impl<31>(offset));
            case 32: return new bv_frag<32>((*(bv_frag<32>*)(fragment)));
        }
    }
    printf("bv_fragment_resize failed (%ld -> %ld not implemented)\n", source_size, target_size);
    assert(false);
}
void* bv_fragment_resize_signed(void* fragment, long source_size, long target_size, long offset)
{
    // printf("bv_fragment_resize_signed (%ld -> %ld)\n", source_size, target_size);
    // TODO check if safe
    // if (source_size == target_size)
    //     return fragment;
    switch (source_size) {
        case 2: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<2>*)(fragment)));
            case 3: return new bv_frag<3>((*(bv_frag<2>*)(fragment)).sign_extend_impl<3>());
            case 4: return new bv_frag<4>((*(bv_frag<2>*)(fragment)).sign_extend_impl<4>());
            case 5: return new bv_frag<5>((*(bv_frag<2>*)(fragment)).sign_extend_impl<5>());
            case 6: return new bv_frag<6>((*(bv_frag<2>*)(fragment)).sign_extend_impl<6>());
            case 7: return new bv_frag<7>((*(bv_frag<2>*)(fragment)).sign_extend_impl<7>());
            case 8: return new bv_frag<8>((*(bv_frag<2>*)(fragment)).sign_extend_impl<8>());
            case 9: return new bv_frag<9>((*(bv_frag<2>*)(fragment)).sign_extend_impl<9>());
            case 10: return new bv_frag<10>((*(bv_frag<2>*)(fragment)).sign_extend_impl<10>());
            case 11: return new bv_frag<11>((*(bv_frag<2>*)(fragment)).sign_extend_impl<11>());
            case 12: return new bv_frag<12>((*(bv_frag<2>*)(fragment)).sign_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<2>*)(fragment)).sign_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<2>*)(fragment)).sign_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<2>*)(fragment)).sign_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<2>*)(fragment)).sign_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<2>*)(fragment)).sign_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<2>*)(fragment)).sign_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<2>*)(fragment)).sign_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<2>*)(fragment)).sign_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<2>*)(fragment)).sign_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<2>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<2>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<2>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<2>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<2>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<2>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<2>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<2>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<2>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<2>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<2>*)(fragment)).sign_extend_impl<32>());
        }
        case 3: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<3>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<3>*)(fragment)));
            case 4: return new bv_frag<4>((*(bv_frag<3>*)(fragment)).sign_extend_impl<4>());
            case 5: return new bv_frag<5>((*(bv_frag<3>*)(fragment)).sign_extend_impl<5>());
            case 6: return new bv_frag<6>((*(bv_frag<3>*)(fragment)).sign_extend_impl<6>());
            case 7: return new bv_frag<7>((*(bv_frag<3>*)(fragment)).sign_extend_impl<7>());
            case 8: return new bv_frag<8>((*(bv_frag<3>*)(fragment)).sign_extend_impl<8>());
            case 9: return new bv_frag<9>((*(bv_frag<3>*)(fragment)).sign_extend_impl<9>());
            case 10: return new bv_frag<10>((*(bv_frag<3>*)(fragment)).sign_extend_impl<10>());
            case 11: return new bv_frag<11>((*(bv_frag<3>*)(fragment)).sign_extend_impl<11>());
            case 12: return new bv_frag<12>((*(bv_frag<3>*)(fragment)).sign_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<3>*)(fragment)).sign_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<3>*)(fragment)).sign_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<3>*)(fragment)).sign_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<3>*)(fragment)).sign_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<3>*)(fragment)).sign_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<3>*)(fragment)).sign_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<3>*)(fragment)).sign_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<3>*)(fragment)).sign_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<3>*)(fragment)).sign_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<3>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<3>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<3>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<3>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<3>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<3>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<3>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<3>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<3>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<3>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<3>*)(fragment)).sign_extend_impl<32>());
        }
        case 4: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<4>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<4>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<4>*)(fragment)));
            case 5: return new bv_frag<5>((*(bv_frag<4>*)(fragment)).sign_extend_impl<5>());
            case 6: return new bv_frag<6>((*(bv_frag<4>*)(fragment)).sign_extend_impl<6>());
            case 7: return new bv_frag<7>((*(bv_frag<4>*)(fragment)).sign_extend_impl<7>());
            case 8: return new bv_frag<8>((*(bv_frag<4>*)(fragment)).sign_extend_impl<8>());
            case 9: return new bv_frag<9>((*(bv_frag<4>*)(fragment)).sign_extend_impl<9>());
            case 10: return new bv_frag<10>((*(bv_frag<4>*)(fragment)).sign_extend_impl<10>());
            case 11: return new bv_frag<11>((*(bv_frag<4>*)(fragment)).sign_extend_impl<11>());
            case 12: return new bv_frag<12>((*(bv_frag<4>*)(fragment)).sign_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<4>*)(fragment)).sign_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<4>*)(fragment)).sign_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<4>*)(fragment)).sign_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<4>*)(fragment)).sign_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<4>*)(fragment)).sign_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<4>*)(fragment)).sign_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<4>*)(fragment)).sign_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<4>*)(fragment)).sign_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<4>*)(fragment)).sign_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<4>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<4>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<4>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<4>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<4>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<4>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<4>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<4>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<4>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<4>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<4>*)(fragment)).sign_extend_impl<32>());
        }
        case 5: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<5>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<5>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<5>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<5>*)(fragment)));
            case 6: return new bv_frag<6>((*(bv_frag<5>*)(fragment)).sign_extend_impl<6>());
            case 7: return new bv_frag<7>((*(bv_frag<5>*)(fragment)).sign_extend_impl<7>());
            case 8: return new bv_frag<8>((*(bv_frag<5>*)(fragment)).sign_extend_impl<8>());
            case 9: return new bv_frag<9>((*(bv_frag<5>*)(fragment)).sign_extend_impl<9>());
            case 10: return new bv_frag<10>((*(bv_frag<5>*)(fragment)).sign_extend_impl<10>());
            case 11: return new bv_frag<11>((*(bv_frag<5>*)(fragment)).sign_extend_impl<11>());
            case 12: return new bv_frag<12>((*(bv_frag<5>*)(fragment)).sign_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<5>*)(fragment)).sign_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<5>*)(fragment)).sign_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<5>*)(fragment)).sign_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<5>*)(fragment)).sign_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<5>*)(fragment)).sign_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<5>*)(fragment)).sign_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<5>*)(fragment)).sign_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<5>*)(fragment)).sign_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<5>*)(fragment)).sign_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<5>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<5>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<5>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<5>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<5>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<5>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<5>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<5>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<5>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<5>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<5>*)(fragment)).sign_extend_impl<32>());
        }
        case 6: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<6>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<6>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<6>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<6>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<6>*)(fragment)));
            case 7: return new bv_frag<7>((*(bv_frag<6>*)(fragment)).sign_extend_impl<7>());
            case 8: return new bv_frag<8>((*(bv_frag<6>*)(fragment)).sign_extend_impl<8>());
            case 9: return new bv_frag<9>((*(bv_frag<6>*)(fragment)).sign_extend_impl<9>());
            case 10: return new bv_frag<10>((*(bv_frag<6>*)(fragment)).sign_extend_impl<10>());
            case 11: return new bv_frag<11>((*(bv_frag<6>*)(fragment)).sign_extend_impl<11>());
            case 12: return new bv_frag<12>((*(bv_frag<6>*)(fragment)).sign_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<6>*)(fragment)).sign_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<6>*)(fragment)).sign_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<6>*)(fragment)).sign_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<6>*)(fragment)).sign_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<6>*)(fragment)).sign_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<6>*)(fragment)).sign_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<6>*)(fragment)).sign_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<6>*)(fragment)).sign_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<6>*)(fragment)).sign_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<6>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<6>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<6>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<6>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<6>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<6>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<6>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<6>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<6>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<6>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<6>*)(fragment)).sign_extend_impl<32>());
        }
        case 7: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<7>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<7>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<7>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<7>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<7>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<7>*)(fragment)));
            case 8: return new bv_frag<8>((*(bv_frag<7>*)(fragment)).sign_extend_impl<8>());
            case 9: return new bv_frag<9>((*(bv_frag<7>*)(fragment)).sign_extend_impl<9>());
            case 10: return new bv_frag<10>((*(bv_frag<7>*)(fragment)).sign_extend_impl<10>());
            case 11: return new bv_frag<11>((*(bv_frag<7>*)(fragment)).sign_extend_impl<11>());
            case 12: return new bv_frag<12>((*(bv_frag<7>*)(fragment)).sign_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<7>*)(fragment)).sign_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<7>*)(fragment)).sign_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<7>*)(fragment)).sign_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<7>*)(fragment)).sign_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<7>*)(fragment)).sign_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<7>*)(fragment)).sign_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<7>*)(fragment)).sign_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<7>*)(fragment)).sign_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<7>*)(fragment)).sign_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<7>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<7>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<7>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<7>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<7>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<7>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<7>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<7>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<7>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<7>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<7>*)(fragment)).sign_extend_impl<32>());
        }
        case 8: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<8>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<8>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<8>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<8>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<8>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<8>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<8>*)(fragment)));
            case 9: return new bv_frag<9>((*(bv_frag<8>*)(fragment)).sign_extend_impl<9>());
            case 10: return new bv_frag<10>((*(bv_frag<8>*)(fragment)).sign_extend_impl<10>());
            case 11: return new bv_frag<11>((*(bv_frag<8>*)(fragment)).sign_extend_impl<11>());
            case 12: return new bv_frag<12>((*(bv_frag<8>*)(fragment)).sign_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<8>*)(fragment)).sign_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<8>*)(fragment)).sign_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<8>*)(fragment)).sign_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<8>*)(fragment)).sign_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<8>*)(fragment)).sign_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<8>*)(fragment)).sign_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<8>*)(fragment)).sign_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<8>*)(fragment)).sign_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<8>*)(fragment)).sign_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<8>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<8>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<8>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<8>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<8>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<8>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<8>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<8>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<8>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<8>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<8>*)(fragment)).sign_extend_impl<32>());
        }
        case 9: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<9>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<9>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<9>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<9>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<9>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<9>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<9>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<9>*)(fragment)));
            case 10: return new bv_frag<10>((*(bv_frag<9>*)(fragment)).sign_extend_impl<10>());
            case 11: return new bv_frag<11>((*(bv_frag<9>*)(fragment)).sign_extend_impl<11>());
            case 12: return new bv_frag<12>((*(bv_frag<9>*)(fragment)).sign_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<9>*)(fragment)).sign_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<9>*)(fragment)).sign_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<9>*)(fragment)).sign_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<9>*)(fragment)).sign_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<9>*)(fragment)).sign_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<9>*)(fragment)).sign_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<9>*)(fragment)).sign_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<9>*)(fragment)).sign_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<9>*)(fragment)).sign_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<9>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<9>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<9>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<9>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<9>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<9>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<9>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<9>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<9>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<9>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<9>*)(fragment)).sign_extend_impl<32>());
        }
        case 10: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<10>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<10>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<10>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<10>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<10>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<10>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<10>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<10>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<10>*)(fragment)));
            case 11: return new bv_frag<11>((*(bv_frag<10>*)(fragment)).sign_extend_impl<11>());
            case 12: return new bv_frag<12>((*(bv_frag<10>*)(fragment)).sign_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<10>*)(fragment)).sign_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<10>*)(fragment)).sign_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<10>*)(fragment)).sign_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<10>*)(fragment)).sign_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<10>*)(fragment)).sign_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<10>*)(fragment)).sign_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<10>*)(fragment)).sign_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<10>*)(fragment)).sign_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<10>*)(fragment)).sign_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<10>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<10>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<10>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<10>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<10>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<10>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<10>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<10>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<10>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<10>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<10>*)(fragment)).sign_extend_impl<32>());
        }
        case 11: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<11>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<11>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<11>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<11>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<11>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<11>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<11>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<11>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<11>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<11>*)(fragment)));
            case 12: return new bv_frag<12>((*(bv_frag<11>*)(fragment)).sign_extend_impl<12>());
            case 13: return new bv_frag<13>((*(bv_frag<11>*)(fragment)).sign_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<11>*)(fragment)).sign_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<11>*)(fragment)).sign_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<11>*)(fragment)).sign_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<11>*)(fragment)).sign_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<11>*)(fragment)).sign_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<11>*)(fragment)).sign_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<11>*)(fragment)).sign_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<11>*)(fragment)).sign_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<11>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<11>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<11>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<11>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<11>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<11>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<11>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<11>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<11>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<11>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<11>*)(fragment)).sign_extend_impl<32>());
        }
        case 12: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<12>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<12>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<12>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<12>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<12>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<12>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<12>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<12>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<12>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<12>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<12>*)(fragment)));
            case 13: return new bv_frag<13>((*(bv_frag<12>*)(fragment)).sign_extend_impl<13>());
            case 14: return new bv_frag<14>((*(bv_frag<12>*)(fragment)).sign_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<12>*)(fragment)).sign_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<12>*)(fragment)).sign_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<12>*)(fragment)).sign_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<12>*)(fragment)).sign_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<12>*)(fragment)).sign_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<12>*)(fragment)).sign_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<12>*)(fragment)).sign_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<12>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<12>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<12>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<12>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<12>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<12>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<12>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<12>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<12>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<12>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<12>*)(fragment)).sign_extend_impl<32>());
        }
        case 13: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<13>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<13>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<13>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<13>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<13>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<13>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<13>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<13>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<13>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<13>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<13>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<13>*)(fragment)));
            case 14: return new bv_frag<14>((*(bv_frag<13>*)(fragment)).sign_extend_impl<14>());
            case 15: return new bv_frag<15>((*(bv_frag<13>*)(fragment)).sign_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<13>*)(fragment)).sign_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<13>*)(fragment)).sign_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<13>*)(fragment)).sign_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<13>*)(fragment)).sign_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<13>*)(fragment)).sign_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<13>*)(fragment)).sign_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<13>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<13>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<13>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<13>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<13>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<13>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<13>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<13>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<13>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<13>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<13>*)(fragment)).sign_extend_impl<32>());
        }
        case 14: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<14>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<14>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<14>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<14>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<14>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<14>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<14>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<14>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<14>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<14>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<14>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<14>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<14>*)(fragment)));
            case 15: return new bv_frag<15>((*(bv_frag<14>*)(fragment)).sign_extend_impl<15>());
            case 16: return new bv_frag<16>((*(bv_frag<14>*)(fragment)).sign_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<14>*)(fragment)).sign_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<14>*)(fragment)).sign_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<14>*)(fragment)).sign_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<14>*)(fragment)).sign_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<14>*)(fragment)).sign_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<14>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<14>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<14>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<14>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<14>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<14>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<14>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<14>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<14>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<14>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<14>*)(fragment)).sign_extend_impl<32>());
        }
        case 15: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<15>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<15>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<15>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<15>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<15>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<15>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<15>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<15>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<15>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<15>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<15>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<15>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<15>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<15>*)(fragment)));
            case 16: return new bv_frag<16>((*(bv_frag<15>*)(fragment)).sign_extend_impl<16>());
            case 17: return new bv_frag<17>((*(bv_frag<15>*)(fragment)).sign_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<15>*)(fragment)).sign_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<15>*)(fragment)).sign_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<15>*)(fragment)).sign_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<15>*)(fragment)).sign_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<15>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<15>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<15>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<15>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<15>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<15>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<15>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<15>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<15>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<15>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<15>*)(fragment)).sign_extend_impl<32>());
        }
        case 16: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<16>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<16>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<16>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<16>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<16>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<16>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<16>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<16>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<16>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<16>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<16>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<16>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<16>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<16>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<16>*)(fragment)));
            case 17: return new bv_frag<17>((*(bv_frag<16>*)(fragment)).sign_extend_impl<17>());
            case 18: return new bv_frag<18>((*(bv_frag<16>*)(fragment)).sign_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<16>*)(fragment)).sign_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<16>*)(fragment)).sign_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<16>*)(fragment)).sign_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<16>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<16>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<16>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<16>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<16>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<16>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<16>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<16>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<16>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<16>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<16>*)(fragment)).sign_extend_impl<32>());
        }
        case 17: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<17>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<17>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<17>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<17>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<17>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<17>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<17>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<17>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<17>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<17>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<17>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<17>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<17>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<17>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<17>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<17>*)(fragment)));
            case 18: return new bv_frag<18>((*(bv_frag<17>*)(fragment)).sign_extend_impl<18>());
            case 19: return new bv_frag<19>((*(bv_frag<17>*)(fragment)).sign_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<17>*)(fragment)).sign_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<17>*)(fragment)).sign_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<17>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<17>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<17>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<17>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<17>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<17>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<17>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<17>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<17>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<17>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<17>*)(fragment)).sign_extend_impl<32>());
        }
        case 18: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<18>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<18>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<18>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<18>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<18>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<18>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<18>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<18>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<18>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<18>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<18>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<18>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<18>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<18>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<18>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<18>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<18>*)(fragment)));
            case 19: return new bv_frag<19>((*(bv_frag<18>*)(fragment)).sign_extend_impl<19>());
            case 20: return new bv_frag<20>((*(bv_frag<18>*)(fragment)).sign_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<18>*)(fragment)).sign_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<18>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<18>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<18>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<18>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<18>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<18>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<18>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<18>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<18>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<18>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<18>*)(fragment)).sign_extend_impl<32>());
        }
        case 19: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<19>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<19>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<19>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<19>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<19>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<19>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<19>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<19>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<19>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<19>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<19>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<19>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<19>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<19>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<19>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<19>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<19>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<19>*)(fragment)));
            case 20: return new bv_frag<20>((*(bv_frag<19>*)(fragment)).sign_extend_impl<20>());
            case 21: return new bv_frag<21>((*(bv_frag<19>*)(fragment)).sign_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<19>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<19>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<19>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<19>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<19>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<19>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<19>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<19>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<19>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<19>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<19>*)(fragment)).sign_extend_impl<32>());
        }
        case 20: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<20>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<20>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<20>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<20>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<20>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<20>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<20>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<20>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<20>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<20>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<20>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<20>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<20>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<20>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<20>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<20>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<20>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<20>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<20>*)(fragment)));
            case 21: return new bv_frag<21>((*(bv_frag<20>*)(fragment)).sign_extend_impl<21>());
            case 22: return new bv_frag<22>((*(bv_frag<20>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<20>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<20>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<20>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<20>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<20>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<20>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<20>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<20>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<20>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<20>*)(fragment)).sign_extend_impl<32>());
        }
        case 21: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<21>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<21>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<21>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<21>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<21>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<21>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<21>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<21>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<21>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<21>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<21>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<21>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<21>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<21>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<21>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<21>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<21>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<21>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<21>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<21>*)(fragment)));
            case 22: return new bv_frag<22>((*(bv_frag<21>*)(fragment)).sign_extend_impl<22>());
            case 23: return new bv_frag<23>((*(bv_frag<21>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<21>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<21>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<21>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<21>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<21>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<21>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<21>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<21>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<21>*)(fragment)).sign_extend_impl<32>());
        }
        case 22: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<22>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<22>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<22>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<22>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<22>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<22>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<22>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<22>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<22>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<22>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<22>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<22>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<22>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<22>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<22>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<22>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<22>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<22>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<22>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<22>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<22>*)(fragment)));
            case 23: return new bv_frag<23>((*(bv_frag<22>*)(fragment)).sign_extend_impl<23>());
            case 24: return new bv_frag<24>((*(bv_frag<22>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<22>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<22>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<22>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<22>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<22>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<22>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<22>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<22>*)(fragment)).sign_extend_impl<32>());
        }
        case 23: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<23>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<23>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<23>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<23>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<23>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<23>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<23>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<23>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<23>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<23>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<23>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<23>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<23>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<23>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<23>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<23>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<23>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<23>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<23>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<23>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<23>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<23>*)(fragment)));
            case 24: return new bv_frag<24>((*(bv_frag<23>*)(fragment)).sign_extend_impl<24>());
            case 25: return new bv_frag<25>((*(bv_frag<23>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<23>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<23>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<23>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<23>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<23>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<23>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<23>*)(fragment)).sign_extend_impl<32>());
        }
        case 24: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<24>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<24>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<24>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<24>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<24>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<24>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<24>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<24>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<24>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<24>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<24>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<24>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<24>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<24>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<24>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<24>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<24>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<24>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<24>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<24>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<24>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<24>*)(fragment)).slice_impl<23>(offset));
            case 24: return new bv_frag<24>((*(bv_frag<24>*)(fragment)));
            case 25: return new bv_frag<25>((*(bv_frag<24>*)(fragment)).sign_extend_impl<25>());
            case 26: return new bv_frag<26>((*(bv_frag<24>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<24>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<24>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<24>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<24>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<24>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<24>*)(fragment)).sign_extend_impl<32>());
        }
        case 25: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<25>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<25>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<25>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<25>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<25>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<25>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<25>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<25>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<25>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<25>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<25>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<25>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<25>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<25>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<25>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<25>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<25>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<25>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<25>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<25>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<25>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<25>*)(fragment)).slice_impl<23>(offset));
            case 24: return new bv_frag<24>((*(bv_frag<25>*)(fragment)).slice_impl<24>(offset));
            case 25: return new bv_frag<25>((*(bv_frag<25>*)(fragment)));
            case 26: return new bv_frag<26>((*(bv_frag<25>*)(fragment)).sign_extend_impl<26>());
            case 27: return new bv_frag<27>((*(bv_frag<25>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<25>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<25>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<25>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<25>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<25>*)(fragment)).sign_extend_impl<32>());
        }
        case 26: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<26>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<26>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<26>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<26>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<26>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<26>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<26>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<26>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<26>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<26>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<26>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<26>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<26>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<26>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<26>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<26>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<26>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<26>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<26>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<26>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<26>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<26>*)(fragment)).slice_impl<23>(offset));
            case 24: return new bv_frag<24>((*(bv_frag<26>*)(fragment)).slice_impl<24>(offset));
            case 25: return new bv_frag<25>((*(bv_frag<26>*)(fragment)).slice_impl<25>(offset));
            case 26: return new bv_frag<26>((*(bv_frag<26>*)(fragment)));
            case 27: return new bv_frag<27>((*(bv_frag<26>*)(fragment)).sign_extend_impl<27>());
            case 28: return new bv_frag<28>((*(bv_frag<26>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<26>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<26>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<26>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<26>*)(fragment)).sign_extend_impl<32>());
        }
        case 27: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<27>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<27>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<27>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<27>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<27>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<27>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<27>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<27>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<27>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<27>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<27>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<27>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<27>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<27>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<27>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<27>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<27>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<27>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<27>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<27>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<27>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<27>*)(fragment)).slice_impl<23>(offset));
            case 24: return new bv_frag<24>((*(bv_frag<27>*)(fragment)).slice_impl<24>(offset));
            case 25: return new bv_frag<25>((*(bv_frag<27>*)(fragment)).slice_impl<25>(offset));
            case 26: return new bv_frag<26>((*(bv_frag<27>*)(fragment)).slice_impl<26>(offset));
            case 27: return new bv_frag<27>((*(bv_frag<27>*)(fragment)));
            case 28: return new bv_frag<28>((*(bv_frag<27>*)(fragment)).sign_extend_impl<28>());
            case 29: return new bv_frag<29>((*(bv_frag<27>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<27>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<27>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<27>*)(fragment)).sign_extend_impl<32>());
        }
        case 28: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<28>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<28>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<28>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<28>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<28>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<28>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<28>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<28>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<28>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<28>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<28>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<28>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<28>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<28>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<28>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<28>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<28>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<28>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<28>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<28>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<28>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<28>*)(fragment)).slice_impl<23>(offset));
            case 24: return new bv_frag<24>((*(bv_frag<28>*)(fragment)).slice_impl<24>(offset));
            case 25: return new bv_frag<25>((*(bv_frag<28>*)(fragment)).slice_impl<25>(offset));
            case 26: return new bv_frag<26>((*(bv_frag<28>*)(fragment)).slice_impl<26>(offset));
            case 27: return new bv_frag<27>((*(bv_frag<28>*)(fragment)).slice_impl<27>(offset));
            case 28: return new bv_frag<28>((*(bv_frag<28>*)(fragment)));
            case 29: return new bv_frag<29>((*(bv_frag<28>*)(fragment)).sign_extend_impl<29>());
            case 30: return new bv_frag<30>((*(bv_frag<28>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<28>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<28>*)(fragment)).sign_extend_impl<32>());
        }
        case 29: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<29>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<29>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<29>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<29>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<29>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<29>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<29>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<29>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<29>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<29>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<29>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<29>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<29>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<29>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<29>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<29>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<29>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<29>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<29>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<29>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<29>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<29>*)(fragment)).slice_impl<23>(offset));
            case 24: return new bv_frag<24>((*(bv_frag<29>*)(fragment)).slice_impl<24>(offset));
            case 25: return new bv_frag<25>((*(bv_frag<29>*)(fragment)).slice_impl<25>(offset));
            case 26: return new bv_frag<26>((*(bv_frag<29>*)(fragment)).slice_impl<26>(offset));
            case 27: return new bv_frag<27>((*(bv_frag<29>*)(fragment)).slice_impl<27>(offset));
            case 28: return new bv_frag<28>((*(bv_frag<29>*)(fragment)).slice_impl<28>(offset));
            case 29: return new bv_frag<29>((*(bv_frag<29>*)(fragment)));
            case 30: return new bv_frag<30>((*(bv_frag<29>*)(fragment)).sign_extend_impl<30>());
            case 31: return new bv_frag<31>((*(bv_frag<29>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<29>*)(fragment)).sign_extend_impl<32>());
        }
        case 30: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<30>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<30>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<30>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<30>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<30>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<30>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<30>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<30>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<30>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<30>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<30>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<30>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<30>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<30>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<30>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<30>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<30>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<30>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<30>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<30>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<30>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<30>*)(fragment)).slice_impl<23>(offset));
            case 24: return new bv_frag<24>((*(bv_frag<30>*)(fragment)).slice_impl<24>(offset));
            case 25: return new bv_frag<25>((*(bv_frag<30>*)(fragment)).slice_impl<25>(offset));
            case 26: return new bv_frag<26>((*(bv_frag<30>*)(fragment)).slice_impl<26>(offset));
            case 27: return new bv_frag<27>((*(bv_frag<30>*)(fragment)).slice_impl<27>(offset));
            case 28: return new bv_frag<28>((*(bv_frag<30>*)(fragment)).slice_impl<28>(offset));
            case 29: return new bv_frag<29>((*(bv_frag<30>*)(fragment)).slice_impl<29>(offset));
            case 30: return new bv_frag<30>((*(bv_frag<30>*)(fragment)));
            case 31: return new bv_frag<31>((*(bv_frag<30>*)(fragment)).sign_extend_impl<31>());
            case 32: return new bv_frag<32>((*(bv_frag<30>*)(fragment)).sign_extend_impl<32>());
        }
        case 31: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<31>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<31>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<31>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<31>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<31>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<31>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<31>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<31>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<31>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<31>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<31>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<31>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<31>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<31>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<31>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<31>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<31>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<31>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<31>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<31>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<31>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<31>*)(fragment)).slice_impl<23>(offset));
            case 24: return new bv_frag<24>((*(bv_frag<31>*)(fragment)).slice_impl<24>(offset));
            case 25: return new bv_frag<25>((*(bv_frag<31>*)(fragment)).slice_impl<25>(offset));
            case 26: return new bv_frag<26>((*(bv_frag<31>*)(fragment)).slice_impl<26>(offset));
            case 27: return new bv_frag<27>((*(bv_frag<31>*)(fragment)).slice_impl<27>(offset));
            case 28: return new bv_frag<28>((*(bv_frag<31>*)(fragment)).slice_impl<28>(offset));
            case 29: return new bv_frag<29>((*(bv_frag<31>*)(fragment)).slice_impl<29>(offset));
            case 30: return new bv_frag<30>((*(bv_frag<31>*)(fragment)).slice_impl<30>(offset));
            case 31: return new bv_frag<31>((*(bv_frag<31>*)(fragment)));
            case 32: return new bv_frag<32>((*(bv_frag<31>*)(fragment)).sign_extend_impl<32>());
        }
        case 32: switch (target_size) {
            case 2: return new bv_frag<2>((*(bv_frag<32>*)(fragment)).slice_impl<2>(offset));
            case 3: return new bv_frag<3>((*(bv_frag<32>*)(fragment)).slice_impl<3>(offset));
            case 4: return new bv_frag<4>((*(bv_frag<32>*)(fragment)).slice_impl<4>(offset));
            case 5: return new bv_frag<5>((*(bv_frag<32>*)(fragment)).slice_impl<5>(offset));
            case 6: return new bv_frag<6>((*(bv_frag<32>*)(fragment)).slice_impl<6>(offset));
            case 7: return new bv_frag<7>((*(bv_frag<32>*)(fragment)).slice_impl<7>(offset));
            case 8: return new bv_frag<8>((*(bv_frag<32>*)(fragment)).slice_impl<8>(offset));
            case 9: return new bv_frag<9>((*(bv_frag<32>*)(fragment)).slice_impl<9>(offset));
            case 10: return new bv_frag<10>((*(bv_frag<32>*)(fragment)).slice_impl<10>(offset));
            case 11: return new bv_frag<11>((*(bv_frag<32>*)(fragment)).slice_impl<11>(offset));
            case 12: return new bv_frag<12>((*(bv_frag<32>*)(fragment)).slice_impl<12>(offset));
            case 13: return new bv_frag<13>((*(bv_frag<32>*)(fragment)).slice_impl<13>(offset));
            case 14: return new bv_frag<14>((*(bv_frag<32>*)(fragment)).slice_impl<14>(offset));
            case 15: return new bv_frag<15>((*(bv_frag<32>*)(fragment)).slice_impl<15>(offset));
            case 16: return new bv_frag<16>((*(bv_frag<32>*)(fragment)).slice_impl<16>(offset));
            case 17: return new bv_frag<17>((*(bv_frag<32>*)(fragment)).slice_impl<17>(offset));
            case 18: return new bv_frag<18>((*(bv_frag<32>*)(fragment)).slice_impl<18>(offset));
            case 19: return new bv_frag<19>((*(bv_frag<32>*)(fragment)).slice_impl<19>(offset));
            case 20: return new bv_frag<20>((*(bv_frag<32>*)(fragment)).slice_impl<20>(offset));
            case 21: return new bv_frag<21>((*(bv_frag<32>*)(fragment)).slice_impl<21>(offset));
            case 22: return new bv_frag<22>((*(bv_frag<32>*)(fragment)).slice_impl<22>(offset));
            case 23: return new bv_frag<23>((*(bv_frag<32>*)(fragment)).slice_impl<23>(offset));
            case 24: return new bv_frag<24>((*(bv_frag<32>*)(fragment)).slice_impl<24>(offset));
            case 25: return new bv_frag<25>((*(bv_frag<32>*)(fragment)).slice_impl<25>(offset));
            case 26: return new bv_frag<26>((*(bv_frag<32>*)(fragment)).slice_impl<26>(offset));
            case 27: return new bv_frag<27>((*(bv_frag<32>*)(fragment)).slice_impl<27>(offset));
            case 28: return new bv_frag<28>((*(bv_frag<32>*)(fragment)).slice_impl<28>(offset));
            case 29: return new bv_frag<29>((*(bv_frag<32>*)(fragment)).slice_impl<29>(offset));
            case 30: return new bv_frag<30>((*(bv_frag<32>*)(fragment)).slice_impl<30>(offset));
            case 31: return new bv_frag<31>((*(bv_frag<32>*)(fragment)).slice_impl<31>(offset));
            case 32: return new bv_frag<32>((*(bv_frag<32>*)(fragment)));
        }
    }
    printf("bv_fragment_resize_signed failed (%ld -> %ld not implemented)\n", source_size, target_size);
    assert(false);
}
void* bv_fragment_op(bv_op op, long size, void* a, void* b)
{
    switch (size) {
        case 1: switch (op) {
            case bv_op::AND: return new bv_frag<1>(*((bv_frag<1>*)a) & *((bv_frag<1>*)b));
            case bv_op::OR : return new bv_frag<1>(*((bv_frag<1>*)a) | *((bv_frag<1>*)b));
            case bv_op::XOR: return new bv_frag<1>(*((bv_frag<1>*)a) ^ *((bv_frag<1>*)b));
        }
        case 2: switch (op) {
            case bv_op::AND: return new bv_frag<2>(*((bv_frag<2>*)a) & *((bv_frag<2>*)b));
            case bv_op::OR : return new bv_frag<2>(*((bv_frag<2>*)a) | *((bv_frag<2>*)b));
            case bv_op::XOR: return new bv_frag<2>(*((bv_frag<2>*)a) ^ *((bv_frag<2>*)b));
        }
        case 3: switch (op) {
            case bv_op::AND: return new bv_frag<3>(*((bv_frag<3>*)a) & *((bv_frag<3>*)b));
            case bv_op::OR : return new bv_frag<3>(*((bv_frag<3>*)a) | *((bv_frag<3>*)b));
            case bv_op::XOR: return new bv_frag<3>(*((bv_frag<3>*)a) ^ *((bv_frag<3>*)b));
        }
        case 4: switch (op) {
            case bv_op::AND: return new bv_frag<4>(*((bv_frag<4>*)a) & *((bv_frag<4>*)b));
            case bv_op::OR : return new bv_frag<4>(*((bv_frag<4>*)a) | *((bv_frag<4>*)b));
            case bv_op::XOR: return new bv_frag<4>(*((bv_frag<4>*)a) ^ *((bv_frag<4>*)b));
        }
        case 5: switch (op) {
            case bv_op::AND: return new bv_frag<5>(*((bv_frag<5>*)a) & *((bv_frag<5>*)b));
            case bv_op::OR : return new bv_frag<5>(*((bv_frag<5>*)a) | *((bv_frag<5>*)b));
            case bv_op::XOR: return new bv_frag<5>(*((bv_frag<5>*)a) ^ *((bv_frag<5>*)b));
        }
        case 6: switch (op) {
            case bv_op::AND: return new bv_frag<6>(*((bv_frag<6>*)a) & *((bv_frag<6>*)b));
            case bv_op::OR : return new bv_frag<6>(*((bv_frag<6>*)a) | *((bv_frag<6>*)b));
            case bv_op::XOR: return new bv_frag<6>(*((bv_frag<6>*)a) ^ *((bv_frag<6>*)b));
        }
        case 7: switch (op) {
            case bv_op::AND: return new bv_frag<7>(*((bv_frag<7>*)a) & *((bv_frag<7>*)b));
            case bv_op::OR : return new bv_frag<7>(*((bv_frag<7>*)a) | *((bv_frag<7>*)b));
            case bv_op::XOR: return new bv_frag<7>(*((bv_frag<7>*)a) ^ *((bv_frag<7>*)b));
        }
        case 8: switch (op) {
            case bv_op::AND: return new bv_frag<8>(*((bv_frag<8>*)a) & *((bv_frag<8>*)b));
            case bv_op::OR : return new bv_frag<8>(*((bv_frag<8>*)a) | *((bv_frag<8>*)b));
            case bv_op::XOR: return new bv_frag<8>(*((bv_frag<8>*)a) ^ *((bv_frag<8>*)b));
        }
        case 9: switch (op) {
            case bv_op::AND: return new bv_frag<9>(*((bv_frag<9>*)a) & *((bv_frag<9>*)b));
            case bv_op::OR : return new bv_frag<9>(*((bv_frag<9>*)a) | *((bv_frag<9>*)b));
            case bv_op::XOR: return new bv_frag<9>(*((bv_frag<9>*)a) ^ *((bv_frag<9>*)b));
        }
        case 10: switch (op) {
            case bv_op::AND: return new bv_frag<10>(*((bv_frag<10>*)a) & *((bv_frag<10>*)b));
            case bv_op::OR : return new bv_frag<10>(*((bv_frag<10>*)a) | *((bv_frag<10>*)b));
            case bv_op::XOR: return new bv_frag<10>(*((bv_frag<10>*)a) ^ *((bv_frag<10>*)b));
        }
        case 11: switch (op) {
            case bv_op::AND: return new bv_frag<11>(*((bv_frag<11>*)a) & *((bv_frag<11>*)b));
            case bv_op::OR : return new bv_frag<11>(*((bv_frag<11>*)a) | *((bv_frag<11>*)b));
            case bv_op::XOR: return new bv_frag<11>(*((bv_frag<11>*)a) ^ *((bv_frag<11>*)b));
        }
        case 12: switch (op) {
            case bv_op::AND: return new bv_frag<12>(*((bv_frag<12>*)a) & *((bv_frag<12>*)b));
            case bv_op::OR : return new bv_frag<12>(*((bv_frag<12>*)a) | *((bv_frag<12>*)b));
            case bv_op::XOR: return new bv_frag<12>(*((bv_frag<12>*)a) ^ *((bv_frag<12>*)b));
        }
        case 13: switch (op) {
            case bv_op::AND: return new bv_frag<13>(*((bv_frag<13>*)a) & *((bv_frag<13>*)b));
            case bv_op::OR : return new bv_frag<13>(*((bv_frag<13>*)a) | *((bv_frag<13>*)b));
            case bv_op::XOR: return new bv_frag<13>(*((bv_frag<13>*)a) ^ *((bv_frag<13>*)b));
        }
        case 14: switch (op) {
            case bv_op::AND: return new bv_frag<14>(*((bv_frag<14>*)a) & *((bv_frag<14>*)b));
            case bv_op::OR : return new bv_frag<14>(*((bv_frag<14>*)a) | *((bv_frag<14>*)b));
            case bv_op::XOR: return new bv_frag<14>(*((bv_frag<14>*)a) ^ *((bv_frag<14>*)b));
        }
        case 15: switch (op) {
            case bv_op::AND: return new bv_frag<15>(*((bv_frag<15>*)a) & *((bv_frag<15>*)b));
            case bv_op::OR : return new bv_frag<15>(*((bv_frag<15>*)a) | *((bv_frag<15>*)b));
            case bv_op::XOR: return new bv_frag<15>(*((bv_frag<15>*)a) ^ *((bv_frag<15>*)b));
        }
        case 16: switch (op) {
            case bv_op::AND: return new bv_frag<16>(*((bv_frag<16>*)a) & *((bv_frag<16>*)b));
            case bv_op::OR : return new bv_frag<16>(*((bv_frag<16>*)a) | *((bv_frag<16>*)b));
            case bv_op::XOR: return new bv_frag<16>(*((bv_frag<16>*)a) ^ *((bv_frag<16>*)b));
        }
        case 17: switch (op) {
            case bv_op::AND: return new bv_frag<17>(*((bv_frag<17>*)a) & *((bv_frag<17>*)b));
            case bv_op::OR : return new bv_frag<17>(*((bv_frag<17>*)a) | *((bv_frag<17>*)b));
            case bv_op::XOR: return new bv_frag<17>(*((bv_frag<17>*)a) ^ *((bv_frag<17>*)b));
        }
        case 18: switch (op) {
            case bv_op::AND: return new bv_frag<18>(*((bv_frag<18>*)a) & *((bv_frag<18>*)b));
            case bv_op::OR : return new bv_frag<18>(*((bv_frag<18>*)a) | *((bv_frag<18>*)b));
            case bv_op::XOR: return new bv_frag<18>(*((bv_frag<18>*)a) ^ *((bv_frag<18>*)b));
        }
        case 19: switch (op) {
            case bv_op::AND: return new bv_frag<19>(*((bv_frag<19>*)a) & *((bv_frag<19>*)b));
            case bv_op::OR : return new bv_frag<19>(*((bv_frag<19>*)a) | *((bv_frag<19>*)b));
            case bv_op::XOR: return new bv_frag<19>(*((bv_frag<19>*)a) ^ *((bv_frag<19>*)b));
        }
        case 20: switch (op) {
            case bv_op::AND: return new bv_frag<20>(*((bv_frag<20>*)a) & *((bv_frag<20>*)b));
            case bv_op::OR : return new bv_frag<20>(*((bv_frag<20>*)a) | *((bv_frag<20>*)b));
            case bv_op::XOR: return new bv_frag<20>(*((bv_frag<20>*)a) ^ *((bv_frag<20>*)b));
        }
        case 21: switch (op) {
            case bv_op::AND: return new bv_frag<21>(*((bv_frag<21>*)a) & *((bv_frag<21>*)b));
            case bv_op::OR : return new bv_frag<21>(*((bv_frag<21>*)a) | *((bv_frag<21>*)b));
            case bv_op::XOR: return new bv_frag<21>(*((bv_frag<21>*)a) ^ *((bv_frag<21>*)b));
        }
        case 22: switch (op) {
            case bv_op::AND: return new bv_frag<22>(*((bv_frag<22>*)a) & *((bv_frag<22>*)b));
            case bv_op::OR : return new bv_frag<22>(*((bv_frag<22>*)a) | *((bv_frag<22>*)b));
            case bv_op::XOR: return new bv_frag<22>(*((bv_frag<22>*)a) ^ *((bv_frag<22>*)b));
        }
        case 23: switch (op) {
            case bv_op::AND: return new bv_frag<23>(*((bv_frag<23>*)a) & *((bv_frag<23>*)b));
            case bv_op::OR : return new bv_frag<23>(*((bv_frag<23>*)a) | *((bv_frag<23>*)b));
            case bv_op::XOR: return new bv_frag<23>(*((bv_frag<23>*)a) ^ *((bv_frag<23>*)b));
        }
        case 24: switch (op) {
            case bv_op::AND: return new bv_frag<24>(*((bv_frag<24>*)a) & *((bv_frag<24>*)b));
            case bv_op::OR : return new bv_frag<24>(*((bv_frag<24>*)a) | *((bv_frag<24>*)b));
            case bv_op::XOR: return new bv_frag<24>(*((bv_frag<24>*)a) ^ *((bv_frag<24>*)b));
        }
        case 25: switch (op) {
            case bv_op::AND: return new bv_frag<25>(*((bv_frag<25>*)a) & *((bv_frag<25>*)b));
            case bv_op::OR : return new bv_frag<25>(*((bv_frag<25>*)a) | *((bv_frag<25>*)b));
            case bv_op::XOR: return new bv_frag<25>(*((bv_frag<25>*)a) ^ *((bv_frag<25>*)b));
        }
        case 26: switch (op) {
            case bv_op::AND: return new bv_frag<26>(*((bv_frag<26>*)a) & *((bv_frag<26>*)b));
            case bv_op::OR : return new bv_frag<26>(*((bv_frag<26>*)a) | *((bv_frag<26>*)b));
            case bv_op::XOR: return new bv_frag<26>(*((bv_frag<26>*)a) ^ *((bv_frag<26>*)b));
        }
        case 27: switch (op) {
            case bv_op::AND: return new bv_frag<27>(*((bv_frag<27>*)a) & *((bv_frag<27>*)b));
            case bv_op::OR : return new bv_frag<27>(*((bv_frag<27>*)a) | *((bv_frag<27>*)b));
            case bv_op::XOR: return new bv_frag<27>(*((bv_frag<27>*)a) ^ *((bv_frag<27>*)b));
        }
        case 28: switch (op) {
            case bv_op::AND: return new bv_frag<28>(*((bv_frag<28>*)a) & *((bv_frag<28>*)b));
            case bv_op::OR : return new bv_frag<28>(*((bv_frag<28>*)a) | *((bv_frag<28>*)b));
            case bv_op::XOR: return new bv_frag<28>(*((bv_frag<28>*)a) ^ *((bv_frag<28>*)b));
        }
        case 29: switch (op) {
            case bv_op::AND: return new bv_frag<29>(*((bv_frag<29>*)a) & *((bv_frag<29>*)b));
            case bv_op::OR : return new bv_frag<29>(*((bv_frag<29>*)a) | *((bv_frag<29>*)b));
            case bv_op::XOR: return new bv_frag<29>(*((bv_frag<29>*)a) ^ *((bv_frag<29>*)b));
        }
        case 30: switch (op) {
            case bv_op::AND: return new bv_frag<30>(*((bv_frag<30>*)a) & *((bv_frag<30>*)b));
            case bv_op::OR : return new bv_frag<30>(*((bv_frag<30>*)a) | *((bv_frag<30>*)b));
            case bv_op::XOR: return new bv_frag<30>(*((bv_frag<30>*)a) ^ *((bv_frag<30>*)b));
        }
        case 31: switch (op) {
            case bv_op::AND: return new bv_frag<31>(*((bv_frag<31>*)a) & *((bv_frag<31>*)b));
            case bv_op::OR : return new bv_frag<31>(*((bv_frag<31>*)a) | *((bv_frag<31>*)b));
            case bv_op::XOR: return new bv_frag<31>(*((bv_frag<31>*)a) ^ *((bv_frag<31>*)b));
        }
        case 32: switch (op) {
            case bv_op::AND: return new bv_frag<32>(*((bv_frag<32>*)a) & *((bv_frag<32>*)b));
            case bv_op::OR : return new bv_frag<32>(*((bv_frag<32>*)a) | *((bv_frag<32>*)b));
            case bv_op::XOR: return new bv_frag<32>(*((bv_frag<32>*)a) ^ *((bv_frag<32>*)b));
        }
    }
    printf("bv_fragment_op failed (op=%d size=%ld)\n", op, size);
    assert(false);
}
bool bv_fragment_cmp(bv_cmp_op op, long size, void* a, void* b)
{
    switch (size) {
        case 1: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<1>*)a) == *((bv_frag<1>*)b);
            case bv_cmp_op::LT : return *((bv_frag<1>*)a) < *((bv_frag<1>*)b);
        }
        case 2: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<2>*)a) == *((bv_frag<2>*)b);
            case bv_cmp_op::LT : return *((bv_frag<2>*)a) < *((bv_frag<2>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<2>*)a)).signed_lower_than_impl(*((bv_frag<2>*)b));
        }
        case 3: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<3>*)a) == *((bv_frag<3>*)b);
            case bv_cmp_op::LT : return *((bv_frag<3>*)a) < *((bv_frag<3>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<3>*)a)).signed_lower_than_impl(*((bv_frag<3>*)b));
        }
        case 4: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<4>*)a) == *((bv_frag<4>*)b);
            case bv_cmp_op::LT : return *((bv_frag<4>*)a) < *((bv_frag<4>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<4>*)a)).signed_lower_than_impl(*((bv_frag<4>*)b));
        }
        case 5: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<5>*)a) == *((bv_frag<5>*)b);
            case bv_cmp_op::LT : return *((bv_frag<5>*)a) < *((bv_frag<5>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<5>*)a)).signed_lower_than_impl(*((bv_frag<5>*)b));
        }
        case 6: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<6>*)a) == *((bv_frag<6>*)b);
            case bv_cmp_op::LT : return *((bv_frag<6>*)a) < *((bv_frag<6>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<6>*)a)).signed_lower_than_impl(*((bv_frag<6>*)b));
        }
        case 7: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<7>*)a) == *((bv_frag<7>*)b);
            case bv_cmp_op::LT : return *((bv_frag<7>*)a) < *((bv_frag<7>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<7>*)a)).signed_lower_than_impl(*((bv_frag<7>*)b));
        }
        case 8: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<8>*)a) == *((bv_frag<8>*)b);
            case bv_cmp_op::LT : return *((bv_frag<8>*)a) < *((bv_frag<8>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<8>*)a)).signed_lower_than_impl(*((bv_frag<8>*)b));
        }
        case 9: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<9>*)a) == *((bv_frag<9>*)b);
            case bv_cmp_op::LT : return *((bv_frag<9>*)a) < *((bv_frag<9>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<9>*)a)).signed_lower_than_impl(*((bv_frag<9>*)b));
        }
        case 10: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<10>*)a) == *((bv_frag<10>*)b);
            case bv_cmp_op::LT : return *((bv_frag<10>*)a) < *((bv_frag<10>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<10>*)a)).signed_lower_than_impl(*((bv_frag<10>*)b));
        }
        case 11: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<11>*)a) == *((bv_frag<11>*)b);
            case bv_cmp_op::LT : return *((bv_frag<11>*)a) < *((bv_frag<11>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<11>*)a)).signed_lower_than_impl(*((bv_frag<11>*)b));
        }
        case 12: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<12>*)a) == *((bv_frag<12>*)b);
            case bv_cmp_op::LT : return *((bv_frag<12>*)a) < *((bv_frag<12>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<12>*)a)).signed_lower_than_impl(*((bv_frag<12>*)b));
        }
        case 13: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<13>*)a) == *((bv_frag<13>*)b);
            case bv_cmp_op::LT : return *((bv_frag<13>*)a) < *((bv_frag<13>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<13>*)a)).signed_lower_than_impl(*((bv_frag<13>*)b));
        }
        case 14: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<14>*)a) == *((bv_frag<14>*)b);
            case bv_cmp_op::LT : return *((bv_frag<14>*)a) < *((bv_frag<14>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<14>*)a)).signed_lower_than_impl(*((bv_frag<14>*)b));
        }
        case 15: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<15>*)a) == *((bv_frag<15>*)b);
            case bv_cmp_op::LT : return *((bv_frag<15>*)a) < *((bv_frag<15>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<15>*)a)).signed_lower_than_impl(*((bv_frag<15>*)b));
        }
        case 16: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<16>*)a) == *((bv_frag<16>*)b);
            case bv_cmp_op::LT : return *((bv_frag<16>*)a) < *((bv_frag<16>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<16>*)a)).signed_lower_than_impl(*((bv_frag<16>*)b));
        }
        case 17: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<17>*)a) == *((bv_frag<17>*)b);
            case bv_cmp_op::LT : return *((bv_frag<17>*)a) < *((bv_frag<17>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<17>*)a)).signed_lower_than_impl(*((bv_frag<17>*)b));
        }
        case 18: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<18>*)a) == *((bv_frag<18>*)b);
            case bv_cmp_op::LT : return *((bv_frag<18>*)a) < *((bv_frag<18>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<18>*)a)).signed_lower_than_impl(*((bv_frag<18>*)b));
        }
        case 19: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<19>*)a) == *((bv_frag<19>*)b);
            case bv_cmp_op::LT : return *((bv_frag<19>*)a) < *((bv_frag<19>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<19>*)a)).signed_lower_than_impl(*((bv_frag<19>*)b));
        }
        case 20: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<20>*)a) == *((bv_frag<20>*)b);
            case bv_cmp_op::LT : return *((bv_frag<20>*)a) < *((bv_frag<20>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<20>*)a)).signed_lower_than_impl(*((bv_frag<20>*)b));
        }
        case 21: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<21>*)a) == *((bv_frag<21>*)b);
            case bv_cmp_op::LT : return *((bv_frag<21>*)a) < *((bv_frag<21>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<21>*)a)).signed_lower_than_impl(*((bv_frag<21>*)b));
        }
        case 22: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<22>*)a) == *((bv_frag<22>*)b);
            case bv_cmp_op::LT : return *((bv_frag<22>*)a) < *((bv_frag<22>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<22>*)a)).signed_lower_than_impl(*((bv_frag<22>*)b));
        }
        case 23: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<23>*)a) == *((bv_frag<23>*)b);
            case bv_cmp_op::LT : return *((bv_frag<23>*)a) < *((bv_frag<23>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<23>*)a)).signed_lower_than_impl(*((bv_frag<23>*)b));
        }
        case 24: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<24>*)a) == *((bv_frag<24>*)b);
            case bv_cmp_op::LT : return *((bv_frag<24>*)a) < *((bv_frag<24>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<24>*)a)).signed_lower_than_impl(*((bv_frag<24>*)b));
        }
        case 25: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<25>*)a) == *((bv_frag<25>*)b);
            case bv_cmp_op::LT : return *((bv_frag<25>*)a) < *((bv_frag<25>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<25>*)a)).signed_lower_than_impl(*((bv_frag<25>*)b));
        }
        case 26: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<26>*)a) == *((bv_frag<26>*)b);
            case bv_cmp_op::LT : return *((bv_frag<26>*)a) < *((bv_frag<26>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<26>*)a)).signed_lower_than_impl(*((bv_frag<26>*)b));
        }
        case 27: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<27>*)a) == *((bv_frag<27>*)b);
            case bv_cmp_op::LT : return *((bv_frag<27>*)a) < *((bv_frag<27>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<27>*)a)).signed_lower_than_impl(*((bv_frag<27>*)b));
        }
        case 28: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<28>*)a) == *((bv_frag<28>*)b);
            case bv_cmp_op::LT : return *((bv_frag<28>*)a) < *((bv_frag<28>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<28>*)a)).signed_lower_than_impl(*((bv_frag<28>*)b));
        }
        case 29: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<29>*)a) == *((bv_frag<29>*)b);
            case bv_cmp_op::LT : return *((bv_frag<29>*)a) < *((bv_frag<29>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<29>*)a)).signed_lower_than_impl(*((bv_frag<29>*)b));
        }
        case 30: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<30>*)a) == *((bv_frag<30>*)b);
            case bv_cmp_op::LT : return *((bv_frag<30>*)a) < *((bv_frag<30>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<30>*)a)).signed_lower_than_impl(*((bv_frag<30>*)b));
        }
        case 31: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<31>*)a) == *((bv_frag<31>*)b);
            case bv_cmp_op::LT : return *((bv_frag<31>*)a) < *((bv_frag<31>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<31>*)a)).signed_lower_than_impl(*((bv_frag<31>*)b));
        }
        case 32: switch (op) {
            case bv_cmp_op::EQ : return *((bv_frag<32>*)a) == *((bv_frag<32>*)b);
            case bv_cmp_op::LT : return *((bv_frag<32>*)a) < *((bv_frag<32>*)b);
            case bv_cmp_op::SLT: return (*((bv_frag<32>*)a)).signed_lower_than_impl(*((bv_frag<32>*)b));
        }
    }
    printf("bv_fragment_cmp failed (op=%d size=%ld)\n", op, size);
    assert(false);
}
void bv_fragment_del(long size, void* a)
{
    switch (size) {
        case 1: delete ((bv_frag<1>*)a); return;
        case 2: delete ((bv_frag<2>*)a); return;
        case 3: delete ((bv_frag<3>*)a); return;
        case 4: delete ((bv_frag<4>*)a); return;
        case 5: delete ((bv_frag<5>*)a); return;
        case 6: delete ((bv_frag<6>*)a); return;
        case 7: delete ((bv_frag<7>*)a); return;
        case 8: delete ((bv_frag<8>*)a); return;
        case 9: delete ((bv_frag<9>*)a); return;
        case 10: delete ((bv_frag<10>*)a); return;
        case 11: delete ((bv_frag<11>*)a); return;
        case 12: delete ((bv_frag<12>*)a); return;
        case 13: delete ((bv_frag<13>*)a); return;
        case 14: delete ((bv_frag<14>*)a); return;
        case 15: delete ((bv_frag<15>*)a); return;
        case 16: delete ((bv_frag<16>*)a); return;
        case 17: delete ((bv_frag<17>*)a); return;
        case 18: delete ((bv_frag<18>*)a); return;
        case 19: delete ((bv_frag<19>*)a); return;
        case 20: delete ((bv_frag<20>*)a); return;
        case 21: delete ((bv_frag<21>*)a); return;
        case 22: delete ((bv_frag<22>*)a); return;
        case 23: delete ((bv_frag<23>*)a); return;
        case 24: delete ((bv_frag<24>*)a); return;
        case 25: delete ((bv_frag<25>*)a); return;
        case 26: delete ((bv_frag<26>*)a); return;
        case 27: delete ((bv_frag<27>*)a); return;
        case 28: delete ((bv_frag<28>*)a); return;
        case 29: delete ((bv_frag<29>*)a); return;
        case 30: delete ((bv_frag<30>*)a); return;
        case 31: delete ((bv_frag<31>*)a); return;
        case 32: delete ((bv_frag<32>*)a); return;
    }
    printf("bv_fragment_del(%ld, *) failed. not implemented\n", size);
    assert(false);
}
bool bv_fragment_symbolic(long size, void* a)
{
    switch (size) {
        case 1: return ((bv_frag<1>*)a)->get_is_symbolic();
        case 2: return ((bv_frag<2>*)a)->get_is_symbolic();
        case 3: return ((bv_frag<3>*)a)->get_is_symbolic();
        case 4: return ((bv_frag<4>*)a)->get_is_symbolic();
        case 5: return ((bv_frag<5>*)a)->get_is_symbolic();
        case 6: return ((bv_frag<6>*)a)->get_is_symbolic();
        case 7: return ((bv_frag<7>*)a)->get_is_symbolic();
        case 8: return ((bv_frag<8>*)a)->get_is_symbolic();
        case 9: return ((bv_frag<9>*)a)->get_is_symbolic();
        case 10: return ((bv_frag<10>*)a)->get_is_symbolic();
        case 11: return ((bv_frag<11>*)a)->get_is_symbolic();
        case 12: return ((bv_frag<12>*)a)->get_is_symbolic();
        case 13: return ((bv_frag<13>*)a)->get_is_symbolic();
        case 14: return ((bv_frag<14>*)a)->get_is_symbolic();
        case 15: return ((bv_frag<15>*)a)->get_is_symbolic();
        case 16: return ((bv_frag<16>*)a)->get_is_symbolic();
        case 17: return ((bv_frag<17>*)a)->get_is_symbolic();
        case 18: return ((bv_frag<18>*)a)->get_is_symbolic();
        case 19: return ((bv_frag<19>*)a)->get_is_symbolic();
        case 20: return ((bv_frag<20>*)a)->get_is_symbolic();
        case 21: return ((bv_frag<21>*)a)->get_is_symbolic();
        case 22: return ((bv_frag<22>*)a)->get_is_symbolic();
        case 23: return ((bv_frag<23>*)a)->get_is_symbolic();
        case 24: return ((bv_frag<24>*)a)->get_is_symbolic();
        case 25: return ((bv_frag<25>*)a)->get_is_symbolic();
        case 26: return ((bv_frag<26>*)a)->get_is_symbolic();
        case 27: return ((bv_frag<27>*)a)->get_is_symbolic();
        case 28: return ((bv_frag<28>*)a)->get_is_symbolic();
        case 29: return ((bv_frag<29>*)a)->get_is_symbolic();
        case 30: return ((bv_frag<30>*)a)->get_is_symbolic();
        case 31: return ((bv_frag<31>*)a)->get_is_symbolic();
        case 32: return ((bv_frag<32>*)a)->get_is_symbolic();
    }
    printf("bv_fragment_symbolic(%ld, *) failed. not implemented\n", size);
    assert(false);
}
void* bv_fragment_new(long size, long val)
{
    switch (size) {
        case 1: return new bv_frag<1>(val);
        case 2: return new bv_frag<2>(val);
        case 3: return new bv_frag<3>(val);
        case 4: return new bv_frag<4>(val);
        case 5: return new bv_frag<5>(val);
        case 6: return new bv_frag<6>(val);
        case 7: return new bv_frag<7>(val);
        case 8: return new bv_frag<8>(val);
        case 9: return new bv_frag<9>(val);
        case 10: return new bv_frag<10>(val);
        case 11: return new bv_frag<11>(val);
        case 12: return new bv_frag<12>(val);
        case 13: return new bv_frag<13>(val);
        case 14: return new bv_frag<14>(val);
        case 15: return new bv_frag<15>(val);
        case 16: return new bv_frag<16>(val);
        case 17: return new bv_frag<17>(val);
        case 18: return new bv_frag<18>(val);
        case 19: return new bv_frag<19>(val);
        case 20: return new bv_frag<20>(val);
        case 21: return new bv_frag<21>(val);
        case 22: return new bv_frag<22>(val);
        case 23: return new bv_frag<23>(val);
        case 24: return new bv_frag<24>(val);
        case 25: return new bv_frag<25>(val);
        case 26: return new bv_frag<26>(val);
        case 27: return new bv_frag<27>(val);
        case 28: return new bv_frag<28>(val);
        case 29: return new bv_frag<29>(val);
        case 30: return new bv_frag<30>(val);
        case 31: return new bv_frag<31>(val);
        case 32: return new bv_frag<32>(val);
    }
    printf("bv_fragment_new(%ld, *) failed. not implemented\n", size);
    assert(false);
}
void* bv_fragment_new(long size, std::shared_ptr<SymbolicExpression> val)
{
    switch (size) {
        case 1: return new bv_frag<1>(val);
        case 2: return new bv_frag<2>(val);
        case 3: return new bv_frag<3>(val);
        case 4: return new bv_frag<4>(val);
        case 5: return new bv_frag<5>(val);
        case 6: return new bv_frag<6>(val);
        case 7: return new bv_frag<7>(val);
        case 8: return new bv_frag<8>(val);
        case 9: return new bv_frag<9>(val);
        case 10: return new bv_frag<10>(val);
        case 11: return new bv_frag<11>(val);
        case 12: return new bv_frag<12>(val);
        case 13: return new bv_frag<13>(val);
        case 14: return new bv_frag<14>(val);
        case 15: return new bv_frag<15>(val);
        case 16: return new bv_frag<16>(val);
        case 17: return new bv_frag<17>(val);
        case 18: return new bv_frag<18>(val);
        case 19: return new bv_frag<19>(val);
        case 20: return new bv_frag<20>(val);
        case 21: return new bv_frag<21>(val);
        case 22: return new bv_frag<22>(val);
        case 23: return new bv_frag<23>(val);
        case 24: return new bv_frag<24>(val);
        case 25: return new bv_frag<25>(val);
        case 26: return new bv_frag<26>(val);
        case 27: return new bv_frag<27>(val);
        case 28: return new bv_frag<28>(val);
        case 29: return new bv_frag<29>(val);
        case 30: return new bv_frag<30>(val);
        case 31: return new bv_frag<31>(val);
        case 32: return new bv_frag<32>(val);
    }
    printf("bv_fragment_new(%ld, *) failed. not implemented\n", size);
    assert(false);
}
