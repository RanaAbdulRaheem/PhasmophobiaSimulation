#include "defs.h"

/*
  Function:  initHunterArray
  Purpose:   the puspose of the function is to initialize the hunter array 
       in:   the function takes in a array of hunters
*/

void initHunterArray(HunterArrayType* arr){
    arr->size = 0;
}

/*
  Function:  initHunter
  Purpose:   the puspose of the function is to initialize the hunters
       in:   the function takes in a room 
       in:   the function takes in a the evidenceClassType basically the evidence
       in:   the function takes in the huner 
       in:   the function takes in the name of the hunter
*/

void initHunter(RoomType* r, EvidenceClassType et, char* name, HunterType** hunter){
    *hunter = malloc(sizeof(HunterType));
    (*hunter)->currRoom = r;
    (*hunter)->evidence = et;
    (*hunter)->fear = 0;
    (*hunter)->timer = BOREDOM_MAX;
    strcpy((*hunter)->name, name);
    
    EvidenceListType* hunterEvidence = malloc(sizeof(EvidenceListType));
    initEvidenceList(hunterEvidence);
    (*hunter)->evidenceList = hunterEvidence;

    EvidenceListType* ghostlyEvidence = malloc(sizeof(EvidenceListType));
    initEvidenceList(ghostlyEvidence);
    (*hunter)->ghostlyEvidence = ghostlyEvidence;
}

/*
  Function:  addHunter
  Purpose:   the purpose of the function is to add the hunter into the building and the room 
       in:   the function takes in the array of the hunters
       in:   the function takes i the hunter itself
*/

void addHunter(HunterArrayType* arr, HunterType* hunter){
    if(arr->size < MAX_HUNTERS){
        arr->elements[arr->size] = hunter;
        arr->size ++;
    }
}

/*
  Function:  removeHunter
  Purpose:   the purpose of the function is to remove the hunter from the room and building
       in:   the function takes in the array of hunters
       in:   the function takes in the hunter itself
*/

void removeHunter(HunterArrayType* arr, HunterType* hunter){
    // HunterType* newHunter = malloc(sizeof(HunterType));
    for(int i = 0; i < arr->size; i++){
        if(arr->elements[i] == hunter){
            for(int j = i ; j < arr->size; j++){
                arr->elements[j] = arr->elements[j + 1];
            }
            arr->size--;
            break;
        }
    }
}

/*
  Function:  moveHunter
  Purpose:   the puopose of the function is to move the hunter form one room to another
       in:   the function takes in the hunter
       in:   the function takes in the room 
*/

void moveHunter(HunterType* hunter, RoomType* room){
    removeHunter(hunter->currRoom->hunters, hunter);
    addHunter(room->hunters, hunter);
    hunter->currRoom = room;
    printf("%s will move to %s.\n", hunter->name, room->name);

}

/*
  Function:  hunterAction
  Purpose:   the purpose of the function is to keep track o fthe hunter actions
       in:   the function takes in the hunter
       in:   the function also takes in the ghostType ghost
   return:   the function returns 1 when the hunter is ready to leave the building
*/

int hunterAction(HunterType* hunter, GhostType* ghost){
    int result = randInt(0,3);
    if(hunter->currRoom == ghost->room){
        hunter->fear ++;
    }
    if(hunter->timer <= 0 || hunter->fear >= 100){
        return 0;
    }
    if(result == 0){
        int numRooms = hunter->currRoom->rooms->size;
        int randomRoom = randInt(0, numRooms);
        RoomNodeType* curr = hunter->currRoom->rooms->head;
        for(int i = 0; i < randomRoom; i++){
            curr = curr->next;
        }
        moveHunter(hunter, curr->room);
        hunter->timer--;
    }
    else if(result == 1){
        readEvidence(hunter);
    }
    else{
        if(hunter->currRoom->hunters->size > 1){
            communicateEvidence(hunter);
            hunter->timer--;
        }
    }
    return 1;
}


/*
  Function:  printHunters
  Purpose:   the purpose of this function is to print the hunters
       in:   the function in an arrya of hunters
*/

void printHunters(HunterArrayType* arr){
    for(int i = 0; i < arr->size; i++){
        printf("%s\n", arr->elements[i]->name);
    }
}

/*
  Function:  cleanUpHunter
  Purpose:   the purpose of the function is to clean up the hunter 
       in:   the function takes in a hunter
*/

void cleanUpHunter(HunterType* hunter){
    cleanUpEvidenceList(hunter->evidenceList);
    cleanUpEvidenceList(hunter->ghostlyEvidence);
    free(hunter->ghostlyEvidence);
    free(hunter->evidenceList);
    free(hunter);
}

/*
  Function:  cleanUpHunterArray
  Purpose:   the purpose of the function is to clean up the hunter array
       in:   the function takes in the array of hunters
*/

void cleanUpHunterArray(HunterArrayType* arr){
    for(int i = 0; i < arr->size; i++){
        cleanUpHunter(arr->elements[i]);
    }
    free(arr);
}


