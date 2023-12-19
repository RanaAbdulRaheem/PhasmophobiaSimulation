#include "defs.h"
/*
  Function:  initRoomList
  Purpose:   purpose of this function is to initialize the room linke dlist
       in:   this function takes in a list of rooms
*/

void initRoomList(RoomListType* list){
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

/*
  Function:  initRoom
  Purpose:   the purpose of this function is to initialize the room
       in:   takes in a room
       in:   also takes in the name of the room
*/

void initRoom(RoomType* room, char* name){
    strcpy(room->name, name);

    RoomListType* connectedRooms = malloc(sizeof(RoomListType));
    initRoomList(connectedRooms);
    room->rooms = connectedRooms;

    EvidenceListType* evidenceInRoom = malloc(sizeof(EvidenceListType));
    initEvidenceList(evidenceInRoom);
    room->evidence = evidenceInRoom;

    HunterArrayType* hunters = malloc(sizeof(HunterArrayType));
    initHunterArray(hunters);
    room->hunters = hunters;

    GhostType* ghost = NULL;
    sem_init(&room->mutex, 0, 1);
}

/*
  Function:  appendRoom
  Purpose:   purpose of this function is to append th rooms into the building
       in:   this function takes in a list of rooms
       in:   this function also takes in in the room node for rhe linked list
*/

void appendRoom(RoomListType* list, RoomNodeType* roomNode){
    RoomNodeType* curr = list->head;
    RoomNodeType* prev = NULL;
    
    while(curr != NULL){
        prev = curr;
        curr = curr->next;
    }
    if(prev == NULL){
        list->tail = roomNode;
        list->head = roomNode;
    }
    else{
        list->tail = roomNode;
        prev->next = roomNode;
    }
    roomNode->next = curr;
    list->size++;
}

/*
  Function:  connectRooms
  Purpose:   the purpose of this function is to connect the rooms
       in:   it takes in the first room
       in:   it take sin the second room to connect to
*/

void connectRooms(RoomType* first, RoomType* second){
    RoomNodeType* firstRoomNode = malloc(sizeof(RoomNodeType));
    RoomNodeType* secondRoomNode = malloc(sizeof(RoomNodeType));

    firstRoomNode->room = first;
    secondRoomNode->room = second;

    appendRoom(first->rooms, secondRoomNode);
    appendRoom(second->rooms, firstRoomNode);
}

/*
  Function:  printRooms
  Purpose:   the purpose of this function is to print the rooms
       in:   this function takes in a room
*/

void printRooms(RoomType* room){
    RoomNodeType* curr = room->rooms->head;
    
    while(curr != NULL){
        printf("%s\n", curr->room->name);
        curr = curr->next;
    }
}

/*
  Function:  cleanUpRoom
  Purpose:   the purpose of this function is to clean the room
       in:   this function takes in a room
*/

void cleanUpRoom(RoomType* room){
    RoomNodeType* curr = room->rooms->head;
    RoomNodeType* next = NULL;

    while(curr != NULL){
        next = curr->next;
        free(curr);
        curr = next;
    }

    free(room->rooms);
    cleanUpEvidenceList(room->evidence);
    free(room->evidence);

    free(room->hunters);
    // free(room);
}


