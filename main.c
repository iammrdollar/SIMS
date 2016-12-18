#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/linux_conio.h"

#define FILENAME "data/lpustud.bin"
#define FILENAME_TEMP "data/lpustud_temp.bin"

#define HEADING printf("\t\t SIMS (Student Information Management System)\t\t");printf("\n\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\t\t");

#define FILE_ERROR "Error Opening File !!"

void clearScreen();
void auth(char [], char []);
void insertData();
void display();
void search();
void delete();
void update();
void sort();

typedef struct {
  char name[100];
  char section[10];
  char group;
  int rollNo;
  long regNo;
  int marks;
} STUDENT;

int main()
{
 auth("Login Here","Login Successful !");

//Creating File At Start
  FILE *file;
  if ( (file = fopen(FILENAME,"rb")) == NULL ) {
    fclose(file);
    file = fopen(FILENAME,"wb");
    fclose(file);
  }
  else
  {
    fclose(file);
  }

  int choice;
  while (choice != 7) {
    clearScreen();
    HEADING;
    printf("\n\n\t\t\t 1. INSERT");
    printf("\n\t\t\t 2. DISPLAY");
    printf("\n\t\t\t 3. SEARCH");
    printf("\n\t\t\t 4. DELETE");
    printf("\n\t\t\t 5. UPDATE");
    printf("\n\t\t\t 6. SORT");
    printf("\n\t\t\t 7. EXIT");
    printf("\n\n\n\t\t Enter Choice : ");
    scanf(" %d",&choice);
    getch();
    switch (choice) {
      case 1 :  insertData();
                break;

      case 2 :  display();
                break;

      case 3 :  search();
                break;

      case 4 :  delete();
                break;

      case 5 :  update();
                break;

      case 6 :  sort();
                break;

      case 7 :  clearScreen();
                HEADING;
                printf("\n\n\n\n\t\tThanks For Using SIMS. Have A Great Day!\n");
                getch();
                clearScreen();
                exit(0);
      default : printf("\n\t\t Incorrect Choice ! Please Use Choice 1-7");
                getch();
                break;
    }
  }

  getch();
  return 0;
}

/*
** Clear Screen For Both *nix & Windows
*/
void clearScreen()
{
  system("cls");
  system("clear");
}

/*
** Authentication Function
** Allows 3 Attempts
** USERNAME defined as a preprocessor
** PASSPHRASE defined as a preprocessor
*/
#define USERNAME "username"
#define PASSPHRASE "password"

void auth(char message[],char output_message[])
{
  int attempt=4;

  while (attempt>0) {
    clearScreen();
    char username[20];
    char passphrase[20];
    char ch; // temp character
    int i; // iterator

    HEADING;

    printf("\n\n\n\t\t\t%s",message);

    printf("\n\n\t\t\tEnter Username : ");
    fflush(stdin);
    scanf("%s", username);

    printf("\n\t\t\tEnter Password : ");
    getch();
    for (i = 0; (ch=getch())!='\n'; i++) {
      printf("*");
      passphrase[i] = ch;
    }
    passphrase[i] = '\0';

    if ( (strcmp(username,USERNAME) == 0) && (strcmp(passphrase,PASSPHRASE) == 0) ){
      printf("\n\t\t\t%s",output_message);
      printf("\n\n\t\t\tPress Any Key To Continue ");
      getch();
      return;
    }
    else
    {
      attempt--;
      printf("\n\t\t\tIncorrect Password !!");

      if (attempt>0) {
        printf("\n\t\t\t%d Attempts Left ",attempt);
      }
      else
      {
        printf("\n\t\t\t Exiting......\n");
      }

      getch();
    }
  }
  exit(0);
}

/*
** Student Data Insertion
*/
void insertData()
{
  STUDENT lpuStud;
  STUDENT lpuStud_exists;
  FILE *file;
  int exists = 0; // 0 - Doesn't Exist ; 1 - Exist

  int continueStatus = 1;
  while(continueStatus)
  {
    clearScreen();
    HEADING;
    printf("\n\n\t\t\t INSERT STUDENT's DATA\t\t");

    printf(" \n\n\t\tStudent's Reg.No : ");
    scanf("%ld", &lpuStud.regNo);

    //Check If Reg.No. Already Exist
    if ( (file = fopen(FILENAME, "rb")) == NULL ) {
      printf("\n\n\t\t%s",FILE_ERROR);
      exit(0);
    }

    fread(&lpuStud_exists, sizeof(lpuStud_exists), 1, file);
    while (!feof(file)) {
      if (lpuStud_exists.regNo == lpuStud.regNo) {
        exists = 1;
        break;
      }
      fread(&lpuStud_exists, sizeof(lpuStud_exists), 1, file);
    }
    fclose(file);

    if (exists) {
      printf("\n\n\t\tRegistration Number Already Exist !!");
      printf("\n\n\t\tPress Any Key To Go To Main Menu");
      continueStatus = 0;
      getch();getch();
    }
    else
    {
      printf(" \n\t\tStudent's Name : ");
      fflush(stdin);
      scanf(" %[^\n]s", lpuStud.name);

      printf(" \n\t\tStudent's Section : ");
      fflush(stdin);
      scanf(" %[^\n]s", lpuStud.section);

      printf(" \n\t\tStudent's Group (A/B): ");
      fflush(stdin);
      scanf(" %c", &lpuStud.group);

      printf(" \n\t\tStudent's Roll.No : ");
      scanf("%d", &lpuStud.rollNo);

      printf(" \n\t\tStudent's Total Marks : ");
      scanf("%d", &lpuStud.marks);

      //Saving Data
      if ( (file = fopen(FILENAME, "ab")) == NULL ) {
        printf("\n\n\t\t%s",FILE_ERROR);
        exit(0);
      }
      fwrite(&lpuStud, sizeof(lpuStud), 1, file);
      fclose(file);

      printf("\n\n\t\t\tSaved Successfully !!");
      printf(" \n\n\t\tDo You Want To Add More (Y/N) or (y/n) : ");
      char yesOrNo;
      scanf(" %c", &yesOrNo);
      if (!( (yesOrNo == 'Y') || (yesOrNo == 'y') )) {
        continueStatus = 0;
      }
    }

  }
}

/*
** Student Data Display in Tabular Form
*/

void display()
{
  int counter=0;
  STUDENT lpuStud;
  FILE *file;

  if ( (file = fopen(FILENAME,"rb")) == NULL ) {
    printf("\n\n\t\t%s",FILE_ERROR);
    exit(0);
  }

  fread(&lpuStud, sizeof(lpuStud), 1, file);

  while(!feof(file)) {
    // Checking Counter To Clear The Page & Print The Heading Materials
    if (counter == 0) {
      clearScreen();
      HEADING;
      printf("\n\n\n%-23s %-10s %-9s %-12s %-12s %-8s\n","STUDENT NAME","SECTION","GROUP","ROLL.NO.","REG.NO.","TOTAL MARKS");
    }

    //Printing Actual Data
    printf("\n%-23s %-10s %-9c %-12d %-12ld %-8d", lpuStud.name, lpuStud.section, lpuStud.group, lpuStud.rollNo, lpuStud.regNo, lpuStud.marks);
    fread(&lpuStud, sizeof(lpuStud), 1, file);

    ++counter;

    //Checking If Results Are Greater Than 20
    if (counter > 20) {
      counter = 0;
      printf("\n\n\t\tPress Enter To See More Results !");
      getch();
    }

  }

  printf("\n\n\t\tNo More Results. Press Any Key To Go To The Main Menu");
  getch();
  fclose(file);
}

/*
** Search A Particular Student Details According To Registration ID
*/
void search()
{
  clearScreen();
  long searchTerm;
  int found=0; // 0 - Not Found ; 1 - Found

  STUDENT lpuStud;
  FILE *file;

  if ( (file = fopen(FILENAME,"rb")) == NULL ) {
    printf("\n\n\t\t%s",FILE_ERROR);
    exit(0);
  }

  HEADING;
  printf("\n\n\t\tEnter The Reg.No To Search : ");
  scanf("%ld",&searchTerm);
  getch();

  fread(&lpuStud, sizeof(lpuStud), 1, file);

  while (!feof(file)) {
    if (lpuStud.regNo == searchTerm) {
      found = 1;
      printf("\n\n%-23s %-10s %-9s %-12s %-12s %-8s\n","STUDENT NAME","SECTION","GROUP","ROLL.NO.","REG.NO.","TOTAL MARKS");
      printf("\n%-23s %-10s %-9c %-12d %-12ld %-8d", lpuStud.name, lpuStud.section, lpuStud.group, lpuStud.rollNo, lpuStud.regNo, lpuStud.marks);
      printf("\n\n\t\tPress Any Key To Go To The Main Menu");
      getch();
      break;
    }
    fread(&lpuStud, sizeof(lpuStud), 1, file);
  }

  if(!found) {
    printf("\n\t\tNo Student Found !");
    printf("\n\n\t\tPress Any Key To Go To The Main Menu");
    getch();
  }

  fclose(file);
}

/*
** Delete a student record by copying from one file to another without
** including the deleted record
*/
void delete()
{
  clearScreen();
  long searchTerm;
  int deleted=0; // 0 - Not Found ; 1 - Found

  STUDENT lpuStud;
  FILE *file, *temp_file;

  if ( (file = fopen(FILENAME,"rb")) == NULL ) {
    printf("\n\n\t\t%s",FILE_ERROR);
    exit(0);
  }

  if ( (temp_file = fopen(FILENAME_TEMP,"wb")) == NULL ) {
    printf("\n\n\t\t%s",FILE_ERROR);
    exit(0);
  }

  HEADING;
  printf("\n\n\t\tEnter The Reg.No To Delete  : ");
  scanf("%ld",&searchTerm);
  getch();

  fread(&lpuStud, sizeof(lpuStud), 1, file);

  while (!feof(file)) {
    if (lpuStud.regNo == searchTerm) {
      printf("\n\n%-23s %-10s %-9s %-12s %-12s %-8s\n","STUDENT NAME","SECTION","GROUP","ROLL.NO.","REG.NO.","TOTAL MARKS");
      printf("\n%-23s %-10s %-9c %-12d %-12ld %-8d", lpuStud.name, lpuStud.section, lpuStud.group, lpuStud.rollNo, lpuStud.regNo, lpuStud.marks);

      printf("\n\n\t\tDo You Really Want To Delete (Y/N) or (y/n) : ");
      char yesOrNo;
      scanf(" %c", &yesOrNo);
      getch();
      if (!( (yesOrNo == 'Y') || (yesOrNo == 'y') )) {
        fwrite(&lpuStud, sizeof(lpuStud), 1, temp_file);
        printf("\n\t\tDeletion Cancelled!");
        printf("\n\n\t\tPress Any Key To Go To The Main Menu");
        getch();
        deleted = 2;
      }
      else
      {
        auth("Enter Your Credentials To Confirm","Authenticated !");
        deleted = 1;
      }
    }
    else
    {
      fwrite(&lpuStud, sizeof(lpuStud), 1, temp_file);
    }
    fread(&lpuStud, sizeof(lpuStud), 1, file);
  }

  if(!deleted) {
    printf("\n\t\tNo Student Record Found For Deletion!");
    printf("\n\n\t\tPress Any Key To Go To The Main Menu");
    getch();
  }
  else if(deleted == 1)
  {
    printf("\n\n\n\t\t\tStudent Record Deleted Successfully");
    printf("\n\n\t\t\tPress Any Key To Go To The Main Menu");
    getch();
  }

  remove(FILENAME);
	rename(FILENAME_TEMP, FILENAME);

  fclose(file);
  fclose(temp_file);
}

/*
** UPDATE a student record
*/
void update()
{
  clearScreen();
  long searchTerm;
  int updated=0; // 0 - Not Found ; 1 - Found & Updated

  STUDENT lpuStud;
  STUDENT lpuStud_update_data;
  FILE *file, *temp_file;

  if ( (file = fopen(FILENAME,"rb")) == NULL ) {
    printf("\n\n\t\t%s",FILE_ERROR);
    exit(0);
  }

  if ( (temp_file = fopen(FILENAME_TEMP,"wb")) == NULL ) {
    printf("\n\n\t\t%s",FILE_ERROR);
    exit(0);
  }

  HEADING;
  printf("\n\n\t\tEnter The Reg.No To Update  : ");
  scanf("%ld",&searchTerm);
  getch();

  fread(&lpuStud, sizeof(lpuStud), 1, file);

  while (!feof(file)) {
    if (lpuStud.regNo == searchTerm) {
      //Print The Data
      printf("\n\n%-23s %-10s %-9s %-12s %-12s %-8s\n","STUDENT NAME","SECTION","GROUP","ROLL.NO.","REG.NO.","TOTAL MARKS");
      printf("\n%-23s %-10s %-9c %-12d %-12ld %-8d", lpuStud.name, lpuStud.section, lpuStud.group, lpuStud.rollNo, lpuStud.regNo, lpuStud.marks);

      //Copy Original Data To Temp Structure
      lpuStud_update_data = lpuStud;
      updated = 1;

      //Ask What To Update
      printf("\n\n\t\tWhat Do You Want To Update ?");
      int choice;
      printf("\n\t\t 1. Name");
      printf("\n\t\t 2. Section");
      printf("\n\t\t 3. Group");
      printf("\n\t\t 4. Roll.No");
      printf("\n\t\t 5. Marks");
      printf("\n\t\t 6. None");
      printf("\n\n\t\tChoice : ");
      scanf("%d", &choice);
      getch();

      switch (choice) {
        case 1 :  printf("\n\t\tEnter New Name : ");
                  scanf("%[^\n]s", lpuStud_update_data.name);
                  break;

        case 2 :  printf("\n\t\tEnter New Section : ");
                  scanf("%[^\n]s", lpuStud_update_data.section);
                  break;

        case 3 :  printf("\n\t\tEnter New Group (A/B) : ");
                  scanf("%c", &lpuStud_update_data.group);
                  break;

        case 4 :  printf("\n\t\tEnter New Roll.No : ");
                  scanf("%d", &lpuStud_update_data.rollNo);
                  break;

        case 5 :  printf("\n\t\tEnter New Marks : ");
                  scanf("%d", &lpuStud_update_data.marks);
                  break;

        case 6 :  printf("\n\n\t\tNothing Changed !!");
                  printf("\n\n\t\tPress Any Key To Go To The Main Menu");
                  updated = 2;
                  getch();
                  break;

        default : printf("\n\n\t\tInvalid Choice");
                  printf("\n\n\t\tPress Any Key To Go To The Main Menu");
                  getch();
                  updated = 2;
                  break;
      }
      fwrite(&lpuStud_update_data, sizeof(lpuStud_update_data), 1, temp_file);
    }
    else
    {
      fwrite(&lpuStud, sizeof(lpuStud), 1, temp_file);
    }

    fread(&lpuStud, sizeof(lpuStud), 1, file);
  }

  if(updated==0) {
    printf("\n\t\tNo Student Record Found!");
    printf("\n\n\t\tPress Any Key To Go To The Main Menu");
    getch();
  } else
  if(updated == 1)
  {
    printf("\n\t\tStudent Record Updated Successfully");
    printf("\n\n\t\tPress Any Key To Go To The Main Menu");
    getch();getch();
  }

  remove(FILENAME);
	rename(FILENAME_TEMP, FILENAME);

  fclose(file);
  fclose(temp_file);
}

void sort()
{
  int i,j; //iterators
  clearScreen();
  HEADING;
  int total_results=0;
  int sorting_criteria;
  int sorting_status=0; // 0 - Not Sorted ; 1 - Sorted ; >2 - Skip

  STUDENT lpuStud;
  STUDENT temp;

  FILE *file,*temp_file;

  // Getting total Number of records
  if ( (file = fopen(FILENAME,"rb")) == NULL ) {
    printf("\n\n\t\t%s",FILE_ERROR);
    exit(0);
  }

  fread(&lpuStud, sizeof(lpuStud), 1, file);
  while (!feof(file)) {
    ++total_results;
    fread(&lpuStud, sizeof(lpuStud), 1, file);
  }
  fclose(file);

  //If results found then sort otherwise not
  if (!total_results) {
    printf("\n\t\tNo Student Record Found!");
    printf("\n\n\t\tPress Any Key To Go To The Main Menu");
    getch();
  }
  else
  {
    //Store Total Data in Temp Struct Array
    STUDENT lpuStud_sorting[total_results];

    if ( (file = fopen(FILENAME,"rb")) == NULL ) {
      printf("\n\n\t\t%s",FILE_ERROR);
      exit(0);
    }
    fread(&lpuStud_sorting, sizeof(lpuStud_sorting), total_results, file);
    fclose(file);

    //Getting Criteria For Sorting
    printf("\n\n\t\tEnter The Criteria For Sorting ");
    printf("\n\t\t1. Registration Number");
    printf("\n\t\t2. Marks");
    printf("\n\t\t3. None");
    printf("\n\n\t\tChoice : ");
    scanf("%d", &sorting_criteria);

    switch(sorting_criteria)
    {
      case 1:
              for(i=0; i<(total_results-1); i++)
              {
                for (j=0; j<(total_results - i - 1); j++) {
                  if (lpuStud_sorting[j].regNo > lpuStud_sorting[j+1].regNo) {
                    temp = lpuStud_sorting[j];
                    lpuStud_sorting[j] = lpuStud_sorting[j+1];
                    lpuStud_sorting[j+1] = temp;
                  }
                }
              }
              sorting_status = 1;
              break;

      case 2:
              for(i=0; i<(total_results-1); i++)
              {
                for (j=0; j<(total_results - i - 1); j++) {
                  if (lpuStud_sorting[j].marks > lpuStud_sorting[j+1].marks) {
                    temp = lpuStud_sorting[j];
                    lpuStud_sorting[j] = lpuStud_sorting[j+1];
                    lpuStud_sorting[j+1] = temp;
                  }
                }
              }
              sorting_status = 1;
              break;

      case 3: printf("\n\n\t\tNothing Sorted !!");
              printf("\n\n\t\tPress Any Key To Go To The Main Menu");
              sorting_status = 2;
              getch();getch();
              break;

      default:printf("\n\n\t\tInvalid Choice");
              printf("\n\n\t\tPress Any Key To Go To The Main Menu");
              getch();getch();
              sorting_status = 2;
              break;
    }

    if(sorting_status == 1)
    {
      //Writing Again to a temp file
      if ( (temp_file = fopen(FILENAME_TEMP,"wb")) == NULL ) {
        printf("\n\n\t\t%s",FILE_ERROR);
        exit(0);
      }

      fwrite(&lpuStud_sorting, sizeof(lpuStud_sorting), 1, temp_file);
      fclose(temp_file);

      remove(FILENAME);
    	rename(FILENAME_TEMP, FILENAME);

      printf("\n\t\tStudent Record Updated Sorted !");
      printf("\n\n\t\tPress Any Key To Go To The Main Menu");
      getch();getch();
    }
  }
}
