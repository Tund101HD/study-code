/*
 * Lab 6 - Section II: Car Refueling System
 */

#include <stdio.h>
#include <string.h>

#define MODEL_NAME_LENGTH 50

/*
 * Structure representing a car with fuel information
 */
typedef struct {
    float fuel_level;           /* Current fuel in liters */
    float max_fuel_level;       /* Maximum tank capacity in liters */
    char model[MODEL_NAME_LENGTH]; /* Car model name */
} Car;

/*
 * Refuel a car with a specified amount of fuel
 *
 * Parameters:
 *   car: Pointer to the Car struct to refuel
 *   amount: Amount of fuel to add in liters
 *
 * The function ensures the fuel level does not exceed max_fuel_level
 */
void refuel(Car *car, float amount) {
    if (car == NULL) {
        printf("Error: Invalid car pointer\n");
        return;
    }

    if (amount < 0.0f) {
        printf("Error: Cannot refuel with negative amount\n");
        return;
    }

    float new_level = car->fuel_level + amount;

    if (new_level > car->max_fuel_level) {
        /* Tank would overflow, fill only to maximum */
        float actually_added = car->max_fuel_level - car->fuel_level;
        car->fuel_level = car->max_fuel_level;

        printf("Refueling '%s':\n", car->model);
        printf("  Requested:     %.2f L\n", amount);
        printf("  Actually added: %.2f L\n", actually_added);
        printf("  Tank is now full!\n");
    } else {
        /* Normal refueling */
        car->fuel_level = new_level;

        printf("Refueling '%s':\n", car->model);
        printf("  Added:         %.2f L\n", amount);
        printf("  New level:     %.2f L / %.2f L\n",
               car->fuel_level, car->max_fuel_level);
    }
}

/*
 * Print current status of a car
 */
void print_car_status(const Car *car) {
    if (car == NULL) {
        printf("Error: Invalid car pointer\n");
        return;
    }

    float fill_percentage = (car->fuel_level / car->max_fuel_level) * 100.0f;

    printf("\n--- Car Status ---\n");
    printf("Model:         %s\n", car->model);
    printf("Fuel level:    %.2f L / %.2f L (%.1f%%)\n",
           car->fuel_level, car->max_fuel_level, fill_percentage);
    printf("------------------\n\n");
}

int main(void) {
    printf("Lab 6 - Section II: Car Refueling System\n");
    printf("=========================================\n\n");
    Car my_car;
    my_car.fuel_level = 15.5f;
    my_car.max_fuel_level = 60.0f;
    strncpy(my_car.model, "VW Golf", MODEL_NAME_LENGTH - 1);
    my_car.model[MODEL_NAME_LENGTH - 1] = '\0';
    print_car_status(&my_car);
    printf("Test 1: Refueling with 30 liters\n");
    refuel(&my_car, 30.0f);
    print_car_status(&my_car);
    printf("Test 2: Attempting to refuel with 20 liters (would exceed capacity)\n");
    refuel(&my_car, 20.0f);
    print_car_status(&my_car);
    printf("Test 3: Attempting to refuel with negative amount\n");
    refuel(&my_car, -10.0f);
    print_car_status(&my_car);

    return 0;
}
