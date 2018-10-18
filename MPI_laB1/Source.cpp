#include "mpi.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include <ctime>
#include <cmath>



int main(int argv, char *argc[]) {
	srand(time(0));
	int diff1 = 0, diff2 = 0, diffsum = 0;
	int bufPos = 0;
	int newSize;
	int ProcNum;
	int rank;
	double start, end;
	char *str;
	MPI_Status status;
	std::string st1;
	std::string st2;
	for (int k = 0; k < 400; k++) {
		st1 += 'a' + rand() % 26;
		st2 += 'a' + rand() % 26;
	}
	diff1 = abs((int)st1.size() - (int)st2.size());
	st1.size() > st2.size() ? st1.resize(st2.size()) : st2.resize(st1.size());
	int n = st1.size();
	str = new char();

	MPI_Init(&argv, &argc);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		start = MPI_Wtime();
		newSize = ceil((float)n / ProcNum) * ProcNum;
		st1 += std::string((newSize - n), '-');
		st2 += std::string((newSize - n), '-');
		str = new char[ProcNum * newSize];
		strcpy_s(str, newSize * 2 + 1, (st1  + st2).c_str());	

		for (int i = 1; i < ProcNum; i++) {
			MPI_Send(&newSize, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(str, newSize * 2, MPI_CHAR, i, 0, MPI_COMM_WORLD);
		}
	}
	else {

		MPI_Recv(&newSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status );
		MPI_Recv(str, newSize * 2, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	}
	//MPI_Bcast(&newSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
	//MPI_Bcast(str, newSize * 2, MPI_CHAR, 0, MPI_COMM_WORLD);

	for (int i = newSize / (ProcNum) * rank; i < newSize / (ProcNum) * (rank + 1); i++)
		diff2 += str[i] != str[i + newSize] ? 1 : 0;

	MPI_Reduce(&diff2, &diffsum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		end = MPI_Wtime();
		std::cout << "Work time  : " << end - start << std::endl;
		std::cout << diffsum + diff1 << std::endl; 
	}
	
	MPI_Finalize();
	
}