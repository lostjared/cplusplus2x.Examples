/* use GCC 13.2 or greater */
#include<iostream>
#include<stop_token>
#include<thread>
#include<future>

void thread1(std::stop_token token) {
    auto id = std::this_thread::get_id();
    std::stop_callback callb{token, [id]() {
        std::cout << "callback called for: " << id << "\n";
        std::cout << "cleanup thread....\n";
    }};
    std::this_thread::sleep_for(std::chrono::seconds(8));
    std::cout << "finished here..\n";
}


int main() {
    std::jthread t(thread1);
    std::cout << "sleep 3 seconds request stop\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    t.request_stop();
    return 0;
}