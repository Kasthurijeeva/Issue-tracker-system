#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "main.h"
#include "login.h"
#include "issue.h"

//Main function
int main()
{
    int choice = 0;
    char user_name[100], password[100];

    // Print Title of project using ANSI escape
    printf("\033[91m\n====================================================================================\033[0m\033[41m     Issue Tracking System      \033[0m\033[91m==================================================================================\033[0m\n\n");

    // Loop to keep showing the menu until user chooses to exit (e.g., option 3)
    do
    {
        // Display menu to user
        printf("\033[35mMain Menu\033[0m\n\n");
        printf("1. Sign up\n2. Sign in\n3. Exit\n");
        // set ANSI escape code for red colour
        printf("\033[0;34m");
        printf("\n\033[34mEnter your choice: \033[0m");
        // Ask input from user
        scanf("%d", &choice);

        // Clear the input buffer to remove leftover newline after scanf
        while (getchar() != '\n')
            ;

        // Based on choice, prompt user for details or perform actions
        switch (choice)
        {
        case 2: // Sign in
            printf("Enter a user name: ");
            fgets(user_name, sizeof(user_name), stdin);
            // Remove trailing newline character if present
            user_name[strcspn(user_name, "\n")] = 0;

            printf("Enter a password: ");
            fgets(password, sizeof(password), stdin);
            // Remove trailing newline character if present
            password[strcspn(password, "\n")] = 0;
            // Function calling to login in to the system
            int result_login = login_credentials(user_name, password);
            if (result_login != 1)
            {
                printf("\n\033[32mWelcome %s! You are signed in.\n\n\033[0m", user_name);
                int Issue_choice;
                // Promt menu to user
                do
                {
                    printf("\033[35mIssue Menu\033[0m\n\n");
                    printf("1.Add Issue\n2.List All Issue\n3.Edit Issue\n4.Search Issue\n5.Resolve Issue\n6.Logout\n");

                    printf("\n\033[34mEnter your choice: \033[0m");
                    scanf("%d", &Issue_choice);

                    // Clear the input buffer after scanf to avoid newline consumption by fgets
                    while (getchar() != '\n')
                        ;

                    switch (Issue_choice)
                    {
                    case 1:
                        printf("\n\033[91mAdd Issue Details \033[0m\n");

                        // Array of issue details (Title, Description, Priority, Status)
                        char issueDetails[4][100];
                        const char *prompts[] = {
                            "Enter the Title of Issue :",
                            "Enter Issue Description :",
                            "Enter the Priority (High/Medium/Low):",
                            "Enter the Status (Open/In Progress/Resolved):"};

                        // Loop to get valid inputs
                        for (int i = 0; i < 4; i++)
                        {
                            printf("\n%s", prompts[i]);
                            fgets(issueDetails[i], sizeof(issueDetails[i]), stdin);
                            issueDetails[i][strcspn(issueDetails[i], "\n")] = 0; // Remove newline character

                            if (input(issueDetails[i]) == 1)
                            {
                                printf("\033[91mPlease re-enter.\033[0m\n");
                                i--; // Retry the same input
                            }
                        }

                        // Email input with validation
                        char mailID[100];
                        do
                        {
                            printf("\nEnter your mail ID :");
                            fgets(mailID, sizeof(mailID), stdin);
                            mailID[strcspn(mailID, "\n")] = 0; // Remove newline

                            if (!is_valid_email(mailID))
                            {
                                printf("\n\033[91mInvalid email format. Please enter a valid email (e.g., user@gmail.com): \033[0m\n");
                            }
                        } while (!is_valid_email(mailID));

                        // Call function with optimized parameters
                        Add_Issue(issueDetails[0], issueDetails[1], issueDetails[2], issueDetails[3], user_name, mailID);

                        printf("\n");
                        Issue_choice = 7;
                        break;
                    case 2:
                        List_Issues();
                        Issue_choice = 7;
                        break;
                    case 3:
                        // Create variable to store user enter issue id
                        int id;
                        // Promt user to enter issue id to edit
                        printf("Enter issue ID :");
                        scanf("%d", &id);
                        Edit_Issues(id);
                        Issue_choice = 7;
                        break;
                    case 4:
                        // Promt user to enter issue id to edit
                        printf("Enter issue ID :");
                        scanf("%d", &id);
                        Search_Issues(id);
                        Issue_choice = 7;
                        break;
                    case 5:
                        // Create variable to store user enter issue id
                        int Id;
                        char resolve_mailID[100];
                        // Promt user to enter issue id to edit
                        printf("Enter issue ID to Resolve:");
                        scanf("%d", &Id);
                        printf("Enter your mailID :");
                        scanf("%s", resolve_mailID);
                        // Validate the email format
                        while (!is_valid_email(resolve_mailID))
                        {
                            printf("\n\033[91mInvalid email format. Please enter a valid email (e.g., user@gmail.com): \033[0m");
                            scanf("%s", resolve_mailID);
                        }
                        Resolve_Issues(Id, resolve_mailID, user_name);
                        Issue_choice = 7;
                        break;
                    case 6:
                        choice = 6;
                        break;
                    default:
                        printf("\033[91mInvalid choice..:( Try again!\033[0m\n");
                    }
                } while (Issue_choice > 6 || Issue_choice < 1);
            }
            break;
        case 1: // Sign up
            printf("Enter a new user name: ");
            fgets(user_name, sizeof(user_name), stdin);
            // Remove trailing newline character if present
            user_name[strcspn(user_name, "\n")] = 0;

            printf("Enter a new password: ");
            fgets(password, sizeof(password), stdin);
            // Remove trailing newline character if present
            password[strcspn(password, "\n")] = 0;

            // Validate password
            if (!is_valid_password(password))
            {
                printf("\n\033[91mWeak password! Ensure it has letters, numbers, and special characters.\033[0m\n");
                return 1;
            }
            // function call to signup credentials
            int result_signup = signup_credentials(user_name, password);
            if (result_signup == 0)
            {
                printf("\n\033[92mAccount created successfully for %s! Now sigin for further.\n\n\033[0m", user_name);
            }
            choice = 5;
            break;
        case 3: // Exit
            printf("\n\033[91mExiting Issue Tracking System. Goodbye!\n\033[0m\n");
            return 0; // Exit the program
        default:      // Invalid option
            printf("\n\033[91mInvalid choice, please try again.\n\n\033[0m");
        }
    } while (choice > 3 || choice < 1);

    return 0;
}

// Function definition to valid password
int is_valid_password(const char *password)
{
    // Create variable's to ensure password has all these
    int has_letters = 0, has_special_char = 0, has_num = 0;

    // Create a constant variable to store all special character
    const char *special_chars = "!@#$%^&*()-_+=<>?/"; // Define allowed special characters

    // Loop to go through full password
    for (int i = 0; password[i] != '\0'; i++)
    {
        if (isalpha(password[i]))
        {
            has_letters = 1;
        }
        else if (isdigit(password[i]))
        {
            has_num = 1;
        }
        else if (strchr(special_chars, password[i]))
        {
            has_special_char = 1;
        }
    }

    // return value of has_letters , has_num and has_special_chars
    return (has_letters && has_special_char && has_num);
}

// Function to validate email format
int is_valid_email(const char email[])
{
    char *at_pos = strchr(email, '@'); // Find '@' in the email
    if (at_pos == NULL)
    {
        return 0; // '@' symbol not found
    }

    char *gmail_pos = strstr(at_pos, "gmail"); // Find 'gmail' after '@'
    if (gmail_pos == NULL || gmail_pos != at_pos + 1)
    {
        return 0; // 'gmail' not immediately after '@'
    }

    char *dot_pos = strstr(gmail_pos, ".com"); // Find '.com' after 'gmail'
    if (dot_pos == NULL || dot_pos != gmail_pos + 5)
    {
        return 0; // '.com' not immediately after 'gmail'
    }

    return 1; // Valid email format
}

// Function definition to find whether the user enter input or not

int input(char string[])
{
    if (strlen(string) == 0)
    {
        printf("\n\033[91mError: Input cannot be empty! \033[0m");
        return 1;
    }
    else
    {
        return 0;
    }
}