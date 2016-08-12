#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <ctime>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <pthread.h>
#include <string>
#include <map>

using namespace std;

pthread_t *thread_array = NULL;
pthread_mutex_t mtx;
map<long long int, long long int> buff;
int thread_num;
int data[1000][7];
int maxNum[7];
int cycle;
//long long int N = 1024;
long long int N = 678223072849;

int to_arr(long long int id, int *arr);
long long int to_id(int *arr);
int verify(long long int id);
void* analyzer(void* arg);

int main(int argc, char *argv[]) {
  #ifdef debug
  long long int id = 1090809832;
  id = 678223072848;
  int arr[7] = {0, 2, 4,5,6,7,8};
  cout << "id: " << to_id(arr) << endl;
  to_arr(id, arr);
  cout << "id: " << id << endl;
  for(int i = 0 ; i < 7 ; ++i ) {
    cout << arr[i] << " ";
  }
  cout << endl;
  #endif

  // config
  thread_num = 8;
  for(int i = 0; i < 7; ++i) {
    maxNum[i] = 100000;
  }
  cycle      = (argc >= 2) ? atoi(argv[1]) : 200;
  maxNum[2]  = (argc >= 3) ? atoi(argv[2]) : 18;   // 3: 18
  maxNum[3]  = (argc >= 4) ? atoi(argv[3]) : 6;   // 4: 6
  thread_num = (argc >= 5) ? atoi(argv[4]) : 8;


  // open input file
	cout << "open file" << endl;
	ifstream ifs("data.txt");
	if(!ifs) {
		cout << "open data.txt failed." << endl;
		return 1;
	}

  // input file
	cout << "input file" << endl;
	for(int i = 0; i < cycle; ++i) {
		for(int j=0;j<7;++j){
			 ifs >> data[i][j];
		}
	}

  // mutex init
  pthread_mutex_init(&mtx, NULL);

  // create threads
	cout << "pthread create" << endl;
  cout << "| 0%" << setw(76) << "100% |" << endl;
	thread_array = (pthread_t*) malloc(sizeof(pthread_t) * thread_num);
	for(int i = 0; i < thread_num; ++i) {
		int rc = pthread_create(thread_array+i, NULL, analyzer, (void *) i);
		assert(rc == 0);
	}

  // join
	for(int i = 0; i < thread_num; ++i) {
		pthread_join(thread_array[i], NULL);
	}

  // mutex destroy
  pthread_mutex_destroy(&mtx);

  // open output file
	cout << "\noutput file" << endl;
	ofstream ofs("result.txt");

  // print data to file
  ofs << "n1 n2 n3 n4 n5 n6 n7 sum3 sum4" << endl;
  map<long long int, long long int>::iterator iter;
  for(iter = buff.begin(); iter != buff.end(); iter++) {
    int arr[7] = {0};
    int sum3 = iter->second / 1000;
    int sum4 = iter->second % 1000;
    to_arr(iter->first, arr);

    for(int i = 0; i < 7; ++i) {
      ofs << setw(2) << arr[i] << " ";
    }
    ofs << setw(4) << sum3 << " ";
    ofs << setw(4) << sum4 << endl;
  }

  // free
  free(thread_array);

	return 0;
}

int to_arr(long long int id, int *arr) {
  int i;
  for(i = 0; i < 7; ++i) {
    arr[i] = id % 49;
    id /= 49;
  }
  return 0;
}

long long int to_id(int *arr) {
  int i;
  long long int n, m;
  n = 0;
  m = 1;
  for(i = 0; i < 7; ++i) {
    n += arr[i] * m;
    m *= 49;
  }
  return n;
}

int verify(long long int id) {
  int arr[7] = {0};
  to_arr(id, arr);
  // same number
  int i, j;
  for(i = 0; i < 7; ++i) {
    for(j = 0; j < i; ++j) {
      if(arr[i] == arr[j]) {
        return 1;
      }
    }
  }
  // greater than
  for(i = 0; i < 6; ++i) {
    if(arr[i+1] <= arr[i]) {
      return 2;
    }
  }
  return 0;
}

void* analyzer(void* arg) {
	int thread_id = *((int*)(&arg));
  long long int start, end, id;
	start = (thread_id    ) * N / thread_num;
	end   = (thread_id + 1) * N / thread_num;
  #ifdef debug
  printf("id: %d, start: %lld, end: %lld\n", thread_id, start, end);
  #endif

  for(id = start; id < end; ++id) {
    #ifdef debug
    printf("id: %lld f\n",id);
    #endif
    if( !((start-id) % N/10) ) { cout << "#"; }
    if(verify(id)) {
      continue;
    }
    // convert id to arr
    int arr[7] = {0};
    to_arr(id, arr);

    // analyis
    int sum[7] = {0};
		for(int k = 0; k < cycle; ++k) {
			int cmp = 0;
			for(int i = 0; i < 6; ++i) {
				for(int j = 0; j < 7; ++j) {
					if(data[k][i] == arr[j]) {
						cmp++;
					}
				}
			}

      for(int i = cmp-1; i >= 0; --i){
        sum[i]++;
      }
		}

    // keep needed data
    for(int i = 0; i < 7; ++i) {
      #ifdef debug
      printf("i: %d, sum: %d, max: %d\n", i, sum[i], maxNum[i]);
      #endif
      if(sum[i] >= maxNum[i]) {
        long long int key = id;
        long long int value = 1000 * sum[2] + sum[3];
        pthread_mutex_lock(&mtx);
        buff.insert(pair<long long int, long long int>(key, value));
        pthread_mutex_unlock(&mtx);
        break;
      }
    }
	}
}
