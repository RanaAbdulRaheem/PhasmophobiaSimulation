#include "defs.h"
#include "stdio.h"
#include "stdlib.h"
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>



// BuildingType building;
void* startGhost(void* args){
  GhostAction* action = (GhostAction*) args;
  while(1){
    usleep(20);
    sem_wait(&(action->ghost->room->mutex));
    int res = ghostAction(action->ghost, action->building);
    sem_post(&(action->ghost->room->mutex));
    if(res == 0){
      break;
    }
  }
}

void* startHunter(void* args){
  ActionType* action = (ActionType*) args;
  while(hunterAction(action->hunter, action->ghost) != 0);
  while(1){
    usleep(20);
    sem_wait(&(action->hunter->currRoom->mutex));
    int res = hunterAction(action->hunter, action->ghost);
    sem_post(&(action->hunter->currRoom->mutex));
    if(res == 0){
      removeHunter(action->hunter->currRoom->hunters, action->hunter);
      break;
    }
  }
}

int main(int argc, char *argv[])
{
  // Initialize a random seed for the random number generators
    // srand(time(NULL));

    char first[MAX_STR];
    char second[MAX_STR];
    char third[MAX_STR];
    char fourth[MAX_STR];
    char* hunterName[] = {first, second, third, fourth};

    for(int i = 0; i < 4; i++){
      printf("Enter the name of the Hunter: ");
      fgets(hunterName[i], sizeof(hunterName[i]), stdin);
      int length = strlen(hunterName[i]);
      hunterName[i][length - 1] = '\0';
    }

    BuildingType building;
    initBuilding(&building);
    populateRooms(&building);

    HunterType* firstHunter;
    HunterType* secondHunter;
    HunterType* thirdHunter;
    HunterType* fourthHunter;

    EvidenceClassType firstType = EMF;
    EvidenceClassType secondType = TEMPERATURE;
    EvidenceClassType thirdType  = FINGERPRINTS;
    EvidenceClassType fourthType = SOUND;

    initHunter(building.rooms->head->room, firstType, hunterName[0], &firstHunter);
    initHunter(building.rooms->head->room, secondType, hunterName[1], &secondHunter);
    initHunter(building.rooms->head->room, thirdType, hunterName[2], &thirdHunter);
    initHunter(building.rooms->head->room, fourthType, hunterName[3], &fourthHunter);
    
    addHunter(building.rooms->head->room->hunters, firstHunter);
    addHunter(building.rooms->head->room->hunters, secondHunter);
    addHunter(building.rooms->head->room->hunters, thirdHunter);
    addHunter(building.rooms->head->room->hunters, fourthHunter);

    addHunter(building.hunters, firstHunter);
    addHunter(building.hunters, secondHunter);
    addHunter(building.hunters, thirdHunter);
    addHunter(building.hunters, fourthHunter);

    pthread_t thread [5];

    ActionType* action1 = malloc(sizeof(ActionType));
    ActionType* action2 = malloc(sizeof(ActionType));
    ActionType* action3 = malloc(sizeof(ActionType));
    ActionType* action4 = malloc(sizeof(ActionType));

    GhostAction* gAction = malloc(sizeof(GhostAction));
    gAction->building = building;
    gAction->ghost = building.ghost;

    action1->ghost = building.ghost;
    action1->hunter = firstHunter;

    action2->ghost = building.ghost;
    action2->hunter = secondHunter;

    action3->ghost = building.ghost;
    action3->hunter = thirdHunter;

    action4->ghost = building.ghost;
    action4->hunter = fourthHunter;

    for(int i = 0; i < 5; i++){
      if(i == 0){
        pthread_create(thread+i, NULL, startGhost, gAction);
      }
      else if(i == 1){
        pthread_create(thread + i, NULL, startHunter, action1);
      }
      else if(i == 2){
        pthread_create(thread + i, NULL, startHunter, action2);
      }
      else if(i == 3){
        pthread_create(thread + i, NULL, startHunter, action3);
      }
      else{
        pthread_create(thread + i, NULL, startHunter, action4);
      }
    }

    for(int i = 0; i < 5; i++){
      pthread_join(thread[i], NULL);
    }
    printf("RESULTS:\n");
    int res = 0;
    for(int i = 0; i < 4; i++){
      if(building.hunters->elements[i]->fear >= 100){
        printf("%s have left the building.\n", building.hunters->elements[i]->name);
        res ++;
      }
    }
    if(res == 4){
      printGhostType(building.ghost);
      printf(" has won!\n");
    }
    else{
      printf("The Hunters have won.\n");
    }
    free(action1);
    free(action2);
    free(action3);
    free(action4);
    cleanUpBuilding(building);
    return 0;
}


/*
  Function:  randInt
  Purpose:   returns a pseudo randomly generated number, 
             in the range min to (max - 1), inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [min, max-1) 
*/
int randInt(int min, int max)
{
    return rand() % (max - min) + min;
}

/*
  Function:  randFloat
  Purpose:   returns a pseudo randomly generated number, 
             in the range min to max, inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [0, max-1) 
*/

float randFloat(float a, float b) {
    // Get a percentage between rand() and the maximum
    float random = ((float) rand()) / (float) RAND_MAX;
    // Scale it to the range we want, and shift it
    return random * (b - a) + a;
}
