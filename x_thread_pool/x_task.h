//
// Created by Homin Su on 2021/11/2.
//

/**
 * @file
 */

#ifndef DES_X_THREAD_POOL_X_TASK_H_
#define DES_X_THREAD_POOL_X_TASK_H_

#include <functional>
#include <future>

/**
 * @brief 线程池任务基类
 */
class XTaskBase {
 public:
  std::function<bool()> is_running = nullptr; ///< 线程池运行状态函数指针

 public:
  virtual void Main() = 0;
};

/**
 * @brief 线程池任务模版类，ret_type 设定值类型
 * @tparam ret_type 值类型，不允许为 std::thread
 * @example
 * @verbatim
 *  class TestTask : public XTask<std::string> {
 *    public:
 *      std::string name_;
 *
 *    public:
 *      TestTask() = default;
 *      explicit TestTask(std::string _name) : name_(std::move(_name)) {};
 *
 *      // 业务函数
 *      void Main() override {
 *        for (int i = 0; i < 10; ++i) {
 *          if (!is_running()) {
 *            break;
 *          }
 *        printf("%s: %d", name_.c_str(), i);
 *        std::this_thread::sleep_for(std::chrono::milliseconds(500));
 *        set_return(std::forward<std::string>(name_));   // 设置返回值
 *        }
 *      }
 *  };
 * @endverbatim
 * @see XThreadPool
 */
template<class ret_type,
    class = typename std::enable_if
        <!std::is_same<ret_type, std::thread>::value>::type
>
class XTask : public XTaskBase {
 private:
  std::promise<ret_type> p_; ///< 接收返回值

 public:
  /**
   * @brief 设置 future 的值
   * @param ret_type int value
   */
  void set_return(ret_type &&_value) {
    p_.set_value(std::forward<ret_type>(_value));
  }

  void set_return(const ret_type &_value) {
    p_.set_value(_value);
  }

  /**
   * @brief 阻塞等待 set_value
   * @return decltype(auto)
   */
  decltype(auto) get_return() {
    return p_.get_future().get();
  }
};

#endif //DES_X_THREAD_POOL_X_TASK_H_
