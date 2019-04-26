#include <stdio.h> 
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

int afiseazaMapa( uint64_t harta ) {

	uint64_t v;
	int i = 64, adevarat = 0, zeros_number = 0;

	v = 1; 
	v <<=63; 

	while ( i > 0) {
		if ( i % 8 == 0) {
				printf("\n");
		}

		if ((v & harta) == 0) {	
			printf(".");
			zeros_number++;
		}
		else  {
			printf("#");
			adevarat++;
		}	

		i--; 
		v >>= 1;
	}
	printf("\n");

	return zeros_number;
}

float score(int zeros_number, int completedLines) {
	float score;
	score = sqrt(zeros_number) + pow(1.25, completedLines);
	return score;
}

void moveSideways(int32_t *piesa, int horiz_movement,
				uint64_t *piesa_map, uint64_t harta)
{
	int nr_of_moves;

	nr_of_moves = 1;

	while (nr_of_moves <= abs(horiz_movement)) {
		if (horiz_movement < 0) {
			if ((((*piesa & 255) << 1) >> 8) > 0)
				break;
			else if ((((*piesa << 1) >> 16) == 0) && (*piesa_map << 1 & harta) == 0) {
				*piesa_map = *piesa_map << 1;  // actualizam harta cu piesa
				*piesa = *piesa << 1;          // actualizam piesa
			}
			else
				break;
		}
		else if (horiz_movement > 0) {
			if (*piesa >> 8 != *piesa >> 9 << 1)
				break;
			if ((*piesa == (*piesa >> 1) << 1) &&
											(*piesa_map >> 1 & harta) == 0) {
				*piesa_map = *piesa_map >> 1;
				*piesa = *piesa >> 1; 
			}
			else
				break;
		}
		nr_of_moves++;
	}
}

uint64_t moveDown(uint64_t harta, int nr_rind,
						int32_t *piesa, int horiz_movement, int *success){
	uint64_t piesa_map = 0;

	piesa_map = *piesa;
	piesa_map = piesa_map << (7 - nr_rind) * 8;

	if( (harta & piesa_map) == 0 ) {
		moveSideways(piesa, horiz_movement, &piesa_map, harta);
		harta = harta | piesa_map;  //punem piesa pe harta
		*success = 1;
	}
	else {
		if (nr_rind > 0) {
			piesa_map = *piesa;
			piesa_map = piesa_map << (7 - nr_rind + 1) * 8;
			harta = harta | piesa_map;
		}
	}
	return harta;
}

uint64_t toPower(int i)
{
	uint64_t result;

	if (i == 0)
		return 0;
	result = 1;
	while (i > 0) {
		result = result * 256;
		i--;
	}
	return result - 1;
}

int clearLines(uint64_t *harta, int *completed_lines)
{
	int i, flag;
	uint64_t temp;
	
	i = 1;
	flag = 0;
	uint64_t a_line = 255;

	while (i < 9) {
		if ((*harta & a_line) == a_line) {
			flag++;
			temp = *harta & toPower(i - 1);
			if (temp > 0)
				*harta = ((*harta >> 8 * i) << 8 * i - 8) | temp;
			else
				*harta = (*harta >> 8);
		}
		else {
			a_line = a_line << 8;
			i++;
		}
	}
	*completed_lines = *completed_lines + flag;
	return flag;
}

int main() {

	uint64_t harta_dynamic;
	int completed_lines = 0, zeros_number = 0, nr_of_pieces;

	scanf("%llu", &harta_dynamic);
	scanf("%d", &nr_of_pieces);

	int i, j, success;
	int32_t Tab[nr_of_pieces][9];
	uint64_t harta_static = harta_dynamic;
	
	for( i = 0; i < nr_of_pieces; i++ ) {
		for( j = 0 ; j < 9; j++) {
			scanf("%d", &Tab[i][j]);
		}
	}

	zeros_number = afiseazaMapa( harta_dynamic );
	
	for( i = 0; i < nr_of_pieces; i++ ) {
		for( j = 1; j < 9 ; j++ ) {
			success = 0;
			harta_dynamic = moveDown(harta_static, j - 1 , &(Tab[i][0]),
									Tab[i][j], &success);
			if( success )
				zeros_number = afiseazaMapa(harta_dynamic);
			else
				break;
		}
	    if (j == 1 || (j == 2 && Tab[i][0] > 255))	{
			zeros_number = afiseazaMapa(harta_dynamic);
			break;
		}
		if (clearLines(&harta_dynamic, &completed_lines))
			zeros_number = afiseazaMapa(harta_dynamic);
		harta_static = harta_dynamic;
	}
	printf("\n\nGAME OVER! \n");
	printf("Score:%.2f\n", score(zeros_number,completed_lines));
	
	return 0;
}
