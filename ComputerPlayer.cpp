#include <ComputerPlayer.h>
#include <iostream>

#define INFINTY 900


ComputerPlayer::ComputerPlayer(Board* _board)
{
	board = _board;
}

ComputerPlayer::~ComputerPlayer()
{
	//dtor
}

int ComputerPlayer::putToken()
{
	auto shot = minimaxPlacementPhase(board->vertices, 2, false);
	std::cout << "MINIMAX_SCORE_" << std::to_string(std::get<1>(shot)) << std::endl;
	return std::get<0>(shot);
	
}


//returns <token,score>
std::tuple<int, int> ComputerPlayer::minimaxPlacementPhase(std::array<int, 24> vertices, int depth, bool isMax)
{
	auto best = std::make_tuple(-1, isMax ? -INFINITY : INFINTY);

	//Final leaf or depth reached
	if (depth == 0 || placementPhaseOver(vertices)) {
		auto score = evaluatePlacement(vertices);
		return std::make_tuple(-1, score);
	}
	//Generating children leaves
	for (int i = 0; i < 24; i++) {
		if (vertices[i] == 0) {
			vertices[i] = isMax ? 1 : 2;
			auto score = minimaxPlacementPhase(vertices, depth - 1, !isMax);
			std::cout << "(Token " << std::to_string(i) << " scored " << std::to_string(std::get<1>(score)) << std::endl ;
			vertices[i] = 0;
			std::get<0>(score) = i;
			//minimax
			if (isMax) {
				int newScore = std::get<1>(score);
				int bestScore = std::get<1>(best);
				if (bestScore < newScore) //we're loking forward the max value
					best = score; ; 
			}
			else {
				int newScore = std::get<1>(score);
				int bestScore = std::get<1>(best);
				if (bestScore > newScore) //we're loking forward the min value
					best = score; 
			}
		}
	}
	return best;
}

int ComputerPlayer::evaluatePlacement(std::array<int, 24> vertices) {
	auto score = 0;
	//local evaluation, meaning we're evaluating each token's score
	for (int i = 0; i < 24; i++) {
		if (vertices[i] == 0) continue;
		auto token = i;
		bool computer = vertices[i] == 2;
		if (computer) { //Mini
			score -= morrisClosed(vertices, token);
			score -= numberOfBlockedEnnemyTokens(vertices, token);
			score -= numberOfMillsBlocked(vertices, token);
		}
		else { //Max
			score += morrisClosed(vertices, token);
			score += numberOfBlockedEnnemyTokens(vertices, token);
			score += numberOfMillsBlocked(vertices, token);
		}
			
	}
	//global evaluation
	//Mini
	auto miniNumMills = numberOfMills(vertices,	true);
	auto miniNumTokens = numberOfTokens(vertices,	true);
	//Max
	auto maxNumMills = numberOfMills(vertices, false);
	auto maxNumTokens = numberOfTokens(vertices, false);
	return score-miniNumMills-miniNumTokens+maxNumMills+maxNumTokens;
}

std::tuple<int, int> ComputerPlayer::removeToken(std::array<int, 24> vertices)
{
	int bestScore = -INFINITY;
	auto best = std::make_tuple(bestScore, -1);
	for (int token = 0; token < 24; token++) {
		int score = -5000;
		for (int i = 0; i < 16; i++) {
			auto mill = board->possibleMillPositions[i];
			if (tokenInMill(mill, token)) {
				if (mill[0] == token && vertices[mill[1]] == 1 && vertices[mill[2]] == 0)
					score -= 20;
				if (mill[0] == 1 && vertices[mill[1]] == token && vertices[mill[2]] == 0)
					score -= 20;
				if (mill[0] == 1 && vertices[mill[1]] == 0 && vertices[mill[2]] == token)
					score -= 20;
				if (mill[0] == token && vertices[mill[1]] == 0 && vertices[mill[2]] == 1)
					score -= 20;
			}
			if (isBlockingMill(vertices, mill, token))
				score -= 1000;
		}
		if (score < bestScore) {//max score
			bestScore = score;
			best = std::make_tuple(bestScore, token);
		}
	}
	return best;
}

std::tuple<int, int> ComputerPlayer::capture(std::array<int, 24> vertices)
{
	int bestScore = -INFINTY;
	auto best = std::make_tuple(bestScore, -1);
	//we're going through all capturable cells and comput its scores
	for (int token = 0; token < 24; token++) {
		if (vertices[token] == 0 || vertices[token] == 2) continue;//free or computer token
		int score = 10;
		//calclating score
		for (int i = 0; i < 16; i++) {
			auto mill = board->possibleMillPositions[i];
			//case 1:the token needs one more ally on his side to form a mill
			if (tokenInMill(mill, token)) {
				if (mill[0] == token && vertices[mill[1]] == 1 && vertices[mill[2]] == 0)
					score += 20;
				if (mill[0] == 1 && vertices[mill[1]] == token && vertices[mill[2]] == 0)
					score += 20;
				if (mill[0] == 1 && vertices[mill[1]] == 0 && vertices[mill[2]] == token)
					score += 20;
				if (mill[0] == token && vertices[mill[1]] == 0 && vertices[mill[2]] == 1)
					score += 20;
			}
			//case 2:the token is blocking an ally mill
			if (isBlockingMill(vertices, mill, token))
				score += 50;
		}
		if (bestScore < score) {//max score
			bestScore = score;
			best = std::make_tuple(bestScore, token);
		}
	}
	return best;
}


//Evaluation functions

//Case 1: check if the chosen token is going to close a mill
int ComputerPlayer::morrisClosed(std::array<int, 24> vertices, int token)
{
	int score = 0;
	int side = vertices[token];
	for (int i = 0; i < 16; i++) {
		//checking for mills completed mills that contains the current token
		if ((board->possibleMillPositions[i][0] == token && vertices[board->possibleMillPositions[i][1]] == side && vertices[board->possibleMillPositions[i][2]] == side) || (board->possibleMillPositions[i][1] == token && vertices[board->possibleMillPositions[i][0]] == side && vertices[board->possibleMillPositions[i][2]] == side) || (board->possibleMillPositions[i][2] == token && vertices[board->possibleMillPositions[i][1]] == side && vertices[board->possibleMillPositions[i][0]] == side)) {
			score += 10;
		}
	}
	return score;
}

//Case 2: we're checking how much mills are on the current configuration
int ComputerPlayer::numberOfMills(std::array<int, 24> vertices, bool computer)
{
	const int playerSide = computer ? 2 : 1;
	int score = 0;
	for (int i = 0; i < 16; i++) {
		//checking for mills
		if (vertices[board->possibleMillPositions[i][0]] == playerSide && vertices[board->possibleMillPositions[i][1]] == playerSide && board->vertices[board->possibleMillPositions[i][2]] == playerSide) {
			score += 1;
		}
	}
	return score;
}

//Case 3: checking if the current token is going to block any enemy token (check if they're adjacent)
int ComputerPlayer::numberOfBlockedEnnemyTokens(std::array<int, 24> vertices, int token)
{
	int score = 0;
	int enemy = vertices[token] == 1 ? 2 : 1;
	for (int i = 0; i < 16; i++) {
		//checking if the possible mill contains the token
		if (board->possibleMillPositions[i][0] == token) {
			//if his adjacents are ennemies
			if (vertices[board->possibleMillPositions[i][1]] == enemy)
				score = score + 20;
		}
		else if (board->possibleMillPositions[i][1] == token) {
			if (vertices[board->possibleMillPositions[i][0]] == enemy)
				score = score + 20;
			if (vertices[board->possibleMillPositions[i][2]] == enemy)
				score = score + 20;
		}
		else if (board->possibleMillPositions[i][2] == token) {
			if (vertices[board->possibleMillPositions[i][1]] == enemy)
				score = score + 20;
		}
	}
	return score;
}

//case 4: counting the tokens
int ComputerPlayer::numberOfTokens(std::array<int, 24> vertices, bool computer)
{
	auto score = 0;
	int side = computer ? 2 : 1;
	for (int i = 0; i < 24; i++) {
		if (vertices[i] == side)
			score++;
	}
	return score;
}

int ComputerPlayer::numberOfMillsBlocked(std::array<int, 24> vertices, int token)
{
	int score = 0;
	for (int i = 0; i < 16; i++) {
		//checking for mills
		if (hasBlockedMill(vertices,board->possibleMillPositions[i], token))
			score+=100;
	}
	return score;
}

//Utility
bool ComputerPlayer::placementPhaseOver(std::array<int, 24>)
{
	return false;
}

bool ComputerPlayer::hasBlockedMill(std::array<int,24> vertices,std::array<int, 3> mill, int token)
{
	int enemySide = vertices[token] == 1?2:1;
	int side = vertices[token];
	if (!tokenInMill(mill, token))
		return false;
	if (millIsFull(vertices, mill)) {
		if (vertices[mill[0]] == enemySide && vertices[mill[1]] == enemySide && vertices[mill[2]] == side)
			return true;
		if (vertices[mill[0]] == side && vertices[mill[1]] == enemySide && vertices[mill[2]] == enemySide)
			return true;
		if (vertices[mill[0]] == enemySide && vertices[mill[1]] == side && vertices[mill[2]] == enemySide)
			return true;
	}
	return false;
}

bool ComputerPlayer::millIsFormed(std::array<int, 24> vertices, std::array<int, 3> mill, int side)
{
	return vertices[mill[0]] == side && vertices[mill[1]] == side && vertices[mill[2]] == side;
}

bool ComputerPlayer::millIsFull(std::array<int, 24> vertices, std::array<int, 3> mill)
{
	return vertices[mill[0]] != 0 && vertices[mill[1]] != 0 && vertices[mill[2]] != 0;
}

bool ComputerPlayer::tokenInMill(std::array<int, 3> mill, int token)
{
	return mill[0] == token || mill[1] == token || mill[2] == token;
}

bool ComputerPlayer::isBlockingMill(std::array<int, 24> vertices, std::array<int, 3> mill, int token)
{
	int side = vertices[token];
	int enemySide = side == 1 ? 2 : 1;
	if (mill[0] == enemySide && mill[1] == enemySide && mill[2] == side) return true;
	if (mill[0] == enemySide && mill[1] == side && mill[2] == enemySide) return true;
	if (mill[0] == side && mill[1] == enemySide && mill[2] == enemySide) return true;
	return false;
}







