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

    for (i = 0; i < mx_size; ++i)
        for (j = 0; j < mx_size; ++j)
            (*phc)[i * mx_size + j] = (double)(i+1);
}

void cleanupMatrices(double *pha, double *phb, double *phc) {
    delete pha;
    delete phb;
    delete phc;
}

void OnMult(int mx_size)
{

    SYSTEMTIME Time1, Time2;

    char st[100];
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

    // display 10 elements of the result matrix to verify correctness
    cout << "Result matrix: " << endl;
    for (i = 0; i < 1; i++)
        for (j = 0; j < min(10, mx_size); j++)
            cout << phc[j] << " ";
    cout << endl;

    // Free memory used by matrices
    cleanupMatrices(pha, phb, phc);
}

// add code here for line x line matrix multiplication
void OnMultLine(int mx_size) {

    SYSTEMTIME Time1, Time2;

    char st[100];
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

    // display 10 elements of the result matrix to verify correctness
    cout << "Result matrix: " << endl;
    for (i = 0; i < 1; ++i)
        for (j = 0; j < min(10, mx_size); ++j)
            cout << phc[j] << " ";
    cout << endl;

    // Free memory used by matrices
    cleanupMatrices(pha, phb, phc);
}

// add code here for block x block matrix multiplication
void OnMultBlock(int mx_size, int bkSize) {
    SYSTEMTIME Time1, Time2;

    char st[100];
    int i, j, k;

    double *pha, *phb, *phc;

    // Initialize matrices
    setupMatrices(mx_size, &pha, &phb, &phc);

    Time1 = clock();

    //code for block x block matrix multiplication

    int side_len_blocks = mx_size / bkSize;

    double *block_a, *block_b;

    block_a = new double[bkSize * bkSize];
    block_b = new double[bkSize * bkSize];

    for (int block_row_c = 0; block_row_c < side_len_blocks; ++block_row_c) {
        for (int block_col_c = 0; block_col_c < side_len_blocks; ++block_col_c) {
            
            // with this we select the block in the destiny matrix
            uint8_t block_y_offset_c = block_row_c * bkSize;
            uint8_t block_x_offset_c = block_col_c * bkSize;

            // since this is a matrix multiplication, we have to traverse the matrix side in blocks

            for (int block_k = 0; block_k < side_len_blocks; ++block_k) {
                int row, col;

                int x_a, y_a, x_b, y_b;

                // pick the block in matrix A
                uint8_t block_row_a = block_row_c;
                uint8_t block_col_a = block_k;

                // pick the block in matrix B
                uint8_t block_row_b = block_k;
                uint8_t block_col_b = block_col_c;

                // the computed offset of Matrix A's block
                uint8_t block_x_offset_a = block_col_a * bkSize;
                uint8_t block_y_offset_a = block_row_a * bkSize;

                // the computed offset of Matrix B's block
                uint8_t block_x_offset_b = block_col_b * bkSize;
                uint8_t block_y_offset_b = block_row_b * bkSize;

                // TODO: instead of populating the blocks, we could spend some more time thinking about this and figure out how to do the product straight away

                // populate blocks
                for (row = 0; row < bkSize; ++row){
                    for (col = 0; col < bkSize; ++col) {

                        y_a = block_y_offset_a + row;
                        x_a = block_x_offset_a + col;

                        y_b = block_y_offset_b + row;
                        x_b = block_x_offset_b + col;

                        block_a[row * bkSize + col] = pha[y_a * mx_size + x_a];
                        block_b[row * bkSize + col] = pha[y_b * mx_size + x_b];
                    }
                }

                for (i = 0; i < bkSize; ++i)
                    for (k = 0; k < bkSize; ++k)
                        for (j = 0; j < bkSize; ++j) {

                            int x_c, y_c;

                            y_c = block_y_offset_c + i;
                            x_c = block_x_offset_c + j;

                            phc[y_c * mx_size + x_c] += block_a[i * bkSize + k] * block_b[k * bkSize + j];
                        }
            }
        }
    }

    delete block_a;
    delete block_b;

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

    int mx_size, blockSize;
    int op;

    int EventSet = PAPI_NULL;
    long long values[2] = {0};
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
        cout << "0. Quit" << endl;
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
