#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MOVIES 100
#define MAX_SEATS 50
#define MAX_DATES 2
#define MAX_TIMINGS 4

const char *timings[MAX_TIMINGS] = {"9:00", "12:15", "3:30", "7:30"};
const char *dates[MAX_DATES] = {"Today", "Tommorow"};

// Structure to store movie details
typedef struct {
    char name[100];
    int ticketsSold;
    int ticketPrice;
    int totalSeats;
    int availableSeats[MAX_DATES][MAX_TIMINGS]; // Available seats for each date and time
    int seats[MAX_DATES][MAX_TIMINGS][MAX_SEATS]; // 3D array to track booked seats for each date and time
} Movie;

// Function to load movie data from file
int loadMovies(Movie movies[], int *count) {
    FILE *file = fopen("movies.txt", "r");
    if (!file) return 0;

    *count = 0;
    while (fgets(movies[*count].name, sizeof(movies[*count].name), file)) {
        if (movies[*count].name[0] == '\n') { // Skip empty lines
            continue;
        }
        movies[*count].name[strcspn(movies[*count].name, "\n")] = '\0'; // Remove newline

        if (fscanf(file, "%d %d %d\n", &movies[*count].ticketsSold, &movies[*count].ticketPrice, &movies[*count].totalSeats) != 3) {
            break; // Handle read failure
        }
        
        for (int d = 0; d < MAX_DATES; d++) {
            for (int t = 0; t < MAX_TIMINGS; t++) {
                fscanf(file, "%d", &movies[*count].availableSeats[d][t]);
                for (int i = 0; i < movies[*count].totalSeats; i++) {
                    fscanf(file, "%d", &movies[*count].seats[d][t][i]);
                }
            }
        }

        (*count)++;
    }
    fclose(file);
    return 1;
}

// Function to save movie data to file
void saveMovies(Movie movies[], int count) {
    FILE *file = fopen("movies.txt", "w");
    if (!file) {
        printf("Error opening file for writing!\n");
        return;
    }

    fprintf(file, "==========================================\n");
    fprintf(file, "         QFX CINEMAS - MOVIES LIST       \n");
    fprintf(file, "==========================================\n\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "Movie Name: %s\n", movies[i].name);
        fprintf(file, "------------------------------------------\n");
        fprintf(file, "Tickets Sold   : %d\n", movies[i].ticketsSold);
        fprintf(file, "Ticket Price   : %d\n", movies[i].ticketPrice);
        fprintf(file, "Total Seats    : %d\n", movies[i].totalSeats);
        fprintf(file, "------------------------------------------\n");
        for (int d = 0; d < MAX_DATES; d++) {
            for (int t = 0; t < MAX_TIMINGS; t++) {
                fprintf(file, "Date: %s, Time: %s\n", dates[d], timings[t]);
                fprintf(file, "Available Seats: %d\n", movies[i].availableSeats[d][t]);
                fprintf(file, "Seat Map: (0 = Available, 1 = Booked)\n");
                for (int j = 0; j < movies[i].totalSeats; j++) {
                    fprintf(file, "%d ", movies[i].seats[d][t][j]);
                    if ((j + 1) % 10 == 0) fprintf(file, "\n");
                }
                fprintf(file, "\n");
            }
        }
        fprintf(file, "==========================================\n\n");
    }
    fclose(file);
}

// Function to display movies in a table format
void displayMovies(Movie movies[], int count) {
    printf("\n================================================================\n");
    printf("| %-20s | %-10s | %-10s |\n", "Movie Name", "Price(Rs)", "Total Seats");
    printf("================================================================\n");
    for (int i = 0; i < count; i++) {
        printf("| %-20s | %-10d | %-10d |\n", movies[i].name, movies[i].ticketPrice, movies[i].totalSeats);
    }
    printf("================================================================\n");
}

// Function to add a new movie
void addMovie(Movie movies[], int *count) {
    if (*count >= MAX_MOVIES) {
        printf("Movie list is full!\n");
        return;
    }
    
    printf("Enter movie name: ");
    fgets(movies[*count].name, sizeof(movies[*count].name), stdin);
    movies[*count].name[strcspn(movies[*count].name, "\n")] = '\0';

    printf("Enter ticket price: ");
    scanf("%d", &movies[*count].ticketPrice);
    
    printf("Enter total seats available: ");
    scanf("%d", &movies[*count].totalSeats);
    
    movies[*count].ticketsSold = 0;
    
    for (int d = 0; d < MAX_DATES; d++) {
        for (int t = 0; t < MAX_TIMINGS; t++) {
            movies[*count].availableSeats[d][t] = movies[*count].totalSeats;
            for (int i = 0; i < movies[*count].totalSeats; i++) {
                movies[*count].seats[d][t][i] = 0; // Initialize all seats as available
            }
        }
    }
    
    (*count)++;
    saveMovies(movies, *count);
    printf("Movie added successfully!\n");
}

// Function to calculate total revenue
void calculateRevenue(Movie movies[], int count) {
    int totalRevenue = 0;
    for (int i = 0; i < count; i++) {
        totalRevenue += movies[i].ticketsSold * movies[i].ticketPrice;
    }
    printf("\n========================================\n");
    printf("| Total Revenue: Rs.%-10d           |\n", totalRevenue);
    printf("========================================\n");
}

// Function to search for a movie by name
void searchMovie(Movie movies[], int count) {
    if (count == 0) {
        printf("No movies available to search!\n");
        return;
    }

    char searchName[100];
    printf("Enter the name of the movie to search: ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = '\0'; // Remove newline

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(movies[i].name, searchName) == 0) {
            found = 1;
            printf("\n========================================\n");
            printf("Movie Details for: %s\n", movies[i].name);
            printf("========================================\n");
            printf("Tickets Sold   : %d\n", movies[i].ticketsSold);
            printf("Ticket Price   : %d\n", movies[i].ticketPrice);
            printf("Total Seats    : %d\n", movies[i].totalSeats);
            printf("----------------------------------------\n");
            for (int d = 0; d < MAX_DATES; d++) {
                for (int t = 0; t < MAX_TIMINGS; t++) {
                    printf("Date: %s, Time: %s\n", dates[d], timings[t]);
                    printf("Available Seats: %d\n", movies[i].availableSeats[d][t]);
                    printf("Seat Map: (0 = Available, 1 = Booked)\n");
                    for (int j = 0; j < movies[i].totalSeats; j++) {
                        printf("%d ", movies[i].seats[d][t][j]);
                        if ((j + 1) % 10 == 0) printf("\n");
                    }
                    printf("\n");
                }
            }
            printf("========================================\n");
            break;
        }
    }

    if (!found) {
        printf("Movie not found!\n");
    }
}

// Function to book tickets
void bookTickets(Movie movies[], int count) {
    if (count == 0) {
        printf("No movies available for booking!\n");
        return;
    }

    printf("\nAvailable Movies:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, movies[i].name);
    }
    
    int movieIndex;
    printf("Select a movie (1-%d): ", count);
    scanf("%d", &movieIndex);
    getchar();
    
    if (movieIndex < 1 || movieIndex > count) {
        printf("Invalid selection!\n");
        return;
    }
    movieIndex--;

    // Select date
    printf("Available Dates:\n");
    for (int i = 0; i < MAX_DATES; i++) {
        printf("%d. %s\n", i + 1, dates[i]);
    }
    int dateIndex;
    printf("Select a date (1-%d): ", MAX_DATES);
    scanf("%d", &dateIndex);
    getchar();
    if (dateIndex < 1 || dateIndex > MAX_DATES) {
        printf("Invalid date selection!\n");
        return;
    }
    dateIndex--;

    // Select timing
    printf("Available Timings:\n");
    for (int i = 0; i < MAX_TIMINGS; i++) {
        printf("%d. %s\n", i + 1, timings[i]);
    }
    int timeIndex;
    printf("Select timing (1-%d): ", MAX_TIMINGS);
    scanf("%d", &timeIndex);
    getchar();
    if (timeIndex < 1 || timeIndex > MAX_TIMINGS) {
        printf("Invalid timing selection!\n");
        return;
    }
    timeIndex--;

    if (movies[movieIndex].availableSeats[dateIndex][timeIndex] == 0) {
        printf("Sorry, no seats available!\n");
        return;
    }

    int numTickets, seatNumber;
    printf("Enter number of tickets: ");
    scanf("%d", &numTickets);
    if (numTickets > movies[movieIndex].availableSeats[dateIndex][timeIndex]) {
        printf("Not enough seats available!\n");
        return;
    }

    printf("Available Seats: ");
    for (int j = 0; j < movies[movieIndex].totalSeats; j++) {
        if (movies[movieIndex].seats[dateIndex][timeIndex][j] == 0) {
            printf("%d ", j + 1);
        }
    }
    printf("\n");

    for (int j = 0; j < numTickets; j++) {
        do {
            printf("Choose seat number: ");
            scanf("%d", &seatNumber);
            if (movies[movieIndex].seats[dateIndex][timeIndex][seatNumber - 1] != 0) {
                printf("Seat already booked! Choose another seat.\n");
            }
        } while (movies[movieIndex].seats[dateIndex][timeIndex][seatNumber - 1] != 0);
        movies[movieIndex].seats[dateIndex][timeIndex][seatNumber - 1] = 1;
    }

    printf("\nChoose payment method (1. Esewa / 2. Khalti): ");
    int paymentMethod;
    scanf("%d", &paymentMethod);
    printf("Payment successful!\n");

    movies[movieIndex].ticketsSold += numTickets;
    movies[movieIndex].availableSeats[dateIndex][timeIndex] -= numTickets;

    // Show booking summary
    printf("\n====================================\n");
    printf("Booking Confirmation\n");
    printf("====================================\n");
    printf("Movie: %s\n", movies[movieIndex].name);
    printf("Date: %s\n", dates[dateIndex]);
    printf("Time: %s\n", timings[timeIndex]);
    printf("Seats Booked: %d\n", numTickets);
    printf("Total Price: Rs. %d\n", numTickets * movies[movieIndex].ticketPrice);
    printf("====================================\n");
}

int main() {
    Movie movies[MAX_MOVIES];
    int count = 0;
    if (!loadMovies(movies, &count)) {
        printf("Failed to load movies from file.\n");
    }

    int choice;
    do {
        printf("\nQFX Cinemas - Movie Ticket Booking System\n");
        printf("========================================\n");
        printf("1. Display Movies\n2. Add Movie\n3. Book Tickets\n4. Calculate Revenue\n5. Search Movie\n6. Exit\nEnter choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            displayMovies(movies, count);
        } else if (choice == 2) {
            addMovie(movies, &count);
        } else if (choice == 3) {
            bookTickets(movies, count);
        } else if (choice == 4) {
            calculateRevenue(movies, count);
        } else if (choice == 5) {
            searchMovie(movies, count);
        }
    } while (choice != 6);

    saveMovies(movies, count); // Save movies before exiting
    return 0;
}
