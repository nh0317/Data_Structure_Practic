#include <algorithm>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <list>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
using namespace std;

#define SWAP(x, y, t) ((t) = (x), (x) = (y), (y) = (t))

#define MAX_NAME_LEN 64

class player {
  private:
    string name;
    double score;

  public:
    string getName() { return name; }
    double getScore() { return score; }
    void setName(string name) { this->name = name; }
    void setScore(double score) { this->score = score; }
};

class Info {
  private:
    char name[MAX_NAME_LEN + 1];
    double score;

  public:
    Info();
    Info(string name, double score);

    void setName(string name);
    void setScore(double score);

    string getName(void);
    double getScore(void);
};

Info::Info() {
    this->score = -1.0;
    memset(this->name, 0x00, MAX_NAME_LEN + 1);
}

Info::Info(string name, double score) {
    memcpy(this->name, name.c_str(), MAX_NAME_LEN);
    this->score = score;
}

void Info::setName(string name) {
    memcpy(this->name, name.c_str(), MAX_NAME_LEN);
}

void Info::setScore(double score) { this->score = score; }

string Info::getName(void) { return string(this->name); }

double Info::getScore(void) { return this->score; }

void fileWrite(double score) {
    list<Info> InfoList;
    char *tmp;

    string name = "";
    cout << "Enter the NAME : ";
    cin >> name;

    cout << "<< Score: " << score << endl;

    Info info(name, score);
    InfoList.push_back(info);
    int num = InfoList.size();

    cout << "플레이어 정보가 성공적으로 저장되었습니다!" << endl;

    string filepath = "./PlayerInfo.txt";
    int fd = open(filepath.c_str(), O_CREAT | O_APPEND | O_WRONLY, 0644);
    if (fd == -1) {
        perror("open() error!!");
        exit(1);
    }

    list<Info>::iterator iter;
    for (iter = InfoList.begin(); iter != InfoList.end(); ++iter) {
        if (write(fd, &(*iter), sizeof(Info)) == -1) {
            perror("write() error!");
            return;
        }
        /*else
        {
            for (int i = 1; i < InfoList.size(); i++) {
                char cursor = InfoList[i];
                int j = i - 1;
                while ((j >= 0) &&
                       (tmp[j] > cursor)) { // while A[j] is out of order
                    tmp[j + 1] = tmp[j];    // move A[j] right
                    j--;                    // decrement j
                }
                tmp[j + 1] = cursor; // this is the proper place for cur
            }
        } */
        close(fd);
    }
}
void fileRead() {
    ssize_t rsize = 0;
    int file = 0;
    int k = 0;
    player player[1024], tmp;
    Info info[1024];

    // Info *info = new Info();

    string filepath = "./PlayerInfo.txt";
    file = open(filepath.c_str(), O_RDONLY, 0644);

    /*if (in.is_open()) {
        in >> s;
        cout << "입력 받은 정보 : " << s << endl;
    } else {
        cout << "파일을 찾을 수 없습니다!" << endl;
    }*/

    do {
        rsize = read(file, (Info *)info, sizeof(Info));

        if (rsize == sizeof(Info)) {
            player[k].setName(info->getName());
            player[k++].setScore(info->getScore());
        } else if (rsize == -1) {
            perror("read() error!");
            exit(-1);
        }
    } while (rsize > 0);
    close(file);

    for (int i = k - 1; i > 0; i--) {
        for (int j = 0; j < i; j++) {
            if (player[j].getScore() < player[j + 1].getScore()) {
                SWAP(player[j], player[j + 1], tmp);
            }
        }
    }
    for (int i = 0; i < k; i++) {
        cout << i + 1 << "등 "
             << "NAME : " << player[i].getName()
             << " SCORE : " << player[i].getScore() << endl;
    }
}

// test
int main() { // main에서 score 변수값 넣기 score값 임시로 지정
    double score = 100;
    fileWrite(score);
    fileRead();

    double score2 = 200;
    fileWrite(score2);
    fileRead();

    return 0;
}