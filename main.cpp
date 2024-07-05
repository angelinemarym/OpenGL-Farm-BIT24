#include<GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include "SOIL.h"
#define tambase 0.1
#define pi 3.141592
void showCharacter();
void createAutoAnimation();
void createEnvironment();
void draw();
void createAnimationWalk(double x, double z);
void createPlantAnimation();
void controlFloorPlan(int a);

//Animation Queue------------------------------------
typedef struct {
    double x;
    double y;
    double z;
    double time;
    float width;
    float height;
    float width2;
    float height2;
    float ray;
} tPoint;

typedef struct {
    float top;
    float bottom;
    float topSide;
    float topSpin;
} arm;

typedef struct {
    float top;
    float bottom;
    float topSide;
} leg;

typedef float hand;
typedef float foot;


typedef struct {
    arm rightArm;
    arm leftArm;
    leg rightLeg;
    leg leftLeg;
    float head;
    hand rightHandSide;
    hand leftHandSide;
    hand rightHandUpDown;
    hand leftHandUpDown;
    foot rightFoot;
    foot leftFoot;
    float headSide;
    float headUpDown;
    float angTrunk;
    int stop;
    tPoint position;

}humanoid;

typedef humanoid farmer;

typedef struct tCell {
    farmer item;
    struct tCell* next;
}tCell;

typedef struct {
    tCell* initial;
    tCell* last;
    tCell* current;
}tQueueAnimate;

void startAnimate(tQueueAnimate* queue) {
    queue->initial = NULL;
    queue->last = NULL;
    queue->current = NULL;
}
int inexistencetAnimation(tQueueAnimate* queue, farmer p) {
    if (queue->initial == NULL) {
        queue->initial = (tCell*)malloc(sizeof(tCell));
        if (queue->initial == NULL)
            return 0;
        queue->initial->item = p;
        queue->initial->next = NULL;
        queue->last = queue->initial;
        queue->current = queue->initial;
        return 1;
    }
    else {
        queue->last->next = (tCell*)malloc(sizeof(tCell));
        if (queue->last->next == NULL)
            return 0;
        queue->last = queue->last->next;
        queue->last->next = NULL;
        queue->last->item = p;
        return 1;
    }
}

farmer removeAnimation(tQueueAnimate* queue) {
    if (queue->initial != NULL) {
        farmer aux;
        if (queue->initial == queue->last) {
            aux = queue->initial->item;
            free(queue->initial);
            queue->initial = NULL;
            queue->last = NULL;
            queue->current = NULL;
            return aux;
        }
        else {
            aux = queue->initial->item;
            tCell* aux2;
            aux2 = queue->initial;
            queue->initial = queue->initial->next;
            free(aux2);
            queue->current = queue->initial;
            return aux;
        }

    }

}

void turnOffAnimation(tQueueAnimate* queue) {
    tCell* aux;
    if (queue->initial != NULL) {
        while (queue->initial->next != NULL) {
            aux = queue->initial;
            queue->initial = queue->initial->next;
            free(aux);
        }
        free(queue->initial);
        queue->initial = NULL;
        queue->last = NULL;
        queue->current = NULL;
    }
}

farmer nextQueueAnimation(tQueueAnimate* queue) {
    farmer p;p.stop = 1;
    p.rightArm.top = 0;
    p.rightArm.bottom = 0;
    p.headUpDown = 0;
    p.headSide = 0;
    p.rightHandSide = 0;
    p.angTrunk = 0;
    p.rightFoot = 0;
    p.rightLeg.bottom = 0;
    p.leftLeg.top = 0;
    p.leftLeg.bottom = 0;
    p.leftHandSide = 0;
    p.rightHandUpDown = 0;
    p.leftHandUpDown = 0;
    p.rightLeg.topSide = 0;
    p.leftLeg.topSide = 0;
    p.rightArm.topSpin = 0;
    p.rightArm.topSide = 0;
    p.leftArm.topSpin = 0;
    p.leftArm.topSide = 0;
    p.rightLeg.top = 0;
    p.leftArm.bottom = 0;
    p.rightArm.bottom = 0;
    p.rightArm.top = 0;
    p.leftArm.top = 0;
    p.leftFoot = 0;
    p.position.x = 0;
    p.position.y = 0;
    p.position.z = 0;
    p.position.time = 0;
    if (queue->current != NULL) {
        tCell* aux;
        aux = queue->current;
        queue->current = queue->current->next;
        return aux->item;
    }
    else {
        return p;
    }
}
void goBackToInitialAnimation(tQueueAnimate* queue) {
    queue->current = queue->initial;
}
//----------------------------------------


//queue para os clicks do mause------------------------------------


typedef tPoint tItem;

typedef struct cell {
    tItem item;
    struct cell* next;
}cell;

typedef struct {
    cell* initial;
    cell* last;
    cell* current;
    cell* current2;
}tQueue;

void start(tQueue* queue) {
    queue->initial = NULL;
    queue->last = NULL;
    queue->current = NULL;
    queue->current2 = NULL;
}
int inexistencet(tQueue* queue, tItem p) {
    if (queue->initial == NULL) {
        queue->initial = (cell*)malloc(sizeof(cell));
        if (queue->initial == NULL)
            return 0;
        queue->initial->item = p;
        queue->initial->next = NULL;
        queue->last = queue->initial;
        queue->current = queue->initial;
        queue->current2 = queue->initial;
        return 1;
    }
    else {
        queue->last->next = (cell*)malloc(sizeof(cell));
        if (queue->last->next == NULL)
            return 0;
        queue->last = queue->last->next;
        queue->last->next = NULL;
        queue->last->item = p;
        return 1;
    }
}

tItem remove(tQueue* queue) {
    if (queue->initial != NULL) {
        tItem aux;
        if (queue->initial == queue->last) {
            aux = queue->initial->item;
            free(queue->initial);
            queue->initial = NULL;
            queue->last = NULL;
            queue->current = NULL;
            queue->current2 = NULL;
            return aux;
        }
        else {
            aux = queue->initial->item;
            cell* aux2;
            aux2 = queue->initial;
            queue->initial = queue->initial->next;
            free(aux2);
            queue->current = queue->initial;
            queue->current2 = queue->initial;
            return aux;
        }

    }

}

void deleteQueue(tQueue* queue) {
    cell* aux;
    if (queue->initial != NULL) {
        while (queue->initial->next != NULL) {
            aux = queue->initial;
            queue->initial = queue->initial->next;
            free(aux);
        }
        free(queue->initial);
        queue->initial = NULL;
        queue->last = NULL;
        queue->current = NULL;
        queue->current2 = NULL;
    }
}

tItem nextQueue(tQueue* queue) {
    tItem p;p.time = -1;
    if (queue->current != NULL) {
        cell* aux;
        aux = queue->current;
        queue->current = queue->current->next;
        return aux->item;
    }
    else {
        return p;
    }
}

tItem* nextCurrentQueue2(tQueue* queue) {
    if (queue->current2 != NULL) {
        cell* aux;
        aux = queue->current2;
        queue->current2 = queue->current2->next;
        return &aux->item;
    }
    else {
        return NULL;
    }
}

tItem currentQueue(tQueue* queue) {
    if (queue->current != NULL) {
        return queue->current->item;
    }
    else {
        tItem p;p.time = -1;
        return p;
    }
}

tItem* current2Queue(tQueue* queue) {
    if (queue->current2 != NULL) {
        return &queue->current2->item;
    }
    else {
        return NULL;
    }

}

void goBackToInitialQueue(tQueue* queue) {
    queue->current = queue->initial;
}

void goBackToCurrentInitialQueue2(tQueue* queue) {
    queue->current2 = queue->initial;
}

//----------------------------------------

// humanoid

humanoid existence;

// camera
double za, xa, ya, cxa, cza, theta;

float angy, angx;

// menu
int action;

//window

int mainWindow, subWindow;
//position light
GLfloat position[] = { 10, 50 ,20, 0.0 };
GLfloat position1[] = { 0, 50 ,10 , 0.0 };

//draw hoe ?
int showHoe;

//gameStatus
int autoAnimationStatus;
int gameStatus;
// autoAnimation

tQueueAnimate autoAnimate;
tQueueAnimate walk;
tQueueAnimate plant;

//itens do game

tQueue trees, trees2, soils, mousePosition;

//------------------------------------------------
void start3() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    existence.rightArm.top = 0;
    existence.rightArm.bottom = 0;
    existence.headUpDown = 0;
    existence.headSide = 0;
    existence.rightHandSide = 0;
    existence.angTrunk = 0;
    existence.rightFoot = 0;
    existence.leftFoot = 0;
    existence.rightLeg.bottom = 0;
    existence.leftLeg.top = 0;
    existence.leftLeg.bottom = 0;
    existence.leftHandSide = 0;
    existence.rightHandUpDown = 0;
    existence.leftHandUpDown = 0;
    existence.rightLeg.topSide = 0;
    existence.leftLeg.topSide = 0;
    existence.rightArm.topSpin = 0;
    existence.rightArm.topSide = 0;
    existence.leftArm.topSpin = 0;
    existence.leftArm.topSide = 0;
    existence.rightLeg.top = 0;
    existence.leftArm.bottom = 0;
    existence.rightArm.bottom = 0;
    existence.stop = 0;
    existence.rightArm.top = 0;
    existence.leftArm.top = 0;

    existence.position.y = 0;

    position[0] = 10;
    position[1] = 50;
    position[2] = 20;
    position1[0] = 0;
    position1[1] = 50;
    position1[2] = 10;
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT1, GL_POSITION, position1);
    //existence.position.z=0;
    existence.position.time = 0;
    showHoe = 0;
    gameStatus = 0;
    angy = 0;
    angx = 0;
    action = 0;

}
void start2() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    existence.rightArm.top = 0;
    existence.rightArm.bottom = 0;
    existence.headUpDown = 0;
    existence.headSide = 0;
    existence.rightHandSide = 0;
    existence.angTrunk = 0;
    existence.rightFoot = 0;
    existence.leftFoot = 0;
    existence.rightLeg.bottom = 0;
    existence.leftLeg.top = 0;
    existence.leftLeg.bottom = 0;
    existence.leftHandSide = 0;
    existence.rightHandUpDown = 0;
    existence.leftHandUpDown = 0;
    existence.rightLeg.topSide = 0;
    existence.leftLeg.topSide = 0;
    existence.rightArm.topSpin = 0;
    existence.rightArm.topSide = 0;
    existence.leftArm.topSpin = 0;
    existence.leftArm.topSide = 0;
    existence.rightLeg.top = 0;
    existence.leftArm.bottom = 0;
    existence.rightArm.bottom = 0;
    existence.stop = 0;
    existence.rightArm.top = 0;
    existence.leftArm.top = 0;
    existence.position.ray = 0;
    existence.position.x = 0;
    existence.position.y = 0;
    existence.position.z = 0;
    existence.position.time = 0;
    existence.position.width2 = 0;
    showHoe = 0;
    gameStatus = 0;
    angy = 0;
    angx = 0;
    position[0] = 10;
    position[1] = 50;
    position[2] = -20;
    position1[0] = 0;
    position1[1] = 50;
    position1[2] = -10;
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT1, GL_POSITION, position1);
    action = 0;
    za = -2;
    action = 0;
    xa = 1;ya = 0;cxa = 0;cza = 0, theta = -26.565051 * pi / 180.0;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(xa, 0, za, cxa, 0, cza, 0, 1, 0);
}

int load6Textures(char* nome, GLuint* texture);
int loadUniqueTexture(char* nome, GLuint* texture);
//declaration of the variables that will store the texture identifiers;
GLuint textFarmer[6];
GLuint textFloor[1];
GLuint textFarmerTrunk[6];
GLuint textFarmerArm[1];
GLuint textFarmerLeg[1];
GLuint textFarmerFoot[1];
GLuint textFarmerHead[1];
GLuint textGround[1];
GLuint textWallHouse[1];
GLuint textRoofHouse[1];

void start() {
    start(&trees);
    start(&trees2);
    start(&soils);
    start(&mousePosition);

    startAnimate(&autoAnimate);
    startAnimate(&walk);
    startAnimate(&plant);

    char texture1[] = "farmerhead";
    char texture2[] = "farmtrunk";
    char texture3[] = "textchao";
    char texture4[] = "roofhouse";
    char texture5[] = "wallhouse";
    char texture6[] = "farmerleg";
    char texture7[] = "farmerarm";
    char texture8[] = "farmerfoot";
    char texture9[] = "farmerhead3";
    char texture10[] = "stonecutter";

    glClearColor(0.0, 0.0, 0.0, 0.0);
    load6Textures(texture1, textFarmer);
    load6Textures(texture2, textFarmerTrunk);
    loadUniqueTexture(texture3, textFloor);
    loadUniqueTexture(texture4, textRoofHouse);
    loadUniqueTexture(texture5, textWallHouse);
    loadUniqueTexture(texture6, textFarmerLeg);
    loadUniqueTexture(texture7, textFarmerArm);
    loadUniqueTexture(texture8, textFarmerFoot);
    loadUniqueTexture(texture9, textFarmerHead);
    loadUniqueTexture(texture10, textGround);
    autoAnimationStatus = 0;
    existence.rightArm.top = 0;
    existence.rightArm.bottom = 0;
    existence.headUpDown = 0;
    existence.headSide = 0;
    existence.rightHandSide = 0;
    existence.angTrunk = 0;
    existence.rightFoot = 0;
    existence.rightLeg.bottom = 0;
    existence.leftLeg.top = 0;
    existence.leftLeg.bottom = 0;
    existence.leftHandSide = 0;
    existence.rightHandUpDown = 0;
    existence.leftHandUpDown = 0;
    existence.rightLeg.topSide = 0;
    existence.leftLeg.topSide = 0;
    existence.rightArm.topSpin = 0;
    existence.rightArm.topSide = 0;
    existence.leftArm.topSpin = 0;
    existence.leftArm.topSide = 0;
    existence.rightLeg.top = 0;
    existence.leftArm.bottom = 0;
    existence.rightArm.bottom = 0;
    existence.stop = 0;
    existence.rightArm.top = 0;
    existence.leftArm.top = 0;

    existence.position.x = 0;
    existence.position.y = 0;
    existence.position.z = 0;
    existence.position.time = 0;
    showHoe = 0;
    gameStatus = 0;
    angy = 0;
    angx = 0;
    za = -2;
    action = 0;
    xa = 1;ya = 0;cxa = 0;cza = 0, theta = -26.565051 * pi / 180.0;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(xa, 0, za, cxa, 0, cza, 0, 1, 0);
    createEnvironment();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);

}
void cubeWith6Textures(float size, GLuint texture[]) {
    static GLfloat normal[6][3] =
    {
      {-1.0, 0.0, 0.0},
      {0.0, 1.0, 0.0},
      {1.0, 0.0, 0.0},
      {0.0, -1.0, 0.0},
      {0.0, 0.0, 1.0},
      {0.0, 0.0, -1.0}
    };
    static GLint faces[6][4] =
    {
      {1, 2, 3, 0},
      {6, 7, 3, 2},
      {5, 6, 7, 4},
      {5, 4, 0, 1},
      {5, 6, 2, 1},
      {4, 7, 3, 0}
    };
    GLfloat v[8][3];
    GLint i;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    for (i = 5; i >= 0; i--) {
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glBegin(GL_QUADS);
        glNormal3fv(&normal[i][0]);
        glTexCoord2f(1.0, 0.0);glVertex3fv(&v[faces[i][0]][0]);
        glTexCoord2f(1.0, 1.0);glVertex3fv(&v[faces[i][1]][0]);
        glTexCoord2f(0.0, 1.0);glVertex3fv(&v[faces[i][2]][0]);
        glTexCoord2f(0.0, 0.0);glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
    }
    glBindTexture(GL_TEXTURE_2D, 0);

}
void floorWithTexture(float size, GLuint texture[]) {
    static GLfloat normal[6][3] =
    {
      {-1.0, 0.0, 0.0},
      {0.0, 1.0, 0.0},
      {1.0, 0.0, 0.0},
      {0.0, -1.0, 0.0},
      {0.0, 0.0, 1.0},
      {0.0, 0.0, -1.0}
    };
    static GLint faces[6][4] =
    {
      {1, 2, 3, 0},
      {6, 7, 3, 2},
      {5, 6, 7, 4},
      {5, 4, 0, 1},
      {5, 6, 2, 1},
      {4, 7, 3, 0}
    };
    GLfloat v[8][3];
    GLint i;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    for (i = 5; i >= 0; i--) {
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glBegin(GL_QUADS);
        glNormal3fv(&normal[i][0]);
        glTexCoord2f(50.0, 0.0);glVertex3fv(&v[faces[i][0]][0]);
        glTexCoord2f(50.0, 50.0);glVertex3fv(&v[faces[i][1]][0]);
        glTexCoord2f(0.0, 50.0);glVertex3fv(&v[faces[i][2]][0]);
        glTexCoord2f(0.0, 0.0);glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
    }
    glBindTexture(GL_TEXTURE_2D, 0);

}

void cubeWithUniqueTexture(float size, GLuint texture[]) {
    static GLfloat normal[6][3] =
    {
      {-1.0, 0.0, 0.0},
      {0.0, 1.0, 0.0},
      {1.0, 0.0, 0.0},
      {0.0, -1.0, 0.0},
      {0.0, 0.0, 1.0},
      {0.0, 0.0, -1.0}
    };
    static GLint faces[6][4] =
    {
      {1, 2, 3, 0},
      {6, 7, 3, 2},
      {5, 6, 7, 4},
      {5, 4, 0, 1},
      {5, 6, 2, 1},
      {4, 7, 3, 0}
    };
    GLfloat v[8][3];
    GLint i;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    for (i = 5; i >= 0; i--) {
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glBegin(GL_QUADS);
        glNormal3fv(&normal[i][0]);
        glTexCoord2f(1.0, 0.0);glVertex3fv(&v[faces[i][0]][0]);
        glTexCoord2f(1.0, 1.0);glVertex3fv(&v[faces[i][1]][0]);
        glTexCoord2f(0.0, 1.0);glVertex3fv(&v[faces[i][2]][0]);
        glTexCoord2f(0.0, 0.0);glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
    }
    glBindTexture(GL_TEXTURE_2D, 0);

}

int load6Textures(char* nome, GLuint* texture) {
    int i = 0;
    char format[] = ".png";
    char* ext = format;
    char num[] = "0";
    char result[200];

    for (i = 0;i < 6;i++) {
        strcpy_s(result, "textures\\");
        strcat_s(result, nome);
        num[0] = i + 48;
        strcat_s(result, num);
        strcat_s(result, ext);
        texture[i] = SOIL_load_OGL_texture(result, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y); //loads texture with a function from the SOIL library

        if (texture[i] == 0)
            return 0;

    }

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return 1;
}

int loadUniqueTexture(char* nome, GLuint* texture) {

    char format[] = ".png";
    char* ext = format;
    char num[] = "0";
    char result[200];

    strcpy_s(result, "textures\\");
    strcat_s(result, nome);
    strcat_s(result, ext);
    texture[0] = SOIL_load_OGL_texture(result, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y); // loads texture with a function from the SOIL library

    if (texture[0] == 0)
        return 0;



    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return 1;
}

void drawHumanoid() {
    glColor3f(1, 1, 1);
    glPushMatrix();
    glColor3f(1, 1, 1);
    showHoe = existence.position.width2;
    glTranslatef(existence.position.x * tambase, existence.position.y * tambase, existence.position.z * tambase);
    glRotatef(existence.position.ray, 0, 1, 0);
    glPushMatrix();
    glTranslatef(0, -3 * tambase, 0);
    glRotatef(-existence.angTrunk, 1, 0, 0);
    glTranslatef(0, 3 * tambase, 0);
    glPushMatrix();
    //trunk

    glScalef(3, 6, 1);
    cubeWith6Textures(tambase, textFarmerTrunk);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -0.375 * tambase, 0);
    glTranslatef(1.5 * tambase, 3 * tambase, 0);
    glColor3f(0.3607, 0.2, 0.0902);
    glutSolidSphere(0.375 * tambase, 20, 20);

    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, -0.375 * tambase, 0);
    glTranslatef(-1.5 * tambase, 3 * tambase, 0);

    glutSolidSphere(0.375 * tambase, 20, 20);
    glPopMatrix();
    glColor3f(1, 1, 1);
    //rightArm
    glPushMatrix();
    glTranslatef(0, -0.375 * tambase, 0);
    glTranslatef(1.75 * tambase, 3 * tambase, 0);
    glRotatef(existence.rightArm.topSpin, 0, 1, 0);
    glRotatef(existence.rightArm.topSide, 0, 0, 1);
    glRotatef(existence.rightArm.top, 1, 0, 0);
    glTranslatef(0, 0.375 * tambase, 0);

    glPushMatrix();
    glTranslatef(0.375 * tambase, -1.5 * tambase, 0);
    glScalef(0.75, 3, 0.75);
    cubeWithUniqueTexture(tambase, textFarmerArm);
    glPopMatrix();
    //sphere
    glPushMatrix();
    glColor3f(0.3607, 0.2, 0.0902);
    glTranslatef(0.375 * tambase, -3 * tambase, 0);
    glutSolidSphere(0.375 * tambase, 20, 20);
    glPopMatrix();
    glColor3f(1, 1, 1);

    glPushMatrix();
    glTranslatef(0, -3 * tambase, 0);
    //rotate in z put here
    glRotatef(existence.rightArm.bottom, 1, 0, 0);
    glPushMatrix();
    glTranslatef(0.375 * tambase, -1.5 * tambase, 0);
    glScalef(0.75, 3, 0.75);
    cubeWithUniqueTexture(tambase, textFarmerArm);
    glPopMatrix();

    glTranslatef(0.1875 * tambase, -3 * tambase, 0);
    glRotatef(existence.rightHandSide, 0, 0, 1);
    glRotatef(existence.rightHandUpDown, 1, 0, 0);
    glPushMatrix();

    glTranslatef(0.125 * tambase, -0.5 * tambase, 0);
    glScalef(0.25, 1, 1);
    cubeWithUniqueTexture(tambase, textFarmerHead);
    glPopMatrix();

    //draw hoe
    if (showHoe == 1) {
        glTranslatef(0, -0.7 * tambase, 0);
        glTranslatef(-0.1 * tambase, 0.3 * tambase, -1.2 * tambase);
        glPushMatrix();
        glColor3f(0.5882, 0.2941, 0);
        glRotatef(90, 1, 0, 0);
        glScalef(0.35, 4, 0.5);
        glutSolidCube(tambase);
        glPopMatrix();

        glPushMatrix();
        glColor3f(0.1254, 0.1098, 0.12156);
        glTranslatef(0, -0.25 * tambase, -2 * tambase);
        glTranslatef(0, 0, -0.05 * tambase);
        glScalef(1.5, 1, 0.1);
        glutSolidCube(tambase);

        glPopMatrix();
    }
    glColor3f(1, 1, 1);

    glPopMatrix();


    glPopMatrix();


    //left arm
    glPushMatrix();
    glTranslatef(0, -0.375 * tambase, 0);
    glTranslatef(-1.75 * tambase, 3 * tambase, 0);
    glRotatef(existence.leftArm.topSpin, 0, 1, 0);
    glRotatef(existence.leftArm.topSide, 0, 0, 1);
    glRotatef(existence.leftArm.top, 1, 0, 0);
    glTranslatef(0, 0.375 * tambase, 0);

    glPushMatrix();
    glTranslatef(-0.375 * tambase, -1.5 * tambase, 0);
    glScalef(0.75, 3, 0.75);
    cubeWithUniqueTexture(tambase, textFarmerArm);
    glPopMatrix();
    //sphere
    glPushMatrix();
    glColor3f(0.3607, 0.2, 0.0902);
    glTranslatef(-0.375 * tambase, -3 * tambase, 0);
    glutSolidSphere(0.375 * tambase, 20, 20);
    glPopMatrix();
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(0, -3 * tambase, 0);
    //rotate in z put here
    glRotatef(existence.leftArm.bottom, 1, 0, 0);
    glPushMatrix();
    glTranslatef(-0.375 * tambase, -1.5 * tambase, 0);
    glScalef(0.75, 3, 0.75);
    cubeWithUniqueTexture(tambase, textFarmerArm);
    glPopMatrix();

    glTranslatef(-0.1875 * tambase, -3 * tambase, 0);
    glRotatef(existence.leftHandSide, 0, 0, 1);
    glRotatef(existence.leftHandUpDown, 1, 0, 0);
    glPushMatrix();

    glTranslatef(-0.125 * tambase, -0.5 * tambase, 0);
    glScalef(0.25, 1, 1);
    cubeWithUniqueTexture(tambase, textFarmerHead);
    glPopMatrix();
    if (showHoe == 2) {
        glTranslatef(0, -0.7 * tambase, 0);
        glTranslatef(+0.1 * tambase, 0.3 * tambase, -1.2 * tambase);
        glPushMatrix();
        glColor3f(0.5882, 0.2941, 0);
        glRotatef(90, 1, 0, 0);
        glScalef(0.35, 4, 0.5);
        glutSolidCube(tambase);
        glPopMatrix();

        glPushMatrix();
        glColor3f(0.1254, 0.1098, 0.12156);
        glTranslatef(0, -0.25 * tambase, -2 * tambase);
        glTranslatef(0, 0, -0.05 * tambase);
        glScalef(1.5, 1, 0.1);
        glutSolidCube(tambase);

        glPopMatrix();
    }
    glColor3f(1, 1, 1);

    glPopMatrix();


    glPopMatrix();
    //head

    glPushMatrix();
    glTranslatef(0, 3.6 * tambase, 0);
    glRotatef(existence.headSide, 0, 1, 0);
    glRotatef(existence.headUpDown, 1, 0, 0);
    glTranslatef(0, 1 * tambase, -0.2 * tambase);

    cubeWith6Textures(0.2, textFarmer);

    glPopMatrix();

    //foot

    glPushMatrix();
    glTranslatef(0, 3 * tambase, 0);
    glTranslatef(0, 0.35 * tambase, 0);
    glScalef(0.5, 0.7, 0.5);
    cubeWithUniqueTexture(tambase, textFarmerHead);
    glPopMatrix();

    glPopMatrix();


    glPushMatrix();
    glColor3f(0.3607, 0.2, 0.0902);
    glTranslatef(-tambase, -3 * tambase, 0);
    glutSolidSphere(0.5 * tambase, 20, 20);
    glPopMatrix();
    glColor3f(1, 1, 1);
    //right leg
    glPushMatrix();
    glTranslatef(1.5 * tambase, -3 * tambase, 0);
    glRotatef(existence.rightLeg.topSide, 0, 0, 1);
    glRotatef(existence.rightLeg.top, 1, 0, 0);
    glPushMatrix();
    glTranslatef(-0.5 * tambase, -2 * tambase, 0);
    glScalef(1, 4, 1);
    cubeWithUniqueTexture(tambase, textFarmerLeg);
    glPopMatrix();
    //sphere
    glPushMatrix();
    glColor3f(0.3607, 0.2, 0.0902);
    glTranslatef(-0.5 * tambase, -4 * tambase, 0);
    glutSolidSphere(0.5 * tambase, 20, 20);
    glPopMatrix();
    glColor3f(1, 1, 1);

    glPushMatrix();
    glTranslatef(0, -4 * tambase, 0);

    glRotatef(existence.rightLeg.bottom, 1, 0, 0);
    glPushMatrix();
    glTranslatef(-0.5 * tambase, -2 * tambase, 0);
    glScalef(1, 4, 1);
    cubeWithUniqueTexture(tambase, textFarmerLeg);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -4 * tambase, 0);
    glRotatef(existence.rightFoot, 1, 0, 0);
    glTranslatef(-0.5 * tambase, -0.25 * tambase, -0.5 * tambase);
    glScalef(1, 0.5, 2);
    cubeWithUniqueTexture(tambase, textFarmerFoot);
    glPopMatrix();

    glPopMatrix();


    glPopMatrix();





    //left leg
    glPushMatrix();
    glColor3f(0.3607, 0.2, 0.0902);
    glTranslatef(tambase, -3 * tambase, 0);
    glutSolidSphere(0.5 * tambase, 20, 20);
    glPopMatrix();
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(-1.5 * tambase, -3 * tambase, 0);
    glRotatef(existence.leftLeg.topSide, 0, 0, 1);
    glRotatef(existence.leftLeg.top, 1, 0, 0);
    glPushMatrix();
    glTranslatef(+0.5 * tambase, -2 * tambase, 0);
    glScalef(1, 4, 1);
    cubeWithUniqueTexture(tambase, textFarmerLeg);
    glPopMatrix();
    //sphere
    glPushMatrix();
    glColor3f(0.3607, 0.2, 0.0902);
    glTranslatef(+0.5 * tambase, -4 * tambase, 0);
    glutSolidSphere(0.5 * tambase, 20, 20);
    glPopMatrix();
    glColor3f(1, 1, 1);

    glPushMatrix();
    glTranslatef(0, -4 * tambase, 0);

    glRotatef(existence.leftLeg.bottom, 1, 0, 0);
    glPushMatrix();
    glTranslatef(+0.5 * tambase, -2 * tambase, 0);
    glScalef(1, 4, 1);
    cubeWithUniqueTexture(tambase, textFarmerLeg);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -4 * tambase, 0);
    glRotatef(existence.leftFoot, 1, 0, 0);
    glTranslatef(+0.5 * tambase, -0.25 * tambase, -0.5 * tambase);
    glScalef(1, 0.5, 2);
    cubeWithUniqueTexture(tambase, textFarmerFoot);
    glPopMatrix();

    glPopMatrix();


    glPopMatrix();





    glPopMatrix();
}

void drawTree(tItem parameters) {

    glPushMatrix();
    glTranslatef(parameters.x, parameters.y, parameters.z);
    glTranslatef(0, -1.15, 0);
    glTranslatef(0, parameters.height * tambase / 2, 0);
    glPushMatrix();
    //add color
    glColor3f(0.545, 0.2705, 0);
    glScalef(parameters.width, parameters.height, parameters.width);
    glutSolidCube(tambase);
    glPopMatrix();


    glColor3f(0, 1, 0);
    glTranslatef(0, tambase * parameters.height / 2, 0);
    glPushMatrix();

    glScalef(parameters.width2, parameters.height2 / 4, parameters.width2);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(tambase, tambase, 20, 20);
    glPopMatrix();

    glTranslatef(0, parameters.height2 * tambase / 8, 0);
    glPushMatrix();
    glScalef(parameters.width2, parameters.height2 / 2, parameters.width2);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(tambase, tambase, 20, 20);
    glPopMatrix();

    glTranslatef(0, -parameters.height2 * tambase / 3, 0);
    glPushMatrix();
    glScalef(parameters.width2, parameters.height2, parameters.width2);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(tambase, tambase, 20, 20);
    glPopMatrix();

    glPopMatrix();

}

void drawTree2(tItem parameters) {

    glPushMatrix();
    glTranslatef(parameters.x, parameters.y, parameters.z);
    glTranslatef(0, -1.15, 0);
    glTranslatef(0, parameters.height * tambase / 2, 0);
    glPushMatrix();
    //add color
    glColor3f(0.545, 0.2705, 0);
    glScalef(parameters.width, parameters.height, parameters.width);
    glutSolidCube(tambase);
    glPopMatrix();


    glColor3f(0, 1, 0);
    glTranslatef(0, tambase * parameters.height, 0);
    glPushMatrix();

    glScalef(parameters.ray, parameters.ray, parameters.ray);

    glutSolidSphere(tambase, 20, 20);
    glPopMatrix();

    glTranslatef(0, parameters.ray * tambase / 4, 0);
    glPushMatrix();
    glScalef(parameters.ray, parameters.ray, parameters.ray);

    glutSolidSphere(tambase, 20, 20);

    glPopMatrix();

    glTranslatef(0, parameters.ray * tambase / 3, 0);
    glPushMatrix();
    glScalef(parameters.ray, parameters.ray, parameters.ray);

    glutSolidSphere(tambase, 20, 20);
    glPopMatrix();

    glPopMatrix();

}

void drawFloor() {

    glColor3f(0.7, 1, 0.7);
    glPushMatrix();
    glTranslatef(0, -1.16, 0);
    glTranslatef(0, 0.005, 0);
    glScalef(1500, 0.1, 1500);
    floorWithTexture(0.1, textFloor);

    glPopMatrix();
}
void drawSky() {
    glColor3f(0, 0.7490, 1);
    glPushMatrix();
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60);

    glScalef(700, 700, 700);
    glutSolidSphere(tambase, 10, 10);

    glPopMatrix();


}

void drawPlant() {
    glColor3f(0.2, 1, 0.2);
    glPushMatrix();

    glRotatef(rand() % 90, 0, 1, 0);
    glTranslatef(0, -0.95, 0);
    glPushMatrix();

    glScalef(0.35, 4, 0.35);
    glutSolidCube(tambase);
    glPopMatrix();

    glPushMatrix();
    glColor3f((rand() % 10) / 10.0, 0, (rand() % 10) / 10.0);
    glTranslatef(0, 0.2, 0);
    glutSolidSphere(0.0175, 5, 5);
    glPopMatrix();
    glColor3f(0.2, 1, 0.2);

    glTranslatef(0.0525, -0.04, 0);
    glPushMatrix();
    glScalef(1, 0.1, 1);
    glutSolidCube(tambase);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-0.095, 0.015, 0);
    glScalef(1, 0.1, 1);
    glutSolidCube(tambase);
    glPopMatrix();

    glTranslatef(0, 0.2, 0);
    glPushMatrix();
    glScalef(1, 0.1, 1);
    glutSolidCube(tambase);
    glPopMatrix();
    glTranslatef(-0.095, 0.015, 0);
    glPushMatrix();
    glScalef(1, 0.1, 1);
    glutSolidCube(tambase);
    glPopMatrix();



    glPopMatrix();

}

void drawGround(tItem parameter) {

    glColor3f(0.3, 0.2, 0.03);
    glPushMatrix();
    glTranslatef(parameter.x, parameter.y, parameter.z);
    glPushMatrix();
    glTranslatef(0, -1.15, 0);

    glPushMatrix();
    glScalef(parameter.width, 0.5, parameter.width);
    cubeWithUniqueTexture(tambase, textGround);
    glPopMatrix();
    glPopMatrix();
    if (parameter.time == 2)
        drawPlant();

    glPopMatrix();

}



void drawHouse(tItem parameters) {

    glColor3f(0.93334, 0.81176, 0.63137);//cordacasa
    glPushMatrix();
    glTranslatef(parameters.x, parameters.y, parameters.z);
    glPushMatrix();
    glTranslatef(0, -0.05, 0);
    glPushMatrix();
    glTranslatef(0, 0, -0.5);
    glPushMatrix();
    glTranslatef(0, 0, 0.5);
    glScalef(22, 22, 10);
    cubeWithUniqueTexture(tambase, textWallHouse);
    glPopMatrix();

    glTranslatef(-0.55, 0.275, -0.01);
    glPushMatrix();
    glColor3f(0.54118, 0.16863, 0.88627);
    glScalef(6, 6, 0.1);
    glutSolidCube(tambase);
    glPopMatrix();

    glColor3f(0.96078, 0.96078, 0.86275);//put gray
    glTranslatef(0, 0, -0.01);
    glPushMatrix();

    glScalef(0.5, 6, 0.1);
    glutSolidCube(tambase);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glScalef(6, 0.5, 0.1);
    glutSolidCube(tambase);
    glPopMatrix();


    glPopMatrix();

    glColor3f(0.93334, 0.81176, 0.63137);//ropehouse


    glPushMatrix();
    glTranslatef(1.1, 0, 0);
    glPushMatrix();
    glTranslatef(0.01, -0.15, 0);
    glColor3f(0.54509, 0.27843, 0.14902);//door
    glScalef(0.1, 19, 5);
    glutSolidCube(tambase);
    glPopMatrix();
    glColor3f(0.96078, 0.96078, 0.86275);//a silver;
    glTranslatef(0, 0, -0.2);
    glutSolidSphere(0.05, 5, 5);
    glPopMatrix();


    glTranslatef(0, 1.1, 0);
    glColor3f(0.6, 0.3, 0.11765);//roof color
    glPushMatrix();
    glTranslatef(0, 0.5, 0);
    glTranslatef(0, 0, -0.2668);
    glRotatef(27, 1, 0, 0);
    glTranslatef(0, -0.15, 0);
    glScalef(22, 15, 0.1);
    cubeWithUniqueTexture(tambase, textRoofHouse);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0.5, 0);
    glTranslatef(0, 0, +0.2668);
    glRotatef(-27, 1, 0, 0);
    glTranslatef(0, -0.15, 0);
    glScalef(22, 15, 0.1);
    cubeWithUniqueTexture(tambase, textRoofHouse);
    glPopMatrix();

    glColor3f(0.93334, 0.81176, 0.63137);
    glBindTexture(GL_TEXTURE_2D, textWallHouse[0]);
    glPushMatrix();
    glTranslatef(-1.1, 0, 0);
    glBegin(GL_TRIANGLES);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(1.0, 0.0);  glVertex3f(0, 0, 0.5);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0.0, 0.0);  glVertex3f(0, 0, -0.5);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0.5, 1.0);  glVertex3f(0, 1, 0);
    glEnd();


    glPopMatrix();
    glPushMatrix();
    glTranslatef(1.1, 0, 0);
    glBegin(GL_TRIANGLES);
    glNormal3f(1, 0, 0);
    glTexCoord2f(1.0, 0.0);glVertex3f(0, 0, 0.5);
    glNormal3f(1, 0, 0);
    glTexCoord2f(0.0, 0.0);glVertex3f(0, 0, -0.5);
    glNormal3f(1, 0, 0);
    glTexCoord2f(0.5, 1.0);glVertex3f(0, 1, 0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();
}


void  wheelPlant(int a) {
    if (gameStatus == 0) {
        farmer aux;
        aux = nextQueueAnimation(&plant);

        if (aux.stop != 1) {
            existence = aux;
            glutTimerFunc(10, wheelPlant, 0);

            draw();
        }
        else {
            tItem* aux = current2Queue(&mousePosition);
            if (aux != NULL)
                aux->time = 2;
            turnOffAnimation(&plant);
            glutPostRedisplay();
        }

    }
}

void createAnimationWalk(double x, double z) {


    int i;
    humanoid aux;
    aux.rightArm.top = 0;
    aux.rightArm.bottom = 0;
    aux.headUpDown = 0;
    aux.headSide = 0;
    aux.rightHandSide = 0;
    aux.angTrunk = 0;
    aux.rightFoot = 0;
    aux.rightLeg.bottom = 0;
    aux.leftLeg.top = 0;
    aux.leftLeg.bottom = 0;
    aux.leftHandSide = 0;
    aux.rightHandUpDown = 0;
    aux.leftHandUpDown = 0;
    aux.rightLeg.topSide = 0;
    aux.leftLeg.topSide = 0;
    aux.rightArm.topSpin = 0;
    aux.rightArm.topSide = 0;
    aux.leftArm.topSpin = 0;
    aux.leftArm.topSide = 0;
    aux.rightLeg.top = 0;
    aux.leftArm.bottom = 0;
    aux.rightArm.bottom = 0;
    aux.leftFoot = 0;
    aux.rightArm.top = 0;
    aux.leftArm.top = 0;
    aux.stop = 0;
    aux.position.ray = 0;
    aux.position.x = existence.position.x;
    aux.position.y = 0;
    aux.position.z = existence.position.z;
    aux.position.time = 0;

    float frames;

    frames = 12.0;
    int j;


    double deltax = x - existence.position.x * 0.1;
    double deltaxpos = deltax;
    double deltaz = +z - existence.position.z * 0.1;
    double deltazpos = deltaz;
    double startx = existence.position.x;
    double startz = existence.position.z;

    if (deltax < 0)
        deltaxpos = -1 * deltax;


    if (deltaz < 0)
        deltazpos = -1 * deltaz;

    int prevIdx = 0;
    aux.position.z = startz;
    //x-shift
    if (x > existence.position.x * 0.1)
        aux.position.ray = -90;
    else if (x < existence.position.x * 0.1)
        aux.position.ray = 90;
    if (0.364 < deltaxpos) {
        aux.rightLeg.bottom = aux.rightLeg.bottom - 35.0;
        aux.rightArm.top = -5.0;

        for (j = 0;j * 0.7044 + (0.364) < deltaxpos;j++) {
            for (i = 0;i < frames;i++) {

                aux.rightLeg.top = aux.rightLeg.top + 30.0 / frames;

                aux.rightArm.top = aux.rightArm.top - 5.0 / frames;
                aux.leftLeg.top = aux.leftLeg.top - 15 / frames;
                aux.leftFoot = aux.leftFoot + 15 / frames;

                aux.leftArm.top = aux.leftArm.top + 15 / frames;
                aux.leftArm.bottom = aux.leftArm.bottom + 15 / frames;

                //y-shift
                aux.position.y = -8 + 8 * cos(((i + 1) * (15.0 / frames)) * pi / 180.0);

                //x-shift
                if (deltax > 0)
                    aux.position.x = startx + 8 * sin(((i + 1) * (15.0 / frames)) * pi / 180.0);
                else
                    aux.position.x = startx - 8 * sin(((i + 1) * (15.0 / frames)) * pi / 180.0);

                inexistencetAnimation(&walk, aux);
            }
            startx = aux.position.x;
            for (i = 0;i < frames / 2;i++) {
                aux.rightLeg.top = aux.rightLeg.top - 8.725 / (frames / 2);
                aux.rightLeg.bottom = aux.rightLeg.bottom + 13.725 / (frames / 2);
                aux.leftArm.top = aux.leftArm.top - 7.5 / (frames / 2);
                aux.leftArm.bottom = aux.leftArm.bottom - 7.5 / (frames / 2);
                aux.rightArm.top = aux.rightArm.top + 5.0 / (frames / 2);

                inexistencetAnimation(&walk, aux);

            }

            for (i = 0;i < frames;i++) {
                aux.rightLeg.top = aux.rightLeg.top - 21.275 / frames;
                aux.rightLeg.bottom = aux.rightLeg.bottom + 21.274 / frames;
                aux.rightArm.top = aux.rightArm.top + 5.0 / frames;
                if (deltax > 0)
                    aux.position.x = startx + 4 * sin(((i + 1) * (21.275 / frames)) * pi / 180.0);
                else
                    aux.position.x = startx - 4 * sin(((i + 1) * (21.275 / frames)) * pi / 180.0);

                //y-shift
                aux.position.y = -8 + 8 * cos((15.0 - ((i + 1) * (15.0 / frames))) * pi / 180.0);

                aux.leftLeg.top = aux.leftLeg.top + 15 / frames;
                aux.leftLeg.bottom = aux.leftLeg.bottom - 35 / frames;
                aux.leftFoot = aux.leftFoot - 15 / frames;

                aux.leftArm.top = aux.leftArm.top - 12.5 / frames;
                aux.leftArm.bottom = aux.leftArm.bottom - 7.5 / frames;

                inexistencetAnimation(&walk, aux);

            }
            if ((j + 1) * 0.7044 < deltaxpos) {
                startx = aux.position.x;
                for (i = 0;i < frames;i++) {
                    aux.leftLeg.top = aux.leftLeg.top + 30.0 / frames;


                    aux.rightLeg.top = aux.rightLeg.top - 15 / frames;
                    aux.rightFoot = aux.rightFoot + 15 / frames;

                    aux.rightArm.top = aux.rightArm.top + 15 / frames;
                    aux.rightArm.bottom = aux.rightArm.bottom + 15 / frames;
                    aux.leftArm.top = aux.leftArm.top - 5.0 / frames;
                    //y-shift
                    aux.position.y = -8 + 8 * cos(((i + 1) * (15.0 / frames)) * pi / 180.0);

                    //x-shift
                    if (deltax > 0)
                        aux.position.x = startx + 8 * sin(((i + 1) * (15.0 / frames)) * pi / 180.0);
                    else
                        aux.position.x = startx - 8 * sin(((i + 1) * (15.0 / frames)) * pi / 180.0);

                    inexistencetAnimation(&walk, aux);
                }
                startx = aux.position.x;
                for (i = 0;i < frames / 2;i++) {
                    aux.leftLeg.top = aux.leftLeg.top - 8.725 / (frames / 2);
                    aux.leftLeg.bottom = aux.leftLeg.bottom + 13.725 / (frames / 2);
                    aux.rightArm.top = aux.rightArm.top - 7.5 / (frames / 2);
                    aux.rightArm.bottom = aux.rightArm.bottom - 7.5 / (frames / 2);
                    aux.leftArm.top = aux.leftArm.top + 5.0 / (frames / 2);
                    inexistencetAnimation(&walk, aux);

                }
                for (i = 0;i < frames;i++) {
                    aux.leftLeg.top = aux.leftLeg.top - 21.275 / frames;
                    aux.leftLeg.bottom = aux.leftLeg.bottom + 21.274 / frames;
                    aux.leftArm.top = aux.leftArm.top + 5.0 / frames;
                    if (deltax > 0)
                        aux.position.x = startx + 4 * sin(((i + 1) * (21.275 / frames)) * pi / 180.0);
                    else
                        aux.position.x = startx - 4 * sin(((i + 1) * (21.275 / frames)) * pi / 180.0);

                    //y-shift
                    aux.position.y = -8 + 8 * cos((15.0 - ((i + 1) * (15.0 / frames))) * pi / 180.0);

                    aux.rightLeg.top = aux.rightLeg.top + 15 / frames;
                    aux.rightLeg.bottom = aux.rightLeg.bottom - 35 / frames;
                    aux.rightFoot = aux.rightFoot - 15 / frames;

                    aux.rightArm.top = aux.rightArm.top - 12.5 / frames;
                    aux.rightArm.bottom = aux.rightArm.bottom - 7.5 / frames;

                    inexistencetAnimation(&walk, aux);

                }
                startx = aux.position.x;
            }
            else {
                for (i = 0;i < frames;i++) {
                    aux.leftArm.top = aux.leftArm.top + 10 / frames;
                    aux.rightArm.top = aux.rightArm.top - 5 / frames;
                    aux.leftLeg.bottom = aux.leftLeg.bottom + 35 / frames;
                    inexistencetAnimation(&walk, aux);
                }
                prevIdx = 1;
                break;
            }
        }
        if (prevIdx == 0) {
            for (i = 0;i < frames;i++) {

                aux.rightLeg.bottom = aux.rightLeg.bottom + 35 / frames;
                inexistencetAnimation(&walk, aux);

            }
        }
    }

    //-------------------------------------now for z
    prevIdx = 0;
    if (z > existence.position.z * 0.1)
        aux.position.ray = -180;
    else if (z < existence.position.z * 0.1)
        aux.position.ray = 0;
    if (0.364 < deltazpos) {
        aux.rightLeg.bottom = aux.rightLeg.bottom - 35.0;
        aux.rightArm.top = -5.0;
        startz = existence.position.z;
        for (j = 0;j * 0.7044 + (0.364) < deltazpos;j++) {
            for (i = 0;i < frames;i++) {

                aux.rightLeg.top = aux.rightLeg.top + 30.0 / frames;

                aux.rightArm.top = aux.rightArm.top - 5.0 / frames;
                aux.leftLeg.top = aux.leftLeg.top - 15 / frames;
                aux.leftFoot = aux.leftFoot + 15 / frames;

                aux.leftArm.top = aux.leftArm.top + 15 / frames;
                aux.leftArm.bottom = aux.leftArm.bottom + 15 / frames;

                //y-shift
                aux.position.y = -8 + 8 * cos(((i + 1) * (15.0 / frames)) * pi / 180.0);

                //x-shift
                if (deltaz > 0)
                    aux.position.z = startz + 8 * sin(((i + 1) * (15.0 / frames)) * pi / 180.0);
                else
                    aux.position.z = startz - 8 * sin(((i + 1) * (15.0 / frames)) * pi / 180.0);

                inexistencetAnimation(&walk, aux);
            }
            startz = aux.position.z;
            for (i = 0;i < frames / 2;i++) {
                aux.rightLeg.top = aux.rightLeg.top - 8.725 / (frames / 2);
                aux.rightLeg.bottom = aux.rightLeg.bottom + 13.725 / (frames / 2);
                aux.leftArm.top = aux.leftArm.top - 7.5 / (frames / 2);
                aux.leftArm.bottom = aux.leftArm.bottom - 7.5 / (frames / 2);
                aux.rightArm.top = aux.rightArm.top + 5.0 / (frames / 2);

                inexistencetAnimation(&walk, aux);

            }

            for (i = 0;i < frames;i++) {
                aux.rightLeg.top = aux.rightLeg.top - 21.275 / frames;
                aux.rightLeg.bottom = aux.rightLeg.bottom + 21.274 / frames;
                aux.rightArm.top = aux.rightArm.top + 5.0 / frames;
                if (deltaz > 0)
                    aux.position.z = startz + 4 * sin(((i + 1) * (21.275 / frames)) * pi / 180.0);
                else
                    aux.position.z = startz - 4 * sin(((i + 1) * (21.275 / frames)) * pi / 180.0);

                //y-shift
                aux.position.y = -8 + 8 * cos((15.0 - ((i + 1) * (15.0 / frames))) * pi / 180.0);

                aux.leftLeg.top = aux.leftLeg.top + 15 / frames;
                aux.leftLeg.bottom = aux.leftLeg.bottom - 35 / frames;
                aux.leftFoot = aux.leftFoot - 15 / frames;

                aux.leftArm.top = aux.leftArm.top - 12.5 / frames;
                aux.leftArm.bottom = aux.leftArm.bottom - 7.5 / frames;

                inexistencetAnimation(&walk, aux);

            }
            if ((j + 1) * 0.7044 < deltazpos) {
                startz = aux.position.z;
                for (i = 0;i < frames;i++) {
                    aux.leftLeg.top = aux.leftLeg.top + 30.0 / frames;


                    aux.rightLeg.top = aux.rightLeg.top - 15 / frames;
                    aux.rightFoot = aux.rightFoot + 15 / frames;

                    aux.rightArm.top = aux.rightArm.top + 15 / frames;
                    aux.rightArm.bottom = aux.rightArm.bottom + 15 / frames;
                    aux.leftArm.top = aux.leftArm.top - 5.0 / frames;
                    //y-shift
                    aux.position.y = -8 + 8 * cos(((i + 1) * (15.0 / frames)) * pi / 180.0);

                    //x-shift
                    if (deltaz > 0)
                        aux.position.z = startz + 8 * sin(((i + 1) * (15.0 / frames)) * pi / 180.0);
                    else
                        aux.position.z = startz - 8 * sin(((i + 1) * (15.0 / frames)) * pi / 180.0);

                    inexistencetAnimation(&walk, aux);
                }
                startz = aux.position.z;
                for (i = 0;i < frames / 2;i++) {
                    aux.leftLeg.top = aux.leftLeg.top - 8.725 / (frames / 2);
                    aux.leftLeg.bottom = aux.leftLeg.bottom + 13.725 / (frames / 2);
                    aux.rightArm.top = aux.rightArm.top - 7.5 / (frames / 2);
                    aux.rightArm.bottom = aux.rightArm.bottom - 7.5 / (frames / 2);
                    aux.leftArm.top = aux.leftArm.top + 5.0 / (frames / 2);
                    inexistencetAnimation(&walk, aux);

                }
                for (i = 0;i < frames;i++) {
                    aux.leftLeg.top = aux.leftLeg.top - 21.275 / frames;
                    aux.leftLeg.bottom = aux.leftLeg.bottom + 21.274 / frames;
                    aux.leftArm.top = aux.leftArm.top + 5.0 / frames;
                    if (deltaz > 0)
                        aux.position.z = startz + 4 * sin(((i + 1) * (21.275 / frames)) * pi / 180.0);
                    else
                        aux.position.z = startz - 4 * sin(((i + 1) * (21.275 / frames)) * pi / 180.0);

                    //y-shift
                    aux.position.y = -8 + 8 * cos((15.0 - ((i + 1) * (15.0 / frames))) * pi / 180.0);

                    aux.rightLeg.top = aux.rightLeg.top + 15 / frames;
                    aux.rightLeg.bottom = aux.rightLeg.bottom - 35 / frames;
                    aux.rightFoot = aux.rightFoot - 15 / frames;

                    aux.rightArm.top = aux.rightArm.top - 12.5 / frames;
                    aux.rightArm.bottom = aux.rightArm.bottom - 7.5 / frames;

                    inexistencetAnimation(&walk, aux);

                }
                startz = aux.position.z;
            }
            else {
                for (i = 0;i < frames;i++) {
                    aux.leftArm.top = aux.leftArm.top + 10 / frames;
                    aux.rightArm.top = aux.rightArm.top - 5 / frames;
                    aux.leftLeg.bottom = aux.leftLeg.bottom + 35 / frames;
                    inexistencetAnimation(&walk, aux);
                }
                prevIdx = 1;
                break;
            }
        }
        if (prevIdx == 0) {
            for (i = 0;i < frames;i++) {

                aux.rightLeg.bottom = aux.rightLeg.bottom + 35 / frames;
                inexistencetAnimation(&walk, aux);

            }

        }
    }

}
void createPlantAnimation() {
    int i;
    humanoid aux;
    aux.rightArm.top = 0;
    aux.rightArm.bottom = 0;
    aux.headUpDown = 0;
    aux.headSide = 0;
    aux.rightHandSide = 0;
    aux.angTrunk = 0;
    aux.rightFoot = 0;
    aux.rightLeg.bottom = 0;
    aux.leftLeg.top = 0;
    aux.leftLeg.bottom = 0;
    aux.leftHandSide = 0;
    aux.rightHandUpDown = 0;
    aux.leftHandUpDown = 0;
    aux.rightLeg.topSide = 0;
    aux.leftLeg.topSide = 0;
    aux.rightArm.topSpin = 0;
    aux.rightArm.topSide = 0;
    aux.leftArm.topSpin = 0;
    aux.leftArm.topSide = 0;
    aux.rightLeg.top = 0;
    aux.leftArm.bottom = 0;
    aux.rightArm.bottom = 0;
    aux.leftFoot = 0;
    aux.rightArm.top = 0;
    aux.leftArm.top = 0;
    aux.stop = 0;
    aux.position.ray = existence.position.ray;
    aux.position.x = existence.position.x;
    aux.position.y = 0;
    aux.position.z = existence.position.z;
    aux.position.time = 0;

    float frames = 10.0;
    int j;

    for (i = 0;i < frames;i++) {
        //squat
        aux.rightLeg.top = aux.rightLeg.top + 75.0 / frames;
        aux.rightLeg.bottom = aux.rightLeg.bottom - 150.0 / frames;
        aux.rightFoot = aux.rightFoot + 75.0 / frames;
        aux.angTrunk = aux.angTrunk + 15.0 / frames;
        aux.leftArm.top = aux.leftArm.top + 10 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom + 10 / frames;
        //make the translation
        aux.position.y = -8 + 8 * cos((i + 1) * (75.0 / frames) * pi / 180.0);

        aux.leftLeg.top = aux.leftLeg.top + 75.0 / frames;
        aux.leftLeg.bottom = aux.leftLeg.bottom - 150.0 / frames;
        aux.leftFoot = aux.leftFoot + 75.0 / frames;

        //-----------------------------
        aux.rightArm.top = aux.rightArm.top + 10 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom + 90 / frames;


        inexistencetAnimation(&plant, aux);
    }
    inexistencetAnimation(&plant, aux);
    aux.position.width2 = 1;
    inexistencetAnimation(&plant, aux);

    for (j = 0;j < 4;j++) {
        for (i = 0;i < frames;i++) {
            aux.rightArm.bottom = aux.rightArm.bottom - 70 / frames;
            inexistencetAnimation(&plant, aux);
        }
        for (i = 0;i < frames;i++) {
            aux.rightArm.bottom = aux.rightArm.bottom + 70 / frames;
            inexistencetAnimation(&plant, aux);
        }
    }

    for (i = 0;i < frames;i++) {
        aux.leftArm.bottom = aux.leftArm.bottom + 80 / frames;
        inexistencetAnimation(&plant, aux);
    }
    for (i = 0;i < frames;i++) {
        aux.leftHandSide = aux.leftHandSide + 70 / frames;
        inexistencetAnimation(&plant, aux);
    }
    for (j = 0;j < 3;j++) {
        for (i = 0;i < frames;i++) {
            aux.leftHandSide = aux.leftHandSide - 110 / frames;
            inexistencetAnimation(&plant, aux);
        }
        for (i = 0;i < frames;i++) {
            aux.leftHandSide = aux.leftHandSide + 110 / frames;
            inexistencetAnimation(&plant, aux);
        }
    }
    for (i = 0;i < frames;i++) {
        aux.leftHandSide = aux.leftHandSide - 70 / frames;
        inexistencetAnimation(&plant, aux);
    }

    for (i = 0;i < frames;i++) {
        //squat
        aux.rightLeg.top = aux.rightLeg.top - 75.0 / frames;
        aux.rightLeg.bottom = aux.rightLeg.bottom + 150.0 / frames;
        aux.rightFoot = aux.rightFoot - 75.0 / frames;
        //make the translation
        aux.position.y = -8 + 8 * cos((75 - (i + 1) * (75.0 / frames)) * pi / 180.0);

        aux.leftLeg.top = aux.leftLeg.top - 75.0 / frames;
        aux.leftLeg.bottom = aux.leftLeg.bottom + 150.0 / frames;
        aux.leftFoot = aux.leftFoot - 75.0 / frames;

        aux.leftArm.top = aux.leftArm.top - 10 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom - 90 / frames;
        aux.rightArm.top = aux.rightArm.top - 10 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom - 90 / frames;

        aux.angTrunk = aux.angTrunk - 15.0 / frames;


        inexistencetAnimation(&plant, aux);

    }
    aux.position.width2 = 0;
    inexistencetAnimation(&plant, aux);
}


void createEnvironment() {
    int i, numTrees1 = 5, numTrees2 = 5;
    tItem aux;
    aux.width = 4; aux.height = 24;aux.width2 = 24;aux.height2 = 48;
    aux.y = 0;aux.time = 0;aux.ray = 20;
    srand(10007);

    for (i = 0;i < numTrees1;i++) {
        aux.x = (rand() % 50) - 50;
        aux.z = (rand() % 45) + 5;
        inexistencet(&trees, aux);
    }
    for (i = 0;i < numTrees2;i++) {
        aux.x = (rand() % 50) - 50;
        aux.z = (rand() % 45) + 5;
        inexistencet(&trees2, aux);
    }

    for (i = 0;i < numTrees1;i++) {
        aux.x = (rand() % 50) - 50;
        aux.z = (rand() % 45) - 50;
        inexistencet(&trees, aux);
    }
    for (i = 0;i < numTrees2;i++) {
        aux.x = (rand() % 50) - 50;
        aux.z = (rand() % 45) - 50;
        inexistencet(&trees2, aux);
    }
    //-----------

    for (i = 0;i < numTrees1;i++) {
        aux.x = (rand() % 50) + 30;
        aux.z = (rand() % 45) - 3;
        inexistencet(&trees, aux);
    }
    for (i = 0;i < numTrees2;i++) {
        aux.x = (rand() % 50) + 30;
        aux.z = (rand() % 45) - 3;
        inexistencet(&trees2, aux);
    }

    for (i = 0;i < numTrees1;i++) {
        aux.x = (rand() % 50) + 30;
        aux.z = (rand() % 45) + 30;
        inexistencet(&trees, aux);
    }
    for (i = 0;i < numTrees2;i++) {
        aux.x = (rand() % 50) + 30;
        aux.z = (rand() % 45) + 30;
        inexistencet(&trees2, aux);
    }

    for (i = 0;i < numTrees1;i++) {
        aux.x = (rand() % 50) + 20;
        aux.z = (rand() % 45) - 30;
        inexistencet(&trees, aux);
    }
    for (i = 0;i < numTrees2;i++) {
        aux.x = (rand() % 50) + 20;
        aux.z = (rand() % 45) - 30;
        inexistencet(&trees2, aux);
    }

    aux.x = 0.4;
    aux.y = 0;
    aux.z = 0.4;
    aux.time = 2;
    aux.width = 8;
    for (i = 0;i < 10;i++) {
        aux.x = aux.x + 0.5;
        inexistencet(&soils, aux);
        aux.z = aux.z + 0.5;
        inexistencet(&soils, aux);
        aux.z = aux.z + 0.5;
        inexistencet(&soils, aux);
        aux.z = aux.z + 0.5;
        inexistencet(&soils, aux);
        aux.z = aux.z + 0.5;
        inexistencet(&soils, aux);
        aux.z = aux.z - 2.0;
    }

    aux.x = 0.4;
    aux.y = 0;
    aux.z = 3.8;
    aux.time = 2;
    aux.width = 8;
    for (i = 0;i < 10;i++) {
        aux.x = aux.x + 0.5;
        inexistencet(&soils, aux);
        aux.z = aux.z + 0.5;
        inexistencet(&soils, aux);
        aux.z = aux.z + 0.5;
        inexistencet(&soils, aux);
        aux.z = aux.z + 0.5;
        inexistencet(&soils, aux);
        aux.z = aux.z + 0.5;
        inexistencet(&soils, aux);
        aux.z = aux.z - 2.0;
    }
    aux.x = 0.4;
    aux.y = 0;
    aux.z = 7.5;
    aux.time = 2;
    aux.width = 8;
    for (i = 0;i < 10;i++) {
        aux.x = aux.x + 0.5;
        inexistencet(&soils, aux);
        aux.z = aux.z + 0.5;
        inexistencet(&soils, aux);
        aux.z = aux.z + 0.5;
        inexistencet(&soils, aux);
        aux.z = aux.z + 0.5;
        inexistencet(&soils, aux);
        aux.z = aux.z + 0.5;
        inexistencet(&soils, aux);
        aux.z = aux.z - 2.0;
    }
    aux.x = -0.981687;
    aux.z = 3.221200;
    inexistencet(&soils, aux);

    aux.x = -1.317674;
    aux.z = 3.058875;
    inexistencet(&soils, aux);

    aux.x = -1.619865;
    aux.z = 3.445374;
    inexistencet(&soils, aux);

    aux.x = -1.854489;
    aux.z = 3.225374;
    inexistencet(&soils, aux);

    aux.x = -2.161338;
    aux.z = 3.097190;
    inexistencet(&soils, aux);

    aux.x = -2.554346;
    aux.z = 3.254736;
    inexistencet(&soils, aux);

    aux.x = -3.042595;
    aux.z = 3.377130;
    inexistencet(&soils, aux);

    aux.x = -1.719773;
    aux.z = 2.976987;
    inexistencet(&soils, aux);

    aux.x = -2.863410;
    aux.z = 3.208781;
    inexistencet(&soils, aux);

    aux.x = -3.336220;
    aux.z = 3.647461;
    inexistencet(&soils, aux);

    aux.x = -3.271369;
    aux.z = 4.167013;
    inexistencet(&soils, aux);

    aux.x = -3.295493;
    aux.z = 4.422232;
    inexistencet(&soils, aux);

    aux.x = -1.146440;
    aux.z = 4.659145;
    inexistencet(&soils, aux);

    aux.x = -1.545807;
    aux.z = 4.679392;
    inexistencet(&soils, aux);

    aux.x = -1.945241;
    aux.z = 4.704529;
    inexistencet(&soils, aux);

    aux.x = -2.319798;
    aux.z = 4.689934;
    inexistencet(&soils, aux);

    aux.x = -2.625922;
    aux.z = 4.685446;
    inexistencet(&soils, aux);

    aux.x = -2.908730;
    aux.z = 4.704473;
    inexistencet(&soils, aux);

    aux.x = -3.238203;
    aux.z = 4.698647;
    inexistencet(&soils, aux);

    aux.x = -3.361828;
    aux.z = 3.917706;
    inexistencet(&soils, aux);

    aux.x = -1.229808;
    aux.z = 4.892498;
    inexistencet(&soils, aux);

    aux.x = -1.739836;
    aux.z = 4.853353;
    inexistencet(&soils, aux);

    aux.x = -2.162704;
    aux.z = 4.905857;
    inexistencet(&soils, aux);

    aux.x = -2.491581;
    aux.z = 4.804374;
    inexistencet(&soils, aux);

    aux.x = -3.387214;
    aux.z = 4.574269;
    inexistencet(&soils, aux);

    aux.x = -3.306363;
    aux.z = 3.417534;
    inexistencet(&soils, aux);

}


void createAutoAnimation() {
    int i;
    humanoid aux;
    aux.rightArm.top = 0;
    aux.rightArm.bottom = 0;
    aux.headUpDown = 0;
    aux.headSide = 0;
    aux.rightHandSide = 0;
    aux.angTrunk = 0;
    aux.rightFoot = 0;
    aux.rightLeg.bottom = 0;
    aux.leftLeg.top = 0;
    aux.leftLeg.bottom = 0;
    aux.leftHandSide = 0;
    aux.rightHandUpDown = 0;
    aux.leftHandUpDown = 0;
    aux.rightLeg.topSide = 0;
    aux.leftLeg.topSide = 0;
    aux.rightArm.topSpin = 0;
    aux.rightArm.topSide = 0;
    aux.leftArm.topSpin = 0;
    aux.leftArm.topSide = 0;
    aux.rightLeg.top = 0;
    aux.leftArm.bottom = 0;
    aux.rightArm.bottom = 0;
    aux.leftFoot = 0;
    aux.rightArm.top = 0;
    aux.leftArm.top = 0;
    aux.stop = 0;
    aux.position.x = existence.position.x;
    aux.position.y = 0;
    aux.position.ray = existence.position.ray;
    aux.position.z = existence.position.z;
    aux.position.time = 0;

    float frames = 10.0;
    //lower the trunk
    for (i = 0;i < frames;i++) {
        aux.angTrunk = aux.angTrunk + 40.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);
    }
    //lift the trunk
    for (i = 0;i < frames;i++) {
        aux.angTrunk = aux.angTrunk - 40 / frames;
        inexistencetAnimation(&autoAnimate, aux);
    }
    // move arms


    for (i = 0;i < frames;i++) {
        aux.rightArm.top = aux.rightArm.top + 90.0 / frames;

        aux.rightArm.topSide = aux.rightArm.topSide + 90.0 / frames;
        aux.rightArm.topSpin = aux.rightArm.topSpin - 90.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom + 90.0 / frames;

        aux.leftArm.top = aux.leftArm.top + 90.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin + 60.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom + 90.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);
    }

    for (i = 0;i < frames;i++) {
        aux.rightArm.top = aux.rightArm.top - 90.0 / frames;

        aux.rightArm.topSide = aux.rightArm.topSide - 90.0 / frames;
        aux.rightArm.topSpin = aux.rightArm.topSpin + 90.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom - 90.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom - 90.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin - 60.0 / frames;
        aux.leftArm.top = aux.leftArm.top - 90.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);
    }


    for (i = 0;i < frames;i++) {
        aux.rightArm.top = aux.rightArm.top + 90.0 / frames;
        aux.rightArm.topSpin = aux.rightArm.topSpin - 60.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom + 90.0 / frames;

        aux.headSide = aux.headSide + 60.0 / frames;
        aux.headUpDown = aux.headUpDown + 10.0 / frames;
        aux.leftArm.top = aux.leftArm.top + 90.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin - 60.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom + 90.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);

    }

    for (i = 0;i < frames;i++) {

        aux.rightArm.topSpin = aux.rightArm.topSpin + 120.0 / frames;
        aux.headSide = aux.headSide - 120.0 / frames;
        if (i < 30)
            aux.headUpDown = aux.headUpDown - 10.0 / frames;
        else
            aux.headUpDown = aux.headUpDown + 10.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin + 120.0 / frames;

        inexistencetAnimation(&autoAnimate, aux);

    }

    for (i = 0;i < frames;i++) {
        aux.rightArm.top = aux.rightArm.top - 90.0 / frames;
        aux.rightArm.topSpin = aux.rightArm.topSpin - 60.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom - 90.0 / frames;

        aux.headSide = aux.headSide + 60.0 / frames;
        aux.headUpDown = aux.headUpDown - 10.0 / frames;

        aux.leftArm.top = aux.leftArm.top - 90.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin - 60.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom - 90.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);

    }

    // the squat avatar
    for (i = 0;i < frames;i++) {
        //squat
        aux.rightLeg.top = aux.rightLeg.top + 50.0 / frames;
        aux.rightLeg.bottom = aux.rightLeg.bottom - 100.0 / frames;
        aux.rightFoot = aux.rightFoot + 50.0 / frames;
        //make the translation
        aux.position.y = -8 + 8 * cos((i + 1) * (50.0 / frames) * pi / 180.0);

        aux.leftLeg.top = aux.leftLeg.top + 50.0 / frames;
        aux.leftLeg.bottom = aux.leftLeg.bottom - 100.0 / frames;
        aux.leftFoot = aux.leftFoot + 50.0 / frames;

        //-----------------------------

        aux.rightArm.top = aux.rightArm.top + 90.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom + 90.0 / frames;
        aux.rightArm.topSide = aux.rightArm.topSide + 75.0 / frames;

        aux.leftArm.top = aux.leftArm.top + 90.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom + 90.0 / frames;
        aux.leftArm.topSide = aux.leftArm.topSide - 75.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);
    }

    for (i = 0;i < frames;i++) {
        //squat
        aux.rightLeg.top = aux.rightLeg.top - 50.0 / frames;
        aux.rightLeg.bottom = aux.rightLeg.bottom + 100.0 / frames;
        aux.rightFoot = aux.rightFoot - 50.0 / frames;
        //make the translation
        aux.position.y = -8 + 8 * cos((50.0 - (i + 1) * (50.0 / frames)) * pi / 180.0);

        aux.leftLeg.top = aux.leftLeg.top - 50.0 / frames;
        aux.leftLeg.bottom = aux.leftLeg.bottom + 100.0 / frames;
        aux.leftFoot = aux.leftFoot - 50.0 / frames;

        //-----------------------------

        aux.rightArm.top = aux.rightArm.top - 90.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom - 90.0 / frames;
        aux.rightArm.topSide = aux.rightArm.topSide - 75.0 / frames;

        aux.leftArm.top = aux.leftArm.top - 90.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom - 90.0 / frames;
        aux.leftArm.topSide = aux.leftArm.topSide + 75.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);
    }
    //-------------------------------------------------------------------------------------------------

    for (i = 0;i < frames;i++) {
        aux.rightArm.top = aux.rightArm.top + 90.0 / frames;

        aux.rightArm.topSide = aux.rightArm.topSide + 90.0 / frames;
        aux.rightArm.topSpin = aux.rightArm.topSpin - 90.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom + 90.0 / frames;

        aux.leftArm.top = aux.leftArm.top + 90.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin + 60.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom + 90.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);
    }

    for (i = 0;i < frames;i++) {
        aux.rightArm.top = aux.rightArm.top - 90.0 / frames;

        aux.rightArm.topSide = aux.rightArm.topSide - 90.0 / frames;
        aux.rightArm.topSpin = aux.rightArm.topSpin + 90.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom - 90.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom - 90.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin - 60.0 / frames;
        aux.leftArm.top = aux.leftArm.top - 90.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);
    }


    for (i = 0;i < frames;i++) {
        aux.rightArm.top = aux.rightArm.top + 90.0 / frames;
        aux.rightArm.topSpin = aux.rightArm.topSpin - 60.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom + 90.0 / frames;

        aux.headSide = aux.headSide + 60.0 / frames;
        aux.headUpDown = aux.headUpDown + 10.0 / frames;
        aux.leftArm.top = aux.leftArm.top + 90.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin - 60.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom + 90.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);

    }

    for (i = 0;i < frames;i++) {

        aux.rightArm.topSpin = aux.rightArm.topSpin + 120.0 / frames;
        aux.headSide = aux.headSide - 120.0 / frames;
        if (i < frames / 2)
            aux.headUpDown = aux.headUpDown - 10.0 / frames;
        else
            aux.headUpDown = aux.headUpDown + 10.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin + 120.0 / frames;

        inexistencetAnimation(&autoAnimate, aux);

    }

    for (i = 0;i < frames;i++) {
        aux.rightArm.top = aux.rightArm.top - 90.0 / frames;
        aux.rightArm.topSpin = aux.rightArm.topSpin - 60.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom - 90.0 / frames;

        aux.headSide = aux.headSide + 60.0 / frames;
        aux.headUpDown = aux.headUpDown - 10.0 / frames;

        aux.leftArm.top = aux.leftArm.top - 90.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin - 60.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom - 90.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);

    }

    // the squat avatar
    for (i = 0;i < frames;i++) {
        //squat
        aux.rightLeg.top = aux.rightLeg.top + 50.0 / frames;
        aux.rightLeg.bottom = aux.rightLeg.bottom - 100.0 / frames;
        aux.rightFoot = aux.rightFoot + 50.0 / frames;
        aux.rightLeg.topSide = aux.rightLeg.topSide + 30.0 / frames;
        //make the translation
        aux.position.y = -8 + 8 * cos((i + 1) * (50.0 / frames) * pi / 180.0);

        aux.leftLeg.top = aux.leftLeg.top + 50.0 / frames;
        aux.leftLeg.bottom = aux.leftLeg.bottom - 100.0 / frames;
        aux.leftFoot = aux.leftFoot + 50.0 / frames;

        //-----------------------------

        aux.rightArm.top = aux.rightArm.top + 90.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom + 90.0 / frames;
        aux.rightArm.topSide = aux.rightArm.topSide + 75.0 / frames;

        aux.leftArm.top = aux.leftArm.top + 90.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom + 90.0 / frames;
        aux.leftArm.topSide = aux.leftArm.topSide - 75.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);
    }

    for (i = 0;i < frames;i++) {
        //squat
        aux.rightLeg.top = aux.rightLeg.top - 50.0 / frames;
        aux.rightLeg.bottom = aux.rightLeg.bottom + 100.0 / frames;
        aux.rightFoot = aux.rightFoot - 50.0 / frames;
        aux.rightLeg.topSide = aux.rightLeg.topSide - 30.0 / frames;
        //make the translation
        aux.position.y = -8 + 8 * cos((50 - (i + 1) * (50.0 / frames)) * pi / 180.0);

        aux.leftLeg.top = aux.leftLeg.top - 50.0 / frames;
        aux.leftLeg.bottom = aux.leftLeg.bottom + 100.0 / frames;
        aux.leftFoot = aux.leftFoot - 50.0 / frames;

        //-----------------------------

        aux.rightArm.top = aux.rightArm.top - 90.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom - 90.0 / frames;
        aux.rightArm.topSide = aux.rightArm.topSide - 75.0 / frames;

        aux.leftArm.top = aux.leftArm.top - 90.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom - 90.0 / frames;
        aux.leftArm.topSide = aux.leftArm.topSide + 75.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);
    }

    //----------------------------------------------------------------------------------------------------------------aaaaaa
    for (i = 0;i < frames;i++) {
        aux.rightArm.top = aux.rightArm.top + 90.0 / frames;

        aux.rightArm.topSide = aux.rightArm.topSide + 90.0 / frames;
        aux.rightArm.topSpin = aux.rightArm.topSpin - 90.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom + 90.0 / frames;

        aux.leftArm.top = aux.leftArm.top + 90.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin + 60.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom + 90.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);
    }

    for (i = 0;i < frames;i++) {
        aux.rightArm.top = aux.rightArm.top - 90.0 / frames;

        aux.rightArm.topSide = aux.rightArm.topSide - 90.0 / frames;
        aux.rightArm.topSpin = aux.rightArm.topSpin + 90.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom - 90.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom - 90.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin - 60.0 / frames;
        aux.leftArm.top = aux.leftArm.top - 90.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);
    }


    for (i = 0;i < frames;i++) {
        aux.rightArm.top = aux.rightArm.top + 90.0 / frames;
        aux.rightArm.topSpin = aux.rightArm.topSpin - 60.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom + 90.0 / frames;

        aux.headSide = aux.headSide + 60.0 / frames;
        aux.headUpDown = aux.headUpDown + 10.0 / frames;
        aux.leftArm.top = aux.leftArm.top + 90.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin - 60.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom + 90.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);

    }

    for (i = 0;i < frames;i++) {

        aux.rightArm.topSpin = aux.rightArm.topSpin + 120.0 / frames;
        aux.headSide = aux.headSide - 120.0 / frames;
        if (i < frames / 2)
            aux.headUpDown = aux.headUpDown - 10.0 / frames;
        else
            aux.headUpDown = aux.headUpDown + 10.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin + 120.0 / frames;

        inexistencetAnimation(&autoAnimate, aux);

    }

    for (i = 0;i < frames;i++) {
        aux.rightArm.top = aux.rightArm.top - 90.0 / frames;
        aux.rightArm.topSpin = aux.rightArm.topSpin - 60.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom - 90.0 / frames;

        aux.headSide = aux.headSide + 60.0 / frames;
        aux.headUpDown = aux.headUpDown - 10.0 / frames;

        aux.leftArm.top = aux.leftArm.top - 90.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin - 60.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom - 90.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);

    }

    // the squat avatar
    for (i = 0;i < frames;i++) {
        //squat
        aux.rightLeg.top = aux.rightLeg.top + 50.0 / frames;
        aux.rightLeg.bottom = aux.rightLeg.bottom - 100.0 / frames;
        aux.rightFoot = aux.rightFoot + 50.0 / frames;
        //make the translation
        aux.position.y = -8 + 8 * cos((i + 1) * (50.0 / frames) * pi / 180.0);

        aux.leftLeg.top = aux.leftLeg.top + 50.0 / frames;
        aux.leftLeg.bottom = aux.leftLeg.bottom - 100.0 / frames;
        aux.leftFoot = aux.leftFoot + 50.0 / frames;

        //-----------------------------

        aux.rightArm.top = aux.rightArm.top + 90.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom + 90.0 / frames;
        aux.rightArm.topSide = aux.rightArm.topSide + 75.0 / frames;

        aux.leftArm.top = aux.leftArm.top + 90.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom + 90.0 / frames;
        aux.leftArm.topSide = aux.leftArm.topSide - 75.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);
    }

    for (i = 0;i < frames;i++) {
        //squat
        aux.rightLeg.top = aux.rightLeg.top - 50.0 / frames;
        aux.rightLeg.bottom = aux.rightLeg.bottom + 100.0 / frames;
        aux.rightFoot = aux.rightFoot - 50.0 / frames;
        //make the translation
        aux.position.y = -8 + 8 * cos((50 - (i + 1) * (50.0 / frames)) * pi / 180.0);

        aux.leftLeg.top = aux.leftLeg.top - 50.0 / frames;
        aux.leftLeg.bottom = aux.leftLeg.bottom + 100.0 / frames;
        aux.leftFoot = aux.leftFoot - 50.0 / frames;

        //-----------------------------

        aux.rightArm.top = aux.rightArm.top - 90.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom - 90.0 / frames;
        aux.rightArm.topSide = aux.rightArm.topSide - 75.0 / frames;

        aux.leftArm.top = aux.leftArm.top - 90.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom - 90.0 / frames;
        aux.leftArm.topSide = aux.leftArm.topSide + 75.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);
    }
    //-------------------------------------------------------------------------------------------------

    for (i = 0;i < frames;i++) {
        aux.rightArm.top = aux.rightArm.top + 90.0 / frames;

        aux.rightArm.topSide = aux.rightArm.topSide + 90.0 / frames;
        aux.rightArm.topSpin = aux.rightArm.topSpin - 90.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom + 90.0 / frames;

        aux.leftArm.top = aux.leftArm.top + 90.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin + 60.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom + 90.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);
    }

    for (i = 0;i < frames;i++) {
        aux.rightArm.top = aux.rightArm.top - 90.0 / frames;

        aux.rightArm.topSide = aux.rightArm.topSide - 90.0 / frames;
        aux.rightArm.topSpin = aux.rightArm.topSpin + 90.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom - 90.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom - 90.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin - 60.0 / frames;
        aux.leftArm.top = aux.leftArm.top - 90.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);
    }


    for (i = 0;i < frames;i++) {
        aux.rightArm.top = aux.rightArm.top + 90.0 / frames;
        aux.rightArm.topSpin = aux.rightArm.topSpin - 60.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom + 90.0 / frames;

        aux.headSide = aux.headSide + 60.0 / frames;
        aux.headUpDown = aux.headUpDown + 10.0 / frames;
        aux.leftArm.top = aux.leftArm.top + 90.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin - 60.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom + 90.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);

    }

    for (i = 0;i < frames;i++) {

        aux.rightArm.topSpin = aux.rightArm.topSpin + 120.0 / frames;
        aux.headSide = aux.headSide - 120.0 / frames;
        if (i < frames / 2)
            aux.headUpDown = aux.headUpDown - 10.0 / frames;
        else
            aux.headUpDown = aux.headUpDown + 10.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin + 120.0 / frames;

        inexistencetAnimation(&autoAnimate, aux);

    }

    for (i = 0;i < frames;i++) {
        aux.rightArm.top = aux.rightArm.top - 90.0 / frames;
        aux.rightArm.topSpin = aux.rightArm.topSpin - 60.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom - 90.0 / frames;

        aux.headSide = aux.headSide + 60.0 / frames;
        aux.headUpDown = aux.headUpDown - 10.0 / frames;

        aux.leftArm.top = aux.leftArm.top - 90.0 / frames;
        aux.leftArm.topSpin = aux.leftArm.topSpin - 60.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom - 90.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);

    }

    // the squat avatar
    for (i = 0;i < frames;i++) {
        //squat
        aux.rightLeg.top = aux.rightLeg.top + 50.0 / frames;
        aux.rightLeg.bottom = aux.rightLeg.bottom - 100.0 / frames;
        aux.rightFoot = aux.rightFoot + 50.0 / frames;
        aux.leftLeg.topSide = aux.leftLeg.topSide - 30.0 / frames;
        //make the translation
        aux.position.y = -8 + 8 * cos((i + 1) * (50.0 / frames) * pi / 180.0);

        aux.leftLeg.top = aux.leftLeg.top + 50.0 / frames;
        aux.leftLeg.bottom = aux.leftLeg.bottom - 100.0 / frames;
        aux.leftFoot = aux.leftFoot + 50.0 / frames;

        //-----------------------------

        aux.rightArm.top = aux.rightArm.top + 90.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom + 90.0 / frames;
        aux.rightArm.topSide = aux.rightArm.topSide + 75.0 / frames;

        aux.leftArm.top = aux.leftArm.top + 90.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom + 90.0 / frames;
        aux.leftArm.topSide = aux.leftArm.topSide - 75.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);
    }

    for (i = 0;i < frames;i++) {
        //squat
        aux.rightLeg.top = aux.rightLeg.top - 50.0 / frames;
        aux.rightLeg.bottom = aux.rightLeg.bottom + 100.0 / frames;
        aux.rightFoot = aux.rightFoot - 50.0 / frames;
        aux.leftLeg.topSide = aux.leftLeg.topSide + 30.0 / frames;
        //make the translation
        aux.position.y = -8 + 8 * cos((50 - (i + 1) * (50.0 / frames)) * pi / 180.0);

        aux.leftLeg.top = aux.leftLeg.top - 50.0 / frames;
        aux.leftLeg.bottom = aux.leftLeg.bottom + 100.0 / frames;
        aux.leftFoot = aux.leftFoot - 50.0 / frames;

        //-----------------------------

        aux.rightArm.top = aux.rightArm.top - 90.0 / frames;
        aux.rightArm.bottom = aux.rightArm.bottom - 90.0 / frames;
        aux.rightArm.topSide = aux.rightArm.topSide - 75.0 / frames;

        aux.leftArm.top = aux.leftArm.top - 90.0 / frames;
        aux.leftArm.bottom = aux.leftArm.bottom - 90.0 / frames;
        aux.leftArm.topSide = aux.leftArm.topSide + 75.0 / frames;
        inexistencetAnimation(&autoAnimate, aux);
    }
}


void autoAnimation(int a) {

    existence = nextQueueAnimation(&autoAnimate);

    if (existence.stop != 1) {
        glutTimerFunc(50, autoAnimation, 0);

        draw();
    }
    else {
        goBackToInitialAnimation(&autoAnimate);
        glutPostRedisplay();
    }


}


void draw() {

    if (action == 0 || action == 15) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glRotatef(angx, 1, 0, 0);
        glRotatef(angy, 0, 1, 0);
        drawFloor();
        drawSky();

        drawHumanoid();


        tItem aux, aux2;


        aux = nextQueue(&trees);
        while (aux.time != -1) {
            drawTree(aux);
            aux = nextQueue(&trees);

        }

        aux = nextQueue(&trees2);
        while (aux.time != -1) {
            drawTree2(aux);
            aux = nextQueue(&trees2);

        }
        srand(10);

        aux = nextQueue(&soils);
        while (aux.time != -1) {
            drawGround(aux);
            aux = nextQueue(&soils);

        }

        aux = nextQueue(&mousePosition);
        while (aux.time != -1) {
            aux.width = 8;

            drawGround(aux);
            aux = nextQueue(&mousePosition);

        }

        aux2.x = -2;aux2.y = 0;aux2.z = 4;
        drawHouse(aux2);
        //--------------------------------------------------------------------------------

        goBackToInitialQueue(&trees);
        goBackToInitialQueue(&trees2);
        goBackToInitialQueue(&soils);
        goBackToInitialQueue(&mousePosition);


        glutSwapBuffers();
    }
    else if (action < 15) {
        showCharacter();
    }
}


void withWalk(unsigned char letra, int x, int y) {

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    switch (letra) {
    case 27:
        deleteQueue(&soils);
        deleteQueue(&trees);
        deleteQueue(&trees2);
        deleteQueue(&mousePosition);

        turnOffAnimation(&autoAnimate);
        turnOffAnimation(&walk);
        turnOffAnimation(&plant);
        exit(0);
        break;
    case 'a':
        xa = xa - 0.2 * sin(theta - pi / 2);
        za = za - 0.2 * cos(theta - pi / 2);
        cza = cza - 0.2 * cos(theta - pi / 2);
        cxa = cxa - 0.2 * sin(theta - pi / 2);
        break;
    case 'd':
        xa = xa + 0.2 * sin(theta - pi / 2);
        za = za + 0.2 * cos(theta - pi / 2);
        cza = cza + 0.2 * cos(theta - pi / 2);
        cxa = cxa + 0.2 * sin(theta - pi / 2);
        break;
    case 's':
        xa = xa - 0.2 * sin(theta);
        za = za - 0.2 * cos(theta);
        cza = cza - 0.2 * cos(theta);
        cxa = cxa - 0.2 * sin(theta);
        break;
    case 'w':
        xa = xa + 0.2 * sin(theta);
        za = za + 0.2 * cos(theta);
        cza = cza + 0.2 * cos(theta);
        cxa = cxa + 0.2 * sin(theta);
        break;
    case 'q':
        theta = theta + pi / 100;
        cza = za + cos(theta);
        cxa = xa + sin(theta);
        break;
    case 'e':
        theta = theta - pi / 100;
        cza = za + cos(theta);
        cxa = xa + sin(theta);
    }

    gluLookAt(xa, 0, za, cxa, 0, cza, 0, 1, 0);
    glutPostRedisplay();
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT1, GL_POSITION, position1);

}

void withWalk2(int key, int x, int y) {
    if (action == 1) {
        switch (key) {
        case GLUT_KEY_LEFT:

            break;
        case GLUT_KEY_RIGHT:

            break;
        case GLUT_KEY_DOWN:
            if (existence.rightFoot - 5 >= -15)
                existence.rightFoot = existence.rightFoot - 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            if (existence.rightFoot + 5 <= 30)
                existence.rightFoot = existence.rightFoot + 5;
            glutPostRedisplay();
            break;
        }
    }
    else if (action == 2) {
        switch (key) {
        case GLUT_KEY_LEFT:

            break;
        case GLUT_KEY_RIGHT:

            break;
        case GLUT_KEY_DOWN:
            if (existence.leftFoot - 5 >= -15)
                existence.leftFoot = existence.leftFoot - 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            if (existence.leftFoot + 5 <= 30)
                existence.leftFoot = existence.leftFoot + 5;
            glutPostRedisplay();
            break;
        }


    }
    else if (action == 3) {
        switch (key) {
        case GLUT_KEY_LEFT:
            if (existence.rightLeg.topSide - 5 >= -15)
                existence.rightLeg.topSide = existence.rightLeg.topSide - 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:

            if (existence.rightLeg.topSide + 5 <= 20)
                existence.rightLeg.topSide = existence.rightLeg.topSide + 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            if (existence.rightLeg.top - 5 >= -40)
                existence.rightLeg.top = existence.rightLeg.top - 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:

            if (existence.rightLeg.top + 5 <= 50)
                existence.rightLeg.top = existence.rightLeg.top + 5;
            glutPostRedisplay();
            break;
        }

    }
    else if (action == 4) {
        switch (key) {
        case GLUT_KEY_LEFT:
            if (existence.leftLeg.topSide - 5 >= -15)
                existence.leftLeg.topSide = existence.leftLeg.topSide - 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:

            if (existence.leftLeg.topSide + 5 <= 20)
                existence.leftLeg.topSide = existence.leftLeg.topSide + 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            if (existence.leftLeg.top - 5 >= -40)
                existence.leftLeg.top = existence.leftLeg.top - 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:

            if (existence.leftLeg.top + 5 <= 50)
                existence.leftLeg.top = existence.leftLeg.top + 5;
            glutPostRedisplay();
            break;
        }

    }
    else if (action == 5) {
        switch (key) {
        case GLUT_KEY_LEFT:
            if (existence.rightArm.topSide + 5 <= 90)
                existence.rightArm.topSide = existence.rightArm.topSide + 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            if (existence.rightArm.topSide - 5 >= -15)
                existence.rightArm.topSide = existence.rightArm.topSide - 5;
            glutPostRedisplay();

            break;
        case GLUT_KEY_DOWN:
            if (existence.rightArm.top - 5 >= -40)
                existence.rightArm.top = existence.rightArm.top - 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:

            if (existence.rightArm.top + 5 <= 90)
                existence.rightArm.top = existence.rightArm.top + 5;
            glutPostRedisplay();
            break;
        }

    }
    else if (action == 6) {
        switch (key) {
        case GLUT_KEY_LEFT:
            if (existence.leftArm.topSide + 5 <= 20)
                existence.leftArm.topSide = existence.leftArm.topSide + 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            if (existence.leftArm.topSide - 5 >= -90)
                existence.leftArm.topSide = existence.leftArm.topSide - 5;
            glutPostRedisplay();

            break;
        case GLUT_KEY_DOWN:
            if (existence.leftArm.top - 5 >= -40)
                existence.leftArm.top = existence.leftArm.top - 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:

            if (existence.leftArm.top + 5 <= 90)
                existence.leftArm.top = existence.leftArm.top + 5;
            glutPostRedisplay();
            break;
        }

    }
    else if (action == -1 || action == -2) {
        switch (key) {
        case GLUT_KEY_LEFT:
            if (existence.rightHandSide + 5 <= 20)
                existence.rightHandSide = existence.rightHandSide + 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            if (existence.rightHandSide - 5 >= -30)
                existence.rightHandSide = existence.rightHandSide - 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            if (existence.rightHandUpDown + 5 <= 10)
                existence.rightHandUpDown = existence.rightHandUpDown + 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            if (existence.rightHandUpDown - 5 >= -10)
                existence.rightHandUpDown = existence.rightHandUpDown - 5;
            glutPostRedisplay();
            break;
        }

    }
    else if (action == -3 || action == -4) {
        switch (key) {
        case GLUT_KEY_LEFT:
            if (existence.leftHandSide + 5 <= 20)
                existence.leftHandSide = existence.leftHandSide + 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            if (existence.leftHandSide - 5 >= -30)
                existence.leftHandSide = existence.leftHandSide - 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            if (existence.leftHandUpDown + 5 <= 10)
                existence.leftHandUpDown = existence.leftHandUpDown + 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            if (existence.leftHandUpDown - 5 >= -10)
                existence.leftHandUpDown = existence.leftHandUpDown - 5;
            glutPostRedisplay();
            break;
        }


    }
    else if (action == 9) {
        switch (key) {
        case GLUT_KEY_LEFT:
            if (existence.headSide - 5 >= -75)
                existence.headSide = existence.headSide - 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:

            if (existence.headSide + 5 <= 75)
                existence.headSide = existence.headSide + 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            if (existence.headUpDown - 5 >= -30)
                existence.headUpDown = existence.headUpDown - 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:

            if (existence.headUpDown + 5 <= 35)
                existence.headUpDown = existence.headUpDown + 5;
            glutPostRedisplay();
            break;
        }

    }
    else if (action == 11) {
        switch (key) {
        case GLUT_KEY_LEFT:

            break;
        case GLUT_KEY_RIGHT:

            break;
        case GLUT_KEY_DOWN:
            if (existence.rightLeg.bottom - 5 >= -100)
                existence.rightLeg.bottom = existence.rightLeg.bottom - 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:

            if (existence.rightLeg.bottom + 5 <= 0)
                existence.rightLeg.bottom = existence.rightLeg.bottom + 5;
            glutPostRedisplay();
            break;
        }

    }
    else if (action == 12) {
        switch (key) {
        case GLUT_KEY_LEFT:

            break;
        case GLUT_KEY_RIGHT:

            break;
        case GLUT_KEY_DOWN:
            if (existence.leftLeg.bottom - 5 >= -100)
                existence.leftLeg.bottom = existence.leftLeg.bottom - 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            if (existence.leftLeg.bottom + 5 <= 0)
                existence.leftLeg.bottom = existence.leftLeg.bottom + 5;
            glutPostRedisplay();
            break;
        }

    }
    else if (action == 13) {
        switch (key) {
        case GLUT_KEY_LEFT:

            break;
        case GLUT_KEY_RIGHT:

            break;
        case GLUT_KEY_DOWN:
            if (existence.rightArm.bottom - 5 >= 0)
                existence.rightArm.bottom = existence.rightArm.bottom - 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:

            if (existence.rightArm.bottom + 5 <= 100)
                existence.rightArm.bottom = existence.rightArm.bottom + 5;
            glutPostRedisplay();
            break;
        }

    }
    else if (action == 14) {
        switch (key) {
        case GLUT_KEY_LEFT:

            break;
        case GLUT_KEY_RIGHT:

            break;
        case GLUT_KEY_DOWN:

            if (existence.leftArm.bottom - 5 >= 0)
                existence.leftArm.bottom = existence.leftArm.bottom - 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            if (existence.leftArm.bottom + 5 <= 100)
                existence.leftArm.bottom = existence.leftArm.bottom + 5;
            glutPostRedisplay();
            break;
        }

    }
    else if (action == -10) {
        switch (key) {
        case GLUT_KEY_LEFT:
            break;
        case GLUT_KEY_RIGHT:

            break;
        case GLUT_KEY_DOWN:

            if (existence.angTrunk - 5 >= 0)
                existence.angTrunk = existence.angTrunk - 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            if (existence.angTrunk + 5 <= 90)
                existence.angTrunk = existence.angTrunk + 5;
            glutPostRedisplay();
            break;
        }

    }
    glutPostRedisplay();
}
void defineWindow(int x, int y) {
    glViewport(0, 0, x, y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60, 1.777777778, 0.1, 200);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(xa, 0, za, 0, 0, 0, 0, 1, 0);

}

void showCharacter() {
    glClearColor(0, 0.5, 0.5, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    glRotatef(angy, 0, 1, 0);
    glRotatef(angx, 1, 0, 0);

    drawHumanoid();

    glPopMatrix();
    glutSwapBuffers();

}

void controlAutoAnimate(int num) {
    farmer aux;int timed = 15;
    int timean = 10;
    if (gameStatus == 1) {
        if (autoAnimationStatus == 0) {
            aux = nextQueueAnimation(&autoAnimate);
            if (aux.stop != 1) {
                existence = aux;
                draw();
                glutTimerFunc(timed, controlAutoAnimate, 0);
            }
            else {
                autoAnimationStatus = 1;
                turnOffAnimation(&autoAnimate);
                createAnimationWalk(-6, 0);
                glutTimerFunc(0, controlAutoAnimate, 0);
            }
        }
        else if (autoAnimationStatus == 1) {
            aux = nextQueueAnimation(&walk);
            if (aux.stop != 1) {
                existence = aux;
                draw();
                glutTimerFunc(timean, controlAutoAnimate, 0);
            }
            else {
                autoAnimationStatus = 2;
                turnOffAnimation(&walk);
                createAutoAnimation();
                glutTimerFunc(0, controlAutoAnimate, 0);
            }
        }
        else if (autoAnimationStatus == 2) {
            aux = nextQueueAnimation(&autoAnimate);
            if (aux.stop != 1) {
                existence = aux;
                draw();
                glutTimerFunc(timed, controlAutoAnimate, 0);
            }
            else {
                autoAnimationStatus = 3;
                turnOffAnimation(&autoAnimate);
                createAnimationWalk(-6, -6);
                glutTimerFunc(0, controlAutoAnimate, 0);
            }
        }
        else if (autoAnimationStatus == 3) {
            aux = nextQueueAnimation(&walk);
            if (aux.stop != 1) {
                existence = aux;
                draw();
                glutTimerFunc(timean, controlAutoAnimate, 0);
            }
            else {
                autoAnimationStatus = 4;
                turnOffAnimation(&walk);
                createAutoAnimation();
                glutTimerFunc(0, controlAutoAnimate, 0);
            }
        }
        else if (autoAnimationStatus == 4) {
            aux = nextQueueAnimation(&autoAnimate);
            if (aux.stop != 1) {
                existence = aux;
                draw();
                glutTimerFunc(timed, controlAutoAnimate, 0);
            }
            else {
                autoAnimationStatus = 5;
                turnOffAnimation(&autoAnimate);
                createAnimationWalk(0, -6);
                glutTimerFunc(0, controlAutoAnimate, 0);
            }
        }
        else if (autoAnimationStatus == 5) {
            aux = nextQueueAnimation(&walk);
            if (aux.stop != 1) {
                existence = aux;
                draw();
                glutTimerFunc(timean, controlAutoAnimate, 0);
            }
            else {
                autoAnimationStatus = 6;
                turnOffAnimation(&walk);
                createAutoAnimation();
                glutTimerFunc(0, controlAutoAnimate, 0);
            }
        }
        else if (autoAnimationStatus == 6) {
            aux = nextQueueAnimation(&autoAnimate);
            if (aux.stop != 1) {
                existence = aux;
                draw();
                glutTimerFunc(timed, controlAutoAnimate, 0);
            }
            else {
                autoAnimationStatus = 7;
                turnOffAnimation(&autoAnimate);
                createAnimationWalk(0, 0);
                glutTimerFunc(0, controlAutoAnimate, 0);
            }
        }
        else if (autoAnimationStatus == 7) {
            aux = nextQueueAnimation(&walk);
            if (aux.stop != 1) {
                existence = aux;
                draw();
                glutTimerFunc(timean, controlAutoAnimate, 0);
            }
            else {
                autoAnimationStatus = 0;
                turnOffAnimation(&walk);
                createAutoAnimation();
                glutTimerFunc(0, controlAutoAnimate, 0);
            }
        }

    }
}
void moveCharacterFoot(int item) {
    if (gameStatus != 2) {
        start2();
        turnOffAnimation(&plant);
    }
    gameStatus = 2;
    action = item;
    if (item == -1)
        existence.position.width2 = 1;
    else if (item == -3)
        existence.position.width2 = 2;
    else if (item == -2 || item == -4)
        existence.position.width2 = 0;
    showCharacter();
}
void goBackToGameMode(int a) {
    deleteQueue(&mousePosition);
    turnOffAnimation(&plant);
    turnOffAnimation(&autoAnimate);
    turnOffAnimation(&walk);


    existence.position.ray = 0;
    existence.position.x = 0;
    existence.position.y = 0;
    existence.position.z = 0;
    existence.position.time = 0;
    existence.position.width2 = 0;
    action = 0;
    za = -2;
    action = 0;
    xa = 1;ya = 0;cxa = 0;cza = 0, theta = -26.565051 * pi / 180.0;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(xa, 0, za, cxa, 0, cza, 0, 1, 0);
    start3();
    glutPostRedisplay();
}
void game(int item) {
    if (item == 2) {
        if (gameStatus != 0) {
            turnOffAnimation(&plant);

            tItem* aux = current2Queue(&mousePosition);
            if (aux != NULL && aux->time != 2) {
                aux->time = 0;
            }
            glutTimerFunc(0, controlFloorPlan, 0);

            turnOffAnimation(&autoAnimate);
            turnOffAnimation(&walk);
            turnOffAnimation(&plant);
            start3();
        }

        gameStatus = 0;
        action == 0;
        glutPostRedisplay();

    }
    else if (item == 1) {
        if (gameStatus != 1) {
            turnOffAnimation(&walk);
            start3();
            gameStatus = 1;
            autoAnimationStatus = 0;
            createAutoAnimation();
            controlAutoAnimate(0);
        }
        action = 15;
        glutPostRedisplay();
    }
    else if (item == 3) {
        gameStatus = 3;
        glutTimerFunc(0, goBackToGameMode, 0);
    }
}

void createMenu() {
    int configuraenxadaesquerdo = glutCreateMenu(moveCharacterFoot);
    glutAddMenuEntry("with the hoe", -3);
    glutAddMenuEntry("without the hoe", -4);
    int configuraenxadadireito = glutCreateMenu(moveCharacterFoot);
    glutAddMenuEntry("with the hoe", -1);
    glutAddMenuEntry("without the hoe", -2);

    int moverfootrsonagem = glutCreateMenu(moveCharacterFoot);
    glutAddMenuEntry("right foot", 1);
    glutAddMenuEntry("left foot", 2);
    glutAddMenuEntry("top right leg", 3);
    glutAddMenuEntry("bottom right leg", 11);
    glutAddMenuEntry("top left leg", 4);
    glutAddMenuEntry("bottom left leg", 12);
    glutAddMenuEntry("top right arm", 5);
    glutAddMenuEntry("bottom right arm", 13);
    glutAddMenuEntry("top left arm", 6);
    glutAddMenuEntry("bottom left arm", 14);
    glutAddSubMenu("right hand", configuraenxadadireito);
    glutAddSubMenu("left hand", configuraenxadaesquerdo);
    glutAddMenuEntry("head", 9);
    glutAddMenuEntry("stem", -10);



    glutCreateMenu(game);
    glutAddSubMenu("move footrsonage", moverfootrsonagem);
    glutAddMenuEntry("automatic animation", 1);
    glutAddMenuEntry("game mode", 2);
    glutAddMenuEntry("reset the game", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);


}


void light() {
    GLfloat light_ambient[] = { 0.1, 0.1,0.1, 1.0 };
    GLfloat light_ambient2[] = { 0.25, 0.25,0.25, 1.0 };
    GLfloat object_environment[] = { 0.4,0.4, 0.4, 1.0 };
    GLfloat light_diffuse[] = { 0.7,0.7,0.7,1.0 };
    GLfloat light_diffuse2[] = { 0.5,0.5,0.7,1.0 };
    GLfloat objeto_diffuse[] = { 0.4,0.4, 0.4, 1.0 };
    GLfloat light_esfootcular[] = { 1.0,1.0, 1.0, 1.0 };
    GLfloat objeto_esfootcular[] = { 0.5,0.5, 0.5, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_esfootcular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient2);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_esfootcular);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glLightfv(GL_LIGHT1, GL_POSITION, position1);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}
void controlFloorPlan(int a) {
    tItem* aux = current2Queue(&mousePosition);
    farmer aux2;
    if (gameStatus == 0) {
        if (aux != NULL) {
            if (aux->time == 0) {
                createAnimationWalk(aux->x, aux->z);
                aux->time = 1;
                glutTimerFunc(0, controlFloorPlan, 0);
            }
            else if (aux->time == 1) {
                aux2 = nextQueueAnimation(&walk);
                if (aux2.stop != 1) {
                    existence = aux2;
                    draw();
                    glutTimerFunc(7, controlFloorPlan, 0);
                }
                else {
                    createPlantAnimation();
                    wheelPlant(0);
                    aux->time = 3;
                    draw();
                    turnOffAnimation(&walk);
                    glutTimerFunc(0, controlFloorPlan, 0);
                }
            }
            else if (aux->time == 2) {
                aux = nextCurrentQueue2(&mousePosition);
                glutTimerFunc(0, controlFloorPlan, 0);
            }
            else if (aux->time == 3) {
                glutTimerFunc(0, controlFloorPlan, 0);
            }
        }
        else {

        }
    }
}
void catchClick(int button, int state, int x, int y) {
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN && gameStatus == 0) {
            GLdouble projection[16];
            glGetDoublev(GL_PROJECTION_MATRIX, projection);


            GLdouble modelvieww[16];
            glGetDoublev(GL_MODELVIEW_MATRIX, modelvieww);

            GLint view[4];
            glGetIntegerv(GL_VIEWPORT, view);

            GLdouble objx1, objx2, objy1, objy2, objz1, objz2, z;
            tItem aux;
            GLfloat winx, winy, winz;

            winx = x;
            winy = view[3] - y;
            glReadPixels(winx, winy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winz);

            gluUnProject(winx, winy, winz, modelvieww, projection, view, &objx1, &objy1, &objz1);

            aux.x = objx1;
            aux.y = 0;
            aux.z = objz1;
            tItem* aux2;
            aux.time = 0;

            aux2 = current2Queue(&mousePosition);
            if (aux2 == NULL) {
                glutTimerFunc(0, controlFloorPlan, 5);
                goBackToCurrentInitialQueue2(&mousePosition);
            }
            if (aux.x<50 && aux.x>-50 && aux.z<50 && aux.z>-50) {
                inexistencet(&mousePosition, aux);
            }
            glutPostRedisplay();
        }

        break;


    }

}
int main(int num, char** arg) {

    glutInit(&num, arg);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(0, 0);
    glutInitWindowSize(400, 500);
    mainWindow = glutCreateWindow("humanoid");
    start();
    light();
    createMenu();

    glutFullScreen();

    glutDisplayFunc(draw);
    glutMouseFunc(catchClick);
    glutKeyboardFunc(withWalk);
    glutSpecialFunc(withWalk2);
    glutReshapeFunc(defineWindow);
    glutMainLoop();
    return 0;
}
