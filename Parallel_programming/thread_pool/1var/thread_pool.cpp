#include <iostream>
#include <tgmath.h>
#include <queue>
#include <atomic>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>

#define TEST_NUMBERS_CNT 10

using namespace std;

template <typename R>
class thread_pool  {
    /*
     *При запуске thread_pool создаётся некоторое количество
     *потоков с executant_thread в качестве исполняемого функтора. Оператор () этого класса в бесконечном цикле
     *берёт задачи, возвращающие тип R, из потокобезопасной очереди, которая принадлежит классу thread_pool. Каждый
     *executant_thread знает ссылку на свой thread_pool и умеет отслеживать его состояние: FINAL означает, что в
     *очереди больше нет задач и пользователь не собирается их добавлять - условие выхода из цикла и завершения потока.
     */
    class executant_thread {
        thread_pool& pool_ptr;
        int id;
    public:
        executant_thread(thread_pool& _pool_ptr, int _id): pool_ptr(_pool_ptr), id(_id) {}
        ~executant_thread() {}
        void operator()() {
            while(true) {
                if (pool_ptr.state == FINAL) {
                    break;
                }
                pair<function <R()>, bool> func = pool_ptr.tasks.pop();
                if (func.second) {
                    func.first();
                }
            }
        }
    };

    class thread_safe_queue {
        queue<function<R()> > queue_for_tasks;
        mutex queue_mtx;
        condition_variable queue_not_empty_cond;
        thread_pool& pool_ptr;

    public:
        thread_safe_queue(thread_pool& _pool_ptr):pool_ptr(_pool_ptr) {}
        ~thread_safe_queue() {}
        pair<function <R()>, bool> pop() {
            function <R()> answer;
            bool flag = false;
            while (true) {
                unique_lock<mutex> lock(queue_mtx);
                queue_not_empty_cond.wait(lock);
                if (!queue_for_tasks.empty()) {
                    answer = queue_for_tasks.front();
                    queue_for_tasks.pop();
                    flag = true;
                    break;
                } else {
                    if (pool_ptr.state == FINAL) {
                        flag = false;
                        break;
                    }
                }
            }
            return pair<function <R()>, bool>(answer, flag);
        }
        void push(function<R()> func_with_args) {
            lock_guard<mutex> lock(queue_mtx);
            queue_for_tasks.push(func_with_args);
            queue_not_empty_cond.notify_one();
        }
        void before_closing() {
            while (!queue_for_tasks.empty()) {
                queue_not_empty_cond.notify_one();
            }
            pool_ptr.state = FINAL;
            queue_not_empty_cond.notify_all();
        }
    };

    thread_safe_queue tasks;
    vector<thread> executant_threads;

    enum state_t {NON_STARTED = -1, STARTED, FINAL, CLOSED} state;

public :
    thread_pool(int size = 4) : tasks((*this)), state(NON_STARTED), executant_threads(size) {}
    ~thread_pool() {
        if (state != CLOSED) {
            this->close();
        }
    }

    void start() {
        if (state == NON_STARTED) {
            state = STARTED;
            for (int i = 0; i < executant_threads.size(); ++i) {
               executant_threads[i] = thread {executant_thread(*this, i)};
            }
        } else {
            throw "Wrong state: NON_STARTED is expected";
        }
    }
    void execute(function<R()> func_with_args) {
        if (state == NON_STARTED) {
            this->start();
        }
        if (state != STARTED) {
            throw "Wrong state: STARTED is expected";
        }
        tasks.push(func_with_args);
    }
    void close() {
        tasks.before_closing();
        for (int i = 0; i < executant_threads.size(); ++i) {
            executant_threads[i].join();
        }
        state = CLOSED;
    }
};

void check_on_prime(int x) {
    bool is_prime = true;
    int i = 2;
    while(i <= sqrt(x)) {
        if (x % i == 0) {
            is_prime = false;
            break;
        }
        i += 1;
    }
    if (is_prime) {
        cout << x << " is prime" << endl;
    } else {
        cout << x << " is not prime" << endl;
    }
}

int pow2(int x) {
    return x*x;
}

int main() {
    thread_pool<int> first_pool;

    try {
        for (int i = 0; i < TEST_NUMBERS_CNT; i++) {
            first_pool.execute(bind(pow2, i));
            /*
             *Сделаю небольшой тест на этом месте
             */
        }
    } catch (string exc) {
        cerr << exc << endl;
    }

    first_pool.close();

    thread_pool<void> second_pool;

    try {
        for (int i = 0; i < TEST_NUMBERS_CNT; i++) {
            /*
             *Изменю генерацию певдослучайных чисел
             */
            second_pool.execute(bind(check_on_prime, rand() % 1000));
        }
    } catch (string exc) {
        cerr << exc << endl;
    }

    second_pool.close();

    return 0;
}