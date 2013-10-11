#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include"table.h"

typedef struct reback {
    int data[64];
    int pdata[64];
    int cipher[64];
    int plaint[64];
    int kdata[56];
    int kdata1[16][48];
    int key_LL[28];
    int key_RR[28];
    int RR[32];
    int LL[32];
    int tempkdata[48];
    int temppdata[48];
    int temp[32];
    char plaintext[8];
    char ciphertext[8];
}Reback;

void CharToBit(char *In, Reback *Back) {
    char a;
    int i;
    int j;

    for (i = 0; i < 8; i++) {
        a = In[i];
        for (j = 0; j < 8; j++) {
            Back->data[8*i+7-j] = a&1;
            a = (char)((int) a >> 1);
        }
    }
}

void BitToChar(Reback *Back) {
    int i; 
    int j = 7;
    int k = 0;
    int x = 0;
    char ch;
    for (i = 1; i <= 64; i++) {
        if (i % 8 == 0) {
            x += Back->cipher[i-1]*pow(2,j);
          //  printf("%d ",x);
            ch = (char)x;
            printf("%c\n", ch);
            Back->ciphertext[k] = ch;
            k++;
            j = 7;
            x = 0;
        } else {
            x += Back->cipher[i-1]*pow(2,j);
            j--;
        }
    }
}

void Initplaint(char *Plaint, Reback *Back) {
    CharToBit(Plaint, Back);
    int i;
    for (i = 0; i < 64; i++) {
        Back->pdata[i] = Back->data[IP_Table[i]-1];
    }
}

void Initcipher(char *Ciphertext, Reback *Back) {
    CharToBit(Ciphertext, Back);
    int i;
    for (i = 0; i < 56; i++) {
        Back->kdata[i] = Back->data[PC1_Table[i]-1];
    }
}

void LoopLeftandsub(Reback *Back) {
    int i;
    int j;
    int k;
    int tempL;
    int tempR;
    for (i = 0; i < 28; i++) {
        Back->key_LL[i] = Back->kdata[i];
    }
    for (i = 28; i < 56; i++) {
        Back->key_RR[i] = Back->kdata[i];
    }
    for (i = 0; i < 16; i++) {
        for (j = 0; j < LOOP_Table[i]; j++) {
            tempL = Back->key_LL[0];
            tempR = Back->key_RR[0];
            for (k = 1; k < 28; k++) {
                Back->key_LL[k-1] = Back->key_LL[k];
                Back->key_RR[k-1] = Back->key_RR[k];
            }
            Back->key_LL[27] = tempL;
            Back->key_RR[27] = tempR;
        }
        for (j = 0; j < 28; j++) {
            Back->kdata[j] = Back->key_LL[j];
            Back->kdata[j+28] = Back->key_RR[j];
        }
        //substitute
        for (j = 0; j < 48; j++) {
            Back->kdata1[i][j] = Back->kdata[PC2_Table[j]-1];
            //printf("%d ",Back->kdata1[i]);
        }
    }
    //printf("\n");
}

void XOR(Reback *Back, int num) {
    int i;
    for (i = 0; i < 48; i++) {
        Back->tempkdata[i] = (Back->tempkdata[i] + Back->kdata1[num][i]) % 2;
       // printf("%d ", Back->tempkdata[i]);
    }
}

void XOR1(Reback *Back, int num) {
    int i;
    int temp1[32];

    for (i = 0; i < 32; i++) {
        temp1[i] = Back->RR[i];
        Back->RR[i] = (Back->LL[i] + Back->temp[i]) % 2; 
        Back->LL[i] = temp1[i];
    }
}
void S_Func(int *In, Reback *Back) {
    int i;
    int j;
    int k;
    int x;
    int y;
    int t;
    for (i = 0; i < 8; i++) {
        x = In[i*6]*2 + In[i*6+5];
        y = In[i*6+1]*8 + In[i*6+2]*4 + In[i*6+3]*2 + In[i*6+4];
        t = S_Box[i][x][y];
        for (j = 0; j < 4; j++) {
            k = i*4 + j;
            Back->temp[k] = SS[t][j];            
        }
    }
    for (i = 0; i < 32; i++) {
        Back->temp[i] = Back->temp[P_Table[i]-1];
    }
}
 

void Encryption(char *plain, char *key, Reback *Back) {
    int i;
    int j;
    Initplaint(plain, Back);
    Initcipher(key, Back);
    for (i = 0; i < 32; i++) {
        Back->LL[i] = Back->pdata[i];
        Back->RR[i] = Back->pdata[i+32];
    }
    printf("i am here");
    LoopLeftandsub(Back);
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 48; j++) {
            Back->temppdata[j] = Back->RR[E_Table[j]-1];
        }    
/*    for (j = 0; i < 48; j++) { 
    printf("%d ", Back->cipher[1]);}
    printf("\n");*/
        XOR(Back, i);
        S_Func(Back->tempkdata, Back);
        XOR1(Back, i);
    }
    for (j = 0; j < 32; j++) {
        Back->cipher[j] = Back->LL[j];
        Back->cipher[j+32] = Back->RR[j];
    }
    for (j = 0; j < 64; j++) {
        Back->cipher[j] = Back->cipher[IPR_Table[j]-1];
    }
    BitToChar(Back);
}

void Desection(Reback *Back) {
    int i;
    int j;
    int k = 15;
    for (i = 0; i < 64; i++) {
        Back->cipher[i] = Back->cipher[IP_Table[i]-1];
    } 
    for (i = 0; i < 32; i++) {
        Back->LL[i] = Back->cipher[i];
        Back->RR[i] = Back->cipher[i+32];
    }
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 48; j++) {
            Back->temppdata[j] = Back->RR[E_Table[j]-1];
        }
        XOR(Back, k);
        S_Func(Back->tempkdata, Back);
        XOR1(Back, i);
        k--;
    }
    for (i = 0; i < 32; i++) {
        Back->cipher[i] = Back->LL[i];
        Back->cipher[i+32] = Back->RR[i];
    }
    for (i = 0; i < 64; i++) {
        Back->cipher[i] = Back->cipher[IPR_Table[i]-1];
    }
    BitToChar(Back);
}

void main() {
    char In[100];

    char name[] = "asdfggha";
    char key[] = "zxcvbnma";
    printf("i am here");
   // char name1[] = "zxcvbnma";
    int i;
    Reback *Back = (Reback *)malloc(sizeof(struct reback));
    Encryption(name, key, Back);
    Desection(Back);
    free(Back);
    //Initplaint(name, Back);
   // Initcipher(name, Back);
//   Back1 = CharToBit(name, Back);
   // for (i = 0; i < 64; i++) {
     //   printf("%d ", Back->data[i]);
    //}
    //printf("\n");
 /*   BitToChar(Back1);
    for (i = 0; i < 8; i++) {
        printf("%c", Back1->plaintext[i]);
    }
    printf("\n");
    Initplaint(Back->pdata[i], )
*/
    return;
}
