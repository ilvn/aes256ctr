#!/bin/bash
cbmc demo.c aes256.c -D BACK_TO_TABLES --unwind 256 --bounds-check --pointer-check --refine
