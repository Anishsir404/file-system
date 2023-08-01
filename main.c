#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
// include headers as needed

enum nodeType {File, Folder}; // Enumeration type to represent the possible types of a node (file or folder)

typedef struct node{
    enum nodeType type; // Type of the node (file or folder)
    char* name; // Name of the node
    int numberOfItems; // Number of items in the folder (only applicable if the node is a folder)
    size_t size; // Size of the file (only applicable if the node is a file)
    time_t date; // Date of the last modification
    char* content; // Content of the file (only applicable if the node is a file)
    struct node* previous; // Pointer to the previous node in the list
    struct node* parent; // Pointer to the parent node
    struct node* next; // Pointer to the next node in the list
    struct node* child; // Pointer to the first child node (only applicable if the node is a folder)
}node;

char *getString(){
    size_t size = 10; // size of the buffer
    char *str; // pointer to the buffer
    int ch; // variable to store the input character
    size_t len = 0; // keeps track of the size of input string
    // Allocate memory for the strings buffer
    str = realloc(NULL, sizeof(char)*size);
    // Check if memory allocation was successful
    if(!str)return str;
    // Read input from the keyboard until a newline character is encountered
    while(EOF!=(ch=fgetc(stdin)) && ch != '\n'){
        str[len++]=ch;
        // If the size of the input string has reached the buffer size reallocate memory for the string
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0'; // Add null termination to the string

    return realloc(str, sizeof(char)*len);
}
// Function to get a node with a given name and type from a given folder
node* getNode(node *currentFolder, char* name, enum nodeType type) {
    // Check if the folder has any children
    if (currentFolder->child != NULL) {

        node *currentNode = currentFolder->child;

        while (currentNode->next != NULL) {
            // Check if the name and type of the current node matches the given name and type
            if (strcmp(name, currentNode->name) == 0 && currentNode->type == type) {
                return currentNode;
            }
            currentNode = currentNode->next; // Move to the next node
        }
        // Check the last node
        if (strcmp(name, currentNode->name) == 0 && currentNode->type == type) {
            return currentNode;
        } else return NULL;

    } else return NULL;
}
// Function to get a node with a given name from a given folder
node* getNodeTypeless(node *currentFolder, char* name) {

    if (currentFolder->child != NULL) {

        node *currentNode = currentFolder->child;
        // Check if the name of the current node matches the given name
        while (currentNode->next != NULL) {
            // Check if the name of the current node matches the name 
            if (strcmp(name, currentNode->name) == 0) {
                return currentNode; // Return the current node
            }
            currentNode = currentNode->next;// Move to the next node
        }
        // Check the last node
        if (strcmp(name, currentNode->name) == 0) {
            return currentNode;
        } else return NULL;

    } else return NULL;
}
// Function to make a new folder
void mkdir(node *currentFolder, char *command) {
    if (strtok(command, " ") != NULL) {
        // Get the name of the new folder
        char* folderName = strtok(NULL, " ");
        if (folderName != NULL) {
            // Check if the folder already exists
            if (getNodeTypeless(currentFolder, folderName) == NULL) {
                // Increase the number of items in the current folder
                currentFolder->numberOfItems++;
                node *newFolder = (node*) malloc(sizeof(node)); // allocate memory for the new folder

                if (currentFolder->child == NULL) {
                    // If the current folder has no children, make the new folder the first child
                    currentFolder->child = newFolder;
                    newFolder->previous = NULL;
                    newFolder->parent = currentFolder;
                } else {

                    node *currentNode = currentFolder->child;

                    while (currentNode->next != NULL) {
                        currentNode = currentNode->next;
                    }
                    currentNode->next = newFolder;
                    newFolder->previous = currentNode;
                    newFolder->parent = NULL;
                }
                // Allocate memory for the name of the new folder
                char* newFolderName = (char*) malloc(sizeof(char)*(strlen(folderName)+1));
                strcpy(newFolderName, folderName);
                //Assign the attributes of the new folder
                newFolder->name = newFolderName;
                newFolder->type = Folder;
                newFolder->numberOfItems = 0;
                newFolder->size = 0;
                newFolder->date = time(NULL);
                newFolder->content = NULL;
                newFolder->next = NULL;
                newFolder->child = NULL;

                printf("Folder '%s' added\n", newFolder->name);
            } else {
                fprintf(stderr, "'%s' is already exist in current directory!\n",  folderName);
            }
        }
    }
}
// Function to make a new file
void touch(node *currentFolder, char *command) {

    if (strtok(command, " ") != NULL) {
        char *fileName = strtok(NULL, " ");
        if (fileName != NULL) {
            if (getNodeTypeless(currentFolder, fileName) == NULL) {

                currentFolder->numberOfItems++;

                node *newFile = (node *) malloc(sizeof(node));

                if (currentFolder->child == NULL) {
                    currentFolder->child = newFile;
                    newFile->previous = NULL;
                    newFile->parent = currentFolder;
                } else {

                    node *currentNode = currentFolder->child;

                    while (currentNode->next != NULL) {
                        currentNode = currentNode->next;
                    }
                    currentNode->next = newFile;
                    newFile->previous = currentNode;
                    newFile->parent = NULL;
                }

                char* newFileName = (char*) malloc(sizeof(char)*(strlen(fileName)+1));
                strcpy(newFileName, fileName);

                newFile->name = newFileName;
                newFile->type = File;
                newFile->numberOfItems = 0;
                newFile->size = 0;
                newFile->date = time(NULL);
                newFile->content = NULL;
                newFile->next = NULL;
                newFile->child = NULL;

                printf("File '%s' added\n", newFile->name);
            } else {
                fprintf(stderr, "'%s' is already exist in current directory!\n", fileName);
            }
        }
    }
}
// Function to list the current directory
void ls(node *currentFolder) {
    if (currentFolder->child == NULL) {
        printf("___Empty____\n");
    } else {

        node *currentNode = currentFolder->child;

        while (currentNode->next != NULL) {

            struct tm *date_time = localtime(&currentNode->date);
            char dateString[26];
            strftime(dateString, 26, "%d %b %H:%M", date_time);

            if (currentNode->type == Folder) {
                // Print the number of items, date and name of the current node
                printf("%d items\t%s\t%s\n", currentNode->numberOfItems, dateString, currentNode->name);
            } else {
                printf("%dB\t%s\t%s\n", (int)currentNode->size, dateString, currentNode->name);
            }

            currentNode = currentNode->next;
        }

        struct tm *date_time = localtime(&currentNode->date);
        char dateString[26];
        strftime(dateString, 26, "%d %b %H:%M", date_time);

        if (currentNode->type == Folder) {
            printf("%d items\t%s\t%s\n", currentNode->numberOfItems, dateString, currentNode->name);
        } else {
            printf("%dB\t%s\t%s\n", (int)currentNode->size, dateString, currentNode->name);
        }
    }
}
// Function to list the current directory recursively
void lsrecursive(node *currentFolder, int indentCount) {

    if (currentFolder->child == NULL) {
        for (int i = 0; i < indentCount; ++i) {
            printf("\t");
        }
        if (indentCount != 0 ) {
            printf("|_");
        }
        printf("___Empty____\n");
    } else {

        node *currentNode = currentFolder->child;

        while (currentNode->next != NULL) {

            for (int i = 0; i < indentCount; ++i) {
                printf("\t");
            }
            if (indentCount != 0 ) {
                printf("|_");
            }
            struct tm *date_time = localtime(&currentNode->date);
            char dateString[26];
            strftime(dateString, 26, "%d %b %H:%M", date_time);

            if (currentNode->type == Folder) {
                printf("%d items\t%s\t%s\n", currentNode->numberOfItems, dateString, currentNode->name);
            } else {
                printf("%dB\t%s\t%s\n", (int)currentNode->size, dateString, currentNode->name);
            }
            if (currentNode->type == Folder){
                lsrecursive(currentNode, indentCount+1);
            }
            currentNode = currentNode->next;
        }
        for (int i = 0; i < indentCount; ++i) {
            printf("\t");
        }
        if (indentCount != 0 ) {
            printf("|_");
        }
        struct tm *date_time = localtime(&currentNode->date);
        char dateString[26];
        strftime(dateString, 26, "%d %b %H:%M", date_time);

        if (currentNode->type == Folder) {
            printf("%d items\t%s\t%s\n", currentNode->numberOfItems, dateString, currentNode->name);
        } else {
            printf("%dB\t%s\t%s\n", (int)currentNode->size, dateString, currentNode->name);
        }
        if (currentNode->type == Folder){
            lsrecursive(currentNode, indentCount+1);
        }
    }
}
// Function to edit a file
void edit(node *currentFolder, char *command) {

    if (strtok(command, " ") != NULL) {
        char *fileName = strtok(NULL, " ");
        if (fileName != NULL) {
            node * editingNode = getNode(currentFolder, fileName, File);
            if (editingNode != NULL) {
                printf("%s\n", "Please enter some text to overwrite the file content:");
                if (editingNode->content != NULL){
                    free(editingNode->content);
                }
                char* content = getString();
                if (content != NULL) {
                    editingNode->content = content;
                    editingNode->size = strlen(editingNode->content);
                    editingNode->date = time(NULL);
                }
            }
        }
    }
}
// function to print the current directory
void pwd(char *path) {
    if (strlen(path) != 1){

        for (int i = 0; i < strlen(path)-1 ; ++i) {
            printf("%c", path[i]);
        }
        printf("\n");
    } else {
        printf("%s\n", path);
    }
}
// function to read a file
void cat(node *currentFolder, char *command) {
    if (strtok(command, " ") != NULL) {
        char *fileName = strtok(NULL, " ");
        if (fileName != NULL) {
            node *fileNode = getNodeTypeless(currentFolder, fileName);
            if (fileNode != NULL && fileNode->type == File) {
                if (fileNode->content != NULL) {
                    printf("%s\n", fileNode->content);
                } else {
                    printf("File '%s' is empty.\n", fileNode->name);
                }
            } else {
                fprintf(stderr, "File '%s' not found.\n", fileName);
            }
        }
    }
}

// function to change the current directory

node* cd(node *currentFolder, char *command, char **path) {
    if (strtok(command, " ") != NULL) {
        char* folderName = strtok(NULL, " ");
        if (folderName != NULL) {
            if (strcmp(folderName, "..") == 0) {
                // Move to the parent directory
                if (currentFolder->parent != NULL) {
                    currentFolder = currentFolder->parent;
                    size_t newPathLength = strlen(*path) - strlen(currentFolder->name) - 1;
                    *path = (char*)realloc(*path, sizeof(char) * newPathLength);
                    (*path)[newPathLength - 1] = '\0';
                }
                return currentFolder;
            } else if (strcmp(folderName, "/") == 0) {
                // Move to the root directory
                while (currentFolder->parent != NULL) {
                    currentFolder = currentFolder->parent;
                }
                *path = (char*)realloc(*path, sizeof(char) * 2);
                (*path)[0] = '/';
                (*path)[1] = '\0';
                return currentFolder;
            } else {
                node *destinationFolder = getNode(currentFolder, folderName, Folder);
                if (destinationFolder != NULL) {
                    size_t newPathLength = strlen(*path) + strlen(destinationFolder->name) + 2;
                    *path = (char*)realloc(*path, sizeof(char) * newPathLength);
                    strcat(strcat(*path, destinationFolder->name), "/");
                    return destinationFolder;
                } else {
                    fprintf(stderr, "There is no '%s' folder in the current directory!\n", folderName);
                    return currentFolder;
                }
            }
        }
    }
    return currentFolder;
}


void freeNode(node *freeingNode) {

    if (freeingNode->child != NULL) {

        node* currentNode = freeingNode->child;

        while (currentNode->next != NULL) {
            node* nextNode = currentNode->next;
            freeNode(currentNode);
            currentNode = nextNode;
        }
        freeNode(currentNode);
    }
    free(freeingNode->name);
    free(freeingNode->content);
    free(freeingNode);

}

void removeNode(node *removingNode) {
    if (removingNode->parent != NULL){
        if (removingNode->next != NULL) {
            removingNode->next->parent = removingNode->parent;
            removingNode->parent->child = removingNode->next;
            removingNode->next->previous = NULL;
        } else {
            removingNode->parent->child = NULL;
        }
    } else {
        if (removingNode->next != NULL) {
            removingNode->previous->next = removingNode->next;
            removingNode->next->previous  = removingNode->previous;
        } else {
            removingNode->previous->next = NULL;
        }
    }
}

void rm(node *currentFolder, char *command) {

    if (strtok(command, " ") != NULL) {
        char *nodeName = strtok(NULL, " ");
        if (nodeName != NULL) {
            node *removingNode = getNodeTypeless(currentFolder, nodeName);

            if (removingNode != NULL) {

                printf("Do you really want to remove %s and all of its content? (y/n)\n", removingNode->name);

                char *answer = getString();
                if (strcmp(answer, "y") == 0 ) {
                    currentFolder->numberOfItems--;
                    removeNode(removingNode);
                    printf("%s and its all content is removed!\n", removingNode->name);
                    freeNode(removingNode);
                }
                free(answer);
            } else {
                fprintf(stderr, "'%s' is not exist in current directory!\n",  nodeName);
            }
        }
    }
}

void moveNode(node *movingNode, node *destinationFolder) {

    if (destinationFolder->child == NULL) {
        destinationFolder->child = movingNode;
        movingNode->previous = NULL;
        movingNode->parent = destinationFolder;
        movingNode->next = NULL;
    } else {

        node *currentNode = destinationFolder->child;
        while (currentNode->next != NULL) {
            currentNode = currentNode->next;
        }

        currentNode->next = movingNode;
        movingNode->previous = currentNode;
        movingNode->parent = NULL;
        movingNode->next = NULL;
    }
    destinationFolder->numberOfItems++;
}

void mov(node *currentFolder, char *command) {

    char* nodeName;
    char* destinationName;

    if (strtok(command, " ") != NULL) {
        nodeName = strtok(NULL, " ");
        if (nodeName != NULL) {
            destinationName = strtok(NULL, " ");
            if (destinationName != NULL) {
                if (strtok(NULL, " ")) {
                    return;
                } else {

                    node* movingNode = getNodeTypeless(currentFolder, nodeName);
                    node* destinationFolder = getNode(currentFolder, destinationName, Folder);

                    if (destinationFolder != NULL && movingNode != NULL && destinationFolder != movingNode) {

                        removeNode(movingNode);
                        moveNode(movingNode, destinationFolder);
                    } else {
                        fprintf(stderr, "Something you made wrong!\n");
                    }
                }
            }
        }
    }
}



int main() {

    node *root = (node*) malloc(sizeof(node));

    char *rootName = (char *) malloc(sizeof(char)*2);
    strcpy(rootName, "/");
    root->type = Folder;
    root->name = rootName;
    root->numberOfItems = 0;
    root->size = 0;
    root->date = time(NULL);
    root->content =NULL;
    root->previous = NULL;
    root->parent = NULL;
    root->next = NULL;
    root->child = NULL;

    node *currentFolder = root;

    char *path = (char *) malloc(sizeof(char)*2);
    strcpy(path, "/");

    while (1) {

        printf("> ");
        char *command = getString();

        if (strncmp(command, "mkdir", 5) == 0) {
            mkdir(currentFolder, command);
        } else if (strncmp(command, "touch", 5) == 0) {
            touch(currentFolder, command);
        } else if (strcmp(command, "ls") == 0) {
            ls(currentFolder);
        } else if (strcmp(command, "lsrecursive") == 0) {
            lsrecursive(currentFolder, 0);
        } else if (strncmp(command, "edit", 4) == 0 ) {
            edit(currentFolder, command);
        } else if (strcmp(command, "pwd") == 0) {
            pwd(path);  
        } else if (strncmp(command, "cd", 2) == 0){
            currentFolder = cd(currentFolder, command, &path);
        } else if (strncmp(command, "rm", 2) == 0) {
            rm(currentFolder, command);
        } else if (strncmp(command, "mov", 3) == 0) {
            mov(currentFolder, command);
        } else if (strcmp(command, "exit") == 0){
            free(command);
            freeNode(root);
            free(path);
            break;
        }else if (strncmp(command, "cat", 3) == 0) {
            cat(currentFolder, command);
        }
        free(command);
    }

    return 0;
}
