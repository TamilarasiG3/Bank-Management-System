#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

struct date
{
    int day;
    int month;
    int year;
};

struct account
{
    int acc_no;
    char name[100];
    int age;
    char address[100];
    char citizenship[50];
    char acc_type[20];
    char phone[20];
    float balance;

    struct date dob;
};

struct account add;

void menu();
void create_account();
void update_account();
void transaction();
void check_account();
void delete_account();
void view_customers();
void exit_program();

void clear_input();
int valid_int();
float valid_float();
void valid_string(char str[]);
int valid_date(int d, int m, int y);
int valid_phone(char phone[]);

int main()
{
    char password[20];
    char original_password[] = "admin";

    printf("\n======================================");
    printf("\n      BANK MANAGEMENT SYSTEM");
    printf("\n======================================");

    printf("\n\nEnter Password: ");
    scanf("%s", password);

    if(strcmp(password, original_password) == 0)
    {
        printf("\nLogin Successful!\n");
        menu();
    }
    else
    {
        printf("\nWrong Password!\n");
    }

    return 0;
}

void clear_input()
{
    while(getchar() != '\n');
}

int valid_int()
{
    int x;
    char ch;

    while(1)
    {
        if(scanf("%d", &x) == 1)
        {
            while((ch = getchar()) != '\n')
            {
                if(!isspace(ch))
                {
                    printf("Invalid input! Enter numbers only: ");
                    break;
                }
            }

            if(ch == '\n')
            {
                return x;
            }
        }
        else
        {
            printf("Invalid input! Enter numbers only: ");

            while(getchar() != '\n');
        }
    }
}

float valid_float()
{
    float x;
    char ch;

    while(1)
    {
        if(scanf("%f", &x) == 1)
        {
            while((ch = getchar()) != '\n')
            {
                if(!isspace(ch))
                {
                    printf("Invalid amount! Enter numbers only: ");
                    break;
                }
            }

            if(ch == '\n')
            {
                return x;
            }
        }
        else
        {
            printf("Invalid amount! Enter numbers only: ");

            while(getchar() != '\n');
        }
    }
}

void valid_string(char str[])
{
    scanf(" %[^\n]", str);
}

int valid_date(int d, int m, int y)
{
    if(y < 1900 || y > 2100)
        return 0;

    if(m < 1 || m > 12)
        return 0;

    if(d < 1 || d > 31)
        return 0;

    return 1;
}

int valid_phone(char phone[])
{
    int i;

    if(strlen(phone) != 10)
        return 0;

    for(i = 0; phone[i] != '\0'; i++)
    {
        if(!isdigit(phone[i]))
            return 0;
    }

    return 1;
}

void menu()
{
    int choice;

    while(1)
    {
        printf("\n\n======================================");
        printf("\n             MAIN MENU");
        printf("\n======================================");

        printf("\n1. Create New Account");
        printf("\n2. Update Account");
        printf("\n3. Transaction");
        printf("\n4. Check Account");
        printf("\n5. Delete Account");
        printf("\n6. View Customers");
        printf("\n7. Exit");

        printf("\n\nEnter your choice: ");

        choice = valid_int();

        switch(choice)
        {
            case 1:
                create_account();
                break;

            case 2:
                update_account();
                break;

            case 3:
                transaction();
                break;

            case 4:
                check_account();
                break;

            case 5:
                delete_account();
                break;

            case 6:
                view_customers();
                break;

            case 7:
                exit_program();
                break;

            default:
                printf("\nInvalid Choice!\n");
        }
    }
}

void create_account()
{
    FILE *fp;
    struct account temp;

    int found = 0;
    int new_acc_no;

    fp = fopen("record.dat", "ab+");

    if(fp == NULL)
    {
        printf("\nFile Error!\n");
        return;
    }

    printf("\n\n========== CREATE ACCOUNT ==========");

    printf("\nEnter Account Number: ");
    new_acc_no = valid_int();

    rewind(fp);

    while(fread(&temp, sizeof(temp), 1, fp))
    {
        if(temp.acc_no == new_acc_no)
        {
            found = 1;
            break;
        }
    }

    if(found)
    {
        printf("\nAccount Number Already Exists!\n");
        fclose(fp);
        return;
    }

    add.acc_no = new_acc_no;

    printf("Enter Name: ");
    valid_string(add.name);

    do
    {
        printf("Enter DOB (dd/mm/yyyy): ");

        scanf("%d/%d/%d",
              &add.dob.day,
              &add.dob.month,
              &add.dob.year);

        if(!valid_date(add.dob.day,
                       add.dob.month,
                       add.dob.year))
        {
            printf("Invalid Date!\n");
        }

    }while(!valid_date(add.dob.day,
                       add.dob.month,
                       add.dob.year));

    do
    {
        printf("Enter Age: ");

        add.age = valid_int();

        if(add.age < 18 || add.age > 100)
        {
            printf("Invalid Age!\n");
        }

    }while(add.age < 18 || add.age > 100);

    printf("Enter Address: ");
    valid_string(add.address);

    printf("Enter Citizenship Number: ");
    scanf("%s", add.citizenship);

    do
    {
        printf("Enter Phone Number: ");
        scanf("%s", add.phone);

        if(!valid_phone(add.phone))
        {
            printf("Invalid Phone Number!\n");
        }

    }while(!valid_phone(add.phone));

    do
    {
        printf("Enter Account Type (saving/current): ");
        scanf("%s", add.acc_type);

        if(strcmp(add.acc_type, "saving") != 0 &&
           strcmp(add.acc_type, "current") != 0)
        {
            printf("Invalid Account Type!\n");
        }

    }while(strcmp(add.acc_type, "saving") != 0 &&
           strcmp(add.acc_type, "current") != 0);

    do
    {
        printf("Enter Initial Deposit Amount: ");

        add.balance = valid_float();

        if(add.balance < 0)
        {
            printf("Amount Cannot Be Negative!\n");
        }

    }while(add.balance < 0);

    fseek(fp, 0, SEEK_END);

    fwrite(&add, sizeof(add), 1, fp);

    fclose(fp);

    printf("\n\nAccount Created Successfully!\n");
}

void update_account()
{
    FILE *fp, *temp;

    int found = 0;
    int acc_no;
    int choice;

    fp = fopen("record.dat", "rb");
    temp = fopen("temp.dat", "wb");

    if(fp == NULL || temp == NULL)
    {
        printf("\nFile Error!\n");
        return;
    }

    printf("\nEnter Account Number: ");

    acc_no = valid_int();

    while(fread(&add, sizeof(add), 1, fp))
    {
        if(add.acc_no == acc_no)
        {
            found = 1;

            printf("\n1. Update Name");
            printf("\n2. Update Address");
            printf("\n3. Update Phone");
            printf("\n4. Update Account Type");

            printf("\nEnter Choice: ");

            choice = valid_int();

            switch(choice)
            {
                case 1:
                    printf("Enter New Name: ");
                    valid_string(add.name);
                    break;

                case 2:
                    printf("Enter New Address: ");
                    valid_string(add.address);
                    break;

                case 3:

                    do
                    {
                        printf("Enter New Phone: ");
                        scanf("%s", add.phone);

                        if(!valid_phone(add.phone))
                        {
                            printf("Invalid Phone Number!\n");
                        }

                    }while(!valid_phone(add.phone));

                    break;

                case 4:

                    do
                    {
                        printf("Enter New Account Type: ");
                        scanf("%s", add.acc_type);

                        if(strcmp(add.acc_type, "saving") != 0 &&
                           strcmp(add.acc_type, "current") != 0)
                        {
                            printf("Invalid Account Type!\n");
                        }

                    }while(strcmp(add.acc_type, "saving") != 0 &&
                           strcmp(add.acc_type, "current") != 0);

                    break;

                default:
                    printf("Invalid Choice!\n");
            }
        }

        fwrite(&add, sizeof(add), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("record.dat");
    rename("temp.dat", "record.dat");

    if(found)
        printf("\nAccount Updated Successfully!\n");
    else
        printf("\nAccount Not Found!\n");
}

void transaction()
{
    FILE *fp, *temp;

    int found = 0;
    int acc_no;
    int choice;

    float amount;

    fp = fopen("record.dat", "rb");
    temp = fopen("temp.dat", "wb");

    if(fp == NULL || temp == NULL)
    {
        printf("\nFile Error!\n");
        return;
    }

    printf("\nEnter Account Number: ");

    acc_no = valid_int();

    while(fread(&add, sizeof(add), 1, fp))
    {
        if(add.acc_no == acc_no)
        {
            found = 1;

            printf("\nCurrent Balance: %.2f", add.balance);

            printf("\n1. Deposit");
            printf("\n2. Withdraw");

            printf("\nEnter Choice: ");

            choice = valid_int();

            if(choice == 1)
            {
                printf("Enter Deposit Amount: ");

                amount = valid_float();

                if(amount <= 0)
                {
                    printf("Invalid Amount!\n");
                }
                else
                {
                    add.balance += amount;

                    printf("Amount Deposited Successfully!\n");
                }
            }
            else if(choice == 2)
            {
                printf("Enter Withdraw Amount: ");

                amount = valid_float();

                if(amount <= 0)
                {
                    printf("Invalid Amount!\n");
                }
                else if(amount > add.balance)
                {
                    printf("Insufficient Balance!\n");
                }
                else
                {
                    add.balance -= amount;

                    printf("Amount Withdrawn Successfully!\n");
                }
            }
            else
            {
                printf("Invalid Choice!\n");
            }
        }

        fwrite(&add, sizeof(add), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("record.dat");
    rename("temp.dat", "record.dat");

    if(!found)
    {
        printf("\nAccount Not Found!\n");
    }
}

void check_account()
{
    FILE *fp;

    int found = 0;
    int acc_no;

    fp = fopen("record.dat", "rb");

    if(fp == NULL)
    {
        printf("\nNo Records Found!\n");
        return;
    }

    printf("\nEnter Account Number: ");

    acc_no = valid_int();

    while(fread(&add, sizeof(add), 1, fp))
    {
        if(add.acc_no == acc_no)
        {
            found = 1;

            printf("\n\n========== ACCOUNT DETAILS ==========");

            printf("\nAccount Number : %d", add.acc_no);
            printf("\nName           : %s", add.name);

            printf("\nDOB            : %02d/%02d/%04d",
                   add.dob.day,
                   add.dob.month,
                   add.dob.year);

            printf("\nAge            : %d", add.age);
            printf("\nAddress        : %s", add.address);
            printf("\nCitizenship No : %s", add.citizenship);
            printf("\nPhone Number   : %s", add.phone);
            printf("\nAccount Type   : %s", add.acc_type);
            printf("\nBalance        : %.2f", add.balance);

            break;
        }
    }

    fclose(fp);

    if(!found)
    {
        printf("\nAccount Not Found!\n");
    }
}

void delete_account()
{
    FILE *fp, *temp;

    int found = 0;
    int acc_no;

    fp = fopen("record.dat", "rb");
    temp = fopen("temp.dat", "wb");

    if(fp == NULL || temp == NULL)
    {
        printf("\nFile Error!\n");
        return;
    }

    printf("\nEnter Account Number To Delete: ");

    acc_no = valid_int();

    while(fread(&add, sizeof(add), 1, fp))
    {
        if(add.acc_no == acc_no)
        {
            found = 1;
        }
        else
        {
            fwrite(&add, sizeof(add), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("record.dat");
    rename("temp.dat", "record.dat");

    if(found)
        printf("\nAccount Deleted Successfully!\n");
    else
        printf("\nAccount Not Found!\n");
}

void view_customers()
{
    FILE *fp;

    int found = 0;

    fp = fopen("record.dat", "rb");

    if(fp == NULL)
    {
        printf("\nNo Records Found!\n");
        return;
    }

    printf("\n\n================ CUSTOMER LIST ================");

    printf("\n%-10s %-20s %-12s %-12s",
           "ACC NO",
           "NAME",
           "TYPE",
           "BALANCE");

    printf("\n--------------------------------------------------------");

    while(fread(&add, sizeof(add), 1, fp))
    {
        found = 1;

        printf("\n%-10d %-20s %-12s %-12.2f",
               add.acc_no,
               add.name,
               add.acc_type,
               add.balance);
    }

    fclose(fp);

    if(!found)
    {
        printf("\nNo Records Available!\n");
    }
}

void exit_program()
{
    printf("\n\nThank You For Using Banking System!");
    printf("\nHave A Nice Day!\n");

    exit(0);
}