//
// Created by Евгений on 08/09/2018.
//

#ifndef TESTRISK_CONST_H
#define TESTRISK_CONST_H

#endif //TESTRISK_CONST_H


#define MEMORY_SIZE 1000 //максимальный размер памяти
#define MAX_COMMAND 100  //максимально команд
#define REG 16 //количество регистров
#define MAXLENGTHLINE 64 //максимальная длина строки

#define ADD 0
#define ADDI 1
#define ADDU 2
#define SUB 3
#define SUBU 4
#define LW 5
#define SW 6
#define BEQ 7
#define J 8
#define JR 9
#define SLT 10


char *commandFileName; //имя файла с командами
char *machineFileName; //имя файла инициализации
int counter;
struct singleCommand {
    int command;
    int regA;
    int regB;
    int regC;
};