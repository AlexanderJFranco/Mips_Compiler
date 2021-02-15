#include <stdio.h>
#include <string.h>


char * getreg(int a);
int get_opcode(int instruction,int start,int bits);

void get_reg(int hold);
void print(int set[]);

typedef struct instructions{

	int inst;
	char type;
	int rs;
	int rd;
	int rt;
	int imm;


}instructions;

void stall(instructions a[],int ins);

typedef struct IF{

	int inst ;
	int PC_PL;

}IF;

typedef struct ID{

	int inst;
	int PC_PL;
	int branch;
	int rs,rd,rt;
	int readData1,readData2;
	int imm;
}ID;

typedef struct EX{
	int inst;
	int ALU;
	int write_data,write_reg;
	int rt;
}EX;

typedef struct MEM{
	int inst;
	int mem_data;
	int alu_data;
	int  reg;
}MEM;


typedef struct State{

	IF IF_ID;
	ID ID_EX;
	EX EX_MEM;
	MEM MEM_WB;

}State;

void showstate(State target);

int main(){
instructions mips[100];
int data[100];
int j;
for(j=0;j<100;j++)
data[j]=0;
int regf[32];
for(j=0;j<32;j++)
regf[j]=0;


int memory[32];
for(j=0;j<32;j++)
memory[j]=0;


State pre,post;
//Branch_Predictor Search[100];
int branch=0;
int cycles=0;
int stalls=0;
int branches=0;
int misses=0;


char *token;
char word[10];
char store[80];
int count=0;
int data_index=0;
int is_data=0;

while(fgets(store,80,stdin)){
	token=strtok(store,"\n");
	if(token!=NULL&&is_data==0){
	data[count++]=atoi(token);
	
	}
	if(token==NULL&&is_data==0)
	is_data=1;
	
	if(token!=NULL&&is_data==1){
	memory[data_index]=atoi(token);
	data_index++;
	}



	

}
int i=0;
for(i=0;i<count;i++){


if(data[i]==1){
mips[i].type='I';
mips[i].imm=32;
}
else if(data[i]!=1){
int opcode=get_opcode(data[i],26,6);
	mips[i].inst=opcode;
	if(opcode==0){
	opcode =(data[i]<<26)>>26;
	mips[i].type='R';
	mips[i].inst=opcode;
	}
	else
	mips[i].type='I';

}//end else	
	
/*
	EXTRACT REGISTERS DEPENDING ON INSTRUCTION TYPE

*/

	int temp;
	if(mips[i].type=='I'){
	
	mips[i].rs=get_opcode(data[i],21,5);
	mips[i].rt=get_opcode(data[i],16,5);
//	mips[i].imm=get_opcode(data[i],0,16);
//	printf("BASE: %d\n RT: %d\n OFFSET: %d\n",mips[i].rs,mips[i].rt,mips[i].imm);
		}


	if(mips[i].type=='R'){
	      
//	mips[i].rs=get_opcode(data[i],21,5);
	mips[i].rt=get_opcode(data[i],16,5);
//	mips[i].rd=get_opcode(data[i],11,5);
//	mips[i].imm=get_opcode(data[i],0,16);
//	printf("RS: %d\n RT: %d\n RD: %d\n",mips[i].inst,mips[i].rt,mips[i].rd);
	}
	






}//end for

int bool=0;

int limit=count+4;
for(i=0;i<limit;i++){
	printf("State at the beginning of cycle %d\n", cycles++);
	printf("\nData Memory:\n");
	print(memory);
	printf("\nRegisters:\n");
	print(regf);

if(i>=4){
		pre.MEM_WB.inst=0;
		pre.MEM_WB.mem_data=0;
		pre.MEM_WB.alu_data=0;
		pre.MEM_WB.reg=0;
		
		pre.MEM_WB.inst=pre.EX_MEM.inst;
		if(mips[i-4].inst!=1){
		pre.MEM_WB.alu_data=pre.EX_MEM.ALU;		

		if(mips[i-4].type=='R')			
		pre.MEM_WB.reg=mips[i-4].rd;
		else if(mips[i-4].type=='I')
		pre.MEM_WB.reg=mips[i-4].rt;


		if(mips[i-4].inst==32||mips[i-4].inst==-32){
		regf[mips[i-4].rd]=pre.MEM_WB.alu_data;
		pre.MEM_WB.reg=mips[i-4].rd;
		}

		if(mips[i-4].inst==34||mips[i-4].inst==-34){
		regf[mips[i-4].rd]=pre.MEM_WB.alu_data;
		pre.MEM_WB.reg=mips[i-4].rd;
		}

		if(mips[i-4].inst==12||mips[i-4].inst==-12){
		regf[mips[i-4].rt]=pre.MEM_WB.alu_data;
		pre.MEM_WB.reg=mips[i-4].rt;
		}
		
                if(mips[i-4].inst==13||mips[i-4].inst==-13){
		regf[mips[i-4].rt]=pre.MEM_WB.alu_data;
		pre.MEM_WB.reg=mips[i-4].rt;
		}                

		if(mips[i-4].inst==35||mips[i-4].inst==-35){
		int p;
		p=mips[i-4].imm/4;
		regf[mips[i-4].rt]=memory[regf[mips[i-4].rd]+p];
		pre.MEM_WB.mem_data=regf[mips[i-4].rt];
		pre.MEM_WB.alu_data=pre.EX_MEM.ALU;
		pre.MEM_WB.reg=mips[i-4].rt;

		}

		if(mips[i-4].inst==43||mips[i-4].inst==-43){
		int p;
		p=mips[i-4].imm/4;
		memory[regf[mips[i-4].rd]+p]=regf[mips[i-4].rt];
		pre.MEM_WB.reg=mips[i-4].rd;

		}		



	}//end if
		
		
			}//MEM/WB Check


	if(i>=3){
	
		pre.EX_MEM.inst=0;
		pre.EX_MEM.ALU=0;
		pre.EX_MEM.write_data=0;
		pre.EX_MEM.write_reg=0;
		pre.EX_MEM.rt=0;
		
		pre.EX_MEM.inst=pre.ID_EX.inst;
                
		if(mips[i-3].type=='I')
                pre.EX_MEM.write_reg=mips[i-3].rt;
		else if(mips[i-3].type=='R')
                pre.EX_MEM.write_reg=mips[i-3].rd;

		if(mips[i-3].inst==35||mips[i-3].inst==-35)
		pre.EX_MEM.ALU=pre.ID_EX.imm+regf[pre.ID_EX.rs];

		if(mips[i-3].inst==32||mips[i-3].inst==-32)
		pre.EX_MEM.ALU=regf[mips[i-3].rs]+regf[mips[i-3].rt];
		

		if(mips[i-3].inst==12||mips[i-3].inst==-12)
		pre.EX_MEM.ALU=regf[mips[i-3].rs]&mips[i-3].imm;
		
		if(mips[i-3].inst==13||mips[i-3].inst==-13){
		pre.EX_MEM.ALU=regf[mips[i-3].rs]|mips[i-3].imm;
		pre.EX_MEM.write_reg=mips[i-3].rt;
		pre.EX_MEM.write_data=regf[mips[i-3].rt];
		}

		if(mips[i-3].inst==34||mips[i-3].inst==-34)
		pre.EX_MEM.ALU=regf[mips[i-3].rs]-regf[mips[i-3].rt];		
		}//EX/MEM Check

	if(i>1){
		pre.ID_EX.inst=0;
		pre.ID_EX.PC_PL=0;
		pre.ID_EX.branch=0;
		pre.ID_EX.rs=0;
		pre.ID_EX.rd=0;
		pre.ID_EX.rt=0;
		pre.ID_EX.readData1=0;
		pre.ID_EX.readData2=0;
		pre.ID_EX.imm=0;
		pre.ID_EX.inst=	pre.IF_ID.inst;
		pre.ID_EX.PC_PL=(i-2)*4+4;		
	

		if(mips[i-2].inst!=0)
		pre.ID_EX.branch=(get_opcode(data[i-2],0,16)*4+pre.ID_EX.PC_PL);
		pre.ID_EX.rs=mips[i-2].rs;		
		pre.ID_EX.rt=mips[i-2].rt;
		pre.ID_EX.rd=mips[i-2].rd;
		pre.ID_EX.imm=mips[i-2].imm;
		
		if(mips[i-2].type=='I'){
		pre.ID_EX.readData1=regf[pre.ID_EX.rs];
		}
		else if(mips[i-2].type=='R'){
		pre.ID_EX.readData1=regf[pre.ID_EX.rs];
		pre.ID_EX.readData2=regf[pre.ID_EX.rt];
		}


		}//ID/EX  Check

	if(i>0){
		pre.IF_ID.inst=data[i-1];
	pre.IF_ID.PC_PL=((i-1)*4)+4;
	}//IF/ID check
	else{
		pre.IF_ID.inst=0;
		pre.IF_ID.PC_PL=0;
	
	}//IF/Id

  showstate(pre);
              if(mips[i-3].inst==35||mips[i-3].inst==-35){
		
		int index=0;
                        if(mips[i-2].rs==mips[i-3].rt||mips[i-2].rt==mips[i-3].rt){
			pre.ID_EX.inst=0;
			int x,t1[100],t2;
			

			for(x=0;x<100;x++){
			t1[x]=data[index];
			if(x==i+1){
			t1[x]=0;
			t1[++x]=data[index];
			}
			index++;
			}	
			for(x=0;x<100;x++)
			data[x]=t1[x];
			}
                        stall(mips,(i-1));

			
                bool=1;

                        }

               
if(bool==1){
for(i=0;i<count;i++)
printf("\n%d | %d\n",mips[i].inst,data[i]);
bool=0;
i--;
}



//showstate(pre);
}//end for

}

int get_opcode(int instruction,int start,int bits){

	int result = (instruction)>>start&((1<<bits)-1);
	return result;

}



void print(int set[])
{
int a;
for(a=0;a<16;a++)
printf("\tdataMem[%d] = %d\tdataMem[%d] = %d\n",a,set[a],(a+16),set[a+16]);

}

void showstate(State target){

printf("\nIF/ID:\n\tInstruction: %d\n\tPCPlus4: %d\n",target.IF_ID.inst,target.IF_ID.PC_PL);

printf("\nID/EX:\n\tInstruction: %d\n\tPCPlus4: %d\n\tbranchTarget: %d\n\treadData1: %d\n\treadData2: %d\n\timmed: %d",target.ID_EX.inst,target.ID_EX.PC_PL,target.ID_EX.branch,target.ID_EX.readData1,target.ID_EX.readData2,target.ID_EX.imm);
printf("\n\trs: %s\n\trt: %s\n\trd: %s",getreg(target.ID_EX.rs),getreg(target.ID_EX.rt),getreg(target.ID_EX.rd));

printf("\nEX/MEM\n\tInstruction: %d\n\taluResult: %d\n\twriteDataReg: %d\n\twriteReg: %s",target.EX_MEM.inst,target.EX_MEM.ALU,target.EX_MEM.write_data,getreg(target.EX_MEM.write_reg));

printf("\nMEM/WB\n\tInstruction: %d\n\twriteDataMem: %d\n\twriteDataALU: %d\n\twriteReg: %s",target.MEM_WB.inst,target.MEM_WB.mem_data,target.MEM_WB.alu_data,getreg(target.MEM_WB.reg));

printf("\n*********************************\n");
}

char * getreg(int a){

	if(a==0)	return"0";
	if(a==8)	return"t0";
	if(a==9)	return"t1";
        if(a==10)       return"t2";
        if(a==11)       return"t3";
        if(a==12)       return"t4";
        if(a==13)       return"t5";
        if(a==14)       return"t6";
	if(a==15)       return"t7";
	if(a==16)	return"s0";
        if(a==17)       return"s1";
        if(a==18)       return"s2";
        if(a==19)       return"s3";
        if(a==20)       return"s4";
        if(a==21)       return"s5";
        if(a==22)       return"s6";
        if(a==23)       return"s7";
}


void stall(instructions a[],int ins){
instructions temp[100];



int i,x=0;
	for(i=ins;i<99;i++){
		temp[i].inst=a[x].inst;
		temp[i].imm=a[x].imm;
		temp[i].rd=a[x].rd;
		temp[i].rt=a[x].rt;
		temp[i].rs=a[x].rs;
		temp[i].type=a[x].type;
		if(i==ins){
		temp[i+1].inst=a[x].inst;
		temp[i+1].imm=a[x].imm;
		temp[i+1].rt=a[x].rt;
		temp[i+1].rs=a[x].rs;
		temp[i+1].rd=a[x].rd;
		temp[i+1].type=a[x].type;
		temp[i].inst=0;
		temp[i].imm=0;
		temp[i].rt=0;
		temp[i].rd=0;
		temp[i].rs=0;
		
		i++;
		}		
	for(i=0;i<99;i++){
	a[i].inst=temp[i].inst;
	a[i].rt=temp[i].rt;
	a[i].rs=temp[i].rs;
	a[i].rd=temp[i].rd;
	a[i].imm=temp[i].imm;
	a[i].type=temp[i].type;
	}
	}

}
