#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

/*  Declarations
/*   *   *   *   *   *   *   *   *   *   */

typedef struct task {
    int r, p, q, x;
} task_t;

void readTill( string fileHeader, ifstream &data, vector<task_t> &tasks );

int scheduler( vector<task_t> &tasks );


/*  Executor
/*   *   *   *   *   *   *   *   *   *   */

int main( int argc, char **argv ) {

    //  initial dataset
    vector<task_t> tasks;
    ifstream data( "rpq.data.txt" );
    int totalTime = 0;

    //  process for all 4 datasets
    for ( int i = 0; i < 4; i++ ) {

        //  reset tasks
        tasks.clear();

        //  create new file header
        string fileHeader = "data." + to_string( i + 1 );

        //  read data
        readTill( fileHeader, data, tasks );

        //  get partial result
        int result = scheduler( tasks );

        //  print result
        cout << endl;
        cout << "---+---+---+---+---+---+---+---+---+---" << endl;

        cout << "Current run: " << i + 1 << endl;
        cout << "Calculated time: " << result << endl;
        cout << "Order:" << endl;
        for( int i = 0; i < tasks.size(); i++ ) {
            cout << i + 1 << ". [ r: " << tasks[i].r << " | p: " << tasks[i].p << " | q: " << tasks[i].q << " | x: " << tasks[i].x << "]" << endl;
        }

        cout << "---+---+---+---+---+---+---+---+---+---" << endl;
        cout << endl;

        //  update total result
        totalTime += result;
    }

    //  get partial result
    int result = scheduler( tasks );

    //  print result
    cout << endl;
    cout << "---+---+---+---+---+---+---+---+---+---" << endl;

    cout << "<*> Final result <*>" << endl;
    cout << "Calculated total time: " << totalTime << endl;

    cout << "---+---+---+---+---+---+---+---+---+---" << endl;
    cout << endl;

    //  close data stream
    data.close();

    //  final statement
    return 0;
}


/*  Solutions
/*   *   *   *   *   *   *   *   *   *   */

void readTill( string fileHeader, ifstream &data, vector<task_t> &tasks ) {

    //  initial dataset
    string buffer;
    int size;

    //  read till heder found
    do {
        data >> buffer;
    } while( buffer != fileHeader );

    //  read size
    data >> size;

    //  read data
    for( int  i = 0; i < size; i++ ) {

        //  task buffer
        task_t tmp;

        //  read data into task
        data >> tmp.r >> tmp.p >> tmp.q;

        //  set task id
        tmp.x = i;

        //  add task to list
        tasks.push_back( tmp );
    }
}

int scheduler( vector<task_t> &tasks ) {

    //  initial dataset
    int m = 0;
    int c = 0;


    /*  scheduling algorithm
    /*   *   *   *   *   *   *   *   */

    //  initial values
    int t = 0;
    vector<task_t> N = tasks;
    vector<task_t> G;
    vector<task_t> K;

    //  initial N set sort
    sort( N.begin(), N.end(), []( task_t const &p, task_t const &q ) -> bool {
        return p.r < q.r;
    });

    //  starts procedure
    while( G.size() || N.size() ) {
        
        //  first step
        while( N.size() && N.front().r <= t ) {

            //  get task
            task_t tmp = N.front();

            //  erase task from N set
            N.erase( N.begin() );

            //  add task to G set
            G.push_back( tmp );
        }

        //  second step
        if( !G.size() ) {

            //  update t value
            t = N.front().r;

        } else {

            //  resort G set
            sort( G.begin(), G.end(), []( task_t const &p, task_t const &q ) -> bool {
                return p.q > q.q;
            });

            //  get task
            task_t tmp = G.front();

            //  erase task from G set
            G.erase( G.begin() );

            //  add task to K set
            K.push_back( tmp );

            //  update t value
            t += tmp.p;
        }
    }

    //  update taksk ref
    tasks = K;

    /*   *   *   *   *   *   *   *   */


    //  recalc values
    for( int i = 0; i < tasks.size(); i++ ) {
        m = max( m, tasks[i].r ) + tasks[i].p;
        c = max( c, m + tasks[i].q );
    }

    //  return result
    return c;
}