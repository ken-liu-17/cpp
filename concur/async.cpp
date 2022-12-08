#include <numeric>
#include <iostream>
#include <vector>
#include <thread>
#include <future>

int accumulate_block_worker_ret(int* data, size_t count) {
  std::this_thread::sleep_for(std::chrono::seconds(3));
  return std::accumulate(data, data + count, 0);
}

int main(int argc, const char** argv) {
  std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};
  std::future<int> fut = std::async(
      std::launch::async, accumulate_block_worker_ret, v.data(), v.size());
  while (fut.wait_for(std::chrono::seconds(1)) != std::future_status::ready) {
    std::cout << "... still not ready\n";
  }
  std::cout << "use_worker_in_std_async computed " << fut.get() << "\n";

  return 0;
}
