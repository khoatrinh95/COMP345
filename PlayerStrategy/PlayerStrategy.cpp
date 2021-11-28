//
// Created by Khoa Trinh on 2021-11-21.
//

#include "PlayerStrategy.h"

// THONG
void HumanPlayer::toDefend() {

}

void HumanPlayer::toAttack() {

}

void HumanPlayer::issueOrder() {

}

void AgressivePlayer::toDefend() {

}

void AgressivePlayer::toAttack() {

}

void AgressivePlayer::issueOrder() {

}


// SARAH
void BenevolentPlayer::toDefend() {

}

void BenevolentPlayer::toAttack() {

}

void BenevolentPlayer::issueOrder() {

}

void NeutralPlayer::toDefend() {

}

void NeutralPlayer::toAttack() {

}

void NeutralPlayer::issueOrder() {

}


// KHOA
void CheaterPlayer::toDefend() {

}

void CheaterPlayer::toAttack() {

}

void CheaterPlayer::issueOrder() {

}

PlayerStrategy::PlayerStrategy(Player* p) {
    player = p;
}

PlayerStrategy::~PlayerStrategy() {
    delete player;
}

PlayerStrategy::PlayerStrategy() {}

void PlayerStrategy::setPlayer(Player *p) {
    player = p;
}
