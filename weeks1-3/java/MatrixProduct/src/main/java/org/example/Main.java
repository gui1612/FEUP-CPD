package org.example;

import java.util.Scanner;

public class Main {
    public static void onMult(int mx_size) {
        int i, j;

        // Setup
        double[][] pha = new double[mx_size][mx_size];
        double[][] phb = new double[mx_size][mx_size];
        double[][] phc = new double[mx_size][mx_size];

        for (i = 0; i < mx_size; ++i)
            for (j = 0; j < mx_size; ++j)
                pha[i][j] = 1.0;

        for (i = 0; i < mx_size; ++i)
            for (j = 0; j < mx_size; ++j)
                phb[i][j] = (double)(i + 1);

        // Start Counting
        long start = System.currentTimeMillis();

        /////////// EXERCISE /////////////

        // Code in here

        //////////////////////////////////

        long end = System.currentTimeMillis();
        double timeElapsed = (double)(end - start)/1000;
        String st = String.format("Time: %3.3f seconds%n", timeElapsed);

        System.out.println("Result matrix: ");
        for (i = 0; i < 1; ++i)
            for (j = 0; j < Math.min(10, mx_size); j++)
                System.out.println(phc[i][j] + " ");

    }

    public static void onMultLine(int mx_size) {
        System.out.println("onMultLine function");
    }

    public static void onMultBlock(int mx_size, int blockSize) {
        System.out.println("onMultBlock function");
    }
    public static void main(String[] args) {
        char c;
        int mx_size, blockSize;
        int op;

        Scanner scanner = new Scanner(System.in);

        do {
            System.out.println("1. Multiplication");
            System.out.println("2. Line Multiplication");
            System.out.println("3. Block Multiplication");
            System.out.print("Selection?: ");

            op = scanner.nextInt();
            if (op == 0)
                break;

            System.out.print("Dimensions: lins=cols ? ");
            mx_size = scanner.nextInt();

            switch (op) {
                case 1:
                    onMult(mx_size);
                    break;
                case 2:
                    onMultLine(mx_size);
                    break;
                case 3:
                    System.out.print("Block Size? ");
                    blockSize = scanner.nextInt();
                    onMultBlock(mx_size, blockSize);
                    break;
            }

        } while (true);

        scanner.close();
    }
}