lp = atoi(tab[1]);
printf("Liczba procesów potomnych: %d\n", lp);
fflush(stdout);
double krok=0.000000001;
for(int i=0;i<lp;i++) {
    x0 = (double)i/lp;
    x1 = (double)(i+1)/lp;
    pid = fork();
    if (pid == 0) {
        licz(i, x0 x1, krok);
        sleep(i);
        printf("proc: %d\n", i);
        fflush(stdout);
        return 0;
    }
}
printf("przed\n");
fflush(stdout);
while(wait(0) > 0);
printf("po\n");
fflush(stdout);
