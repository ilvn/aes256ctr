#!/bin/bash
cbmc demo.c aes256.c -I./inc/ -I./arch/x86/ -D BACK_TO_TABLES --unwind 256 --bounds-check --pointer-check --refine
