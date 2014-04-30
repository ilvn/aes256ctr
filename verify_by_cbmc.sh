#!/bin/sh
cbmc demo.c aes256.c -D BACK_TO_TABLES --partial-loops --bounds-check --pointer-check --div-by-zero-check --memory-leak-check --signed-overflow-check --unsigned-overflow-check --refine
# cbmc demo.c aes256.c --function gf_alog --unwind 288 --bounds-check --pointer-check --div-by-zero-check --memory-leak-check --signed-overflow-check --unsigned-overflow-check --refine
# cbmc demo.c aes256.c --function gf_log --bounds-check --pointer-check --div-by-zero-check --memory-leak-check --signed-overflow-check --unsigned-overflow-check --refine

