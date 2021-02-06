#if 0
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Size: 1
typedef enum ScreenFlag {
	SCREEN_NONE    = 0x00,
	SCREEN_PLAYER1 = 0x01,
	SCREEN_PLAYER2 = 0x02,
	SCREEN_CREDIT  = 0x40,
	SCREEN_ATTRACT = 0x80
} ScreenFlag;

// Offset: 0xFFE028
uint32_t Seed;

// Offset: 0xFFE040
ScreenFlag ScreenFlags;

// Size: 1
typedef enum PlayerNum {
	PLAYER1 = 1,
	PLAYER2 = 2,
	NUMPLAYERS = 2
} PlayerNum;

// Size: 1
typedef enum BlockType {
	BLOCKTYPE_I,
	BLOCKTYPE_Z,
	BLOCKTYPE_S,
	BLOCKTYPE_O,
	BLOCKTYPE_T,
	BLOCKTYPE_L,
	BLOCKTYPE_J,
	NUMBLOCKTYPES
} BlockType;

// Size: 128
typedef struct Player {
	PlayerNum num;    // Offset: 0x00
	uint8_t stageNum; // Offset: 0x0D
} Player;

// Offset: 0xFFE300
Player Players[NUMPLAYERS];

// Offset: 0xFFE400
BlockType BlockSequences[NUMPLAYERS][1024];

// Offset: 0x000D32
uint16_t Rand() {
	uint16_t result;
	// The upper 16 bits of the seed are ignored here, so the seed gets reset if
	// the low 16 bits are 0.
	if ((int16_t)Seed == 0) {
		Seed ^= 0x2A6D365Bu;
	}

	result =
		(((Seed * 41u) & 0xFFFFu) >>  0u) +
		( (Seed * 41u)            >> 16u);

	Seed =
		((uint32_t)result << 16u) |
		((Seed * (41u << 16u)) >> 16u);

	return result;
}

// Offset: 0x0086FC
// Note: The original is actually a table of offsets that are used in a way
// more typical of hand-written assembly. This C code reimplements how the
// original code worked, in a more C-friendly style.
uint16_t GenBlockOffsets[100] = {
	 0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,
	 0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,
	 0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,
	 0u, 64u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,
	 0u,  0u,  0u, 64u,  0u,  0u,  0u,  0u,  0u, 64u,
	 0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u, 64u,  0u,
	 0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,
	 0u,  0u,  0u,  0u, 64u,  0u,  0u,  0u,  0u,  0u,
	 0u,  0u,  0u,  0u,  0u, 64u,  0u,  0u, 64u,  0u,
	 0u,  0u,  0u, 64u,  0u,  0u, 64u,  0u,  0u,  0u
};

BlockType GenBlockTable[128] = {
	// These 64 values are used when an offset of 0 of the GenBlockOffsets array is used.
	// When i < 64, GenBlockTable[i] == i % NUMBLOCKTYPES.
	BLOCKTYPE_I, BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_O, BLOCKTYPE_T, BLOCKTYPE_L, BLOCKTYPE_J,
	BLOCKTYPE_I, BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_O, BLOCKTYPE_T, BLOCKTYPE_L, BLOCKTYPE_J,
	BLOCKTYPE_I, BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_O, BLOCKTYPE_T, BLOCKTYPE_L, BLOCKTYPE_J,
	BLOCKTYPE_I, BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_O, BLOCKTYPE_T, BLOCKTYPE_L, BLOCKTYPE_J,
	BLOCKTYPE_I, BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_O, BLOCKTYPE_T, BLOCKTYPE_L, BLOCKTYPE_J,
	BLOCKTYPE_I, BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_O, BLOCKTYPE_T, BLOCKTYPE_L, BLOCKTYPE_J,
	BLOCKTYPE_I, BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_O, BLOCKTYPE_T, BLOCKTYPE_L, BLOCKTYPE_J,
	BLOCKTYPE_I, BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_O, BLOCKTYPE_T, BLOCKTYPE_L, BLOCKTYPE_J,
	BLOCKTYPE_I, BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_O, BLOCKTYPE_T, BLOCKTYPE_L, BLOCKTYPE_J,
	BLOCKTYPE_I,

	// These 64 values are used when an offset of 64 of the GenBlockOffsets
	// array is used.
	//
	// Notice that indices 65 to 68 differ from indices 1 to 4, which will
	// result in the generated sequence being different than when offset 0 is used.
	BLOCKTYPE_I, BLOCKTYPE_I, BLOCKTYPE_I, BLOCKTYPE_I, BLOCKTYPE_I, BLOCKTYPE_L, BLOCKTYPE_J,
	BLOCKTYPE_I, BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_O, BLOCKTYPE_T, BLOCKTYPE_L, BLOCKTYPE_J,
	BLOCKTYPE_I, BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_O, BLOCKTYPE_T, BLOCKTYPE_L, BLOCKTYPE_J,
	BLOCKTYPE_I, BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_O, BLOCKTYPE_T, BLOCKTYPE_L, BLOCKTYPE_J,
	BLOCKTYPE_I, BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_O, BLOCKTYPE_T, BLOCKTYPE_L, BLOCKTYPE_J,
	BLOCKTYPE_I, BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_O, BLOCKTYPE_T, BLOCKTYPE_L, BLOCKTYPE_J,
	BLOCKTYPE_I, BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_O, BLOCKTYPE_T, BLOCKTYPE_L, BLOCKTYPE_J,
	BLOCKTYPE_I, BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_O, BLOCKTYPE_T, BLOCKTYPE_L, BLOCKTYPE_J,
	BLOCKTYPE_I, BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_O, BLOCKTYPE_T, BLOCKTYPE_L, BLOCKTYPE_J,
	BLOCKTYPE_I
};

void GenPlayerBlockSequence(Player* player);
void GenGameOverBlockSequence(Player* player);
BlockType GenBlock(Player* player);
BlockType* PlayerBlockSequence(Player* player);

// Offset: 0x00544C
BlockType TutorialBlockSequence[60] = {
	BLOCKTYPE_I, BLOCKTYPE_S, BLOCKTYPE_Z, BLOCKTYPE_T, BLOCKTYPE_L, BLOCKTYPE_J, BLOCKTYPE_I, BLOCKTYPE_I, BLOCKTYPE_T, BLOCKTYPE_J,
	BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_T, BLOCKTYPE_T, BLOCKTYPE_L, BLOCKTYPE_J, BLOCKTYPE_I, BLOCKTYPE_S, BLOCKTYPE_T, BLOCKTYPE_J,
	BLOCKTYPE_I, BLOCKTYPE_S, BLOCKTYPE_T, BLOCKTYPE_S, BLOCKTYPE_L, BLOCKTYPE_J, BLOCKTYPE_I, BLOCKTYPE_S, BLOCKTYPE_T, BLOCKTYPE_J,
	BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_T, BLOCKTYPE_S, BLOCKTYPE_L, BLOCKTYPE_J, BLOCKTYPE_I, BLOCKTYPE_S, BLOCKTYPE_T, BLOCKTYPE_J,
	BLOCKTYPE_I, BLOCKTYPE_S, BLOCKTYPE_T, BLOCKTYPE_S, BLOCKTYPE_L, BLOCKTYPE_J, BLOCKTYPE_I, BLOCKTYPE_S, BLOCKTYPE_T, BLOCKTYPE_J,
	BLOCKTYPE_Z, BLOCKTYPE_S, BLOCKTYPE_T, BLOCKTYPE_S, BLOCKTYPE_L, BLOCKTYPE_J, BLOCKTYPE_I, BLOCKTYPE_S, BLOCKTYPE_T, BLOCKTYPE_J
};

// Offset: 0x004F2E
void GenPlayerBlockSequence(Player* player) {
	if (ScreenFlags & SCREEN_ATTRACT) {
		BlockType* sequence = BlockSequences[PLAYER1 - 1];
		BlockType* initSequence = TutorialBlockSequence;
		for (int i = 59; i != -1; i--, sequence++, initSequence++) {
			*sequence = *initSequence;
		}
	}
	else {
		BlockType* sequence1p = BlockSequences[PLAYER1 - 1];
		BlockType* sequence2p = BlockSequences[PLAYER2 - 1];
		for (int i = 999; i != -1; i--, sequence1p++, sequence2p++) {
			const BlockType block = GenBlock(player);
			*sequence1p = block;
			*sequence2p = block;
		}
	}
}

#define BLOCKSEQUENCE_LENGTH 1000
// Offset: 0x004F60
void GenGameOverBlockSequence(Player* player) {
	BlockType* sequence = PlayerBlockSequence(player);
	for (int i = BLOCKSEQUENCE_LENGTH - 1; i != -1; i--, sequence++) {
		*sequence = GenBlock(player);
	}
}

// Offset: 0x004F76
BlockType GenBlock(Player* player) {
	return GenBlockTable[GenBlockOffsets[player->stageNum] + (Rand() % 64u)];
}

// Offset: 0x00512E
BlockType* PlayerBlockSequence(Player* player) {
	if (player->num == PLAYER1) {
		return BlockSequences[PLAYER1 - 1];
	}
	else {
		return BlockSequences[PLAYER2 - 1];
	}
}

// This is just some demonstration code, and is not reflective of the structure
// of the original code. The code and data defined above is basically identical
// to the original, though.
int main() {
	// Set the SCREEN_ATTRACT flag to get the tutorial block sequence in player
	// 1's block sequence array, which is only 60 blocks.
	ScreenFlags = SCREEN_NONE;

	// The power on sequence results from a seed of 0.
	Seed = 0u;

	Players[PLAYER1 - 1].num = PLAYER1;
	// Player::stageNum is based at 0, so stage 1 in the game menu will result
	// in Player::stageNum being set to 0.
	//
	// The original game sets the stage number to 0 when generating the
	// sequences for new games.
	//
	// Probably just to change the seed, the game generates a new sequence
	// via GenGameOverBlockSequence(playerThatEndedTheirGame) after the
	// game fully ends and transitions to the attract sequence, and
	// whatever the stage number was upon game over will be used in that
	// randomization.
	Players[PLAYER1 - 1].stageNum = 0u;

	// Player 2's sequence will be identical to player 1's. Player 2's sequence
	// is only generated when starting a new game, and won't be modified when
	// copying attract mode's tutorial sequence to player 1's sequence.
	Players[PLAYER2 - 1].num = PLAYER2;
	Players[PLAYER2 - 1].stageNum = 0u;

	// Have the stage number be set to 0 when generating the sequence for a new
	// game. The stage number is set to the selected stage later, after
	// generating the new game's sequence.
	GenPlayerBlockSequence(&Players[PLAYER1 - 1]);
	// Players[PLAYER1 - 1].stageNum = selectedStageNum.

	// Gameplay here, using the generated sequence.

	// Now, once a player game-over's, generate a block sequence that won't be
	// used, with the randomizer seed getting modified in the process. The stage
	// number set to the selected stage of the player that game-over'd will be
	// used when generating this sequence. You could use this to predict the
	// next game's seed and sequence, when the seed is known before starting the
	// previous game, such as knowing the seed is 0 for the power on sequence.
	//GenGameOverBlockSequence(gameOverPlayer);

#define PRINT_BLOCK(block) case BLOCKTYPE_##block: printf(#block); break
	const size_t width = 20u;
	for (size_t row = 0u; row < BLOCKSEQUENCE_LENGTH / width; row++) {
		for (size_t col = 0u; col < width; col++) {
			switch (BlockSequences[PLAYER1 - 1][row * width + col]) {
			PRINT_BLOCK(I);
			PRINT_BLOCK(Z);
			PRINT_BLOCK(S);
			PRINT_BLOCK(O);
			PRINT_BLOCK(T);
			PRINT_BLOCK(L);
			PRINT_BLOCK(J);
			default: break;
			}
		}
		printf("\n");
	}
	for (size_t col = 0u; col < BLOCKSEQUENCE_LENGTH % width; col++) {
			switch (BlockSequences[PLAYER1 - 1][(BLOCKSEQUENCE_LENGTH - (BLOCKSEQUENCE_LENGTH % width)) + col]) {
			PRINT_BLOCK(I);
			PRINT_BLOCK(Z);
			PRINT_BLOCK(S);
			PRINT_BLOCK(O);
			PRINT_BLOCK(T);
			PRINT_BLOCK(L);
			PRINT_BLOCK(J);
			default: break;
			}
	}
	printf("\n");
#undef PRINT_BLOCK

	return EXIT_SUCCESS;
}
