#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "issue.h"

// Function to get the current date and time
void get_current_datetime(char *date, char *time_str, size_t size_date, size_t size_time)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(date, size_date, "%Y-%m-%d", t);     // store date in date array
    strftime(time_str, size_time, "%H:%M:%S", t); // store time in time array
}

// Function definition to send mail
void send_email(char *to_email, char *subject, char *message)
{
    char command[1024];

    // printf("\033[33mPlease wait...Sending email to: %s\033[0m\n", to_email);
    snprintf(command, sizeof(command), "echo \"Subject: %s\n\n%s\" | msmtp --from=kasthuri2021eee@gmail.com -t %s", subject, message, to_email);
    system(command);
}

// Function definition for adding issue
void Add_Issue(char title[], char description[], char priority[], char status[], char user_name[], char email[])
{
    // Open file in append mode to write issue
    FILE *file = fopen("issues_log.csv", "a");

    // Check whether the file is opened or not
    if (file == NULL)
    {
        printf("Failed to open file\n");
        return; // Exit if the file couldn't be opened
    }

    // Get last updated id from function
    int id = get_last_id();

    // Increment id for every new issue
    id++;

    char date[11], time[9];

    get_current_datetime(date, time, sizeof(date), sizeof(time)); // Get the current date

    // After getting data from the user, write it to the file
    fprintf(file, "%d,%s,%s,%s,%s,%s,%s,%s\n", id, title, description, priority, status, user_name, date, time);

    // Close the file
    fclose(file);

    // Update the last id in the last_id.txt file
    update_last_id(id);

    // Print success message to user
    printf("\n\033[92mSuccessfully Added Issue by %s with id %d \033[0m\n\n", user_name, id);

    // Send email notification
    char email_subject[100], email_body[500];
    snprintf(email_subject, sizeof(email_subject), "New Issue Added ID %d by %s", id, user_name);
    snprintf(email_body, sizeof(email_body), "Issue Details:\n\nID: %d\n\nTitle: %s\nDescription: %s\n\nPriority: %s\n\nStatus: %s\n\nUser: %s\n\nDate: %s\n\nTime: %s\n\n", id, title, description, priority, status, user_name, date, time);

    // Promt user to wait for sending mail to manager
    printf("\033[33mPlease wait...Sending email to Manager and User\033[0m\n");
    // function call to send email to manager and user
    send_email("kasthuriselvi2000@gmail.com", email_subject, email_body);
    send_email(email, email_subject, email_body);
}

// Function definition for list the all issues
void List_Issues(void)
{
    // Open file in read mode to display issues
    FILE *file = fopen("issues_log.csv", "r");
    // check whether the file is opened or not
    if (file == NULL)
    {
        printf("Failed to open file\n");
        return;
    }
    // create variable to store id,description,prority,status,datetime
    int id;
    char title[100], description[100], priority[100], status[100], user_name[100], date[11], time[9];
    // create buffer variable to store each line from file
    char line[1000];
    // create variable to know issue details present or not
    int found_issue = 0;

    printf("\n\033[91mList of Issues\033[0m\n\n");
    printf("\033[100m%-5s| %-25s| %-40s| %-15s| %-15s| %-20s| %-12s| %-10s|\033[0m\n", "ID", "Title", "Description", "Priority", "Status", "Assigned User", "Date", "Time");

    while (fgets(line, sizeof(line), file))
    {
        // Remove new line character if any present
        line[strcspn(line, "\n")] = 0;

        // Split data from csv file and store it in variables
        sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]", &id, title, description, priority, status, user_name, date, time);

        // Print the issue details in properly aligned format
        printf("\033[37m%-5d| %-25s| %-40s| %-15s| %-15s| %-20s| %-12s| %-10s|\033[0m\n", id, title, description, priority, status, user_name, date, time);

        // Print separator line after each entry
        printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        found_issue = 1;
    }

    if (!found_issue)
    {
        printf("Error: No issues found in the file.\n");
    }

    // Close file after read data
    fclose(file);
}

// Function definition for Editing the issues

void Edit_Issues(int user_id)
{
    printf("Edit Issue\n");

    // Open the .csv file to read existing issues.
    FILE *file = fopen("issues_log.csv", "r");

    // Check whether the file is present or not
    if (!file)
    {
        perror("Error opening file");
        return;
    }

    // Temporary file to store updated content.
    FILE *temp_file = fopen("temp.csv", "w");
    if (!temp_file)
    {
        perror("Error creating temporary file");
        fclose(file);
        return;
    }

    // Variables to hold current line data.
    int id;
    char line[1000];
    char title[100], description[100], priority[100], status[100], user_name[100], date[11], time[9];
    int found = 0;

    // Loop to go line by line
    while (fgets(line, sizeof(line), file))
    {
        // Remove the newline character.
        line[strcspn(line, "\n")] = 0;

        // Get data from issues_log.csv
        sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", &id, title, description, priority, status, user_name, date, time);

        // Check if the user-entered ID matches any of the existing issue IDs in the file.
        if (id == user_id)
        {
            // Display current issue details.
            printf("\n\033[91mCurrent Details\033[0m\n\n");
            printf("\033[100m%-5s| %-25s| %-40s| %-15s| %-15s| %-20s| %-12s| %-10s|\033[0m\n", "ID", "Title", "Description", "Priority", "Status", "Assigned User", "Date", "Time");
            // Print the issue details in properly aligned format
            printf("\033[37m%-5d| %-25s| %-40s| %-15s| %-15s| %-20s| %-12s| %-10s|\033[0m\n", id, title, description, priority, status, user_name, date, time);
            printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------\n");

            // Prompt the user for new details.
            printf("\033[34mEnter New Priority: \033[0m");
            scanf("%s", priority);
            while (getchar() != '\n')
                ; // Clear newline character from input buffer.

            printf("\n\033[34mEnter New Status: \033[0m");
            fgets(status, sizeof(status), stdin);
            status[strcspn(status, "\n")] = 0; // Remove newline.

            printf("\n\033[34mEnter User Name to Assign Issue: \033[0m");
            fgets(user_name, sizeof(user_name), stdin);
            user_name[strcspn(user_name, "\n")] = 0; // Remove new line.

            // Indicate the issue was found and modified.
            found = 1;

            // Function call to get current date and time
            get_current_datetime(date, time, sizeof(date), sizeof(time)); // Get the current date
        }

        // Write the updated issue details to the temporary file.
        fprintf(temp_file, "%d,%s,%s,%s,%s,%s,%s,%s\n", id, title, description, priority, status, user_name, date, time);
    }

    fclose(file);
    fclose(temp_file);

    // Replace the original file with the temporary file.
    if (found)
    {
        if (remove("issues_log.csv") == 0 && rename("temp.csv", "issues_log.csv") == 0)
        {
            printf("\n\033[92mSuccessfully updated the issue.\033[0m\n\n");
        }
        else
        {
            perror("Error updating the file");
        }
    }
    else
    {
        printf("\033[91mIssue with ID %d not found.\033[0m\n", id);
        remove("temp.csv"); // Clean up temporary file.
    }
}

// Function definition to search issue
void Search_Issues(int user_id)
{
    printf("\n\033[91mSearch Issue\033[0m\n\n");

    // Open the .csv file to read IDs and check if the user-entered ID already exists.
    FILE *file = fopen("issues_log.csv", "r");

    int found = 0;

    // create buffer to store data from file
    int id;
    char line[1000];
    char title[100], priority[100], status[100], user_name[100], description[100], date[11], time[9];

    while (fgets(line, sizeof(line), file))
    {
        // Remove new line character
        line[strcspn(line, "\n")] = 0;

        // split data from csv file and store it in variable
        sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", &id, title, description, priority, status, user_name, date, time);

        if (id == user_id)
        {
            printf("\033[100m%-5s| %-25s| %-40s| %-15s| %-15s| %-20s| %-12s| %-10s|\033[0m\n", "ID", "Title", "Description", "Priority", "Status", "Assigned User", "Date", "Time");
            // Print the issue details in properly aligned format
            printf("\033[37m%-5d| %-25s| %-40s| %-15s| %-15s| %-20s| %-12s| %-10s|\033[0m\n", id, title, description, priority, status, user_name, date, time);
            // print data from file
            printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
            found = 1;
        }
    }
    if (found == 0)
    {
        printf("Error:No issues found in the file\n");
    }
    printf("\n");

    // Close file after displaying issue
    fclose(file);
}

// Function definition to resolve issue

void Resolve_Issues(int user_entered_id, char mail_Id[], char user[])
{
    printf("\n\033[91mUpdate issue status as resolved\033[0m\n");

    // Open the .csv file to read existing issues.
    FILE *file = fopen("issues_log.csv", "r");

    // Check whether the file is present or not
    if (!file)
    {
        perror("Error opening file");
        return;
    }

    // Temporary file to store updated content.
    FILE *temp_file = fopen("temp.csv", "w");
    if (!temp_file)
    {
        perror("Error creating temporary file");
        fclose(file);
        return;
    }

    // Variables to hold current line data.
    int id;
    char line[1000];
    char title[100], description[100], priority[100], status[100], user_name[100], date[11], time[9];
    int found = 0;

    // Loop to go line by line
    while (fgets(line, sizeof(line), file))
    {
        // Remove the newline character.
        line[strcspn(line, "\n")] = 0;

        // Get data from issues_log.csv
        sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", &id, title, description, priority, status, user_name, date, time);

        // Check if the user-entered ID matches any of the existing issue IDs in the file.
        if (id == user_entered_id)
        {
            // Display current issue details.
            printf("\n\033[91mCurrent Details\033[0m\n\n");
            // printf("\033[100mID    |       Description                   |       Priority       |       Status         |     Assigned User    |    Date     |   Time      \033[0m\n");
            // printf("\033[37m| %-3d | %-35s | %-20s | %-20s | %-20s | %-11s | %-10s |\033[0m\n", id, description, priority, status, user_name, date, time);
            printf("\033[100m%-5s| %-25s| %-40s| %-15s| %-15s| %-20s| %-12s| %-10s|\033[0m\n", "ID", "Title", "Description", "Priority", "Status", "Assigned User", "Date", "Time");
            // Print the issue details in properly aligned format
            printf("\033[37m%-5d| %-25s| %-40s| %-15s| %-15s| %-20s| %-12s| %-10s|\033[0m\n", id, title, description, priority, status, user_name, date, time);
            printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
            while (getchar() != '\n')
                ;

            // Prompt the user for new details.
            printf("\033[34mEnter New Description: \033[0m");
            fgets(description, sizeof(description), stdin);
            description[strcspn(description, "\n")] = 0; // Remove newline.

            printf("\n\033[34mEnter New Status: \033[0m");
            fgets(status, sizeof(status), stdin);
            status[strcspn(status, "\n")] = 0; // Remove newline.

            // Indicate the issue was found and modified.
            found = 1;
            printf("\n\033[92mUpdated issue ID: %d\n\033[0m", id);
            get_current_datetime(date, time, sizeof(date), sizeof(time)); // Get the current date

            // Send email notification
            char email_subject[5000], email_body[5000];
            snprintf(email_subject, sizeof(email_subject), "ID %d Issue Resolved by %s", id, user);
            snprintf(email_body, sizeof(email_body), "Issue Details:\n\nID: %d\n\nTitle: %s\n\nDescription: %s\n\nPriority: %s\n\nStatus: %s\n\nUser: %s\n\nDate: %s\n\nTime: %s\n\n", id, title, description, priority, status, user, date, time);
            // Promt user to wait for sending mail to manager
            printf("\033[33mPlease wait...Sending email to Manager and User\033[0m\n");
            send_email("kasthuriselvi2000@gmail.com", email_subject, email_body);
            send_email(mail_Id, email_subject, email_body);
        }

        // Write the updated issue details to the temporary file.
        fprintf(temp_file, "%d,%s,%s,%s,%s,%s,%s,%s\n", id, title, description, priority, status, user_name, date, time);
    }
    //Close file and temp file
    fclose(file);
    fclose(temp_file);

    // Replace the original file with the temporary file.
    if (found)
    {
        if (remove("issues_log.csv") == 0 && rename("temp.csv", "issues_log.csv") == 0)
        {
            printf("\n\033[92mSuccessfully updated the issue.\033[0m\n\n");
        }
        else
        {
            perror("Error updating the file");
        }
    }
    else
    {
        printf("\033[91mIssue with ID %d not found.\033[0m\n", id);
        remove("temp.csv"); // Clean up temporary file.
    }
}

// Function to get last id
int get_last_id()
{
    // Open file for read id
    FILE *file = fopen("last_id.txt", "r");

    // Create variable to store initial id
    int last_id = 100;

    // Check whether the file opened, if opened means read data from file
    if (file != NULL)
    {
        // Read id from file
        fscanf(file, "%d", &last_id);

        // Close file after read
        fclose(file);
    }

    // Return the last used ID that was read from the file
    return last_id;
}

// Function definition to write last updated id
void update_last_id(int new_id)
{
    // Open last_id.txt file to write last updated id
    FILE *file = fopen("last_id.txt", "w");

    if (file != NULL)
    {
        // Write last updated id to file
        fprintf(file, "%d", new_id);

        // Close file after wrote
        fclose(file);
    }
}
