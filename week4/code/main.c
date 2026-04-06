#include <stdio.h>
#include <time.h>

// KIT205 week 4 lab - timing functions to check big O predictions
// Samsul Alam 759423

// pasted functions from the tutorial sheet

long f1(long n){
    long k = 0;
    for (long i = 0; i < n; i++){
        for (long j = 0; j < n; j++){
            k++;
        }
    }
    return k;
}

void f2(long n){
    long k = 0;
    for (long i = 0; i < n; i++){
        for (long j = 0; j < i; j++){
            k++;
        }
        for (long t = 0; t < 10000; t++){
            k++;
        }
    }
}

void f3(long n){
    if (n > 0){
        f3(n / 2);
        f3(n / 2);
    }
}

void f4(long n){
    if (n > 0){
        f4(n / 2);
        f4(n / 2);
        f2(n);
    }
}

void f5(long n){
    long k = 0;
    for (long i = 0; i < 10; i++){
        long j = n;
        while (j > 0){
            f1(1000);
            k++;
            j = j / 2;
        }
    }
}

void f6(long n){
    f2(n);
    f3(n);
    f5(n);
}

void f7(long n){
    long k = 0;
    for (long i = 0; i < f1(n); i++){
        k++;
    }
    for (long j = 0; j < n; j++){
        k++;
    }
}

int main(){

    long n;
    clock_t start;
    clock_t diff;
    long msec;

    // my theoretical analysis before running anything:
    // f1 - two loops both 0 to n, so n*n = O(n^2)
    // f2 - first inner loop is 0 to i, thats arithmetic series so n^2/2,
    //      plus the 10000 loop which is just a constant. so still O(n^2)
    // f3 - two recursive calls each halving n, no loop work at all
    //      T(n) = 2T(n/2) + O(1), master theorem a=2 b=2 log_b(a)=1
    //      f(n)=1 which is less than n^1 so case 1, answer is O(n)
    // f4 - same recursion as f3 but also calls f2(n) each time
    //      T(n) = 2T(n/2) + O(n^2), f(n)=n^2 bigger than n^log_b(a)=n
    //      so case 3, answer O(n^2)
    // f5 - outer loop is just 10 (constant), inner while halves j each time
    //      so log(n) iterations, each calls f1(1000) which is constant
    //      total = 10 * logn * constant = O(log n)
    // f6 - calls f2 + f3 + f5, f2 is O(n^2) which dominates, so O(n^2)
    // f7 - i thought O(n^2) first but actually f1(n) costs O(n^2) to call
    //      and then the loop runs n^2 times too, so its O(n^2) * O(n^2) = O(n^4)
    //      the timing data showed this clearly, n=200 was way more than 4x n=100

    printf("n\t\ttime(ms)\n");

    // --- f1 ---
    // starting at 1000 and doubling. if O(n^2) then time should go up 4x each time
    printf("\nf1 - expect O(n^2)\n");
    long vals1[] = {1000, 2000, 4000, 8000, 16000, 32000};
    int len1 = 6;
    for (int x = 0; x < len1; x++){
        n = vals1[x];
        start = clock();
        f1(n);
        diff = clock() - start;
        msec = diff * 1000 / CLOCKS_PER_SEC;
        printf("%ld\t\t%ld\n", n, msec);
    }

    // --- f2 ---
    // also O(n^2) but the 10000 constant loop might make it slower than f1
    // using same n values to compare
    printf("\nf2 - expect O(n^2)\n");
    for (int x = 0; x < len1; x++){
        n = vals1[x];
        start = clock();
        f2(n);
        diff = clock() - start;
        msec = diff * 1000 / CLOCKS_PER_SEC;
        printf("%ld\t\t%ld\n", n, msec);
    }

    // --- f3 ---
    // expect O(n), kept n smaller here because it recurses a lot
    // if linear then doubling n should double the time
    printf("\nf3 - expect O(n)\n");
    long vals3[] = {100, 200, 400, 800, 1600, 3200, 6400};
    int len3 = 7;
    for (int x = 0; x < len3; x++){
        n = vals3[x];
        start = clock();
        f3(n);
        diff = clock() - start;
        msec = diff * 1000 / CLOCKS_PER_SEC;
        printf("%ld\t\t%ld\n", n, msec);
    }

    // --- f4 ---
    // expect O(n^2) but kept n small because it calls f2 inside recursion
    // tried bigger values first and it took forever so dropped it way down
    printf("\nf4 - expect O(n^2) - using small n, gets slow fast\n");
    long vals4[] = {50, 100, 200, 400, 800};
    int len4 = 5;
    for (int x = 0; x < len4; x++){
        n = vals4[x];
        start = clock();
        f4(n);
        diff = clock() - start;
        msec = diff * 1000 / CLOCKS_PER_SEC;
        printf("%ld\t\t%ld\n", n, msec);
    }

    // --- f5 ---
    // expect O(log n) so using much bigger n values
    // time should barely change even as n gets huge
    printf("\nf5 - expect O(log n)\n");
    long vals5[] = {100, 1000, 10000, 100000, 1000000};
    int len5 = 5;
    for (int x = 0; x < len5; x++){
        n = vals5[x];
        start = clock();
        f5(n);
        diff = clock() - start;
        msec = diff * 1000 / CLOCKS_PER_SEC;
        printf("%ld\t\t%ld\n", n, msec);
    }

    // --- f6 ---
    // just calls f2+f3+f5, f2 should dominate so O(n^2)
    printf("\nf6 - expect O(n^2)\n");
    long vals6[] = {1000, 2000, 4000, 8000, 16000};
    int len6 = 5;
    for (int x = 0; x < len6; x++){
        n = vals6[x];
        start = clock();
        f6(n);
        diff = clock() - start;
        msec = diff * 1000 / CLOCKS_PER_SEC;
        printf("%ld\t\t%ld\n", n, msec);
    }

    // --- f7 ---
    // originally thought O(n^2) but its actually O(n^4)
    // f1(n) is called to get the loop limit which already costs O(n^2)
    // then the loop runs n^2 times on top of that
    // so total is O(n^2) x O(n^2) = O(n^4)
    // n=400 took 13 seconds which proves it grows way faster than quadratic
    // keeping n very small here
    printf("\nf7 - actually O(n^4) not O(n^2) - very small n only\n");
    long vals7[] = {10, 20, 40, 80};
    int len7 = 4;
    for (int x = 0; x < len7; x++){
        n = vals7[x];
        start = clock();
        f7(n);
        diff = clock() - start;
        msec = diff * 1000 / CLOCKS_PER_SEC;
        printf("%ld\t\t%ld\n", n, msec);
    }

    // reflection:
    // most predictions matched the timing data pretty well
    // f1 and f2 both showed roughly 4x growth when n doubled which confirms O(n^2)
    // f2 was slower than f1 for same n because of the constant 10000 inner loop
    // same complexity class but constants still matter in practice
    // f3 times were very small and hard to read clearly but growth looked linear
    // f4 got slow really fast even with small n, the recursion plus f2 adds up quick
    // f5 was the most interesting - barely changed even going from n=100 to n=1000000
    //    that really shows what O(log n) looks like in practice
    // f6 followed f2 pattern which makes sense since f2 dominates
    // f7 was wrong in my original prediction - i said O(n^2) but it was actually O(n^4)
    //    n=200 was about 15x slower than n=100, not 4x, which gave it away
    //    the mistake was not counting the cost of calling f1(n) inside the loop condition
    //    f1(n) itself runs in O(n^2) so the total multiplies out to O(n^4)

    return 0;
}