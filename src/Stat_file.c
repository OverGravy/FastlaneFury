#include "../libs/Stat_file.h"

// function that open the file and return the file pointer
int check_statistic_file(){

    int row = 0;
    char c;

    CarFile.fp = fopen("../Assets/Data/Car.txt", "r");
    TruckFile.fp = fopen("../Assets/Data/Truck.txt", "r");
    MotorcycleFile.fp = fopen("../Assets/Data/Motorcycle.txt", "r");
    SupercarFile.fp = fopen("../Assets/Data/Supercar.txt", "r");

    if (CarFile.fp == NULL)
    {
        printf("Error opening file Car.txt\n");
        return 0;
    }
    if (TruckFile.fp == NULL)
    {
        printf("Error opening file Truck.txt\n");
        return 0;
    }
    if (MotorcycleFile.fp == NULL)
    {
        printf("Error opening file Motorcycle.txt\n");
        return 0;
    }
    if (SupercarFile.fp == NULL)
    {
        printf("Error opening file Supercar.txt\n");
        return 0;
    }

    // count the row of the Car file
    for (c = getc(CarFile.fp); c != EOF; c = getc(CarFile.fp))
        if (c == '\n') // Increment count if this character is newline
            row++;
    
    CarFile.rowNumber = row;
    // reset file pointer
    rewind(CarFile.fp);                                

    // count the row of the Truck file
    row = 0;
    for (c = getc(TruckFile.fp); c != EOF; c = getc(TruckFile.fp))
        if (c == '\n') // Increment count if this character is newline
            row++;
    
    TruckFile.rowNumber = row;
    // reset file pointer
    rewind(TruckFile.fp);

    // count the row of the Motorcycle file
    row = 0;
    for (c = getc(MotorcycleFile.fp); c != EOF; c = getc(MotorcycleFile.fp))
        if (c == '\n') // Increment count if this character is newline
            row++;
    
    MotorcycleFile.rowNumber = row;
    // reset file pointer
    rewind(MotorcycleFile.fp);


    // count the row of the Supercar file
    row = 0;
    for (c = getc(SupercarFile.fp); c != EOF; c = getc(SupercarFile.fp))
        if (c == '\n') // Increment count if this character is newline
            row++;
    
    SupercarFile.rowNumber = row;
    // reset file pointer
    rewind(SupercarFile.fp);

    return 1;
}

// function that read line and retun car Statistic struct
void get_veicle_staitistics(int type, struct Veicle_Statistics *Statistics){
   
    FILE * fp;
    int randomRow;

    switch (type)
    {
    case CAR:
        fp = CarFile.fp;
        randomRow = rand() % CarFile.rowNumber +1;
        break;
    case TRUCK:
        fp = TruckFile.fp;
        randomRow = rand() % TruckFile.rowNumber +1;
        break;
    case MOTORCYCLE:
        fp = MotorcycleFile.fp;
        randomRow = rand() % MotorcycleFile.rowNumber +1;
        break;
    case SUPERCAR:
        fp = SupercarFile.fp;
        randomRow = rand() % SupercarFile.rowNumber +1;
        break;
    }

    // read randomRow avoiding - as separator
    int i = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp))
    {
        if (i == randomRow)
        {
            sscanf(line, "%lf-%lf-%lf-%lf", &Statistics->maxSpeed, &Statistics->maxAcceleration, &Statistics->maxDeceleration, &Statistics->minDistance);
            break;
        }
        i++;
    }

    rewind(fp);
}

// function that close the file
void close_statistic_file(){
    fclose(CarFile.fp);
    fclose(TruckFile.fp);
    fclose(MotorcycleFile.fp);
    fclose(SupercarFile.fp);
}