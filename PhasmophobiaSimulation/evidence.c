#include "defs.h"

/*
  Function:  initEvidenceList
  Purpose:   the purpose of the function is to initialize the evience list
       in:   the function takes in the list of evidence
*/

void initEvidenceList(EvidenceListType* evidenceList){
    evidenceList->head = NULL;
    evidenceList->tail = NULL;
    evidenceList->size = 0;
}

/*
  Function:  initEvidence
  Purpose:   the purpose of the function is to initialize the the evidenve data
       in:   the function takes in the array of evidence names
       in:   the function takes in the reading of the evidence
       in:   the function takes in the double pointer evidence
*/

void initEvidence(EvidenceClassType et, float reading, EvidenceType** evidence){
    *evidence = malloc(sizeof(EvidenceType));
    (*evidence)->evidence = et;
    (*evidence)->dataReading = reading;
}

/*
  Function:  addEvidence
  Purpose:   the purpose of the function is to add evidence to the evidence linked list
       in:   the function takes in the the evidence list
       in:   the function also take sthe type of evidence it is
*/

void addEvidence(EvidenceListType* evidenceList, EvidenceType* evidence){
    EvidenceNodeType* curr = evidenceList->head;
    EvidenceNodeType* prev = NULL;
    EvidenceNodeType* new = malloc(sizeof(EvidenceNodeType));
    new->data = evidence;
    
    while(curr != NULL){
        prev = curr;
        curr = curr->next;
    }
    if(prev == NULL){
        evidenceList->tail = new;
        evidenceList->head = new;
    }
    else{
        evidenceList->tail = new;
        prev->next = new;
    }
    new->next = curr;
    evidenceList->size ++;
}

/*
    Function:   removeEvidence
    Purpose:    to remove the evidence that is passed in from the evidenceList
    in:         the evidence we want to remove
    in/out:     the evidence list from which we want to remove the passed in evidence from
*/

int removeEvidence(EvidenceListType* evidenceList, EvidenceType* evidence){
    EvidenceNodeType* curr = evidenceList->head;
    EvidenceNodeType* prev = NULL;

    while(curr != NULL){
        if(curr->data == evidence){
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    if(curr == NULL){
        return -1;
    }
    if(prev == NULL){
        evidenceList->head = curr->next;
        evidenceList->tail = curr->next;
    }
    else{
        prev->next = curr->next;
    }
    evidenceList->size --;
    return 0;
}

/*
  Function:  cleanUpEvidence
  Purpose:   the purpose of the function is to clean up the evidence
       in:   the function taks in the evidence list
*/

void cleanUpEvidence(EvidenceListType* evidenceList){
    EvidenceNodeType* next = NULL;
    EvidenceNodeType* curr = evidenceList->head;
    while(curr != NULL){
        next = curr->next;
        free(curr->data);
        curr = next;
    }

}

/*
  Function:  cleanUpEvidenceList
  Purpose:   the purpose of the function is to clean Up Evidence List
       in:   the function takes in the evidence list
*/

void cleanUpEvidenceList(EvidenceListType* evidenceList){
    EvidenceNodeType* next = NULL;
    EvidenceNodeType* curr = evidenceList->head;

    while(curr != NULL){
        next = curr->next;
        free(curr);
        curr = next;
    }
    evidenceList->size = 0;
}

/*
  Function:  checkForGhostlyEvidence
  Purpose:   the puspose of the function is to check for a ghostly evidece
       in:   the function takes in the the evidence type
       in:   the function also takes in the hunter
   return:   the function returns the result the evidencce is ghostly
*/

int checkForGhostlyEvidence(EvidenceType* evidence){
    int result = 1;
    EvidenceClassType type = evidence->evidence;
    if(type == EMF && evidence->dataReading > 4.90){
        result = 0;
    }
    else if(type == TEMPERATURE && evidence->dataReading < 0.00){
        result = 0;
    }
    else if(type == FINGERPRINTS && evidence->dataReading == 1.00){
        result = 0;
    }
    else{
        if(evidence->dataReading > 70.00){
            result = 0;
        }
    }
    return result;
}

/*
  Function:  generateEvidence
  Purpose:   the purpose of the function is to generate evidence
       in:   the function takes in the room type 
       in:   the function also take sin the ghost type
*/

void generateEvidence(RoomType* room, GhostType* ghost, BuildingType building){
    float randomFloat;
    int typeChosen = randInt(0,3);
    EvidenceClassType type = ghost->dropTypes[typeChosen];

    if(type == EMF){
        randomFloat = randFloat(0.00, 5.00);
    }
    else if(type == TEMPERATURE){
        randomFloat = randFloat(-10.00, 27.00);
    }
    else if(type == FINGERPRINTS){
        randomFloat = randInt(0, 2);
    }
    else{
        randomFloat = randFloat(40.00, 75.00);
    }

    EvidenceType* newEvidence;
    initEvidence(type, randomFloat, &newEvidence);
    // this malloc is not getting freed
    EvidenceNodeType* newEvidenceNode = malloc(sizeof(EvidenceNodeType));
    newEvidenceNode->data = newEvidence;
    addEvidence(room->evidence, newEvidenceNode->data);
    addEvidence(ghost->droppedEvidence, newEvidenceNode->data);

    // new
    addEvidence(building.evidenceList, newEvidenceNode->data);

    int res = checkForGhostlyEvidence(newEvidence);
    if(res == 1){
        printf("The ghost has left GHOSTLY ");
        printEvidenceType(newEvidence);
        printf(" evidence in the %s\n", room->name);
    }

    // try
    // IF THE PROGRAM CRASHES THIS IS THE REASOIN BRO
    free(newEvidenceNode);
}

/*
    Function:   checkEvidence
    Purpose:    To check whether the passed in evidence is in the list
    in:         the evidence we want to check
    in:         the list that we want to search the evidence in
    return:     integer that allows us to know whether the evidence is in the list or not
*/

int checkEvidence(EvidenceListType* list, EvidenceType* evidence){
    int res = 1;
    EvidenceNodeType* curr = list->head;
    while(curr != NULL){
        if(curr->data->dataReading == evidence->dataReading && curr->data->evidence == evidence->evidence){
            res = 0;  
            break;
        }
        curr = curr->next;
    }
    return res;
}

/*
  Function:  readEvidence
  Purpose:   the purpose of the function is to read evidence from the ghost
       in:   the function takes in a hunter that will read the evidence
*/

void readEvidence(HunterType* hunter){
    EvidenceNodeType* curr = hunter->currRoom->evidence->head;
    while(curr != NULL){
        if(checkEvidence(hunter->evidenceList, curr->data)== 1 && hunter->evidence == curr->data->evidence){
            EvidenceType* newEvidence;
            initEvidence(curr->data->evidence, curr->data->dataReading, &newEvidence);
            addEvidence(hunter->evidenceList, newEvidence);
            if(checkForGhostlyEvidence(curr->data) == 0){
                addEvidence(hunter->ghostlyEvidence, newEvidence);
                printf("%s has collected GHOSTLY ", hunter->name);
                printEvidenceType(newEvidence);
                printf(" from the %s\n",hunter->currRoom->name);
            }
            else{
                printf("%s has collected some RANDOM ", hunter->name);
                printEvidenceType(newEvidence);
                printf(" evidence.\n");
            }
            removeEvidence(hunter->currRoom->evidence, curr->data);
        }
        if(checkForGhostlyEvidence(curr->data) == 0){
            hunter->timer = BOREDOM_MAX;
        }
        else{
            hunter->timer --;
        }
        curr = curr->next;
    }
}

/*
  Function:  addGhostlyEvidence
  Purpose:   the purpose of the function is to add ghostly evidence to the hunters list
       in:   the function takes in the first hunter
       in:   the function also takes in the second hunter
*/

void addGhostlyEvidence(HunterType* hunter1, HunterType* hunter2){
    EvidenceNodeType* curr = hunter1->evidenceList->head;
    while(curr != NULL){
        EvidenceClassType type = curr->data->evidence;
        if(type == EMF && curr->data->dataReading > 4.90 && checkEvidence(hunter2->evidenceList, curr->data) == 1){
            printf("%s has shared GHOSTLY EMF evidence with %s\n", hunter1->name, hunter2->name);
            addEvidence(hunter2->ghostlyEvidence, curr->data);
        }
        else if(type == TEMPERATURE && curr->data->dataReading < 0.00 && checkEvidence(hunter2->evidenceList, curr->data) == 1){
            printf("%s has shared GHOSTLY TEMPERATURE evidence with %s\n", hunter1->name, hunter2->name);
            addEvidence(hunter2->ghostlyEvidence, curr->data);
        }
        else if(type == FINGERPRINTS && curr->data->dataReading == 1.00 && checkEvidence(hunter2->evidenceList, curr->data) == 1){
            printf("%s has shared GHOSTLY FINGERPRINTS evidence with %s\n", hunter1->name, hunter2->name);       
            addEvidence(hunter2->ghostlyEvidence, curr->data);
        }
        else{
            if(curr->data->dataReading > 70.00 && checkEvidence(hunter2->evidenceList, curr->data) == 1){
                printf("%s has shared GHOSTLY SOUND evidence with %s\n", hunter1->name, hunter2->name);
                addEvidence(hunter2->ghostlyEvidence, curr->data);
            }
        }
        curr = curr->next;
    }
}

/*
  Function:  communicateEvidence
  Purpose:   the purpose of the function is to communicate evidecne between hunters
       in:   the function takes in a hunter
*/

void communicateEvidence(HunterType* hunter1){
    int randomHunter;
    HunterType* hunter2;
    while(1){
        if(hunter1->currRoom->hunters->size <= 1){
            break;
        }
        randomHunter = randInt(0, hunter1->currRoom->hunters->size);
        if(hunter1->currRoom->hunters->elements[randomHunter] != hunter1){
            hunter2 = hunter1->currRoom->hunters->elements[randomHunter];
            addGhostlyEvidence(hunter1, hunter2);
            addGhostlyEvidence(hunter2, hunter1);
            break;
        }
    }
}

/*
  Function:  printEvidence
  Purpose:   the purpose of the function is to print the evidecne
       in:   the function takes in the room the evidence is in
*/

void printEvidence(RoomType* room){
    EvidenceNodeType* curr = room->evidence->head;

    while(curr != NULL){
        printf("%f\n", curr->data->dataReading);
        curr = curr->next;
    }
}

/*
  Function:  printHunterEvidence
  Purpose:   the purpose of the function is to print the evidecne the hunter is holding
       in:   the function takes in the hunter whos evidnece is being printed
*/

void printHunterEvidence(HunterType* hunter){
    EvidenceNodeType* curr = hunter->evidenceList->head;

    while(curr != NULL){
        printf("%f\n",curr->data->dataReading);
        curr = curr->next;
    }
}

/*
  Function:  printHunterGhostlyEvidence
  Purpose:   the purpose of the function is to print the hunters ghostky evidence
       in:   the function takes in the hunter 
*/

void printHunterGhostlyEvidence(HunterType* hunter){
    EvidenceNodeType* curr = hunter->ghostlyEvidence->head;

    while(curr != NULL){
        printf("%f\n",curr->data->dataReading);
        curr = curr->next;
    }
}

/*
    Function:   printEvidenceType
    Purpose:    to print the type of evidence that was passed in
    in:         the evidence whose type we want to print
*/

void printEvidenceType(EvidenceType* evidence){
    if(evidence->evidence == EMF){
        printf("EMF");
    }
    else if(evidence->evidence == FINGERPRINTS){
        printf("FINGERPRINTS");
    }
    else if(evidence->evidence == TEMPERATURE){
        printf("TEMPERATURE");
    }
    else{
        printf("SOUND");
    }
}
