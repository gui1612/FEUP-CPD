package pt.up.fe.cpd2223.common.message;

import java.nio.channels.SocketChannel;
import java.util.HashMap;

public abstract class Message {

    private SocketChannel socket;

    public static Message fromFormattedString(String message) {
        String[] parts = message.split(Message.metadataSeparator());

        int type = Integer.parseInt(parts[0]);

        var messageType = MessageType.from(type);

        return switch (messageType) {
            case AUTH_LOGIN -> {
                var payload = parts[1];

                var info = payload.split(Message.payloadDataSeparator());

                // TODO: Handle malformed messages
                String username = info[0], password = info[1];

                yield new LoginMessage(username, password);
            }
            case AUTH_REGISTER -> {
                var payload = parts[1];

                var info = payload.split(Message.payloadDataSeparator());

                String username = info[0], password = info[1];

                yield new RegisterMessage(username, password);
            }
            case ACK -> {
                var additionalData = parts[1];

                if (additionalData.equals("null")) yield new AckMessage();

                var dataMap = new HashMap<String, Object>();

                for (var entry : additionalData.split(Message.payloadDataSeparator())) {

                    var entryArr = entry.split("=");
                    String key = entryArr[0], value = entryArr[1];

                    dataMap.put(key, value);
                }

                yield new AckMessage(dataMap);
            }
            case NACK -> new NackMessage();
            case ENQUEUE_USER -> {
                long userId = Long.parseLong(parts[1]);

                yield new EnqueueUserMessage(userId);
            }
            case GAME_JOINED -> {

                yield new GameJoinedMessage();
            }
            case USER_DISCONNECTED -> new UserDisconnectMessage();
            case PLAYER_TO_MOVE -> {
                int playerId = Integer.parseInt(parts[1]);

                yield new PlayerToMoveMessage(playerId);
            }
            case MOVE -> {
                var payload = parts[1];

                var info = payload.split(Message.payloadDataSeparator());

                long userId = Long.parseLong(info[0]);
                int x = Integer.parseInt(info[1]), y = Integer.parseInt(info[2]);

                yield new MoveMessage(x, y, userId);
            }
            case AUTHENTICATED -> {
                long userId = Long.parseLong(parts[1]);

                yield new AuthenticatedMessage(userId);
            }
            case GAME_WON -> {
                var payload = parts[1];

                var info = payload.split(Message.payloadDataSeparator());

                int winnerId = Integer.parseInt(info[0]);

                yield new GameWonMessage(winnerId);
            }
            case GAME_DRAW -> new GameDrawMessage();
            default -> new UnknownMessage();
        };
    }

    public static String payloadDataSeparator() {
        return ";";
    }

    public static String messageDelimiter() {
        return "\n";
    }

    public static String metadataSeparator() {
        return ":";
    }

    public Message withChannel(SocketChannel channel) {
        this.socket = channel;
        return this;
    }

    public SocketChannel getClientSocket() {
        return this.socket;
    }

    public abstract MessageType type();

    public abstract String payload();

    public String toFormattedString() {
        return "%d%s%s%s".formatted(this.type().getIdentifier(), Message.metadataSeparator(), this.payload(), Message.messageDelimiter());
    }
}
