#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <numeric>
#include <vector>

using namespace std;

//PARAMS
const int NUM = 5; //Number of cooks
const int MAX_TABLE_PORTIONS = 4;
const int MAX_TABLE_WEIGHT = 15;
const int MAX_FOOD_WEIGHT = 8;

mutex forks[NUM];


//----------------------MESSAGE QUEUING----------------------
class Table{
    vector<int> tableVector; // Contains table content
    mutex tableMutex;
    condition_variable table_full, table_empty;
public:
    void prepare_dish(int id){
        //Getting exclusive access to the table's resources
        unique_lock<mutex> lock(tableMutex);
        printf("Cook%d is cooking something...\n", id);

        //Counting table load
        auto current_weight = reduce(begin(tableVector), end(tableVector));
        int current_portions = tableVector.size();

        //Preparing food... mmmmmmmmmmmm...
        int dish_weight = rand() % MAX_FOOD_WEIGHT + 1;
        this_thread::sleep_for(chrono::milliseconds(dish_weight*300));

        //Waiting until there is space on the table
        while(current_portions >= MAX_TABLE_PORTIONS
        || (current_weight + dish_weight) > MAX_TABLE_WEIGHT){
            table_full.wait(lock);
            current_weight = reduce(begin(tableVector), end(tableVector));
        }

        //PLACE THE DISH!!!!
        tableVector.push_back(dish_weight);
        printf("Cook%d just delivered a dish! It weights %d units!\nCurrent load: W=%d P=%d\n",
        id, dish_weight, current_weight+dish_weight, current_portions+1);

        //The table is not empty notification
        table_empty.notify_all();
    }

    void consume_dish(int id){
        unique_lock<mutex> lock(tableMutex);

        printf("Cook%d wants to eat!\n", id);
        //wanted to try this in the upper method,
        //but it doesnt seem to work for more than one condition
        table_empty.wait(lock, [&]() { return !tableVector.empty(); });

        int dish_weight = tableVector.front();
        tableVector.erase(begin(tableVector));

        printf("Cook%d is eating a %d units big portion...\n", id, dish_weight);
        this_thread::sleep_for(chrono::milliseconds(dish_weight*300));
        printf("Cook%d finished eating his portion.\n", id);

        //The table is not full notification
        table_full.notify_all();
    }
};


//--------------------------THREADS--------------------------
void cook_thread(int id){
    static Table table;

    int left_fork = id;
    int right_fork = (id + 1) % NUM;

    //preventing deadlock
    //cooks with even indexes ask for forks in reverse order
    if (id % 2 == 0) {
        swap(left_fork, right_fork);
    }

    while(true){
        //IDLE
        printf("Cook%d is thinking.\n", id);

        // Picking forks up
        forks[left_fork].lock();
        forks[right_fork].lock();

        //simulating dish preparation
        table.prepare_dish(id);

        // Putting forks down
        forks[left_fork].unlock();
        forks[right_fork].unlock();

        std::this_thread::yield();

        forks[left_fork].lock();
        forks[right_fork].lock();

        //simulating eating a portion
        table.consume_dish(id);

        forks[left_fork].unlock();
        forks[right_fork].unlock();

        this_thread::yield();
    }
}

int main(){
    thread cooks[NUM];

    for(int i = 0; i<NUM; i++){
        cooks[i] = thread(cook_thread, i);
    }

    for(int i = 0; i<NUM; i++){
        cooks[i].join();
    }

    return 0;
}
