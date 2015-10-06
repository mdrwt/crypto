#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

const double freq[] = {  // Character freq in english language
    0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015,
    0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 0.02406, 0.06749,
    0.07507, 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 0.02758,
    0.00978, 0.02360, 0.00150, 0.01974, 0.00074
};

int main() {
    char cipherText[1000] = "ofbxfcjzmdkgugdjqoxbutxteierqqwbagwcpmlpadhixzrnkcxatcxtpphinzhbapummywpugwwkqlxtsummgltkwxaqrwpqgxzzopmatrcdduqyspqzwvbqfdafvhsqmzwdrwpqfhumwqqzuvmzhhvosvidsmcehwwqbvcdswpmhbwgqdvnfhiwhkqehhffsdauzbqrhkmxsqofvrnfvhkudkmdhhffwvvahhvaijpfvhvqjhvfvhbamfqbvhzeqdvnskidrwwnfhiwtrzqldubzhizcwpqftcqgwqablvfvlatcpmicusmgnakcxbagkwihkifhkmevlnfqlxtsuqedhzrsfbxmvmoiumutrvxmrvqqkidofbqflaqbfzkdwmpivqzulbfvlaugdoacgbuahbafhipoewghwpqhzwfmsmeciifhdkwgzmpwvkggvmpwqbtsftmgv";
    
    int keylen;
    int k;
    char coloumnText[1000];
    int i;
    int j;
    
    double max = INT_MIN;
    int maxIndex = 0;
    

    /*
    * For keylengths from 1-10 we find out the different index of coincidences(ioc)
    * The key length with the maximum ioc is chosen as the probable actual keylength
    */
    for (keylen = 1; keylen <= 10; ++keylen) { // Testing IoC for keylength upto 10
        
        int j;
        double avgioc;
        
        for(k = 0; k < keylen; k++) {
            int index=0;
            for (j=k; j < strlen(cipherText); j=j+keylen) { //Save coloumn chars in a char array
                coloumnText[index++] = cipherText[j]; 
            }            
            
            float count[26];
            for(i=0; i<strlen(coloumnText); i++) { // count alphabet occurances of each char char array
                int ascii = tolower(coloumnText[i]);
                ascii = ascii-97;
                count[ascii]++;
            }
            
            //Calculate index of coincidence
            double ioc=0;
            for(i=0; i<26; i++) { 
                ioc = ioc+((count[i]*(count[i]-1.0))/(strlen(coloumnText)*(strlen(coloumnText)-1)));
            }
            ioc = ioc*26;
            avgioc = avgioc+ioc;
            
            for(i=0; i<26; i++) {
                count[i]=0;
            }
            for (i = 0; i < 1000; ++i) {
                coloumnText[i]='\0';
            }
        }
        avgioc = avgioc/keylen;
        printf("%d %f\n", keylen, avgioc);
        if(avgioc>max) { // the max avg ioc is for probably for our key.
            max = avgioc;
            maxIndex = keylen;
        }
        
        
        avgioc = 0.0;
    }
    
    printf("keylength %d\n", maxIndex);
    printf("Press a key...\n");
    getchar();
    
    keylen = maxIndex;
    
    char possible[1000];
    char colcip[1000];
    double min=9999999999.0;
    char foundKey[keylen];
    for(k=0; k<keylen; k++) {
        foundKey[k] = '\0';
    }
    printf("%d\n", keylen);
    

    /*
    * Taking the found probable keylength we use Chi Squared Statistic.
    * This compares the probability distribution of alphabets in our Coloumn texts to
    * expected English frequency distribution.
    * For each coloumn of text we find the Chi-square statistic for all the 26 shift/caesar ciphers.
    * The deciphered text will be having lowest chi-squared statistic for the given cipher text and key.
    * we then find the key by simply calculationt the difference.
    */
    for(k = 0; k < keylen; k++) {
        int index=0;
        for (j=k; j < strlen(cipherText); j=j+keylen) { //Fetch coloumn chars according to keylength.
            coloumnText[index++] = cipherText[j];
        }
        strcpy(colcip, coloumnText);
        
        printf("\n.....................................................\n");
        printf("%s\n", coloumnText);
        
        min=999999999999.0;
        
        for(i = 0; i < 26; i++) {
            int j;
            float count[26];
            
            // Count occurances of alphabet of coloumn characters.
            for(j=0; j<strlen(coloumnText); j++) {
                int ascii = tolower(coloumnText[j]);
                ascii = ascii-97;
                count[ascii]++;
            }
            
            float chi=0;
            
            for(j=0; j<26; j++) { // Calculate chi statistic for this caesar shift
                float adj = strlen(coloumnText)*freq[j];
                chi = chi+(((count[j]-adj)*(count[j]-adj))/(adj));
            }
            printf("%d:%s : %f\n", i+1, coloumnText, chi);
            
            if(chi<min) { // we will save the min chi stat and corresponding text, which has high prob of being plain text
                min = chi;
                int k;
                for (k = 0; k < 1000; ++k) {
                    possible[k]='\0';
                }
                strcpy(possible, coloumnText);
            }
            
            
            for(j=0; j<26; j++) {
                count[j]=0;
            }
            for(j=0; j<strlen(coloumnText); j++) { // Shift the chars by 1 ,26 times. Casesar shift.
                
                int ascii = tolower(coloumnText[j]);
                ascii = ascii-97;
                ascii = (ascii-1);
                if(ascii==-1)
                    ascii = 25;
                coloumnText[j]=ascii+97;
            }
        }
        
        for (i = 0; i < 1000; ++i) {
            coloumnText[i]='\0';
        }
        
        printf("\n %s %f", possible, min);
        
        int c = colcip[0];
        int o = possible[0];
        c = c-97;
        o = o-97;
        int kk;
        if(c>o) {
            kk = c-o;
        }
        else {
            kk = (26-o)+c;
        }
        
        printf("\nkey[%d]: %c\n", k,(char)(kk+97)); // possible key. Will print each character of the given key one by one as they 
        //are found
        foundKey[k] = (char)(kk+97);
        
        getchar();
        
    }

    // printf("FoundKey %s %lu\n", foundKey, strlen(foundKey));
    // char found[keylen];
    // for(i=0; i<keylen; i++) {
    //     found[i]=foundKey[i];
    // }
    // printf("FoundKey %s %lu\n", found, strlen(found));
}