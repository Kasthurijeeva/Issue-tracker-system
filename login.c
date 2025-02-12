#include <stdio.h>
#include <string.h>
#include "login.h"

int login_credentials(char user_name[], char password[])
{
    // Open the users.csv file in read mode
    FILE *file_users = fopen("users.csv", "r");

    // Check if the file opened successfully
    if (file_users == NULL)
    {
        printf("Error: Unable to open the file.\n");
        return 1;
    }

    // create array to store each line to read from the file
    char line[200];

    // Variables to store username and password from the file
    char file_username[100], file_password[100];
    int login_success = 0;

    // Read each line from the file
    while (fgets(line, sizeof(line), file_users))
    {
        // Remove newline character if present
        line[strcspn(line, "\n")] = 0;

        // Split the line into username and password
        sscanf(line, "%[^,],%s", file_username, file_password);

        // Check if the username and password match
        if (strcmp(user_name, file_username) == 0 && strcmp(password, file_password) == 0)
        {
            login_success = 1;
            break;
        }
    }

    // Close the file
    fclose(file_users);

    // Check if login was successful
    if (login_success)
    {
        printf("\n\033[92mLogin successful.\n\033[0m");
        return 0; // Login successful
    }
    else
    {
        printf("\n\033[91mLogin failed. Invalid username or password.\n\033[0m");
        return 1; // Login failed
    }
}

// Function definition for signup
int signup_credentials(char user_name[], char password[])
{
    FILE *file_users = fopen("users.csv", "r");

    // Check whether the file is opened or not
    if (file_users == NULL)
    {
        printf("Failed to open file\n");
    }

    // create array to store each line read from the file
    char line[200];
    // Variables to store username and password from the file
    char file_username[100], file_password[100];
    int login_success = 0;

    // Read each line from the file
    while (fgets(line, sizeof(line), file_users))
    {
        // Remove newline character if present
        line[strcspn(line, "\n")] = 0;

        // Split the line into username and password
        sscanf(line, "%[^,],%s", file_username, file_password);

        // Check if the username and password match
        if (strcmp(user_name, file_username) == 0)
        {
            printf("\n\033[91mUsername is already exist. Please Try again\n\033[0m\n");
            login_success = 1;
            break;
        }
        else if (strcmp(password, file_password) == 0)
        {
            printf("\n\033[91mPassword is already exist. Please Try again\n\033[0m\n");
            login_success = 1;
            break;
        }
    }
    // Close file after reading file
    fclose(file_users);
    // Reopen file to write username and password if given user name and password did not exist
    file_users = fopen("users.csv", "a");

    // Check whether the username or password exist , if no it will create new user name and password
    if (login_success != 1)
    {
        // store new username and password to users.csv file
        fprintf(file_users, "%s,%s\n", user_name, password);
        // Close file
        fclose(file_users);

        // Return success
        return 0;
    }

    if (login_success == 1)
    {
        return 1;
    }
}
