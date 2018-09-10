#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "const.h"

//Перевод в десяичную обычного числа в registrA и registrB
int toDec(const char *binaryCode) {
    int decValue = 0;
    for (int i = 0; i < 7; i++) {
        if (binaryCode[i] == '1') {
            decValue = (int) (decValue + pow(2, 6 - i));
        }
    }
    return decValue;
}

//Перевод в десятичную обычного числа registrC
int toDecForBigC(const char *binaryCode) {
    int decValue = 0;
    for (int i = 0; i < 11; i++) {
        if (binaryCode[i] == '1') {
            decValue = (int) (decValue + pow(2, 10 - i));
        }
    }
    return decValue;
}

//Перевод в десяичную imm числа registrC
int toDecForImm(const char *binaryCode) {
    int decValue = 0;
    int sign = -1;
    for (int i = 1; i < 11; i++) {
        if (binaryCode[i] == '1') {
            decValue = (int) (decValue + pow(2, 10 - i));
        }
    }
    if (binaryCode[0] == '1') {
        decValue = decValue * sign;
    }
    return decValue;
}

void machineInit(FILE *file, int *fixMemory) {
    int a;
    fscanf(file, "%d", &a);
    int maxSize = a;
    for (int i = 0; i <= maxSize; i++) {
        fscanf(file, "%d", &a);
        fixMemory[i] = a;
    }
}

int risk(int counter, int currentCommand, int registerA, int registerB, int registerC,
         int *fixMemory, int *registersMemory) {

    switch (currentCommand) {

        /**ADD, складываем значения ячеек B и C и кладем в A*/
        case ADD : {
            registersMemory[registerA] = registersMemory[registerB] + registersMemory[registerC];
            counter++;
            break;
        }
            /**ADDI, складываем B и число C(отриц) и кладем в A*/
        case ADDI : {
            registersMemory[registerA] = registersMemory[registerB] + registerC;
            counter++;
            break;
        }
            /**ADDU, складываем B и число C(Без знака, убирает "-") и кладем в A*/
        case ADDU : {
            if (registerC < 0) {
                registersMemory[registerA] = registersMemory[registerB] - registerC;
            } else registersMemory[registerA] = registersMemory[registerB] + registerC;
            counter++;
            break;
        }
            /**SUB, вычитаем из значения ячейки B зн. ячейки C и кладем в A*/
        case SUB : {
            registersMemory[registerA] = registersMemory[registerB] - registersMemory[registerC];
            counter++;
            break;
        }

            /**SUBU, вычитаем из значения ячейки B число C (Без знака, убирает "-") и кладем в A*/
        case SUBU : {
            if (registerC < 0) {
                registersMemory[registerA] = registersMemory[registerB] + registerC;
            } else registersMemory[registerA] = registersMemory[registerB] - registerC;
            counter++;
            break;
        }
            /**LW, в ячейку A выгружаем данные из фикс памяти из ячейки с индексом С*/
        case LW : {
            registersMemory[registerA] = fixMemory[registerC];
            counter++;
            break;
        }
            /**SW, в ячейку фикс памяти с индексом С загружаем данные из ячейки А регистра*/
        case SW : {
            fixMemory[registerC] = registersMemory[registerA];
            counter++;
            break;
        }
            /**BEQ, если данные A и B равны, то перемещаемся на строку counter в C + 1, иначе +1*/
        case BEQ : {
            if (registersMemory[registerA] == registersMemory[registerB]) {
                counter = counter + 1 + registerC;
            } else {
                counter++;
            }
            break;
        }
            /**J, перемещаемся на строку counter в C */
        case J : {
            counter = counter + registerC;
            break;
        }
            /**JR, перемещаемся на строку значения регистра B */
        case JR : {
            counter = registersMemory[registerB]; //переместиться в позицию значения в регистре B
            break;
        }
            /**SLT, Если C меньше знач в B, то в А записываем С, иначе идем дальше */
        case SLT : {
            if (registerC < registersMemory[registerB]) {
                registersMemory[registerA] = registerC;
                counter++;
            } else {
                counter++;
            }
            break;
        }
        default:
            break;
    }
    return counter;
}

int nextStep() {
    int temp;
    while (true) {
        printf("Идём дальше? \n");
        printf("1 - да 0 - нет \n");
        scanf("%d", &temp);
        break;
    }
    return temp;
}

void resultDebuggerToFile(int step, FILE *resultFile, int currentCommand, int registerA, int registerB, int registerC,
                          int *registersMemory, int *fixMemory) {
    char *nameCommand = NULL;
    if (currentCommand == ADD) {
        nameCommand = "ADD";
    } else if (currentCommand == ADDI) {
        nameCommand = "ADDI";
    } else if (currentCommand == ADDU) {
        nameCommand = "ADDU";
    } else if (currentCommand == SUB) {
        nameCommand = "SUB";
    } else if (currentCommand == SUBU) {
        nameCommand = "SUBU";
    } else if (currentCommand == LW) {
        nameCommand = "LW";
    } else if (currentCommand == SW) {
        nameCommand = "SW";
    } else if (currentCommand == BEQ) {
        nameCommand = "BEQ";
    } else if (currentCommand == J) {
        nameCommand = "J";
    } else if (currentCommand == JR) {
        nameCommand = "JR";
    } else if (currentCommand == SLT) {
        nameCommand = "SLT";
    }
    fprintf(resultFile, "Шаг № %d \n", step);
    fprintf(resultFile, "%s regiserA: %d registerB: %d registerC: %d\n", nameCommand, registerA, registerB, registerC);
    for (int i = 0; i < REG; i++) {
        fprintf(resultFile, "Регистр[%d] = %d \n", i, registersMemory[i]);
    }
    fprintf(resultFile, "Данные в памяти машины RISC \n");
    for (int i = 0; i <= fixMemory[1]; i++) {
        fprintf(resultFile, "Адрес[%d] = %d \n", i, fixMemory[i]);
    }
}

void resultDebuggerToTerminal(int step, int currentCommand, int registerA, int registerB, int registerC,
                              int *registersMemory, int *fixMemory) {

    char *nameCommand = NULL;
    if (currentCommand == ADD) {
        nameCommand = "ADD";
    } else if (currentCommand == ADDI) {
        nameCommand = "ADDI";
    } else if (currentCommand == ADDU) {
        nameCommand = "ADDU";
    } else if (currentCommand == SUB) {
        nameCommand = "SUB";
    } else if (currentCommand == SUBU) {
        nameCommand = "SUBU";
    } else if (currentCommand == LW) {
        nameCommand = "LW";
    } else if (currentCommand == SW) {
        nameCommand = "SW";
    } else if (currentCommand == BEQ) {
        nameCommand = "BEQ";
    } else if (currentCommand == J) {
        nameCommand = "J";
    } else if (currentCommand == JR) {
        nameCommand = "JR";
    } else if (currentCommand == SLT) {
        nameCommand = "SLT";
    }
    printf("step %d \n", step);
    printf("%s registerA: %d registerB: %d registerC: %d\n", nameCommand, registerA, registerB, registerC);
    for (int i = 0; i < REG; i++) {
        printf("регистр[%d] = %d \n", i, registersMemory[i]);
    }
    printf("память машины \n");
    for (int i = 0; i <= fixMemory[1]; i++) {
        printf("адрес[%d] = %d \n", i, fixMemory[i]);
    }

}

int main(int argv, char **argc) {
    argv = 5;

    printf("Симулятор машины Risk FPGA \n");

    machineFileName = argc[1]; //"/Users/nolit/CLionProjects/testRisk/Machine.txt"; //здесь файл инициализации
    commandFileName = argc[2]; //здесь файл с командами

    int *fixMemory = malloc(sizeof(int) * MEMORY_SIZE); //выделяем место под фиксированную память
    struct singleCommand *command = malloc(
            sizeof(struct singleCommand) * MAX_COMMAND); //выделяем место под команды команды

    int registersMemory[REG]; //память регистров
    for (int i = 0; i < REG; i++) {
        registersMemory[i] = 0;
    }

    int countCommand = 0; //счет
    char *currentCommand; //текущий код команды
    char *argA;
    char *argB;
    char *argC;
    int line = 0; //номер строки

    char inputCommand[MAXLENGTHLINE]; //массив для команды
    FILE *commandInput;
    commandInput = fopen(commandFileName, "rt"); //Открытие текстового файла "Команды" на чтение

    //=====Начало считывания=====

    //        if (!strcmp("000", "000")) { //Если равны, то null, поэтому инвертируем чтобы If работал и применялся
    //            printf("true");
    //        } else printf("false");

    //если конца файла не достигнули, то 0. Ставим отрицание и получаем единицу. Пока 1 – работает
    //
    while (!feof(commandInput)) {
        //в массив inputCommand сохраняется команда, sizeof.. - максимальная длина, file – сам файл
        fgets(inputCommand, sizeof(inputCommand), commandInput);
//        printf("\n");
//        printf("InputCommand:");
//        printf("%s", inputCommand);
        currentCommand = strtok(inputCommand, " "); //разбиение строки на лексемы
//        printf("\n");
//        printf("currentCommand:");
//        printf("%s", currentCommand);

        if (!strcmp(currentCommand, "endFile")) {
            countCommand = line;
        } else {
            argA = strtok(NULL, " ");
            argB = strtok(NULL, " ");
            argC = strtok(NULL, "\n");

            if (!strcmp(currentCommand, "0000000")) {
                command[line].command = ADD;
                command[line].regC = toDecForBigC(argC);
                line++;
            } else if (!strcmp(currentCommand, "0000001")) {
                command[line].command = ADDI;
                command[line].regC = toDecForImm(argC);
                line++;
            } else if (!strcmp(currentCommand, "0000010")) {
                command[line].command = ADDU;
                command[line].regC = toDecForImm(argC);
                line++;
            } else if (!strcmp(currentCommand, "0000011")) {
                command[line].command = SUB;
                command[line].regC = toDecForBigC(argC);
                line++;
            } else if (!strcmp(currentCommand, "0000100")) {
                command[line].command = SUBU;
                command[line].regC = toDecForImm(argC);
                line++;
            } else if (!strcmp(currentCommand, "0000101")) {
                command[line].command = LW;
                command[line].regC = toDecForBigC(argC);
                line++;
            } else if (!strcmp(currentCommand, "0000110")) {
                command[line].command = SW;
                command[line].regC = toDecForBigC(argC);
                line++;
            } else if (!strcmp(currentCommand, "0000111")) {
                command[line].command = BEQ;
                command[line].regC = toDecForBigC(argC);
                line++;
            } else if (!strcmp(currentCommand, "0001000")) {
                command[line].command = J;
                command[line].regC = toDecForBigC(argC);
                line++;
            } else if (!strcmp(currentCommand, "0001001")) {
                command[line].command = JR;
                command[line].regC = toDecForBigC(argC);
                line++;
            } else if (!strcmp(currentCommand, "0001010")) {
                command[line].command = SLT;
                command[line].regC = toDecForBigC(argC);
                line++;
            } else if (!strcmp(currentCommand, "endFile")) {
                countCommand = line;
            }
            command[line].regA = toDec(argA);
            command[line].regB = toDec(argB);
        }
    }

    //Инициализация фиксированной памяти машины
    FILE *initInput;
    initInput = fopen(machineFileName, "rt");
    machineInit(initInput, fixMemory);
    counter = 0;

    //-------------Работа алгоритма----------------

    /**
     * 1 - Полная работа машины, вывод в result.txt
     * 2 - Полная работа машины, вывод работы по шагам в debugger
     * 3 – Указать место начала и далее пошаговая отладка через терминал
     * */


    if (!strcmp(argc[5], "1")) {

        printf("%s", argc[3]);
        printf("\n");
        printf("%s", argc[1]);

        while (counter != countCommand) {
            counter = risk(counter, command[counter].command,
                           command[counter].regA, command[counter].regB, command[counter].regC,
                           fixMemory, registersMemory);
        }

        char *resultName = argc[3];
        FILE *resultF;
        resultF = fopen(resultName, "wt");
        fwrite("Регистры: \n", strlen("Регистры: \n"), 1, resultF);
        for (int i = 0; i < REG; i++) {
            fprintf(resultF, "Регистр №[%d] = %d \n", i, registersMemory[i]);
        }
        fprintf(resultF, "Фиксированная память \n");
        for (int i = 0; i <= fixMemory[1]; i++) {
            fprintf(resultF, "Адрес памяти[%d] = %d \n", i, fixMemory[i]);
        }
        fclose(resultF);
    } else {
        if (!strcmp(argc[5], "2")) {
            char *resultDebugName = argc[4];
            FILE *resultDebug;
            resultDebug = fopen(resultDebugName, "wt");
            int stepCount = 0;
            printf("\n\n");
            printf("Введите число шагов: \n");
            int temp;
            scanf("%d", &temp);
            stepCount = temp;
            counter = 0;
            int stepCounter = 0;
            while (stepCounter != stepCount) {
                counter = risk(counter, command[counter].command,
                               command[counter].regA, command[counter].regB, command[counter].regC,
                               fixMemory, registersMemory);
                resultDebuggerToFile(stepCounter, resultDebug, command[counter].command, command[counter].regA,
                                     command[counter].regB, command[counter].regC, registersMemory, fixMemory);
                stepCounter++;
                if (counter == countCommand) {
                    break;
                }
            }
            fclose(resultDebug);
        } else {
            if (!strcmp(argc[5], "3")) {
                printf("\n\n");
                printf("Введите шаг, с которого начать: \n");
                int temp;
                scanf("%d", &temp);
                int stepCounter = 0;
                while (counter != temp) {
                    counter = risk(counter, command[counter].command,
                                   command[counter].regA, command[counter].regB, command[counter].regC,
                                   fixMemory, registersMemory);
                }
                while (counter != countCommand) {
                    if (nextStep() == 1) {
                        counter = risk(counter, command[counter].command,
                                       command[counter].regA, command[counter].regB, command[counter].regC,
                                       fixMemory, registersMemory);
                        resultDebuggerToTerminal(stepCounter, command[counter].command, command[counter].regA,
                                                 command[counter].regB, command[counter].regC, registersMemory,
                                                 fixMemory);
                        stepCounter++;
                    } else {
                        break;
                    }
                    if (counter == countCommand) {
                        printf("Конец\n");
                        break;
                    }
                }
            }


        }

        //освобождение памяти
        fclose(commandInput);
        fclose(initInput);
        free(fixMemory);
        free(command);
        return 0;
    }
}
