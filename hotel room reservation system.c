#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROOMS 10
#define ROOM_PRICE 50.0  // Price per night

// Structure to hold room details
typedef struct {
    int roomNumber;
    char guestName[50];
    char phone[20];
    int days;
    float totalBill;
    int isBooked; // 0 = Available, 1 = Booked
} Room;

Room hotel[MAX_ROOMS];

// Function Prototypes
void initializeRooms();
void displayMenu();
void bookRoom();
void viewRooms();
void searchGuest();
void checkOut();
void saveData();
void loadData();

int main() {
    int choice;

    initializeRooms();
    loadData(); // Load previous records if available

    while (1) {
        displayMenu();
        printf("Enter your choice (1-6): ");
        if (scanf("%d", &choice) != 1) {
            printf("\nInvalid input! Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        switch (choice) {
            case 1:
                bookRoom();
                break;
            case 2:
                viewRooms();
                break;
            case 3:
                searchGuest();
                break;
            case 4:
                checkOut();
                break;
            case 5:
                saveData();
                printf("\nData saved successfully!\n");
                break;
            case 6:
                saveData();
                printf("\nThank you for using the Hotel Reservation System. Goodbye!\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }

    return 0;
}

// Initialize room default values
void initializeRooms() {
    for (int i = 0; i < MAX_ROOMS; i++) {
        hotel[i].roomNumber = 101 + i;
        hotel[i].guestName[0] = '\0';
        hotel[i].phone[0] = '\0';
        hotel[i].days = 0;
        hotel[i].totalBill = 0.0;
        hotel[i].isBooked = 0;
    }
}

// Display System Menu
void displayMenu() {
    printf("\n=========================================\n");
    printf("    HOTEL ROOM RESERVATION SYSTEM        \n");
    printf("=========================================\n");
    printf("1. Book a Room\n");
    printf("2. View All Rooms\n");
    printf("3. Search Guest Details\n");
    printf("4. Check-Out / Clear Room\n");
    printf("5. Save Data\n");
    printf("6. Exit\n");
    printf("=========================================\n");
}

// Book a room
void bookRoom() {
    int roomNum, found = 0;
    printf("\nEnter Room Number to Book (101 - %d): ", 100 + MAX_ROOMS);
    scanf("%d", &roomNum);

    for (int i = 0; i < MAX_ROOMS; i++) {
        if (hotel[i].roomNumber == roomNum) {
            found = 1;
            if (hotel[i].isBooked == 1) {
                printf("Sorry, Room %d is already booked!\n", roomNum);
            } else {
                printf("Enter Guest Name: ");
                getchar(); // Clear newline character from buffer
                fgets(hotel[i].guestName, sizeof(hotel[i].guestName), stdin);
                hotel[i].guestName[strcspn(hotel[i].guestName, "\n")] = 0; // Remove trailing newline

                printf("Enter Phone Number: ");
                fgets(hotel[i].phone, sizeof(hotel[i].phone), stdin);
                hotel[i].phone[strcspn(hotel[i].phone, "\n")] = 0;

                printf("Enter Number of Days: ");
                scanf("%d", &hotel[i].days);

                hotel[i].totalBill = hotel[i].days * ROOM_PRICE;
                hotel[i].isBooked = 1;

                printf("\n[SUCCESS] Room %d booked successfully for %s!\n", roomNum, hotel[i].guestName);
                printf("Total Bill: $%.2f\n", hotel[i].totalBill);
            }
            break;
        }
    }

    if (!found) {
        printf("Invalid Room Number!\n");
    }
}

// View room availability and status
void viewRooms() {
    printf("\n%-10s %-12s %-20s %-15s %-10s\n", "Room No", "Status", "Guest Name", "Phone", "Days");
    printf("--------------------------------------------------------------------\n");
    for (int i = 0; i < MAX_ROOMS; i++) {
        if (hotel[i].isBooked == 1) {
            printf("%-10d %-12s %-20s %-15s %-10d\n", 
                   hotel[i].roomNumber, "Booked", hotel[i].guestName, hotel[i].phone, hotel[i].days);
        } else {
            printf("%-10d %-12s %-20s %-15s %-10s\n", 
                   hotel[i].roomNumber, "Available", "-", "-", "-");
        }
    }
}

// Search guest by room number
void searchGuest() {
    int roomNum, found = 0;
    printf("\nEnter Room Number to Search: ");
    scanf("%d", &roomNum);

    for (int i = 0; i < MAX_ROOMS; i++) {
        if (hotel[i].roomNumber == roomNum) {
            found = 1;
            if (hotel[i].isBooked == 1) {
                printf("\n--- Details for Room %d ---\n", roomNum);
                printf("Guest Name : %s\n", hotel[i].guestName);
                printf("Phone      : %s\n", hotel[i].phone);
                printf("Days Stayed: %d\n", hotel[i].days);
                printf("Total Bill : $%.2f\n", hotel[i].totalBill);
            } else {
                printf("Room %d is currently Available (Not Booked).\n", roomNum);
            }
            break;
        }
    }

    if (!found) {
        printf("Room Number not found!\n");
    }
}

// Check-out guest and reset room status
void checkOut() {
    int roomNum, found = 0;
    printf("\nEnter Room Number for Check-Out: ");
    scanf("%d", &roomNum);

    for (int i = 0; i < MAX_ROOMS; i++) {
        if (hotel[i].roomNumber == roomNum) {
            found = 1;
            if (hotel[i].isBooked == 1) {
                printf("\n--- Check-Out Bill ---\n");
                printf("Guest Name : %s\n", hotel[i].guestName);
                printf("Total Paid : $%.2f\n", hotel[i].totalBill);
                printf("[SUCCESS] Check-out completed! Room %d is now available.\n", roomNum);

                // Reset room record
                hotel[i].guestName[0] = '\0';
                hotel[i].phone[0] = '\0';
                hotel[i].days = 0;
                hotel[i].totalBill = 0.0;
                hotel[i].isBooked = 0;
            } else {
                printf("Room %d is not currently booked!\n", roomNum);
            }
            break;
        }
    }

    if (!found) {
        printf("Room Number not found!\n");
    }
}

// Save data to binary file
void saveData() {
    FILE *fp = fopen("hotel_data.bin", "wb");
    if (fp == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }
    fwrite(hotel, sizeof(Room), MAX_ROOMS, fp);
    fclose(fp);
}

// Load data from binary file
void loadData() {
    FILE *fp = fopen("hotel_data.bin", "rb");
    if (fp == NULL) {
        return; // File doesn't exist yet on first run
    }
    fread(hotel, sizeof(Room), MAX_ROOMS, fp);
    fclose(fp);
}