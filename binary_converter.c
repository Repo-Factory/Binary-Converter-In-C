#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct UserInput
{
    long* decimalNumber;
    int* bitPreference;
};

void welcomeMessage();
void initiateMainLoop();
    struct UserInput* getInput();
        long* promptInputLong(char* message);
        int* promptInput(char* message);
    int validateInput(struct UserInput* input);
    void showConversions(struct UserInput* input);
        void confirmationMessage(struct UserInput* input);
        int* binaryConvert(struct UserInput* input);
            int* allocateArray(int* size);
            void iterateArray(int* array, int* arraySize, int* arg, int (*iterativeFunction)(int*));
                int binaryFunction(int* decimalNumber);
        int* onesComplement(int* binaryArray, int* size) ;
            int* allocateArray(int* size);
            void iterateArrayComparative(int* array, int* arraySize, int* arg, int (*iterativeFunction)(int*));
                int onesFunction(int* binaryArrayDigit);
        int* twosComplement(int* onesCompArray, int* size) ;
            int* allocateArray(int* size);
            void iterateArrayTwo(int* array, int* arraySize, int* arg, int* arg2, int (*iterativeFunction)(int*, int*));
                int twosFunction(int* onesCompDigit, int* carry);
        void printArray(int* array, int* size);
        void printArrayHelp(int* array, char* message, struct UserInput* input);
    int promptContinue();
void exitMessage();
void cleanup(void* pointer);


void main()
 {
    welcomeMessage();
    initiateMainLoop();
    exitMessage();
}

void welcomeMessage() 
{
    printf("\nHello And Welcome To The Binary Number Generator Tool!\n");
    printf("This Tool Will Accept A Decimal Number And Convert It To Binary, OnesComplement, And Twos Complement Form.\n");
    printf("Enjoy! \n\n");
}

void initiateMainLoop() 
{
    // Infinite Loop Until User Wants To Exit
    while(1) {
        // * STEP 1:  Prompt and Validate Input * //
        struct UserInput* input = getInput();
        if (validateInput(input) != 1) 
        {
            continue;
        }
    
        // * STEP 2: Confirm and show Conversions. Make sure to free pointers.* //
        showConversions(input);
        
        cleanup(input->decimalNumber);
        cleanup(input->bitPreference);
        cleanup(input); // Free Pointers Before Exit

        // * STEP 3: Decide to Continue * //
        if (promptContinue() != 1) {
            break; // If User Doesn't Want To Continue, Main Loop Ends
        }
    }
}

void cleanup(void* pointer) 
{
  free(pointer);
  pointer = NULL;
}

struct UserInput* getInput()
{
    struct UserInput* input = (struct UserInput*)malloc(sizeof(struct UserInput));
    input->decimalNumber = promptInputLong("Please Enter A Decimal Number to Convert To Binary. If You Do Not Enter A Number, This Program Will Terminate\n");;
    input->bitPreference = promptInput("Please Tell Me How Many Bits You Want Your Number To Be Represented With - ie. 8, 16, etc...\n");
    return input;
}

int* promptInput(char* message) 
{ 
    int* input = (int*)malloc(sizeof(int));
    printf("\n%s\n", message); // prompt user for message
    if (scanf("%d", input) == 1) 
    { // store number to input to be returned
        return input;
    }
    else 
    {                      // if input is not a number, returns 0 and terminates program
        *input = 0;
        return input;
    }
}

long* promptInputLong(char* message) 
{ 
    long* input = (long*)malloc(sizeof(long));
    printf("\n%s\n", message); // prompt user for message
    if (scanf("%ld", input) == 1) // store number to input to be returned
    { 
        return input;
    }
    else 
    {                      // if input is not a number, returns 0 and terminates program
        *input = 0;
        return input;
    }
}

int validateInput(struct UserInput* input) 
{
    // odd number bits
    if (*input->bitPreference %2 != 0)
    {
        printf("\nMust enter an even number of bits, Try Again\n");
        goto invalid;
    }

    // too many input bits
    if (*input->bitPreference > 1024) 
    {
        printf("\nOuch that's a lot of bits...., Try Again!\n");
        goto invalid;
    }

    // negative number
    if (*input->decimalNumber < 0) 
    {
        printf("\nNumber Must Be Greater Than Zero, Try Again\n");
        goto invalid;
    }

    // number too large
    if (*input->decimalNumber > (pow(2, 62) - 1)) 
    {
        printf("\nNumber Requested Too Large, Try Again\n");
        goto invalid;
    }

    // number too large for given number of bits
    if ((pow(2, *input->bitPreference)) < *input->decimalNumber) 
    {
        printf("\nNumber Will Not Fit In That Number Of Bits, Try Again\n");
        goto invalid;
    }
    
    return 1;

    invalid:
        cleanup(input->bitPreference);
        cleanup(input->decimalNumber);
        cleanup(input);
        return 0;
}

void confirmationMessage(struct UserInput* input) 
{
    printf
    (
        "\nOkay So We Want To Convert %ld Using %d Bits --- Coming Right Up: \n\n\n", *input->decimalNumber, *input->bitPreference
    );
}

void showConversions(struct UserInput* input) 
{
        confirmationMessage(input);
        
        int* binaryArray = binaryConvert(input);
        printArrayHelp(binaryArray, "Binary Conversion: ", input);

        int* onesCompArray = onesComplement(binaryArray, input->bitPreference);
        printArrayHelp(onesCompArray, "Ones Complement: ", input);

        int* twosCompArray = twosComplement(onesCompArray, input->bitPreference);
        printArrayHelp(twosCompArray, "Twos Complement: ", input);
       
        cleanup(binaryArray);
        cleanup(onesCompArray);
        cleanup(twosCompArray);
}

int* binaryConvert(struct UserInput* input) 
{
    int* binaryArray = allocateArray(input->bitPreference); 
    iterateArray(binaryArray, input->bitPreference, (int*)input->decimalNumber, binaryFunction);
    return binaryArray;
}

int binaryFunction(int* decimalNumber)
{
    if (*decimalNumber & 1 == 1) 
    {
        *decimalNumber = *decimalNumber/2;
        return 1;
    }
    else
    {
        *decimalNumber = *decimalNumber/2;
        return 0;
    }
}

int* onesComplement(int* binaryArray, int* size) 
{
    int* onesCompArray = allocateArray(size);
    iterateArrayComparative(onesCompArray, size, binaryArray + *size, onesFunction);
    return onesCompArray;
}

int onesFunction(int* binaryArrayDigit)
{
    if (*binaryArrayDigit == 1) 
    {
        return 0;
    }
    else {
        return 1;
    }
}

int* twosComplement(int* onesCompArray, int* size) 
{
    int* twosCompArray = allocateArray(size);
    int* carry = (int*)malloc(sizeof(int));
    *carry = 1;
    iterateArrayTwo(twosCompArray, size, onesCompArray+ *size, carry, twosFunction);
    cleanup(carry);
    return twosCompArray;
}

int twosFunction(int* onesCompDigit, int* carry)
{
    // starting from back iterate through each entry in onesComplement and use carry to determine value for twosComplement array
        if (*carry == 0) 
        {
            return *onesCompDigit; // if there is no carry, preserve oneComplement value
        } 
        else 
        {
            if (*onesCompDigit == 0)
            {
                *carry = 0;
                return 1; // If there is carry but the bit is 0, flip number to 1 and get rid of carry
            }
            else 
            {
                return 0;
            }
        }
}

int* allocateArray(int* size) 
{
    int* array = (int*)malloc((*size)*sizeof(int));
    return array;
}

void iterateArray(int* array, int* arraySize, int* arg, int (*iterativeFunction)(int*)) 
{
    int* startPtr = array;
    int* endPtr = array + *arraySize;
    while (endPtr >= startPtr) 
    {
        *endPtr = (*iterativeFunction)(arg);
        endPtr--;
    }
}

void iterateArrayComparative(int* array, int* arraySize, int* arg, int (*iterativeFunction)(int*)) 
{
    int* startPtr = array;
    int* endPtr = array + *arraySize;
    while (endPtr >= startPtr) 
    {
        *endPtr = (*iterativeFunction)(arg);
        endPtr--;
        arg--;
    }
}

void iterateArrayTwo(int* array, int* arraySize, int* arg, int* arg2, int (*iterativeFunction)(int*, int*)) 
{
    int* startPtr = array;
    int* endPtr = array + *arraySize;
    while (endPtr >= startPtr) 
    {
        *endPtr = (*iterativeFunction)(arg, arg2);
        endPtr--;
        arg--;
    }
}

void printArray(int* array, int* size) 
{  
    int* arrayEndPtr = array + *size; // Ending point for our loop
    while (array <= arrayEndPtr)
    {
        printf("%d", *array);  // iterate through array and print
        array += 1;
    }
    printf("\n");
}

void printArrayHelp(int* array, char* message, struct UserInput* input) 
{
    printf("%s", message);
    printArray(array, input->bitPreference);
    printf("\n");
}

int promptContinue() 
{
    int* contConvert = promptInput("\nIf You Would Like To Convert Another Number, Press 1. Otherwise, press any key\n");
    if (*contConvert != 1) 
    {
        cleanup(contConvert);
        return 0;
    }
    cleanup(contConvert);
    return 1;
}

void exitMessage() 
{
    printf("\n\nThanks For Using This Tool. See You Next Time ;)!\n");
}