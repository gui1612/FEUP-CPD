package pt.up.fe.cpd2223;

import pt.up.fe.cpd2223.client.Client;
import pt.up.fe.cpd2223.server.Server;

import java.util.Arrays;

public class Main {

    public interface Application extends Runnable {}

    public static void main(String[] args) throws Exception {

        if (args.length < 1) {
            System.err.println("Error: should specify type of application to run");
            System.exit(1);
        }

        String type = args[0];

        var executableArgs = Arrays.copyOfRange(args, 1, args.length);

        System.out.println("Running " + type + " with args: " + Arrays.toString(executableArgs));

        Application app = switch (type.toLowerCase()) {
            case "server" -> Server.configure(executableArgs);
            case "client" -> Client.create(executableArgs);
            default -> throw new Exception("Type must be one of [server|client] (case-insensitive)");
        };

        if (app == null)
            throw new Exception("Error creating app executable, aborting");

        app.run();
    }
}