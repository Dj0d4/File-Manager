#include <iostream>
#include <direct.h>
#include <dirent.h>
#include <Windows.h>
#include <string>
#include <regex>

using namespace std;

string notAllowed = "\\/:*?\"<>|";

int mainMenu();

bool validFileName(const char *fileName)
{
    if(fileName != NULL && strlen(fileName) >= 6)
    {
        if(fileName[strlen(fileName) - 1] == '.') return false;

        for(int i = 1; i < (int) strlen(fileName); i++)
        {
            if((int) fileName[i] >= 0 && (int) fileName[i] <= 31) return false;

            if(string(notAllowed).find(fileName[i]) != string::npos) return false;
        }
    } else return false;


    return true;
}

bool validPath(const char *path) {
    if(path != NULL) {
        string regexPattern = "^[a-zA-Z]:\\(((?![<>:""/\\|?*]).)*[^ ]\\)*((?![<>:""/\\|?*]).)*[^ ]\\?$";
        regex regexRule(regexPattern);

        bool valid = regex_match(path, regexRule);
        if(valid) return true;
    }

    return false;
}

void createFile(const char *fileName, const char *path, const char *folderName)
{
    if(validFileName(fileName) && path != NULL)
    {
        char *file = new char[strlen(path) + strlen(fileName) + strlen(folderName) + 1];
        strcpy(file, path);
        strcat(file, folderName);
        strcat(file, fileName);

        FILE *newFile = fopen(file, "w");

        if(newFile == NULL) cout << "Oops! Something went wrong!" << endl;
        else cout << "Successfully created a new file!" << endl;

        free(file);
        file = nullptr;
    } else cout << "Error: Check the name or path!" << endl;

    return;
}

void createFolder(const char *folderName, const char *path) {
    char *folder = new char[strlen(path) + strlen(folderName) + 1];
    strcpy(folder, path);
    strcat(folder, folderName);

    if(mkdir(folder) == -1) cout << "Error: Failed to create a new folder!" << endl;
    else cout << "Successfully created a new folder!" << endl;

    free(folder);
    folder = nullptr;

    return;
}

void multipleFiles(const char *path) {
    char *folderName = new char[100];
    char *fileName = new char[100];
    short int nFiles;

    cout << "Enter the folder name (eg. \\Name)" << endl;
    cin >> folderName;

    createFolder(folderName, path);

    cout << "How many files do you want? (Limit 15)" << endl;
    cin >> nFiles;

    if(nFiles > 15 || nFiles < 1) {
        cout << "Invalid number of files!" << endl;
        Sleep(2000);
        system("cls");
        mainMenu();
    }

    cout << "Enter their names along with their extension (eg. \\Name.txt)" << endl;
    for(int i = 0; i < nFiles; i++) {
            cout << i+1 << ". File: ";
            cin >> fileName;
            createFile(fileName, path, folderName);
    }
    cout << endl;
    cout << "Success!" << endl;

    free(folderName);
    free(fileName);
    folderName = nullptr;
    fileName = nullptr;

    return;
}

void deleteFile(const char *fileName, const char *path, const char *folderName) {
    char *fileToDelete = new char[strlen(fileName) + strlen(path) + strlen(folderName) + 1];
    strcpy(fileToDelete, path);
    strcat(fileToDelete, folderName);
    strcat(fileToDelete, fileName);

    if(remove(fileToDelete) != 0) perror("Error deleting file ");
    else puts("File successfully deleted!");

    free(fileToDelete);
    fileToDelete = nullptr;

    return;
}

void deleteFolder(const char *path, const char *folderName) {
    char *folderToDelete = new char[strlen(path) + strlen(folderName) + 1];
    strcpy(folderToDelete, path);
    strcat(folderToDelete, folderName);

    char *p = new char[strlen(folderName) + strlen(path) + 1];
    strcpy(p, path);
    strcat(p, folderName);

    DIR *d;
    struct dirent *dir;
    d = opendir(p);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            char *fileToDelete = new char[strlen(dir->d_name) + 1];

            strcpy(fileToDelete, "\\");
            strcat(fileToDelete, dir->d_name);
            deleteFile(fileToDelete, path, folderName);

            free(fileToDelete);
            fileToDelete = nullptr;
        }
        closedir(d);
    }

    if(RemoveDirectoryA(folderToDelete) == 0) cout << "Error!" << endl;
    else cout << "Success!" << endl;

    free(folderToDelete);
    folderToDelete = nullptr;

    return;
}

void displayFiles(const char *folderName, const char *path) {
    char *p = new char[strlen(folderName) + strlen(path) + 1];
    strcpy(p, path);
    strcat(p, folderName);

    DIR *d;
    struct dirent *dir;
    d = opendir(p);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }

    free(p);
    p = nullptr;

    return;
}

int mainMenu() {
    char *path = new char[200]; //= "C:\\Users\\filip\\OneDrive\\Desktop";
    char *folderName = new char[100];
    char *fileName = new char[100];
    int choice;

    cout << "Enter the path" << endl;
    cin >> path;

    if(!validPath(path)) {
        cout << "That is not a valid path!" << endl;
        Sleep(3000);
        system("cls");
        mainMenu();
    }

    system("cls");

    cout << "File Manager V0.1" << endl;
    cout << "Choose your next action" << endl;
    cout << "   1. Create a new folder\n   2. Create a new file\n   3. Create multiple files and store them inside of a folder\n   4. Delete a file\n   5. Delete a folder\n   6. Display files\n   7. Exit" << endl;
    cin >> choice;

    switch(choice) {
        case 1:
            cout << "Enter the name for your folder (eg. \\Name)" << endl;
            cin >> folderName;
            createFolder(folderName, path);
            break;
        case 2:
            cout << "Enter the file name with it's extension (eg. \\FileName.txt)" << endl;
            cin >> fileName;
            cout << "Enter the folder name in which you want to make your file (eg. \\MyFolder)" << endl;
            cin >> folderName;
            createFile(fileName, path, folderName);
            break;
        case 3:
            multipleFiles(path);
            break;
        case 4:
            cout << "Enter the name of the file with it's extension you wanna delete (eg. \\Name.txt)" << endl;
            cin >> fileName;
            cout << "Enter the name of the folder in which your file is stored (eg. \\Name)" << endl;
            cin >> folderName;
            deleteFile(fileName, path, folderName);
            break;
        case 5:
            cout << "Enter the name of the folder you want to delete (eg. \\Name)" << endl;
            cin >> folderName;
            deleteFolder(path, folderName);
            break;
        case 6:
            cout << "Enter the name of the folder (eg. \\Name)" << endl;
            cin >> folderName;
            cout << "These are the current files in " << folderName << endl;
            displayFiles(folderName, path);
            break;
        case 7:
            cout << "Bye!" << endl;
            return 0;
            break;
        default:
            cout << "That option does not exist" << endl;
            break;
    }

    Sleep(5000);
    system("cls");

    free(path);
    free(fileName);
    free(folderName);
    path = nullptr;
    fileName = nullptr;
    folderName = nullptr;

    mainMenu();

    return 0;
}

int main()
{
    mainMenu();
}
