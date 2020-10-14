#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <bits/stdc++.h>
#include <thread>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

using namespace std;

volatile sig_atomic_t stop;

void stopHandle(int signum) {
    stop = 1;
}

void sys_err(string s) {
    perror(s.c_str());
    exit(1);
}

static char letters[] = {'a', 'b', 'c'};
static constexpr int MAX_SIZE = 100*1024*1024;

void generate() {
	// ftok to generate unique key 
    key_t key = ftok("shmfile",67); 

    // shmget returns an identifier in shmid
    int shmid = shmget(key,MAX_SIZE,0666|IPC_CREAT);
    if (shmid<0) {
        sys_err("Cannot shmget");
    }

    // shmat to attach to shared memory
    char *str = (char*) shmat(shmid,(void*)0,0);

    while(!stop) {
	    for (int i = 0 ; i < MAX_SIZE; i++) {
    	    str[i] = letters[random() % 3];
    	}
	    str[MAX_SIZE] = '\0';
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    //detach from shared memory
    shmdt(str);

    // destroy the shared memory
    shmctl(shmid,IPC_RMID,NULL);
}

void process() {
    // ftok to generate unique key
    key_t key = ftok("shmfile",65);

    // shmget returns an identifier in shmid
    int shmid = shmget(key,MAX_SIZE,0666|IPC_CREAT);

    // shmat to attach to shared memory
    char *str = (char*) shmat(shmid,(void*)0,0);

    while (!stop) {
        std::map<char, int> found;
	    for (int i = 0; i < INT_MAX; i++) {
		    if (str[i] == '\0') {
			    break;
		    }
            found[str[i]]++;
	    }
        cout << "Found: ";
        for (const auto & pair: found) {
            cout << pair.first << ": " << pair.second << ' ';
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        cout << endl;
    }
 
    //detach from shared memory
    shmdt(str);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, stopHandle);
    signal(SIGTERM, stopHandle);

    cout << "Starting\n";

    if (strcmp(argv[1], "generate") == 0) {
        generate();
    } else if(strcmp(argv[1], "process") == 0) {
        process();
    } else {
        cout << "Unknown args: " << argc << ";" << argv[1] << ";" << endl;
    }

    cout << "exiting safely\n";

    return 0;
} 
