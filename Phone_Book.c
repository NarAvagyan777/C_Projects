#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

typedef struct {
	char Name[50];
	char Phone_Number[15]; 
	char Email[50];
	char Address[100]; 
}phonebook;

int Duplicate_Detaction(const char* phone_number) {
	FILE *file = fopen("phonebook.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }
  	char name[50], phone_number_in_file[15], email[50], address[100];

	while (fgets(name, sizeof(name), file)) {
        fgets(phone_number_in_file, sizeof(phone_number_in_file), file);
        fgets(email, sizeof(email), file);
        fgets(address, sizeof(address), file);

        name[strlen(name) - 1] = '\0';
        phone_number_in_file[strlen(phone_number_in_file) - 1] = '\0';
        email[strlen(email) - 1] = '\0';
        address[strlen(address) - 1] = '\0';

        if (strncmp(phone_number_in_file, phone_number, 15) == 0) {
            fclose(file);
            return 1;  
        }
    }
	fclose(file);
	return 0;
}

void add_contact(phonebook* Book) {

	FILE *file = fopen("phonebook.txt", "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

	printf("How many contact you want\n");
	int num = 0;
	scanf("%d",&num);
	getchar();
	for (int i = 0; i < num; ++i) {
		printf("Enter the Name\n");
		fgets(Book -> Name, 50, stdin);
		Book -> Name[strlen(Book -> Name) - 1] = '\0';

		printf("Enter the Phone Number\n");
		fgets(Book -> Phone_Number, 15, stdin);
		Book -> Phone_Number[strlen(Book -> Phone_Number) - 1] = '\0';

		if (Duplicate_Detaction(Book->Phone_Number)) {
            printf("<<< A contact with phone number %s already exists! >>>\n", Book->Phone_Number);
            continue;
        }

		printf("Enter the Email\n");
		fgets(Book -> Email, 50, stdin);
		Book -> Email[strlen(Book -> Email) - 1] = '\0';
	
		printf("Enter the Address\n");
		fgets(Book -> Address, 50, stdin);
		Book -> Address[strlen(Book -> Address) - 1] = '\0';

		fprintf(file,"%s\n%s\n%s\n%s\n", Book -> Name, Book -> Phone_Number, Book -> Email, Book -> Address);
	}
	fclose(file);
}



void View_all_contact(phonebook* Book) {
	FILE *file = fopen("phonebook.txt", "r");
	    if (file == NULL) {
		    printf("Error opening file.\n");
			 exit(1);
    }
	while (fgets(Book->Name, sizeof(Book->Name), file) != NULL) {
        Book->Name[strlen(Book->Name) - 1] = '\0'; 

        if (fgets(Book->Phone_Number, sizeof(Book->Phone_Number), file) != NULL) {
            Book->Phone_Number[strlen(Book->Phone_Number) - 1] = '\0';
        }

        if (fgets(Book->Email, sizeof(Book->Email), file) != NULL) {
            Book->Email[strlen(Book->Email) - 1] = '\0';
        }

        if (fgets(Book->Address, sizeof(Book->Address), file) != NULL) {
            Book->Address[strlen(Book->Address) - 1] = '\0';
        }

        printf("%s\n%s\n%s\n%s\n", Book->Name, Book->Phone_Number, Book->Email, Book->Address);
        putchar(10);  
    }

	fclose(file);
	
}

int read_contacts(phonebook* Book, phonebook* contacts[]) {
    FILE *file = fopen("phonebook.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    int count = 0;
    while (fgets(Book->Name, sizeof(Book->Name), file) != NULL) {
        Book->Name[strlen(Book->Name) - 1] = '\0';  

        if (fgets(Book->Phone_Number, sizeof(Book->Phone_Number), file) != NULL) {
            Book->Phone_Number[strlen(Book->Phone_Number) - 1] = '\0';
        }

        if (fgets(Book->Email, sizeof(Book->Email), file) != NULL) {
            Book->Email[strlen(Book->Email) - 1] = '\0';
        }

        if (fgets(Book->Address, sizeof(Book->Address), file) != NULL) {
            Book->Address[strlen(Book->Address) - 1] = '\0';
        }

        contacts[count] = (phonebook*)malloc(sizeof(phonebook));
        strcpy(contacts[count]->Name, Book->Name);
        strcpy(contacts[count]->Phone_Number, Book->Phone_Number);
        strcpy(contacts[count]->Email, Book->Email);
        strcpy(contacts[count]->Address, Book->Address);
        count++;
    }

    fclose(file);
    return count;
}

void sort_contacts_by_name(phonebook* contacts[], int count) {
    phonebook* temp;
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(contacts[i]->Name, contacts[j]->Name) > 0) {
                temp = contacts[i];
                contacts[i] = contacts[j];
                contacts[j] = temp;
            }
        }
    }
}

void save_sorted_contacts(phonebook* contacts[], int count) {
    FILE *file = fopen("phonebook.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s\n%s\n%s\n%s\n", contacts[i]->Name, contacts[i]->Phone_Number, contacts[i]->Email, contacts[i]->Address);
    }

    fclose(file);
}

void Search_Contact(phonebook* Book) {

	char Search[50];
	int tox;

	printf("Search by:\n");
    printf("1. Name\n");
    printf("2. Phone\n");
    printf("Enter your line (1 or 2): ");
    scanf("%d", &tox);

	getchar();

	printf("Enter the searching data\n");
	fgets(Search,50,stdin);
	Search[strlen(Search) -1] = '\0';


	FILE *file = fopen("phonebook.txt", "r");
	    if (file == NULL) {
		    printf("Error opening file.\n");
			exit(1);
		}
	int flag = 0;
	 while (fgets(Book->Name, sizeof(Book->Name), file) != NULL) {
        fgets(Book->Phone_Number, sizeof(Book->Phone_Number), file);
        fgets(Book->Email, sizeof(Book->Email), file);
        fgets(Book->Address, sizeof(Book->Address), file);

        Book->Name[strlen(Book->Name) - 1] = '\0';
        Book->Phone_Number[strlen(Book->Phone_Number) - 1] = '\0';
        Book->Email[strlen(Book->Email) - 1] = '\0';
        Book->Address[strlen(Book->Address) - 1] = '\0';

        if ((tox == 1 && strcmp(Book->Name, Search) == 0) || 
            (tox == 2 && strcmp(Book->Phone_Number, Search) == 0)) {
            printf("Contact found:\n");
            printf("Name: %s\n", Book->Name);
            printf("Phone: %s\n", Book->Phone_Number);
            printf("Email: %s\n", Book->Email);
            printf("Address: %s\n", Book->Address);
            flag = 1;
            break;
        }
    }
	if (!flag) {
		printf("Contact not founded\n");
	}
	fclose(file);

}

void Advanced_Search(phonebook* Book) {
	char Search[50];
	printf("Enter the Advanced contact data\n");
	fgets(Search, sizeof(Search), stdin);
	Search[strlen(Search) -1] = '\0';

	FILE *file = fopen("phonebook.txt", "r");
	    if (file == NULL) {
		    printf("Error opening file.\n");
			exit(1);
		}
	
	int flag = 0;

	while (fgets(Book->Name, sizeof(Book->Name), file) != NULL) {
        Book->Name[strlen(Book->Name) - 1] = '\0';

        if (fgets(Book->Phone_Number, sizeof(Book->Phone_Number), file) != NULL) {
            Book->Phone_Number[strlen(Book->Phone_Number) - 1] = '\0';
        }

        if (fgets(Book->Email, sizeof(Book->Email), file) != NULL) {
            Book->Email[strlen(Book->Email) - 1] = '\0';
        }

        if (fgets(Book->Address, sizeof(Book->Address), file) != NULL) {
            Book->Address[strlen(Book->Address) - 1] = '\0';
        }
		if (strstr(Book -> Name, Search) != NULL || strstr(Book -> Phone_Number, Search) != NULL 
			|| strstr(Book -> Email, Search) != NULL || strstr(Book -> Address, Search) != NULL) {
				printf("Found contact:\n");
				printf("Name: %s\n", Book->Name);
				printf("Phone: %s\n", Book->Phone_Number);
				printf("Email: %s\n", Book->Email);
				printf("Address: %s\n", Book->Address);
				flag = 1;
		}			
	}
	if (!flag) {
		printf("Contact not found\n");
	}
	fclose(file);		
}

void Delete_Contact(phonebook* Book) {
	
	char Search[50];
    int tox;

    printf("Delete by:\n");
    printf("1. Name\n");
    printf("2. Phone\n");
    printf("Enter your line  (1 or 2): ");
    scanf("%d", &tox);
	getchar();

	printf("Enter the data for deletion\n");
    fgets(Search, 50, stdin);
    Search[strlen(Search) - 1] = '\0'; 

	FILE *file = fopen("phonebook.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

	FILE *temp_file = fopen("temp_phonebook.txt", "w");
    if (temp_file == NULL) {
        printf("Error opening temporary file.\n");
        fclose(file);
        exit(1);
    }

	int flag = 0;

	while (fgets(Book->Name, sizeof(Book->Name), file) != NULL) {
        Book->Name[strlen(Book->Name) - 1] = '\0';
        if (fgets(Book->Phone_Number, sizeof(Book->Phone_Number), file) != NULL) {
            Book->Phone_Number[strlen(Book->Phone_Number) - 1] = '\0';
        }

        if (fgets(Book->Email, sizeof(Book->Email), file) != NULL) {
            Book->Email[strlen(Book->Email) - 1] = '\0';
        }

        if (fgets(Book->Address, sizeof(Book->Address), file) != NULL) {
            Book->Address[strlen(Book->Address) - 1] = '\0';
        }

        if ((tox == 1 && strcmp(Book->Name, Search) == 0) || (tox == 2 && strcmp(Book->Phone_Number, Search) == 0)) {
            printf("Found contact:\n");
            printf("Name: %s\n", Book->Name);
            printf("Phone: %s\n", Book->Phone_Number);
            printf("Email: %s\n", Book->Email);
            printf("Address: %s\n", Book->Address);
            flag = 1;
        } else {
            fprintf(temp_file, "%s\n%s\n%s\n%s\n", Book->Name, Book->Phone_Number, Book->Email, Book->Address);
        }
    }
	if (!flag) {
        printf("Dont have that contact for delate\n");
    }
	fclose(file);
	fclose(temp_file);

	remove("phonebook.txt");
    rename("temp_phonebook.txt", "phonebook.txt");

}

void Update_Contact(phonebook* Book) {
	char Search[50];
	int tox = 2;
	
	printf("Change by:\n");
    printf(" Phone\n");


	printf("Enter the data for changing\n");
    fgets(Search, 50, stdin);
    Search[strlen(Search) - 1] = '\0'; 

	FILE *file = fopen("phonebook.txt", "r");
	    if (file == NULL) {
		    printf("Error opening file.\n");
			exit(1);
		}

	FILE *temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        printf("Error opening temporary file.\n");
        fclose(file);
        exit(1);
    }

	int flag = 0;

	while (fgets(Book->Name, sizeof(Book->Name), file) != NULL) {
        Book->Name[strlen(Book->Name) - 1] = '\0';
        fgets(Book->Phone_Number, sizeof(Book->Phone_Number), file);
        Book->Phone_Number[strlen(Book->Phone_Number) - 1] = '\0';
        fgets(Book->Email, sizeof(Book->Email), file);
        Book->Email[strlen(Book->Email) - 1] = '\0';
        fgets(Book->Address, sizeof(Book->Address), file);
        Book->Address[strlen(Book->Address) - 1] = '\0';
		if (tox == 2 && strcmp(Book->Phone_Number, Search) == 0) {
			printf("Found Contact: \n");
			printf("Enter the Name\n");
			fgets(Book -> Name, 50, stdin);
			Book -> Name[strlen(Book -> Name) - 1] = '\0';

			printf("Enter the Phone Number\n");
			fgets(Book -> Phone_Number, 15, stdin);
			Book -> Phone_Number[strlen(Book -> Phone_Number) - 1] = '\0';

			printf("Enter the Email\n");
			fgets(Book -> Email, 50, stdin);
			Book -> Email[strlen(Book -> Email) - 1] = '\0';
	
			printf("Enter the Address\n");
			fgets(Book -> Address, 50, stdin);
			Book -> Address[strlen(Book -> Address) - 1] = '\0';

			fprintf(temp_file,"%s\n %s\n %s\n %s\n", Book -> Name, Book -> Phone_Number, Book -> Email, Book -> Address);
			flag = 1;
		} else {
			fprintf(temp_file,"%s\n%s\n%s\n%s\n", Book -> Name, Book -> Phone_Number, Book -> Email, Book -> Address);
		}
	}
		if (!flag) {
			printf("Dont have that contact for changing\n");
		}
		fclose(file);
		fclose(temp_file);

		if (remove("phonebook.txt") != 0) {
        printf("Error deleting the old phonebook file.\n");
        exit(1);
    }

    if (rename("temp.txt", "phonebook.txt") != 0) {
        printf("Error renaming the temporary file.\n");
        exit(1);
    }
}

void Backup_Phonebook(phonebook* Book) {
    FILE* file = fopen("phonebook.txt", "r");
    if (file == NULL) {
        printf("Error opening the source phonebook file.\n");
        return;
    }

    FILE *backupFile = fopen("phonebook_backup.txt", "w");
    if (backupFile == NULL) {
        printf("Error opening the backup phonebook file.\n");
        fclose(file);
        return;
    }

    char ch;
    
    while ((ch = fgetc(file)) != EOF) {
        fputc(ch, backupFile);
    }

    printf("Backup created successfully.\n");

    fclose(file);
    fclose(backupFile);
}




int main() {

	phonebook *Book = (phonebook *)malloc(sizeof(phonebook));
    if (Book == NULL) {
        printf("Memory failed.\n");
        return 1;
    }
while (1) {    
    char choice[10];
        

        printf("\nWhat do you want to do?\n");
        printf("1. Add Contact\n");
        printf("2. View All Contacts\n");
        printf("3. Search Contact\n");
        printf("4. Advanced Search\n");
        printf("5. Delete Contact\n");
        printf("6. Update Contact\n");
        printf("7. Backup Phonebook\n");
        printf("Type 'exit' to quit.\n");

        fgets(choice, sizeof(choice), stdin);
        choice[strlen(choice) - 1] = '\0'; 

        if (strcmp(choice, "exit") == 0) {
            break; 
        }

        if (strcmp(choice, "1") == 0) {
            add_contact(Book);
        } else if (strcmp(choice, "2") == 0) {
            View_all_contact(Book);
        } else if (strcmp(choice, "3") == 0) {
            Search_Contact(Book);
        } else if (strcmp(choice, "4") == 0) {
            Advanced_Search(Book);
        } else if (strcmp(choice, "5") == 0) {
            Delete_Contact(Book);
        } else if (strcmp(choice, "6") == 0) {
            Update_Contact(Book);
        } else if (strcmp(choice, "7") == 0) {
            Backup_Phonebook(Book);
        } else {
            printf("Invalid choice, please try again.\n");
        }

        
        phonebook* contacts[100];  
        int count = read_contacts(Book, contacts);  
        if (count > 0) {
            sort_contacts_by_name(contacts, count);  
            save_sorted_contacts(contacts, count); 
        }

    }
	free(Book);
	return 0;

}