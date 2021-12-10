#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstring>
using namespace std;

class Player
{
public:
	Player(string);
	static const int MAX_ROLLS_ALLOWED = 23;
	const string& getName() const;
	int getScore() const;
	void roll(int);
	bool isCanPlay() const;

private:
	string name;
	int score;
	int rolls[MAX_ROLLS_ALLOWED];
	bool isStrike();
	bool isSpare();
	bool firstRoll;
	int frameIndex;
	bool canPlay;
	int currentRoll;
	void updateScore();
};

class GameSession
{
public:
	void setAlley(int alley);
	int getId() const;
	void setPlayers(const vector<Player> &players);
	bool declareWinner();
	GameSession();
	void makeRoll(Player&, int);

private:
	int alley;
	int id;
	int getUniqueId();
	vector<Player> players;
};

class Game
{
private:
	unordered_map<int, GameSession> gameSessions;
public:
	static vector<int> alleys;
	static void makeActive(int alley);
	int createSession(vector<Player>& players);
	void roll(int, Player, int);
	void declareWinner(int);
};

vector<int> Game::alleys = { 1,2,3,4 };

int Game::createSession(vector<Player>& players)
{
	if(Game::alleys.size() == 0)
	{
		cout << "All alleys are currently occupied!\n";
		return 0;
	}
	GameSession gameSession;
	gameSession.setPlayers(players);
	gameSession.setAlley(Game::alleys.back());
	Game::alleys.pop_back();
	gameSessions[gameSession.getId()] = gameSession;
	return gameSession.getId();
}

void Game::makeActive(int alley)
{
	Game::alleys.push_back(alley);
}

void Game::declareWinner(int index)
{
	bool winnerFlag = gameSessions[index].declareWinner();

	if(winnerFlag == false)
	{
		cout << "No winners for this game yet" << endl;
	}
}

void Game::roll(int index, Player player, int pins)
{
	GameSession *gameSession = &gameSessions[index];
	gameSession->makeRoll(player, pins);
}

void GameSession::makeRoll(Player& player, int pins)
{
	for(Player &p: players)
	{
		if(p.getName() == player.getName())
			p.roll(pins);
	}
}

GameSession::GameSession()
{
	this->alley = -1;
	this->id = getUniqueId();
	this->players.clear();
}

int GameSession::getUniqueId()
{
	static int gameSessionId = 1;
	return gameSessionId++;
}

void GameSession::setAlley(int alley) {
	this->alley = alley;
}

int GameSession::getId() const {
	return id;
}

void GameSession::setPlayers(const vector<Player> &players) {
	this->players = players;
}

bool GameSession::declareWinner()
{
	int maxScore = 0;
	Player *winner = nullptr;
	for(Player &p: players)
	{
		if(p.isCanPlay() == true)
		{
            cout << "Player " << p.getName().c_str() << " hasn't completed yet. The current score: " << p.getScore() << endl;
            cout << "Match is in progress\n";
            return false;
		}
		if(p.getScore() > maxScore)
		{
			maxScore = p.getScore();
			winner = &p;
		}
	}
	if(winner != nullptr)
	{
        cout << "The winner is: " << winner->getName().c_str() << " with a score of " << winner->getScore() << "\n";
	}

	Game::makeActive(this->alley);
	return true;
}

Player::Player(string name)
{
	memset(rolls, 0, sizeof rolls);
	this->name = name;
	this->score = 0;
	this->firstRoll = true;
	this->frameIndex = 0;
	this->canPlay = true;
	this->currentRoll = 0;
}

bool Player::isStrike()
{
	return this->firstRoll == true && rolls[frameIndex] == 10;
}

bool Player::isCanPlay() const {
	return canPlay;
}

bool Player::isSpare()
{
	return rolls[frameIndex] + rolls[frameIndex + 1] == 10;
}

void Player::roll(int pins)
{
	if(this->canPlay == false)
		return;
	rolls[currentRoll++] = pins;
	updateScore();
}

void Player::updateScore()
{
	if(isStrike())
	{
		score += 20;
		rolls[currentRoll++] = 0;
		frameIndex += 2;
		if(frameIndex >= MAX_ROLLS_ALLOWED)
			this->canPlay = false;
	}
	else
	{
		if(frameIndex >= MAX_ROLLS_ALLOWED - 1)
		{
			score += rolls[frameIndex];
			this->canPlay = false;
		}
		else if(firstRoll == true)
		{
			firstRoll = false;
		}
		else
		{
			if(isSpare())
				score += 5;

			score += (rolls[frameIndex] + rolls[frameIndex + 1]);
			frameIndex += 2;
			firstRoll = true;
			if(frameIndex >= MAX_ROLLS_ALLOWED - 3)
				this->canPlay = false;
		}
	}
}

const string& Player::getName() const {
	return name;
}

int Player::getScore() const {
	return score;
}

int main() {
    Player p1("Thor");
    Player p2("Loki");
    Player p3("Hela");
    Player p4("Odin");

    vector<Player> vec;
    vec.push_back(p1);
    vec.push_back(p2);
    vec.push_back(p3);
    vec.push_back(p4);

    Game g;
    int index = g.createSession(vec);

    vector<int> s1;
    vector<int> s2;
    vector<int> s3;
    vector<int> s4;
    int score;
    for(int i=0; i<20; ++i)
    {
    	score = abs(rand()%10);
    	s1.push_back(score);
        g.roll(index, p1, score);
    	score = abs(rand()%10);
    	s2.push_back(score);
        g.roll(index, p2, score);
    	score = abs(rand()%10);
    	s3.push_back(score);
        g.roll(index, p3, score);
    	score = abs(rand()%10);
    	s4.push_back(score);
        g.roll(index, p4, score);
    }
    cout << "Player 1: ";
    for(int i: s1)
    	cout << i << " ";
    cout << "\n";

    cout << "Player 2: ";
    for(int i: s2)
    	cout << i << " ";
    cout << "\n";

    cout << "Player 3: ";
    for(int i: s3)
    	cout << i << " ";
    cout << "\n";

    cout << "Player 4: ";
    for(int i: s4)
    	cout << i << " ";
    cout << "\n";


    g.createSession(vec);
    g.createSession(vec);
    g.createSession(vec);
    g.declareWinner(index);
    g.createSession(vec);
	return 0;
}
