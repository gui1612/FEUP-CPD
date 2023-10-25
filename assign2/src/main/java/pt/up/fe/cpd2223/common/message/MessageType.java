package pt.up.fe.cpd2223.common.message;

public enum MessageType {

    AUTH_LOGIN(1),
    AUTH_REGISTER(2),
    ACK(3),
    NACK(4),
    ENQUEUE_USER(5),
    GAME_JOINED(6),
    USER_DISCONNECTED(7),
    PLAYER_TO_MOVE(8),
    MOVE(9),
    GAME_WON(10),
    GAME_DRAW(11),
    AUTHENTICATED(12),
    UNKNOWN(Integer.MAX_VALUE)
    ;

    private final int identifier;

    MessageType(int identifier) {
        this.identifier = identifier;
    }

    public int getIdentifier() {
        return this.identifier;
    }

    static MessageType from(int typeIdentifier) {
        return switch (typeIdentifier) {
            case 1 -> MessageType.AUTH_LOGIN;
            case 2 -> MessageType.AUTH_REGISTER;
            case 3 -> MessageType.ACK;
            case 4 -> MessageType.NACK;
            case 5 -> MessageType.ENQUEUE_USER;
            case 6 -> MessageType.GAME_JOINED;
            case 7 -> MessageType.USER_DISCONNECTED;
            case 8 -> MessageType.PLAYER_TO_MOVE;
            case 9 -> MessageType.MOVE;
            case 10 -> MessageType.GAME_WON;
            case 11 -> MessageType.GAME_DRAW;
            case 12 -> MessageType.AUTHENTICATED;
            default -> MessageType.UNKNOWN;
        };
    }
}
