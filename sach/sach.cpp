#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

#include <custom_shaders/chess_vert_glsl.h>
#include <custom_shaders/chess_frag_glsl.h>

#include "CustomMesh.h"

using namespace std;
using namespace glm;
using namespace ppgso;

const unsigned int WINDOW_SIZE = 512;

struct Piece {
    char type, color;

    Piece() : Piece('-', '-') {}

    Piece(char type, char color) : type(type), color(color) {}

    bool operator==(const Piece& rhs) const {
        return type == rhs.type && color == rhs.color;
    }
};

struct Move {
    bool active, finishAfter, gameFinished;

    double startTime, afterFinishTime;

    float toHeight;

    int horizontalUnits, downUnits;
    float rowUnit, colUnit;

    Piece piece;

    mat4 mat;
};

struct Disappear {
    bool active;

    Piece piece;

    mat4 mat;
};

class ChessWindow : public Window {
private:
    static constexpr float FULL_CIRCLE = 2.0f * (float)M_PI;
    static constexpr int CIRCLE_UNITS = 32;
    static constexpr float CAMERA_ROTATION_UNIT = FULL_CIRCLE / CIRCLE_UNITS;

    Shader program = {chess_vert_glsl, chess_frag_glsl};

    CustomMesh pawnW = {"./mesh/structure/pawn-white-sphere.obj"};
    CustomMesh pawnB = {"./mesh/structure/pawn-black-sphere.obj"};

    CustomMesh rookW = {"./mesh/structure/rook-white-cube.obj"};
    CustomMesh rookB = {"./mesh/structure/rook-black-cube.obj"};

    CustomMesh knightW = CustomMesh();
    CustomMesh knightB = {"./mesh/custom/knight-custom-black.obj"};

    CustomMesh bishopW = {"./mesh/structure/bishop-white-cylinder.obj"};
    CustomMesh bishopB = {"./mesh/structure/bishop-black-cylinder.obj"};

    CustomMesh queenW = {"./mesh/structure/queen-white-unwrap.obj"};
    CustomMesh queenB = {"./mesh/structure/queen-black-unwrap.obj"};

    CustomMesh kingW = {"./mesh/normals/king.obj"};
    CustomMesh kingB = {"./mesh/normals/king.obj"};

    CustomMesh quad = {"./mesh/quad.obj"};

    Texture white = {image::loadBMP("./texture/white.bmp")};
    Texture black = {image::loadBMP("./texture/black.bmp")};

    Texture highlight = {image::loadBMP("./texture/green.bmp")};

    Texture board = {image::loadBMP("./texture/woodenboard.bmp")};

    bool isZoomActive = false;
    int angleUnitsY;
    mat4 cameraMat;
    mat4 quadMat;
    mat4 pieceMat;

    Piece pieces[8][8];
    int highlightPos;
    bool isPlayerWhite = true;
    int togoPos = -1;

    Move move = Move();
    Disappear disappear = Disappear();
    vector<mat4> winnerChildren;

    void initCamera(const bool zoom) {
        angleUnitsY = 0;
        cameraMat = mat4{};
        cameraMat = rotate(cameraMat, 5 * CAMERA_ROTATION_UNIT, {1.0f, 0.0f, 0.0f});
        if (zoom) {
            cameraMat = translate(cameraMat, {0.0f, -4.0f, -5.0f});
        }
        else {
            cameraMat = translate(cameraMat, {0.0f, -7.0f, -6.0f});
        }
    }

    mat4 leftDown() {
        return translate(mat4{}, {-3.97f, 0.0f, 2.9f});
    }

    void highlightPiece(const char type) {
        const Piece searchedPiece = Piece(type, isPlayerWhite ? 'w' : 'b');
        int currentPos = (highlightPos + 1) % 64;
        while (currentPos != highlightPos) {
            if (pieces[currentPos / 8][currentPos % 8] == searchedPiece) {
                highlightPos = currentPos;
                return;
            }
            currentPos = (currentPos + 1) % 64;
        }
    }

    CustomMesh& getMesh(const Piece& piece) {
        switch (piece.type) {
            case 'p': {
                if (piece.color == 'w') return pawnW;
                else if (piece.color == 'b') return pawnB;
                else exit(EXIT_FAILURE);
            }
            case 'r': {
                if (piece.color == 'w') return rookW;
                else if (piece.color == 'b') return rookB;
                else exit(EXIT_FAILURE);
            }
            case 'h': {
                if (piece.color == 'w') return knightW;
                else if (piece.color == 'b') return knightB;
                else exit(EXIT_FAILURE);
            }
            case 'b': {
                if (piece.color == 'w') return bishopW;
                else if (piece.color == 'b') return bishopB;
                else exit(EXIT_FAILURE);
            }
            case 'q': {
                if (piece.color == 'w') return queenW;
                else if (piece.color == 'b') return queenB;
                else exit(EXIT_FAILURE);
            }
            case 'k': {
                if (piece.color == 'w') return kingW;
                else if (piece.color == 'b') return kingB;
                else exit(EXIT_FAILURE);
            }
            default: exit(EXIT_FAILURE);
        }
    }

    Texture& getTexture(const Piece& piece) {
        if (piece.color == 'w') return white;
        else if (piece.color == 'b') return black;
        else exit(EXIT_FAILURE);
    }

    void initMove() {
        move.toHeight = 3.0f;
        move.downUnits = 300;

        int rowUnits = ((togoPos % 8) - (highlightPos % 8)) * 100;
        int colUnits = ((togoPos / 8) - (highlightPos / 8)) * 100;

        if (rowUnits == 0) {
            move.rowUnit = 0.0f;
            move.colUnit = -0.01f;
        }
        else {
            move.rowUnit = 0.01f;
            move.colUnit = -0.01f * abs(1.0f * colUnits / rowUnits);
        }

        if (rowUnits < 0) move.rowUnit = -move.rowUnit;
        if (colUnits < 0) move.colUnit = -move.colUnit;

        if (rowUnits < 0) rowUnits = -rowUnits;
        if (colUnits < 0) colUnits = -colUnits;

        if (rowUnits == 0) move.horizontalUnits = colUnits;
        else move.horizontalUnits = rowUnits;

        move.mat = translate(leftDown(), {1.0f * (highlightPos % 8), 0.0f, -1.0f * (highlightPos / 8)});
        move.piece = pieces[highlightPos / 8][highlightPos % 8];
        pieces[highlightPos / 8][highlightPos % 8] = Piece();

        move.finishAfter = (pieces[togoPos / 8][togoPos % 8].type == 'k');
        move.active = true;
        move.startTime = glfwGetTime();
    }

    void doMove() {
        if (move.toHeight > 0.0f) {
            double diffTime = glfwGetTime() - move.startTime;
            double diffHeight = 0.001f + diffTime * 0.005f;
            move.mat = translate(move.mat, {0.0f, diffHeight, 0.0f});
            move.toHeight -= diffHeight;
        }
        else if (move.horizontalUnits > 0) {
            move.mat = translate(move.mat, {move.rowUnit, 0.0f, move.colUnit});
            move.horizontalUnits--;
            if (move.horizontalUnits == 0 && pieces[togoPos / 8][togoPos % 8].type != '-') initDisappear();
        }
        else if (move.downUnits > 0) {
            move.mat = translate(move.mat, {0.0f, -0.01f, 0.0f});
            move.downUnits--;
        }
        else endMove();
    }

    void endMove() {
        disappear = Disappear();

        if (move.finishAfter) {
            move.afterFinishTime = glfwGetTime();
            move.gameFinished = true;
            return;
        }
        pieces[togoPos / 8][togoPos % 8] = move.piece;

        isPlayerWhite = !isPlayerWhite;
        rotateCameraLeft(CIRCLE_UNITS / 2);
        highlightPiece('k');

        togoPos = -1;
        move = Move();
    }

    void initDisappear() {

        disappear.mat = translate(leftDown(), {1.0f * (togoPos % 8), 0.0f, -1.0f * (togoPos / 8)});
        disappear.piece = pieces[togoPos / 8][togoPos % 8];
        pieces[togoPos / 8][togoPos % 8] = Piece();

        disappear.active = true;
    }

    void rotateCameraLeft(int units) {
        angleUnitsY = (angleUnitsY + units) % CIRCLE_UNITS;
        cameraMat = rotate(cameraMat, units * CAMERA_ROTATION_UNIT, {0.0f, 1.0f, 0.0f});
    }

    void rotateCameraUp(int units) {
        cameraMat = rotate(cameraMat, units * CAMERA_ROTATION_UNIT,
                           {cos(angleUnitsY * CAMERA_ROTATION_UNIT), 0.0f, sin(angleUnitsY * CAMERA_ROTATION_UNIT)});
    }

public:
    ChessWindow() : Window{"Chess", WINDOW_SIZE, WINDOW_SIZE} {

        // Enable Z-buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if (row == 0 || row == 1) pieces[row][col].color = 'w';
                else if (row == 6 || row == 7) pieces[row][col].color = 'b';
                if (row == 1 || row == 6) pieces[row][col].type = 'p';
                else if (row == 0 || row == 7) {
                    if (col == 0 || col == 7) pieces[row][col].type = 'r';
                    else if (col == 1 || col == 6) pieces[row][col].type = 'h';
                    else if (col == 2 || col == 5) pieces[row][col].type = 'b';
                    else if (col == 3) pieces[row][col].type = 'q';
                    else if (col == 4) pieces[row][col].type = 'k';
                }
            }
        }
        highlightPiece('k');

        initCamera(isZoomActive);

        quadMat = mat4{};
        quadMat = scale(quadMat, {4.0f, 4.0f, 4.0f});
        quadMat = rotate(quadMat, PI/2, {-1.0f, 0.0f, 0.0f});
        quadMat = translate(quadMat, {0.0f, 0.0f, -0.01f});

        program.setUniform("LightDirection1", normalize(vec3{1.0f, 1.0f, 1.0f}));
        //program.setUniform("LightDirection1", normalize(vec3{-1.0f, 1.0f, 1.0f}));
        program.setUniform("LightDirection2", normalize(vec3{1.0f, 1.0f, -1.0f}));
        //program.setUniform("LightDirection2", normalize(vec3{-1.0f, 1.0f, -1.0f}));
    }

    void onKey(int key, int scanCode, int action, int mods) override {
        // START camera movements
        if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            rotateCameraLeft(1);
        }
        else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            rotateCameraLeft(-1);
        }
        else if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            rotateCameraUp(1);
        }
        else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            rotateCameraUp(-1);
        }
        // END camera movements

        // some key inputs are locked during piece animation
        else if (move.active) return;

        // zoom
        else if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
            isZoomActive = !isZoomActive;
            initCamera(isZoomActive);
        }

        // START piece choice
        else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
            highlightPiece('p');
        }
        else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
            highlightPiece('r');
        }
        else if (key == GLFW_KEY_H && action == GLFW_PRESS) {
            highlightPiece('h');
        }
        else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
            highlightPiece('b');
        }
        else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
            highlightPiece('q');
        }
        else if (key == GLFW_KEY_K && action == GLFW_PRESS) {
            highlightPiece('k');
        }
        // END piece choice

        // move piece
        else if ((key >= GLFW_KEY_0 && key <= GLFW_KEY_7) && action == GLFW_PRESS) {
            if (togoPos == -1) togoPos = (key - GLFW_KEY_0) * 8;
            else {
                togoPos += (key - GLFW_KEY_0);
                // TODO check here whether the move consistent with chess rules
                initMove();
            }
        }
    }

    void onIdle() override {
        // Sky background
        glClearColor(0.055f, 0.671f, 0.961f, 0.5f);
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.setUniform("ViewMatrix", cameraMat);
        program.setUniform("ProjectionMatrix", perspective(35.0f * FULL_CIRCLE / 180.f, 1.0f, 0.1f, 20.0f));

        program.setUniform("Texture", board);
        program.setUniform("ModelMatrix", quadMat);
        quad.render();

        pieceMat = leftDown();
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if (pieces[row][col].type != '-') {
                    if (row == highlightPos / 8 && col == highlightPos % 8) program.setUniform("Texture", highlight);
                    else program.setUniform("Texture", getTexture(pieces[row][col]));
                    program.setUniform("ModelMatrix", pieceMat);
                    getMesh(pieces[row][col]).render();
                }
                pieceMat = translate(pieceMat, {1.0f, 0.0f, 0.0f});
            }
            pieceMat = translate(pieceMat, {-8.0f, 0.0f, -1.0f});
        }

        if (move.active) {
            program.setUniform("Texture", getTexture(move.piece));
            program.setUniform("ModelMatrix", move.mat);
            getMesh(move.piece).render();

            if (move.gameFinished) {
                double dTime = glfwGetTime() - move.afterFinishTime;
                if (dTime > 15.0f) {
                    exit(EXIT_SUCCESS);
                }
                move.mat = translate(move.mat, {0.0f, -0.01f, 0.0f});
                if (winnerChildren.size() < dTime) {
                    winnerChildren.push_back(move.mat);
                }
            }
            else {
                doMove();
            }

            for (int i = 0; i < winnerChildren.size(); i++) {
                float dx, dz;
                if (i % 4 == 0) {
                    dx = 0.01f;
                }
                if (i % 4 == 1) {
                    dz = 0.01f;
                }
                if (i % 4 == 2) {
                    dx = -0.01f;
                }
                if (i % 4 == 3) {
                    dz = -0.01f;
                }
                winnerChildren[i] = translate(winnerChildren[i], {dx, -0.005f, dz});

                program.setUniform("ModelMatrix", winnerChildren[i]);
                getMesh(move.piece).render();
            }

            if (winnerChildren.size() > 4 && winnerChildren.size() % 3 == 2) {
                winnerChildren.push_back(winnerChildren[winnerChildren.size() - 3]);
            }
        }

        if (disappear.active) {
            disappear.mat = scale(disappear.mat, {0.99f, 0.99f, 0.99f});
            program.setUniform("Texture", getTexture(disappear.piece));
            program.setUniform("ModelMatrix", disappear.mat);
            getMesh(disappear.piece).render();
        }
    }
};

int main() {
    ChessWindow window;

    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}
