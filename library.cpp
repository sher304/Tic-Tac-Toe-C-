#include "Main.h"
#include "string"

int boardSize = 5;
std::vector<std::vector<int>> board;

int currentPlayer = 1;
bool gameOver = false;
int cursorRow = 0;
int cursorColumn = 0;


void initializeBoard(int size) {
    boardSize = size;
    board = std::vector<std::vector<int>>(boardSize, std::vector<int>(boardSize, 0));
}

bool checkHorizontally(int symbolValue) {
    for (int i = 0; i < boardSize; i++) {
        int valueCounter = 0;
        for (int j = 0; j < boardSize; j++) {
            valueCounter = (board[i][j] == symbolValue) ? valueCounter + 1 : 0;
            if (valueCounter == 4) return true;
        }
    }
    return false;
}

bool checkVertically(int symbolValue) {
    for (int i = 0; i < boardSize; i++) {
        int valueCounter = 0;
        for (int j = 0; j < boardSize; j++) {
            valueCounter = (board[j][i] == symbolValue) ? valueCounter + 1 : 0;
            if (valueCounter == 4) return true;
        }
    }
    return false;
}

bool checkDiagonalLeftTopRightBottom(int player) {
    for (int i = 0; i <= boardSize - 4; i++) {
        for (int j = 0; j <= boardSize - 4; j++) {
            int valueCounter = 0;
            for (int k = 0; k < 4; k++) {
                if (board[i + k][j + k] == player) valueCounter++;
                else valueCounter = 0;
                if (valueCounter == 4) return true;
            }
        }
    }
    return false;
}

bool checkDiagonalLeftBottomRightTop(int player) {
    for (int i = 0; i <= boardSize - 4; i++) {
        for (int j = boardSize - 1; j >= 3; j--) {
            int valueCounter = 0;
            for (int k = 0; k < 4; k++) {
                if (board[i + k][j - k] == player) valueCounter++;
                else valueCounter = 0;
                if (valueCounter == 4) return true;
            }
        }
    }
    return false;
}


bool winValidator() {
    if (checkHorizontally(currentPlayer) || checkVertically(currentPlayer)
        || checkDiagonalLeftTopRightBottom(currentPlayer) || checkDiagonalLeftBottomRightTop(currentPlayer)) {
        gameOver = true;
        return true;
    }
    return false;
}

void makeMovement(int row, int column, int player) {
    if (board[row][column] == 0) {
        board[row][column] = player;
        if (winValidator()) {
            gameOver = true;
        } else {
            currentPlayer = (currentPlayer == 1) ? 2 : 1;
        }
    }
}

JNIEXPORT void JNICALL Java_Main_setBoardSize
    (JNIEnv *env, jobject jobj, jint size) {
    initializeBoard(size);
}


JNIEXPORT jobjectArray JNICALL Java_Main_getBoard(JNIEnv *env, jobject obj) {
    jobjectArray boardArray = env->NewObjectArray(boardSize, env->FindClass("[I"), nullptr);
    for (int i = 0; i < boardSize; i++) {
        jintArray row = env->NewIntArray(boardSize);
        env->SetIntArrayRegion(row, 0, boardSize, board[i].data());
        env->SetObjectArrayElement(boardArray, i, row);
        env->DeleteLocalRef(row);
    }
    return boardArray;
}

JNIEXPORT void JNICALL Java_Main_makeMovement(JNIEnv *env, jobject obj, jint row, jint column, jint player) {
    makeMovement(row, column, player);
}

JNIEXPORT jboolean JNICALL Java_Main_gameOver(JNIEnv *env, jobject obj) {
    return gameOver;
}

JNIEXPORT jint JNICALL Java_Main_getNextPlayer(JNIEnv *env, jobject obj) {
    return currentPlayer;
}

JNIEXPORT jint JNICALL Java_Main_getDimension
        (JNIEnv *env, jobject obj, jstring name, jboolean isRow) {
    const char *nameStrPtr = env->GetStringUTFChars(name, nullptr);
    std::string buttonValues(nameStrPtr);
    std::string valueStr = isRow ? buttonValues.substr(0, 1) : buttonValues.substr(1 + 1);
    return std::stoi(valueStr);
}

JNIEXPORT jstring JNICALL Java_Main_getPlayerValue
        (JNIEnv *env, jobject obj, jint value) {
    std::string text = (value == 1) ? "X" : ((value == 2) ? "O" : "");
    return env->NewStringUTF(text.c_str());
}

JNIEXPORT jint JNICALL Java_Main_gameKeyReader
        (JNIEnv *env, jobject jobj, jint keyCode) {
    switch(keyCode) {
        case 0x52:
            // 0x51 Is For R - to restart the game
            board = std::vector<std::vector<int>>(boardSize, std::vector<int>(boardSize, 0));
            gameOver = false;
            cursorRow = 0;
            cursorColumn = 0;
            return 1;
        case 0x51:
            // 0x51 Is For Q - to quite the game
            gameOver = false;
            return -1;
    }
    return 0;
}

JNIEXPORT void JNICALL Java_Main_placeMark
(JNIEnv *env, jobject jobj, jint player) {
    makeMovement(cursorRow, cursorColumn, currentPlayer);
}

JNIEXPORT jint JNICALL Java_Main_getCursorRow
        (JNIEnv *env, jobject jobj) {
        return cursorRow;
}

JNIEXPORT jint JNICALL Java_Main_getCursorColumn
        (JNIEnv *env, jobject jobj) {
    return cursorColumn;
}

JNIEXPORT void JNICALL Java_Main_moveCursor
(JNIEnv *env, jobject jobj, jint direction) {
    switch (direction) {
        case 0: // UP
            if (cursorRow > 0) cursorRow--;
            break;
        case 1: // DOWN
            if (cursorRow < boardSize - 1) cursorRow++;
            break;
        case 2: // LEFT
            if (cursorColumn > 0) cursorColumn--;
            break;
        case 3: // RIGHT
            if (cursorColumn < boardSize - 1) cursorColumn++;
            break;
    }
}