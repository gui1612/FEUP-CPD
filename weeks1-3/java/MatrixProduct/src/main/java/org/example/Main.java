package org.example;

import java.util.Scanner;

public class Main {
    public static void onMult(int lin, int col) {
        System.out.println("onMult function");
    }

    public static void onMultLine(int lin, int col) {
        System.out.println("onMultLine function");
    }

    public static void onMultBlock(int lin, int col, int blockSize) {
        System.out.println("onMultBlock function");
    }
    public static void main(String[] args) {
        char c;
        int lin, col, blockSize;
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
            lin = scanner.nextInt();
            col = lin;

            switch (op) {
                case 1:
                    onMult(lin, col);
                    break;
                case 2:
                    onMultLine(lin, col);
                    break;
                case 3:
                    System.out.print("Block Size? ");
                    blockSize = scanner.nextInt();
                    onMultBlock(lin, col, blockSize);
                    break;
            }

        } while (true);

        scanner.close();
    }
}