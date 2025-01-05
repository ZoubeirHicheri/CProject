#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CARS 100
#define MAX_STRING_LENGTH 50
#define DATABASE_FILE "carsdb.txt"

typedef struct {
    int id;
    char make[MAX_STRING_LENGTH];
    char model[MAX_STRING_LENGTH];
    int year;            // year of fabrication of the car
    float daily_rate;
    int is_available;   // Specifies whether the car available or not
    int rented_days;   // Days the car is rented
    float total_cost;  // Total rental cost
    char renter_name[MAX_STRING_LENGTH]; // Name of the renter
    char renter_id[MAX_STRING_LENGTH];   // ID number of the renter
} Car;

Car cars[MAX_CARS];
int total_cars = 0;

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// function that load cars from the file carsdb.txt (as indicated in the function name)
void load_cars_from_file() {
    FILE *file = fopen(DATABASE_FILE, "r");
    if (file == NULL) {
        printf("Database file not found. Starting with empty database.\n");
        return;
    }

    total_cars = 0;
    char line[256];
    
    while (fgets(line, sizeof(line), file) != NULL && total_cars < MAX_CARS) {
        Car *car = &cars[total_cars];
        if (sscanf(line, "%d|%[^|]|%[^|]|%d|%f|%d|%d|%f|%[^|]|%s", 
                   &car->id, 
                   car->make, 
                   car->model, 
                   &car->year, 
                   &car->daily_rate, 
                   &car->is_available, 
                   &car->rented_days,
                   &car->total_cost,
                   car->renter_name,
                   car->renter_id) == 10) {
            total_cars++;
        }
    }

    fclose(file);
    printf("Successfully loaded %d cars from database.\n", total_cars);
}
// function that saves the inserted data into the file
void save_cars_to_file() {
    FILE *file = fopen(DATABASE_FILE, "w");
    if (file == NULL) {
        perror("Error opening database file for writing");
        return;
    }

    printf("\nSaving %d cars to database...\n", total_cars);
    
    for (int i = 0; i < total_cars; i++) {
        fprintf(file, "%d|%s|%s|%d|%.2f|%d|%d|%.2f|%s|%s\n",
                 cars[i].id,
                 cars[i].make,
                 cars[i].model,
                 cars[i].year,
                 cars[i].daily_rate,
                 cars[i].is_available,
                 cars[i].rented_days,
                 cars[i].total_cost,
                 cars[i].renter_name,
                 cars[i].renter_id);
    }

    fclose(file);
    printf("Database save completed.\n");
}

int find_car_by_id(int id) {
    for (int i = 0; i < total_cars; i++) {
        if (cars[i].id == id) {
            return i;
        }
    }
    return -1;
}

void add_car() {
    if (total_cars >= MAX_CARS) {
        printf("Error: Database is full. Cannot add more cars.\n");
        return;
    }

    Car new_car;
    printf("\nEnter Car Details\n");
    printf("----------------\n");
    
    new_car.id = total_cars + 1;  // Auto-increment ID
    printf("Enter Make: ");
    fgets(new_car.make, MAX_STRING_LENGTH, stdin);
    new_car.make[strcspn(new_car.make, "\n")] = 0;

    printf("Enter Model: ");
    fgets(new_car.model, MAX_STRING_LENGTH, stdin);
    new_car.model[strcspn(new_car.model, "\n")] = 0;

    printf("Enter Year: ");
    scanf("%d", &new_car.year);
    clear_input_buffer();

    printf("Enter Daily Rate: ");
    scanf("%f", &new_car.daily_rate);
    clear_input_buffer();

    new_car.is_available = 1;  // Set as available by default
    new_car.rented_days = 0;
    new_car.total_cost = 0.0;
    new_car.renter_name[0] = '\0';  // No renter by default
    new_car.renter_id[0] = '\0';    // No renter ID by default
    cars[total_cars++] = new_car;

    save_cars_to_file();
    printf("Car added successfully.\n");
}

void display_all_cars() {
    if (total_cars == 0) {
        printf("\nNo cars in the database.\n");
        return;
    }

    printf("\n--- Car Inventory ---\n");
    printf("%-5s %-15s %-15s %-6s %-12s %-10s %-12s %-12s %-15s %-10s\n", 
           "ID", "Make", "Model", "Year", "Daily Rate", "Status", "Days Rented", "Total Cost", "Renter Name", "Renter ID");
    printf("---------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < total_cars; i++) {
        printf("%-5d %-15s %-15s %-6d $%-11.2f %-10s %-12d $%-11.2f %-15s %-10s\n",
               cars[i].id,
               cars[i].make,
               cars[i].model,
               cars[i].year,
               cars[i].daily_rate,
               cars[i].is_available ? "Available" : "Rented",
               cars[i].rented_days,
               cars[i].total_cost,
               cars[i].renter_name[0] ? cars[i].renter_name : "N/A",
               cars[i].renter_id[0] ? cars[i].renter_id : "N/A");
    }
}

void rent_car() {
    if (total_cars == 0) {
        printf("\nNo cars available in the database.\n");
        return;
    }

    display_all_cars();
    
    int car_id;
    printf("\nEnter Car ID to rent: ");
    scanf("%d", &car_id);
    clear_input_buffer();

    int index = find_car_by_id(car_id);
    if (index == -1) {
        printf("Error: Car with ID %d not found.\n", car_id);
        return;
    }

    if (!cars[index].is_available) {
        printf("Error: Car is already rented.\n");
        return;
    }

    int rented_days;
    printf("Enter number of days to rent: ");
    scanf("%d", &rented_days);
    clear_input_buffer();

    if (rented_days <= 0) {
        printf("Error: Invalid number of rental days.\n");
        return;
    }

    char renter_name[MAX_STRING_LENGTH];
    char renter_id[MAX_STRING_LENGTH];

    printf("Enter renter's name: ");
    fgets(renter_name, MAX_STRING_LENGTH, stdin);
    renter_name[strcspn(renter_name, "\n")] = 0;

    printf("Enter renter's ID number: ");
    fgets(renter_id, MAX_STRING_LENGTH, stdin);
    renter_id[strcspn(renter_id, "\n")] = 0;

    // Store renter's information
    strcpy(cars[index].renter_name, renter_name);
    strcpy(cars[index].renter_id, renter_id);
    cars[index].rented_days = rented_days;
    cars[index].total_cost = rented_days * cars[index].daily_rate;
    cars[index].is_available = 0;  // Mark as rented

    save_cars_to_file();
    printf("Car rented successfully!\n");
    printf("Total cost for %d days: $%.2f\n", rented_days, cars[index].total_cost);
}

void return_car() {
    int car_id;
    printf("\nEnter Car ID to return: ");
    scanf("%d", &car_id);
    clear_input_buffer();

    int index = find_car_by_id(car_id);
    if (index == -1) {
        printf("Error: Car with ID %d not found.\n", car_id);
        return;
    }

    if (cars[index].is_available) {
        printf("Error: Car is not currently rented.\n");
        return;
    }

    cars[index].is_available = 1;  // Mark as available again
    cars[index].rented_days = 0;
    cars[index].total_cost = 0.0;
    cars[index].renter_name[0] = '\0';  // Clear renter name
    cars[index].renter_id[0] = '\0';    // Clear renter ID

    save_cars_to_file();
    printf("Car returned successfully!\n");
}

void delete_car() {
    if (total_cars == 0) {
        printf("\nNo cars in the database.\n");
        return;
    }

    int car_id;
    printf("\nEnter Car ID to delete: ");
    scanf("%d", &car_id);
    clear_input_buffer();

    int index = find_car_by_id(car_id);
    if (index == -1) {
        printf("Error: Car with ID %d not found.\n", car_id);
        return;
    }

    for (int i = index; i < total_cars - 1; i++) {
        cars[i] = cars[i + 1];
    }
    total_cars--;

    save_cars_to_file();
    printf("Car deleted successfully!\n");
}

void display_menu() {
    printf("\n=== Car Rental Management System ===\n");
    printf("1. Add New Car\n");
    printf("2. Display All Cars\n");
    printf("3. Rent a Car\n");
    printf("4. Return a Car\n");
    printf("5. Delete a Car\n");
    printf("6. Exit\n");
    printf("Enter your choice (1-6): ");
}

int main() {
    load_cars_from_file();
    int choice;

    do {
        display_menu();
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
                add_car();
                break;
            case 2:
                display_all_cars();
                break;
            case 3:
                rent_car();
                break;
            case 4:
                return_car();
                break;
            case 5:
                delete_car();
                break;
            case 6:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}
