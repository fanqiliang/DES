#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include"table.h"

int FLAG = 1;
int RoundKey[16][48];
int tmp1[32];
int cipher[8];
int  Out[64];

void CharToBit(char *In, int flag);
void BitToChar(int out[64]);
void Initplaint(int DATA[64]);
void Initcipher(int key_64[64]);
void LoopLeftandsub(int key_56[56]);
void F(int data[64]);
void S_Func(int B[8][6]);
void RLANDIP_1(int R[32], int L[32]);

void main() {
    int i;
    char ch;
    char name[8] = "testdata";
    char key[8] = "mydeskey";
  
    printf("i am here\n");
    CharToBit(key, 0);
    CharToBit(name, 1);
    FLAG = 0;
    CharToBit(key, 0);
    Initplaint(Out);
    for (i = 0; i < 8; i++) {
        ch = (char)cipher[i];
        printf("%c ", ch);
    }
    printf("\n");
   

    return;
}


void CharToBit(char *In, int flag) {
    char a;
    int i;
    int j;
    int key_64[64],DATA[64];
    if (flag == 0) {
        for (i = 0; i < 8; i++) {
            a = In[i];
            for (j = 0; j < 8; j++) {
                key_64[8*i+7-j] = a&1;
                a = (char)((int) a >> 1);
            }
        }
        Initcipher(key_64);
    }
    if (flag == 1) {
        for (i = 0; i < 8; i++) {
            a = In[i];
            for (j = 0; j < 8; j++) {
                DATA[8*i+7-j] = a&1;
                a = (char)((int) a >> 1);
            }
        }
        Initplaint(DATA);
    }
}

void Initcipher(int key_64[64]) {
    int i;
    int key_56[56];
    for (i = 0; i < 56; i++) {
        key_56[i] = key_64[PC1_Table[i]-1];
    }
    LoopLeftandsub(key_56);
}

void Initplaint(int DATA[64]) {
    int i;
    int data[64];
    for (i = 0; i < 64; i++) {
        data[i] = DATA[IP_Table[i]-1];
    }
    F(data);
}

void LoopLeftandsub(int key_56[56]) {
    int i;
    int j;
    int k;
    int tempL;
    int tempR;
    int key_LL[28];
    int key_RR[28];
    int data[56];
    int key[16][48];
    for (i = 0; i < 28; i++) {
        key_LL[i] = key_56[i];
        key_RR[i] = key_56[i+28];
    }
    for (i = 0; i < 16; i++) {
        for (j = 0; j < LOOP_Table[i]; j++) {
            tempL = key_LL[0];
            tempR = key_RR[0];
            for (k = 1; k < 28; k++) {
                key_LL[k-1] = key_LL[k];
                key_RR[k-1] = key_RR[k];
            }
            key_LL[27] = tempL;
            key_RR[27] = tempR;
        }
        for (j = 0; j < 28; j++) {
            data[j] = key_LL[j];
            data[j+28] = key_RR[j];
        }

        //substitute
        for (j = 0; j < 48; j++) {
            key[i][j] = data[PC2_Table[j]-1];
        }
    }
    if (FLAG == 0) {
        for (i = 0; i < 16; i++) {
            for (j = 0; j < 48; j++)
                RoundKey[i][j] = key[15-i][j];
        }
    }
    else {
        for (i = 0; i < 16; i++) {
            for (j = 0; j < 48; j++) {
                RoundKey[i][j] = key[i][j];
            }
        }
    }
}

void F(int data[64]) {
    int i;
    int j;
    int k;
    int l;
    int R_48[48];
    int R1[48];
    int L[17][32];
    int R[17][32];
    int B[8][6];
    for (i = 0; i < 32; i++) {
        L[0][i] = data[i];
    }
    for (i = 32; i < 64; i++) {
        R[0][i-32] = data[i];
    }
    for (k = 0; k < 16; k++) {
        for (i = 0; i < 48; i++) {
            R1[i] = R[k][E_Table[i]-1];
        }
        for (j = 0; j < 48; j++) {
            R_48[j] = R1[j] ^ RoundKey[k][j];
        }
        j = 0;
        for (i = 0; i < 48; i++) {
            if ((i%6 == 0) && (i != 0)) {
                j++;
            }
            B[j][i%6] = R_48[i];
        }
        S_Func(B);
        for (l = 0; l < 32; l++) {
            R[k+1][l] = L[k][l]^tmp1[l];
        }
        for (l = 0; l < 32; l++) {
            L[k+1][l] = R[k][l];
        }       
    }
    RLANDIP_1(R[16], L[16]);
}

void S_Func(int B[8][6]) {
    int i;
    int j;
    int k;
    int x;
    int y;
    int t;
    int temp[32];
 
    for (i = 0; i < 8; i++) {
        j = 0;
        x = B[i][0]*2 + B[i][5];
        y = B[i][1]*8 + B[i][2]*4 + B[i][3]*2 + B[i][4];
        t = S_Box[i][x][y];
        for (j = 0; j < 4; j++) {
            k = i*4 + j;
            temp[k] = SS[t][j];            
        }
    }
    for (i = 0; i < 32; i++) {
        tmp1[i] = temp[P_Table[i]-1];
    }
}

void RLANDIP_1(int R[32], int L[32]) {
        int i;
        int tmp2[64];
       // int out[64];
        for (i = 0; i < 32; i++) {
            tmp2[i] = R[i];
            tmp2[i+32] = L[i];
        }
        for (i = 0; i < 64; i++) {
            Out[i] = tmp2[IPR_Table[i]-1];
            printf("%d ", Out[i]);
        }
        printf("\n");
        BitToChar(Out);
    }

void BitToChar(int out[64]) {
    int i; 
    int j = 0;
    int k;
    int x;
    char ch;
    for (i = 0; i < 8; i++) {
            x = 0;
            k = 7;
            for (k = 7; k >=0; k--) { 
                x += (int)(out[j] * pow(2,k));
                j++;
            }
            cipher[i] = x;


    }
}
