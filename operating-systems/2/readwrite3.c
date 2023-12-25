semaphore_t priowriter=1, w=1, r=1;
semaphore_t mutexrc=1, mutexwc=1;
int rc = 0, wc = 0;

/* Leser-Prozess */
while(1) {
  P(priowriter);
  P(r);
  P(mutexrc);
  rc++;
  if (rc == 1)
    P(w);
  V(mutexrc);
  V(r);
  V(priowriter);
  /* lesender Zugriff */
  P(mutexrc);
  rc--;
  if (rc == 0)
    V(w);
  V(mutexrc);
 }

/* Schreiber-Prozess */
while(1) {
  P(mutexwc);
  wc++;
  if (wc == 1)
    P(r);
  V(mutexwc);
  P(w);
  /* schreibender Zugriff */
  V(w);
  P(mutexwc);
  wc--;
  if (wc == 0)
    V(r);
  V(mutexwc);
}
