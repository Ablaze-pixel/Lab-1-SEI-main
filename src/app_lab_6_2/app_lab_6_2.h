#ifndef APP_LAB_6_2_H
#define APP_LAB_6_2_H

// Stările clare ale intersecției (Combinații de culori pentru ambele direcții)
#define STATE_EAST_GREEN_NORTH_RED    0
#define STATE_EAST_YELLOW_NORTH_RED   1
#define STATE_EAST_RED_NORTH_GREEN    2
#define STATE_EAST_RED_NORTH_YELLOW   3

// Evenimentele care forțează tranzițiile
#define EVENT_TRAFFIC_REQUEST         1  // S-a detectat mașină la Nord
#define EVENT_STEP_SEQUENCE           2  // Trecerea la următorul pas din secvență

void app_lab_6_2_setup(void);
void app_lab_6_2_run(void);

#endif