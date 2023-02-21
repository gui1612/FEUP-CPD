#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <cstdlib>
#include <papi.h>

using namespace std;

#define SYSTEMTIME clock_t

void setupMatrices(int mx_size, double **pha, double **phb, double **phc) {
    int i,j;

    *pha = new double[mx_size * mx_size];
    *phb = new double[mx_size * mx_size];
    *phc = new double[mx_size * mx_size];

    for (i = 0; i < mx_size; ++i)
        for (j = 0; j < mx_size; ++j)
            (*pha)[i * mx_size + j] = (double)1.0;

    for (i = 0; i < mx_size; ++i)
        for (j = 0; j < mx_size; ++j)
            (*phb)[i * mx_size + j] = (double)(i+1);
}

void cleanupMatrices(double *pha, double *phb, double *phc) {
    free(pha);
    free(phb);
    free(phc);
}

void OnMult(int mx_size)
{

    SYSTEMTIME Time1, Time2;

    char st[100];
    double temp;
    int i, j, k;

    double *pha, *phb, *phc;

    // Initialize matrices
    setupMatrices(mx_size, &pha, &phb, &phc);

    Time1 = clock();

    for (i = 0; i < mx_size; ++i)
        for (j = 0; j < mx_size; ++j)
            for (k = 0; k < mx_size; ++k)
                phc[i * mx_size + j] += pha[i * mx_size + k] * phb[k * mx_size + j];

    Time2 = clock();
    sprintf(st, "Time: %3.3f seconds\n", (double)(Time2 - Time1) / CLOCKS_PER_SEC);
    cout << st;

    // display 10 elements of the result matrix tto verify correctness
    cout << "Result matrix: " << endl;
    for (i = 0; i < 1; i++)
        for (j = 0; j < min(10, mx_size); j++)
            cout << phc[j] << " ";
    cout << endl;

    // Free memory used by matrices
    cleanupMatrices(pha, phb, phc);
}

// add code here for line x line matriz multiplication
void OnMultLine(int mx_size) {

    SYSTEMTIME Time1, Time2;

    char st[100];
    double temp;
    int i, j, k;

    double *pha, *phb, *phc;

    // Initialize matrices
    setupMatrices(mx_size, &pha, &phb, &phc);

    Time1 = clock();

    for (i = 0; i < mx_size; ++i)
        for (k = 0; k < mx_size; ++k)
            for (j = 0; j < mx_size; ++j)
                phc[i * mx_size + j] += pha[i * mx_size + k] * phb[k * mx_size + j];

    Time2 = clock();
    sprintf(st, "Time: %3.3f seconds\n", (double)(Time2 - Time1) / CLOCKS_PER_SEC);
    cout << st;

    // display 10 elements of the result matrix tto verify correctness
    cout << "Result matrix: " << endl;
    for (i = 0; i < 1; ++i)
        for (j = 0; j < min(10, mx_size); ++j)
            cout << phc[j] << " ";
    cout << endl;

    // Free memory used by matrices
    cleanupMatrices(pha, phb, phc);
}

// add code here for block x block matriz multiplication
void OnMultBlock(int mx_size, int bkSize) {
    SYSTEMTIME Time1, Time2;

    char st[100];
    double temp;
    int i, j;

    double *pha, *phb, *phc;

    // Initialize matrices
    setupMatrices(mx_size, &pha, &phb, &phc);

    Time1 = clock();

    //code for block x block matrix multiplication


    Time2 = clock();
    sprintf(st, "Time: %3.3f seconds\n", (double)(Time2 - Time1) / CLOCKS_PER_SEC);
    cout << st;

    // display 10 elements of the result matrix tto verify correctness
    cout << "Result matrix: " << endl;
    for (i = 0; i < 1; ++i)
        for (j = 0; j < min(10, mx_size); ++j)
            cout << phc[j] << " ";

    cout << endl;

    // Free memory used by matrices
    cleanupMatrices(pha, phb, phc);
}

void handle_error(int retval) {
    printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
    exit(1);
}

void init_papi() {
    int retval = PAPI_library_init(PAPI_VER_CURRENT);
    
    if (retval != PAPI_VER_CURRENT && retval < 0) {
        printf("PAPI library version mismatch!\n");
        exit(1);
    }

    if (retval < 0)
        handle_error(retval);

    std::cout << "PAPI Version Number: MAJOR: " << PAPI_VERSION_MAJOR(retval)
              << " MINOR: " << PAPI_VERSION_MINOR(retval)
              << " REVISION: " << PAPI_VERSION_REVISION(retval) << "\n";
}

int main(int argc, char *argv[]) {

    char c;
    int mx_size, blockSize;
    int op;

    int EventSet = PAPI_NULL;
    long long values[2];
    int ret;

    ret = PAPI_library_init(PAPI_VER_CURRENT);
    if (ret != PAPI_VER_CURRENT)
        std::cout << "FAIL" << endl;

    ret = PAPI_create_eventset(&EventSet);
    if (ret != PAPI_OK)
        cout << "ERROR: create eventset" << endl;

    ret = PAPI_add_event(EventSet, PAPI_L1_DCM);
    if (ret != PAPI_OK)
        cout << "ERROR: PAPI_L1_DCM" << endl;

    ret = PAPI_add_event(EventSet, PAPI_L2_DCM);
    if (ret != PAPI_OK)
        cout << "ERROR: PAPI_L2_DCM" << endl;

    op = 1;
    do {
        cout << endl
             << "1. Multiplication" << endl;
        cout << "2. Line Multiplication" << endl;
        cout << "3. Block Multiplication" << endl;
        cout << "Selection?: ";
        cin >> op;
        if (op == 0)
            break;
        printf("Dimensions: lins=cols ? ");
        cin >> mx_size;

        // Start counting
        ret = PAPI_start(EventSet);
        if (ret != PAPI_OK)
            cout << "ERROR: Start PAPI" << endl;

        switch (op) {
        case 1:
            OnMult(mx_size);
            break;
        case 2:
            OnMultLine(mx_size);
            break;
        case 3:
            cout << "Block Size? ";
            cin >> blockSize;
            OnMultBlock(mx_size, blockSize);
            break;
        }

        ret = PAPI_stop(EventSet, values);
        if (ret != PAPI_OK)
            cout << "ERROR: Stop PAPI" << endl;
        printf("L1 DCM: %lld \n", values[0]);
        printf("L2 DCM: %lld \n", values[1]);

        ret = PAPI_reset(EventSet);
        if (ret != PAPI_OK)
            std::cout << "FAIL reset" << endl;

    } while (op != 0);

    ret = PAPI_remove_event(EventSet, PAPI_L1_DCM);
    if (ret != PAPI_OK)
        std::cout << "FAIL remove event" << endl;

    ret = PAPI_remove_event(EventSet, PAPI_L2_DCM);
    if (ret != PAPI_OK)
        std::cout << "FAIL remove event" << endl;

    ret = PAPI_destroy_eventset(&EventSet);
    if (ret != PAPI_OK)
        std::cout << "FAIL destroy" << endl;
}
