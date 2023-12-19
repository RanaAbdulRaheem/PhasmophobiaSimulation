#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_STR            64
#define FEAR_RATE           1
#define MAX_FEAR          100
#define MAX_HUNTERS         4
#define USLEEP_TIME     50000
#define BOREDOM_MAX        99

// You may rename these types if you wish
typedef enum { EMF, TEMPERATURE, FINGERPRINTS, SOUND } EvidenceClassType;
typedef enum { POLTERGEIST, BANSHEE, BULLIES, PHANTOM } GhostClassType;

typedef struct{
    char name[MAX_STR];
    struct RoomList *rooms;
    struct EvidenceList *evidence;
    struct Hunters* hunters;
    struct Ghost* ghost;
    sem_t mutex;
}RoomType;

typedef struct Node{
    RoomType* room;
    struct Node* next;
}RoomNodeType;

typedef struct RoomList{
    RoomNodeType* head;
    RoomNodeType* tail;
    int size;
}RoomListType;

typedef struct Ghost{
    GhostClassType ghostType;
    RoomType* room;
    int boredom;
    struct EvidenceList* droppedEvidence;

    EvidenceClassType dropTypes[3];
}GhostType;

typedef struct{
    EvidenceClassType evidence;
    float dataReading;
}EvidenceType;

typedef struct NodeEvidence{
    EvidenceType* data;
    struct NodeEvidence* next;
}EvidenceNodeType;

typedef struct EvidenceList{
    EvidenceNodeType* head;
    EvidenceNodeType* tail;
    int size;
}EvidenceListType;

typedef struct{
    RoomType* currRoom;
    EvidenceClassType evidence;
    struct EvidenceList *evidenceList;
    struct EvidenceList *ghostlyEvidence;
    char name[MAX_STR];
    int fear;
    int timer;
}HunterType;

typedef struct Hunters{
    HunterType* elements[MAX_HUNTERS];
    int size;
}HunterArrayType;

typedef struct{
    GhostType* ghost;
    HunterArrayType* hunters;
    struct RoomList* rooms;
    struct EvidenceList *evidenceList;
}BuildingType;

typedef struct{
    GhostType* ghost;
    HunterType* hunter;
}ActionType;

typedef struct{
    GhostType* ghost;
    BuildingType building;
}GhostAction;

void initRoomList(RoomListType*);
void initRoom(RoomType*, char*);
void appendRoom(RoomListType*, RoomNodeType*);
void connectRooms(RoomType*, RoomType*);
void printRooms(RoomType*);

void initGhost(GhostClassType, RoomType*, GhostType**);
void addGhost(RoomType*, GhostType*);
void removeGhost(RoomType*);
void moveGhost(GhostType*, RoomType*);
void withHunter(GhostType*);
void withoutHunter(GhostType*);
int ghostAction(GhostType*, BuildingType);
void ghostStart(GhostType*);
void printGhostType(GhostType*);


void initHunterArray(HunterArrayType*);
void initHunter(RoomType*, EvidenceClassType, char*, HunterType**);
void addHunter(HunterArrayType*, HunterType*);
void removeHunter(HunterArrayType*, HunterType*);
void moveHunter(HunterType*, RoomType*);
void withGhost(HunterType*);
int hunterAction(HunterType*, GhostType*);
void printHunters(HunterArrayType*);

void initEvidenceList(EvidenceListType*);
void initEvidence(EvidenceClassType, float, EvidenceType**);
void addEvidence(EvidenceListType*, EvidenceType*);
int removeEvidence(EvidenceListType*, EvidenceType*);
int checkEvidence(EvidenceListType*, EvidenceType*);
int checkForGhostlyEvidence(EvidenceType*);

// added BuildingType parameter
void generateEvidence(RoomType*, GhostType*, BuildingType);


void readEvidence(HunterType*);
void addGhostlyEvidence(HunterType*, HunterType*);
void communicateEvidence(HunterType*);
void printEvidence(RoomType*);
void printHunterEvidence(HunterType*);
void printHunterGhostlyEvidence(HunterType*);
void printEvidenceType(EvidenceType*);

void populateRooms(BuildingType*);   // Populates the building with sample data for rooms
void initBuilding(BuildingType*);

int randInt(int, int);          // Generates a pseudorandom integer between the parameters
float randFloat(float, float);  // Generates a pseudorandom float between the parameters

void cleanUpEvidence(EvidenceListType*);
void cleanUpEvidenceList(EvidenceListType*);

void cleanUpHunter(HunterType*);
void cleanUpHunterArray(HunterArrayType*);

void cleanUpGhost(GhostType*);

void cleanUpRoom(RoomType*);

void cleanUpBuilding(BuildingType);

void* startHunter(void*);
void* startGhost(void*);


