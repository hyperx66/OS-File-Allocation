#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#define nodeNum 130

int blockConfig, totalBlockNum, lastMainBlock;
clock_t start_time, totalTime;
double time_taken;
allocType[20];

struct nodeStruct
{
    int blockNumber;
    char blockVal[300];
};

struct nodeStruct nodeTable[nodeNum];

struct dictionary
{
    char key[30];
    char value[100];
};

struct dictionary dictionaryTable[nodeNum];

void contiguousFileAllocation()
{
    struct tempArray
    {
        char dataVal[300];
    };

    struct tempArray temporaryArr[nodeNum];

    int dataCounter = 0;

    printf("\n---------------------------------\n   Contiguous File Allocation   \n---------------------------------\n\n");
    strcpy(allocType, "contiguous");
    //read CSV file
    FILE *fp = fopen("sample3.csv", "r");
    char buf[1024];

    if (!fp)
    {
        printf("Can't open file\n");
        main();
    }

    int storageNode = -1;
    int startNode = -1;

    int startIndex = -1;
    int storageLocation = 0;
    while (fgets(buf, 1024, fp))
    {
        int noSpace = 0;
        int pointerCounter = 0;
        int checkDuplicate = 0;
        char operator[10];
        char fileName[10];
        char *instruction = strtok(buf, ",");
        while (instruction != NULL)
        {
            if (instruction[0] != '\n')
            {
                pointerCounter++;
                if (strcmp(instruction, "add") == 0 || strcmp(instruction, "delete") == 0 || strcmp(instruction, "read") == 0)
                {
                    //Check previous command
                    if (strcmp(temporaryArr[0].dataVal, "\000") != 0 && pointerCounter == 1)
                    {
                        int startIndex = getEmptyNodeContiguous(startNode, dataCounter);
                        if (startIndex > 130)
                        {
                            noSpace = 1;
                        }
                        else
                        {
                            char currentString[100];
                            char buffer[30];
                            strcpy(currentString, nodeTable[storageLocation].blockVal);
                            strcat(currentString, ",");
                            itoa(startNode, buffer, 10);
                            strcat(currentString, buffer);
                            strcat(currentString, ",");
                            itoa(dataCounter, buffer, 10);
                            strcat(currentString, buffer);
                            strcpy(nodeTable[storageLocation].blockVal, currentString);
                            for (int i = 0; i <= dataCounter - 1; i++)
                            {
                                int nodePointer = i + startIndex;
                                reduceBlockNum(nodeTable[nodePointer].blockNumber);
                                strcpy(nodeTable[nodePointer].blockVal, temporaryArr[i].dataVal);
                            }
                            //Reset temp array
                            for (int i = 0; i <= dataCounter - 1; i++)
                            {
                                char tempTextStore[300];
                                strcpy(tempTextStore, temporaryArr[i].dataVal);
                                memset(tempTextStore, 0, strlen(tempTextStore));
                                strcpy(temporaryArr[i].dataVal, tempTextStore);
                            }
                        }
                    }
                    if (checkDuplicate == 1)
                    {
                        checkDuplicate = 0;
                    }
                    //Execute new instructions
                    strcpy(operator, instruction);
                    if (strcmp(operator, "add") == 0)
                    {
                        startNode = -1;
                        storageNode = -1;
                        dataCounter = 0;
                        storageLocation = 0;
                        storageLocation = getDirectoryEmpty();
                        if (storageLocation == 0)
                        {
                            printf("No storage space available\n");
                            break;
                        }
                    }
                    pointerCounter = 0;
                }
                if (pointerCounter == 1)
                {
                    if (strcmp(operator, "add") == 0)
                    {
                        if (storageLocation != 0)
                        {
                            printf("Adding File name %s to Block Number 0 at Index %d\n", instruction, storageLocation);
                            //check if file exists
                            if (checkFileExistContiguous(instruction) == 0 && nodeTable[storageLocation].blockNumber == 0)
                            {
                                strcpy(fileName, instruction);
                                strcpy(nodeTable[storageLocation].blockVal, instruction);
                                reduceBlockNum(0);
                            }
                            else
                            {
                                printf("File name %s exists\n", instruction);
                                checkDuplicate = 1;
                            }
                        }
                    }
                    if (strcmp(operator, "delete") == 0)
                    {
                        printf("Deleting File name %s\n", instruction);
                        //Find node with the File Name
                        int i;
                        bool fileExists = false;
                        for (i = 1; i < 130; i++)
                        {
                            char nodeDirectoryData[100];
                            char startingNode[30];
                            char endingNode[30];
                            strcpy(nodeDirectoryData, nodeTable[i].blockVal);
                            if (strstr(nodeDirectoryData, instruction) != NULL)
                            {
                                //Node number found
                                deleteContiguous(nodeDirectoryData, i);
                                fileExists = true;
                            }
                        }
                        if (fileExists == false)
                        {
                            printf("File Name %s Not Found\n", instruction);
                        }
                    }
                    if (strcmp(operator, "read") == 0)
                    {
                        //Find for the file data in the file name
                        printf("Finding for %s\n", instruction);
                        char nodeToFind[200];
                        strcpy(nodeToFind, instruction);
                        readContiguous(nodeToFind);
                    }
                }
                if (pointerCounter > 1)
                {
                    //Duplicate validation
                    if (checkDuplicate == 0)
                    {
                        if (strcmp(operator, "add") == 0)
                        {
                            if (startNode == -1)
                            {
                                startNode = getEmptyNode();
                                storageNode = getEmptyNode();
                                int delimFound = 0;
                                for (int i = 0; i < strlen(instruction); i++)
                                {
                                    //Checking if have \n to remove from the string or ensure \n is not put in as a data on its own
                                    if (delimFound == 0)
                                    {
                                        if (instruction[i] == '\n')
                                        {
                                            instruction[i] = '\000';
                                            delimFound++;
                                        }
                                    }
                                }
                                if (strcmp(instruction, "\000") != 0)
                                {
                                    //Store to a temporary array
                                    strcpy(temporaryArr[dataCounter].dataVal, instruction);
                                }
                            }
                            else
                            {
                                int delimFound = 0;
                                for (int i = 0; i < strlen(instruction); i++)
                                {
                                    if (delimFound == 0)
                                    {
                                        //Checking if have \n to remove from the string or ensure \n is not put in as a data on its own
                                        if (instruction[i] == '\n')
                                        {
                                            instruction[i] = '\000';
                                            delimFound++;
                                        }
                                    }
                                }
                                strcpy(temporaryArr[dataCounter].dataVal, instruction);
                            }
                            if (strcmp(instruction, "\000") != 0)
                            {
                                //Datacounter add 1 to use as an argument later
                                dataCounter = dataCounter + 1;
                            }
                        }
                    }
                }

                instruction = strtok(NULL, ",");
            }
            else
            {
                instruction = strtok(NULL, ",");
            }
        }
        //Will run after 1 whole instruction has completed and if the instruction was add
        if (instruction == NULL && strcmp(operator, "add") == 0)
        {
            if (strcmp(temporaryArr[0].dataVal, "\000") != 0)
            {
                int startIndex = getEmptyNodeContiguous(startNode, dataCounter);
                if (noSpace == 1 || startIndex == 131)
                {
                    printf("No space available for %s\n", fileName);
                    strcpy(nodeTable[storageLocation].blockVal, "\000");
                    //Reset temp array
                    for (int i = 0; i <= dataCounter; i++)
                    {
                        char tempTextStore[300];
                        strcpy(tempTextStore, temporaryArr[i].dataVal);
                        memset(tempTextStore, 0, strlen(tempTextStore));
                        strcpy(temporaryArr[i].dataVal, tempTextStore);
                    }
                }
                else
                {
                    char currentString[100];
                    char buffer[30];
                    strcpy(currentString, nodeTable[storageLocation].blockVal);
                    strcat(currentString, ",");
                    itoa(startIndex, buffer, 10);
                    strcat(currentString, buffer);
                    strcat(currentString, ",");
                    itoa(dataCounter, buffer, 10);
                    strcat(currentString, buffer);
                    strcpy(nodeTable[storageLocation].blockVal, currentString);
                    for (int i = 0; i <= dataCounter - 1; i++)
                    {
                        int nodePointer = i + startIndex;
                        reduceBlockNum(nodeTable[nodePointer].blockNumber);
                        strcpy(nodeTable[nodePointer].blockVal, temporaryArr[i].dataVal);
                    }
                    //Reset temp array
                    for (int i = 0; i <= dataCounter; i++)
                    {
                        char tempTextStore[300];
                        strcpy(tempTextStore, temporaryArr[i].dataVal);
                        memset(tempTextStore, 0, strlen(tempTextStore));
                        strcpy(temporaryArr[i].dataVal, tempTextStore);
                    }
                }
            }
        }
    }

    fclose(fp);
}

void linkedFileAllocation()
{
    printf("\n---------------------------------\n     Linked File Allocation     \n---------------------------------\n\n");
    strcpy(allocType, "linked");
    //read CSV file
    FILE *fp = fopen("sample3.csv", "r");
    char buf[1024];

    if (!fp)
    {
        while (!fp)
        {
            printf("Can't open file\n");
            main();
        }
    }

    int endingNode = -1;
    int pointerCounter = 0;
    int storageLocation = 0;
    while (fgets(buf, 1024, fp))
    {
        char operator[10];
        char *instruction = strtok(buf, ",");
        int checkDuplicate = 0;
        while (instruction != NULL)
        {
            if (strcmp(instruction, "\n") != 0)
            {
                pointerCounter++;
                if (strcmp(instruction, "add") == 0 || strcmp(instruction, "delete") == 0 || strcmp(instruction, "read") == 0)
                {
                    if (checkDuplicate == 1)
                    {
                        checkDuplicate = 0;
                    }
                    if (endingNode != -1)
                    {
                        int i;
                        char endNodeHold[100];
                        strcpy(endNodeHold, nodeTable[endingNode].blockVal);
                        //remove everything after the , for the last node
                        int delimFound = 0;
                        for (i = 0; i < strlen(endNodeHold); i++)
                        {
                            if (delimFound == 0)
                            {
                                if (endNodeHold[i] == '\n')
                                {
                                    endNodeHold[i] = '\000';
                                    delimFound++;
                                }
                                if (endNodeHold[i] == ',')
                                {
                                    endNodeHold[i] = '\000';
                                    delimFound++;
                                }
                            }
                            else
                            {
                                endNodeHold[i] = '\000';
                            }
                        }
                        strcpy(nodeTable[endingNode].blockVal, endNodeHold);
                        //Add the last node to the storage location then reset everything
                        char directoryData[100];
                        char buffer[30];
                        strcpy(directoryData, nodeTable[storageLocation].blockVal);
                        strcat(directoryData, ",");
                        itoa(endingNode, buffer, 10);
                        strcat(directoryData, buffer);
                        strcpy(nodeTable[storageLocation].blockVal, directoryData);
                        endingNode = -1;
                    }
                    strcpy(operator, instruction);
                    if (strcmp(operator, "add") == 0)
                    {
                        storageLocation = 0;
                        storageLocation = getDirectoryEmpty();
                        if (storageLocation == 0)
                        {
                            printf("No storage space available\n");
                            break;
                        }
                    }
                    pointerCounter = 0;
                }
                if (pointerCounter == 1)
                {
                    if (strcmp(operator, "add") == 0)
                    {
                        if (storageLocation != 0)
                        {
                            printf("Adding File name %s to Block Number 0 at Index %d\n", instruction, storageLocation);
                            if (checkFileExistContiguous(instruction) == 0 && nodeTable[storageLocation].blockNumber == 0)
                            {
                                strcpy(nodeTable[storageLocation].blockVal, instruction);
                                reduceBlockNum(0);
                            }
                            else
                            {
                                printf("File name %s exists\n", instruction);
                                checkDuplicate = 1;
                            }
                        }
                    }
                    if (strcmp(operator, "delete") == 0)
                    {
                        int nodeFound = 0;
                        printf("Deleting File name %s\n", instruction);
                        //Find node with the File Name
                        int i;
                        for (i = 1; i < 130; i++)
                        {
                            char nodeDirectoryData[100];
                            char startingNode[30];
                            char endingNode[30];
                            strcpy(nodeDirectoryData, nodeTable[i].blockVal);
                            if (strstr(nodeDirectoryData, instruction) != NULL)
                            {
                                //Node number found
                                deleteLinked(nodeDirectoryData, i);
                                nodeFound = 1;
                                break;
                            }
                        }
                        if (nodeFound == 0)
                        {
                            printf("File name %s not found. Delete aborted\n", instruction);
                        }
                    }
                    if (strcmp(operator, "read") == 0)
                    {
                        printf("Reading File name %s\n", instruction);
                        //Find node with the File Name
                        readLinked(instruction);
                    }
                }
                if (pointerCounter > 1)
                {
                    if (checkDuplicate == 0)
                    {
                        char currentString[100];
                        char buffer[30];
                        char bufferSecond[30];
                        char nodeData[100];
                        int storageNode = getEmptyNode();
                        strcpy(nodeTable[storageNode].blockVal, instruction);
                        reduceBlockNum(nodeTable[storageNode].blockNumber);
                        int nextNodeInsert = getEmptyNode();
                        if (nextNodeInsert != NULL)
                        {
                            //Add next node address to current node
                            strcpy(nodeData, nodeTable[storageNode].blockVal);
                            strcat(nodeData, ",");
                            itoa(nextNodeInsert, bufferSecond, 10);
                            strcat(nodeData, bufferSecond);
                            strcpy(nodeTable[storageNode].blockVal, nodeData);

                            //Reduce number of available nodes in free space management table
                            endingNode = storageNode;
                        }
                        if (pointerCounter == 2)
                        {
                            //Update Directory block on starting node number
                            strcpy(currentString, nodeTable[storageLocation].blockVal);
                            strcat(currentString, ",");
                            itoa(storageNode, buffer, 10);
                            strcat(currentString, buffer);
                            strcpy(nodeTable[storageLocation].blockVal, currentString);
                        }
                    }
                }

                instruction = strtok(NULL, ",");
            }
            else
            {
                instruction = strtok(NULL, ",");
            }
        }
        if (instruction == NULL)
        {
            if (checkDuplicate == 0)
            {
                if (endingNode != -1)
                {
                    int i;
                    char endNodeHold[100];
                    strcpy(endNodeHold, nodeTable[endingNode].blockVal);
                    //remove everything after the , for the last node
                    int delimFound = 0;
                    for (i = 0; i < strlen(endNodeHold); i++)
                    {
                        if (delimFound == 0)
                        {
                            if (endNodeHold[i] == '\n')
                            {
                                endNodeHold[i] = '\000';
                                delimFound++;
                            }
                            if (endNodeHold[i] == ',')
                            {
                                endNodeHold[i] = '\000';
                                delimFound++;
                            }
                        }
                        else
                        {
                            endNodeHold[i] = '\000';
                        }
                    }
                    strcpy(nodeTable[endingNode].blockVal, endNodeHold);
                    //Add the last node to the storage location then reset everything
                    char directoryData[100];
                    char buffer[30];
                    strcpy(directoryData, nodeTable[storageLocation].blockVal);
                    strcat(directoryData, ",");
                    itoa(endingNode, buffer, 10);
                    strcat(directoryData, buffer);
                    strcpy(nodeTable[storageLocation].blockVal, directoryData);
                    endingNode = -1;
                }
            }
        }
    }

    fclose(fp);
}

void indexedFileAllocation()
{
    printf("\n---------------------------------\n     Indexed File Allocation    \n---------------------------------\n\n");
    strcpy(allocType, "indexed");
    //read CSV file
    FILE *fp = fopen("sample3.csv", "r");
    char buf[1024];

    if (!fp)
    {
        printf("Can't open file\n");
        main();
    }

    while (fgets(buf, 1024, fp))
    {
        int pointerCounter = 0;
        int storageLocation = 0;
        char operator[10];
        int checkDuplicate = 0;
        char *instruction = strtok(buf, ",");

        while (instruction != NULL)
        {
            if (strcmp(instruction, "\n") != 0)
            {
                pointerCounter++;
                if (strcmp(instruction, "add") == 0 || strcmp(instruction, "delete") == 0 || strcmp(instruction, "read") == 0)
                {
                    strcpy(operator, instruction);
                    if (strcmp(operator, "add") == 0)
                    {
                        storageLocation = 0;
                        storageLocation = getDirectoryEmpty();
                        if (storageLocation == 0)
                        {
                            printf("No storage space available\n");
                            break;
                        }
                    }
                    pointerCounter = 0;
                }
                if (pointerCounter == 1)
                {
                    if (strcmp(operator, "add") == 0)
                    {
                        if (storageLocation != 0)
                        {
                            printf("Adding File name %s to Block Number 0 at Index %d\n", instruction, storageLocation);
                            if (checkFileExistContiguous(instruction) == 0 && nodeTable[storageLocation].blockNumber == 0)
                            {
                                strcpy(nodeTable[storageLocation].blockVal, instruction);
                                reduceBlockNum(0);
                            }
                            else
                            {
                                printf("File name %s exists\n", instruction);
                                checkDuplicate = 1;
                            }
                        }
                    }
                    if (strcmp(operator, "delete") == 0)
                    {
                        printf("Deleting File name %s\n", instruction);
                        //Find node with the File Name
                        int i, fileDeleted = 0;
                        for (i = 1; i < 130; i++)
                        {
                            char nodeDirectoryData[100];
                            char startingNode[30];
                            char endingNode[30];
                            strcpy(nodeDirectoryData, nodeTable[i].blockVal);
                            if (indexCheckFile(nodeDirectoryData, instruction) == 1)
                            {
                                //Node number found
                                fileDeleted = 1;
                                deleteIndexed(nodeDirectoryData, i);
                                break;
                            }
                        }
                        if (fileDeleted == 0)
                        {
                            printf("File name %s not found. Delete aborted\n", instruction);
                        }
                    }
                    if (strcmp(operator, "read") == 0)
                    {
                        printf("Reading File name %s\n", instruction);
                        readIndexed(instruction);
                    }
                }
                if (pointerCounter > 1)
                {
                    if (checkDuplicate == 0)
                    {
                        if (strcmp(operator, "add") == 0)
                        {
                            //Adding Data
                            char currentString[100];
                            char buffer[30];
                            int storageNode = getEmptyNode();
                            int delimFound = 0;
                            for (int i = 0; i < strlen(instruction); i++)
                            {
                                if (delimFound == 0)
                                {
                                    if (instruction[i] == '\n')
                                    {
                                        instruction[i] = '\000';
                                        delimFound++;
                                    }
                                }
                            }
                            strcpy(nodeTable[storageNode].blockVal, instruction);
                            strcpy(currentString, nodeTable[storageLocation].blockVal);
                            strcat(currentString, ",");
                            itoa(storageNode, buffer, 10);
                            strcat(currentString, buffer);
                            strcpy(nodeTable[storageLocation].blockVal, currentString);

                            //Reduce number of available nodes in free space management table
                            int blockNum = nodeTable[storageNode].blockNumber;
                            reduceBlockNum(blockNum);
                        }
                    }
                }

                instruction = strtok(NULL, ",");
            }
            else
            {
                instruction = strtok(NULL, ",");
            }
        }
    }

    fclose(fp);
}

void hashTableFileAllocation()
{
    printf("\n---------------------------------\n   Hash Table Allocation    \n---------------------------------\n\n");
    strcpy(allocType, "hashTable");
    //read CSV file
    FILE *fp = fopen("sample3.csv", "r");
    char buf[1024];

    if (!fp)
    {
        printf("Can't open file\n");
        main();
    }

    int previousNode = 0;
    int pointerCounter = 0;
    int storageLocation = 0;
    while (fgets(buf, 1024, fp))
    {
        int nodeFound = 0;
        char operator[10];
        char *instruction = strtok(buf, ",");
        char previousData[200];
        while (instruction != NULL)
        {
            if (strcmp(instruction, "\n") != 0 || strcmp(instruction, "") != 0)
            {
                pointerCounter++;
                if (strcmp(instruction, "add") == 0 || strcmp(instruction, "delete") == 0 || strcmp(instruction, "read") == 0)
                {
                    if (strcmp(operator, "add") == 0)
                    {
                        if (strcmp(previousData, "") != 0)
                        {
                            strcpy(dictionaryTable[previousNode].value, previousData);
                            previousNode = 0;
                            strcpy(previousData, '\000');
                        }
                    }
                    strcpy(operator, instruction);
                    pointerCounter = 0;
                }
                else if (pointerCounter == 1)
                {
                    if (strcmp(operator, "add") == 0)
                    {
                        previousNode = getEmptyNodeDictionary();
                        printf("Adding File name %s to Block Number 0 at Index %d\n", instruction, previousNode);
                        strcpy(dictionaryTable[previousNode].key, instruction);
                    }
                    else if (strcmp(operator, "delete") == 0)
                    {
                        int deleteFound = 0;
                        printf("Deleting File Name %s\n", instruction, previousNode);
                        for (int i = 0; i < nodeNum; i++)
                        {
                            if (strcmp(dictionaryTable[i].key, instruction) == 0)
                            {
                                deleteFound = 1;
                                strcpy(dictionaryTable[i].key, "\000");
                                strcpy(dictionaryTable[i].value, "\000");
                                break;
                            }
                        }
                        if (deleteFound == 0)
                        {
                            printf("File %s does not exist", instruction);
                        }
                    }
                    else
                    {
                        printf("Reading %s\n", instruction);
                        int nodePointer = -1;
                        while (nodeFound == 0)
                        {
                            nodePointer++;
                            if (strstr(dictionaryTable[nodePointer].value, instruction))
                            {
                                printf("Data %s(%s) Found\n", instruction, dictionaryTable[nodePointer].key);
                                nodeFound = 1;
                                break;
                            }
                            if (nodePointer > blockConfig)
                            {
                                break;
                            }
                        }
                        if (nodeFound == 0)
                        {
                            printf("Data %s does not exist\n", instruction);
                        }
                    }
                }
                else
                {
                    if (strcmp(operator, "add") == 0)
                    {
                        if (strcmp(previousData, "") == 0)
                        {
                            int delimFound = 0;
                            for (int i = 0; i < strlen(instruction); i++)
                            {
                                if (delimFound == 0)
                                {
                                    if (instruction[i] == '\n')
                                    {
                                        instruction[i] = '\000';
                                        delimFound++;
                                    }
                                }
                            }
                            strcpy(previousData, instruction);
                        }
                        else
                        {
                            int delimFound = 0;
                            for (int i = 0; i < strlen(instruction); i++)
                            {
                                if (delimFound == 0)
                                {
                                    if (instruction[i] == '\n')
                                    {
                                        instruction[i] = '\000';
                                        delimFound++;
                                    }
                                }
                            }
                            if (strcmp(instruction, "") != 0)
                            {
                                strcat(previousData, ",");
                                strcat(previousData, instruction);
                            }
                        }
                    }
                }
                instruction = strtok(NULL, ",");
            }
            else
            {
                instruction = strtok(NULL, ",");
            }
        }
        if (instruction == NULL)
        {
            if (strcmp(operator, "add") == 0)
            {
                strcpy(dictionaryTable[previousNode].value, previousData);
                previousNode = 0;
                strcpy(previousData, "\000");
            }
        }
    }

    fclose(fp);
}

//Get directory block that is available to input
int getDirectoryEmpty()
{
    int blockNum = 0;
    int emptyNumInBlock = 0;
    char node0Data[1000] = "";
    strcpy(node0Data, nodeTable[0].blockVal);
    char *end_str;
    char *ptr = strtok_r(node0Data, ",", &end_str);

    while (ptr != NULL)
    {
        char *end_block;
        char *ptr2 = strtok_r(ptr, ";", &end_block);
        if (ptr2 != NULL)
        {
            blockNum = atoi(ptr2);
            ptr2 = strtok_r(NULL, ";", &end_block);
            emptyNumInBlock = atoi(ptr2);
            if (blockNum == 0)
            {
                if (emptyNumInBlock > 0)
                {
                    for (int i = 1; i < 130; i++)
                    {
                        if (strcmp(nodeTable[i].blockVal, "") == 0 && nodeTable[i].blockNumber == 0)
                        {
                            return i;
                        }
                    }
                }
                else
                {
                    return 0;
                }
            }
            break;
        }
        ptr = strtok_r(NULL, ",", &end_str);
    }
}

//Get node that is empty for data
int getEmptyNode()
{
    char node0Data[300] = "";
    int blockNum = 0;
    int emptyNumBlock = 0;
    strcpy(node0Data, nodeTable[0].blockVal);
    char *end_str;
    char *ptr = strtok_r(node0Data, ",", &end_str);

    while (ptr != NULL)
    {
        char *end_block;
        char *ptr2 = strtok_r(ptr, ";", &end_block);
        if (ptr2 != NULL)
        {
            blockNum = atoi(ptr2);
            ptr2 = strtok_r(NULL, ";", &end_block);
            emptyNumBlock = atoi(ptr2);
            if (blockNum > 0)
            {
                if (emptyNumBlock > 0)
                {
                    break;
                }
            }
        }
        ptr = strtok_r(NULL, ",", &end_str);
    }

    for (int i = 0; i < 130; i++)
    {
        if (nodeTable[i].blockNumber == blockNum)
        {
            if (strcmp(nodeTable[i].blockVal, "") == 0)
            {
                return i;
            }
        }
        else
        {
            i = i + blockConfig - 1;
        }
    }
}

//Special for contiguous
int getEmptyNodeContiguous(int startIndex, int dataNum)
{
    char node0Data[1000] = "";
    int blockNum = 0;
    int emptyNumBlock = 0;
    strcpy(node0Data, nodeTable[0].blockVal);
    char *end_str;
    char *ptr = strtok_r(node0Data, ",", &end_str);

    while (ptr != NULL)
    {
        char *end_block;
        char *ptr2 = strtok_r(ptr, ";", &end_block);
        if (ptr2 != NULL)
        {
            blockNum = atoi(ptr2);
            ptr2 = strtok_r(NULL, ";", &end_block);
            emptyNumBlock = atoi(ptr2);
            if (blockNum > 0)
            {
                if (emptyNumBlock >= dataNum)
                {
                    break;
                }
            }
        }
        ptr = strtok_r(NULL, ",", &end_str);
    }

    //Check if there's enough space in one line
    int blockNumber = nodeTable[startIndex].blockNumber;
    int tempCounter = startIndex;
    for (int j = 0; j <= dataNum; j++)
    {
        if (tempCounter > nodeNum)
        {
            return 131;
        }
        else
        {
            if (strcmp(nodeTable[tempCounter].blockVal, "") != 0)
            {
                return getEmptyNodeContiguous(startIndex + blockConfig - 1, dataNum);
            }
            else if (j == dataNum && strcmp(nodeTable[tempCounter].blockVal, "") == 0)
            {
                return startIndex;
            }
            else
            {
                tempCounter++;
            }
        }
    }
}

int getEmptyNodeDictionary()
{
    for (int i = 0; i <= 130; i++)
    {
        if (strcmp(dictionaryTable[i].key, "") == 0)
        {
            return i;
        }
    }
}

//To minus with blockNum
int reduceBlockNum(blockNum)
{
    char node0Data[1000] = "";
    char newNodeData[1000] = "";
    char buffer[10];
    int blockNumber;
    int blockNode;
    strcpy(node0Data, nodeTable[0].blockVal);
    char *end_str;
    char *ptr = strtok_r(node0Data, ",", &end_str);

    while (ptr != NULL)
    {
        char *end_block;
        char *ptr2 = strtok_r(ptr, ";", &end_block);
        if (ptr2 != NULL)
        {
            blockNumber = atoi(ptr2);
            strcat(newNodeData, ptr2);
            strcat(newNodeData, ";");
            ptr2 = strtok_r(NULL, ";", &end_block);
            if (blockNumber == blockNum)
            {
                blockNode = atoi(ptr2);
                blockNode--;
                strcat(newNodeData, itoa(blockNode, buffer, 10));
                strcat(newNodeData, ",");
            }
            else
            {
                if (blockNumber != lastMainBlock)
                {
                    strcat(newNodeData, ptr2);
                    strcat(newNodeData, ",");
                }
                else
                {
                    strcat(newNodeData, ptr2);
                }
            }
        }
        ptr = strtok_r(NULL, ",", &end_str);
    }
    strcpy(nodeTable[0].blockVal, newNodeData);
}

int indexCheckFile(nodeDirectoryData, instruction)
{
    char *end_str;
    char nodeData[300] = "";
    int pointerCounter = 0;
    strcpy(nodeData, nodeDirectoryData);
    char *ptr = strtok_r(nodeData, ",", &end_str);
    while (ptr != NULL)
    {
        pointerCounter++;
        if (pointerCounter == 1)
        {
            if (strcmp(ptr, instruction) == 0)
            {
                return 1;
            }
            else
            {
                break;
            }
        }
    }
    return 0;
}

int increaseBlockNum(blockNum)
{
    char node0Data[1000] = "";
    char newNodeData[1000] = "";
    char buffer[10];
    int blockNumber;
    int blockNode;
    strcpy(node0Data, nodeTable[0].blockVal);
    char *end_str;
    char *ptr = strtok_r(node0Data, ",", &end_str);

    while (ptr != NULL)
    {
        char *end_block;
        char *ptr2 = strtok_r(ptr, ";", &end_block);
        if (ptr2 != NULL)
        {
            blockNumber = atoi(ptr2);
            strcat(newNodeData, ptr2);
            strcat(newNodeData, ";");
            ptr2 = strtok_r(NULL, ";", &end_block);
            if (blockNumber == blockNum)
            {
                blockNode = atoi(ptr2);
                blockNode++;
                strcat(newNodeData, itoa(blockNode, buffer, 10));
                strcat(newNodeData, ",");
            }
            else
            {
                strcat(newNodeData, ptr2);
                strcat(newNodeData, ",");
            }
        }
        ptr = strtok_r(NULL, ",", &end_str);
    }
    strcpy(nodeTable[0].blockVal, newNodeData);
}

int checkFileExistContiguous(instruction)
{
    char node0Data[300] = "";
    int pointerCounter = 0;
    for (int i = 1; i < blockConfig; i++)
    {
        strcpy(node0Data, nodeTable[i].blockVal);
        char *end_str;
        char *ptr = strtok_r(node0Data, ",", &end_str);

        while (ptr != NULL)
        {
            pointerCounter++;
            if (pointerCounter == 1)
            {
                if (strcmp(ptr, instruction) == 0)
                {
                    return 1;
                }
            }
            ptr = strtok_r(NULL, ",", &end_str);
        }
        if (ptr == NULL)
        {
            pointerCounter = 0;
        }
        if (i == blockConfig)
        {
            return 0;
        }
    }
    return 0;
}

//HERE IS WHERE ALL THE DELETE FUNCTIONS ARE NESTED

//Delete for Contiguous Allocation
void deleteContiguous(directoryData, directoryNum)
{
    int pointerCounter = 0;
    char startingNode[30];
    char lengthNode[30];
    int endNode = -1;
    char *end_str;
    char *ptr = strtok_r(directoryData, ",", &end_str);

    while (ptr != NULL)
    {
        pointerCounter++;
        if (pointerCounter == 2)
        {
            strcpy(startingNode, ptr);
        }
        else
        {
            strcpy(lengthNode, ptr);
        }
        ptr = strtok_r(NULL, ",", &end_str);
    }

    endNode = atoi(startingNode) + (atoi(lengthNode));
    for (int i = atoi(startingNode); i < endNode; i++)
    {
        *nodeTable[i].blockVal = 0;
        increaseBlockNum(nodeTable[i].blockNumber);
    }
    *nodeTable[directoryNum].blockVal = 0;
    increaseBlockNum(nodeTable[directoryNum].blockNumber);
}

//Delete for Linked Allocation
void deleteLinked(directoryData, directoryNum)
{
    int pointerCounter = 0;
    int endPointer = 0;
    int nodePointer = 0;
    int nextNodePointer = 0;
    char startingNode[30];
    char endingNode[30];
    char nodeData[300];
    char *end_str;
    char *ptr = strtok_r(directoryData, ",", &end_str);

    while (ptr != NULL)
    {
        pointerCounter++;
        if (pointerCounter == 2)
        {
            strcpy(startingNode, ptr);
            nodePointer = atoi(startingNode);
            nextNodePointer = atoi(startingNode);
        }
        else
        {
            strcpy(endingNode, ptr);
        }
        ptr = strtok_r(NULL, ",", &end_str);
    }
    while (endPointer == 0)
    {
        char *end_str2;
        strcpy(nodeData, nodeTable[nodePointer].blockVal);
        char *ptr2 = strtok_r(nodeData, ",", &end_str2);
        int nextCounter = 0;
        while (ptr2 != NULL)
        {
            nextCounter++;
            if (nextCounter == 2)
            {
                nextNodePointer = atoi(ptr2);
            }
            ptr2 = strtok_r(NULL, ",", &end_str2);
        }
        *nodeTable[nodePointer].blockVal = 0;
        increaseBlockNum(nodeTable[nodePointer].blockNumber);
        if (nodePointer == atoi(endingNode))
        {
            endPointer++;
        }
        nodePointer = nextNodePointer;
    }
    *nodeTable[directoryNum].blockVal = 0;
    increaseBlockNum(nodeTable[directoryNum].blockNumber);
}

//Delete for Indexed Allocation
void deleteIndexed(directoryData, directoryNum)
{
    int pointerCounter = 0;
    char *end_str;
    char *ptr = strtok_r(directoryData, ",", &end_str);

    while (ptr != NULL)
    {
        pointerCounter++;
        if (pointerCounter >= 2)
        {
            int dataNode = atoi(ptr);
            *nodeTable[dataNode].blockVal = 0;
            increaseBlockNum(nodeTable[dataNode].blockNumber);
        }
        ptr = strtok_r(NULL, ",", &end_str);
    }

    *nodeTable[directoryNum].blockVal = 0;
    increaseBlockNum(nodeTable[directoryNum].blockNumber);
}

void readContiguous(nodeToFind)
{
    //File Name, Start, Length
    bool nodeFound = false;
    int nodeCounter = 0;
    char dataToFind[300];
    char directoryData[300];
    strcpy(dataToFind, nodeToFind);
    while (nodeFound == false)
    {
        if (nodeCounter != 130)
        {
            int pointerCounter = 0;
            nodeCounter++;
            if (strcmp(nodeTable[nodeCounter].blockVal, "") != 0 || nodeCounter != nodeNum + 1)
            {
                strcpy(directoryData, nodeTable[nodeCounter].blockVal);
                char *end_str;
                char *ptr = strtok_r(directoryData, ",", &end_str);
                int fileName = 0;
                int startNode = 0;
                while (ptr != NULL)
                {
                    pointerCounter++;
                    if (pointerCounter == 1)
                    {
                        fileName = atoi(ptr);
                    }
                    else if (pointerCounter == 2)
                    {
                        startNode = atoi(ptr);
                        if (strcmp(nodeTable[startNode].blockVal, dataToFind) == 0)
                        {
                            printf("Data %s(%d) found\n", nodeTable[startNode].blockVal, fileName);
                            nodeFound = true;
                            break;
                        }
                    }
                    else
                    {
                        for (int i = 1; i < atoi(ptr); i++)
                        {
                            if (strcmp(nodeTable[startNode + i].blockVal, dataToFind) == 0)
                            {
                                printf("Data %s(%d) found\n", nodeTable[startNode + i].blockVal, fileName);
                                nodeFound = true;
                                break;
                            }
                        }
                    }
                    ptr = strtok_r(NULL, ",", &end_str);
                }
            }
        }
        else
        {
            break;
        }
    }
    if (nodeFound == false)
    {
        printf("Data %s does not exist\n", dataToFind);
    }
}

void readLinked(nodeToFind)
{
    bool nodeFound = false;
    int nodeCounter = 0;
    char dataToFind[300];
    char directoryData[300];
    char nodeData[300];
    int pointerCounter = 0;
    strcpy(dataToFind, nodeToFind);
    while (nodeFound == false)
    {
        nodeCounter++;
        if (nodeCounter <= blockConfig)
        {
            if (strcmp(nodeTable[nodeCounter].blockVal, "") != 0)
            {
                //Get directory
                strcpy(directoryData, nodeTable[nodeCounter].blockVal);
                char *end_str;
                char *ptr = strtok_r(directoryData, ",", &end_str);
                int fileName = 0;
                int startNode = 0;
                int endNode = 0;
                while (ptr != NULL)
                {
                    pointerCounter++;
                    if (pointerCounter == 1)
                    {
                        //Get file name
                        fileName = atoi(ptr);
                    }
                    else if (pointerCounter == 2)
                    {
                        //Get starting Node
                        startNode = atoi(ptr);
                    }
                    else
                    {
                        //Last argument, start iterating through the link to check data
                        endNode = atoi(ptr);
                        int hitTheEnd = 0;
                        int nodeToCheck = startNode;
                        int innerPointerCounter = 0;
                        while (hitTheEnd == 0)
                        {
                            strcpy(nodeData, nodeTable[nodeToCheck].blockVal);
                            char *end_str2;
                            char *ptr2 = strtok_r(nodeData, ",", &end_str2);
                            while (ptr2 != NULL)
                            {
                                innerPointerCounter++;
                                if (innerPointerCounter == 1)
                                {
                                    if (strcmp(dataToFind, ptr2) == 0)
                                    {
                                        printf("Data %s(%d) found\n", ptr2, fileName);
                                        nodeFound = true;
                                        hitTheEnd = 1;
                                        break;
                                    }
                                }
                                else if (innerPointerCounter == 2)
                                {
                                    //Didnt match the node
                                    if (atoi(ptr2) != endNode)
                                    {
                                        nodeToCheck = atoi(ptr2);
                                    }
                                    else
                                    {
                                        hitTheEnd = 1;
                                        pointerCounter = 0;
                                        if (strcmp(nodeTable[endNode].blockVal, dataToFind) == 0)
                                        {
                                            printf("Data %s(%d) found\n", dataToFind, fileName);
                                            nodeFound = true;
                                            hitTheEnd = 1;
                                            break;
                                        }
                                    }
                                    innerPointerCounter = 0;
                                }
                                ptr2 = strtok_r(NULL, ",", &end_str2);
                            }
                        }
                    }
                    ptr = strtok_r(NULL, ",", &end_str);
                }
            }
        }
        else
        {
            break;
        }
    }
    if (nodeFound == false)
    {
        printf("Data %s does not exist\n", dataToFind);
    }
}

void readIndexed(nodeToFind)
{
    //File Name, Index Block
    bool nodeFound = false;
    int nodeCounter = 0;
    char directoryData[300];
    while (nodeFound == false)
    {
        nodeCounter++;
        if (nodeCounter <= blockConfig)
        {
            if (strcmp(nodeTable[nodeCounter].blockVal, "") != 0)
            {
                strcpy(directoryData, nodeTable[nodeCounter].blockVal);
                char *end_str;
                char *ptr = strtok_r(directoryData, ",", &end_str);
                int fileName = 0;
                int startNode = 0;
                int pointerCounter = 0;
                while (ptr != NULL)
                {
                    pointerCounter++;
                    if (pointerCounter == 1)
                    {
                        fileName = atoi(ptr);
                    }
                    else
                    {
                        if (strcmp(nodeTable[atoi(ptr)].blockVal, nodeToFind) == 0)
                        {
                            printf("Data %s(%d) found\n", nodeTable[atoi(ptr)].blockVal, fileName);
                            nodeFound = true;
                            break;
                        }
                    }
                    ptr = strtok_r(NULL, ",", &end_str);
                }
            }
        }
        else
        {
            break;
        }
    }
    if (nodeFound == false)
    {
        printf("Data %s does not exist\n", nodeToFind);
    }
}

void emptyBlockPrint()
{
    printf("\n---------------------------------\n          Empty Space           \n---------------------------------\n\n");
    if (strcmp(allocType, "hashTable") == 0)
    {
        //Run here if the previous allocation type was hash table
        char buffer[3];
        int nodeFree = 0;
        for (int i = 0; i < nodeNum; i++)
        {
            if (strcmp(dictionaryTable[i].value, "") == 0)
            {
                if (strcmp(dictionaryTable[i].key, "") == 0)
                {
                    nodeFree++;
                }
            }
        }
        itoa(nodeFree, buffer, 10);
        printf("Total Free Node is %s \n", buffer);
    }
    else
    {
        //Run here if previous allocation type was everything except hash table
        char node0Data[1000] = "";
        strcpy(node0Data, nodeTable[0].blockVal);
        char *end_str;
        char *ptr = strtok_r(node0Data, ",", &end_str);

        while (ptr != NULL)
        {
            char *end_block;
            //Get node 0 control block
            char *ptr2 = strtok_r(ptr, ";", &end_block);
            if (ptr2 != NULL)
            {
                //Print nodes that are empty
                printf("Block Number %s has ", ptr2);
                ptr2 = strtok_r(NULL, ";", &end_block);
                printf("%s nodes free\n", ptr2);
            }
            ptr = strtok_r(NULL, ",", &end_str);
        }
    }
}

void printAll()
{
    printf("Index     Block Number        Data     \n");
    for (int i = 0; i < 130; i++)
    {
        printf("%d     %d        %s     \n", i, nodeTable[i].blockNumber, nodeTable[i].blockVal);
    }
}

void printDictionary()
{
    printf("Index     Key        Data     \n");
    for (int i = 0; i < 130; i++)
    {
        printf("%d     %s        %s     \n", i, dictionaryTable[i].key, dictionaryTable[i].value);
    }
}

void tableInitialise()
{
    int blockRemainder, nodeCounter;

    for (int i = 0; i <= nodeNum; i++)
    {
        nodeTable[i].blockNumber = 0;
        strcpy(nodeTable[i].blockVal, "\000");
    }

    //initiate Startup Seq
    int blockNum = 0;
    printf("Enter number of blocks within 3 to 130\n");
    scanf(" %i", &blockNum);

    //Number of nodes per block
    blockConfig = 130 / blockNum;
    //Remainder nodes per block1
    blockRemainder = 130 % blockNum;
    printf("Number of nodes per block created is %i with a remainder of %i nodes\n\n", blockConfig, blockRemainder);

    nodeCounter = 0;
    char node0Data[300] = "";
    char numNodeDataInput[3];
    //assign block number to nodee
    for (int i = 0; i < blockNum; i++)
    {
        if (i == 0)
        {
            itoa(i, numNodeDataInput, 10);
            strcat(node0Data, numNodeDataInput);
            strcat(node0Data, ";");
            itoa(blockConfig, numNodeDataInput, 10);
            strcat(node0Data, numNodeDataInput);
            strcat(node0Data, ",");
        }
        else if (i == blockNum - 1)
        {
            if (blockRemainder == 0)
            {
                itoa(i, numNodeDataInput, 10);
                strcat(node0Data, numNodeDataInput);
                strcat(node0Data, ";");
                itoa(blockConfig, numNodeDataInput, 10);
                strcat(node0Data, numNodeDataInput);
            }
            else
            {
                itoa(i, numNodeDataInput, 10);
                strcat(node0Data, numNodeDataInput);
                strcat(node0Data, ";");
                itoa(blockConfig, numNodeDataInput, 10);
                strcat(node0Data, numNodeDataInput);
                strcat(node0Data, ",");
            }
        }
        else
        {
            itoa(i, numNodeDataInput, 10);
            strcat(node0Data, numNodeDataInput);
            strcat(node0Data, ";");
            itoa(blockConfig, numNodeDataInput, 10);
            strcat(node0Data, numNodeDataInput);
            strcat(node0Data, ",");
        }

        //setting to ensure number of nodes is assigned equally
        for (int j = 0; j < blockConfig; j++)
        {
            nodeTable[nodeCounter].blockNumber = i;
            nodeCounter++;
            lastMainBlock = i;
        }
    }
    //Generate addresses for extra nodes if any
    if (blockRemainder != 0)
    {
        for (int i = 0; i < blockRemainder; i++)
        {
            if (i != blockRemainder - 1)
            {
                lastMainBlock++;
                nodeTable[nodeCounter].blockNumber = lastMainBlock;
                itoa(lastMainBlock, numNodeDataInput, 10);
                strcat(node0Data, numNodeDataInput);
                strcat(node0Data, ";");
                strcat(node0Data, "1");
                strcat(node0Data, ",");
                nodeCounter++;
            }
            else
            {
                lastMainBlock++;
                nodeTable[nodeCounter].blockNumber = lastMainBlock;
                itoa(lastMainBlock, numNodeDataInput, 10);
                strcat(node0Data, numNodeDataInput);
                strcat(node0Data, ";");
                strcat(node0Data, "1");
                nodeCounter++;
            }
        }
    }

    //assign index 0 Free Space Management Table data
    if (nodeTable[0].blockNumber == 0)
    {
        strcpy(nodeTable[0].blockVal, node0Data);
        reduceBlockNum(0);
    }

    printf("Disk formatted...\n");
}

int main()
{
    int optionAction;
    //Print out instructions
    printf("\nPlease type the option number of your desired action\n1. Contiguous File Allocation\n2. Linked File Allocation\n3. Indexed File Allocation\n4. Hash Table File Allocation\n5. Empty Space\n6. Exit\n\n");
    scanf("%i", &optionAction);
    switch (optionAction)
    {
    case 1:
        tableInitialise();
        start_time = clock();
        contiguousFileAllocation();
        printAll();
        totalTime = clock() - start_time;
        time_taken = ((double)totalTime) / CLOCKS_PER_SEC;
        printf("Total time elapsed: %f", time_taken);
        main();
        break;

    case 2:
        tableInitialise();
        start_time = clock();
        linkedFileAllocation();
        printAll();
        totalTime = clock() - start_time;
        time_taken = ((double)totalTime) / CLOCKS_PER_SEC;
        printf("Total time elapsed: %f", time_taken);
        main();
        break;

    case 3:
        tableInitialise();
        start_time = clock();
        indexedFileAllocation();
        printAll();
        totalTime = clock() - start_time;
        time_taken = ((double)totalTime) / CLOCKS_PER_SEC;
        printf("Total time elapsed: %f", time_taken);
        main();
        break;

    case 4:
        tableInitialise();
        start_time = clock();
        hashTableFileAllocation();
        printDictionary();
        totalTime = clock() - start_time;
        time_taken = ((double)totalTime) / CLOCKS_PER_SEC;
        printf("Total time elapsed: %f", time_taken);
        main();
        break;

    case 5:
        emptyBlockPrint();
        main();
        break;

    case 6:
        break;

    default:
        main();
    }

    return 0;
}