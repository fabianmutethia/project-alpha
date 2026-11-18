#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#define WAIT(ms) Sleep(ms)
#else
#include <unistd.h>
#define WAIT(ms) usleep((ms)*1000)
#endif

#define SAVE_FILE "neon_city_save.dat"

#define RESET "\033[0m"
#define GREEN "\033[1;32m"
#define CYAN "\033[1;36m"
#define RED "\033[1;31m"
#define YELLOW "\033[1;33m"
#define PURPLE "\033[1;35m"

// CITY DATA STRUCTURE
typedef struct{
    char name[40];
    int day;
    int population;
    int energy;
    int water;
    int credits;
    int stability;
}City;
//FUNCTIONS DECLARATIONS
void startup(void);
void displayDashboard(const City*city);
void displayBar (const char *label, int value);
void generateEvent(City*city);
void manageEnergy(City*city);
void manageWater(City*city);
void earnCredits(City*city);
void improveStability(City*city);
void advanceDay(City*city);
void saveGame(const City*city);
void loadGame(City*city);
void cheakCity(City*city);
int getChoice(void);
void showMenu(void);
// ANIMATED STARTUP
void startup(void){
    const char *messages[]={
        "INITIALIZING NEON CITY SYSTEM...",
        "CONNECTING TO ENERGY GRID...",
        "SCANNING WATER RESERVES...",
        "LOADING CITIZEN DATABASE...",
        "ESTABLISHING SECURITY NETWORK...",
        "CITY SYSTEM ONLINE!"
    };

    printf(PURPLE, "\n");
    printf("========================================================\n");
    printf("                   NEON CITY 2099                       \n");
    printf("========================================================\n" RESET);

    for (int i = 0; i < 6; i++){
        printf(CYAN "\n[ SYSTEM ] "RESET "%s",messages[i]);
        fflush(stdout);
        WAIT(500);
    }
    printf(GREEN "\n\nACCESS GRANTED.\n" RESET);
    WAIT(500);
}
// RESOURCE BAR
void displayBar(const char *label, int value){
    int percentage = value;

    if(percentage < 0)
        percentage=0;

    if (percentage > 100)
        percentage = 100;

    printf("%-12s [", label);

    for (int i = 0; i<20; i++){

        if (i<percentage/5)
            printf(GREEN "#" RESET);
        else
            printf("-");
    }
    printf("] %3d%%\n", percentage);
}
//CITY DASHBOARD
void displayDashboard(const City*city){
    printf(CYAN "\n=====================================================\n");
    printf("                 CITY DASHBOARD                             \n");
    printf("======================================================\n" RESET);

    printf(YELLOW "\nCITY:" RESET "%s\n", city->name);
    printf(YELLOW "DAY: " RESET "%d\n", city->day);

    printf("\nPOPULATION: %d citizens\n", city->population);
    printf("CREDITS:%d\n", city->credits);

    printf("\nRESOURCE LEVELS\n");

    displayBar("ENERGY", city->energy);
    displayBar("WATER", city->water);
    displayBar("STABILITY", city->stability);

    printf(CYAN "\n====================================================\n" RESET);
}

//MENU
void showMenu(void){
    printf("\n");
    printf(PURPLE "COMMAND CENTER\n" RESET);

    printf("1. Manage Energy Grid\n");
    printf("2. Manage Water Supply\n");
    printf("3. Generate City Income\n");
    printf("4. Improve City Stability\n");
    printf("5. Advance To Next Day\n");
    printf("6. Save City Progress\n");
    printf("7. Load City Progress\n");
    printf("8. View City Dashboard\n");
    printf("0. Shutdown System\n");

    printf("\nEnter command:");
}
// SAFE INPUT
int getChoice(void){
    char input[50];
    char *end;
    long choice;

    if (fgets(input, sizeof(input),stdin)== NULL)
        return -1;

    choice = strtol(input,&end,10);

    if (end==input)
        return -1;
    while (*end==' '||*end=='\t')
        end++;
    if (*end !='\n' && *end != '\0')
        return -1;
    if (choice <0 ||choice>8 )
        return -1;

    return (int)choice;
}
// ENERGY MANAGEMENT
void manageEnergy(City*city){
    int *energy = &city->energy;
    int *credits = &city->credits;

    if (*credits>=100 && *energy <= 80){
        *energy +=20;
        *credits -= 100;

        if (*energy >100)
            *energy = 100;

        printf(GREEN "\n ENERGY GRID RECHARGED!\n" RESET);
        printf("20 energy units purchased for 100 credits.\n");
    }else{
        printf(RED"\n INSUFFICIENT CREDITS OR ENERGY IS ALREADY HIGH.\n" RESET);
    }

}
//WATER MANAGEMENT
void manageWater(City*city){

    int *water = &city->water;
    int *credits = &city-> credits;

    if (*credits >=80 && *water<= 80){
        *water+=20;
        *credits -=80;

        if(*water > 100)
            *water = 100;
        printf(GREEN "\nWATER RESERVES RESTORED!\n" RESET);

    }else{
        printf(RED"\nINSUFFICIENT CREDITS OR WATER IS ALREADY HIGH.\n" RESET);
    }
}
// GENERATE INCOME
void earnCredits(City*city){
    int income=(rand()% 151)+50;

    city->credits += income;

    printf(GREEN "\nECONOMIC REPORT\n" RESET);
    printf("The city generated %d credits.\n", income);
}
//IMPROVE STABILITY
void improveStability(City*city){
    int *stability =&city->stability;
    int *credits = &city->credits;

    if (*credits>= 120 && *stability<=80){
        *stability+=20;
        *credits -=120;

        if (*stability >100)
            *stability =100;
        printf(GREEN"\nCITY SECURITY IMPROVED!\n" RESET);
    }else{
        printf(RED"\nINSUFFICIENT CREDITS OR STABILITY IS ALREADY HIGH.\n" RESET);
    }
}
//RANDOM EVENTS
void generateEvent(City*city){

    int event = rand()%5;
    printf(YELLOW "\n!!! CITY EVENT DETECTED !!!\n" RESET);

    switch (event){
         case 0:
         printf("SOLAR STORM! Energy grid damaged.\n");
         city->energy -= 15;
         break;

         case 1:
         printf("WATER PIPE FAILURE! Water reserves reduced.\n");
         city->water -=15;
         break;

         case 2:
         printf("ECONOMIC BOOM! Citizens generate extra income.\n");
         city->credits+= 200;
         break;

         case 3:
         printf("SECURITY BREACH! City stability reduced.\n");
         city->stability-= 15;
         break;
    }
}

//CHECK CITY RESOURCES
void checkCity(City*city){

    if (city->energy<0)
        city->energy=0;

    if(city->water<0)
        city->water=0;

    if (city->stability<0)
        city->stability=0;

    if (city->energy>100)
        city->energy=100;

    if (city->water>100)
        city->water=100;

    if (city->stability>100)
        city->stability=100;

    if(city->credits<0)
        city->credits=0;

}
//ADVANCE SIMULATION
void advanceDay(City*city) {

    printf(YELLOW"\nADVANCING CITY CLOCK...\n" RESET);

    city->day++;

    city->energy-=8;
    city->water-=6;
    city->credits+=30;

    generateEvent(city);
    checkCity(city);
    printf(GREEN"\nDAY %d COMPLETED.\n" RESET, city->day);

    if (city->energy==0||
        city->water==0||
        city->stability==0){

            printf(RED"\nWARNING: CITY IS IN CRITICAL CONDITION!\n" RESET);
        }
}

// SAVE GAME
void saveGame(const City*city){
    FILE*file=fopen(SAVE_FILE, "wb");

    if (file==NULL){
        printf(RED "\ERROR: SAVE FAILED.\n" RESET);
        return;
    }
    if (fwrite(city,sizeof(City), 1, file) !=1){
        printf(RED "\nERROR: SAVE FAILED.\n" RESET);
    }else {
        printf(GREEN "\nCITY PROGRESS SAVED SUCCESSFULLY!\n" RESET);
    }
    fclose(file);
}
//LOAD GAME
void loadGame(City*city){
    FILE*file=fopen(SAVE_FILE, "rb");

    if (file == NULL){
        printf(RED "\nNO SAVED CITY FOUND.\n" RESET);
        return;
    }
    City loadedCity;

    if (fread(&loadedCity, sizeof(City), 1, file) !=1){
        printf(RED "\nERROR: COULD NOT LOAD CITY\n." RESET);
    }else if (memchr(loadedCity.name, '\0',
                     sizeof(loadedCity.name))== NULL ||
              loadedCity.day<1||
              loadedCity.population<0||
              loadedCity.energy<0 || loadedCity.energy>100 ||
              loadedCity.water<0 || loadedCity.water> 100||
              loadedCity.stability<0||
              loadedCity.stability>100||
              loadedCity.credits<0){
                  printf(RED "\nSAVE FILE IS INVALID.\n" RESET);
              }else{
                  *city=loadedCity;
                  printf(GREEN "\nCITY PROGRESS LOADED SUCCESSFULLY!\n" RESET);
              }
              fclose(file);
}
//MAIN PROGRAM
int main(void){

    srand((unsigned int) time(NULL));

    City myCity ={
        "NEON METROPOLIS",
        1,
        50000,
        80,
        75,
        500,
        85
    };
    startup();
    int running = 1;

    while (running){
        displayDashboard(&myCity);
        showMenu();

        int choice= getChoice();
        switch(choice){

        case 1:
            manageEnergy(&myCity);
            break;

        case 2:
            manageWater(&myCity);
            break;

        case 3:
            earnCredits(&myCity);
            break;

        case 4:
            improveStability(&myCity);
            break;

        case 5:
            advanceDay(&myCity);
            break;

        case 6:
            saveGame(&myCity);
            break;

        case 7:
            loadGame(&myCity);
            break;

        case 8:
            displayDashboard(&myCity);
            break;

        case 0:
            printf(RED "\nSHUTTING DOWN NEON CITY SYSTEM...\n" RESET);
            running=0;
            break;

        default:
            printf(RED "\nINVALID COMMAND. TRY AGAIN.\n RESET");
        }
        if (running){
            printf("\n Press Enter to continue...");
            getchar();
        }
    }
    printf(GREEN "\nSYSTEM OFFLINE. GOODBYE, COMMANDER.\n" RESET);

    return 0;
}
