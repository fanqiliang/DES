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
    int RR[32];
    int LL[32];
    int tempdata[48];
    int tempdata1[48];
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
   /* for (i = 0; i < 64; i++) {
        printf("%d " ,Back->data[i]);
    }
    printf("\n");*/
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
         //   x += Back->data[i-1]*pow(2,j);
            printf("%d ",x);
            ch = (char)x;
            printf("%c\n", ch);
            Back->ciphertext[k] = ch;
            k++;
            j = 7;
            x = 0;
        } else {
          //  x += Back->data[i-1]*pow(2,j); 
            x += Back->cipher[i-1]*pow(2,j);
            j--;
        }
    }
}

void Initplaint(char *Plaint, Reback *Back) {
    CharToBit(Plaint, Back);
    int i;
   /* for (i = 0; i < 64; i++) {
        printf("%d ",Back->data[i]);
    }
    printf("\n");*/
    //int i;
    for (i = 0; i < 64; i++) {
        Back->pdata[i] = Back->data[IP_Table[i]-1];
    }
}

void Initcipher(char *Ciphertext, Reback *Back) {
    CharToBit(Ciphertext, Back);
    int i;
   /* for (i = 0; i < 64; i++) {
        printf("%d ",Back->data[i]);
    }
    printf("\n");*/
    //int i;
    /*for (i = 0; i < 64; i++) {
        printf("%d " ,Back->data[i]);
    }
    printf("\n");*/
   // int i;
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
    int key_LL[28];
    int key_RR[28];
    int data[56];
    for (i = 0; i < 28; i++) {
        key_LL[i] = Back->kdata[i];
        key_RR[i] = Back->kdata[i+28];
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
            Back->kdata1[i][j] = data[PC2_Table[j]-1];
            //printf("%d ",Back->kdata1[i]);
        }
    }
    //printf("\n");
}

void XOR(Reback *Back, int num) {
    int i;
   /* int j;
    printf("i am here\n");
    for (j = 0; j < 48; j++) { 
    printf("%d ", Back->tempdata[j]);}
    printf("\n");
    for (j = 0; j < 48; j++) { 
    printf("%d ", Back->kdata1[num][j]);}
    printf("\n");*/
    //printf("k = %d", num);
    for (i = 0; i < 48; i++) {
        Back->tempdata1[i] = Back->tempdata[i] ^ Back->kdata1[num][i];
       // printf("%d ", Back->tempkdata[i]);
    }
    /*printf("i am here 123456\n");
    for (j = 0; j < 48; j++) { 
    printf("%d ", Back->tempdata1[j]);}
    printf("\n");*/
}

void XOR1(Reback *Back) {
    int i;
    int j;
    int temp1[32];

    for (i = 0; i < 32; i++) {
        temp1[i] = Back->RR[i];
        Back->RR[i] = Back->LL[i] ^ Back->temp[i]; 
        Back->LL[i] = temp1[i];
    }

 /*   for (j = 0; j < 32; j++) { 
    printf("%d ", Back->LL[j]);}
    printf("\n");*/
}
void S_Func(int In[48], Reback *Back) {
    int i = 0;
    int j = 0;
    int k = 0;
    int x = 0;
    int y = 0;
    int t;
    int temp[32];
 
    for (i = 0; i < 8; i++) {
        x = In[i*6]*2 + In[i*6+5];
        y = In[i*6+1]*8 + In[i*6+2]*4 + In[i*6+3]*2 + In[i*6+4];
        t = S_Box[i][x][y];
        for (j = 0; j < 4; j++) {
            k = i*4 + j;
            temp[k] = SS[t][j];            
        }
    }
    for (i = 0; i < 32; i++) {
        Back->temp[i] = temp[P_Table[i]-1];
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
    
 //   printf("i am here");
    LoopLeftandsub(Back);
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 48; j++) {
            Back->tempdata[j] = Back->RR[E_Table[j]-1];
        }    
   /* for (j = 0; j < 32; j++) { 
    printf("%d ", Back->RR[j]);}
    printf("\n");
    for (j = 0; j < 32; j++) { 
    printf("%d ", Back->LL[j]);}
    printf("\n");*/
        XOR(Back, i);
        S_Func(Back->tempdata1, Back);
        XOR1(Back);
    } 
    for (i = 0; i < 32; i++) {
   /* for (i = 0; i < 64; i++) {
        printf("%d " ,Back->data[i]);
    }
    printf("\n");*/
        Back->cipher[i+32] = Back->LL[i];
        Back->cipher[i] = Back->RR[i];
    }
    
    /*for (i = 0; i < 64; i++) {
        printf("%d ",Back->cipher[i]);
    }
    printf("\n");*/
    for (i = 0; i < 64; i++) {
        Back->cipher[i] = Back->cipher[IPR_Table[i]-1];
    }
    for (i = 0; i < 64; i++) {
        printf("%d ",Back->cipher[i]);
    }
    printf("\n");
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
            Back->tempdata[j] = Back->RR[E_Table[j]-1];
        }
        XOR(Back, k);
        S_Func(Back->tempdata1, Back);
        XOR1(Back);
        k--;
       // printf(" i = %d", i);
       // printf(" k = %d", k);
    }
    for (i = 0; i < 32; i++) {
        Back->cipher[i+32] = Back->LL[i];
        Back->cipher[i] = Back->RR[i];
    }
    for (i = 0; i < 64; i++) {
        Back->cipher[i] = Back->cipher[IPR_Table[i]-1];
    }
   /* for (i = 0; i < 64; i++) {
        printf("%d ", Back->cipher[i]);
    }
    printf("\n");*/
    BitToChar(Back);
}

void main() {
 //   char In[100];

    char name[8] = "testdata";
    char key[8] = "mydeskey";
   // printf("i am here");
   // char name1[] = "zxcvbnma";
 //   int i;
    Reback *Back = (Reback *)malloc(sizeof(struct reback));
    Encryption(name, key, Back);
   /* Back->cipher[0] = 1;
    Back->cipher[1] = 1;
    Back->cipher[2] = 1;
    Back->cipher[3] = 1;
    Back->cipher[4] = 0;
    Back->cipher[5] = 1;
    Back->cipher[6] = 0;
    Back->cipher[7] = 1;
    Back->cipher[8] = 0;
    Back->cipher[9] = 0;
    Back->cipher[10] = 1;
    Back->cipher[11] = 0;
    Back->cipher[12] = 1;
    Back->cipher[13] = 0;
    Back->cipher[14] = 0;
    Back->cipher[15] = 0;
    Back->cipher[16] = 0;
    Back->cipher[17] = 0;
    Back->cipher[18] = 1;
    Back->cipher[19] = 1;
    Back->cipher[20] = 1;
    Back->cipher[21] = 0;
    Back->cipher[22] = 0;
    Back->cipher[23] = 1;
    Back->cipher[24] = 0;
    Back->cipher[25] = 1;
    Back->cipher[26] = 0;
    Back->cipher[27] = 1;
    Back->cipher[28] = 1;
    Back->cipher[29] = 1;
    Back->cipher[30] = 1;
    Back->cipher[31] = 0;

    Back->cipher[32] = 0;
    Back->cipher[33] = 1;
    Back->cipher[34] = 1;
    Back->cipher[35] = 0;
    Back->cipher[36] = 0;
    Back->cipher[37] = 1;
    Back->cipher[38] = 1;
    Back->cipher[39] = 0;
    Back->cipher[40] = 1;
    Back->cipher[41] = 0;
    Back->cipher[42] = 0;
    Back->cipher[43] = 0;
    Back->cipher[44] = 0;
    Back->cipher[45] = 0;
    Back->cipher[46] = 1;
    Back->cipher[47] = 1;
    Back->cipher[48] = 0; 
    Back->cipher[49] = 1;
    Back->cipher[50] = 0;
    Back->cipher[51] = 0;
    Back->cipher[52] = 0;
    Back->cipher[53] = 1;
    Back->cipher[54] = 0;
    Back->cipher[55] = 0;
    Back->cipher[56] = 1;
    Back->cipher[57] = 1;
    Back->cipher[58] = 0;
    Back->cipher[59] = 1;
    Back->cipher[60] = 1;
    Back->cipher[61] = 0;
    Back->cipher[62] = 0;
    Back->cipher[63] = 1;*/

 //   Desection(Back);
    
   // free(Back);
   // Initplaint(name, Back);
  //  BitToChar(Back);
   // Initcipher(name, Back);
   // CharToBit(name, Back);
   // for (i = 0; i < 64; i++) {
     //   printf("%d ", Back->data[i]);
    //}
    //printf("\n");
   /* BitToChar(Back);
    for (i = 0; i < 8; i++) {
        printf("%c", Back->plaintext[i]);
    }
    printf("\n");*/
    return;
}
