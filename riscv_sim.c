#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

//clock cycles
long long cycles = 0;

// registers
long long int regs[32];

// program counter
unsigned long pc = 0;

// memory
#define INST_MEM_SIZE 32*1024
#define DATA_MEM_SIZE 32*1024
unsigned long inst_mem[INST_MEM_SIZE]; //instruction memory
unsigned long long data_mem[DATA_MEM_SIZE]; //data memory

unsigned long x;
int bin[32];
int op[7];
int func7[7], rs2[5], rs1[5], func3[3], rd[5];
int imme[12], imme2[20];
int temp=0;
int tfi=0, tfi_temp=0;

int init(char* filename);

void print_cycles();
void print_reg();
void print_pc();

//binary to decimal
int bintodec(int bin[], int cnt)
{
	int cur=0;
	int dec=0;
	for(int i=cnt; i>=0 ; i--)
	{
		if(bin[i]==1)
			dec += 1 << cur;
		cur++;
	}
	return dec;
}
/*
//decimal to binary
void dectobin(int dec)
{
	for(int i=0;i<12;i++)
	{
		imme[i]=dec%2;
		dec/=2;
	}
}
void dectobin2(int dec)
{
	for(int i=0;i<20;i++)
	{
		imme[i]=dec%2;
		dec/=2;
	}
}
*/

void fetch()
{
	x=inst_mem[pc/4];
	pc+=4;
}

void decode()
{
	//hex to bin
	for(int i=31;i>=0;i--)
	{
		if(x%2==0)
			bin[i]=0;
		else
			bin[i]=1;
		x/=2;
	}

	for(int i=25;i<32;i++)
		op[i-25]=bin[i];

	if(bintodec(op,6)==51)
	{
		//R-type: add

		for(int i=0;i<7;i++)
			func7[i]=bin[i];

		for(int i=7;i<12;i++)
			rs2[i-7]=bin[i];
		
		for(int i=12;i<17;i++)
			rs1[i-12]=bin[i];
		
		for(int i=17;i<20;i++)
			func3[i-17]=bin[i];
		
		for(int i=20;i<25;i++)
			rd[i-20]=bin[i];
	}

	if(bintodec(op,6)==3)
	{
		//I-type: ld

		if(bin[0]==0)
		{
			for(int i=0;i<12;i++)
				imme[i]=bin[i];

			tfi=bintodec(imme,11);
		}

		else if(bin[0]==1)
		{
			for(int i=0;i<12;i++)
			{
				if(bin[i]==0)
					imme[i]=1;
				else
					imme[i]=0;
			}
			tfi_temp=bintodec(imme, 11)+1;
			tfi=-tfi_temp;
		}

		for(int i=12;i<17;i++)
			rs1[i-12]=bin[i];
		
		for(int i=17;i<20;i++)
			func3[i-17]=bin[i];
		
		for(int i=20;i<25;i++)
			rd[i-20]=bin[i];
	}

	if(bintodec(op,6)==19)
	{
		//I-type: addi
		//int imme[12], rs2[5], rs1[5], func3[3], rd[5];

		if(bin[0]==0)
		{
			for(int i=0;i<12;i++)
				imme[i]=bin[i];

			tfi=bintodec(imme,11);
		}
	
		else if(bin[0]==1)
		{
			for(int i=0;i<12;i++)
			{
				if(bin[i]==0)
					imme[i]=1;
				else if(bin[i]==1)
					imme[i]=0;
			}
			
			tfi_temp=bintodec(imme,11)+1;
			tfi=-tfi_temp;
		}
		
		for(int i=12;i<17;i++)
			rs1[i-12]=bin[i];

		for(int i=17;i<20;i++)
			func3[i-17]=bin[i];

		for(int i=20;i<25;i++)
			rd[i-20]=bin[i];
	}

	if(bintodec(op,6)==35)
	{
		//S-type: sd

		if(bin[0]==0)
		{
			for(int i=0;i<7;i++)
				imme[i]=bin[i];
			for(int i=20;i<25;i++)
				imme[i-13]=bin[i];

			tfi=bintodec(imme,11);
		}

		else if(bin[0]==1)
		{
			for(int i=0;i<7;i++)
			{
				if(bin[i]=0)
					imme[i]=1;
				else
					imme[i]=0;
			}

			for(int i=20;i<25;i++)
			{
				if(bin[i]=0)
					imme[i-13]=1;
				else
					imme[i-13]=0;
			}

			tfi_temp=bintodec(imme,11)+1;
			tfi=-tfi_temp;
		}
		
		for(int i=7;i<12;i++)
			rs2[i-7]=bin[i];

		for(int i=12;i<17;i++)
			rs1[i-12]=bin[i];		

		for(int i=17;i<20;i++)
			func3[i-17]=bin[i];
	}

	if(bintodec(op, 6)==99)
	{
		//SB-type: beq
	
		if(bin[0]==0)
		{
			imme[0]=0;
			imme[1]=bin[24];
	
			for(int i=1;i<7;i++)
				imme[i+1]=bin[i];

			for(int i=20;i<24;i++)
				imme[i-12]=bin[i];

			tfi=bintodec(imme,11)*2;
		}

		else if(bin[0]==1)
		{
			imme[0]=0;

			if(bin[24]==1)
				imme[1]=0;
			else
				imme[1]=1;

			for(int i=1;i<7;i++)
			{
				if(bin[i]==1)
					imme[i+1]=0;
				else
					imme[i+1]=1;
			}

			for(int i=20;i<24;i++)
			{
				if(bin[i]==1)
					imme[i-12]=0;
				else
					imme[i-12]=1;
			}

			tfi_temp=bintodec(imme,11)+1;
			tfi=-(tfi_temp*2);
		}

		for(int i=7;i<12;i++)//24-20
			rs2[i-7]=bin[i];

		for(int i=12;i<17;i++)//19-15
			rs1[i-12]=bin[i];
		
		for(int i=17;i<20;i++)
			func3[i-17]=bin[i];
	}

	if(bintodec(op, 6) ==111)
	{
		//UJ-type: jal -> imme2[20]

		if(bin[0]==0)
		{
			imme2[0]=bin[0];
		
			for(int i=1;i<11;i++)
				imme2[i+9]=bin[i];

			imme2[9]=bin[11];

			for(int i=12;i<20;i++)
				imme2[i-11]=bin[i];

			tfi=bintodec(imme2,19)*2;
		}

		else if(bin[0]==1)
		{
			imme2[0]=0;

			for(int i=1;i<11;i++)
			{
				if(bin[i]==1)
					imme2[i+9]=0;
				else
					imme2[i+9]=1;
			}

			if (bin[11]==1)
				imme2[9]=0;
			else
				imme2[9]=1;

			for(int i=12;i<20;i++)
			{
				if(bin[i]==1)
					imme2[i-11]=0;
				else
					imme2[i-11]=1;
			}
			
			tfi_temp=bintodec(imme2,19)+1;
			tfi=-(tfi_temp*2);
		}

		for(int i=20;i<25;i++)
			rd[i-20]=bin[i];
	}

	if(bintodec(op,6)==103)
	{
		//I-type: jalr
		
		if(bin[0]==0)
		{
			for(int i=0;i<12;i++)
			{
				imme[i]=bin[i];
			}
			tfi=bintodec(imme,11)*2;
		}

		else if(bin[0]==1)
		{
			for(int i=0;i<12;i++)
			{
				if(bin[i]==1)
					imme[i]=0;
				else
					imme[i]=1;
			}

			tfi_temp=bintodec(imme,11)+1;
			tfi=-(tfi_temp*2);
		}

		for(int i=12;i<17;i++)
			rs1[i-12]=bin[i];

		for(int i=17;i<20;i++)
			func3[i-17]=bin[i];
		
		for(int i=20;i<25;i++)
			rd[i-20]=bin[i];
	}
}

//perform the appropriate operation 
void exe() 
{
	if(bintodec(op,6)==51)//add
		temp=regs[bintodec(rs2,4)]+regs[bintodec(rs1,4)];

	if(bintodec(op,6)==19)//addi
		temp=regs[bintodec(rs1,4)]+tfi;

	if(bintodec(op,6)==3)//ld
		temp=regs[bintodec(rs1,4)]+tfi;
	
	if(bintodec(op,6)==35)//sd
		temp=regs[bintodec(rs1,4)]+tfi;
	
	if(bintodec(op, 6)==99)//beq
	{
		temp=regs[bintodec(rs2,4)]-regs[bintodec(rs1,4)];

		if(temp==0)
		{
			pc-=4;
			pc+=tfi;
		}	
	}

	if(bintodec(op, 6)==111)//jal
	{
		regs[bintodec(rd,4)]=pc;
		pc-=4;
		pc+=tfi;
	}

	if(bintodec(op,6)==103)//jalr -rd:x0, rs1:x31
	{
		pc-=4;

		if(bintodec(rd,4)==0)
			regs[bintodec(rd,4)]=0;
		else
			regs[bintodec(rd,4)]=pc+4;

		pc=regs[bintodec(rs1,4)]+tfi;
	}
}

//access the data memory
void mem() 
{
	if(bintodec(op,6)==3)//ld
		temp=data_mem[temp];

	if(bintodec(op,6)==35) //sd
		data_mem[temp]=regs[bintodec(rs2,4)];
	
}

//write result of arithmetic operation or data read from the data memory if required
void wb() 
{
	if(bintodec(op,6)==51 || bintodec(op,6)==19)//add, addi
	{
		if(bintodec(rd,4)==0)
			regs[bintodec(rd,4)]=0;
		else
			regs[bintodec(rd,4)]=temp;
	}

	if(bintodec(op,6)==3) //ld
	{
		if(bintodec(rd,4)==0)
			regs[bintodec(rd,4)]=0;
		else
			regs[bintodec(rd,4)]=temp;
	}

	if(bintodec(rd,4)==0)
		regs[bintodec(rd,4)]=0;

}

int main(int ac, char* av[])
{

	if (ac < 3)
	{
		printf("./riscv_sim filename mode\n");
		return -1;
	}


	char done = 0;
	if (init(av[1]) != 0)
		return -1;

	while (!done)
	{

		fetch();
		decode();
		exe();
		mem();
		wb();


		cycles++;    //increase clock cycle

		//if debug mode, print clock cycle, pc, reg 
		if (*av[2] == '0') {
			print_cycles();  //print clock cycles
			print_pc();		 //print pc
			print_reg();	 //print registers
		}

		// check the exit condition, do not delete!! 
		if (regs[9] == 10)  //if value in $t1 is 10, finish the simulation
			done = 1;
	}

	if (*av[2] == '1')
	{
		print_cycles();  //print clock cycles
		print_pc();		 //print pc
		print_reg();	 //print registers
	}

	return 0;
}


/* initialize all datapat elements
//fill the instruction and data memory
//reset the registers
*/
int init(char* filename)
{
	FILE* fp = fopen(filename, "r");
	int i;
	long inst;

	if (fp == NULL)
	{
		fprintf(stderr, "Error opening file.\n");
		return -1;
	}

	/* fill instruction memory */
	i = 0;
	while (fscanf(fp, "%lx", &inst) == 1)
	{
		inst_mem[i++] = inst;
	}

	/*reset the registers*/
	for (i = 0; i < 32; i++)
	{
		regs[i] = 0;
	}

	/*reset pc*/
	pc = 0;

	/*reset clock cycles*/
	cycles = 0;

	return 0;
}

void print_cycles()
{
	printf("---------------------------------------------------\n");

	printf("Clock cycles = %lld\n", cycles);
}

void print_pc()
{
	printf("PC	   = %ld\n\n", pc);
}

void print_reg()
{
	printf("x0   = %lld\n", regs[0]);
	printf("x1   = %lld\n", regs[1]);
	printf("x2   = %lld\n", regs[2]);
	printf("x3   = %lld\n", regs[3]);
	printf("x4   = %lld\n", regs[4]);
	printf("x5   = %lld\n", regs[5]);
	printf("x6   = %lld\n", regs[6]);
	printf("x7   = %lld\n", regs[7]);
	printf("x8   = %lld\n", regs[8]);
	printf("x9   = %lld\n", regs[9]);
	printf("x10  = %lld\n", regs[10]);
	printf("x11  = %lld\n", regs[11]);
	printf("x12  = %lld\n", regs[12]);
	printf("x13  = %lld\n", regs[13]);
	printf("x14  = %lld\n", regs[14]);
	printf("x15  = %lld\n", regs[15]);
	printf("x16  = %lld\n", regs[16]);
	printf("x17  = %lld\n", regs[17]);
	printf("x18  = %lld\n", regs[18]);
	printf("x19  = %lld\n", regs[19]);
	printf("x20  = %lld\n", regs[20]);
	printf("x21  = %lld\n", regs[21]);
	printf("x22  = %lld\n", regs[22]);
	printf("x23  = %lld\n", regs[23]);
	printf("x24  = %lld\n", regs[24]);
	printf("x25  = %lld\n", regs[25]);
	printf("x26  = %lld\n", regs[26]);
	printf("x27  = %lld\n", regs[27]);
	printf("x28  = %lld\n", regs[28]);
	printf("x29  = %lld\n", regs[29]);
	printf("x30  = %lld\n", regs[30]);
	printf("x31  = %lld\n", regs[31]);
	printf("\n");
}
