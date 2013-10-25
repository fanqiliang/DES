#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include<string.h>
#include"table.h"

int FLAG = 1;
int FLAG1 = 0;
int RoundKey[16][48];
int tmp1[32];
int cipher[32][8];
int  Out[32][64];
int  Out2[32][64];
int  Out1[64];
int PlaBit[64];
char KEY[8];
static int T = 0;
static int S = 0;

void CharToBit(char *In, int flag);
void BitToChar(int out[64]);
void Initplaint(int DATA[64]);
void Initcipher(int key_64[64]);
void LoopLeftandsub(int key_56[56]);
void F(int data[64]);
void S_Func(int B[8][6]);
void RLANDIP_1(int R[32], int L[32]);

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
        for (i = 0; i < 64; i++) {
            PlaBit[i] = DATA[i];
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
            R_48[j] = R1[j] ^ RoundKey[0][j];
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
    int j;
    int i;
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
        int out[64];
        for (i = 0; i < 32; i++) {
            tmp2[i] = R[i];
            tmp2[i+32] = L[i];
        }
        for (i = 0; i < 64; i++) {
            Out[T][i] = tmp2[IPR_Table[i]-1];
            out[i] = Out[T][i];
        }
        if (FLAG1 == 0) {
            T++;
        }
        BitToChar(out);
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
            cipher[S][i] = x;
    }
    if (FLAG1 == 1) {
        S++;
    }
}

void ECB(char *name, char *key,int t) {
    int len;
    int i = 0;
    int j;
    int m = 0;
    int n;
    int out[64];
    char tname[t][8];
    char name1[8*t];
    char name2[8];
    char ch1[200];
    char ch;
    FLAG1 = 0;
    S = 0;
    T = 0;
    len = strlen(name);
    while (name[i] != '\0') {
        name1[i] = name[i];
        i++;
    }
    if ((len/8) < t) {
        for (i = len; i < 8*t; i++) {
            name1[i] = 97; //填充数据，填充的为‘a’
        }
    }
    j = 0;
    //进行分组
    for (i = 0; i < t*8; i++) {
        if ((i%8 == 0) && (i != 0)) {
            j++;
        }
        tname[j][i%8] = name1[i];
    }
    //开始加密
    for (i = 0; i < t; i++) {
        CharToBit(key, 0);
        CharToBit(tname[i], 1);
    }
    for (i = 0; i < T; i++) {
        for (j = 0; j < 64; j++) {
            printf("%d ",Out[i][j]);
        }
        printf("\n");
    }
    printf("do you want to decrypt ECB?\n");
    printf("1.Decryption\n");
    printf("0.Exit\n");
    scanf("%d", &n);
    if (n == 1) {
        FLAG1 = 1;
        for (i = 0; i < T; i++) {
            for (j = 0; j < 64; j++) {
                out[j] = Out[i][j];
            } 
            CharToBit(key, 0);
            Initplaint(out);
        }
        for (i = 0; i < S; i++) {
            for (j = 0; j < 8; j++) {
                ch1[m] = (char)cipher[i][j];
                m++;
            }
        }
        for (i = 0; i < len; i++) {
            ch = ch1[i];

            printf("%c", ch);
        }
        printf("\n");
    }
    if (n == 0) {
        return;
    }
}

void CBC(char *name, char *key, char *IV, int t) {
    int len;
    int i = 0;
    int j;
    int m = 0;
    int n;
    int p = 0;
    int x = 0;
    int k;
    int out[64];
    char tname[t][8];
    char name1[8*t];
    char name2[8];
    int  temp[8];
    char ctemp[8];
    char ch1[200];
    char ch;
    FLAG1 = 0;
    S = 0;
    T = 0;
    len = strlen(name);
    while (name[i] != '\0') {
        name1[i] = name[i];
        i++;
    }
    //数据填充
    if ((len/8) < t) {
        for (i = len; i < 8*t; i++) {
            name1[i] = 97;
        }
    }
    j = 0;
    //分组
    for (i = 0; i < t*8; i++) {
        if ((i%8 == 0) && (i != 0)) {
            j++;
        }
        tname[j][i%8] = name1[i];
    }
    for (i = 0; i < 8; i++) {
        tname[0][i] = tname[0][i]^IV[i];
    }
    //第一次加密，单独处理
    CharToBit(key, 0);
    CharToBit(tname[0], 1);
    //第二次及以后
    for (i = 1; i < t; i++) {
        for (j = 0; j < 8; j++) {
            temp[j] = cipher[0][j];
        }
        for (j = 0; j < 8; j++) {
            ctemp[j] = (char)temp[j];
        }
        for (j = 0; j < 8; j++) {
            tname[i][j] = tname[i][j]^ctemp[j];
        }
        CharToBit(key, 0);
        CharToBit(tname[i], 1);
    }
    for (i = 0; i < t; i++) {
        for (j = 0; j < 64; j++) {
            printf("%d ",Out[i][j]);
        }
        printf("\n");
    }
    printf("do you want to decrypt ECB?\n");
    printf("1.Decryption\n");
    printf("0.Exit\n");
    scanf("%d", &n);
    if (n == 1) {
        FLAG1 = 1;
        for (j = 0; j < 64; j++) {
            out[j] = Out[0][j];
        } 
        CharToBit(key, 0);
        Initplaint(out);
        for (i = 0; i < 8; i++) {
            cipher[0][i] = cipher[0][i]^IV[i];
        }
        for (i = 1; i < t; i++) {
            for (j = 0; j < 64; j++) {
                out[j] = Out[i][j];
            }
            CharToBit(key, 0);
            Initplaint(out);
            p = 0;
            for (j = 0; j < 8; j++) {
                    x = 0;
                    k = 7;
                    for (k = 7; k >=0; k--) { 
                        x += (int)(Out[i-1][p] * pow(2,k));
                        p++;
                    }
                    temp[j] = x;
            }
            for (j = 0; j < 8; j++) {
                cipher[i][j] = cipher[i][j]^temp[j];
            }
        } 
        for (i = 0; i < t; i++) {
            for (j = 0; j < 8; j++) {
                ch1[m] = (char)cipher[i][j];
                m++;
            }
        }
        for (i = 0; i < len; i++) {
            ch = ch1[i];
            printf("%c", ch);
        }
        printf("\n");
    }
    if (n == 0) {
        return;
    }
}

void OFB(char *name, char *key, char *IV, int t) {
    int len;
    int i = 0;
    int j;
    int x;
    int k;
    int p = 0;
    int m = 0;
    int n;
    int h;
    int out[64];
    int temp1[8];
    char tname[t][8];
    char name1[8*t];
    char ch1[200];
    char ch;
    char temp[8];
    int  cipher2[t][8];
    FLAG1 = 0;
    T = 0;
    S = 0;
    len = strlen(name);
    while (name[i] != '\0') {
        name1[i] = name[i];
        i++;
    }
    //数据填充
    if ((len/8) < t) {
        for (i = len; i < 8*t; i++) {
            name1[i] = 97;
        }
    }
    j = 0;
    //分组
    for (i = 0; i < t*8; i++) {
        if ((i%8 == 0) && (i != 0)) {
            j++;
        }
        tname[j][i%8] = name1[i];
    }
    //第一组加密
    CharToBit(key, 0);
    CharToBit(IV, 1);
    for (i = 0; i < 8; i++) {
        ch = tname[0][i];
        for (j = 0; j < 8; j++) {
            out[8*i+7-j] = ch&1;
            ch = (char)((int) ch >> 1);
        }
    }
    for (i = 0; i < 64; i++) {
        Out2[0][i] = out[i]^Out[0][i];
    }
    for (i = 0; i < 8; i++) {
        temp[i] = (char)cipher[0][i];
    }
    p = 0;
    //第二组及以后加密
    for(i = 1; i < t; i++) {
        CharToBit(key, 0);
        CharToBit(temp, 1);
        for (k = 0; k < 8; k++) {
            ch = tname[i][k];
            for (j = 0; j < 8; j++) {
                out[8*k+7-j] = ch&1;
                ch = (char)((int) ch >> 1);
            }
        }
        for (j = 0; j < 64; j++) {
            Out2[i][j] = Out[i][j]^out[j];
        }
        for (j = 0; j < 8; j++) {
            temp[j] = (char)cipher[0][j];
        }
        
    }
    for (i = 0; i < T; i++) {
        for (j = 0; j < 64; j++) {
            printf("%d ",Out2[i][j]);
        }
        printf("\n");
    }
    printf("do you want to decrypt ECB?\n");
    printf("1.Decryption\n");
    printf("0.Exit\n");
    scanf("%d", &n);
    if (n == 1) {
        FLAG1 = 1;
        T = 0;
        //第一轮解密
        CharToBit(key, 0);
        CharToBit(IV, 1);
        j = 0;
        for (i = 0; i < 8; i++) {
                x = 0;
                k = 7;
                for (k = 7; k >=0; k--) { 
                    x += (int)(Out2[0][j] * pow(2,k));
                    j++;
                }
                temp1[i] = x;
        }
        for (i = 0; i < 8; i++) {
            cipher2[0][i] = temp1[i]^cipher[0][i];
        }
        for (i = 0; i < 8; i++) {
            temp[i] = (char)cipher[0][i];
        }
        //第二轮及以后解密
        for (i = 1; i < t; i++) {
            CharToBit(key, 0);
            CharToBit(temp, 1);
            j = 0;
            for (h = 0; h < 8; h++) {
                    x = 0;
                    k = 7;
                    for (k = 7; k >=0; k--) { 
                        x += (int)(Out2[i][j] * pow(2,k));
                        j++;
                    }
                    temp1[h] = x;
            }
            for (j = 0; j < 8; j++) {
                cipher2[i][j] = temp1[j]^cipher[i][j];
            }
            for (j = 0; j < 8; j++) {
                temp[j] = (char)cipher[i][j];
            }
        }
        for (i = 0; i < t; i++) {
            for (j = 0; j < 8; j++) {
                ch1[m] = (char)cipher2[i][j];
                m++;
            }
        }
        for (i = 0; i < len; i++) {
            ch = ch1[i];
            printf("%c", ch);
        }
        printf("\n");

    }
    if (n == 0) {
        return;
    }
}

void RC4(char *name, char *key, int length) {
    int  i;
    int  j;
    int  t;
    int  len;
    int  temp;
    int  k1;
    int  x;
    int  f;
    int  n;
    char k[length];
    char a;
    int  out[8*length];
    int  S1[256];
    int  T1[256];
    char ciphern[length];
    len = strlen(key);
    //初始置换
    for (i = 0; i < 256; i++) {
        S1[i] = i;
        T1[i] = key[i % length];
    }
    j = 0;
    for (i = 0; i < 256; i++) {
        j = (j + S1[i] + T1[i]) % 256;
        temp = S1[i];
        S1[i] = S1[j];
        S1[j] = temp;
    }
    i = 0;
    j = 0;
    //产生密钥以及加密
    while(name[i] != '\0') {
        i = (i + 1) % 256;
        j = (j + S1[i]) % 256;
        temp  = S1[i];
        S1[i] = S1[j];
        S1[j] = temp;
        t = (S1[i] + S1[j]) % 256;
        k[i-1] = (char)S1[t];
        ciphern[i-1] = name[i-1]^k[i-1];
    }
    f = i-1;
    for (i = 0; i < f; i++) {
        a = ciphern[i];
        for (j = 0; j < 8; j++) {
            out[8*i+7-j] = a&1;
            a = (char)((int) a >> 1);
        }
    }
    for (i = 0; i < 8*length; i++) {
        printf("%d ",out[i]);
    }
    printf("\n");
    printf("do you want to decrypt ECB?\n");
    printf("1.Decryption\n");
    printf("0.Exit\n");
    scanf("%d", &n);
    if (n == 1) {
        for (i = 0; i < length; i++) {
            ciphern[i] = ciphern[i]^k[i]; 
        }
        for (i = 0; i < length; i++) {
            printf("%c", ciphern[i]);
        }
        printf("\n");
    }
    if (n == 0) {
        return;
    }

}

void main() {
    int  i;
    int  j;
    int  len;
    int  t;
    int flag;
    int t_flag = 1;
    char name[200];// = "testdata";
    char key[8]; //= "mydeskey";
    char ch;
    char IV[8];
    
    while(t_flag) {
        printf("1.ECB\n");
        printf("2.CBC\n");
        printf("3.OFB\n");
        printf("4.RC4\n");
        printf("0.exit\n");
        scanf("%d", &flag);

        if (flag == 0) {
            t_flag = 0; 
        } else {
            printf("please input the plaintext: ");
            scanf("%s", name);
            printf("please input 8 bytes for key: ");
            scanf("%s", key);
        }
        len = strlen(name);
        if (len%8 == 0) {
            t = len / 8;
        } else {
            t = len/8 + 1;
        }
        if (flag == 2) {
            printf("please input 8 bytes for IV: ");
            scanf("%s", IV);

            CBC(name, key, IV, t);
        }
        if (flag == 1) {
            ECB(name, key, t);
        }
        if (flag == 3) {
            printf("please input 8 bytes for IV: ");
            scanf("%s", IV);
            OFB(name, key, IV, t);
        }
        if (flag == 4) {
            RC4(name, key, len);
        }
    }

}
