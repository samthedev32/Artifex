/**
 * @brief Simple Program to demonstrate *Typeless*, the ECS of this engine
 *
 * @author SamTheDev
 */

#include <Artifex/ecs/typeless.h>

#include <stdio.h>

int main() {
  Typeless tl;
  tlInitialize(&tl);

  tlID v[10];
  
  int a;
  v[0] = tlAddValue(tl, &a, sizeof(int));
  v[1] = tlAddValue(tl, &a, sizeof(int));
  v[2] = tlAddValue(tl, &a, sizeof(int));
  v[3] = tlAddValue(tl, &a, sizeof(int));
  v[4] = tlAddValue(tl, &a, sizeof(int));
  v[5] = tlAddValue(tl, &a, sizeof(int));
  v[6] = tlAddValue(tl, &a, sizeof(int));
  v[7] = tlAddValue(tl, &a, sizeof(int));
  v[8] = tlAddValue(tl, &a, sizeof(int));
  v[9] = tlAddValue(tl, &a, sizeof(int));

  for (int i = 0; i < 10; i++)
    printf("%u ", TL_ID_VALUE(v[i]));
    printf("\n");

  tlDelValue(tl, v[0]);
  tlDelValue(tl, v[2]);
  tlDelValue(tl, v[3]);
  tlDelValue(tl, v[7]);

  v[0] = tlAddValue(tl, &a, sizeof(int));
  v[2] = tlAddValue(tl, &a, sizeof(int));
  v[3] = tlAddValue(tl, &a, sizeof(int));
  v[7] = tlAddValue(tl, &a, sizeof(int));

  for (int i = 0; i < 10; i++)
    printf("%u ", TL_ID_VALUE(v[i]));
    printf("\n");

  tlID e[5];
  e[0] = tlAddEntity(tl, 8);
  e[1] = tlAddEntity(tl, 8);
  e[2] = tlAddEntity(tl, 8);
  e[3] = tlAddEntity(tl, 8);
  e[4] = tlAddEntity(tl, 0);

  uint8_t len;
  tlID *data;
  tlGetEntity(tl, e[0], &data, &len);
  data[0] = TL_ID_VALUE(v[0]);
  data[1] = TL_ID_VALUE(v[1]);
  data[2] = TL_ID_VALUE(v[2]);

  len = 0;
  data = NULL;

  tlGetEntity(tl, e[0], &data, &len);
  printf("%u: %u %u %u ...\n", len, data[0], data[1], data[2]);

  for (int i = 0; i < 5; i++)
    tlDelEntity(tl, e[i]);

  for (int i = 0; i < 10; i++)
    tlDelValue(tl, v[i]);

  tlDestroy(&tl);
 
  return 0;
}
