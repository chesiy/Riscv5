//
// Created by admin on 2019/7/4.
//

#ifndef RISCV5_INSTRUCTIONDECODE_HPP
#define RISCV5_INSTRUCTIONDECODE_HPP

#include <iostream>
#include "instruction.hpp"

///根据IF里读到的指令类型，从作为右值的寄存器里读值（rs），
/// 解析立即数的值，然后赋给（src）；
class ID{
private:
    regist *reg;

public:
    inst instruction;
    bool cantdo;

    ID(regist *r):reg(r),cantdo(0){}
    bool setsrc(){
        switch (instruction.type){
            case AUIPC:
                if(reg->unpc)return false;
                instruction.src1=reg->getpc()-4;
                break;
            case JAL:
                if(reg->unpc)return false;
                instruction.src1=reg->getpc()-4;
                break;
            case JALR:
                if(reg->unpc)return false;
                instruction.src1=reg->getreg(instruction.rs1);
                instruction.src2=reg->getpc();
                break;
            case BEQ:case BNE:case BLT:
            case BGE:case BLTU:case BGEU:
                if(reg->unpc)return false;
                instruction.src1=reg->getreg(instruction.rs1);
                instruction.src2=reg->getreg(instruction.rs2);
                instruction.rd=reg->getpc();
                break;
            case LB:case LH:case LW:
            case LBU:case LHU:
                instruction.src1=reg->getreg(instruction.rs1);
                break;
            case SB:
            case SH:
            case SW:
                instruction.src1=reg->getreg(instruction.rs1);
                instruction.src2=reg->getreg(instruction.rs2);
                break;
            case ADDI:
                instruction.src1=reg->getreg(instruction.rs1);
                break;
            case SLTI:
            case SLTIU:
                instruction.src1=reg->getreg(instruction.rs1);
                break;
            case XORI:
            case ORI:case ANDI:
                instruction.src1=reg->getreg(instruction.rs1);
                break;
            case SLLI:case SRLI:case SRAI:
                instruction.src1=reg->getreg(instruction.rs1);
                break;
            case ADD:case SUB:case SLL:case SLT:case SLTU:
            case XOR:case SRL:case SRA:case OR:case AND:
                instruction.src1=reg->getreg(instruction.rs1);
                instruction.src2=reg->getreg(instruction.rs2);
                break;
            default:break;
        }
        return true;
    }

    void lockit(){//保护pc
        switch (instruction.type) {
            case JAL:
            case JALR:
            case BEQ:
            case BNE:
            case BLT:
            case BGE:
            case BLTU:
            case BGEU:
                reg->unpc++;
                break;
            default:
                break;
        }
    }

 /*   bool check(){
        switch(instruction.type){
            case AUIPC:
                if(reg->unpc)return false;
                break;
            case JAL:
                if(reg->unpc)return false;
                break;
            case JALR://???remain
                if((reg->unpc)|(instruction.rs1&&reg->unreg[instruction.rs1]))return false;
                break;
            case BEQ:case BNE:case BLT:
            case BGE:case BLTU:case BGEU:
                if((reg->unpc)|
                (instruction.rs1&&reg->unreg[instruction.rs1])|
                (instruction.rs2&&reg->unreg[instruction.rs2]))return false;
                break;
            case LB:case LH:case LW:
            case LBU:case LHU:
                if(instruction.rs1&&reg->unreg[instruction.rs1])return false;
                break;
            case SB:
            case SH:
            case SW:
                if((instruction.rs1&&reg->unreg[instruction.rs1])|
                (instruction.rs2&&reg->unreg[instruction.rs2]))return false;
                break;
            case ADDI:
            case SLTI:case SLTIU:
            case XORI:
            case ORI:case ANDI:
            case SLLI:case SRLI:case SRAI:
                if(instruction.rs1&&reg->unreg[instruction.rs1])return false;
                break;
            case ADD:case SUB:case SLL:case SLT:case SLTU:
            case XOR:case SRL:case SRA:case OR:case AND:
                if((instruction.rs1&&reg->unreg[instruction.rs1])|
                (instruction.rs2&&reg->unreg[instruction.rs2]))return false;
                break;
            default:break;
        }
        return true;
    }
   */
    void perform(){
        cantdo=false;
        if(instruction.type==LOCK)return;
        instruction.chooseType();
        instruction.setarg();
        if(!setsrc()){
            cantdo=true;
        }//需要等待了
        else lockit();
    }
    void go_on(EX &next){
        next.instruction=instruction;
        if(cantdo)next.instruction.type=LOCK;
    }
};

#endif //RISCV5_INSTRUCTIONDECODE_HPP
