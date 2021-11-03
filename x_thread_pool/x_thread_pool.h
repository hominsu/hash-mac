//
// Created by Homin Su on 2021/11/2.
//

/**
 * @file
 */

#ifndef DES_X_THREAD_POOL_X_THREAD_POOL_H_
#define DES_X_THREAD_POOL_X_THREAD_POOL_H_

#include <memory>
#include <thread>
#include <atomic>
#include <shared_mutex>
#include <condition_variable>
#include <list>
#include <vector>

class XTaskBase;

/**
 * @brief 线程池
 * @example
 * @verbatim
 *  // 初始化线程池
 *  auto thread_num = XThreadPool::Get()->Init(8);
 *
 *  // 准备任务
 *  auto task1 = std::make_shared<TestTask>("task1");
 *
 *  // 任务加入线程池
 *  XThreadPool::Get()->AddTask(task1);
 *
 *  // 阻塞等待任务执行结果
 *  auto ret = task1->get_return();
 * @endverbatim
 * @see XTask
 */
class XThreadPool {
 private:
  size_t thread_nums_ = 0; ///< 线程数量
  std::vector<std::unique_ptr<std::thread>> threads_;  ///< 线程池线程
  std::list<std::shared_ptr<XTaskBase>> x_tasks_;   ///< 任务队列
  std::atomic<bool> is_running_ = false;      ///< 线程池运行状态
  std::atomic<int> task_run_count_ = 0; ///< 正在运行的任务数量，原子变量，线程安全

  mutable std::shared_mutex mutex_;
  std::condition_variable_any cv_;

 public:
  ~XThreadPool();

  // 禁止生成拷移动构造函数, 拷贝构造函数
  XThreadPool(XThreadPool &&_pool) = delete;
  XThreadPool(const XThreadPool &_pool) = delete;
  XThreadPool &operator=(const XThreadPool &_pool) = delete;

  /**
   * @brief 单件模式
   * @return XThreadPool*
   */
  static XThreadPool *Get() {
    static XThreadPool p;
    return &p;
  }

 private:
  XThreadPool() = default;

 public:
  /**
   * @brief 初始化所有线程，并启动线程
   */
  size_t Init();

  /**
   * @brief 线程池退出
   */
  void Stop();

  /**
   * @brief 插入任务
   * @param _x_task 任务指针
   */
  void AddTask(std::shared_ptr<XTaskBase> &&_x_task);

 private:
  /**
   * @brief 线程池线程的入口函数
   */
  void Run();

  /**
   * @brief 获取任务指针
   * @return XTaskBase* 任务指针
   */
  std::shared_ptr<XTaskBase> GetTask();

  // 获取器和设置器
 public:
  /**
   * 获取线程池运行状态
   * @return bool 线程池运行状态
   */
  bool is_running() const;

  /**
   * 获取线程池当前执行中的任务数量
   * @return int 任务数量
   */
  int task_run_count();
};

#endif //DES_X_THREAD_POOL_X_THREAD_POOL_H_
