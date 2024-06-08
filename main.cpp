#include <bits/stdc++.h>
#include <fstream>

using namespace std;

int main()
{
    unsigned int num_ins=0, num_ari=0, num_log=0, num_shi=0, num_mem=0, num_con=0, num_hal=0, num_tot=0, num_dat=0, num_cons=0, num_li=0; //variables for output.txt
    float num_cyc = 0;
    unsigned __int8 pc=0;
    string ins[257] = {""};
    unsigned int data[257] = {0}; //input arrays
    unsigned int reg[17]={0};
    unsigned int creg[17]={0}; //register counter array to see how many instructions is the register in at a moment
    fstream ifile;


    //------------------------------------ *TAKING INPUTS* ----------------------------------


    ifile.open("./input/ICache.txt", ios::in); //taking instruction input from ICache.txt
    for(unsigned int i=0;i<256;i++)
    {
        string s;
        ifile >> s;
        ins[i] = s;
    }
    ifile.close();
    ifile.open("./input/DCache.txt", ios::in); //taking data input from DCache.txt
    for(unsigned int i=0;i<256;i++)
    {
        string s;
        ifile >> s;
        if(s[0]>='0' && s[0]<='9') data[i]+=16*(s[0]-'0'); //hex to dec conversion
        else data[i]+=16*(s[0]-'a' + 10);
        if(s[1]>='0' && s[1]<='9') data[i]+=(s[1]-'0');
        else data[i]+=(s[1]-'a' + 10);
    }
    ifile.close();
    ifile.open("./input/RF.txt", ios::in); //taking registers input from RF.txt
    for(unsigned int i=0;i<16;i++)
    {
        string s;
        ifile >> s;
        if(s[0]>='0' && s[0]<='9') reg[i]+=16*(s[0]-'0'); //hex to dec conversion
        else reg[i]+=16*(s[0]-'a' + 10);
        if(s[1]>='0' && s[1]<='9') reg[i]+=(s[1]-'0');
        else reg[i]+=(s[1]-'a' + 10);
    }
    ifile.close();
    //variables initialisation
    string s1="pp",s2="pp";
    uint8_t rdm=0,rd=0,r1=0,r2=0,imm=0,imm2=0,flag = 0,dat=0;
    string fet1="pp",fet2="pp",dec1="pp",dec2="pp",al1="pp",al2="pp",mem1="pp",mem2="pp",wb1="pp",wb2="pp"; //Instruction forwarding variables
    queue<uint8_t> ALU_OP; //queue for managing the clock cycle gap between pushing and writing back
    int count=0; //no. of cycles


    //-------------------------------- *COMMENCING EXECUTION* -------------------------------------


    while(wb1[0]!='f') //if halt reaches write back, exit the program
    {
        count++;
        if(flag==1) num_cons++; //control stall

        //--------------------------------- *WRITE BACK* --------------------------------

        {
            wb1 = mem1; //instruction forwarding
            wb2 = mem2;
            mem1 = "pp";
            mem2 = "pp";
            if(wb1[0]=='0' || wb1[0]=='1' || wb1[0]=='2' || wb1[0]=='4' || wb1[0]=='5' || wb1[0]=='6') //ADD,SUB,MUL,XOR,OR,AND
            {
                if(wb1[1] >= '0' && wb1[1] <= '9') rdm = wb1[1]-'0'; //destination register
                else rdm = wb1[1]-'a'+10;

                reg[rdm] = ALU_OP.front();
                ALU_OP.pop();

                creg[rdm]--; //written back so count of that reg goes down by 1
            }
            else if(wb1[0]=='8' || wb1[0]=='9' || wb1[0]=='b' || wb1[0]=='c') 
            {
                if(wb1[0]=='8' || wb1[0]=='9') //SLL, SRL
                {
                    if(wb1[1] >= '0' && wb1[1] <= '9') rdm = wb1[1]-'0';
                    else rdm = wb1[1]-'a'+10;

                    reg[rdm] = ALU_OP.front();
                    ALU_OP.pop();

                    creg[rdm]--;
                }
                else
                {
                    if(wb1[0]=='b') //LD
                    {
                        if(wb1[1] >= '0' && wb1[1] <= '9') rdm = wb1[1]-'0';
                        else rdm = wb1[1]-'a'+10;

                        reg[rdm] = dat;

                        creg[rdm]--;
                    }
                }
            }
            else if(wb1[0]=='3') //INC
            {
                if(wb1[1] >= '0' && wb1[1] <= '9') rdm = wb1[1]-'0';
                else rdm = wb1[1]-'a'+10;

                reg[rdm] = ALU_OP.front();
                ALU_OP.pop();

                creg[rdm]--;
            }
            else if(wb1[0]=='7') //NOT
            {
                if(wb1[1] >= '0' && wb1[1] <= '9') rdm = wb1[1]-'0';
                else rdm = wb1[1]-'a'+10;

                reg[rdm] = ALU_OP.front();
                ALU_OP.pop();

                creg[rdm]--;
            }
            else if(wb1[0]=='a') //LI
            {
                if(wb1[1] >= '0' && wb1[1] <= '9') rdm = wb1[1]-'0';
                else rdm = wb1[1]-'a'+10;

                if(wb2[0] >= '0' && wb2[0] <= '9') imm2 = 16*(wb2[0]-'0');
                else imm2 = 16*(wb2[0]-'a'+10);

                if(wb2[1] >= '0' && wb2[1] <= '9') imm2 += wb2[1]-'0';
                else imm2 += wb2[1]-'a'+10;

                reg[rdm] = imm2;
                
                creg[rdm]--;
            }
        }

        //---------------------------------- *MEMORY-OPERATIONS* --------------------------------

        if(mem1[0]!='f') //NOT ENTERING IF HALT
        {
            if(al1[0]=='f') //STOPPING FURTHER EXECUTION
            {
                mem1 = al1;
                mem2 = al2;
                continue;
            }
            mem1 = al1;
            mem2 = al2;
            al1 = "pp";
            al2 = "pp";
            if(mem1[0]=='b') //LD
            {
                dat = data[ALU_OP.front()]; //calculating the value to be loaded into rd from data array
                ALU_OP.pop();
            }
            else if(mem1[0]=='c') //ST
            {
                if(mem1[1] >= '0' && mem1[1] <= '9') rdm = mem1[1]-'0';
                else rdm = mem1[1]-'a'+10;

                data[ALU_OP.front()] = reg[rdm]; //storing rd's value at data[rs+imm]
                ALU_OP.pop();

                //creg[rdm]--;
            }
            
            if(mem1[0] == 'd' || mem1[0] == 'e') //control instruction
            {
                flag = 0; //RESUMING FETCHING 
            }
        }

        //------------------------------------- *EXECUTION* -----------------------------------

        if(al1[0]!='f') //NOT ENTERING IF HALT
        {
            if(dec1[0]=='f') //STOPPING FURTHER DECODING
            {
                al1 = dec1;
                al2 = dec2;
                continue;
            }
            al1 = dec1;
            al2 = dec2;
            dec1 = "pp";
            dec2 = "pp";
            if(al1[0]=='0' || al1[0]=='1' || al1[0]=='2' || al1[0]=='4' || al1[0]=='5' || al1[0]=='6') //ADD,SUB,MUL,XOR,OR,AND
            {
                num_ins++;
                if(al1[0]=='0' || al1[0]=='1' || al1[0]=='2') num_ari++;
                else if(al1[0]=='4' || al1[0]=='5' || al1[0]=='6') num_log++;
                if(al1[0]=='0')
                {
                    ALU_OP.push((reg[r1]+reg[r2])&0xff); //& with 0xff to constrict to 8 bits
                }
                else if(al1[0]=='1')
                {
                    ALU_OP.push((reg[r1]-reg[r2])&0xff);
                }
                else if(al1[0]=='2')
                {
                    ALU_OP.push((reg[r1]*reg[r2])&0xff);
                }
                else if(al1[0]=='4')
                {
                    ALU_OP.push((reg[r1]&reg[r2])&0xff);
                }
                else if(al1[0]=='5')
                {
                    ALU_OP.push((reg[r1]|reg[r2])&0xff);
                }
                else if(al1[0]=='6')
                {
                    ALU_OP.push((reg[r1]^reg[r2])&0xff);
                }
            }
            else if(al1[0]=='8' || al1[0]=='9' || al1[0]=='b' || al1[0]=='c') //SLL,SRL,LD,ST
            {
                num_ins++; //no. of instructions
                if(al1[0]=='8') //SLL
                {
                    num_shi++; //shift instruction
                    ALU_OP.push((reg[r1]*(int)pow(2,imm))&0xff);
                }
                else if(al1[0]=='9') //SRL
                {
                    num_shi++; //shift instruction
                    ALU_OP.push((reg[r1]/(int)pow(2,imm))&0xff);
                }
                else if(al1[0]=='b' || al1[0]=='c') //LD,ST
                {
                    num_mem++; //memory instruction
                    if(al2[1]>='8' && al2[1]<='9') imm-=16;
                    else if(al2[1]>='a' && al2[1]<='f') imm-=16;
                    ALU_OP.push((reg[r1]+imm)&0xff);
                }
            }
            else if(al1[0]=='3') //INC
            {
                num_ins++; //no. of instruction
                num_ari++; //arithmetic instruction
                ALU_OP.push((reg[rd]+1)&0xff);
            }
            else if(al1[0]=='7') //NOT
            {
                num_ins++; //no. of instruction
                num_log++; //logical instruction
                ALU_OP.push((~reg[r1])&0xff);
            }
            else if(al1[0]=='d') //JMP
            {
                num_ins++; //no. of instruction
                num_con++; //control instruction
                pc+=(2*imm); //updating program counter
            }
            else if(al1[0]=='e') //BEQZ
            {
                num_ins++; //no. of instruction
                num_con++; //control instruction
                if(reg[r1]==0)
                {   
                    pc+=(2*imm); //updating program counter only if reg[rs]==0
                }
            }
        }

        //----------------------------- *INSTRUCTION DECODE* ---------------------------------

        if(flag!=1 && dec1[0]!='f') //NOT ENTERING IF HALT OR CONTROL INSTRUCTION IS EXECUTING
        {
            if(fet1[0]=='f') //STOPPING FURTHER FETCHING
            {
                dec1 = fet1;
                dec2 = fet2;
                continue;
            }
            dec1 = fet1;
            dec2 = fet2;
            fet1 = "pp";
            fet2 = "pp";
            if(dec1[0]=='0' || dec1[0]=='1' || dec1[0]=='2' || dec1[0]=='4' || dec1[0]=='5' || dec1[0]=='6') //ADD,SUB,MUL,XOR,OR,AND
            {
                if(dec1[1] >= '0' && dec1[1] <= '9') rd = dec1[1]-'0';
                else rd = dec1[1]-'a'+10;

                if(dec2[0] >= '0' && dec2[0] <= '9') r1 = dec2[0]-'0';
                else r1 = dec2[0]-'a'+10;

                if(dec2[1] >= '0' && dec2[1] <= '9') r2 = dec2[1]-'0';
                else r2 = dec2[1]-'a'+10;
                

                if(creg[r1]>0 || creg[r2]>0) //checking if r1, r2 are already undergoing an instruction. if yes, then data stall
                {   
                    num_dat++; //data stall
                    fet1 = dec1; //storing the current instruction in fetch; to be resumed after data stall gets over
                    fet2 = dec2;
                    dec1 = "pp";
                    dec2 = "pp";
                    continue;
                }
                creg[rd]++; //count increase as it enters an instruction in which value is to be changed or written
            }
            else if(dec1[0]=='8' || dec1[0]=='9' || dec1[0]=='b' || dec1[0]=='c') //SLL,SRL,LD,ST
            {
                if(dec1[1] >= '0' && dec1[1] <= '9') rd = dec1[1]-'0';
                else rd = dec1[1]-'a'+10;

                if(dec2[0] >= '0' && dec2[0] <= '9') r1 = dec2[0]-'0';
                else r1 = dec2[0]-'a'+10;

                if(dec2[1] >= '0' && dec2[1] <= '9') imm = dec2[1]-'0';
                else imm = dec2[1]-'a'+10;

                if(dec1[0]=='b' || dec1[0]=='c')
                {
                    imm = imm&0xff;
                }

                if(dec1[0]=='c') //no need to increase rd's count as we are just checking it's value so we have to check for data stall
                {
                    if(creg[r1]>0 || creg[rd]>0) //data stall
                    {
                        num_dat++; //data stall
                        fet1 = dec1;
                        fet2 = dec2;
                        dec1 = "pp";
                        dec2 = "pp";
                        continue;
                    }
                }

                else //in SLL,SRL,and LD we have to increase count of rd as it's getting written back and check data stall for r1 only
                {
                    if(creg[r1]>0) //data stall
                    {
                        num_dat++; //data stall
                        fet1 = dec1;
                        fet2 = dec2;
                        dec1 = "pp";
                        dec2 = "pp";
                        continue;
                    }
                    creg[rd]++;
                }
            }
            else if(dec1[0]=='3') //INC
            {
                if(dec1[1] >= '0' && dec1[1] <= '9') rd = dec1[1]-'0';
                else rd = dec1[1]-'a'+10;

                if(creg[rd]>0) //data stall
                {
                    num_dat++; //data stall
                    fet1 = dec1;
                    fet2 = dec2;
                    dec1 = "pp";
                    dec2 = "pp";
                    continue;
                }

                creg[rd]++;
            }
            else if(dec1[0]=='7') //NOT
            {
                if(dec1[1] >= '0' && dec1[1] <= '9') rd = dec1[1]-'0';
                else rd = dec1[1]-'a'+10;

                if(dec2[0] >= '0' && dec2[0] <= '9') r1 = dec2[0]-'0';
                else r1 = dec2[0]-'a'+10;

                if(creg[r1]>0) //data stall
                {
                    num_dat++; //data stall
                    fet1 = dec1;
                    fet2 = dec2;
                    dec1 = "pp";
                    dec2 = "pp";
                    continue;
                }
                creg[rd]++;
            }
            else if(dec1[0]=='a') //LI
            {
                num_ins++; //no. of instructions
                num_li++; //load immediate instruction
                if(dec1[1] >= '0' && dec1[1] <= '9') rd = dec1[1]-'0';
                else rd = dec1[1]-'a'+10;

                if(dec2[0] >= '0' && dec2[0] <= '9') imm = 16*(dec2[0]-'0');
                else imm = 16*(dec2[0]-'a'+10);

                if(dec2[1] >= '0' && dec2[1] <= '9') imm += dec2[1]-'0';
                else imm += dec2[1]-'a'+10;

                creg[rd]++;
            }
            else if(dec1[0]=='d') //JMP
            {
                if(dec1[1] >= '0' && dec1[1] <= '9') imm = 16*(dec1[1]-'0');
                else imm = 16*(dec1[1]-'a'+10);

                if(dec2[0] >= '0' && dec2[0] <= '9') imm += (dec2[0]-'0');
                else imm += (dec2[0]-'a'+10);

                flag=1; //setting flag = 1 to start control stall till PC gets updated
            }
            else if(dec1[0]=='e') //BEQZ
            {
                if(dec1[1] >= '0' && dec1[1] <= '9') r1 = (dec1[1]-'0');
                else r1 = (s1[1]-'a'+10);
            
                if(dec2[0] >= '0' && dec2[0] <= '9') imm = 16*(dec2[0]-'0');
                else imm = 16*(s2[0]-'a'+10);

                if(dec2[1] >= '0' && dec2[1] <= '9') imm += (dec2[1]-'0');
                else imm += (dec2[1]-'a'+10);

                if(creg[r1]>0) //data stall
                {
                    num_dat++; //data stall
                    fet1 = dec1;
                    fet2 = dec2;
                    dec1 = "pp";
                    dec2 = "pp";
                    continue;
                }

                flag=1; //setting flag = 1 to start control stall till PC gets updated
            }
        }

        //------------------------------ *INSTRUCTION FETCH* ---------------------------------

        if(flag!=1 && fet1[0]!='f') //NOT ENETRING IF HALT OR CONTROL INSTRUCTION IS EXECUTING
        {
            //first 2 lines make up the full instruction (2 consecutive array elements to be accessed at a time)
            s1 = ins[pc]; //taking the first line (first array element)
            s2 = ins[pc+1]; //taking the second line (first+1 array element)
            pc+=2; 
            fet1 = s1;
            fet2 = s2;
        }
    }


    //-----------------------*PROGRAM EXECUTED*-------------------------------


    num_hal++; //halt instruction
    num_ins++; //no. of instructions(for halt)
    num_cyc = ((1.0)*count)/num_ins; //cycles per instruction
    num_tot = num_cons + num_dat; //total stalls = control stalls + data stalls
    ofstream ofile;
    ofile.open("./output/Output.txt",ios::out); //outputting in Output.txt
    ofile << "Total number of instructions executed : " << num_ins << endl;
    ofile << "Number of instructions in each class" << endl;
    ofile << "Arithmetic instructions : " << num_ari << endl;
    ofile << "Logical instructions : " << num_log << endl;
    ofile << "Shift instructions : " << num_shi << endl;
    ofile << "Memory instructions : " << num_mem << endl;
    ofile << "Load immediate instructions : " << num_li << endl;
    ofile << "Control instructions : " << num_con << endl;
    ofile << "Halt instructions : " << num_hal << endl;
    ofile << "Cycles per instruction : " << num_cyc << endl;
    ofile << "Total number of stalls : " << num_tot << endl;
    ofile << "Data Stalls (RAW) : " << num_dat << endl;
    ofile << "Control Stalls : " << num_cons << endl;
    ofile.close();
    ofile.open("./output/DCache.txt",ios::out); //outputting in DCache.txt with the updated data values
    for(int i=0;i<256;i++)
    {
        if(data[i]>15) ofile << hex << data[i] << endl;
        else ofile << "0" << hex <<  data[i] << endl;
    }
    ofile.close();

    //------------------------------------ *END* ---------------------------------------
}