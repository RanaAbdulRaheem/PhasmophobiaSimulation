#include "defs.h"

/*
  Function:  initGhost
  Purpose:   the purpose of this function is to initialize the ghost
       in:   gt - this function takes inte different ghosts names from the enum
       in:   r* - this function taks in a room
       in:   this function also takes in a ghost
*/

void initGhost(GhostClassType gt, RoomType* r, GhostType** ghost){
    *ghost = malloc(sizeof(GhostType));
    (*ghost)->ghostType = gt;
    (*ghost)->room = r;
    (*ghost)->boredom = BOREDOM_MAX;
    EvidenceListType* evList = malloc(sizeof(EvidenceListType));
    initEvidenceList(evList);
    (*ghost)->droppedEvidence = evList;

    if((*ghost)->ghostType == POLTERGEIST){
        (*ghost)->dropTypes[0] = EMF;
        (*ghost)->dropTypes[1] = TEMPERATURE;
        (*ghost)->dropTypes[2] = FINGERPRINTS;
    }
    else if((*ghost)->ghostType == BANSHEE){
        (*ghost)->dropTypes[0] = EMF;
        (*ghost)->dropTypes[1] = TEMPERATURE;
        (*ghost)->dropTypes[2] = SOUND;
    }
    else if((*ghost)->ghostType == BULLIES){
        (*ghost)->dropTypes[0] = EMF;
        (*ghost)->dropTypes[1] = FINGERPRINTS;
        (*ghost)->dropTypes[2] = SOUND;
    }
    else{
        (*ghost)->dropTypes[0] = TEMPERATURE;
        (*ghost)->dropTypes[1] = FINGERPRINTS;
        (*ghost)->dropTypes[2] = SOUND;
    }
}

/*
  Function:  addGhost
  Purpose:   the purpose of this function is to add the ghost to the room
       in:   this function takes in a ghost
       in:   the function also takes the room the ghost being added to
*/

void addGhost(RoomType* room, GhostType* ghost){
    room->ghost = ghost;
}

/*
  Function:  removeGhost
  Purpose:   the purpose of the function is to remove the ghost from the room
       in:   the function takes in the room that the ghost is being removed from
*/

void removeGhost(RoomType* room){
    room->ghost = NULL;
}

/*
  Function:  moveGhost
  Purpose:   the purpose of the function is to move the ghost
       in:   the function takes in the room that the ghost is being moved to
       in:   the function also takes in the ghost
*/

void moveGhost(GhostType* ghost, RoomType* room){
    removeGhost(ghost->room);
    addGhost(ghost, room);
    ghost->room = room;

    printf("Ghost will move to %s\n",room->name);
}

/*
  Function:  ghostAction
  Purpose:   the purpose of the function is keep track of ghost actions
       in:   the function takes in a ghost
   return:   the function returns 1 when the ghost is bored and will leave the building
*/

int ghostAction(GhostType* ghost, BuildingType building){
    HunterArrayType* arr = ghost->room->hunters;
    
    if(arr->size == 0){
        ghost->boredom --;
        if(ghost->boredom <= 0){
            printf("Ghost has exited the building\n");
            return 0;
        }
        int randomRoom = randInt(0, ghost->room->rooms->size);
        int randomResult = randInt(0,2);
        if (randomResult == 0){
            generateEvidence(ghost->room, ghost, building);
        }
        else if (randomResult == 1){
            RoomNodeType* curr = ghost->room->rooms->head;
            for(int i = 0; i < randomRoom; i++){
                curr = curr->next;
            }
            moveGhost(ghost, curr->room);
        }
    }
    else{
        ghost->boredom = BOREDOM_MAX;
        int addOrNot = randInt(0, 2);
        if(addOrNot == 0){
            generateEvidence(ghost->room, ghost, building);
        }
    }
    return 1;
}

/*
  Function:  cleanUpGhost
  Purpose:   the purpose of the function is to clean the ghost
       in:   the function takes in the ghost that is being cleaned up
*/

void cleanUpGhost(GhostType* ghost){
    cleanUpEvidenceList(ghost->droppedEvidence);
    // free(ghost->droppedEvidence);
    free(ghost);
}
/*
    Function: printGhostType
    Purpose:  the purpose of the fuction is to print the type of ghost that is passed in
    in:       the ghost whose type we want to print
*/

void printGhostType(GhostType* ghost){
    if(ghost->ghostType == POLTERGEIST){
        printf("POLTERGIEST");
    }
    else if(ghost->ghostType == BANSHEE){
        printf("BANSHEE");
    }
    else if(ghost->ghostType == BULLIES){
        printf("BULLIES");
    }
    else{
        printf("PHANTOM");
    }
}

