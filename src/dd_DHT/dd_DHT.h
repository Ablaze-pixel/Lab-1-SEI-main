#ifndef DD_DHT_H
#define DD_DHT_H

#include <stdint.h>
#include <stdbool.h>

// Inițializează driverul DHT22.
// Pinul de date este definit intern cu un macro în implementare.
void dd_dht_setup(void);

// Citește temperatura și umiditatea de la DHT22.
// Returnează true dacă citirea a reușit, false altfel.
bool dd_dht_read(float *temperature, float *humidity);

#endif // DD_DHT_H
