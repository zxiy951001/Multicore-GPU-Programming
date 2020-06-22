
__kernel void encryptKernel(__global char* plaintext, __global char* ciphertext, __global int* N){
    int i = get_global_id(0);
    if(plaintext[i] != ' '){
        if ((plaintext[i] < 'A') || (plaintext[i] > 'Z')){
            ciphertext[i] = plaintext[i];
        }
        else{
            ciphertext[i]=((plaintext[i]+N[i]-65)%26+'A');
            if(ciphertext[i] < 'A'){
                ciphertext[i] = 'Z' - ('A'-ciphertext[i]-1);
            }
        }
    }else{
        ciphertext[i] = ' ';
    }

}

__kernel void decryptKernel(__global char* ciphertext, __global char* plaintext, __global int* N){
    int i = get_global_id(0);
    if(ciphertext[i] != ' '){
        if ((ciphertext[i] < 'A') || (ciphertext[i] > 'Z')){
            plaintext[i] = ciphertext[i];
        }
        else{
            plaintext[i]=((ciphertext[i]-N[i]-65)%26+'A');
            if(plaintext[i] > 'Z')
                plaintext[i] = 'Z'- ('A'-plaintext[i]-1);
            
            if(plaintext[i] < 'A')
                plaintext[i] = 'Z' - ('A'-plaintext[i]-1);
        }
    }else{
        plaintext[i] = ' ';
    }
}

char reverseLookup(char in){
    switch (in){
        case 'G': return 'A';
        case 'X' : return 'B';
        case 'S': return 'C';
        case 'Q': return 'D';
        case 'F': return 'E';
        case 'A': return 'F';
        case 'R': return 'G';
        case 'O': return 'H';
        case 'W': return 'I';
        case 'B': return 'J';
        case 'L': return 'K';
        case 'M': return 'L';
        case 'T': return 'M';
        case 'H': return 'N';
        case 'C': return 'O';
        case 'V': return 'P';
        case 'P': return 'Q';
        case 'N': return 'R';
        case 'Z': return 'S';
        case 'U': return 'T';
        case 'I': return 'U';
        case 'E': return 'V';
        case 'Y': return 'W';
        case 'D': return 'X';
        case 'K': return 'Y';
        case 'J': return 'Z';
        case ' ': return ' ';
        default: return in;
    }
}

char lookupTable(char in){
    switch (in){
        case 'a':
        case 'A': return 'G';
        case 'b':
        case 'B': return 'X';
        case 'c':
        case 'C': return 'S';
        case 'd':
        case 'D': return 'Q';
        case 'e':
        case 'E': return 'F';
        case 'f':
        case 'F': return 'A';
        case 'g':
        case 'G': return 'R';
        case 'h':
        case 'H': return 'O';
        case 'i':
        case 'I': return 'W';
        case 'j':
        case 'J': return 'B';
        case 'k':
        case 'K': return 'L';
        case 'l':
        case 'L': return 'M';
        case 'm':
        case 'M': return 'T';
        case 'n':
        case 'N': return 'H';
        case 'o':
        case 'O': return 'C';
        case 'p':
        case 'P': return 'V';
        case 'q':
        case 'Q': return 'P';
        case 'r':
        case 'R': return 'N';
        case 's':
        case 'S': return 'Z';
        case 't':
        case 'T': return 'U';
        case 'u':
        case 'U': return 'I';
        case 'v':
        case 'V': return 'E';
        case 'w':
        case 'W': return 'Y';
        case 'x':
        case 'X': return 'D';
        case 'y':
        case 'Y': return 'K';
        case 'z':
        case 'Z': return 'J';
        case ' ': return ' ';
        default: return in;
    }
}

__kernel void encryptKernelLookup(__global char* plaintext, __global char* ciphertext){
    int i = get_global_id(0);
    ciphertext[i] = lookupTable(plaintext[i]);
}

__kernel void decryptKernelLookup(__global char* ciphertext, __global char* plaintext){
    int i = get_global_id(0);
    plaintext[i] = reverseLookup(ciphertext[i]);
}
