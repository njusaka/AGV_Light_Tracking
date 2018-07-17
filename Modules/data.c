#include "data.h"

char refine_Data(char rx[], int len, float *ang, float *dst) {

	int i, j;
	int E_Pos = 0, S_Pos = 0, Space_Pos = 0;
	char temp[20] = { 0 };					  //存放数字的临时变量

	if (len < Rx_Len_Minium_Len)
		return 1;

	// 找到E
	for (i = len - 1; i >= 0; i--) {
		if (rx[i] == 'E') {
			E_Pos = i;
			break;
		}
	}
	if (E_Pos != i)
		return 1;
	
	//  找到空格
	for (i = E_Pos; i >= 0; i--) {
		if (rx[i] == ' ') {
			Space_Pos = i;
			break;
		}
	}
	if (Space_Pos != i)
		return 1;
	
	// 找到S
	for (i = Space_Pos; i >= 0; i--) {
		if (rx[i] == 'S') {
			S_Pos = i;
			break;
		}
	}
	if (S_Pos != i)
		return 1;

	if (S_Pos >= Space_Pos || Space_Pos >= E_Pos)
		return 2;
	else if (Space_Pos - S_Pos <= 5 || E_Pos - Space_Pos <= 5)
		return 3;
	
	// distance
	for (i = S_Pos + 1, j = 0; i < Space_Pos; i++, j++)
		temp[j] = rx[i];
	*dst = atof(temp);
	// angle
	for (i = Space_Pos + 1, j = 0; i < E_Pos; i++, j++)
		temp[j] = rx[i];
	*ang = atof(temp);

	return 0;

}
