/***********************************************
*
* @Purpose: This project stores data from files related to bands, musicians and venues. It also calculates the price of a ticket for a given venue and band. It uses a linked list module to store the venues.
* @Author: Cyprien Eckert
* @Creation date: march 2020
* @Date of last modification: 17th of june 2021
*
************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

#define STRING_LENGTH 50

typedef struct {
    char name[STRING_LENGTH];
    int age;
} Musician;

typedef struct {
    char name[STRING_LENGTH];
    int fame;
    int num_musicians;
    Musician *musicians;
} Band;

// I implemented the following structe called Model which encapsulates all the data that needs to be stored. Coherently, this structure contains dynamic arrays of musicians and bands and a pointer to a linked list used to store the venues. Additionally, it also has the number of musicians, bands and venues.
typedef struct Structure{
    Musician* musicians;
    Band* bands;
    LinkedList *list;
    int num_musicians;
    int num_bands;
    int num_venues;
}Model;



/*****************
*
* @Purpose: Print the welcome message
* @Parameters: NA
* @Return: NA
*
****************/
void printWelcomeMessage(){
    printf("Welcome!\n");
}

/*****************
*
* @Purpose: Print the parting message
* @Parameters: NA
* @Return: NA
*
****************/
void printPartingMessage(){
    printf("\nBye!");
}

/*****************
*
* @Purpose: This function takes care of requesting the name of a file from the user. It compares the input to the name of the file and prints an error if they are different or if the file does not exist. I also made sure to free the file name before the end of the funxtion.
* @Parameters: pointer to first element of a character array
* @Return: file pointer to the open file
*
****************/
FILE* openFile(char* file_type){
    FILE* fp;
    char* file_name = (char*)malloc(sizeof(char)*STRING_LENGTH);

    do {
        printf("\t- %s: ", file_type);
        scanf("%s", file_name);
        getchar();
        fp = fopen(file_name, "r");
        if (fp == NULL) {
            printf("ERROR: Can't open file '%s'\n", file_name);
        }
    }while(fp == NULL);

    free(file_name);

    return fp;
}

/*****************
*
* @Purpose: This function is used to initialise an array of musicians. It initialises all the names with null characters and sets all the ages to 0.
* @Parameters: pointer to first element of an array of musicians, number of elements of hat array
* @Return: NA
*
****************/
void initMusicians(Musician *musicians, int num_musicians){
    Musician musician;
    int i, j;

    for(i = 0; i < STRING_LENGTH; i++){
        musician.name[i] = '\0';
    }
    musician.age = 0;

    for(j = 0; j < num_musicians; j++){
        musicians[j] = musician;
    }

}

/*****************
*
* @Purpose: This function adds a musician to the musician dynamic array of a specific band. It loops through it and inserts when an empty position is found.
* @Parameters: Band and musician to add to the band
* @Return: NA
*
****************/
Band addMusician(Band band, Musician musician){
    int i = 0, done = 0;
    for(; i < band.num_musicians; i++) {
        if (band.musicians[i].age == 0 && done == 0){
            band.musicians[i] = musician;
            done = 1;
        }
    }
    return band;
}

/*****************
*
* @Purpose: This function takes care of reading the musician file.
* @Parameters: Model
* @Return: number of musicians read
*
****************/
int getMusiciansData(Model* m){
    char* band_name = NULL;
    band_name =  malloc(sizeof(char)*STRING_LENGTH);
    int num_musicians = 0, musicians_read = 0;
    FILE* fp = openFile("Musicians");

    //first, We read the number of musicians
    fscanf(fp, "%d\n", &num_musicians);

    //we then allocate the appropriate number of elements and initialise them
    m->musicians = malloc(sizeof(Musician) * num_musicians);
    initMusicians(m->musicians, num_musicians);

    if(m->musicians == NULL){
        printf("Failed to allocate for musicians");
    }

    //we now loop through the array until we have read all the musicians
    while(num_musicians != musicians_read && band_name != NULL){

        fgets(m->musicians[musicians_read].name, STRING_LENGTH, fp);
        fgets(band_name, STRING_LENGTH, fp);
        fscanf(fp, "%d\n", &m->musicians[musicians_read].age);

        //we insert the newly read in its corresponding band
        for(int i = 0; i < m->num_bands; i++){
            if(strcmp(m->bands[i].name, band_name) == 0){
                m->bands[i] = addMusician(m->bands[i], m->musicians[musicians_read]);
            }
        }
        musicians_read++;
    }

    fclose(fp);

    //freeing not to be used memory
    free(band_name);
    return num_musicians;
}

/*****************
*
* @Purpose: This function takes care of reading the band file.
* @Parameters: Model
* @Return: number of bands read
*
****************/
int getBandData(Model* m){
    int num_bands = 0, bands_read = 0;
    FILE* fp = openFile("Bands");


    fscanf(fp, "%d\n", &num_bands);

    m->bands = malloc(sizeof(Band) * num_bands);

    if(m->bands == NULL){
        printf("Failed to allocate for bands");
    }

    while(num_bands != bands_read){
        fgets(m->bands[bands_read].name, STRING_LENGTH, fp);
        m->bands[bands_read].musicians = NULL;
        m->bands[bands_read].num_musicians = 0;
        fscanf(fp, "%d\n", &m->bands[bands_read].fame);
        fscanf(fp, "%d\n", &m->bands[bands_read].num_musicians);

        //we allocate memory for the array of musicians of the newly read band and initialise it
        m->bands[bands_read].musicians = malloc((sizeof(Musician)*m->bands[bands_read].num_musicians));
        initMusicians(m->bands[bands_read].musicians, m->bands[bands_read].num_musicians);
        bands_read++;
    }
    fclose(fp);
    return num_bands;
}

/*****************
*
* @Purpose: This function takes care of reading the venue file and storing it in a linked list
* @Parameters: Model
* @Return: number of venues read
*
****************/
int getVenuesData(Model* m){
    int venues_index = 0, num_venues;
    FILE* fp = openFile("Venues");
    Element element;

    //as we dont know how many venues the file contains, we need to loop until the end of file
    while(!feof(fp)){
        //since index is set to 0 at first, we need to increment it to get the real number of fenues read.
        num_venues = venues_index + 1;
        fgets(element.name, STRING_LENGTH, fp);
        fscanf(fp, "%d\n", &element.capacity);

        // we delare an Element and read the data into it before passing it to the add function of the linked list module.
        LINKEDLIST_add(m->list, element);
        venues_index++;
    }

    //*************************************+

    fclose(fp);
    return num_venues;
}

/*****************
*
* @Purpose: This  function takes care of printing the introctiry line to the fle reading process as well as calls the individual fil reading functions.
* @Parameters: Model
* @Return: NA
*
****************/
void openAndCheckAllFiles(Model* m){

    printf("\nIntroduce the file names:\n");
    m->num_bands = getBandData(m);
    m->num_musicians = getMusiciansData(m);
    m->num_venues = getVenuesData(m);
}

/*****************
*
* @Purpose: Print the option one menu
* @Parameters: NA
* @Return: NA
*
****************/
void printOptionOneMenu(){
    printf(" \n1. Show bands |"
           " 2. Show venues"
           "\nSelect option: ");
}

/*****************
*
* @Purpose: Print the main menu
* @Parameters: NA
* @Return: NA
*
****************/
void printMainMenu(){
    printf("\n1. Show bands or venues |"
           " 2. Calculate ticket price |"
           " 3. Show band members |"
           " 4. Exit\n"
           "Select option: ");
}

/*****************
*
* @Purpose: This function checks that the option user input is within the correct range
* @Parameters: option user input
* @Return: 1 if correct, 0 otherwise
*
****************/
int checkOptionInput(int input){
    return input < 5 && input > 0 ? 1 : 0;
}

/*****************
*
* @Purpose: This function request user input in the main menu, calls the error checking function and prints an error if necessary.
* @Parameters: NA
* @Return: correct option input
*
****************/
int getOption()    {
    int input, error = 0;
    do {
        if(error > 0){
            printf("ERROR: Wrong option number\n");
            printMainMenu();
        }
        scanf("%d", &input);
        error++;
    }while(!checkOptionInput(input));
    return input;
}

/*****************
*
* @Purpose: This function takes care of printing a band by looping through its array
* @Parameters: NA
* @Return: NA
*
****************/
void printBands(Band* bands, int num_bands){
    int i = 0;

    printf("\nBands:\n");

    for(; i < num_bands; i++){
        printf("\t%d - %s", i+1, bands[i].name);
    }
}

/*****************
*
* @Purpose: This function aims to print the venues stored in the system. It does so with the help of the linked list module
* @Parameters: Linked list storing the venues, number of stored venues
* @Return: NA
*
****************/
void printVenues(LinkedList *list, int num_venues){
    int venue_number = 1;
    Element element;

    printf("\nVenues:\n");

    LINKEDLIST_goToHead(list);
    while(!LINKEDLIST_isAtEnd(*list)){
        //we declare an Element and we use the get function to feed it in there. We are then able to print it using printf.
        element = LINKEDLIST_get(list);
        printf("\t%d - %s", venue_number, element.name);
        venue_number++;
        //go to next element in linked list
        LINKEDLIST_next(list);
    }
}

/*****************
*
* @Purpose: This function is used to either print the venues or the bands depending on what the user entered
* @Parameters: option user input, Model
* @Return: NA
*
****************/
int manageOptionOne(int option, Model* m){
    switch (option) {
        case 1:
            printBands(m->bands, m->num_bands);
            return 1;
        case 2:
            printVenues(m->list, m->num_venues);
            return 1;
        default:
            break;
    }
    return 0;
}

/*****************
*
* @Purpose: Checks that the option user input in option one is within the correct range
* @Parameters: option usr input
* @Return: 1 of correct and 0 otherwise
*
****************/
int checkOptionOneInput(int input){
    return input > 0 && input <= 2;
}

/*****************
*
* @Purpose: This is the main function for option one It makes sure that the entered input is correct and prints an error otherwise.
* @Parameters: NA
* @Return: NA
*
****************/
void handleOptionOne(Model* m){
    int option, error = 0;
    do {
        if(error > 0){
            printf("ERROR: Wrong option number\n");
        }
        printOptionOneMenu();
        scanf("%d", &option);
        manageOptionOne(option, m);
        error++;
    } while(!checkOptionOneInput(option));
}

/*****************
*
* @Purpose: this function checks that the band and venue numbers inputted by the user are smaller thant the number of their respective elements stored.
* @Parameters: number inputted, number of elements in array
* @Return: NA
*
****************/
int checkOptionTwoNumber(int input, int max){
    return input <= 0 || input > max ? 0 : 1;
}

/*****************
*
* @Purpose: Requests band number from user and checks that it is correct. Prints error otherwise
* @Parameters: numbe of bands
* @Return: band number chosen
*
****************/
int getBandNumber(int num_bands){
    int input, error = 0;

    do{
        if(error > 0){
            printf("ERROR: Invalid band number\n");
        }
        printf("\nSelect band number: ");
        scanf("%d", &input);
        error++;
    }while(!checkOptionTwoNumber(input, num_bands));

    return input;
}

/*****************
*
* @Purpose: Requests venue number from user and checks that it is correct. Prints error otherwise
* @Parameters: numbe of venues
* @Return: venue number chosen
*
****************/
int getVenueNumber(int num_venues){
    int input, error = 0;

    do{
        if(error > 0){
            printf("ERROR: Invalid venue number\n");
        }
        printf("\nSelect venue number: ");
        scanf("%d", &input);
        error++;
    }while(!checkOptionTwoNumber(input, num_venues));

    return input;
}

/*****************
*
* @Purpose: Print the ticket pricer
* @Parameters: float price
* @Return: NA
*
****************/
void printTicketPrice(float price){
    printf("\nThe minimum ticket price is %.2f euros\n", price);
}

/*****************
*
* @Purpose: Calculates the ticket price based on the chosen venue and band numbers.
* @Parameters: band number, venue number and Model
* @Return: ticket price
*
****************/
float getTicketPrice(int band_number, int venue_number, Model* m){
    int index = 1;
    Element element;

    //first, we go to the first element int the list
    LINKEDLIST_goToHead(m->list);
    //we now loop through the list and stop when the correct element is reached
    while(index != venue_number){
        LINKEDLIST_next(m->list);
        index++;
    }
    //we are now able to fetch the wanted element
    element = LINKEDLIST_get(m->list);

    return (float)m->bands[band_number-1].fame / (float) element.capacity;
}

/*****************
*
* @Purpose: This function defines the main flow of option 2.
* @Parameters: Model
* @Return: NA
*
****************/
void handleOptionTwo(Model* m){
    int band_number, venue_number;
    float ticket_price;

    band_number = getBandNumber(m->num_bands);
    venue_number = getVenueNumber(m->num_venues);
    ticket_price = getTicketPrice(band_number, venue_number, m);
    printTicketPrice(ticket_price);
}

/*****************
*
* @Purpose: Print the members of a band
* @Parameters: Band
* @Return: NA
*
****************/
void printBandMembers(Band band){
    int i = 0;

    printf("\nMembers:\n");

    for(; i < band.num_musicians; i++){
        printf("\t- %s", band.musicians[i].name);
    }
}

/*****************
*
* @Purpose: This function uses a switch statement to direct the program to the chosen option module
* @Parameters: option user input, Model
* @Return: NA
*
****************/
void manageOptions(int option, Model* m){
    switch (option){
        case 1:
            handleOptionOne(m);
            break;
        case 2:
            handleOptionTwo(m);
            break;
        case 3:
            printBandMembers(m->bands[getBandNumber(m->num_bands)-1]);
            break;
        default:
            break;
    }
}

/*****************
*
* @Purpose: This function takes care of freeing all the memory borrowed in the program
* @Parameters: NA
* @Return: NA
*
****************/
void freeAllPointers(Model* m){
    int i = 0;

    for(; i < m->num_bands; i++) {
        free(m->bands[i].musicians);
    }
    free(m->bands);
    free(m->musicians);
    free(m->list);
}

int main() {
    int option;
    Model* m = malloc(sizeof(Model));
    m->list = malloc((sizeof(LinkedList)));
    *m->list = LINKEDLIST_create();

    printWelcomeMessage();
    openAndCheckAllFiles(m);

    do {
        printMainMenu();
        option = getOption();
        manageOptions(option, m);
    } while (option != 4);
    LINKEDLIST_destroy(m->list);
    freeAllPointers(m);
    free(m);

    printPartingMessage();
    return 0;
}
