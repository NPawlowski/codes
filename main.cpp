#include <iostream>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <cstdio>

using namespace std;

int row=0, col=0;
bool fromFile = true;

// tablica pc1
int pc1[56] = {
	57,49,41,33,25,17,9,
	1,58,50,42,34,26,18,
	10,2,59,51,43,35,27,
	19,11,3,60,52,44,36,
	63,55,47,39,31,23,15,
	7,62,54,46,38,30,22,
	14,6,61,53,45,37,29,
	21,13,5,28,20,12,4
	};
// tablica pc2
int pc2[48] = {
	14,17,11,24,1,5,
	3,28,15,6,21,10,
	23,19,12,4,26,8,
	16,7,27,20,13,2,
	41,52,31,37,47,55,
	30,40,51,45,33,48,
	44,49,39,56,34,53,
	46,42,50,36,29,32
	};
// tablica przesuniec w lewo
int leftShifts[16] = {
    1, 1, 2, 2,
    2, 2, 2, 2,
    1, 2, 2, 2,
    2, 2, 2, 1
    };

int initial_permutation[64] = {
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6,
	64,56,48,40,32,24,16,8,
	57,49,41,33,25,17,9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7
	};

int expansion_table[48] = {
	32,1,2,3,4,5,4,5,
	6,7,8,9,8,9,10,11,
	12,13,12,13,14,15,16,17,
	16,17,18,19,20,21,20,21,
	22,23,24,25,24,25,26,27,
	28,29,28,29,30,31,32,1
	};

// Sboxes

int sBox1[4][16] = {
    {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
    {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
    {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
    {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
};

int sBox2[4][16] = {
    {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
    {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
    {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
    {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
};

int sBox3[4][16] = {
    {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
    {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
    {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
    {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
};

int sBox4[4][16] = {
    {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
    {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
    {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
    {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
};

int sBox5[4][16] = {
    {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
    {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
    {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
    {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
};

int sBox6[4][16] = {
    {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
    {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
    {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
    {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
};

int sBox7[4][16] = {
    {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
    {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
    {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
    {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
};

int sBox8[4][16] = {
    {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
    {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
    {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
    {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
};

// tablica do permutacji po sBox
int sP[32] = {
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25
};

// permutacja koncowa
int final_permutation[64]= {
	40,8,48,16,56,24,64,32,
	39,7,47,15,55,23,63,31,
	38,6,46,14,54,22,62,30,
	37,5,45,13,53,21,61,29,
	36,4,44,12,52,20,60,28,
	35,3,43,11,51,19,59,27,
	34,2,42,10,50,18,58,26,
	33,1,41,9,49,17,57,25
	};


void divideTable(int tab[48], int index){
    row = tab[index]*2 + tab[index+5];
    col = tab[index+1]*8 + tab[index+2]*4 + tab[index+3]*2 + tab[index+4];
}

int decimalToBinary(int temp, int result[4]){

    if(temp == 0){ result[0] = 0; result[1] = 0; result[2] = 0; result[3] = 0; }
    else if(temp == 1){ result[0] = 0; result[1] = 0; result[2] = 0; result[3] = 1; }
    else if(temp == 2){ result[0] = 0; result[1] = 0; result[2] = 1; result[3] = 0; }
    else if(temp == 3){ result[0] = 0; result[1] = 0; result[2] = 1; result[3] = 1; }
    else if(temp == 4){ result[0] = 0; result[1] = 1; result[2] = 0; result[3] = 0; }
    else if(temp == 5){ result[0] = 0; result[1] = 1; result[2] = 0; result[3] = 1; }
    else if(temp == 6){ result[0] = 0; result[1] = 1; result[2] = 1; result[3] = 0; }
    else if(temp == 7){ result[0] = 0; result[1] = 1; result[2] = 1; result[3] = 1; }
    else if(temp == 8){ result[0] = 1; result[1] = 0; result[2] = 0; result[3] = 0; }
    else if(temp == 9){ result[0] = 1; result[1] = 0; result[2] = 0; result[3] = 1; }
    else if(temp == 10){ result[0] = 1; result[1] = 0; result[2] = 1; result[3] = 0; }
    else if(temp == 11){ result[0] = 1; result[1] = 0; result[2] = 1; result[3] = 1; }
    else if(temp == 12){ result[0] = 1; result[1] = 1; result[2] = 0; result[3] = 0; }
    else if(temp == 13){ result[0] = 1; result[1] = 1; result[2] = 0; result[3] = 1; }
    else if(temp == 14){ result[0] = 1; result[1] = 1; result[2] = 1; result[3] = 0; }
    else if(temp == 15){ result[0] = 1; result[1] = 1; result[2] = 1; result[3] = 1; }

    return result[4];
}

void reset(int row, int col){
    row = 0;
    col = 0;
}

char binaryToHex(int binary[64], char result[16]){
    int temp=0;
    int l=0;

    for(int i=0; i<16; i++){
        for(int j=0; j<4; j++){
            if(binary[j+l] == 1 && j == 0) temp += 8;
            else if(binary[j+l] == 1 && j == 1) temp += 4;
            else if(binary[j+l] == 1 && j == 2) temp += 2;
            else if(binary[j+l] == 1 && j == 3) temp += 1;
        }
        l+=4;
        if(temp == 0) result[i] = '0';
        else if(temp == 1) result[i] = '1';
        else if(temp == 2) result[i] = '2';
        else if(temp == 3) result[i] = '3';
        else if(temp == 4) result[i] = '4';
        else if(temp == 5) result[i] = '5';
        else if(temp == 6) result[i] = '6';
        else if(temp == 7) result[i] = '7';
        else if(temp == 8) result[i] = '8';
        else if(temp == 9) result[i] = '9';
        else if(temp == 10) result[i] = 'A';
        else if(temp == 11) result[i] = 'B';
        else if(temp == 12) result[i] = 'C';
        else if(temp == 13) result[i] = 'D';
        else if(temp == 14) result[i] = 'E';
        else if(temp == 15) result[i] = 'F';

        temp = 0;
    }

    return result[16];

}


int hexToInt(char text[16], int result[8]){

    int buffer[16], j=0;

    for(int i=0; i<16; i++){
        if(text[i] == '0') buffer[i] = 0;
        else if(text[i] == '1') buffer[i] = 1;
        else if(text[i] == '2') buffer[i] = 2;
        else if(text[i] == '3') buffer[i] = 3;
        else if(text[i] == '4') buffer[i] = 4;
        else if(text[i] == '5') buffer[i] = 5;
        else if(text[i] == '6') buffer[i] = 6;
        else if(text[i] == '7') buffer[i] = 7;
        else if(text[i] == '8') buffer[i] = 8;
        else if(text[i] == '9') buffer[i] = 9;
        else if(text[i] == 'A') buffer[i] = 10;
        else if(text[i] == 'B') buffer[i] = 11;
        else if(text[i] == 'C') buffer[i] = 12;
        else if(text[i] == 'D') buffer[i] = 13;
        else if(text[i] == 'E') buffer[i] = 14;
        else if(text[i] == 'F') buffer[i] = 15;
    }

    for(int i=0; i<8; i++){
        result[i] = buffer[j+1] + buffer[j]*16;
        j+=2;
    }

    return result[8];

}


char DES(char plainText[16], int code_version, int key[8]){

    char finalResult[16];
    int z=63, tmp[64], keyToInt[8];
    int binaryKey[64], x=8, y=7;
    int keyToInt2[8], binaryKey2[64], x2=8, y2=7;
    int pc1Table[56], left[28], right[28];
    int buffer1l, buffer2l, buffer1r, buffer2r;
    int subKey[16][48], initialMessage64[64], sk56[56];
    int left2[32], right2[32], buffer48[48], expansion48[48], sBoxResolution[32], sBoxResolution2[32];
    int number1[4], number2[4], number3[4], number4[4], number5[4], number6[4], number7[4], number8[4];
    int sBoxNum1, sBoxNum2, sBoxNum3, sBoxNum4, sBoxNum5, sBoxNum6, sBoxNum7, sBoxNum8;
    int newRight[32], newLeft[32], finalText[64], finalText2[64];


    for(int i=0; i<8; i++) keyToInt[i] = key[i];

    for(int j=0; j<8; j++){
        while(x != 0){
            if(keyToInt[j]%2 == 0) binaryKey[y] = 0;
            else binaryKey[y] = 1;
            keyToInt[j] /= 2;
            x--;
            y--;
        }
        x=8;
        y+=16;
    }

    // pierwsza permutacja


    for(int k=0; k<56; k++){
        pc1Table[k] = binaryKey[pc1[k]-1];
    }

    // podzial na prawy i lewy
    for(int l=0; l<28; l++){
        left[l] = pc1Table[l];
        right[l] = pc1Table[l+28];
    }

    // 16 X Subkey

    for(int z=0; z<16; z++){

            if(leftShifts[z] == 1){

                buffer1l = left[0];
                buffer1r = right[0];

                for(int l=0;l<28;l++){
                    left[l] = left[l+leftShifts[z]];
                    right[l] = right[l+leftShifts[z]];
                }
                left[27] = buffer1l;
                right[27] = buffer1r;
            }
            else{
                buffer1l = left[0];
                buffer2l = left[1];
                buffer1r = right[0];
                buffer2r = right[1];

                for(int l=0;l<28;l++){
                    left[l] = left[l+leftShifts[z]];
                    right[l] = right[l+leftShifts[z]];
                }
                left[26] = buffer1l;
                left[27] = buffer2l;
                right[26] = buffer1r;
                right[27] = buffer2r;
            }

        for(int s=0; s<28; s++){
            sk56[s] = left[s];
            sk56[s+28] = right[s];
        }


        for(int k=0; k<48; k++){
            subKey[z][k] = sk56[pc2[k]-1];
        }

        buffer1l = 0;
        buffer1r = 0;
        buffer2l = 0;
        buffer2r = 0;

    }

    //////////////////////////////////// CZESC DRUGA - MESSAGE BLOCK ////////////////////////////////////////

    if(!fromFile){
        hexToInt(plainText,keyToInt2);
    }
    else{
        for(int i=0; i<8; i++){
            keyToInt2[i] = (int)plainText[i];
        }
    }

    for(int j=0; j<8; j++){
        while(x2 != 0){
            if(keyToInt2[j]%2 == 0) binaryKey2[y2] = 0;
            else binaryKey2[y2] = 1;
            keyToInt2[j] /= 2;
            x2--;
            y2--;
        }
        x2=8;
        y2+=16;
    }

    // poczatkowa permutacja wiadomosci


    for(int j=0; j<64; j++){
        initialMessage64[j] = binaryKey2[initial_permutation[j]-1];
    }

    // podzial na lewy i prawy

    for(int k=0; k<32; k++){
        left2[k] = initialMessage64[k];
        right2[k] = initialMessage64[k+32];
    }


    // 16 powtorzen dla kazdego podklucza

    if(code_version == 1){
    for(int i=0; i<16; i++){

        // prawa strona
        for(int j=0; j<48; j++){
            buffer48[j] = right2[expansion_table[j]-1];
        }

        for(int k=0; k<48; k++){
            if(buffer48[k] == subKey[i][k]) expansion48[k] = 0;
            else expansion48[k] = 1;
        }


        // 8 razy wybieranie z tabeli sBox

        divideTable(expansion48,0);
        sBoxNum1 = sBox1[row][col];
        decimalToBinary(sBoxNum1,number1);
        reset(row,col);

        divideTable(expansion48,6);
        sBoxNum2 = sBox2[row][col];
        decimalToBinary(sBoxNum2,number2);
        reset(row,col);

        divideTable(expansion48,12);
        sBoxNum3 = sBox3[row][col];
        decimalToBinary(sBoxNum3,number3);
        reset(row,col);

        divideTable(expansion48,18);
        sBoxNum4 = sBox4[row][col];
        decimalToBinary(sBoxNum4,number4);
        reset(row,col);

        divideTable(expansion48,24);
        sBoxNum5 = sBox5[row][col];
        decimalToBinary(sBoxNum5,number5);
        reset(row,col);

        divideTable(expansion48,30);
        sBoxNum6 = sBox6[row][col];
        decimalToBinary(sBoxNum6,number6);
        reset(row,col);

        divideTable(expansion48,36);
        sBoxNum7 = sBox7[row][col];
        decimalToBinary(sBoxNum7,number7);
        reset(row,col);

        divideTable(expansion48,42);
        sBoxNum8 = sBox8[row][col];
        decimalToBinary(sBoxNum8,number8);
        reset(row,col);

        for(int d=0; d<4; d++){
            sBoxResolution[d] = number1[d];
            sBoxResolution[d+4] = number2[d];
            sBoxResolution[d+8] = number3[d];
            sBoxResolution[d+12] = number4[d];
            sBoxResolution[d+16] = number5[d];
            sBoxResolution[d+20] = number6[d];
            sBoxResolution[d+24] = number7[d];
            sBoxResolution[d+28] = number8[d];
        }

        for(int n=0; n<32; n++){
            sBoxResolution2[n] = sBoxResolution[sP[n]-1];
        }



        // XOR z lewa strona i zamiana prawej na nowa lewa
        for(int o=0; o<32; o++){
            if(left2[o] == sBoxResolution2[o]) newRight[o] = 0;
            else newRight[o] = 1;
            newLeft[o] = right2[o];
        }

        for(int p=0; p<32; p++){
            left2[p] = newLeft[p];
            right2[p] = newRight[p];
        }

    }
    }
    else{
        for(int i=15; i>=0; i--){

        // prawa strona
        for(int j=0; j<48; j++){
            buffer48[j] = right2[expansion_table[j]-1];
        }

        for(int k=0; k<48; k++){
            if(buffer48[k] == subKey[i][k]) expansion48[k] = 0;
            else expansion48[k] = 1;
        }


        // 8 razy wybieranie z tabeli sBox

        divideTable(expansion48,0);
        sBoxNum1 = sBox1[row][col];
        decimalToBinary(sBoxNum1,number1);
        reset(row,col);

        divideTable(expansion48,6);
        sBoxNum2 = sBox2[row][col];
        decimalToBinary(sBoxNum2,number2);
        reset(row,col);

        divideTable(expansion48,12);
        sBoxNum3 = sBox3[row][col];
        decimalToBinary(sBoxNum3,number3);
        reset(row,col);

        divideTable(expansion48,18);
        sBoxNum4 = sBox4[row][col];
        decimalToBinary(sBoxNum4,number4);
        reset(row,col);

        divideTable(expansion48,24);
        sBoxNum5 = sBox5[row][col];
        decimalToBinary(sBoxNum5,number5);
        reset(row,col);

        divideTable(expansion48,30);
        sBoxNum6 = sBox6[row][col];
        decimalToBinary(sBoxNum6,number6);
        reset(row,col);

        divideTable(expansion48,36);
        sBoxNum7 = sBox7[row][col];
        decimalToBinary(sBoxNum7,number7);
        reset(row,col);

        divideTable(expansion48,42);
        sBoxNum8 = sBox8[row][col];
        decimalToBinary(sBoxNum8,number8);
        reset(row,col);

        for(int d=0; d<4; d++){
            sBoxResolution[d] = number1[d];
            sBoxResolution[d+4] = number2[d];
            sBoxResolution[d+8] = number3[d];
            sBoxResolution[d+12] = number4[d];
            sBoxResolution[d+16] = number5[d];
            sBoxResolution[d+20] = number6[d];
            sBoxResolution[d+24] = number7[d];
            sBoxResolution[d+28] = number8[d];
        }

        for(int n=0; n<32; n++){
            sBoxResolution2[n] = sBoxResolution[sP[n]-1];
        }



        // XOR z lewa strona i zamiana prawej na nowa lewa
        for(int o=0; o<32; o++){
            if(left2[o] == sBoxResolution2[o]) newRight[o] = 0;
            else newRight[o] = 1;
            newLeft[o] = right2[o];
        }

        for(int p=0; p<32; p++){
            left2[p] = newLeft[p];
            right2[p] = newRight[p];
        }

    }
    }

    // odwrocenie prawej z lewa strona

    for(int r=0; r<32; r++){
        finalText[r] = right2[r];
        finalText[r+32] = left2[r];
    }

    for(int s=0; s<64; s++){
        finalText2[s] = finalText[final_permutation[s]-1];
    }

    binaryToHex(finalText2,finalResult);


    //if(code_version == 1)cout << " zakodowany tekst : ";
    //else cout << " zdekodowany tekst : ";
    for(int i=0; i<16; i++){
        //cout << finalResult[i];
        plainText[i] = finalResult[i];
    }

    //cout<<endl;

    return plainText[16];
}



////////////////////////////////////////////////////// main /////////////////////////////////////////////////////////////

int main()
{
    char plainText[16];
    int k1[8], k2[8], k3[8], key[24];
    int code_version = 1; // 1 encryption / 2 decryption

    srand(time(NULL));
    for(int i=0; i<24; i++) key[i] = rand() % 127;

    for(int i=0; i<8; i++){
        k1[i] = key[i];
        k2[i] = key[i+8];
        k3[i] = key[i+16];
    }

    cout<<"Klucz 1: "; for(int i=0; i<8; i++) cout<<(char)k1[i];cout<<endl;
    cout<<"Klucz 2: "; for(int i=0; i<8; i++) cout<<(char)k2[i];cout<<endl;
    cout<<"Klucz 3: "; for(int i=0; i<8; i++) cout<<(char)k3[i];cout<<endl;

    FILE* file = fopen("test.txt", "rb");
    if(!file) return 0;

    clock_t start = clock();

    while(!feof(file)){

        fromFile = true;
        fread(plainText,1,8,file);

        //for(int i=0; i<8; i++) cout<<(char)plainText[i];cout<<endl;

        code_version = 1;   // 1 = kodowanie
        DES(plainText,code_version,k1);

        fromFile = false;
        code_version = 2;   // 2 = dekodowanie
        DES(plainText,code_version,k2);

        code_version = 1;
        DES(plainText,code_version,k3);

        code_version = 2;
        DES(plainText,code_version,k3);

        code_version = 1;
        DES(plainText,code_version,k2);

        code_version = 2;
        DES(plainText,code_version,k1);

        //cout << endl;
        for(int i=0; i<16; i++)plainText[i]=0;

    }

    printf( "Czas wykonywania 3DES: %lu ms\n", clock() - start );



    return 0;
}
