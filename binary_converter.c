#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// struct used to hold and pass around user input in one variable 
// stores number to be converted and bit preference given by user
struct UserInput
{
    long* decimalNumber; // long used to hold number of up to 64 bits
    int* bitPreference;
};


// Program structure skeleton
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
            void iterateBinary(int* array, int* arraySize, long* arg);
                int binaryFunction(long* decimalNumber);

        int* onesComplement(int* binaryArray, int* size);
            //int* allocateArray(int* size);
            void iterateOnes(int* array, int* arraySize, int* arg);
                int onesFunction(int* binaryArrayDigit);

        int* twosComplement(int* onesCompArray, int* size) ;
            //int* allocateArray(int* size);
            void iterateTwos(int* array, int* arraySize, int* arg);
                int twosFunction(int* onesCompDigit, int* carry);

        void printArray(int* array, int* size);
        void printArrayHelp(int* array, int* arraySize, char* message);

    int promptContinue();
    void cleanup(void* pointer);
    void cleanupInput(struct UserInput* input);
void exitMessage();


void main()
{
    welcomeMessage();
    initiateMainLoop();
    exitMessage();
}

void welcomeMessage() 
{
    printf("\nHello And Welcome To The Binary Number Generator Tool!\n");
    printf("This Tool Will Accept A Decimal Number And Convert It To Binary, Ones Complement, And Twos Complement Form.\n");
    printf("Enjoy! \n\n");
}

// Program's main functions all called from this loop until user decides to exit
void initiateMainLoop() 
{
    // Infinite Loop Until User Wants To Exit
    while(1) {
        // * STEP 1:  Prompt and Validate Input * //
        struct UserInput* input = getInput();
        if (validateInput(input) != 1) { continue; } // Invalid input prompts user to enter valid input
            
        // * STEP 2: Confirm and show Conversions. * //
        showConversions(input);
        cleanupInput(input); //Make sure to free pointers before exit

        // * STEP 3: Decide to Continue * //
        if (promptContinue() != 1) { break; } // If User Doesn't Want To Continue, Main Loop Ends
    }
}

// Helper function to free pointers
void cleanup(void* pointer) 
{
  free(pointer);
  pointer = NULL;
}

// Keep main loop clean by cleaning input with one function call 
void cleanupInput(struct UserInput* input) 
{
    cleanup(input->decimalNumber);
    cleanup(input->bitPreference);
    cleanup(input);
}

// Prompt user for both inputs and return answers in struct
struct UserInput* getInput()
{
    struct UserInput* input = (struct UserInput*)malloc(sizeof(struct UserInput));
    input->decimalNumber = promptInputLong("Please Enter A Decimal Number to Convert To Binary.\n");;
    input->bitPreference = promptInput("Please Tell Me How Many Bits You Want Your Number To Be Represented With - ie. 8, 16, etc...\n");
    return input;
}

// Prompts the user with a message and returns their input.
int* promptInput(char* message)
{ 
    int* input = (int*)malloc(sizeof(int));
    printf("\n%s\n", message); // prompt user with message
    if (scanf("%d", input) == 1) // store number to input to be returned
    { // 
        return input;
    }
    else 
    {                       
        fgetc(stdin); // clears buffer to prevent infinite loop
        *input = -9999; // if input completely invalid (letter), set to -9999 for validation function to catch
        return input;
    }
}

// Long input has different procedure than integer input
long* promptInputLong(char* message) 
{ 
    long* input = (long*)malloc(sizeof(long));
    printf("\n%s\n", message); // prompt user with message
    if (scanf("%ld", input) == 1) // store number to input to be returned
    { 
        return input;
    }
    else 
    {                       
        fgetc(stdin); // clears buffer to prevent infinite loop
        *input = -9999; // if input completely invalid, set to -9999 for validation function to catch
        return input;
    }
}

// Checks if the number and preferred amount of bits entered by the user makes sense. Returns "boolean" 0 or 1
int validateInput(struct UserInput* input) 
{
    // character
    if (*input->decimalNumber == -9999 || *input->bitPreference == -9999) 
    {
        printf("\nPlease type a number, not a letter\n");
        goto invalid;
    }

    // negative number
    if (*input->decimalNumber < 0 || input->bitPreference < 0) 
    {
        printf("\nMust use positive numbers, Try Again\n");
        goto invalid;
    }

    // odd number bits
    if (*input->bitPreference != 4 && *input->bitPreference != 8 && *input->bitPreference != 16 && *input->bitPreference != 32 && *input->bitPreference != 64)
    {
        printf("\nPlease choose a valid bit representation - 4, 8, 16, 32, or 64 bits\n");
        goto invalid;
    }

    // too many input bits
    if (*input->bitPreference > 1024) 
    {
        printf("\nOuch that's a lot of bits...., Try Again!\n");
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

// Helper before printing conversions
void confirmationMessage(struct UserInput* input) 
{
    printf
    (
        "\nOkay So We Want To Convert %ld Using %d Bits --- Coming Right Up: \n\n\n", *input->decimalNumber, *input->bitPreference
    );
}

// Calls mathematical functions to create and print the arrays needed to represent the conversions
void showConversions(struct UserInput* input) 
{
        confirmationMessage(input);
        
        int* binaryArray = binaryConvert(input);
        printArrayHelp(binaryArray, input->bitPreference, "Binary Conversion: ");

        int* onesCompArray = onesComplement(binaryArray, input->bitPreference);
        printArrayHelp(onesCompArray, input->bitPreference, "Ones Complement: ");

        int* twosCompArray = twosComplement(onesCompArray, input->bitPreference);
        printArrayHelp(twosCompArray, input->bitPreference, "Twos Complement: ");
       
        cleanup(binaryArray);
        cleanup(onesCompArray);
        cleanup(twosCompArray);
}


/** The three mathematical procedures (binary, ones complement, twos complement) each consist of 3 functions.
 * First is the function which allocates the space needed for array. It calls the iterate function which 
 * runs the loop to run through each item in the array which calls the calculation function that determines whether
 * to fill it with 1 or 0. Then that array is returned to use later.
 */


// uses user input to determine size of array to be allocated. Fills the array and returns it.
int* binaryConvert(struct UserInput* input) 
{
    int* binaryArray = allocateArray(input->bitPreference); 
    iterateBinary(binaryArray, input->bitPreference, input->decimalNumber);
    return binaryArray;
}

// Helper to call the binaryFunction() on each item of the array
void iterateBinary(int* array, int* arraySize, long* decimalNumber) 
{
    int* startPtr = array;
    int* endPtr = array + *arraySize-1;
    while (endPtr >= startPtr) // iterate from back to front
    {
        *endPtr = binaryFunction(decimalNumber);
        endPtr--;
    }
}

// Performs calculation on decimal number to return 0 or 1 to fill in array
int binaryFunction(long* decimalNumber)
{
    if (*decimalNumber & 1 == 1) // extract last bit with AND mask
    {
        *decimalNumber = *decimalNumber/2; // cut off last bit with shift (divide by 2)
        return 1;
    }
    else
    {
        *decimalNumber = *decimalNumber/2;
        return 0;
    }
}

// Uses binary array to allocate an array of the same size and fills it with its values to return
int* onesComplement(int* binaryArray, int* size) 
{
    int* onesCompArray = allocateArray(size);
    iterateOnes(onesCompArray, size, binaryArray);
    return onesCompArray;
}

// Iterates through onesComplement array while also keeping track of binaryArray (passed in comparedArray)
// to base its values off of those in the binary array
void iterateOnes(int* array, int* arraySize, int* comparedArray) 
{
    int* startPtr = array;
    int* endPtr = array + *arraySize-1;
    int* comparedEndPtr = comparedArray + *arraySize-1; // back of the compared (binary) array
    while (endPtr >= startPtr) // iterate from back to front
    {
        *endPtr = onesFunction(comparedEndPtr);
        endPtr--;
        comparedEndPtr--;
    }
}

// Flips bit of binary array to return to ones complement array 
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

// Uses ones complement array to fill and return the associated twos complement array
int* twosComplement(int* onesCompArray, int* size) 
{
    int* twosCompArray = allocateArray(size);
    iterateTwos(twosCompArray, size, onesCompArray);
    return twosCompArray;
}

// Iterates through both the ones complement and twos complement array while passing information about the carry 
// bit to effectively aid the twosFunction() in adding 1 to the ones complement array.
void iterateTwos(int* array, int* arraySize, int* comparedArray) 
{
    int* carry = (int*)malloc(sizeof(int));
    *carry = 1; // passed to twosFunction() to help with calculation
    int* startPtr = array;
    int* endPtr = array + *arraySize-1;                 // pointers set to back of both ones comp and twos comp array
    int* comparedEndPtr = comparedArray + *arraySize-1;
    while (endPtr >= startPtr) // Iterate from back to front
    {
        *endPtr = twosFunction(comparedEndPtr, carry);
        endPtr--;
        comparedEndPtr--;
    }
    cleanup(carry);
}

// compares carry bit to ones complement bit to determine to return 0 or 1 to fill twos comp array
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

void printArray(int* array, int* size) 
{  
    int* arrayEndPtr = array + *size-1 ; // Ending point for our loop
    while (array <= arrayEndPtr)
    {
        printf("%d", *array);  // iterate through array and print
        array += 1;
    }
    printf("\n");
}

// Helps organize the printing of the arrays. Gives a message to explain meaning of array as well as spacing.
void printArrayHelp(int* array, int* arraySize, char* message) 
{
    printf("%s", message);
    printArray(array, arraySize);
    printf("\n");
}

// Use user input to decide whether to perform another loop iteration, return 1 means yes, 0 means no.
int promptContinue() 
{
    int* decideContinue = promptInput("\nIf You Would Like To Quit, Press 0. Otherwise, press any key\n");
    if(*decideContinue != 0)
    {   
        cleanup(decideContinue);
        return 1;
    }
    else
    {
        cleanup(decideContinue);
        return 0;
    }   
}

void exitMessage() 
{
    printf("\n\nThanks For Using This Tool. See You Next Time ;)!\n");
}