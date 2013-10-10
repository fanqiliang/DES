#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include"table.h"

typedef struct reback {
    int data[64];
    int pdata[64];
    int kdata[56];
    int kdata1[48];
    int key_LL[28];
    int key_RR[28];
    int RR[32];
    int LL[32];
    int tempkdata[48];
    int temppdata[48];
    char plaintext[8];
    char ciphertext[8];
}Reback;

Reback *CharToBit(char *In, Reback *Back) {
    char a;
    int i;
    int j;

    for (i = 0; i < 8; i++) {
        a = In[i];
        for (j = 0; j < 8; j++) {
            Back->data[8*i+7-j] = a&1;
            a = a>>1;//(char)((int) a >> 1);
        }
    }
    return Back;
}

void BitToChar(Reback *out) {
 //   printf("i am here");
    int i;
    int j = 7;
    int k = 0;
    int x;
    
    char ch;
    for (i = 1; i <= 64; i++) {
       // printf("%d ",x);
        if (i % 8 == 0) {
            x += out->data[i-1]*pow(2,j);
            //printf("%d ",x);
            ch = (char)x;
            //printf("%c\n", ch);
            out->plaintext[k] = ch;
            k++;
            j = 7;
            x = 0;
        } else {
            x += out->data[i-1]*pow(2,j);
            j--;
        }
    }
}

void Initplaint(char *Plaint, Reback *Back) {
    Reback *plaintBit = NULL;
    plaintBit = CharToBit(Plaint, Back);
    int i;
    for (i = 0; i < 64; i++) {
        plaintBit->pdata[i] = plaintBit->data[IP_Table[i]-1];
     //   printf("%d ", Init[i]);
    }
}

void Initcipher(char *Ciphertext, Reback *Back) {
    Reback *cipherBit = CharToBit(Ciphertext, Back);
    int i;
    for (i = 0; i < 56; i++) {
        cipherBit->kdata[i] = cipherBit->data[PC1_Table[i]-1];
       // printf("%d ", Init[i]);
    }
}

void LoopLeftandsub(Reback *Back, int number) {
    int i;
    int j;
    int tempL;
    int tempR;
    for (i = 0; i < 28; i++) {
        Back->key_LL[i] = Back->kdata[i];
    }
    for (i = 28; i < 56; i++) {
        Back->key_RR[i] = Back->kdata[i];
    }
    for (i = 0; i < LOOP_Table[number]; i++) {
        tempL = Back->key_LL[0];
        tempR = Back->key_RR[0];
        for (j = 1; j < 28; j++) {
            Back->key_LL[j-1] = Back->key_LL[j];
            Back->key_RR[j-1] = Back->key_RR[j];
        }
        Back->key_LL[27] = tempL;
        Back->key_RR[27] = tempR;
    }
    for (i = 0; i < 28; i++) {
        Back->kdata[i] = Back->key_LL[i];
        Back->kdata[i+28] = Back->key_RR[i];
    }
    //substitute
    for (i = 0; i < 48; i++) {
        Back->kdata1[i] = Back->kdata[PC2_Table[i]-1];
    }
}

void XOR(int *plain, int *key, Reback *Back) {
    int i;
    for (i = 0; i < 48; i++) {
        Back->tempkdata[i] = (plain[i] + key[i]) % 2; 
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
    for (i = 0; i < 16; i++) {
        LoopLeftandsub(Back, i);
        for (j = 0; j < 32; j++) {
            Back->temppdata[j] = Back->RR[j]; 
        }
        for (j = 32; j < 48; j++) {
            Back->temppdata[j] = 0;
        }
        for (j = 0; j < 48; j++) {
            Back->temppdata[j] = Back->temppdata[E_Table[j]-1];
        }
        XOR(Back->temppdata, Back->kdata, Back);

    }

}

void main() {
    char In[100];
    printf("please input the plaintext:  ");
    scanf("%s",In);

    
    char name[] = "asdfggha";

   // char name1[] = "zxcvbnma";
   // int i;
    Reback *Back = (Reback *)malloc(sizeof(struct reback));
 //   Initplaint(name, Back);
//    Initcipher(name, Back);
/*    Back1 = CharToBit(name, Back);
    for (i = 0; i < 64; i++) {
        printf("%d ", Back1->data[i]);
    }
    printf("\n");
    BitToChar(Back1);
    for (i = 0; i < 8; i++) {
        printf("%c", Back1->plaintext[i]);
    }
    printf("\n");
    Initplaint(Back->pdata[i], )
*/
    return;
}
