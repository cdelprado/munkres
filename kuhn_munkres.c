#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "EDH.h"

#define NP 5

typedef struct{
	double C[NP*2][NP*2];
	int row_marked[(NP*2)];
	int col_marked[(NP*2)];
	int marked[(NP*2)][(NP*2)];
	int row_cover[(NP*2)];
	int col_cover[(NP*2)];
	int step;
	int lonePrime[2];
	
}Munkres;


void print_matrix_final(double C[NP][NP*2], int indexes[NP][NP*2]){
	int i;
	printf("Asignacion optima\n");
	for(i=0; i<NP;i++){
		int j;
		for(j=0;j<(NP*2);j++){
			if(indexes[i][j]==1){
				printf("%f\n",C[i][j]);
			}
			
		}
	}
}

//vuelve la matrix 2n * 2n a n * 2n
void cut_matrix(Munkres M, int indexes[NP][NP*2]){
	
	int i;
	for(i = 0; i<NP; i++){
		int j;
		for(j=0;j<(NP*2);j++){
			indexes[i][j] = M.marked[i][j];
			//printf("%d\t",indexes[i][j]);
		}
		//printf("\n");
	}

}

Munkres step1(Munkres M){

	int i;
	for(i=0;i<(NP*2);i++){
		double minVal = INFINITY;
		int j;
		for(j = 0; j<(NP*2); j++){
			if(M.C[i][j]<minVal){
				minVal = M.C[i][j];
			}
		}
		for(j = 0; j<(NP*2); j++){
			M.C[i][j] = M.C[i][j] - minVal;
			
		}
		
	}
	M.step = 2;
	return M;
}

Munkres step2(Munkres M){
	
	int i;
	for(i=0;i<(NP*2);i++){
		
		int j;

		for(j = 0; j<(NP*2); j++){
			if(M.C[i][j]==0 && !M.marked[i][j]){
				if(!M.row_marked[i] && !M.col_marked[j]){
					M.marked[i][j] = 1;
					M.row_marked[i] = 1;
					M.col_marked[j] = 1;
					break;
				}
	
			}
		}
		
	}
	M.step = 3;

	return M;
}

Munkres step3(Munkres M, int done){

	int i;
	int cont = 0;
	for(i = 0; i < (NP*2); i++){
		int j;
		for(j = 0; j<(NP*2); j++){
			if(M.marked[i][j]==1){
				M.col_cover[j] = 1;
				cont++;
			}
		}
	}

	if(done==cont){
		M.step = 7;
	}else{
		M.step = 4;
	}
	return M;
}

Munkres step5(Munkres M){
//aqui esta el error
	int mat[(NP*2)][(NP*2)];
	memcpy(mat, M.marked, sizeof(mat));//copiar matriz a una local porque se perdia la memoria al hacer comparaciones

	int path[(NP*2)][2];
	int count = 0;
	
	path[count][0] = M.lonePrime[0];
	path[count][1] = M.lonePrime[1];
	int i;
	for(i=0;i<(NP*2);i++){
		int j;
		for(j=0; j<(NP*2); j++){
		}
	}
	int done=0;
	while(done==0){
		
		int rowEnd = 0;
		for(i = 0; i<(NP*2); i++){
			if(mat[i][ path[count][1] ]==1){
				count++;
				path[count][0] = i;
				path[count][1] = path[count-1][1];
				mat[i][ path[count][1] ] = 0;
				rowEnd = 1;
				break;
			}
		}
		if(rowEnd==0){
			done = 1;
			
		}
		if(done!=1){
			for(i=0; i<(NP*2); i++){
				if(mat[ path[count][0] ][i]==2){
					count++;
					path[count][0] = path[count-1][0];
					path[count][1] = i;
					mat[i][ path[count][1] ] = 0;
					break;
				}
			}
		}
	}
	for(i = 0; i<=count; i++){
		if(M.marked[path[i][0]][path[i][1]]==1){
		}else{
			if(M.marked[path[i][0]][path[i][1]]==2){
			}
		}
	}
	count++;
	for(i=0; i<count; i++){
		if(M.marked[ path[i][0] ][ path[i][1] ]==1){
			
			M.marked[ path[i][0] ][ path[i][1] ] = 0;
			M.row_marked[ path[i][0] ] = 0;
		}else{
			if(M.marked[ path[i][0] ][ path[i][1] ]==2){
				M.marked[ path[i][0] ][ path[i][1] ] = 1;
				M.row_marked[ path[i][0] ] = 1;
			}
		}
	}

	for(i=0; i<(NP*2); i++){
		M.row_cover[i] = 0;
		M.col_cover[i] = 0;
	}
	M.step = 3;
	return M;
}


Munkres step4(Munkres M){
	int end = 0;
	
	int markedIndex=0;
	while(end==0){
		int i;

		for(i = 0; i < (NP*2); i++){
			int j;
			for(j=0;j<(NP*2);j++){
				markedIndex=-1;
				if(M.marked[i][j]==1){
					markedIndex = j;
					
					break;
				}
			}
			
			for(j=0; j<(NP*2); j++){
				
				end = 1;
				if(M.C[i][j]==0 && M.row_cover[i]!=1 && M.col_cover[j]!=1){
					M.marked[i][j] = 2;

					if(markedIndex==-1){
						M.lonePrime[0] = i;
						M.lonePrime[1] = j;
						M.step=5;
						
						return M;

						
					}else{
						end = 0;
						M.row_cover[i] = 1;
						
						M.col_cover[markedIndex] = 0; 
						
					}
				}
			}

		}
	}
	M.step = 6;
	//print_matrix(M);
	return M;
}

Munkres step6(Munkres M){
	
	int i;
	double minVal = INFINITY;
	for(i=0; i<(NP*2); i++){
		int j;
		for(j=0; j<(NP*2); j++){
			if(M.C[i][j]<minVal && M.row_cover[i]!=1 && M.col_cover[j]!=1 ){
				minVal = M.C[i][j];
			}
		}
	}

	for(i=0; i<(NP*2); i++){
		if(M.row_cover[i]==1){
			int j;
			for(j = 0; j<(NP*2); j++){
				M.C[i][j] = M.C[i][j] + minVal;
			}
		}
	}

	for(i=0; i<(NP*2); i++){
		if(M.col_cover[i]!=1){
			int j;
			for(j = 0; j<(NP*2); j++){
				M.C[j][i] = M.C[j][i] - minVal;
			}
		}
	}
//print_matrix(M);
	M.step = 4;
	return M;
}

//llena con 0 la matriz
void pad_matrix(Munkres M){
	int i;

	for(i = NP; i<(NP*2); i++){
		int j;
			
			for(j=0;j<(NP*2);j++){
				M.C[i][j] =0;
			}
		
	}

}

Munkres get_munkres(Munkres M){
	int done = (NP*2);

	pad_matrix(M);

	int i;

	for(i = 0; i<(NP*2); i++){
		int j;
		M.row_cover[i] = 0;
		M.row_marked[i] = 0;
		for(j= 0; j<(NP*2); j++){
			M.marked[i][j] = 0;
			M.col_cover[j] = 0;
			M.col_marked[j] = 0;
			
		}
	}
	
	//print_matrix(M.marked);

	M.step = 1;
	int theEndIsNear = 0;
	while(theEndIsNear==0){
		switch(M.step){
			case 1:
			
				M = step1(M);
				
				break;
			case 2:
				M = step2(M);

				break;
			case 3:
				M = step3(M,done);

				break;
			case 4:
				M = step4(M);
				
				break;
			case 5:
				M = step5(M);

				break;
			case 6:
				M = step6(M);
				
				break;
			case 7:

				theEndIsNear = 1;
				break;
		}
	}

return M;
}

int main(){
	int i;
	double C1[10][10] = {{6.082888, 1.758693, 2.167315, 1.028208, 6.324555, 6.090312, 1.859199, 2.267649, 1.066627, 6.324555},
{5.788006, 0.852706, 1.181810, 1.554489, 7.302967, 5.791662, 0.852914, 1.185656, 1.612571, 7.302967},
{5.440701, 1.573023, 1.938505, 2.400398, 5.656854, 5.447341, 1.662918, 2.028247, 2.490087, 5.656854},
{7.578279, 2.361087, 3.272354, 4.304275, 9.561829, 7.583066, 2.361663, 3.283003, 4.465102, 9.561829},
{2.703506, 7.515289, 10.415832, 13.700417, 19.486833, 2.706805, 7.517123, 10.449730, 14.212326, 19.486833}};
	
	//ESTA MATRIZ LO CICLA Y LA DE ARRIBA NO 
	/*double C1[NP][NP*2] = {{2.518705, 1.309495, 6.324555, 4.529254, 3.151426, 4.305624, 2.531763, 3.219268, 2.605261, 6.324555},
 {2.908350, 0.504025, 7.302967, 5.229932, 3.638954, 4.971706, 2.923428, 1.264771, 1.002766, 7.302967},
 {0.750933, 1.171248, 5.656854, 1.470894, 0.939574, 1.283689, 0.754826, 2.879401, 2.330216, 5.656854},
 {3.807924, 0.928616, 9.561829, 6.847588, 4.764509, 6.509491, 3.827665, 3.207618, 2.356676, 9.561829},
 {1.257377, 2.955766, 19.486833, 3.632014, 0.840380, 6.689916, 0.675137, 10.209779, 7.501249, 19.486833},
};
*/ 
	Munkres M;
	
	for(i=0;i<(NP*2); i++){
		int j;
		
		for(j=0;j<(NP*2);j++){
			if(i<NP){
				M.C[i][j] = C1[i][j];	
			}else{
				M.C[i][j] = 0;
			}
			
			
		}
		
	}


	Munkres matrix = get_munkres(M);
	
	
	int indexes[NP][NP*2]; //matriz final
	cut_matrix(matrix, indexes);
	
	print_matrix_final(C1, indexes);
	return 0;
}
