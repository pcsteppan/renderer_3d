#pragma once

#define N_TREADLES 4
#define N_INSTR_COUNT 8
#define N_THREADS 4
#define N_THREAD_COUNT 8

bool tieup[N_TREADLES][N_THREADS] = {
	{true,  false, false, true},
	{false, false, true, true},
	{false, true,  true, false},
	{true,  true, false, false}
};

int treadling[N_INSTR_COUNT] = {
	0,1,2,3,0,1,2,3
};

int threading[N_THREAD_COUNT] = {
	0,1,2,3,0,1,2,3
};
