#pragma once

#include <mutex>
#include <deque>
#include <condition_variable>
template <class T>
struct LockedQueue {
	// �L���[��x��������
	void enqueue(const T& x) {
		std::unique_lock<std::mutex> lock(m);
		// �O����notify_all()�܂���notify_one()�ɂ���ċN�������܂Ńu���b�N���đ҂�
		// �������A�N�����ꂽ���ɃL���[�����t�������ꍇ�́A�܂��u���b�N���đ҂�
		c_enq.wait(lock, [this] { return mode==0; });
		mode = 1;
		data.push_back(x);
		// dequeue�̏������ł������Ƃ�ʒm
		mode = 0;
		c_deq.notify_one();
	}

	// �L���[����v�f�����o��
	T dequeue() {
		std::unique_lock<std::mutex> lock(m);
		// �O����notify_all()�܂���notify_one()�ɂ���ċN�������܂Ńu���b�N���đ҂�
		// �������A�N�����ꂽ���ɃL���[����ł���ꍇ�́A�܂��u���b�N���đ҂�
		c_deq.wait(lock, [this] { return mode == 0; });
		mode = 1;
		T ret = data.front();
		data.pop_front();
		// enqueue�̏������ł������Ƃ�ʒm
		mode = 0;
		c_enq.notify_one();
		return ret;
	}

	int size(){
		std::unique_lock<std::mutex> lock(m);
		c_enq.wait(lock, [this] { return mode==0; });
		c_deq.wait(lock, [this] { return mode==0; });
		mode = 1;
		auto s = data.size();

		mode = 0;
		c_deq.notify_one();
		c_enq.notify_one();

		return (int)s;
	}

private:
	std::mutex m;
	std::deque<T> data;
	std::condition_variable c_enq;
	std::condition_variable c_deq;
	int mode = 0;
};


class DrawThreadQueue{
public:
	static void enqueue(const std::function<void(void)>& func){
		mQueue.enqueue(func);
	}
	static std::function<void(void)> dequeue(){
		return mQueue.dequeue();
		//return[](){};
	}
	static int size(){
		//return 0;
		return mQueue.size();
	}
private:
	static LockedQueue<std::function<void(void)>> mQueue;
};
